#ifndef __EVALUATOR_CPP_
#define __EVALUATOR_CPP_

#include <fstream>

#include "Evaluator.h"
#include "config.h"

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
    c = new connection("dbname=cs_776 user=system");
    txn = new work(*c);
}

void Evaluator::evaluate(Individual &individual) {
    // TODO: fix the boolean options
    individual[USER_HAS_DESCRIPTION] = false;
//    individual[USER_HAS_URL] = false;
    individual[USER_DEF_PROFILE_PHOTO] = false;
    individual[USER_IS_VERIFIED] = false;

    // query the database
    string query = buildQuery(individual);
    result r = txn->exec(query);

    // if we have nothing, report fitness of 0
    if (r.empty()) {
        individual.fitness = 0;
        individual.distance = 0;
        return;
    }

    // build the output file
    ofstream fout("../weka_temp/" + individual.to_string() + ".arff");
    createFileHeader(fout, individual);

    // output each of the data points (per user) here
    createDataPoints(fout, r, individual);

    fout.close();

    // call the WEKA on this function
    string output = exec(getRunCommand(individual.to_string()));

    // get the data from it
    individual.fitness = getFitnessFromOutput(output) * 1000;

    individual.print();
}

void Evaluator::createFileHeader(ofstream& fout, Individual &individual) {
    fout << "@RELATION twitter" << endl;

    for (unsigned int i = 0; i < 41; i++) {
        if (individual[i]) {

            fout << "@ATTRIBUTE " << config.getFSMap().at(i) << "\t\t";
            if (config.getTypeMap().at(i) == "boolean") {
                fout << "{0,1}" << endl;
            } else {
                fout << config.getTypeMap().at(i) << endl;
            }
        }
    }
    fout << "@ATTRIBUTE class \t\t{REAL,TRADITIONAL,SOCIAL}" << endl;
}

void Evaluator::createDataPoints(ofstream& fout, result &dataPoint, Individual &individual) {
    const string REAL = "REAL",
                 FAKE_T = "TRADITIONAL",
                 FAKE_S = "SOCIAL";
    const int REAL_VALUE = 0,
              FAKE_T_VALUE = 1,
              FAKE_S_VALUE = 2;

    fout << "@DATA" << endl;
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
        if (isGenuine == REAL_VALUE)
            fout << REAL;
        else if (isGenuine == FAKE_T_VALUE)
            fout << FAKE_T;
        else if (isGenuine == FAKE_S_VALUE)
            fout << FAKE_S;

        fout << endl;
    }
}

string Evaluator::buildQuery(Individual &indiv) {
    string query;

    for (unsigned int i = 0; i < config.NUM_FEATURES; i++) {
        if (indiv[i]) {
            query += config.getFSMap().at(i) + ",";
        }
    }
    if (query[query.size()-1] == ',') {
        query = query.substr(0, query.size()-1);
    }
    query += ",classification";

    return "select " + query + " from tss_dev.users_features;";
}

string Evaluator::exec(const char *cmd) {
    return exec(string(cmd));
}

string Evaluator::exec(const string& cmd) {
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

void Evaluator::setWekaLocation(const string &wekaLoc) {
    this->wekaLocation = wekaLoc;
}

void Evaluator::setDataLocation(const string &dataLoc) {
    this->dataLocation = dataLoc;
}

string Evaluator::getRunCommand(const string& filename) {
    char cmd[200];
    snprintf(cmd, 200, "java -classpath %s/weka.jar %s -t %s/%s.arff",
            wekaLocation.c_str(), config.getWEKAClassifierName().c_str(), dataLocation.c_str(), filename.c_str());
    return string(cmd);
}

double Evaluator::getFitnessFromOutput(const string &output) {
    stringstream ss(output);
    string s;

    double numCorrect = 0.0, numIncorrect = 0.0, total, dummy;
    do {
        getline(ss, s);
    } while (s.substr(0,30) != "Correctly Classified Instances");

    stringstream(s) >> s >> s >> s >> numCorrect >> dummy;
    getline(ss, s);
    stringstream(s) >> s >> s >> s >> numIncorrect >> dummy;
    total = numCorrect + numIncorrect;

    return numCorrect/total;
}

#endif
