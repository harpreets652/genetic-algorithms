#ifndef __CONFIG_H_
#define __CONFIG_H_

#include <cstdlib>
#include <string>
#include <iostream>

using namespace std;

#define BREAKINGPOINT char dummychar; cin >> dummychar;

enum City {
    BURMA = 0,  // burma 14
    BERLIN,     // berlin 52
    ELI_51,     // eli 51
    ELI_76,     // eli 76
    LIN_105,    // lin 105
    LIN_318    // lin 318
};

struct Config {
    Config() {}
    Config(City incomingCity) : city(incomingCity) {
        switch(incomingCity) {
            case BURMA:
                CITY_TOUR_SIZE = 14;
                INPUT_FILENAME = "../in/burma14.tsp";
                SOLUTION_FILENAME = "../in/solution/burma14.opt.tour";
                break;
            case BERLIN:
                CITY_TOUR_SIZE = 52;
                INPUT_FILENAME = "../in/berlin52.tsp";
                SOLUTION_FILENAME = "../in/solution/berlin52.opt.tour";
                break;
            case ELI_51:
                CITY_TOUR_SIZE = 51;
                INPUT_FILENAME = "../in/eil51.tsp";
                SOLUTION_FILENAME = "../in/solution/eil51.opt.tour";
                break;
            case ELI_76:
                CITY_TOUR_SIZE = 76;
                INPUT_FILENAME = "../in/eil76.tsp";
                SOLUTION_FILENAME = "../in/solution/eil76.opt.tour";
                break;
            case LIN_105:
                CITY_TOUR_SIZE = 105;
                INPUT_FILENAME = "../in/lin105.tsp";
                SOLUTION_FILENAME = "../in/solution/lin105.opt.tour";
                break;
            case LIN_318:
                CITY_TOUR_SIZE = 318;
                INPUT_FILENAME = "../in/lin318.tsp";
                SOLUTION_FILENAME = "../in/solution/lin318.opt.tour";
                break;
            default:
                break;
        }
    }
    Config& operator=(const Config& other) {
        if (this == &other) {
            return *this;
        }
        this->PROB_MUTATION = other.PROB_MUTATION;
        this->PROB_CROSSOVER = other.PROB_CROSSOVER;
        this->CITY_TOUR_SIZE = other.CITY_TOUR_SIZE;
        this->INPUT_FILENAME = other.INPUT_FILENAME;
        this->SOLUTION_FILENAME = other.SOLUTION_FILENAME;
        this->city = other.city;
        return *this;
    }
    double PROB_MUTATION = 0.001; // 0.01, 0.001, 0.0001
    double PROB_CROSSOVER = 0.20;  // 0.2, 0.67, 0.99
    string INPUT_FILENAME;
    string SOLUTION_FILENAME;
    unsigned int CITY_TOUR_SIZE = 14;
    unsigned int POPULATION_SIZE = 1000;
    unsigned int ITERATION_SIZE = 50000;
    unsigned int TOTAL_GAS_SIZE = 30; // 5 for testing, 30 for reality
    City city;
};

const Config config_1(BURMA);
const Config config_2(BERLIN);
const Config config_3(ELI_51);
const Config config_4(ELI_76);
const Config config_5(LIN_105);
const Config config_6(LIN_318);
const Config config_test(BURMA);

extern Config config;

static string to_string(City city) {
    switch(city) {
        case BURMA:
            return string("Burma");
        case BERLIN:
            return string("Berlin");
        case ELI_51:
            return string("Eli_51");
        case ELI_76:
            return string("Eli_76");
        case LIN_105:
            return string("Lin_105");
        case LIN_318:
            return string("Lin_138");
        default:
            return string ("_");
    }
}

static bool randomWithProbability(double prob) {
    // assuming prob = (0,1]...
    double shot = double(rand())/double(RAND_MAX);
    return (shot <= prob);
}

static int randIntBetween(int low, int high) {
    return (rand() % (high - low)) + low;
}

static string getOutputFilename() {
    string result = to_string(config.city) + "_" + to_string(config.PROB_MUTATION) + "_" + to_string(config.PROB_CROSSOVER);
    return result;
}

// template <typename T>
// static void swap(T& left, T& right) {
//     if (&left != &right) {
//         T temp = left;
//         right = left;
//         left = temp;
//     }
// }

#endif
