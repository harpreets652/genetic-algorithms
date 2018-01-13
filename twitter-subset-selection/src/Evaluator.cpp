#ifndef __EVALUATOR_CPP_
#define __EVALUATOR_CPP_

#include <fstream>
#include <Timer.h>
#include <thread>

#include "Evaluator.h"

Evaluator* Evaluator::instance = nullptr;

Evaluator* Evaluator::getInstance() {
    if (instance == nullptr) {
        instance = new Evaluator();
    }
    return instance;
}

Evaluator::Evaluator() {
  init();
}

Evaluator::~Evaluator() {
    if (instance != nullptr) {
        delete instance;
    }
}

void Evaluator::init() {

}

void Evaluator::evaluate(vector<Individual*> indiv) {
    unsigned int num_threads = min(8, (int)indiv.size());
    vector<thread> workers(num_threads);

    for (unsigned int i = 0; i < indiv.size(); i++) {
        // freshen up
        if (workers[i%num_threads].joinable()) {
            workers[i%num_threads].join();
        }

        workers[i%num_threads] = thread(&Evaluator::evaluateSingle, Evaluator::getInstance(), indiv[i]);
    }

    for (auto& worker : workers) {
        worker.join();
    }
}

void Evaluator::evaluateSingle(Individual *individual) {
    individual->recount();
    bool isAllZeros = true;
    for (bool b : *individual) {
        if (b) {
            isAllZeros = false;
            break;
        }
    }
    if (isAllZeros) {
        individual->accuracy = 0;
        return;
    }

    generateOutputFile(individual);

    // call the WEKA on this function
    Timer t;
    t.start();
    string output = exec(getRunCommand(individual->to_string()));
    t.stop();

    // get the data from it->
    individual->accuracy = getAccuracyFromOutput(output) * 100;
    individual->timeTaken = t.getElapsedTime();
}

void Evaluator::generateOutputFile(Individual *individual) {
    exec("mkdir -p " + getDataLocation());
    string filename = getDataLocation() + "/" + individual->to_string() + ".arff";
    ofstream fout(filename);
    createFileHeader(fout, *individual);

    // connect to the database
    connection c("dbname=cs_776 user=system password=SYSTEM host=hpcvis3.cse.unr.edu");
    work txn(c);

    // query the database
    vector<int> types = {0};
    switch (config.COMPARISON) {
        case TRADITIONAL_V_GENUINE:
            types.push_back(2);
            break;
        case SOCIAL_V_GENUINE:
            types.push_back(1);
            break;
        case FAKE_V_GENUINE:
            types.push_back(3);
            break;
        case MISC_V_GENUINE:
            types.push_back(1);
            types.push_back(2);
            types.push_back(3);
            break;
        case TRADITIONAL_V_SOCIAL_V_GENUINE:
            types.push_back(1);
            types.push_back(2);
            break;
    }

    for (auto type : types) {
        string query = buildQuery(*individual, type);
        result r = txn.exec(query);
        // if we have nothing, report accuracy of 0
        if (r.empty()) {
            individual->accuracy = 0;
            return;
        }
        // output each of the data points (per user) here
        createDataPoints(fout, r, *individual);
    }
    fout.close();
}

void Evaluator::createFileHeader(ofstream& fout, Individual &individual) {
    fout << "@RELATION twitter" << endl;

    for (unsigned int i = 0; i < individual.size(); i++) {
        if (individual[i]) {

            fout << "@ATTRIBUTE " << config.getFSMap().at(i) << "\t\t";
            if (config.getTypeMap().at(i) == "boolean") {
                fout << "{0,1}" << endl;
            } else {
                fout << config.getTypeMap().at(i) << endl;
            }
        }
    }
    fout << "@ATTRIBUTE class \t\t";
    switch (config.COMPARISON) {
        case TRADITIONAL_V_GENUINE:
            fout << "{REAL,TRADITIONAL}";
            break;
        case SOCIAL_V_GENUINE:
            fout << "{REAL,SOCIAL}";
            break;
        case MISC_V_GENUINE:
        case FAKE_V_GENUINE:
            fout << "{REAL,FAKE}";
            break;
        case TRADITIONAL_V_SOCIAL_V_GENUINE:
            fout << "{REAL,TRADITIONAL,SOCIAL}";
            break;
    }
    fout << endl;

    fout << "@DATA" << endl;
}

