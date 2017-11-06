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
    void makeNextGen();
    void run();
    
    vector<double> minTimeline, maxTimeline, averageTimeline;
    Individual bestIndividualEver;
};

#endif