#ifndef __EVALUATOR_H_
#define __EVALUATOR_H_

class Individual;

#include <vector>

#include "config.h"
#include "Individual.h"

using namespace std;

struct Point {
    double x, y;
};

class Evaluator {
public:
    static Evaluator* getInstance();
    ~Evaluator();
    void evaluate(Individual &indiv);
    void getBestSolution();
    
    vector<Point> pointsOfReference;
    
    Individual* bestSolution;
private:
    Evaluator();
    void init();

    static Evaluator* instance;

    double getDistanceBetween(unsigned int startIndex, unsigned int endIndex);
};

#endif
