/*
 * individual.h
 *
 *  Created on: Feb 2, 2016
 *      Author: sushil
 */

#ifndef INDIVIDUAL_H_
#define INDIVIDUAL_H_

#include <iostream>
#include <const.h>
#include <assert.h>
#include "TSPManager.h"

namespace ga {

    class Individual {
    private:
        void setup();

    public:
        int length;
        int chrom[MAX_CHROM_LENGTH];
        double fit, scaledFit;
        double fitness[MAX_CRITERIA];
        double objectives[MAX_CRITERIA];


        Individual();

        Individual(int len);

        ~Individual() {
        }

        void copy(Individual *i);

        void init(int len);

        void mutate(float prob);

        void mutateWithSwap(float prob);

        friend std::ostream &operator<<(std::ostream &output, const Individual &ip) {
            for (int i = 0; i < ip.length; i++) {
                if (i > 0) {
                    output << ", ";
                }
                output << ip.chrom[i];
            }
            output << " "; //std::endl;

            output << "Fitness: " << ip.fit << std::endl;
            return output;
        }
    };
}

#endif /* INDIVIDUAL_H_ */
