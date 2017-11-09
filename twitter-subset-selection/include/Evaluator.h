#ifndef __EVALUATOR_H_
#define __EVALUATOR_H_

class Individual;

#include <vector>

#include "config.h"
#include "Individual.h"

using namespace std;

class Evaluator {
public:
    static Evaluator* getInstance();
    ~Evaluator();
    void evaluate(Individual &indiv);

    string exec(const string& cmd);
private:
    Evaluator();
    void init();

    static Evaluator* instance;
    const string wekaConstructionString = "";
};

#endif
