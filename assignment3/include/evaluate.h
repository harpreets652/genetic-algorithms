/*
 * evaluate.h
 *
 *  Created on: Mar 15, 2016
 *      Author: sushil
 */

#ifndef EVALUATE_H_
#define EVALUATE_H_

#include <individual.h>
#include <cmath>
#include <cstring>

void evaluate(ga::Individual *ent);

void xSquared(ga::Individual *ent);

void deJongFirstFunction(ga::Individual *ent);

void deJongSecondFunction(ga::Individual *ent);

void deJongThirdFunction(ga::Individual *ent);

void deJongFourthFunction(ga::Individual *ent);

void convertToString(int *vec, int size, char *chrom);

#endif /* EVALUATE_H_ */
