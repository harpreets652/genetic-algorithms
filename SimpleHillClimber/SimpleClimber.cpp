#include <iostream>
#include <vector>

double eval(int *pj);

void solveEval(int *solution);

bool foundBetterSolution(double best, double newVal);

void modifySolution(int *candidateSolution, int index);

int main() {
    int vec[100];
    solveEval(vec);

    std::cout << "fitness = " << eval(vec) << std::endl;
    std::cout << "solution = ";

    for (int i = 0; i < 100; i++) {
        std::cout << vec[i];
    }
    std::cout << std::endl;
}

void solveEval(int *solution) {
    //initialize solution
    double objectiveBest = 0;
    double objectiveNew = 0;
    std::vector<int> bestSol;

    for (int i = 0; i < 100; i++) {
        solution[i] = 0;
    }

    int bitIndex = 0;
    for (int j = 0; j < 1000; ++j, bitIndex++) {
        //evaluate
        objectiveNew = eval(solution);

        if (foundBetterSolution(objectiveBest, objectiveNew)) {
            objectiveBest = objectiveNew;
            bestSol.clear();
            bestSol.insert(bestSol.begin(), solution, solution + 100);
        }

        if (bitIndex > 100) {
            bitIndex = 0;
        }
        modifySolution(solution, bitIndex);
    }


    for (int s = 0; s < 100; s++) {
        solution[s] = bestSol[s];
    }
}

//flip the bit
void modifySolution(int *candidateSolution, int index) {
    candidateSolution[index] = 1 - candidateSolution[index];
}

bool foundBetterSolution(double best, double newVal) {
    return newVal >= best;
}



