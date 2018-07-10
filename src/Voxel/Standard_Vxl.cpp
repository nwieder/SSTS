#include "Standard_Vxl.h"
#include <iostream>
#include <math.h>

namespace nw{

void Standard_Vxl::update_state(vector<long> sc_vec)
{
	if (sc_vec.size() == state_vec.size()) {
	//	update the state vector of voxel using the assigned state change vector
		for (size_t i = 0; i < state_vec.size(); ++i) {
			state_vec[i]->mod_n_molecules(sc_vec[i]);
		}
		this->dirty_flag = true;
	} else {
	//	Debug information: Throw error if sc_vec.size() != state_vec
		cout << "ERROR: state change vector and state vector differ in size";
	}
}
} // namespace nw
