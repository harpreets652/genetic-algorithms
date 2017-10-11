/*
 * individual.cpp
 *
 *  Created on: Feb 3, 2016
 *      Author: sushil
 */

#include <individual.h>
#include <stdlib.h>
#include <random.h>

using namespace std;
using namespace ga;

Individual::Individual() {
    length = 0;
    setup();
}

Individual::Individual(int len) {
    length = len;
    setup();
}

void Individual::setup() {
    for (int i = 0; i < length; i++) {
        this->chrom[i] = -1;
    }
}


void Individual::init(int len) {
    assert(len <= MAX_CHROM_LENGTH);
    assert(len == TSPManager::getInstance()->getNumberOfNodes());

    length = len;

    vector<int> nodeKeys = TSPManager::getInstance()->getNodeKeys();
    shuffleVector(nodeKeys);

    for (int i = 0; i < length; i++) {
        this->chrom[i] = nodeKeys[i];
    }
}

void Individual::mutate(float prob) {
    for (int i = 0; i < length; i++) {
        if (flip(prob) == 1)
            chrom[i] = 1 - chrom[i];
    }
}

//swaps two positions
void Individual::mutateWithSwap(float prob) {
    //if mutate individual, select two locations and swap them
    if (flip(prob)) {
        int p1 = intInRange(0, length);
        int p2 = intInRange(0, length);

        int p1Val = chrom[p1];
        chrom[p1] = chrom[p2];
        chrom[p2] = p1Val;
    }
}

void Individual::copy(Individual *ip) {
    assert(ip->length <= MAX_CHROM_LENGTH);
    this->length = ip->length;
    for (int i = 0; i < length; i++) {
        this->chrom[i] = ip->chrom[i];
    }
    for (int i = 0; i < MAX_CRITERIA; i++) {
        this->fitness[i] = ip->fitness[i];
        this->objectives[i] = ip->objectives[i];
    }
    this->fit = ip->fit;
    this->scaledFit = ip->scaledFit;
}

