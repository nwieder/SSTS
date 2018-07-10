#ifndef CHANNEL_SPC_H_
#define CHANNEL_SPC_H_

#include "_Species.h"
#include <iostream>
#include <vector>

namespace nw {

using namespace std;
/** @brief Realization of channel proteins comprised of a fixed number of subunits that
 * open whenever a predefined number of those subunits are in an active state.
 *
 * This species works a little different from others. It is an @"imaginary@" _Species. It
 * only subsumes a certain number of other Species (here subunits). The molecular count
 * only changes if there are enough active subunits. A channel only exists in this implemen-
 * tation if it is open. To keep track of active subunits, there is an event called subunit
 * switch reaction that defines the transition between an inactive and an active subunit.
 * If this event occurs it typecasts the Species at the channel species id (class attribute
 * to be defined in the input file: ch_spc_id) to a channel species and calls activate_subunit()
 * resp. inactivate_subunit(). \n
 * \n
 * Note: To avoid the typcast which can cause instabilities, it might  be beneficial to
 * extend the existing framework with an event handling system.
 * */
class Channel_Spc: public _Species {

public:
	/** @brief Constructor of Channel Species.
	 * @param id _Species ID
	 * @param name _Species name
	 * @param initial_conc _Species initial concentration
	 * @param n_Subunits Number of existing subunits the channel is build of.
	 * @param n_Subunits_To_Open Required number of active subunits to open the channel
	 */
	Channel_Spc(long id, string name, double initial_conc, int n_Subunits, int n_Subunits_To_Open):
		_Species(id,name,initial_conc),
		n_Subunits(n_Subunits),
		n_Subunits_To_Open(n_Subunits_To_Open),
		n_active_Ch(0),
		n_fully_activated_Ch(0),
		n_fully_inactivated_Ch(0){}
	/** Destructor*/
	~Channel_Spc(){};
	/** @brief Implementation of the _Species::get_n_molecules() method.
	 * @return Number of open channel.
	 *
	 * Only the number of open channels are returned. The total number of channels includes
	 * closed non-conducting channels.*/
	long get_n_molecules() {return n_active_Ch;}
	/** @brief Implementation of the _Species::mod_n_molecules() method.
	 * @param n Summand that is added to the current number of molecules.
	 *
	 * This function is empty, because at the moment there is no way how the number of
	 * channels could be modified but through the subunitswitch reaction which calls
	 * activate_Subunit() resp. inactivate_Subunit()*/
	void mod_n_molecules(long n){}
	/** @brief Implementation of the _Species::copy() method.*/
	_Species* copy(){
		_Species* s = new Channel_Spc(id,name,initial_conc,n_Subunits,n_Subunits_To_Open);
		return s;}
	/** @brief Implementation of the _Species::set_n_molecules() method.
	 *
	 * Initialization of the channel species. A vector with a length equal to the number of
	 * existing channels is created. Each element of this vector holds the number of active
	 * subunits for one particular channel protein. If the number of active subunits inside
	 * one vector field becomes equal or bigger than the number of subunits required to open
	 * a channel, the number of activated channel is increased by one. All channel are initially
	 * closed. */
	void set_n_molecules(double volume){
	n_molecules = volume*1e-6*N_AVO*initial_conc;
	ch_Vec.assign(n_molecules,0);
	this->n_fully_inactivated_Ch = n_molecules;}

//	Getter and setter
	long get_n_Activable_Ch(){return n_molecules - n_fully_activated_Ch;}
	long get_n_Inactivable_Ch(){return n_molecules - n_fully_inactivated_Ch;}

	/**@brief Activates a subunit if the SubUnitSwitch Reaction Event increases the number
	 * of active subunits.
	 * @param rnd_id Random number to pick a channel whose subunit will be activated.
	 *
	 * If a subunit is activated, first of all a random number has to be generated (see
	 * SubUnitSwitch_Rct_evt) to pick one of the existing channels whose subunit is activated.\n
	 * \n
	 * Note: It is not possible to increase the number of active channel subunits if all
	 * subunits are in the active state. This makes this function a little bit more complex.*/
	void activate_Subunit(long);
	/** @brief Inactivates a subunit if the SubUnitSwitch Reaction Event decreases the
	 * number of active subunits.
	 * @param rnd_id random number to pick a channel whose subunit will be inactivated.
	 *
	 * If a subunit is inactivated, first of all a random number has to be generated (see
	 * SubUnitSwitch_Rct_evt) to pick one of the existing channels whose subunit is
	 * inactivated.\n
	 * \n
	 * Note: It is not possible to decrease the number of active channel subunits if all
	 * subunits are inactivated. This makes this function a little bit more complex.*/
	void inactivate_Subunit(long);

private:
	int n_Subunits;				//!< Number of subunits of this channel species.
	int n_Subunits_To_Open; 	//!< Number of activated subunits required to open channel.
	long n_active_Ch; 			//!< Number of currently active channels
	long n_fully_activated_Ch; 	//!< Number of activated channels (no activation possible).
	long n_fully_inactivated_Ch;//!< Number of inactivated channels (noinactivation possible)

	vector<long> ch_Vec; 			//!< Vector representing number of active channel subunits
	vector<long>::iterator ch_it; 	//!< Vector iterator for channel vector
};

} // namespace nw

#endif /* CHANNEL_SPC_H_ */
