/*
 * population.cpp
 *
 *  Created on: Feb 3, 2016
 *      Author: sushil
 */

#include <assert.h>
#include <population.h>
#include <random.h>
#include <utils.h>
#include <evaluate.h>
#include <options.h>

using namespace ga;

Population::Population(Options opts) {
    options = opts;
    setup();
}

void Population::setup() {
    int totalPopulationSize = options.popSize + (options.popSize * options.lambda);
    assert(totalPopulationSize <= MAX_POPSIZE);

    for (int i = 0; i < totalPopulationSize; i++) {
        pop[i] = new Individual(options.chromLength);
        pop[i]->init(options.chromLength);
    }
    avg = min = max = -1;
    mini = maxi = -1;
}

void Population::initialize() {
    eval(0, options.popSize);
    statistics();
    report(0);
}

//simple crossover/mutation; not a good function optimizer
void Population::generation(Population *child) {
    int pi1, pi2, ci1, ci2;
    Individual *p1, *p2, *c1, *c2;

    for (int i = 0; i < options.popSize; i += 2) {
        pi1 = proportionalSelector();
        pi2 = proportionalSelector();
        ci1 = i;
        ci2 = i + 1;

        p1 = pop[pi1];
        p2 = pop[pi2];
        c1 = child->pop[ci1];
        c2 = child->pop[ci2];

        xoverAndMutate(p1, p2, c1, c2);
    }

    child->eval(0, options.popSize);
}

//Note~ mu + lambda selection strategy
void Population::muPlusLambdaSelection(Population *child) {
    int parentIndex1, parentIndex2, childIndex1, childIndex2;
    Individual *p1, *p2, *c1, *c2;

    int childPopSize = options.popSize * options.lambda;
    for (int i = 0; i < childPopSize; i += 2) {
        //todo: proportionalSelector() intInRange(0, options.popSize)
        parentIndex1 = intInRange(0, options.popSize);
        parentIndex2 = intInRange(0, options.popSize);
        childIndex1 = options.popSize + i;
        childIndex2 = options.popSize + i + 1;

        p1 = pop[parentIndex1];
        p2 = pop[parentIndex2];
        c1 = pop[childIndex1];
        c2 = pop[childIndex2];

        partiallyMatchingCrossover(p1, p2, c1, c2);
        c1->mutateWithSwap(options.pm);
        c2->mutateWithSwap(options.pm);
    }

    //evaluate all children
    eval(options.popSize, options.popSize + childPopSize);

    //if lambda = 1, then can use parents for subsequent generation, otherwise only children
    int parentOffset = 0;
    if (options.lambda > 1) {
        parentOffset = options.popSize;
    }
    //sort this generation by fitness in descending order
    std::sort(pop + parentOffset,
              pop + (options.popSize + childPopSize),
              [](Individual *i1, Individual *i2) {
                  return i1->fit > i2->fit;
              });

    //select best $popSize individuals shuffled
    std::vector<int> bestInPopIndexesShuffled;
    bestInPopIndexesShuffled.reserve((unsigned long) (options.popSize));
    for (int j = 0; j < options.popSize; j++) {
        bestInPopIndexesShuffled.push_back(j + parentOffset);
    }
    shuffleVector(bestInPopIndexesShuffled);

    for (int k = 0; k < options.popSize; k++) {
        child->pop[k]->copy(pop[bestInPopIndexesShuffled[k]]);
    }
}

void Population::chcReproduce() {
    int pi1, pi2, ci1, ci2;
    Individual *p1, *p2, *c1, *c2;
    for (int i = 0; i < options.popSize; i += 2) {
        pi1 = proportionalSelector();
        pi2 = proportionalSelector();
        ci1 = options.popSize + i;
        ci2 = options.popSize + i + 1;

        p1 = pop[pi1];
        p2 = pop[pi2];
        c1 = pop[ci1];
        c2 = pop[ci2];

        xoverAndMutate(p1, p2, c1, c2);
    }

}

void Population::chc(Population *child) {
//	std::cout << "Parents" << std::endl;
//	printPop(0, options.popSize);

    chcReproduce();
//	std::cout << "Parents + Children" << std::endl;
//	printPop(0, options.popSize * options.lambda);

    eval(options.popSize, options.popSize * options.lambda);

//	std::cout << "Children Evaluated" << std::endl;
//	printPop(0, options.popSize * options.lambda);

    halve(child, 0, options.popSize * options.lambda);
//	std::cout << "Children" << std::endl;
//	child->printPop(0, options.popSize);
}


bool compare(const Individual *i1, const Individual *i2) {
    return (i1->fit > i2->fit);
}


void Population::halve(Population *child, int start, int end) {
//	for(int i = 0; i < options.popSize * options.lambda; i++){
//		std::cout << *(pop[i]) << std::endl;
//	}
//	std::cout << "-----------------------------------------" << std::endl;
    std::sort(&pop[start], &pop[end - 1], compare);
    //printPop(start, end);

    int tmp[options.popSize];
    shuffle(tmp, options.popSize);
    for (int i = 0; i < options.popSize; i++) {
        child->pop[tmp[i]]->copy(pop[i]);
    }
    //std::cout << "Child pop" << std::endl;
    //child->printPop(0, options.popSize);

}

