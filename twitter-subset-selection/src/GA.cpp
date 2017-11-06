#ifndef __GA_CPP_
#define __GA_CPP_

#include "GA.h"
#include "config.h"

void GA::init() {
    cout << "init" << endl;
    parentPop.generate(config.POPULATION_SIZE);
    cout << "init" << endl;
    parentPop.evaluate();
    parentPop.print();
}

void GA::makeNextGen() {
    childPop.clear();
    for(unsigned int i = 0; i < parentPop.size(); i++) {
        childPop.push_back(
            parentPop.tournamentSelect()
        );

        // if cross over probability, mess with individual i, and individual i _1
        if (i < (parentPop.size() - 1) && randomWithProbability(config.PROB_CROSSOVER)) {
            childPop[i].crossoverWith(parentPop[randIntBetween(i+1, parentPop.size())]);
        }
        // it mutation probabiltiy, mess with individual i
        if (randomWithProbability(config.PROB_MUTATION)) {
            childPop[i].mutate();
        }
    }
    childPop.evaluate();
    minTimeline.push_back(childPop.minFitness);
    maxTimeline.push_back(childPop.maxFitness);
    averageTimeline.push_back(childPop.averageFitness);
    if (bestIndividualEver.fitness < childPop.getBestIndividual().fitness) {
        bestIndividualEver = childPop.getBestIndividual();
    }

    // compile both populations, keep the second half
    childPop.insert(childPop.end(), parentPop.begin(), parentPop.end());
    childPop.sortByFitness();
    childPop.erase(childPop.begin(), childPop.begin() + (childPop.size()/2));
}

void GA::run() {
    for (int i = 0; i < config.ITERATION_SIZE; i++) {
        cout << "iteration: " << i << endl;
        makeNextGen();
        parentPop = childPop;
    }
    for (int i = 0; i < averageTimeline.size(); i++) {
        string log = to_string(minTimeline[i]) + "," +
                    to_string(averageTimeline[i]) + "," +
                    to_string(maxTimeline[i]) + ",";
    }
    cout << "my best: ";
    bestIndividualEver.print();
    cout << "actual : ";
    Evaluator::getInstance()->bestSolution->print();
    cout << endl;
}

#endif
