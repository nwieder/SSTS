#include "SubUnitSwitch_Rct_Evt.h"
#include <math.h>
#include <iostream>

using namespace std;
using namespace nw;

void SubUnitSwitch_Rct_Evt::execute(){

	try{
//		execute event by using the state change vector
		tv_vec[nextVoxel].v->update_state(sc_vec);

//		set dirty flag to indicate that dependent events have to be updated properly
		for(long i = 0; i < (long)dep_list.size(); i++){
			dep_list[i]->set_flag(true);
		}

//		update the activation states of the subunits of chs
		chs = dynamic_cast<Channel_Spc*>(tv_vec[nextVoxel].v->get_state_vec()->at(ch_spc_id));
		update_Channel_Spc();
	}
	catch(exception& e){
		cout << "SubuintSwitch_Rct_evt::execute(): " << e.what();
		}
	}

void SubUnitSwitch_Rct_Evt::update_Channel_Spc(){

	if (sc_vec.at(act_su_id) > 0){
		long r_chid = (long)(rg->get_Uni_Rnd() * chs->get_n_Activable_Ch());
		chs->activate_Subunit(r_chid);
	}else if(sc_vec.at(act_su_id) < 0){
		long r_chid = (long)(rg->get_Uni_Rnd() * chs->get_n_Inactivable_Ch());
 		chs->inactivate_Subunit(r_chid);
	}else{
		cout << "ERROR: The SubUnitActSwitch_Rct_Evt state change vector doesn't suit the"
				" assigned active_subUnit_Species_ID. Please check your input!" << endl;
	}
}

