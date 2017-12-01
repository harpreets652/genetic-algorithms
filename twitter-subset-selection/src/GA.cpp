#ifndef __GA_CPP_
#define __GA_CPP_

#include "GA.h"
#include "config.h"

void GA::init() {
    parentPop.generate(config.POPULATION_SIZE);
    parentPop.evaluate();
}

void GA::makeNextGen() {
    childPop.clear();
    for (unsigned int i = 0; i < parentPop.size(); i++) {
        childPop.push_back(
                parentPop.tournamentSelect()
        );

        // if cross over probability, mess with individual i, and individual i _1
        if (i < (parentPop.size() - 1) && Utils::randomWithProbability(config.PROB_CROSSOVER)) {
            childPop[i].crossoverWith(parentPop[Utils::randIntBetween(0, parentPop.size())]);
        }
        // if mutation probabiltiy, mess with individual i
        if (Utils::randomWithProbability(config.PROB_MUTATION)) {
            childPop[i].mutate();
        }
    }
    childPop.evaluate();
    minTimeline.push_back(childPop.minFitness);
    maxTimeline.push_back(childPop.maxFitness);
    averageTimeline.push_back(childPop.averageFitness);
    if (bestIndividualEver.accuracy < childPop.getBestIndividual().accuracy) {
        bestIndividualEver = childPop.getBestIndividual();
    }

    // compile both populations, keep the second half

//    childPop.insert(childPop.end(), parentPop.begin(), parentPop.end());
//    childPop.sortByAccuracy();
//    childPop.erase(childPop.begin(), childPop.begin() + (childPop.size() / 2));
}

void GA::NSGAStep() {
    unsigned long N = parentPop.size();
    childPop.insert(childPop.end(), parentPop.begin(), parentPop.end());
    vector<ParetoFront> f = sortFastNonDominated(childPop);
    parentPop.clear();
    int index = 0;

    // until the parent population is filled
    while (index < f.size() && parentPop.size() + f[index].size() <= N) {
        // calculate the crowding distance
        f[index].assignCrowdingDistance();
        // include the i-th nondominated front to the parent pop
        parentPop.insert(parentPop.end(), f[index].begin(), f[index].end());
        index++;
    }
    if (parentPop.size() < N) {
        f[index].sortByCrowdingOperator();
        parentPop.insert(parentPop.end(), f[index].begin(), f[index].begin() + (N - parentPop.size()));
    } else if (parentPop.size() > N) {
        parentPop.erase(parentPop.begin() + N, parentPop.end());
    }
}

void GA::NSGARun() {
    cout << "NSGA RUN!!" << endl;
    makeNextGen();
    for (int i = 0; i < config.ITERATION_SIZE; i++) {
        minTimeline.push_back(parentPop.minFitness);
        maxTimeline.push_back(parentPop.maxFitness);
        averageTimeline.push_back(parentPop.averageFitness);

        cout << "iteration (NSGA-2): " << i << endl;
        NSGAStep();
        makeNextGen();
    }
    cout << "my best: ";
    bestIndividualEver.print();
}

void GA::run() {
    for (int i = 0; i < config.ITERATION_SIZE; i++) {
        minTimeline.push_back(parentPop.minFitness);
        maxTimeline.push_back(parentPop.maxFitness);
        averageTimeline.push_back(parentPop.averageFitness);

        cout << "iteration (Normal): " << i << endl;
        makeNextGen();
        parentPop = childPop;
    }

    cout << "my best: ";
    bestIndividualEver.print();
}

#endif
