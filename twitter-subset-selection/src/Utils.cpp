//
// Created by Daniel Lopez on 11/8/17.
//

#include "Utils.h"

bool Utils::randomWithProbability(double prob) {
    double shot = double(rand())/double(RAND_MAX);
    return (shot <= prob);
}

int Utils::randIntBetween(int low, int high) {
    return (rand() % (high - low)) + low;
}

string Utils::getOutputFilename() {
    string result = "some_output.log";
    return result;
}

void Utils::to_upper(string &word) {
    for (auto &c : word) {
        c = (char) toupper(c);
    }
}