#ifndef RANDOM_H_
#define RANDOM_H_

namespace nw {

/** @brief abstract base class to define an interface for Random Generators**/
class _Random {
public:
	/** @brief Generate uniformly distributed random number between 0.0 and 1.0
	 * @return Uniformly distributed random number between 0.0 and 1.0*/
	virtual double 		get_Uni_Rnd() = 0;
	/** @brief Destructor */
	virtual 			~_Random(){};

private:
	double 				r;
};

}

#endif /* RANDOM_H_ */
