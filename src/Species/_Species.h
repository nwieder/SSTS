#include <string>

#ifndef _SPECIES_H_
#define _SPECIES_H_

namespace nw{

using namespace std;

/** Definition of Avogadro's constant*/
static const double N_AVO = 6.022e23;

/** @brief Abstract base class for the molecular species of a system.*/
class _Species {

public:

	/** @brief Constructor
	 *
	 * @param id Species ID
	 * @param name Species name
	 * @param init_conc initial concentration
	 */
	_Species(long id,string name, double init_conc):
		id(id),
		name(name),
		dirty_flag(true),
		initial_conc(init_conc) {}
	/** @brief Destructor */
	virtual ~_Species(){};
	/** @brief Get current number of molecules
	 *
	 * @return current number of molecules*/
	virtual long get_n_molecules() = 0;
	/** @brief Modify number of molecules (n_molecules).
	 * @param n Summand that is added to the current number of molecules.
	 *
	 * If an event occurs, this function is called to update the number of molecules
	 * based on the state change vector of the firing _Event. If n is positive, the number
	 * of molecules is increased, if it is negative the number of molecules in decrease. */
	virtual void mod_n_molecules(long n) = 0;
	/** @brief Copy method.
	 * @return Reference to an object identical to this
	 *
	 * Copy method that generates a copy of this. It is is a virtual function to ensure
	 * that whenever this function is called, the most specific object (derived from this
	 * abstract base class) is copied. */
	virtual _Species* copy() = 0;
	/** @brief Set initial molecular count (n_molecules).
	 *
	 * Called in the constructor of every _Voxel to set the correct (_Voxel volume adapted)
	 * number of molecules. This makes it easier to define _Species and _Voxel in the
	 * input file.*/
	virtual void set_n_molecules(double volume){
		this->n_molecules = volume*1e-6*N_AVO*initial_conc;}

	// getter and setter
	long get_id(){return id;}
	string get_name(){return name;}
	double get_initial_conc(){return initial_conc;}
	bool get_dirty_flag(){return dirty_flag;}
	void set_dirty_flag(bool b) {dirty_flag = b;}


protected:
	long id; 				//!< Species ID
	string name;			//!< Species name
	bool dirty_flag; 		//!< Indicates whether this _Species needs to be updated or not
	long n_molecules;		//!< Current number of molecules
	double initial_conc;	//!< Initial concentration
	double volume;			//!< Volume of the _Voxel containing this _Species
};
}

#endif /* EVENT_H_ */
