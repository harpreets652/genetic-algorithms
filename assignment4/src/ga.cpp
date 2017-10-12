//============================================================================
// Name        : ga.cpp
// Author      : Sushil J Louis
// Version     :
// Copyright   : Copyright University of Nevada, Reno
// Description : GA in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <population.h>
#include <ga.h>
#include "utils.h"

using namespace std;
using namespace ga;

int main(int argc, char *argv[]) {
    GA ga = GA(argc, argv);

    ga.init();
    ga.run();
    ga.report();

    return 0;
}

GA::GA(int argc, char *argv[]) {
    setupOptions(argc, argv);
    srandom(options.randomSeed);
    ofstream ofs(options.outfile, std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    ofstream pofs(options.phenotypeFile, std::ofstream::out | std::ofstream::trunc);
    pofs.close();
    maxFitGen = 0;
    this->bestIndividualSoFar = new Individual(options.chromLength);
    bestFitnessSoFar = -1;
    TSPManager::getInstance()->preCalculateDistances();
}

void GA::init() {
    parent = new Population(options);
    child = new Population(options);
    parent->initialize(); // evaluates, stats, and reports on initial population
    updateProgress(0, parent);
//	cout << "initialized" << endl;
}

void GA::run() {
    Population *tmp;
    for (unsigned int i = 1; i < options.maxgens; i++) {
        parent->muPlusLambdaSelection(child);
        child->statistics();
        child->report(i);
        updateProgress(i, child);

        tmp = parent;
        parent = child;
        child = tmp;
    }
}

/**
 * Update and save the best ever individual
 */
void GA::updateProgress(unsigned int gen, Population *p) {

    if (p->max > bestFitnessSoFar) {
        bestFitnessSoFar = p->max;
        maxFitGen = gen;
        bestIndividualSoFar->copy(p->pop[p->maxi]);

        char printbuf[2056];
        std::string chromStr = chromToString(bestIndividualSoFar->chrom, bestIndividualSoFar->length);
        sprintf(printbuf, "%4i \t %0.8f \t %s\n", maxFitGen, bestFitnessSoFar, chromStr.c_str());
        writeBufToFile(printbuf, options.phenotypeFile);
    }
}


void GA::report() {
    //parent->report(options.maxgens);
    cout << *(parent->pop[parent->maxi]) << endl;
}

void GA::configure() {
    ifstream ifs(options.infile);
    if (ifs.good()) {
        ifs >> options.popSize;
        ifs >> options.chromLength;
        ifs >> options.maxgens;
        ifs >> options.px;
        ifs >> options.pm;
        ifs >> options.scaler;
        ifs >> options.lambda;
    }
    ifs.close();
}

void GA::setupOptions(int argc, char *argv[]) {
    options.infile = string("infile");
    options.outfile = string("outfile_189");// append randomseed to output file names

    //or provide as an argument to program
    options.randomSeed = 189;
    //candidate solution length
    options.chromLength = 10;

    //NOTE~ CONFIGURE THESE PARAMETERS***********
    //population size
    options.popSize = 800;
    //maximum number of generations
    options.maxgens = 100;//static_cast<unsigned int>(1.5 * options.popSize);

    //lambda*popSize has to be an even number
    options.lambda = 1;

    options.px = 0.99f; //probability of crossover
    options.pm = 0.1f; //probability of mutation
    //Note~ CONFIGURE END**************************

    options.scaler = 1.05;
    options.nCriteria = 1;
    options.mutator = Mutator::Flip;
    options.xover = Xover::UX;
    options.selector = Selector::Proportionate;

    if (argc == 4) {
        std::string problemFileName = string(argv[1]);
        options.randomSeed = atoi(argv[2]);
        options.outfile = string(string(argv[3]) + "_" + std::to_string(options.randomSeed) + ".data");

        TSPManager::getInstance()->loadTSPProblem(problemFileName);
        options.chromLength = TSPManager::getInstance()->getNumberOfNodes();
    }

    //derived values go after configure() above
    options.phenotypeFile = string(options.outfile + ".pheno"); //derived from options.outfile
    //options.maxgens = options.popSize * 1.5;
}


