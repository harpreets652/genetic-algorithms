#ifndef __CONFIG_CPP_
#define __CONFIG_CPP_

#include "config.h"

Config& Config::operator=(const Config& other) {
    if (this == &other) {
        return *this;
    }
    this->PROB_MUTATION = other.PROB_MUTATION;
    this->PROB_CROSSOVER = other.PROB_CROSSOVER;
    this->INPUT_FILENAME = other.INPUT_FILENAME;
    return *this;
}

#endif