//
// Created by Harpreet Singh on 10/1/17.
//

#ifndef TSPMANAGER_H
#define TSPMANAGER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <math.h>

class TSPManager {
public:
    static TSPManager *getInstance();

    void loadTSPProblem(std::string fileName);

    void preCalculateDistances();

    int getEuclideanDistance(int source, int destination);

    int getNumberOfNodes();

    std::vector<int> getNodeKeys();

private:
    TSPManager();

    static TSPManager *instance;
    std::unordered_map<int, std::pair<float, float>> nodes;
    std::unordered_map<int, std::unordered_map<int, int>> distances;
};

#endif //TSPMANAGER_H
