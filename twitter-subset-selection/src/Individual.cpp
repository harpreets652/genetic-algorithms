#ifndef __INDIVIDUAL_CPP_
#define __INDIVIDUAL_CPP_

#include <utility>
#include <Timer.h>

#include "Individual.h"
#include "config.h"

Config config;

Individual::Individual() : accuracy(0.0), distance(0.0), normalizedProb(0.0), rank(0) {
    resize(config.NUM_FEATURES);
}

void Individual::init(bool allOnes) {
    if (!allOnes)
        generateRandom();
    else {
        for (int i = 0; i < size(); i++) {
            (*this)[i] = true;
        }
    }
}

void Individual::generateRandom() {
    // include all the indices
    for (unsigned int i = 0; i < config.NUM_FEATURES; i++) {
        (*this)[i] = Utils::randomWithProbability(0.5);
    }
}

void Individual::print() const {
    for (unsigned int i = 0; i < this->size(); i++) {
        cout << at(i);
    }
    cout << " - [accuracy: " << accuracy << ", numBits: " << numFeaturesActive << ", time: " << timeTaken << "]" << endl;
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
    for (unsigned int i = 0; i < size(); i++) {
        if (Utils::randomWithProbability(config.PROB_MUTATION)) {
            (*this)[i] = !at(i);
        }
    }
}

bool Individual::paretoDominates(const Individual &opponent) const {
    return !(this->accuracy < opponent.accuracy) && !(this->numFeaturesActive > opponent.numFeaturesActive);
}

bool Individual::operator==(const Individual &other) const {
    for (unsigned int i = 0; size(); i++) {
        if (at(i) != other[i])
            return false;
    }
    return true;
}

void Individual::recount() {
    numFeaturesActive = 0;
    for (auto b : (*this)) {
        numFeaturesActive += b? 1 : 0;
    }
}

#endif
