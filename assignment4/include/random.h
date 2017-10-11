/*
 * random.h
 *
 *  Created on: Feb 3, 2016
 *      Author: sushil
 */

#ifndef RANDOM_H_
#define RANDOM_H_

#include <vector>

int flip(float prob);

int intInRange(int low, int high);

float randomFraction();

void shuffleVector(std::vector<int> &vec);

#endif /* RANDOM_H_ */
