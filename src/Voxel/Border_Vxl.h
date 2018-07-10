#ifndef BORDER_VXL_H_
#define BORDER_VXL_H_

#include  <vector>
#include "_Voxel.h"
#include "../Species/_Species.h"

namespace nw {

/** @brief Specialized _Voxel that can be used to define a system with equilibrium as
 * 	boundary condition
 *
 * The equilibrium boundary condition is an open system that is in equilibrium with itself
 * and the surroundings. Border voxel serve as neighbor voxel for marginal voxel. Their
 * state vector represents an equilibrium state of the considered system and exchanges
 * non-stationary molecules with the main simulation voxels. The only exception is that
 * the state of the border voxel is constant and thus drives the main simulation voxel
 * towards the initial equilibrium. This plays an particular important role when conducting
 * ion channels are introduced to the model. Ions introduced via the channel would accumulate
 * inside the main simulation voxel and thus disturb the relaxation of the system towards
 * its equilibrium state.
 */
class Border_Vxl: public _Voxel {
public:
	/** @brief Constructor
	 *
	 * @param id _Voxel ID
	 * @param box_length _Voxel box length
	 * @param state_vec _Voxel state vector*/
	Border_Vxl(long id, double box_length, SpeciesVector state_vec) :
		_Voxel(id, box_length, state_vec) {}
	/** Destructor */
	~Border_Vxl() {}

	/** @brief Implementation of the _Voxel::update_state() method
	 * @param sc_vec State change vector of firing event
	 *
	 * The state vector of the Border voxel is constant. This function thus does not alter
	 * the number of molecules. However, it does set respective dirty_flags.*/
	void update_state(std::vector<long> sc_vec);
};

}	// namspace nw

#endif /* BORDER_VXL_H_ */
