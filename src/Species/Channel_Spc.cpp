#include "Channel_Spc.h"
#include <iostream>

namespace nw{

void Channel_Spc::activate_Subunit(long rnd_id){
	ch_it = ch_Vec.begin();

//	iterate to an activatable channel
	for (size_t i = 0; i <= size_t(rnd_id);++i){
		if(i!=0){ch_it++;}
		while(*ch_it == n_Subunits){
			ch_it++;
		}
	}

//	decrease number of completely inactivated Channel if it was a Channel with 0 active su
	if(*ch_it == 0){n_fully_inactivated_Ch--; dirty_flag = true;}
//	increase number of active subunits and check if the channel is open!
	if(++*ch_it == n_Subunits_To_Open){n_active_Ch++; dirty_flag = true;}
//	if the channel is completely activated, increase number of fully activated channels
	if (*ch_it == n_Subunits) {n_fully_activated_Ch++; dirty_flag = true;}
}

void Channel_Spc::inactivate_Subunit(long rnd_id){
	ch_it = ch_Vec.begin();
//	iterate to an inactivatable channel
	for (size_t i = 0; i <= (size_t)rnd_id;++i){
		if(i!=0){ch_it++;}
		while(*ch_it == 0){
			ch_it++;
		}
	}

//	decrease number of fully activated Channel if it was a Channel with maximum active su
	if(*ch_it == n_Subunits){n_fully_activated_Ch--; dirty_flag = true;}
//	decrease number of active subunits and check if the channel is open!
	if(*ch_it == n_Subunits_To_Open){n_active_Ch--; dirty_flag = true;}
//	if the channel is completely inactivated, increase number of fully inactivated channels
	if (--*ch_it == 0) {n_fully_inactivated_Ch++; dirty_flag = true;}
}
} // namespace nw
