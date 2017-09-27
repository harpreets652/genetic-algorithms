/*
 * random.cpp
 *
 *  Created on: Feb 3, 2016
 *      Author: sushil
 */

#include <stdlib.h>


int flip(float prob) {
    return (random() < prob * RAND_MAX ? 1 : 0);
}

/* greater than equal to low and strictly less than high */
int intInRange(int low, int high) {
    return low + random() % (high - low);
}

/* greater than equal to 0 and less than 1 */
float randomFraction() {
    return ((float) (random() % 1000)) / (float) 1000.0;
}

