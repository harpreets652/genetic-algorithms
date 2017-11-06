#ifndef __EVALUATOR_CPP_
#define __EVALUATOR_CPP_

#include <fstream>

#include "Evaluator.h"
#include "config.h"

Evaluator* Evaluator::instance = nullptr;

Evaluator* Evaluator::getInstance() {
    if (instance == nullptr) {
        instance = new Evaluator();
    }
    return instance;
}

Evaluator::Evaluator() {
  bestSolution = new Individual();
  init();
}

Evaluator::~Evaluator() {
  if (bestSolution != NULL) {
    delete bestSolution;
  }
}

void Evaluator::init() {
    // check the config
    string solutionFilename, bufferString;
    unsigned int lineNumber;
    ifstream fin;
    Point tempPoint;

    // get filename
    pointsOfReference.clear();
    fin.clear();
    fin.open(config.INPUT_FILENAME.c_str());
    // ignore the initial fluff
    for (int i = 0; i < 6; i++) {
        getline(fin, bufferString);
    }
    // variables
    for (int i = 0; i < config.CITY_TOUR_SIZE; i++) {
        fin >> lineNumber >> tempPoint.x >> tempPoint.y;
        pointsOfReference.push_back(tempPoint);
    }

    fin.close();
}

void Evaluator::getBestSolution() {
  // find the solution file and read in the values
  ifstream fin;
  fin.clear();
  fin.open(config.SOLUTION_FILENAME.c_str());
  for (int i = 0; i < config.CITY_TOUR_SIZE; i++) {
    fin >> (*bestSolution)[i];
  }
  cout << endl;
  fin.close();

  // get my own statistics on the issue
  bestSolution->evaluate();
}

void Evaluator::evaluate(Individual &indiv) {
    double sum = 0.0;
    for (int i = 0; i < indiv.size()-1; i++) {
        sum += getDistanceBetween(indiv[i], indiv[i+1]);
    }
    sum += getDistanceBetween(indiv[0], indiv[indiv.size()-1]);
    indiv.distance = sum;
    switch(config.city) {
        case BURMA:
            indiv.fitness = 1000.0/sum;
            break;
        case BERLIN:
            indiv.fitness = 10000000.0/sum;
            break;
        case ELI_51:
            indiv.fitness = 100000.0/sum;
            break;
        case ELI_76:
            indiv.fitness = 100000.0/sum;
            break;
        case LIN_105:
            indiv.fitness = 10000000.0/sum;
            break;
        case LIN_318:
            indiv.fitness = 100000000.0/sum;
            break;
        default:
            indiv.fitness = 1.0/sum;
            break;
    }

    if (&indiv != bestSolution) {
      if (!haveEvaluatedBestSolution) {
          getBestSolution();
          haveEvaluatedBestSolution = true;
      }
      indiv.diffFitness = (indiv.fitness - bestSolution->fitness);
      indiv.diffDistance = (indiv.distance - bestSolution->distance);
    }
}

double Evaluator::getDistanceBetween(unsigned int startIndex, unsigned int endIndex) {
    // delta x squared + delta y squareds
    //cout << pointsOfReference[startIndex].x << "--" << pointsOfReference[startIndex].y << endl;
    double deltaX = pointsOfReference[startIndex].x - pointsOfReference[endIndex].x;
    deltaX = pow(deltaX, 2);
    double deltaY = pointsOfReference[startIndex].y - pointsOfReference[endIndex].y;
    deltaY = pow(deltaY, 2);

    if (deltaX < 0 || deltaY < 0)
      cout << "negative values...." << endl;

    //TODO: remove the sqrt for faster processings
    return sqrt(deltaX + deltaY);
    return (deltaX + deltaY);
}

#endif