void Population::xoverAndMutate(Individual *p1, Individual *p2, Individual *c1, Individual *c2) {
    //int nPoints = 2; we are doing two point crossover

    // std::cout << "Crossover parents" << std::endl;
    // std::cout << *p1;
    // std::cout << *p2;
    for (int i = 0; i < options.chromLength; i++) { //First copy
        c1->chrom[i] = p1->chrom[i];
        c2->chrom[i] = p2->chrom[i];
    }
    if (flip(options.px)) { // if prob, then cross/exchange bits
        twoPoint(p1, p2, c1, c2);
    }

    // std::cout << "Crossover children" << std::endl;
    // std::cout << *c1;
    // std::cout << *c2;
    c1->mutate(options.pm);
    c2->mutate(options.pm);
}

void Population::twoPoint(Individual *p1, Individual *p2, Individual *c1, Individual *c2) { //not debugged
    int t1 = intInRange(0, options.chromLength);
    int t2 = intInRange(0, options.chromLength);
    int xp1 = std::min(t1, t2);
    int xp2 = std::max(t1, t2);

    for (int i = xp1; i < xp2; i++) {
        c1->chrom[i] = p2->chrom[i];
        c2->chrom[i] = p1->chrom[i];
    }
}

//debugging code
void checkForDuplicates(Individual *p) {
    std::unordered_map<int, int> debugMap;
    for (int j = 0; j < p->length; j++) {
        debugMap[p->chrom[j]]++;
    }

    for (auto entry : debugMap) {
        if (entry.second > 1) {
            std::cout << "duplicate entries" << std::endl;
        }
    }
}

void Population::partiallyMatchingCrossover(Individual *p1, Individual *p2, Individual *c1, Individual *c2) {
    //if not performing crossover, copy parents as is
    if (!flip(options.px)) {
        for (int i = 0; i < options.chromLength; i++) {
            c1->chrom[i] = p1->chrom[i];
            c2->chrom[i] = p2->chrom[i];
        }

        return;
    }

    int t1 = intInRange(0, options.chromLength);
    int t2 = intInRange(0, options.chromLength);
    int xp1 = std::min(t1, t2);
    int xp2 = std::max(t1, t2);

    //if sub-section is entire chromosome, decrease it
    if ((xp1 - xp2) == options.chromLength - 1) {
        xp2 -= 2;
    }

    singlePMXChild(p1, p2, c1, xp1, xp2);
    singlePMXChild(p2, p1, c2, xp1, xp2);
}

//sub-section from parent1 and rest from parent2 into child
void Population::singlePMXChild(Individual *parent1, Individual *parent2,
                                Individual *child, int xP1, int xP2) {
    //copy section from parent one into child
    std::unordered_map<int, int> p1ToP2Swap;
    for (int i = xP1; i <= xP2; i++) {
        child->chrom[i] = parent1->chrom[i];
        p1ToP2Swap[parent1->chrom[i]] = parent2->chrom[i];
    }

    //complete map by copying data from parent 2 into child, while substituting duplicates
    completeChildChrom(parent2, child, p1ToP2Swap, 0, xP1);
    completeChildChrom(parent2, child, p1ToP2Swap, xP2 + 1, options.chromLength);
}

//complete map by copying data from parent into child, while substituting duplicates
void Population::completeChildChrom(Individual *parent,
                                    Individual *child,
                                    std::unordered_map<int, int> &subSecSwapMap,
                                    int start,
                                    int end) {
    for (int i = start; i < end; i++) {
        int pVal = parent->chrom[i];

        //until we find a replacement for duplicate
        while (subSecSwapMap.find(pVal) != subSecSwapMap.end()) {
            pVal = subSecSwapMap[pVal];
        }

        child->chrom[i] = pVal;
    }
}

void Population::ux(Individual *p1, Individual *p2, Individual *c1, Individual *c2) {
    for (int i = 0; i < options.chromLength; i++) {
        if (flip(0.5)) {
            c1->chrom[i] = p2->chrom[i];
            c2->chrom[i] = p1->chrom[i];
        }
    }
}

int Population::proportionalSelector() {
    int i = 0;
    double sum = 0;
    double limit = randomFraction() * sumFit;

    do {
        sum += pop[i++]->fit;
    } while (sum < limit && i < options.popSize);

    return i - 1; //Make sure this works properly
}

void Population::eval(int start, int end) {
    assert(start >= 0 && end >= start);
    for (int i = start; i < end; i++) {
        //evaluate(pop[i]);
        tspEval(pop[i]);
    }
}

void Population::statistics() {
    max = min = avg = sumFit = pop[0]->fit;
    maxi = mini = 0;

    for (int i = 1; i < options.popSize; i++) {
        sumFit += pop[i]->fit;
        if (pop[i]->fit < min) {
            min = pop[i]->fit;
            mini = i;
        }
        if (pop[i]->fit > max) {
            max = pop[i]->fit;
            maxi = i;
        }
    }
    avg = sumFit / options.popSize;
}

//Note~ adjust precision as needed
void Population::report(int gen) {
    char printbuf[1024];
    sprintf(printbuf, "%4i \t %0.8f \t %0.8f \t %0.8f\n ", gen, min, avg, max);
    writeBufToFile(printbuf, options.outfile);
    std::cout << printbuf;// << std::endl;
}

void Population::printPop(int start, int end) {
    for (int i = start; i < end; i++) {
        std::cout << *(pop[i]) << std::endl;
    }
    std::cout << "-----------------------------------------" << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
}
