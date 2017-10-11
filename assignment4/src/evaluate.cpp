/*
 * evaluate.cpp
 *
 *  Created on: Feb 3, 2016
 *      Author: sushil
 */

#include <random>
#include "evaluate.h"


using namespace std;

int getTotalRouteDistance(const int *chrom, int chromosomeLength) {
    int totalDistance = 0;
    for (int i = 0; i < chromosomeLength - 1; i++) {
        if (i == chromosomeLength - 1) {
            TSPManager::getInstance()->getEuclideanDistance(chrom[i], chrom[0]);
        } else {
            totalDistance += TSPManager::getInstance()->getEuclideanDistance(chrom[i], chrom[i + 1]);
        }
    }

    return totalDistance;
}

void tspEval(ga::Individual *ent) {
    int objectiveValue = getTotalRouteDistance(ent->chrom, ent->length);

    ent->fit = 1.0f / objectiveValue;
}
