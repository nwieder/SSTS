#ifndef VOXEL_H_
#define VOXEL_H_

#include "../Species/_Species.h"
#include <iostream>
#include <math.h>

namespace nw{

using namespace std;

/** @brief typedefs of Species related structures*/
typedef vector<_Species*> SpeciesVector;
typedef SpeciesVector::iterator SpeciesIterator;

/**	@brief Abstract base class for voxel (discrete building blocks of the simulation volume)
 *
 * A _Voxel is characterized by its spatial extend (volume), its state vector and its
 * vicinity relations. To access the state vector, the function update_state() updates the
 * state according to a state_change_vector (sc_vec). It is cruical that the sc_vec has the
 * same format as the state vector of the _Voxel. Since only _Voxels affected by the previous
 * _Event need to be updated, a dirty_flag system in analogy to the dirty_flag system in
 * _Event exists. To define vicinity relations of different _Voxel for Diffusion _Events,
 * each _Voxel contains a reference list to all adjacent _Voxels (diff_vec).
 * */
class _Voxel {
public:

	/** @brief Constructor
	 *
	 * @param id Voxel ID
	 * @param box_length defining cubic voxel dimension
	 * @param state_vec Species vector representing the state of this _Voxel.*/
	_Voxel(long id, double box_length, SpeciesVector state_vec):
		id(id),
		box_length(box_length),
		dirty_flag(true),
		state_vec(state_vec){
		// convert m^3 to liter (factor 1e3)
		this->volume = pow(this->box_length,3)*1e3;
		// set the number of molecules of all species
		for (long i = 0; i < (long)state_vec.size();i++) {
			this->state_vec[i]->set_n_molecules(this->volume);
		}
	}
	/** @brief Destructor*/
	virtual ~_Voxel(){
		for (size_t i = 0; i < state_vec.size(); ++i){if(state_vec.at(i)){
			delete state_vec.at(i);state_vec.at(i)=NULL;}
		}
	};
	/** @brief Update the state vector of voxel.
	 * @param sc_vec State change vector of firing event
	 *
	 * If an event changes the molecular number of _Species in a _Voxel, it uses this
	 * function to update the state vector with its state change vector.*/
	virtual void update_state(vector<long> sc_vec) = 0;
	/** @brief Clean dirty flag (set dirty_flag = false).
	 *
	 * The dirty flag is indicates if this _Voxel needs to be updated. Every time an event
	 * occurs inside this _Voxel, the dirty flag is set to @c TRUE and indicates the tau
	 * values of this _Voxel needs to be updated (this concerns only _Events that depend on
	 * the previous fired _Event). */
	void clean(){dirty_flag = false;}
	/** @brief Add voxel to diffusion vector.
	 *
	 * The diffusion vector defines the spatial connection between a set of voxel. It serves
	 * as lookup table for diffusion events to decide which neighbour voxel becomes the
	 * diffusion partner. The add_neighbour function is called after all Voxel have been
	 * created.*/
	void add_neighbour(_Voxel* v){diff_vec.push_back(v);}

	// Getter and setter
	bool get_dirty_flag(){return dirty_flag;}
	double get_volume(){return volume;}
	double get_box_length(){return box_length;}
	long get_id(){return id;}
	SpeciesVector* get_state_vec(){return &state_vec;}
	vector<_Voxel*>* get_diff_vec(){return  &diff_vec;}

protected:
	long id; //!< _Voxel ID
	double box_length; //!< _Voxel box length [micro m]
	double volume; //!< _Voxel volume [l]
	bool dirty_flag; //!< Indicates if the state vector of this _Voxel has been changed
	SpeciesVector state_vec; //!< Reference list for species, defining the voxel state.
	vector<_Voxel*> diff_vec; //!< Reference list of diffusion neighbors of this _Voxel
};
}//namespace nw
#endif /* VOXEL_H_ */
