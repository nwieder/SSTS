#ifndef _SYSTEM_H_
#define _SYSTEM_H_

namespace nw {

/** @brief Abstract base class for the implementation of simulation algorithms
 *
 *	Algorithms integrated in this software framework are derived from _System.
 *	It solely requires the existence of the function go() that executes the algorithm.*/
class _System {

public:
	_System(){};
	virtual ~_System(){};

	/** @brief executes simulation algorithm
	 * 	@param n_it Current number of simulation run (required for output file names to
	 * 	distinguish between multiple trajectories resulting from multiple simulation runs
	 * 	during one program call.)
	 *  */
	virtual void go(long n_it) = 0;
};

}

#endif /* _SYSTEM_H_ */
