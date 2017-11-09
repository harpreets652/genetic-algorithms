#ifndef __CONFIG_H_
#define __CONFIG_H_

#include <cstdlib>
#include <string>
#include <iostream>

using namespace std;

#define BREAKINGPOINT char dummychar; cin >> dummychar;

struct Config {
    Config() = default;

    Config& operator=(const Config& other);

    double PROB_MUTATION = 0.001; // 0.01, 0.001, 0.0001
    double PROB_CROSSOVER = 0.20;  // 0.2, 0.67, 0.99
    string INPUT_FILENAME;

    unsigned int NUM_FEATURES = 30;
    unsigned int POPULATION_SIZE = 1000;
    unsigned int ITERATION_SIZE = 50000;
    unsigned int TOTAL_GAS_SIZE = 30; // 5 for testing, 30 for reality
};

const Config config_1;
const Config config_test;

extern Config config;

#endif
