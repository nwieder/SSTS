#ifndef RANDOM_GENERATOR_H_
#define RANDOM_GENERATOR_H_

#include "_Random.h"
#include <time.h>

namespace nw{

/** @brief "Minimal" random number generator of Park and Miller
 *
 * with Bays-Durham shuffle and added safeguards. Returns a uniform random
 * deviate between 0.0 and 1.0 (exclusive of the endpoint values). Call with
 * idum a negative integer to initialize; thereafter, do not alter idum between
 * successive deviates in a sequence. RNMX should approximate the largest
 * floating value that is less than 1.*/
class Uni_Rnd: public nw::_Random {
	public:
		/** @brief Seed random generator with current time*/
		Uni_Rnd(){seed = long(time(0)) * -1;}
		/** @brief Implementation of the _Random::get_Uni_Rnd() method
		 * @return Uniform random deviate between 0.0 and 1.0*/
		double get_Uni_Rnd();

	private:
		long seed;				//!< Random generator seed
		/** @brief generates uniform random variable
		 * @return uniform random deviate between 0.0 and 1.0*/
		double uni_Rnd(long*);
	};
}

#endif /* RANDOM_GENERATOR_H_ */
