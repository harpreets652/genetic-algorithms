#ifndef __POPULATION_CPP_
#define __POPULATION_CPP_

#include "Population.h"
#include "config.h"
#include <cmath>
#include <climits>
#include <algorithm>
#include <thread>
#include <functional>
#include <cfloat>

Population::Population() : minAccuracy(INT_MAX), maxAccuracy(0.0), averageAccuracy(0.0),
                           minBitCount(INT_MAX), maxBitCount(0.0), averageBitCount(0.0),
                           bestIndividualIndex(0), worstIndividualIndex(0) {

}

void Population::generate(int n) {
    this->clear();
    Individual allTrue;
    allTrue.init();
    for (int i = 0; i < allTrue.size(); i++) {
        allTrue[i] = true;
    }
    for (int i = 0; i < n; i++) {
        Individual randIndividual;
        randIndividual.init();
        this->push_back(randIndividual);
    }
}

void Population::print() const {
    cout << "Max Fitness:   " << maxAccuracy << endl;
    cout << "Min Fitness:   " << minAccuracy << endl;
    cout << "Avg Fitness:   " << averageAccuracy << endl;
    for (unsigned int i = 0; i < this->size(); i++) {
        cout << "[" << ((i < 10) ? "0" : "") << i << "] - ";
        at(i).print();
    }
}

void Population::evaluateEach() {
    map<Individual, vector<unsigned int> > duplicateCount;
    vector<Individual *> evaluatingSet;

    for (unsigned int i = 0; i < size(); i++) {
        // if i have a duplicate, just increment his count
        if (duplicateCount.find(at(i)) != duplicateCount.end()) {
            duplicateCount[at(i)].push_back(i);
        } else {
            duplicateCount.insert(pair<Individual, vector<unsigned int> >(at(i), vector<unsigned int>(i)));
            evaluatingSet.push_back(&at(i));
        }
    }
    Evaluator::getInstance()->evaluate(evaluatingSet);

    for (auto const &value : duplicateCount) {
        for (unsigned int index : value.second) {
            (*this)[index] = at(value.second[0]);
        }
    }
}

void Population::evaluate(bool useParedoToCompare) {
    // eval each individual, and collect your own stats
    minAccuracy = 10000.0;
    maxAccuracy = 0.0;
    minBitCount = 10000.0;
    maxBitCount = 0.0;
    double sumAccuracy = 0.0;
    double sumBitCount = 0.0;

    evaluateEach();

    for (unsigned int i = 0; i < size(); i++) {
        at(i).print();
        minAccuracy = min(minAccuracy, at(i).accuracy);
        maxAccuracy = max(maxAccuracy, at(i).accuracy);
        minBitCount = min(minBitCount, (double)at(i).numFeaturesActive);
        maxBitCount = max(maxBitCount, (double)at(i).numFeaturesActive);
        if ((useParedoToCompare && at(i).paretoDominates(at(bestIndividualIndex))) ||
            (!useParedoToCompare && at(i).accuracy < at(bestIndividualIndex).accuracy)) {
            bestIndividualIndex = i;
        }
        sumAccuracy += at(i).accuracy;
        sumBitCount += at(i).numFeaturesActive;
    }
    averageAccuracy = sumAccuracy / double(size());
    averageBitCount = sumBitCount / double(size());
    // give that individual back its proportional size
    for (int i = 0; i < size(); i++) {
        (*this)[i].normalizedProb = at(i).accuracy / sumAccuracy;
    }

    // get rid of all the temp files
    Evaluator::getInstance()->exec("rm -f " + Evaluator::getInstance()->getDataLocation() + "/*.arff");
}

bool sortAccuracyFunc(Individual i, Individual j) {
    return i.accuracy < j.accuracy;
}
bool sortBitCountFunc(Individual i, Individual j) {
    return i.numFeaturesActive < j.numFeaturesActive;
}

void Population::sortByAccuracy() {
    sort(begin(), end(), sortAccuracyFunc);
}
void Population::sortByBitCount() {
    sort(begin(), end(), sortBitCountFunc);
}

vector<ParetoFront> sortFastNonDominated(Population& p) {
    int N = p.size();

    vector<ParetoFront> fronts(1);

    for (unsigned int i = 0; i < p.size(); i++) {
        p[i].peopleIDominate.clear();
        p[i].numDominateMe = 0;

        for (unsigned int j = 0; j < p.size(); j++) {
            if (j == i) {
                continue;
            }

            if (p[i].paretoDominates(p[j])) {
                p[i].peopleIDominate.push_back(&p[j]);
            } else if (p[j].paretoDominates(p[i])) {
                p[i].numDominateMe++;
            }
        }

        if (p[i].numDominateMe == 0) {
            p[i].rank = 1;
            fronts[0].push_back(p[i]);
        }
    }

    // for each person in the first front
    ParetoFront nextFront;
    int currentFrontIndex = 0;

    // while this front doesn't have people that are empty
    while (!fronts[currentFrontIndex].empty()) {
        for (Individual& p1 : fronts[currentFrontIndex]) {
            for (Individual* p2 : p1.peopleIDominate) {
                p2->numDominateMe--;
                if (p2->numDominateMe == 0) {
                    nextFront.push_back(*p2);
                }
            }
        }
        currentFrontIndex++;
        fronts.push_back(nextFront);
        nextFront.clear();
    }

    return fronts; // STUB
}

Individual Population::proportionalSelect() {
    // get a random number between 0 and 1
    double shot = double(rand()) / double(RAND_MAX);
    for (unsigned int i = 0; i < this->size(); i++) {
        shot -= at(i).normalizedProb;
        if (shot < 0) {
            return at(i);
        }
    }
    return at(size() - 1);
}

Individual Population::tournamentSelect() {
    int left = Utils::randIntBetween(0, size());
    int right = Utils::randIntBetween(0, size());
    if (at(left).accuracy >= at(right).accuracy) {
        return at(left);
    } else {
        return at(right);
    }
}

Individual Population::getBestIndividual() const {
    return at(bestIndividualIndex);
}

void ParetoFront::assignCrowdingDistance() {
    if (isSorted || empty())
        return;

    isSorted = true;

    for (Individual& i : (*this)) {
        i.distance = 0;
    }
    // accuracy
    sortByAccuracy();
    (*this)[0].distance = DBL_MAX;
    (*this)[size()-1].distance = DBL_MAX;
    for (unsigned int i = 1; i < size()-1; i++) {
        double numerator = at(i+1).accuracy - at(i-1).accuracy;
        double denominator = 100;
        (*this)[i].distance += numerator/denominator;
    }

    // bit count
    sortByBitCount();
    (*this)[0].distance = DBL_MAX;
    (*this)[size()-1].distance = DBL_MAX;
    for (unsigned int i = 1; i < size()-1; i++) {
        // we have to flip these instead (because we're minimizing this number)
        double numerator = at(i-1).numFeaturesActive - at(i+1).numFeaturesActive;
        double denominator = 40;
        (*this)[i].distance += numerator/denominator;
    }
}

// CCO - Crowding-Comparison Operator
bool sortCCOFunc(Individual i, Individual j) {
    return (i.rank < j.rank) || ((i.rank == j.rank) && (i.distance > j.distance));
}

void ParetoFront::sortByCrowdingOperator() {
    sort(begin(), end(), sortCCOFunc);
}

#endif
