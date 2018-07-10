#include <math.h>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <exception>

#include "../Voxel/Border_Vxl.h"
#include "Reaction_Evt.h"

namespace nw{

void Reaction_Evt::init(){

//	build educt vector
	build_educt_vector();

//	calculate the correct probability rate constant depending on the reaction order.
	if (educt_vec.size() == 1){
		this->c = k;
	}else if(educt_vec.size() == 2){
		this->c = k/(tv_vec[0].v->get_volume()*N_AVO*1e-6);
	}

//	update the Reaction Event the first time
	update(0);
}

void Reaction_Evt::execute(){
	try{
//		execute event by using the state change vector
		tv_vec[nextVoxel].v->update_state(sc_vec);

//		set dirty flag to indicate that dependent events have to be updated
		this->dirty_flag = true;
		for(size_t i = 0; i < dep_list.size(); i++){
			dep_list[i]->set_flag(true);
		}
	}
	catch(exception& e){
		cout << "Reaction_Evt::execute(): " << e.what();
	}
}

double Reaction_Evt::update(double last_tau){

	double minTau = INFINITY;

//	check if this event depends on the last executed event
	if(this->dirty_flag){
//		run through the tv vector
		for (size_t i = 0; i < tv_vec.size(); ++i){
//			check for voxel that have been affected by last executed event
			if (tv_vec[i].v->get_dirty_flag()){
//				find smallest tau value (->next voxel) for this event
				if(calc_tau(i) < minTau){
					minTau = tv_vec[i].t;
					nextVoxel = i;
				}
//			voxel has not been affected by last event
			} else {
//				adjust tau value to global times scale
				tv_vec[i].t -= last_tau;
//				find smallest tau value (->next voxel) for this event
				if(tv_vec[i].t < minTau){
					minTau = tv_vec[i].t;
					nextVoxel = i;
				}
			}
		}
//  event has not been affected by last event
	} else  {
//		adjust tau values to global times scale
		for (size_t i = 0; i < tv_vec.size(); ++i) {
			tv_vec[i].t -= last_tau;
			if(tv_vec[i].t < minTau){
				minTau = tv_vec[i].t;
				nextVoxel = i;
			}
		}
	}

	this->dirty_flag = false;
	return minTau;
}

double Reaction_Evt::get_a(){
	double a_ges = 0;
	for(size_t i = 0; i < tv_vec.size(); ++i){
		double a = c;
		for (size_t j = 0; j < educt_vec.size(); ++j){
			a *= tv_vec[i].v->get_state_vec()->at(educt_vec[j])->get_n_molecules();
		}
		a_ges += a;
	}
	return a_ges;
}

void Reaction_Evt::build_educt_vector(){
	for(size_t i = 0; i < sc_vec.size(); ++i){
		if(sc_vec[i] < 0)
			educt_vec.push_back(i);
	}
}

double Reaction_Evt::calc_tau(long vid){

// 	make sure the educt vector is valid -> only first -and second order reactions are allowed
	if(educt_vec.size() > 0 && educt_vec.size() <= 2){
		double a = c;
		for (size_t i = 0; i < educt_vec.size(); ++i){
			a *= tv_vec[vid].v->get_state_vec()->at(educt_vec[i])->get_n_molecules();
		}
		tv_vec[vid].t = -log(rg->get_Uni_Rnd()) / a;

		return tv_vec[vid].t;
	} else {
		cout << "ERROR: Only first and second order reactions are allowed! "
				"Check sc_vec of:" << this->name <<endl;
		exit(0);
	}
}
}// namespace nw
