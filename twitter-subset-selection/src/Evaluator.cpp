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
    // for right now, all individuals get 100
    individual.fitness = 100;
    individual.distance = 0;

    // query the database
    string query = buildQuery(individual);
    cout << "query: " << query << endl;
    cout << "indiv as string: " << individual.to_string() << endl;
    result r = txn->exec(query);

    // if we have nothing, report fitness of 0
    if (r.empty()) {
        individual.fitness = 0;
        individual.distance = 0;
        return;
    }

    // build the output file
    ofstream fout("../weka_temp/" + individual.to_string() + ".arff");
    fout << createFileHeader(individual) << endl;

    // output each of the data points (per user) here
    fout << createDataPoints(r, individual) << endl;

    fout.close();

    // call the WEKA on this function
    string output = exec(getRunCommand(individual.to_string()));

    // get the data from it
    individual.fitness = getFitnessFromOutput(output);
}

string Evaluator::createFileHeader(Individual &individual) {
    string result = "@RELATION twitter\n";

    for (unsigned int i = 0; i < 41; i++) {
        if (individual[i]) {
            result += "@ATTRIBUTE " + config.getFSMap().at(i) + "\t\t" + config.getTypeMap().at(i) + "\n";
        }
    }
    result += "@ATTRIBUTE class \t\t{REAL,TRADITIONAL,SOCIAL}\n";
    return result;
}

string Evaluator::createDataPoints(result &dataPoint, Individual &individual) {
    string result = "@DATA\n";
    const string REAL = "REAL\n",
                 FAKE_T = "TRADITIONAL\n",
                 FAKE_S = "SOCIAL\n";
    const int REAL_VALUE = 0,
              FAKE_T_VALUE = 1,
              FAKE_S_VALUE = 2;
    // it is assumed that data point will have at least one point
    // and that at least one feature is enabled in the chromosome
    for (auto row : dataPoint) {
//        for (unsigned int i = 0; i < config.NUM_FEATURES; i++) {
//            if (individual[i]) {
//                auto value;
//                value = row[Individual::getFSMap().at(i)].as<float>();
//                result += to_string(value) + ",";
//            }
//        }
        if (individual[AVG_LEN_TWEET_CHARACTERS]) {
            auto avg = row[config.getFSMap().at(AVG_LEN_TWEET_CHARACTERS)].as<float>();
            result += to_string(avg) + ",";
        }
//        if (individual[AVG_LEN_TWEET_WORDS]) {
//            float avg = row["avg_length_words"].as<float>();
//            result += to_string(avg) + ",";
//        }
//        if (individual[AVG_NUM_POSITIVE_WORDS]) {
//            float avg = row["avg_sentiment_pos_words"].as<float>();
//            result += to_string(avg) + ",";
//        }
//        if (individual[AVG_NUM_NEGATIVE_WORDS]) {
//            float avg = row["avg_sentiment_neg_words"].as<float>();
//            result += to_string(avg) + ",";
//        }
//        if (individual[AVG_SENTIMENT_SCORE]) {
//            float avg = row["avg_sentiment_score"].as<float>();
//            result += to_string(avg) + ",";
//        }
//        if (individual[USER_STATUS_COUNT]) {
//            int avg = row["user_status_count"].as<int>();
//            result += to_string(avg) + ",";
//        }
        int isGenuine = row["classification"].as<int>();
        if (isGenuine == REAL_VALUE)
            result += REAL;
        else if (isGenuine == FAKE_T_VALUE)
            result += FAKE_T;
        else
            result += FAKE_S;
    }
    return result;
}

string Evaluator::buildQuery(Individual &indiv) {
    indiv.print();
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
    snprintf(cmd, 200, "java -classpath %s/weka.jar weka.classifiers.rules.ZeroR -t %s/%s.arff",
            wekaLocation.c_str(), dataLocation.c_str(), filename.c_str());
    return string(cmd);
}

double Evaluator::getFitnessFromOutput(const string &output) {
    stringstream ss(output);
    string s;

    double numCorrect = 0.0, numIncorrect = 0.0, total, dummy;
    do {
        getline(ss, s);
    } while (s.substr(0,30) != "Correctly Classified Instances");

    cout << s << endl;

    stringstream(s) >> s >> s >> s >> numCorrect >> dummy;
    getline(ss, s);
    stringstream(s) >> s >> s >> s >> numIncorrect >> dummy;
    total = numCorrect + numIncorrect;

    return numCorrect/total;
}

#endif
