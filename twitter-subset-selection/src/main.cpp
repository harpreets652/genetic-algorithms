#include <iostream>
#include <array>
#include <argh/argh.h>
#include <Timer.h>

#include "config.h"
#include "Logger.h"
#include "Population.h"
#include "GA.h"

using namespace std;

void getAverageValues(vector<GA> gas, vector<double>& avgMin, vector<double>& avgMax, vector<double>& avgAvg) {
    // iterate
    int vecSize = (int)gas[0].averageTimeline.size();
    for (int i = 0; i < vecSize; i++) {
        double sumMin = 0.0, sumAvg = 0.0, sumMax = 0.0;
        for (int gaIndex = 0; gaIndex < gas.size(); gaIndex++) {
            sumMin += gas[gaIndex].minTimeline[i];
            sumAvg += gas[gaIndex].averageTimeline[i];
            sumMax += gas[gaIndex].maxTimeline[i];
        }
        avgMin.push_back(sumMin/double(gas.size()));
        avgAvg.push_back(sumAvg/double(gas.size()));
        avgMax.push_back(sumMax/double(gas.size()));
    }
}

void runAndReportGA(bool useNSGAII = false) {
    Timer timer;
    timer.start();
    GA ga;
    ga.init();
    if (useNSGAII) {
        ga.NSGARun();
    } else {
        ga.run();
    }
    timer.stop();

    Logger graphLogger(config.getOutputFilename() + ((useNSGAII) ? "_NSGA" : "") + ".tsv");
    cout << "outputting to " << config.getOutputFilename() << endl;
    for (int i = 0; i < ga.minTimeline.size(); i++) {
        string log = to_string(i) + "\t" +
                     to_string(ga.minTimeline[i]) + "\t" +
                     to_string(ga.averageTimeline[i]) + "\t" +
                     to_string(ga.maxTimeline[i]) + "\t";
        graphLogger.log(log);
    }

    cout << "GA took " << timer.getElapsedTime() << " seconds to run total." << endl;
}

int main(int argc, const char *argv[]) {
    srand((unsigned int)time(nullptr));
    if (argc < 1) {
        cout << "You don't have the right inputs, read the README" << endl;
        cout << "You dumb f***" << endl;
        return 0;
    }

    // parse and set the correct cmd line args
    argh::parser cmdl({ "-w", "--wekaloc", "-d", "--data", "-m", "--machine" });
    cmdl.parse(argc, argv);
    Evaluator::getInstance()->setWekaLocation(cmdl("wekaloc").str());
    Evaluator::getInstance()->setDataLocation(cmdl("data").str());
    config.setClassifier(cmdl("machine").str());

    runAndReportGA(false);
    runAndReportGA(true);

    return 0;
}
