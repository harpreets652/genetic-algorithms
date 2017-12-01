#ifndef __POPULATION_H_
#define __POPULATION_H_

#include <vector>

#include "config.h"
#include "Individual.h"

using namespace std;

class Population : public vector<Individual> {
public:
    Population();

    void generate(int n = 100);
    void print() const;
    void evaluate();

    void sortByAccuracy();
    void sortByBitCount();

    Individual proportionalSelect();
    Individual tournamentSelect();
    Individual getBestIndividual() const;

    double minFitness, maxFitness, averageFitness;
    unsigned int bestIndividualIndex, worstIndividualIndex;

private:
    double sumFitness;
};

class ParedoFront : public Population {
public:
    void assignCrowdingDistance();
    void sortByCrowdingOperator();
private:
    bool isSorted = false;
};

vector<ParedoFront> sortFastNonDominated(Population& p);

#endif
