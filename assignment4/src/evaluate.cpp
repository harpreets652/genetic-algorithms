/*
 * evaluate.cpp
 *
 *  Created on: Feb 3, 2016
 *      Author: sushil
 */

#include <random>
#include "evaluate.h"


using namespace std;

float getTotalRouteDistance(const int *chrom, int chromosomeLength) {
    float totalDistance = 0;
    for (int i = 0; i < chromosomeLength; i++) {
        if (i == chromosomeLength - 1) {
            totalDistance += TSPManager::getInstance()->getEuclideanDistance(chrom[i], chrom[0]);
        } else {
            totalDistance += TSPManager::getInstance()->getEuclideanDistance(chrom[i], chrom[i + 1]);
        }
    }

    return totalDistance;
}

void tspEval(ga::Individual *ent) {
    float objectiveValue = (getTotalRouteDistance(ent->chrom, ent->length));

    ent->fit = 1.0f / objectiveValue;
}
