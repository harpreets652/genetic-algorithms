#ifndef __GA_CPP_
#define __GA_CPP_

#include "GA.h"

void GA::init() {
    parentPop.generate(config.POPULATION_SIZE);
    parentPop.evaluate(false);
    collectStats();
}

void GA::makeNextGen(bool useParedoToCompare) {
    childPop.clear();
    for (unsigned int i = 0; i < parentPop.size(); i++) {
        childPop.push_back(
                parentPop.tournamentSelect()
        );

        // if cross over probability, mess with individual i, and individual i _1
        if (i < (parentPop.size() - 1) && Utils::randomWithProbability(config.PROB_CROSSOVER)) {
            childPop[i].crossoverWith(parentPop[Utils::randIntBetween(0, parentPop.size())]);
        }
        // if mutation probability, mess with individual i
        childPop[i].mutate();
    }
    childPop.evaluate(useParedoToCompare);
    childPop.getStatsFromIndividuals(useParedoToCompare);
    if (bestIndividualEver.accuracy < childPop.getBestIndividual().accuracy) {
        bestIndividualEver = childPop.getBestIndividual();
    }
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
    for (int i = 0; i < config.ITERATION_SIZE; i++) {
        parentPop.getStatsFromIndividuals(true);
        parentPop.print();
        collectStats();

        cout << "iteration (NSGA-II): " << i << endl;
        makeNextGen(true);
        NSGAStep();
    }
    printFeaturesOfBestIndividual();
}

void GA::run() {
    cout << "ELITIST GA RUN!!" << endl;
    for (int i = 0; i < config.ITERATION_SIZE; i++) {
        parentPop.getStatsFromIndividuals(false);
        parentPop.sortByChromosome();
        parentPop.print();
        collectStats();

        cout << "iteration (Normal): " << i << endl;
        makeNextGen(false);

        // mix both populations, keep the better half
        childPop.insert(childPop.end(), parentPop.begin(), parentPop.end());
        childPop.sortByAccuracy();
        childPop.erase(childPop.begin(), childPop.begin() + (childPop.size() / 2));

        parentPop = childPop;
    }
    printFeaturesOfBestIndividual();
}

void GA::collectStats() {
    minAccuracyTimeline.push_back(parentPop.minAccuracy);
    maxAccuracyTimeline.push_back(parentPop.maxAccuracy);
    averageAccuracyTimeline.push_back(parentPop.averageAccuracy);

    minBitCountTimeline.push_back(parentPop.minBitCount);
    maxBitCountTimeline.push_back(parentPop.maxBitCount);
    averageBitCountTimeline.push_back(parentPop.averageBitCount);
}

void GA::printFeaturesOfBestIndividual() const {
    cout << "Best Individual: ";
    bestIndividualEver.print();
    // go through and print them out
    for (int i = 0; i < bestIndividualEver.size(); i++) {
        if (bestIndividualEver[i]) {
            if (i < 10)
                cout << "0";
            cout << i << " -- " << config.getFSMap().at(i) << endl;
        }
    }
}

#endif
