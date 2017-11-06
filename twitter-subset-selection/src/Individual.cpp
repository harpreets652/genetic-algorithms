#ifndef __INDIVIDUAL_CPP_
#define __INDIVIDUAL_CPP_

#include <utility>

#include "Individual.h"
#include "config.h"

Config config;

Individual::Individual() : vector<unsigned int>(config.CITY_TOUR_SIZE), fitness(0.0), distance(0.0), normalizedProb(0.0) {
}

Individual::~Individual() {
}

void Individual::init() {

}

void Individual::generateRandom() {
    // include all the indeces
    for (unsigned int i = 0; i < this->size(); i++) {
        (*this)[i] = i;
    }
    // shuffle them up
    for (unsigned int i = 0; i < this->size(); i++) {
        std::swap((*this)[i], (*this)[randIntBetween(i, this->size())]);
    }
}

void Individual::print() const {
    for (int i = 0; i < this->size(); i++) {
        if (at(i) < 10)
            cout << "0";
        cout << at(i) << " ";
    }
    cout << " - [fitness: " << fitness << ", distance: " << distance << "]" << endl;
}

void Individual::evaluate() {
    cout << "pre evaluate" << endl;
    Evaluator::getInstance()->evaluate((*this));
    cout << "POST evaluate" << endl;
}

void Individual::crossoverWith(Individual& other) {
    // copy variables
    Individual left = (*this), right = other;
    // pick random index
    int index = randIntBetween(0, size());

    // make the left dominated child
        // iterate through the right, up to the random index
        for (int i = 0; i < index; i++) {
            int numberToFind = other[i];
            // look for that number in the left
            int j = 0;
            while (left[j] != numberToFind) {
                j++;
            }
            // swap it with that other number
            std::swap(left[i], left[j]);
        }

        // do the same for the right
        for (int i = 0; i < index; i++) {
            int numberToFind = at(i);
            int j = 0;
            while(right[j] != numberToFind) {
                j++;
            }
            std::swap(right[i], right[j]);
        }

    // assign the left and right back to their owners
    (*this) = left;
    other = right;
}

void Individual::mutate() {
    // pick two random indeces
    int index = randIntBetween(0, size());
    int otherIndex  = randIntBetween(0, size());

    // swap them
    std::swap((*this)[index], (*this)[otherIndex]);
}

#endif
