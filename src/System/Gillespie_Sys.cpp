#include <iostream>
#include <algorithm>
#include <exception>
#include <cmath>
#include <ios>
#include <iomanip>

#include "Gillespie_Sys.h"

namespace nw {

using namespace std;

// Constructor
Gillespie_Sys::Gillespie_Sys(long output_mode, long n_steps, double max_Sim_Time,
	double rec_interval, EventVector* evt_vec, VoxelVector* vxl_vec,
	string output_dir_path, vector<long> osid, vector<long> ovid):
	output_mode(output_mode),
	n_steps(n_steps),
	opcntr(0),
	max_Sim_Time(max_Sim_Time),
	rec_interval(rec_interval),
	curr_time(0),
	rec_time(0),
	a_0(0),
	output_dir_path(output_dir_path),
	osid(osid),
	ovid(ovid),
	evt_vec(evt_vec),
	vxl_vec(vxl_vec)
{

//	initialize event counter vector
	evt_cntr.assign(evt_vec->size(), 0);
//  initialize distribution vector
	distr_vec.assign(500, 0);

//	initialize all Events
	for (size_t i = 0; i < evt_vec->size(); ++i) {
		evt_vec->at(i)->init();
	}

	this->a_0 = 0;
//	Calculate a_0 from all system events
	for (size_t i = 0; i < this->evt_vec->size() ;++i){
		this->a_0 += this->evt_vec->at(i)->get_a();
	}

//	Adapt exit conditions and record interval according to the timing_mode
	if (this->output_mode == 1){
		// set the record interval to 10 * average system waiting time
		this->rec_interval = 10 / a_0;
	} else if (this->output_mode == 2){
		this->rec_interval = 0;
	}

//	print data output parameters for data analysis
	this->dcol_path << output_dir_path << "sim_param.py";
	const string &tmp_dcol = this->dcol_path.str();
	this->dcol_file.open(tmp_dcol.c_str());
	this->dcol_file << "mean_tau = " << 1/this->a_0 << endl << "dt = "
					<< this->rec_interval << endl << "n_steps= " << this->n_steps
					<< endl << "T_max = " << this->max_Sim_Time << endl << endl;
	this->dcol_file.close();
	cout 	<< "\n### Timing Parameters ###\n" << "mean_tau = " << 1/this->a_0
			<< endl << "dt = " << this->rec_interval << endl << "n_steps= "
			<< this->n_steps << endl << "T_max = " << this->max_Sim_Time
			<< "\n######################" << endl << endl;

//	initialize channel state log
	channel_state = 0;

//	build the dependency graph
	build_dep_graph();

}

//  start simulation
void Gillespie_Sys::go(long n_it)
{
	try{

// 		build path to output files. n_it represents the current simulation run.

//		data files containing molecular numbers
		data_path << output_dir_path << "data_" << n_it << ".e";
// 		open/close log for channel dynamics
		oc_path << output_dir_path << "oc_log_" << n_it << ".e";
//		independent result log of every simulation run
		info_path << output_dir_path << "sim_res.e";

		const string &tmp_data = data_path.str();
		const string &tmp_info = info_path.str();
		const string &tmp_oclog = oc_path.str();

//		connect streams to output file
		data_output_file.open(tmp_data.c_str());
		oc_log_file.open(tmp_oclog.c_str());
		oc_log_file << std::setprecision(10);

//		reconnect to existing file (adding text)
		if (n_it == 1){
			info_output_file.open(tmp_info.c_str());
		}else{
			info_output_file.open(tmp_info.c_str(), ios::in | ios::ate);
		}

//		console output: indicate simulation start
		info_output_file<< "###### Run: " << n_it << " ######" << endl;
		cout << "--SIMULATION BEGIN-- \n" << endl;
		info_output_file << "--SIMULATION RESULTS-- \n" << endl;

//		print the initial system state
		print_system_state();

//		###############################
//		## START THE SIMULATION LOOP ##
//		###############################

//		set times
		double tau = 0, begin, end;
		begin = time(0);

//		check exit conditions
		for (size_t i = 0; i < n_steps; ++i) {
			if (curr_time <= max_Sim_Time) {

				long nextIndex = 0;
				double minTau = INFINITY;

//				event update procedure
				for (size_t j = 0; j < evt_vec->size(); ++j) {
					double actTau = evt_vec->at(j)->update(tau);
//					determine next Event Index
					if (actTau < minTau) {
						minTau = actTau;
						nextIndex = j;
					}
				}

//				update system times
				tau = minTau;
				rec_time += tau;
				curr_time += tau;

//				update event counter
				evt_cntr[nextIndex]++;

//				execute next Event
				evt_vec->at(nextIndex)->execute();

//				print system state if necessary
				system_output();

			} else{
				n_steps = i;
				break;
			}
		}

//		############################
//		## END OF SIMULATION LOOP ##
//		############################

		end = time(0);

//		output of event distribution in info_output_file
		cout << "Event distribution:\t";
		info_output_file << "Event distribution:\t";
		for (long p = 0; p < (long)evt_cntr.size(); p++){
			cout << evt_cntr.at(p) << ",";
			info_output_file << evt_cntr.at(p) << ",";
		}
		cout << endl;
		info_output_file << endl;

//		system information output to info_output_file.
		cout << "Simulated time:\t\t" << curr_time << " ms" << endl;
		info_output_file << "Simulated time:\t\t" << curr_time << " ms" << endl;
		cout << "Number of steps: \t" << n_steps << endl;
		info_output_file << "Number of steps: \t" << n_steps << endl;
		cout << "Computation time:\t" << end - begin << " s" << endl;
		info_output_file << "Computation time:\t" << end - begin << " s" << endl;

//		console output: indicate simulation end
		print_system_state();
		cout << "\n--SIMULATION END--" << endl;
		info_output_file << "\n--SIMULATION END--" << endl << endl << endl;

// 		disconnect stream from output file
		data_output_file.close();
		info_output_file.close();
		oc_log_file.close();
	}

	catch(exception& e){
		cout << "System::go(): " << e.what() << endl;
	}
}

inline void Gillespie_Sys::system_output(){
//	print system state with a fixed time step dt (recording interval)
	if (rec_time >= rec_interval){
//			run through output voxel vector
			for(size_t k = 0; k < ovid.size(); ++k){
//				run through output species id vector
				for(size_t i = 0; i < osid.size(); ++i){
					data_output_file<<	vxl_vec->at(ovid[k])->get_state_vec()
										->at(osid[i])->get_n_molecules() << ",";
				}
			}
		if (this->output_mode == 2){
			data_output_file << curr_time << ",";
		}
		opcntr++;
		rec_time -= rec_interval;
	}

//	check if the channel state changed and if so log the open and close times
	if(	vxl_vec->at(0)->get_state_vec()->at(vxl_vec->at(0)
		->get_state_vec()->size()-1)->get_n_molecules() != channel_state){
//		update current channel state
		channel_state = vxl_vec->at(0)->get_state_vec()->at(vxl_vec->at(0)
						->get_state_vec()->size() - 1)->get_n_molecules();
//		write current system time to oc_log.e
		oc_log_file << curr_time << ",";
	}
}

void Gillespie_Sys::build_dep_graph(){

	try{
//		temporary vectors
		vector<long> sc_a, sc_b;

//		find dependency relations based on state change vectors
		for (size_t i = 0; i < evt_vec->size(); ++i){
			sc_a = evt_vec->at(i)->get_sc_vec();
			for (size_t j = 0; j < evt_vec->size(); ++j){
				sc_b = evt_vec->at(j)->get_sc_vec();
				if(evt_vec->at(i) != evt_vec->at(j)){
					for (size_t k = 0; k < sc_a.size(); ++k){
						if(sc_a.at(k) != 0 && sc_b.at(k) < 0 ){
							evt_vec->at(i)->add_dep_list(evt_vec->at(j));
							break;
						}
					}
				}
			}
		}
	}

	catch(exception& e){
		cout << "System::build_dep_graph()" << e.what();
	}
}

void Gillespie_Sys::print_system_state(){

//	print current system state
	cout << "---System State---\n";
	info_output_file << "---System State---\n";
	for (long i = 0; i < (long)vxl_vec->size();i++){
		cout << "Voxel " << i << ": ";
		info_output_file << "Voxel " << i << ": ";
		for (long j = 0; j < (long)vxl_vec->at(i)->get_state_vec()->size(); j++){
			cout << vxl_vec->at(i)->get_state_vec()->at(j)->get_n_molecules() << ",";
			info_output_file << vxl_vec->at(i)->get_state_vec()->at(j)->get_n_molecules() << ",";
		}
		cout << endl;
		info_output_file << endl;
	}
	cout << "------------------\n";
	info_output_file << "------------------\n";
}

} // namespace nw
