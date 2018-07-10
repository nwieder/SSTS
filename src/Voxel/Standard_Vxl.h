#ifndef STANDARD_VXL_H_
#define STANDARD_VXL_H_

#include  <vector>
#include "_Voxel.h"
#include "../Species/_Species.h"

namespace nw{

using namespace std;
/** @brief Standard voxel implementation
 *
 * Most importantly it implements the \c update_state() function of its mother class,
 * so that the state vector (state_vec) can be modified by events (in contrast to Border_Vxl).*/
class Standard_Vxl: public _Voxel {
public:
	/** @brief Constructor
	 *
	 * @param id _Voxel ID
	 * @param box_length _Voxel box length
	 * @param state_vec _Voxel state vector*/
	Standard_Vxl(long id,double box_length,SpeciesVector state_vec):_Voxel(id,box_length,state_vec){}
	/** @brief Destructor*/
	~Standard_Vxl(){};
	/** @brief Implementation of the _Voxel::update_state() method
	 * @param sc_vec State change vector of firing event*/
	void update_state(vector<long>);
};
} // namespace nw

#endif /* STANDARD_VXL_H_ */
