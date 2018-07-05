#ifndef REACTION_H_
#define REACTION_H_

#include "_Event.h"
#include <vector>
#include <string>

namespace nw{

using namespace std;
/** @brief Implementation of the reaction of one ore two molecules
 *
 * In contrast to the Diffusion_Event the Reaction_Event alters the state vector of only
 * one Voxel per simulation step.*/
class Reaction_Evt: public _Event {
public:
	/** @brief Constructor
	*  	@param id _Event ID
	*	@param name _Event name
	*	@param k _Event rate constant
	* 	@param vvc _Event voxel vector
	* 	@param sc_vec: state change vector of the reaction event
	* 	@param rg _Event pointer to a Random_Generator.*/
	Reaction_Evt(long id, string name, double k, vector<long> sc_vec, VoxelVector vvc,
			Uni_Rnd* rg):
		_Event(id,name,k,vvc,rg){
		this->sc_vec = sc_vec;
	}
	/**Destructor*/
	virtual ~Reaction_Evt(){};
	/**	@brief Implementation of the _Event::update() method.
	 *  @param last_tau last tau value to update the system time.
	 *  @return The new tau value, so system can sort it right away.
	 *
	 * If dirty_flag is true, this event depends on the event that fired during the
	 * simulation step. For each voxel that has been modified, the _Event specific tau value
	 * has to be updated. All the other tau values have to be adapted to the global system
	 * time*/
	double update(double);
	/** @brief Implementation of the _Event::execute() method*/
	void execute();
	/** @brief Implementation of the _Event::init() method*/
	void init();
	/**@brief Implementation of the _Event::get_a() method
	 * @return a0 of this _Event
	 */
	double get_a();

protected:
	vector<long> educt_vec; 	//!< Educt vector holds pointer to educt species.

	/**@brief builds the educt vector.
	 *
	 * The educt vector holds pointer to each species that is an educt of this reaction.
	 * It facilitates the update procedure*/
	void build_educt_vector();
	/**@brief calculates a new tau value using a random number.
	 * The calc_tau function is dependent on the reaction order:
	 *	- 1. order: -ln(r)/a(j)(x,t) = -ln(r)/(educt * c(j))
	 *	- 2. order: -ln(r)/a(j)(x,t) = -ln(r)/(educt(1) * educt (2) * c(j))
	 *	only first and second order reactions are allowed!*/
	double calc_tau(long);

};
}// namespace nw
#endif /* REACTION_H_ */
