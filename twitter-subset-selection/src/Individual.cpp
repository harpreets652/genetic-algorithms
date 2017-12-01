#ifndef __INDIVIDUAL_CPP_
#define __INDIVIDUAL_CPP_

#include <utility>
#include <Timer.h>

#include "Individual.h"
#include "config.h"

Config config;

Individual::Individual() : vector<bool>(config.NUM_FEATURES), accuracy(0.0), distance(0.0), normalizedProb(0.0), rank(0) {

}

void Individual::init() {
    generateRandom();
}

void Individual::generateRandom() {
    // include all the indices
    for (unsigned int i = 0; i < config.NUM_FEATURES; i++) {
        (*this)[i] = Utils::randomWithProbability(0.05);
    }
}

void Individual::print() const {
    for (unsigned int i = 0; i < this->size(); i++) {
        cout << at(i);
    }
    cout << " - [accuracy: " << accuracy << ", numBits: " << numFeaturesActive << ", time: " << timeTaken << "]" << endl;
}

void Individual::evaluate() {
    recount();
    Evaluator::getInstance()->evaluate((*this));
}

string Individual::to_string() const {
    string result(config.NUM_FEATURES, ' ');
    for (unsigned int i = 0; i < size(); i++) {
        result[i] = at(i) ? '1' : '0';
    }
    return result;
}

void Individual::crossoverWith(Individual& other) {
    // pick random index
    int index = Utils::randIntBetween(0, (int)size());
    // get the two subsets
    vector<bool> lSubset(begin() + index, end()), rSubset(other.begin() + index, other.end());
    // assign them back
    for (int i = index; i < size(); i++) {
        (*this)[i] = rSubset[i - index];
        other[i] = lSubset[i - index];
    }
}

void Individual::mutate() {
    // flip at a random index
    int index = Utils::randIntBetween(0, (int)size());
    (*this)[index] = !at((unsigned int)index);
}

bool Individual::paredoDominates(const Individual &opponent) const {
    return (this->accuracy > opponent.accuracy) && (this->numFeaturesActive < opponent.numFeaturesActive);
}

void Individual::recount() {
    numFeaturesActive = 0;
    for (auto b : (*this)) {
        numFeaturesActive += b? 1 : 0;
    }
}

#endif
