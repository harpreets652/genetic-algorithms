#include <iostream>
#include <array>
#include <argh/argh.h>
#include <Timer.h>

#include "config.h"
#include "Logger.h"
#include "Population.h"
#include "GA.h"

using namespace std;

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
    for (int i = 0; i < ga.minAccuracyTimeline.size(); i++) {
        string log = to_string(i) + "\t" +
                     to_string(ga.minAccuracyTimeline[i]) + "\t" +
                     to_string(ga.averageAccuracyTimeline[i]) + "\t" +
                     to_string(ga.maxAccuracyTimeline[i]) + "\t" +

                     to_string(ga.minBitCountTimeline[i]) + "\t" +
                     to_string(ga.averageBitCountTimeline[i]) + "\t" +
                     to_string(ga.maxBitCountTimeline[i]) + "\t";
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
