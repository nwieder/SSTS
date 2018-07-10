#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <vector>
#include <fstream>
#include <sstream>
#include "_System.h"
#include "../Events/_Event.h"
#include "../Voxel/_Voxel.h"

namespace nw {

using namespace std;

/** @brief Error message that is thrown, whenever the smallest tau value becomes negative.*/
static const string NEGATIVETAUMSG = "NEGATIVE TAU ERROR LAST EVENT ";
/** @brief typedefs of a _Event reference vector*/
typedef vector<_Event*> EventVector;
/** @brief typedefs of a _Event vector iterator*/
typedef EventVector::iterator EventIterator;
/** @brief typedefs of a _Voxel reference vector*/
typedef vector<_Voxel*> VoxelVector;
/** @brief typedefs of a _Voxel vector iterator*/
typedef VoxelVector::iterator VoxelIterator;

/**@brief Coordinates the stochastic simulation algorithm.
 *
 * The main simulation loop is defined in this class. To run the algorithm, all system
 * relevant data structures such as the _Event vector (evt_vec) and the _Voxel (vxl_vec)
 * vector are attributes. The whole algorithm is based on the abstract base classes that
 * define essential properties of each system component. The polymorph approach allows for
 * a great extensibility of these components.
 */
class Gillespie_Sys: public _System {
public:
/** @brief Constructor
 * @param output_mode Output mode
 * @param n_steps Number of simulation steps.
 * @param max_Sim_Time Maximum simulation time.
 * @param rec_interval Time record time interval (for equidistant timeseries)
 * @param evt_vec Pointer to a vector of pointers to all defined Events.
 * @param vxl_vec Pointer to a vector of pointers to all defined Voxel.
 * @param osid Output Output _Species Vector
 * @param ovid Output Output _Voxel Vector
 * @param output_dir_path Output Directory Path*/
	Gillespie_Sys(long, long, double, double, EventVector*, VoxelVector*,
			string, vector<long>, vector<long>);
/** @brief Destructor*/
	virtual ~Gillespie_Sys(){}
/** @brief Implementation of the _System::go() method.
 * @param n_it Current number of simulation run (required for output file names to
 * distinguish between multiple trajectories resulting from multiple simulation runs
 * during one program call.)
 *
 * Start the simulation algorithm until one of the exit condition is met. The algorithm
 * consists of the following steps:
 * 1. sort the event vector ascending of its tau values
 * 2. execute first Element of the event vector
 * 3. update all events
 * 4. clean voxel (set dirty_flags = @c FALSE)
 * 5. loop*/
	void go(long);

private:
/** @brief file output of the system.
 *
 * The function system_output() takes care, that the a predefined part (input file) of the
 * system state is recorded with a discrete record interval.*/
	void system_output();
/** @brief build the dependency graph with defined Events.
 *
 * The dependency graph is an important structure for the update procedure. Through
 * analyzing the state change vectors, dependencies are recognized and saved. All state
 * change vectors are compared to each other. If sc_vec a at index k is != 0 and sc_vec b
 * at index k is < 0, b is dependent on a.*/
	void build_dep_graph();
/** @brief print current Gillespie_Sys State to console*/
	void print_system_state();

/** @brief timing mode
 *
 * 0. write data output with predefined record interval
 * 1. optimized data output for time series analysis (set sim time and n_steps according to 1/a0)
 * 2. set record interval 0. Each simulation step is recorded
*/
	long output_mode;
	size_t n_steps; 					//!< Holds number of simulation steps
	long opcntr;						//!< Output counter
	long channel_state;					//!< Tracks the channel state during simulation
	double max_Sim_Time;				//!< Maximal simulation time
	double rec_interval;				//!< Time interval, system state is recorded
	double curr_time;					//!< Current time
	double rec_time;					//!< Recording interval counter
	double a_0;							//!< Get current total system propensity a0
	string output_dir_path;				//!< Output directory path
	stringstream data_path;				//!< Data output path
	stringstream info_path;				//!< Simulation info output path
	stringstream oc_path;				//!< Event logging output path
	stringstream dcol_path;				//!< Data collection parameter path
	ofstream data_output_file;			//!< Output file stream for simulation data output
	ofstream info_output_file;			//!< Output file stream for simulation info output
	ofstream oc_log_file;				//!< Output file stream for event logging
	ofstream dcol_file;					//!< Output file stream for data collection parameters

	vector<long> evt_cntr;				//!< Counts number of Event execution.
	vector<long> distr_vec;				//!< Distribution of _Species with id=0
	vector<long> osid;					//!< Output species id vector
	vector<long> ovid;					//!< Output voxel id vector
	vector<_Event*>* evt_vec; 			//!< _Event vector
	vector<_Voxel*>* vxl_vec;			//!< _Voxel vector

};
}

#endif /* SYSTEM_H_ */
