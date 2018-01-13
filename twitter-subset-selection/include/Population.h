#ifndef __POPULATION_H_
#define __POPULATION_H_

#include <vector>

#include "config.h"
#include "Individual.h"

using namespace std;

class Population : public vector<Individual> {
public:
    Population();

    void generate(int n = config.POPULATION_SIZE);
    void print() const;
    void evaluate(bool useParedoToCompare = false);
    void evaluateEach();
    void getStatsFromIndividuals(bool useParedoToCompare = false);

    void sortByAccuracy();
    void sortByBitCount();
    void sortByChromosome();

    Individual proportionalSelect();
    Individual tournamentSelect();
    Individual getBestIndividual() const;

    double minAccuracy, maxAccuracy, averageAccuracy;
    unsigned int minBitCount, maxBitCount, averageBitCount;
    unsigned int bestIndividualIndex, worstIndividualIndex;
};

class ParetoFront : public Population {
public:
    void assignCrowdingDistance();
    void sortByCrowdingOperator();
private:
    bool isSorted = false;
};

vector<ParetoFront> sortFastNonDominated(Population& p);

#endif
