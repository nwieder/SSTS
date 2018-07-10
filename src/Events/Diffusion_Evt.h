#ifndef DIFFUSION_EVT_H_
#define DIFFUSION_EVT_H_

#include <vector>
#include <string>
#include "../Voxel/_Voxel.h"
#include "_Event.h"

namespace nw{

using namespace std;
/**@brief Realization of diffusion between two voxels.
 *
 * The Diffusion_Event alters the stae vector of two adjacent Voxel. The execute() function thus
 * needs a second random number to determine the diffusion partner from the voxel diff list.*/
class Diffusion_Evt: public _Event {
public:
/** @brief Constructor of Diffusion Event.
*	@param id _Event ID
* 	@param name _Event name
* 	@param k _Event rate constant
* 	@param vvc vector of all voxel where this event can occur
* 	@param diff_spc_id indicates the id of the diffusing molecule species
* 	@param rg Pointer to the Random_Generator
**/
	Diffusion_Evt(long id, string name, double k, VoxelVector vvc,long diff_spc_id, Uni_Rnd* rg):
		_Event(id,name,k,vvc,rg){
		this->diff_spec_id = diff_spc_id;
		build_sc_vec();
	}
/**	Destructor*/
	virtual 			~Diffusion_Evt(){};

	/**@brief Diffusion implementation of _Event::update() method*/
	double update(double);
	/**@brief Diffusion implementation of _Event::execute() method
	 *
	 * Two voxels have to be updated. Decrease the number of molecules by one inside the
	 * origin voxel and increase the number of molecules by one in the target voxel. The
	 * target voxel is determined randomly from the diffusion vector that defines the
	 * vicinity relation of different voxel.
	 */
	void execute();
	/**@brief Diffusion implementation of _Event::init() method
	 * initializes a diffusion. State change vector is generated and the event propensity
	 * is calculated*/
	void init();
	/**@brief Diffusion implementation of _Event::get_a() method*/
	double get_a();

protected:
	vector<long>		inv_sc_vec;		//!< inverse sc_vec to update origin diff voxel
	long 				diff_spec_id;	//!< id of diffusion species

	/** @brief function to calculate next tau value of a voxel.
	 * @param vid: the Voxel id of the voxel which has to be updated
	 * @return new tau value*/
	double 				calc_tau(long);
	/** @brief builds the state change vector and the inverse state change vector of the
	 * diffusion event.
	 *
	 * Helper function to build an inverse sc_vector. Number of molecules in the origin
	 * voxel has to be decreased, while number of molecules in the target voxel has to be
	 * increased. Thus Diffusion_Evt requires two state change vectors.
	 */
	void 				build_sc_vec();		//!< builds state change vector using diff_spec_id
};
}//namespace nw

#endif /* DIFFUSION_EVT_H_ */