void Evaluator::createDataPoints(ofstream& fout, result &dataPoint, Individual &individual) {
    const string REAL = "REAL",
                 FAKE_S = "SOCIAL",
                 FAKE_T = "TRADITIONAL";
    const int REAL_VALUE = 0,
            FAKE_S_VALUE = 1,
            FAKE_T_VALUE = 2;

    // it is assumed that data point will have at least one point
    // and that at least one feature is enabled in the chromosome
    for (auto row : dataPoint) {
        for (unsigned int i = 0; i < config.NUM_FEATURES; i++) {
            if (individual[i]) {
                if (config.getTypeMap().at(i) == "boolean") {
                    bool flag = row[config.getFSMap().at(i)].as<bool>();
                    fout << ((flag) ? "1," : "0,");
                } else {
                    auto value = row[config.getFSMap().at(i)].c_str();
                    fout << value << ",";
                }
            }
        }

        int isGenuine = row["classification"].as<int>();
        switch (isGenuine) {
            case REAL_VALUE:
                fout << REAL;
                break;
            case FAKE_S_VALUE:
                fout << FAKE_S;
                break;
            default:
            case FAKE_T_VALUE:
                fout << FAKE_T;
                break;
        }
        fout << endl;
    }
}

string Evaluator::buildQuery(Individual &indiv, unsigned int desiredClassification) {
    string query, whereClause, limitClause = "limit 1000";

    for (unsigned int i = 0; i < config.NUM_FEATURES; i++) {
        if (indiv[i]) {
            query += config.getFSMap().at(i) + ",";
        }
    }
    if (query[query.size()-1] == ',') {
        query = query.substr(0, query.size()-1);
    }
    query += ",classification";

    whereClause = "where process_error is null and classification = " + to_string(desiredClassification) +  " ";

    return "select " + query + " from tss_dev.users_features "
            + whereClause
            + limitClause;
}

string Evaluator::exec(const char *cmd) {
    return exec(string(cmd));
}

string Evaluator::exec(const string& cmd) {
//    cout << "CMD: " << cmd << endl;
    char buffer[128];
    string result;
    shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);

    if (!pipe)
        return "";
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != nullptr) {
            result += string(buffer);
        }
    }
    return result;
}

void Evaluator::clearTempFiles() {
    exec("rm -f " + getDataLocation() + "/*.arff");
}

void Evaluator::setWekaLocation(const string &wekaLoc) {
    wekaLocation = wekaLoc;
}

void Evaluator::setDataLocation(const string &dataLoc) {
    dataLocation = dataLoc;
    if (dataLocation[dataLocation.size()-1] == '/') {
        dataLocation = dataLocation.substr(0, dataLocation.size()-1);
    }
}

string Evaluator::getRunCommand(const string& filename) {
    char cmd[500];
    snprintf(cmd, 500, "java -Xmx6000m -classpath %s/weka.jar %s -t %s/%s.arff",
            wekaLocation.c_str(), config.getWEKAClassifierName().c_str(), getDataLocation().c_str(), filename.c_str());
    return string(cmd);
}

double Evaluator::getAccuracyFromOutput(const string &output) {
    stringstream ss(output);
    string s;

    double numCorrect = 0.0, numIncorrect = 0.0, total, dummy;
    do {
        getline(ss, s);
    } while (s != "=== Stratified cross-validation ===");
    do {
        getline(ss, s);
    } while (s.substr(0,30) != "Correctly Classified Instances");

    stringstream(s) >> s >> s >> s >> numCorrect >> dummy >> s;
    getline(ss, s);
    stringstream(s) >> s >> s >> s >> numIncorrect >> dummy >> s;
    total = numCorrect + numIncorrect;
//    cout << numCorrect << " and " << numIncorrect << endl;

    return numCorrect/total;
}

string Evaluator::getDataLocation() const {
    return dataLocation + "/" + config.getSimpleComparisonName() + "/" + config.getSimpleWEKAName();
}

#endif
