#ifndef STANDARD_SPC_H_
#define STANDARD_SPC_H_

#include <iostream>
#include "_Species.h"

namespace nw{
/** @brief Basic implementation of a standard moecular species
 *
 * Basic unit of a Gillespie_Sys. Every _Voxel has a state_vector that consists of molecular
 * _Species. A _Species is defined by its current number of molecules, its id and name, and
 * a dirty_flag. The dirty flag indicates that the species has been updated during the
 * previous _Event.
 */
class Standard_Spc: public _Species {
public:
	/**@brief Constructor
	 * @param id _Species ID.
	 * @param name _Species name
	 * @param initial_conc _Species Initial concentration
	 */
	Standard_Spc(long id, std::string name, double initial_conc):_Species(id,name,initial_conc){}
	/**@brief Destructor */
	~Standard_Spc(){}
	/** @brief Implementation of the _Species::get_n_molecules() method*/
	long get_n_molecules() {return n_molecules;}
	/** @brief Implementation of the _Species::mod_n_molecules() method*/
	void mod_n_molecules(long n){
		n_molecules += n; if(n!=0){dirty_flag = true;}}
	/** @brief Implementation of the _Species::copy() method*/
	_Species* copy(){
		_Species* s = new Standard_Spc(id,name,initial_conc);
		return s;}
};
}

#endif /* STANDARD_SPC_H_ */
