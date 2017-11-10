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
    individual[USER_STATUS_COUNT] = true;
    // for right now, all individuals get 100
    individual.fitness = 100;
    individual.distance = 0;

    // query the database
    string query = buildQuery(individual);
    cout << "query: " << query << endl;
    cout << "indiv as string: " << individual.to_string() << endl;
    result r = txn->exec(query);

    // if we have nothing, report fitness of 0
//    if (r.empty()) {
//        individual.fitness = 0;
//        individual.distance = 0;
//        return;
//    }
    // int employee_id = r[0][0].as<int>();
    // build the output file
    ofstream fout("../weka_temp/" + individual.to_string() + ".txt");
    fout << createFileHeader(individual);
    // TODO ...

    // output each of the data points (per user) here
    fout << createDataPoints(r, individual) << endl;

    fout.close();

    // call the WEKA on this function

    // get the data from it

}

string Evaluator::createFileHeader(Individual &individual) {
    string result = "@RELATION twitter";
    if (individual[AVG_LEN_TWEET_CHARACTERS]) {
        result += "@ATTRIBUTE AVG_LEN_TWEET_CHARACTERS \t\tREAL\n";
    }
    if (individual[AVG_LEN_TWEET_WORDS]) {
        result += "@ATTRIBUTE AVG_LEN_TWEET_WORDS\t\tREAL\n";
    }
    if (individual[AVG_NUM_POSITIVE_WORDS]) {
        result += "@ATTRIBUTE AVG_NUM_POSITIVE_WORDS\t\tREAL\n";
    }
    return result;
}

string Evaluator::createDataPoints(result &dataPoint, Individual &individual) {
    string result = "@DATA\n";
    string REAL = "REAL\n", FAKE = "FAKE\n";

    // it is assumed that data point will have at least one point
    // and that at least one feature is enabled in the chromosome
    for (auto row : dataPoint) {
        if (individual[USER_STATUS_COUNT]) {
            int count = row["statuses_count"].as<int>();
            result += to_string(count) + ",";
        }
        bool isGenuine = row["is_user_genuine"].as<bool>();
        result += (isGenuine ? REAL : FAKE);
    }
    return result;
}

string Evaluator::buildQuery(Individual &indiv) {
    indiv.print();
    string query;

    if (indiv[AVG_LEN_TWEET_CHARACTERS]) {
        query += "length_char,";
    }
    if (indiv[AVG_LEN_TWEET_WORDS]) {
        query += "length_words,";
    }
    if (indiv[AVG_NUM_POSITIVE_WORDS]) {
        query += "sentiment_pos_words,";
    }
    if (indiv[AVG_NUM_NEGATIVE_WORDS]) {
        query += "sentiment_neg_words,";
    }
    if (indiv[AVG_SENTIMENT_SCORE]) {
        query += "sentiment_score,";
    }
    if (indiv[FRAC_CONTAINS_QUESTION]) {
        query += "contains_question,";
    }
    if (indiv[USER_REGISTRATION_AGE]) {
        query += "user_registration_age,";
    }
    if (indiv[USER_STATUS_COUNT]) {
        query += "statuses_count,";
    }
    //...
    if (query[query.size()-1] == ',') {
        query = query.substr(0, query.size()-1);
    }
    query += ",is_user_genuine";

    return "select " + query + " from tss_dev.users;";
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

string Evaluator::getRunCommand() {
    char cmd[200];
    snprintf(cmd, 200, "java -classpath %s/weka.jar weka.classifiers.rules.ZeroR -t %s/data/iris.arff",
            wekaLocation.c_str(), dataLocation.c_str());
    return string(cmd);
}
#endif
