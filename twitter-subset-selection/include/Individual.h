#ifndef __INDIVIDUAL_H_
#define __INDIVIDUAL_H_

#include <vector>
#include <cstdlib>
#include <iostream>
#include <cmath>

class Evaluator;

#include "Evaluator.h"
#include "Utils.h"

using namespace std;

class Individual : public vector<bool> {
public:
    Individual();

    void generateRandom();
    void print() const;
    void init();
    void evaluate();
    string to_string() const;

    void crossoverWith(Individual& other);
    void mutate();
    bool paretoDominates(const Individual &opponent) const;

    double accuracy;
    unsigned int numFeaturesActive;
    double fitness;
    double timeTaken;
    double normalizedProb;

    double distance;
    unsigned int rank;

    // for fast-non-dominating-sort
    vector<Individual*> peopleIDominate;
    unsigned int numDominateMe = 0;


private:
    //double getBinaryAsNumber(vector<bool> subset) const;
    void recount();
};

#endif
