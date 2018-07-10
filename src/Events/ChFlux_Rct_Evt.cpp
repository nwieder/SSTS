#include "ChFlux_Rct_Evt.h"
#include <cmath>

namespace nw{

void ChFlux_Rct_Evt::init(){
	educt_vec.push_back(ch_spc_id);
	c=k;
	update(0);
}

double ChFlux_Rct_Evt::update(double last_tau){

	double minTau = INFINITY;
	double actTau;

//	run through the tv vector
	for (long i = 0; i < (long) tv_vec.size(); i++) {
//		check for voxel that have been affected by last executed event
		if (tv_vec.at(i).v->get_dirty_flag()) {
			actTau = calc_tau(i);
//			find smallest tau value (->next voxel) for this event
			if (actTau < minTau) {
				minTau = actTau;
				nextVoxel = i;
			}
//		this voxel has not been affected by last event
		} else {
//			adjust tau value to global times scale
			tv_vec.at(i).t -= last_tau;
//			find smallest tau value (->next voxel) for this event
			if (tv_vec[i].t < minTau) {
				minTau = tv_vec[i].t;
				nextVoxel = i;
			}
		}
	}
	this->dirty_flag = false;

	return minTau;
}

double ChFlux_Rct_Evt::calc_tau(long vid){
	tv_vec[vid].t = -log(rg->get_Uni_Rnd()) / (tv_vec.at(vid).v->get_state_vec()->
					at(educt_vec.at(0))->get_n_molecules() * c);
	return tv_vec[vid].t;
}
} //namespace nw
