//
// Created by Daniel Lopez on 11/8/17.
//

#ifndef MAIN_UTILS_H
#define MAIN_UTILS_H

#include <string>

using namespace std;

class Utils {
public:
    static bool randomWithProbability(double prob);
    static int randIntBetween(int low, int high);
    static string getOutputFilename();
    static void to_upper(string& word);
};


#endif //MAIN_UTILS_H
