#include <iostream>
#include <random>
#include <thread>
#include <fstream>

//void testing();

double eval(int *pj);

void solveEval(int *solution);

bool foundBetterSolution(double best, double newVal);

void modifySolutionRandom(int *candidateSolution, std::mt19937 &engine);

void modifySolutionConsecutiveNLocations(int *candidateSolution, std::mt19937 &engine);

void modifySolutionConsecutive(int *candidateSolution, int numOfConsecutiveChanges, int startingLocation);

void multiThread();

//configuration parameters
static const int NUM_OF_THREADS = 1;
static const int CANDIDATE_SOLUTION_SIZE = 150;

int main() {
    std::thread parallel[NUM_OF_THREADS];

    for (auto &i : parallel) {
        i = std::thread(multiThread);
    }

    std::cout << "launched " << NUM_OF_THREADS << " threads" << std::endl;

    for (auto &j : parallel) {
        j.join();
    }

    return 0;
}

//todo: remove before submitting
//void testing() {
//    std::string stuff = "000000101010101000100001001100011001100001100111011111001011000001101010001100101111100010110000101101101100100100111110111001011011100010101001110010";
//
//    for (char &i : stuff) {
//        std::cout << i << ",";
//    }
//    std::cout << std::endl;
//}
//
//double eval(int *la) {
//    return 0;
//}

void multiThread() {
    int vec[CANDIDATE_SOLUTION_SIZE];
    solveEval(vec);

    std::cout << "Best fitness = " << eval(vec) << std::endl;
    std::cout << "Best Solution = ";

    for (int i : vec) {
        std::cout << i;
    }
    std::cout << std::endl;
}


//multi-thread this entire method
void solveEval(int *solution) {
    //initialize solution
    double bestFitness = 0;
    double newFitness = 0;
    int bestSol[CANDIDATE_SOLUTION_SIZE];

    std::random_device rd;
    std::mt19937 engine(rd());

    std::uniform_int_distribution<> dist(0, CANDIDATE_SOLUTION_SIZE);
    //initialize solution
    for (int i = 0; i < CANDIDATE_SOLUTION_SIZE; i++) {
        solution[i] = dist(engine) % 2;
    }

    std::string outputFileName = "second4.csv";
    std::ofstream fileOut(outputFileName);
    if (!fileOut) {
        std::cerr << "failed to open file " << outputFileName << std::endl;
        exit(-1);
    }
    fileOut << "Iteration,BestFitness" << "\n";
    fileOut.flush();

    for (int j = 0; j < 100000000; ++j) {
        if (j != 0 && j % 100 == 0) {
            std::cout << "iteration : " << j;
            std::cout << ", best fitness : " << bestFitness;
            std::cout << ", solution : " << std::endl;
            for (int k : bestSol) {
                std::cout << k;
            }

            std::cout << std::endl;
        }

        //evaluate
        newFitness = eval(solution);

        if (foundBetterSolution(bestFitness, newFitness)) {
            bestFitness = newFitness;

            for (int g = 0; g < CANDIDATE_SOLUTION_SIZE; g++) {
                bestSol[g] = solution[g];
            }
        }

        if (j % 100 == 0) {
            fileOut << j << "," << bestFitness << "\n";
            fileOut.flush();
        }

        modifySolutionRandom(solution, engine);
    }

    fileOut.close();
    for (int s = 0; s < CANDIDATE_SOLUTION_SIZE; s++) {
        solution[s] = bestSol[s];
    }
}

void modifySolutionRandom(int *candidateSolution, std::mt19937 &engine) {
    std::uniform_int_distribution<> dist(0, CANDIDATE_SOLUTION_SIZE - 1);
    int numOfChanges = 50;

    for (int i = 0; i < numOfChanges; ++i) {
        //which bits to change
        int elementToChange = dist(engine);
        candidateSolution[elementToChange] = 1 - candidateSolution[elementToChange];
    }
}


void modifySolutionConsecutiveNLocations(int *candidateSolution, std::mt19937 &engine) {
    std::uniform_int_distribution<> dist(0, CANDIDATE_SOLUTION_SIZE - 1);

    for (int i = 0; i < 3; ++i) {
        modifySolutionConsecutive(candidateSolution, 10, dist(engine));
    }
}

//modify consecutive with wrap-around
void modifySolutionConsecutive(int *candidateSolution, int numOfConsecutiveChanges, int startingLocation) {
    for (int i = 0; i < numOfConsecutiveChanges; ++i) {
        int loc = startingLocation + i;
        if (loc >= CANDIDATE_SOLUTION_SIZE) {
            loc -= CANDIDATE_SOLUTION_SIZE;
        }
        //which bits to change
        candidateSolution[loc] = 1 - candidateSolution[loc];
    }
}

bool foundBetterSolution(double best, double newVal) {
    return newVal > best;
}