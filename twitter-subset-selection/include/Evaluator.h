#ifndef __EVALUATOR_H_
#define __EVALUATOR_H_

class Individual;

#include <vector>
#include <pqxx/pqxx>

#include "config.h"
#include "Individual.h"

using namespace std;
using namespace pqxx;

class Evaluator {
public:
    static Evaluator* getInstance();
    ~Evaluator();
    void evaluate(Individual &indiv);

    string exec(const string& cmd);
    string exec(const char * cmd);
    void setWekaLocation(const string& wekaLoc);
    void setDataLocation(const string& dataLoc);

    string getRunCommand();
private:
    Evaluator();
    void init();
    string buildQuery(Individual &individual);
    string createDataPoints(result &dataPoint, Individual &individual);

    static Evaluator* instance;
    string wekaLocation;
    string dataLocation;

    connection *c;
    work *txn;
};

#endif
