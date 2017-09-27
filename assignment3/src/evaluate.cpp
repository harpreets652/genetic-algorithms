/*
 * evaluate.cpp
 *
 *  Created on: Feb 3, 2016
 *      Author: sushil
 */

#include <random>
#include "evaluate.h"


using namespace std;

std::random_device rd;
std::mt19937 engine(rd());

double gaussian(double mean, double stdDev) {
    std::normal_distribution<> d(mean, stdDev);
    return d(engine);
}

void evaluate(ga::Individual *ent) {
//	cout << *ent << endl;
    double sum = 0;
    for (int i = 0; i < ent->length; i++) {
        sum += ent->chrom[i];
    }
    ent->fit = sum;
    for (int i = 0; i < ga::MAX_CRITERIA; i++) {
        ent->fitness[i] = sum;
    }

//	cout << "----------------------------------------" << endl;

}

double xSquaredDecode(int *chrom, int start, int end) {
    double prod = 0;
    for (int i = start; i < end; i++) {
        prod += (chrom[i] == 0 ? 0 : pow(2.0, (double) i));
    }
    return prod;
}

void xSquared(ga::Individual *ent) {
    int value = xSquaredDecode(ent->chrom, 0, ent->length);
    ent->fit = value * value;
}

double decodeDeJong(const int *chrom, int chromosomeLength, int startPos,
                    double upperBound, double lowerBound) {
    unsigned long maxDecimal = 1;
    double decodedDecimal = 0.0;
    for (int i = startPos + chromosomeLength - 1; i > startPos - 1; i--) {
        if (chrom[i]) {
            decodedDecimal += static_cast<double>(maxDecimal);
        }

        maxDecimal <<= 1;
    }

    //not including 0
    maxDecimal--;

    double precision = (upperBound - lowerBound) / (double) maxDecimal;
    return lowerBound + (decodedDecimal * precision);
}

/*
 *  -5.12 <= Xi <= 5.12
 *  10 bits to represent all possible values (max-min)/step = 1024
 *  chromosome length of 30 for x1, x2, and x3
 */
void deJongFirstFunction(ga::Individual *ent) {
    double x1 = decodeDeJong(ent->chrom, 10, 0, 5.12, -5.12);
    double x2 = decodeDeJong(ent->chrom, 10, 10, 5.12, -5.12);
    double x3 = decodeDeJong(ent->chrom, 10, 20, 5.12, -5.12);

    double objectiveValue = pow(x1, 2.0) + pow(x2, 2.0) + pow(x3, 2.0);

    //to convert to a maximization problem
    ent->fit = 1.0 / objectiveValue;
}

/*
 *  -2.048 <= Xi <= 2.048
 *  12 bits to represent all possible values (max-min)/step = 4096
 *  chromosome length of 24
 */
void deJongSecondFunction(ga::Individual *ent) {
    double x1 = decodeDeJong(ent->chrom, 12, 0, 2.048, -2.048);
    double x2 = decodeDeJong(ent->chrom, 12, 12, 2.048, -2.048);

    double objectiveValue = 100 * pow((pow(x1, 2.0) - x2), 2.0) + pow((1 - x1), 2.0);

    //to convert to a maximization problem
    ent->fit = 1.0 / objectiveValue;
}

/*
 *  -5.12 <= Xi <= 5.12
 *  10 bits to represent all possible values (max-min)/step = 1024
 *  chromosome length of 50 for x1, x2, x3, x4, x5
 */
void deJongThirdFunction(ga::Individual *ent) {
    double x1 = decodeDeJong(ent->chrom, 10, 0, 5.12, -5.12);
    double x2 = decodeDeJong(ent->chrom, 10, 10, 5.12, -5.12);
    double x3 = decodeDeJong(ent->chrom, 10, 20, 5.12, -5.12);
    double x4 = decodeDeJong(ent->chrom, 10, 30, 5.12, -5.12);
    double x5 = decodeDeJong(ent->chrom, 10, 40, 5.12, -5.12);

    int objectiveValue = (int) x1 + (int) x2 + (int) x3 + (int) x4 + (int) x5;

    //to convert to a maximization problem
    ent->fit = 25 - objectiveValue;
}

/*
 *  -1.28 <= Xi <= 1.28
 *  8 bits to represent all possible values (max-min)/step = 256
 *  chromosome length of 240 for 30 Xi
 *  Using 1250 to account to make this into an maximization problem
 *  maximum of f(x)=1248.22 (assuming gaussian(0,1) is 0 most of the time)
 */
void deJongFourthFunction(ga::Individual *ent) {
    int startPos = 0;
    double objectiveValue = 0.0;
    for (int i = 0; i < 30; i++) {
        double x_i = decodeDeJong(ent->chrom, 8, startPos, -1.28, 1.28);
        objectiveValue += (i + 1) * pow(x_i, 4) + gaussian(0, 1);
        startPos += 8;
    }

    ent->fit = 1250 - objectiveValue;
}

//Note~ implement De Jong's functions here