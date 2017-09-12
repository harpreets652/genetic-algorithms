#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <chrono>
#include <ctime>

double eval(int *pj);

void solveEval(int *solution);

bool foundBetterSolution(double best, double newVal);

void modifySolution(int *candidateSolution, std::mt19937 &engine);

void multiThread();

static const int numOfThreads = 1;

int main() {
    std::thread parallel[numOfThreads];

    for (int i = 0; i < numOfThreads; ++i) {
        parallel[i] = std::thread(multiThread);
    }

    std::cout << "launched " << numOfThreads << " threads" << std::endl;

    for (int j = 0; j < numOfThreads; ++j) {
        parallel[j].join();
    }

    return 0;
}

void multiThread() {
    int vec[100];
    solveEval(vec);

    std::cout << "fitness = " << eval(vec) << std::endl;
    std::cout << "solution = ";

    for (int i = 0; i < 100; i++) {
        std::cout << vec[i];
    }
    std::cout << std::endl;
}


//multi-thread this entire method
void solveEval(int *solution) {
    //initialize solution
    double objectiveBest = 0;
    double objectiveNew = 0;
    std::vector<int> bestSol;

    //initialize random generator
    std::random_device rd;
    std::mt19937 engine(rd());

    for (int i = 0; i < 100; i++) {
        if (i < 50)
            solution[i] = 1;
        else
            solution[i] = 0;
    }

    for (int j = 0; j < 1000000000; ++j) {
        if (j % 1000000 == 0) {
            time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::cout << "iteration : " << j << ", " << ctime(&tt)
                      << "--------->current fitness: " << objectiveBest << std::endl;
        }

        //evaluate
        objectiveNew = eval(solution);
        if (foundBetterSolution(objectiveBest, objectiveNew)) {
            objectiveBest = objectiveNew;
            bestSol.clear();
            bestSol.insert(bestSol.begin(), solution, solution + 100);
        }

        modifySolution(solution, engine);
    }

    for (int s = 0; s < 100; s++) {
        solution[s] = bestSol[s];
    }
}

void modifySolution(int *candidateSolution, std::mt19937 &engine) {
    std::uniform_int_distribution<> dist(50, 99);
    int numOfChanges = (dist(engine)) + 1;

    for (int i = 0; i < numOfChanges; ++i) {
        //which bits to change
        int elementToChange = dist(engine);
        candidateSolution[elementToChange] = 1 - candidateSolution[elementToChange];
    }
}

bool foundBetterSolution(double best, double newVal) {
    return newVal > best;
}