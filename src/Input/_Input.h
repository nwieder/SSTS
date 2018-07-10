#ifndef INPUT_H_
#define INPUT_H_

#include "../System/_System.h"

namespace nw {

/** @brief Abstract base class for input procedures.
 *
 * Derivatives of \c _Input can implement different input interfaces to ensure
 * compatibility with different input formats*/
class _Input {
public:
	virtual 				~_Input(){};
	/** @brief Translates input files into a _System
	 * @returns _System based on an input file.*/
	virtual nw::_System* 	get_System() = 0;

protected:
	nw::_System* 			s; //< _System that is created from an input file

};

}

#endif /* INPUT_H_ */
