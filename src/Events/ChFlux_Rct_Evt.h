#include "Reaction_Evt.h"

namespace nw{

using namespace std;
/**@brief Channel flux reaction event.
 *
 * This event is derived from the reaction event and introduces the flux of a molecular
 * species through a channel. It differs from an ordinary reaction event due to the fact
 * that the educt (the channel) is not modified by the event itself. Thus the educt vector
 * has to be generated manually in the init() function.*/
class ChFlux_Rct_Evt: public Reaction_Evt{

public:
	ChFlux_Rct_Evt(long id, string name, double k, vector<long> sc_vec, long ch_spc_id,
			VoxelVector vvc, Uni_Rnd* rg)
	:Reaction_Evt(id,name,k,sc_vec,vvc,rg){
		this->ch_spc_id = ch_spc_id;
	}
	virtual ~ChFlux_Rct_Evt(){};
	/** @brief Implementation of the _Event::init() method
	 *
	 * The educt of this first order reaction is not modified by the event itself. The educt
	 * vector has therefore to be generated manually.
	 * */
	void init();
	/** @brief Implementation of the _Event::update method
 	 * @return New tau value for this _Event
	 * @param last_tau Tau value of previous firing _Event
	 *
	 * The channel flux reaction event differs from its mother class in its update function.\n
	 * \n
	 * Note: This event cannot be part of the dependency graph, because there is no event
	 * that causes a new number of open channels. A new tau value is calculated every
	 * simulation step. This is a sub-optimal implementation. One option is to manually add
	 * this event to the dependency graph of the subUnitSwitch reaction.*/
	double update(double);

private:
	double calc_tau(long vid);
	long ch_spc_id;
};
}
