//
// Created by Harpreet Singh on 10/1/17.
//

#include "TSPManager.h"

TSPManager *TSPManager::instance = nullptr;

TSPManager *TSPManager::getInstance() {
    if (!instance) {
        instance = new TSPManager;
    }

    return instance;
}

TSPManager::TSPManager() {
}

void TSPManager::loadTSPProblem(std::string fileName) {
    std::ifstream fileStream(fileName);

    if (!fileStream.is_open()) {
        std::cerr << "could not open file " << fileName << std::endl;
        return;
    }

    //ignore meta data
    std::string metaData;
    while (getline(fileStream, metaData)) {
        if (metaData == "NODE_COORD_SECTION") {
            break;
        }
    }

    float xLoc, yLoc;
    std::string indexString;
    while (fileStream.good()) {
        fileStream >> indexString;

        if (indexString == "EOF") {
            break;
        }

        fileStream >> xLoc >> yLoc;
        std::string::size_type sz;

        int nodeIndex = std::stoi(indexString, &sz);
        nodes[nodeIndex] = std::make_pair(xLoc, yLoc);
    }
}

void TSPManager::preCalculateDistances() {
    for (auto source : nodes) {
        for (auto destination : nodes) {
            //calculate distance
            float xD = source.second.first - destination.second.first;
            float yD = source.second.second - destination.second.second;
            auto distance = (std::sqrt(xD * xD + yD * yD));
            distances[source.first][destination.first] = distance;
        }
    }
}

float TSPManager::getEuclideanDistance(int source, int destination) {
    if (distances.find(source) != distances.end()) {
        if (distances[source].find(destination) != distances[source].end()) {
            return distances[source][destination];
        }
    }

    return 0;
}

int TSPManager::getNumberOfNodes() {
    return static_cast<int>(nodes.size());
}

std::vector<int> TSPManager::getNodeKeys() {
    std::vector<int> keys;
    for (auto kv : nodes) {
        keys.push_back(kv.first);
    }

    return keys;
}
