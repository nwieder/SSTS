#ifndef _EVENT_H_
#define _EVENT_H_

#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include "../Voxel/_Voxel.h"
#include "../Random/Uni_Rnd.h"

namespace nw{

using namespace std;
/** @brief typedefs of Vector related structures*/
typedef vector<_Voxel*> VoxelVector;
typedef VoxelVector::iterator VoxelIterator;

/** @brief Abstract base class for system state transitions (events).
 *
 * A system state transition is called event and is defined by a propensity function, a
 * state change vector and a dependency list. Propensity functions are derived from rate
 * constants that result either from experimental observations and/or fundamental physical
 * laws. A state change vector represent the stoichiometry of an event and represents the
 * numerical changes in molecular populations caused by the respective event. The dependency
 * list is an implementation of a dependency graph that defines how events influence
 * each other. This data structure is crucial to implement Gibbson and Bruck's optimized
 * update procedure.
 **/
class _Event {
public:
	/**@brief Constructor
	 *
	 * @param id Event ID
	 * @param name Event name
	 * @param k Rate constant
	 * @param vvc Voxel vector containing all _Voxels where this _Event can occur.
	 * @param rg Random generator that produces uniform distributed random numbers*/
	_Event(long id, string name, double k, VoxelVector vvc, Uni_Rnd* rg):
		id(id),
		name(name),
		k(k),
		rg(rg),
		dirty_flag(true){

//		add self-reference to dependency list.
		this->dep_list.push_back(this);
//		check if all voxel in the assigned voxel vector have the same size
		for (size_t i = 0; i < tv_vec.size(); i++){
			if(vvc[i]->get_volume() != vvc[0]->get_volume()){
				cout << "ERROR: Voxel sizes in Event: " << this->name << " differ in size. "
						"You either have to create another Event or resize the voxel to one size"
						<< std::endl;
				break;
			}
		}

//		create the tau voxel structure container that assigns a tau value to each _Voxel
//		in the _Voxel vector
		tv_vec.resize(vvc.size());
		for(size_t i = 0; i < tv_vec.size(); i++){
			tv_vec[i].v = vvc[i];
			tv_vec[i].t = 0.0;
		}
	}
	/**@brief Destructor*/
	virtual ~_Event(){};
	/**@brief Updates the event.
	 * @return New tau value for this _Event
	 * @param last_tau Tau value of previous firing _Event
	 *
	 * Implementation of the update procedure. The tau value represents the time that has
	 * to pass until an event fires again. It has to be updated during every simulation step.
	 * An _Event with dirty_flag = @c TRUE has to generate a new random number to recalculate
	 * a tau value. An _Event with dirty_flag = @c FALSE is adapted to the global time scale
	 * by subtracting the last tau from their (necessarily larger) current tau. No negative
	 * tau values should be returned!\n
	 * \n
	 * While updating the tau values, the method determines the smallest updated tau value
	 * and therefore the id of the next voxel to be exectuted.
	 */
	virtual double update(double last_tau) = 0;
	/**@brief Event execution function.
	 *
	 * Executes (fires) this _Event. It looks in the tv_struct for the voxel with the smallest
	 * associated tau value and calls its update_state() function.*/
	virtual void execute() = 0;
	/**@brief Event initialization.*/
	virtual void init() = 0;
	/**@brief returns current _Event propensity a.
	 * @return _Event propensity a.
	 *
	 * This function returns the a_value of the event (inside all voxel in the tv_vec) at
	 * the current system state. This information is used for analytical purposes*/
	virtual double get_a() = 0;
	/**@brief Adds an _Event pointer to the dependency list.
	 * @param e: pointer to event that depends on this event
	 *
	 * Is called during the generation of a dependency graph that connects dependent events
	 * with each other to speed up the update procedure. An _Event A depends on another
	 * _Event B, if at least one of the educts of A is also a product of B.*/
	void add_dep_list(_Event* e) {dep_list.push_back(e);}
	/**@brief set the dirty flag.
	 * @param b: boolean that is assigned to dirty_flag.
	 *
	 * As a _Voxel, every _Event has a dirty flag to indicate weather it has to be updated or not.
	 */
	void set_flag(bool b){dirty_flag=b;}

	// getter and setter
	string get_name(){return name;}
	long get_id(){return id;}
	double get_tau(){return tv_vec[nextVoxel].t;}
	vector<long> get_sc_vec(){return sc_vec;}
	vector<_Event*> get_dep_list(){return dep_list;}

protected:
/** @brief tau-voxel-structure (tv_struct) is a structure that associates a tau value with a _Voxel.
 *
 * @c v points to a _Voxel, while @c t is the corresponding tau value.
 */
	struct tv_struct{
		_Voxel*				v;									//!< pointer to an existing voxel
		double 				t;									//!< corresponding tau value
	};

	long id; 					//!< ID
	string name;				//!< Name
	double k; 					//!< Rate constant
	vector<long> sc_vec; 		//!< State change vector.
	vector<tv_struct> tv_vec; 	//!< Tau voxel vector. Container for tv_structs
	Uni_Rnd* rg; 				//!< Uniform random number generator
	long nextVoxel;				//!< Index of next _Voxel to be executed

	double c; 					//!< Adapted _Event rate constant.
	bool dirty_flag; 			//!< indicates whether tau value has to be recalculated
	vector<_Event*> dep_list; 	//!< Dependency vector that stores references to _Events

};
}// namespace nw

#endif /* EVENT_H_ */
