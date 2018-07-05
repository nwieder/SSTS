#include "Border_Vxl.h"
#include <iostream>
#include <math.h>

namespace nw{
void Border_Vxl::update_state(vector<long> sc_vec)
{
	this->dirty_flag = true;

//	do not update the state vector of Border_vxl!
	for (size_t i = 0; i < sc_vec.size(); ++i) {
		if (sc_vec[i] != 0) {
			state_vec[i]->set_dirty_flag(true);
		}
	}
}
} // namespace nw
