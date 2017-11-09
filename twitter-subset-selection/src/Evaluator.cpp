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

}

void Evaluator::evaluate(Individual &indiv) {
    // for right now, all individuals get 100
    indiv.fitness = 100;
    indiv.distance = 0;
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
