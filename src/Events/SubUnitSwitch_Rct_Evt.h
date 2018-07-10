#ifndef SUBUNITSWITCH_RCT_EVT_H
#define SUBUNITSWITCH_RCT_EVT_H

#include "Reaction_Evt.h"
#include "../Species/Channel_Spc.h"

namespace nw {

using namespace std;

/** @brief Subunit switch reaction event
 *
 * Coordinates reaction events that change the state of a subunit. Whenever a subunit
 * becomes activated the Channl_Spc has to be notified, since the number of open channels
 * depends on the number of active subunits.*/
class SubUnitSwitch_Rct_Evt: public Reaction_Evt {

public:
 	/** @brief Constructor
	*  	@param id _Event ID
	*	@param name _Event name
	*	@param k _Event rate constant
	* 	@param vvc _Event voxel vector
	* 	@param sc_vec Reaction_Evt state change vector
	* 	@param rg _Event pointer to a Random_Generator.
	* 	@param act_su_id Active subunit id
	* 	@param ch_spc_id Channel species id*/
	SubUnitSwitch_Rct_Evt(long id, string name, double k, vector<long> sc_vec, long act_su_id,
							long ch_spc_id, VoxelVector vvc, Uni_Rnd* rg):
								Reaction_Evt(id, name, k ,sc_vec,vvc,rg){
		this->act_su_id = act_su_id;
		this->ch_spc_id = ch_spc_id;
	}
	/** Destructor*/
	virtual ~SubUnitSwitch_Rct_Evt(){};

	/** @brief Implementation of the _Event::execute() method
	 *
	 * Whenever a subunit switch reaction event occurs, a channel subunit switches between
	 * an activated state and an inactivated state. This affects the state of a channel and
	 * thus needs to be communicated. This happens via a typecast of the _Species pointer to
	 * its more specific derivative Channel_Spc (identified via the ch_spc_id attribute).
	 * The result of the typecast allows for the call of public functions that are only
	 * accessible via the Channel_Spc interface. Consequently the function
	 * Channel_Spc::activate_Subunit() or Channel_Spc::inactivate_Subunit() can be called.*/
	void execute();

private:
	Channel_Spc* chs;				//!< Pointer to the downcasted Channel_Spc
	long act_su_id;					//!< _Species id of the activated subunit
	long ch_spc_id;					//!< _Species id of the Channel_Spc

	/** @brief Updates the referenced channel Species every time this event is executed.*/
	void update_Channel_Spc();
};

}

#endif /* SUBUNITSWITCH_RCT_EVT_H */
