#ifndef __POPULATION_CPP_
#define __POPULATION_CPP_

#include "Population.h"
#include "config.h"
#include <cmath>
#include <climits>

Population::Population() : minFitness(INT_MAX), maxFitness(0.0), averageFitness(0.0), sumFitness(0.0), bestIndividualIndex(0), worstIndividualIndex(0) {

}

void Population::generate(int n) {
    this->clear();
    for(int i = 0 ; i < n; i++) {
        Individual randIndividual;
        randIndividual.init();
        this->push_back(randIndividual);
    }
}

void Population::print() const {
    cout << "Max Fitness:   " << maxFitness << endl;
    cout << "Min Fitness:   " << minFitness << endl;
    cout << "Avg Fitness:   " << averageFitness << endl;
    for (unsigned int i = 0; i < this->size(); i++) {
        cout << "[" << ((i < 10) ? "0" : "") << i << "] - ";
        at(i).print();
    }
}

void Population::evaluate() {
    // eval each individual, and collect your own stats
    maxFitness = 0.0;
    minFitness = 10000.0;
    sumFitness = 0.0;
    for (unsigned int i = 0; i < this->size(); i++) {
        at(i).evaluate();
        minFitness = min(minFitness, at(i).fitness);
        maxFitness = max(maxFitness, at(i).fitness);
        if (at(i).fitness > at(bestIndividualIndex).fitness) {
            bestIndividualIndex = i;
        }
        sumFitness += at(i).fitness;
    }
    averageFitness = sumFitness / double(this->size());
    // give that individual back its proportional size
    for (int i = 0; i < this->size(); i++) {
        (*this)[i].normalizedProb = at(i).fitness / sumFitness;
    }
}

void Population::sortByFitness() {
  sortByFitness(0, (unsigned int)size()-1);
}

void Population::sortByFitness(unsigned int left, unsigned int right) {
    if (left == right)
        return;
    else if (left + 1 == right) {
        // single swap check
        if (at(left).fitness > at(right).fitness) {
            std::swap((*this)[left], (*this)[right]);
        }
        return;
    }

    // recursive call...
    unsigned int mid = (right - left)/2 + left;
    sortByFitness(left, mid);
    sortByFitness(mid+1, right);

    // merge
    unsigned int i = left;
    unsigned int j = mid+1;
    while (i <= mid+1) {
        if (at(i).fitness > at(j).fitness) {
            std::swap((*this)[left], (*this)[right]);
            j++;
        }
        i++;
    }
}

Individual Population::proportionalSelect() {
    // get a random number between 0 and 1
    double shot = double(rand())/double(RAND_MAX);
    for (unsigned int i = 0; i < this->size(); i++) {
        shot -= at(i).normalizedProb;
        if (shot < 0) {
            return at(i);
        }
    }
    return at(size()-1);
}

Individual Population::tournamentSelect() {
  int left = Utils::randIntBetween(0, size());
  int right = Utils::randIntBetween(0, size());
  if (at(left).fitness >= at(right).fitness) {
    return at(left);
  } else {
    return at(right);
  }
}

Individual Population::getBestIndividual() const {
    return at(bestIndividualIndex);
}

#endif
