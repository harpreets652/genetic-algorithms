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
    void evaluate(vector<Individual*> indiv);
    void evaluateSingle(Individual *indiv);

    string exec(const string& cmd);
    string exec(const char * cmd);
    void clearTempFiles();
    void setWekaLocation(const string& wekaLoc);
    void setDataLocation(const string& dataLoc);
    string getDataLocation() const;

    string getRunCommand(const string& filename);
    double getAccuracyFromOutput(const string &output);
private:
    Evaluator();
    void init();
    string buildQuery(Individual &individual, unsigned int getReal);
    void createFileHeader(ofstream& fout, Individual &individual);
    void createDataPoints(ofstream& fout, result &dataPoint, Individual &individual);
    void generateOutputFile(Individual *indiv);

    static Evaluator* instance;
    string wekaLocation;
    string dataLocation;
};

#endif
