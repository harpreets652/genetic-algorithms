/*
 * population.h
 *
 *  Created on: Feb 2, 2016
 *      Author: sushil
 */

#ifndef POPULATION_H_
#define POPULATION_H_

#include <const.h>
#include <individual.h>
#include <options.h>

namespace ga {

	class Population{

	protected:
		void setup();

	public:
		Options options;
		Individual*  pop[MAX_POPSIZE];

		double max, min, avg;
		int maxi, mini;
		double sumFit, scaledSumFit;
		double scaledMax, scaledMin;
		unsigned int gen;

		//Population(){}
		Population(Options opts);
		~Population(){
			//delete [] pop;
		}

		void scale();
		void initialize();
		void eval(int start, int end); // this needs to be overridden
		//void parallelEval(int start, int end);
		void generation(Population *c);
		int proportionalSelector();
		void xoverAndMutate(Individual *p1, Individual *p2, Individual *c1, Individual *c2);
		void twoPoint(Individual *p1, Individual *p2, Individual *c1, Individual *c2);
		void ux(Individual *p1, Individual *p2, Individual *c1, Individual *c2);
		void chc(Population *c);
		void chcReproduce();

		void statistics();
		void report(int i);
		void halve(Population *child, int start, int end);
		void printPop(int start, int end);
		//bool compare(const Individual *i1, const Individual *i2);
		//void shuffle(int *x, int size);

		friend std::ostream &operator<<(std::ostream &output, const Population &p){
			for(int i = 0; i < p.options.popSize; i++){
				output << p.pop[i] << std::endl;
			}
			output << "Max: " << p.max << " Avg: " << p.avg << " Min: " << p.min << " SumFit: " << p.sumFit << std::endl;
			return output;
		}

	};

}

#endif /* POPULATION_H_ */
