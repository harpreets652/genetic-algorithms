//
// Created by Harpreet Singh on 9/8/17.
//

#include "Eval.h"

//Min return value will be 0 and max 100
double eval(int *candidate) {
    int output = 0;
    int index = 0;

    //all ones from index 0 - 19
    for (index = 0; index < 20; index++) {
        if (candidate[index] > 0) {
            output ++;
        }
    }

    //all zeroes from index 20 - 29
    for (index = 20; index < 30; index++) {
        if (candidate[index] < 1) {
            output ++;
        }
    }

    //all ones from index 30 - 59
    for (index = 30; index < 60; index++) {
        if (candidate[index] > 0) {
            output ++;
        }
    }

    //all zeroes from index 60 - 82
    for (index = 60; index < 83; index++) {
        if (candidate[index] < 1) {
            output ++;
        }
    }

    //all ones from index 83 - 99
    for (index = 83; index < 100; index++) {
        if (candidate[index] > 0) {
            output ++;
        }
    }

    return output;
}

