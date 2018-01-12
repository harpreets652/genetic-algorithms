#ifndef __INDIVIDUAL_H_
#define __INDIVIDUAL_H_

#include <vector>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <thread>

class Evaluator;

#include "Evaluator.h"
#include "Utils.h"

using namespace std;

class Individual : public vector<bool> {
public:
    Individual();

    void generateRandom();
    void print() const;
    void init(bool allOnes = false);
    string to_string() const;

    void crossoverWith(Individual& other);
    void mutate();
    bool paretoDominates(const Individual &opponent) const;
    bool operator==(const Individual& other) const;

    double accuracy;
    unsigned int numFeaturesActive;
    double timeTaken;
    double normalizedProb;

    double distance;
    unsigned int rank;

    // for fast-non-dominating-sort
    vector<Individual*> peopleIDominate;
    unsigned int numDominateMe = 0;


    void recount();
private:
//    double getBinaryAsNumber(vector<bool> subset) const;
};

#endif
