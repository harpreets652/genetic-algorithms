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
    void sortByFitness();


    Individual proportionalSelect();
    Individual tournamentSelect();
    Individual getBestIndividual() const;

    double minFitness, maxFitness, averageFitness;
    unsigned int bestIndividualIndex, worstIndividualIndex;

private:
    double sumFitness;
    void sortByFitness(unsigned int left, unsigned int right);
};

#endif
