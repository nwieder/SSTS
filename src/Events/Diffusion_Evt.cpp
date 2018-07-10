#include <math.h>
#include <iostream>
#include <exception>

#include "Diffusion_Evt.h"

namespace nw{

void Diffusion_Evt::init(){

//	calculate the correct diffusion probability constant according to the voxel volume
	this->c = (k*1e-3)/pow(tv_vec[0].v->get_box_length()*1e6,2);

//	build state change vector
	build_sc_vec();

//	update the first time
	update(0);
}

void Diffusion_Evt::execute(){
//	update diffusion origin voxel with the inverse state change vector
	tv_vec[nextVoxel].v->update_state(sc_vec);

//	choose diffusion partner
	double r = rg->get_Uni_Rnd();
	double d; // temp
	for (size_t i = 0; i < tv_vec[nextVoxel].v->get_diff_vec()->size(); ++i){
		d = (double)(i+1)/tv_vec[nextVoxel].v->get_diff_vec()->size();
		if(r-d < 0){
			tv_vec[nextVoxel].v->get_diff_vec()->at(i)->update_state(inv_sc_vec);
			break;
		}
	}

//	set dirty flag to indicate that dependent events have to be updated properly
	for(size_t i = 0; i < dep_list.size(); i++){
		dep_list[i]->set_flag(true);
	}
}

double Diffusion_Evt::update(double last_tau){

	double minTau = INFINITY, actTau;

//	check if this event depends on the last executed event
	if (this->dirty_flag) {
//		run through the tv vector
		for (size_t i = 0; i < tv_vec.size(); ++i) {
//			check for voxel that have been affected by last executed event
			if (tv_vec[i].v->get_dirty_flag()) {
				actTau = calc_tau(i);
//				find smallest tau value (->next voxel) for this event
				if (actTau < minTau) {
					minTau = actTau;
					nextVoxel = i;
				}
//			voxel has not been affected by last event
			} else {
//				adjust tau value to global times scale
				tv_vec[i].t -= last_tau;
//				find smallest tau value (->next voxel) for this event
				if (tv_vec[i].t < minTau) {
					minTau = tv_vec[i].t;
					nextVoxel = i;
				}
			}
		}
//  event has not been affected by last event
	} else {
//		adjust tau values to global times scale
		for (size_t i = 0; i < tv_vec.size(); ++i) {
			tv_vec[i].t -= last_tau;
			if (tv_vec[i].t < minTau) {
				minTau = tv_vec[i].t;
				nextVoxel = i;
			}
		}
	}

	this->dirty_flag = false;
	return minTau;
}

void Diffusion_Evt::build_sc_vec(){
//	build sc_vec
	sc_vec.assign(tv_vec[0].v->get_state_vec()->size(),0);
	inv_sc_vec.assign(tv_vec[0].v->get_state_vec()->size(),0);
	sc_vec[diff_spec_id] = -1;
	inv_sc_vec[diff_spec_id] = 1;

}

double Diffusion_Evt::calc_tau(long vid){
//	adapt the c value to the number of diffusion sites!
	double c_adapted =  c * tv_vec.at(vid).v->get_diff_vec()->size();
//	calculate tau
	tv_vec[vid].t = -log(rg->get_Uni_Rnd()) / (tv_vec[vid].v->get_state_vec()->
			at(diff_spec_id)->get_n_molecules() * c_adapted);

	return tv_vec[vid].t;
}

double Diffusion_Evt::get_a(){
	double a_ges = 0;
	for(long i = 0; i < (long) tv_vec.size(); i++){
		a_ges += c*tv_vec[i].v->get_state_vec()->at(diff_spec_id)->get_n_molecules() *
				tv_vec[i].v->get_diff_vec()->size();
	}
	return a_ges;
}
} // namespace nw
