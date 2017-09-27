/*
 * ga.h
 *
 *  Created on: Feb 3, 2016
 *      Author: sushil
 */

#ifndef GA_H_
#define GA_H_

#include <population.h>

namespace ga {

	class GA {

	private:
		void configure();

	public:
		Population *parent;
		Population *child;
		Options options;

		unsigned int maxFitGen;
		Individual *bestIndividualSoFar;
		double     bestFitnessSoFar;

		//GA(){}
		GA(int argc, char *argv[]);
		~GA(){}

		void setupOptions(int argc, char *argv[]);
		void init();
		void run();
		//virtual void CHC(Population *p, Population *c);
		void report();
		void updateProgress(unsigned int gen, Population *p);



	};

}



#endif /* GA_H_ */
