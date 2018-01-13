#ifndef __GA_H_
#define __GA_H_

#include <vector>

#include "config.h"
#include "Evaluator.h"
#include "Population.h"
#include "Individual.h"
#include "Logger.h"

class GA {
public:
    Population parentPop, childPop;
    void init();
    void makeNextGen(bool useParedoToCompare = false);
    void NSGAStep();
    void NSGARun();
    void run();
    void collectStats();
    void printFeaturesOfBestIndividual() const;
    
    vector<double> minAccuracyTimeline, maxAccuracyTimeline, averageAccuracyTimeline;
    vector<unsigned int> minBitCountTimeline, maxBitCountTimeline, averageBitCountTimeline;
    Individual bestIndividualEver;
};

#endif