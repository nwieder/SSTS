#ifndef STANDARD_IPT_H_
#define STANDARD_IPT_H_

#include "_Input.h"
#include "../System/Gillespie_Sys.h"
#include "../EventHeader.h"
#include "../VoxelHeader.h"
#include "../SpeciesHeader.h"
#include "../pugi/pugixml.hpp"

using namespace std;
using namespace pugi;

namespace nw{

/** @brief Input class that parses a .xml files and generates a Gillespie_Sys
 *
 * The structure of the input .xml file represents the objects that need to be created to
 * set up a model. The open source xml parser <EM>pugixml</EM> (http://pugixml.org/)
 * version 0.9 is used. The input procedure
 * is subdivided into three steps:
 * 1. .xml data is parsed with <EM>pugixml</EM>.
 * 2. Parse result is analyzed and model data temporarily stored in structures.
 * 3. Objects are created using attributes of respective structures as parameters.
 * */
class Standard_Ipt: public nw::_Input {
public:
	/** @brief Constructor
	 * @param input_path Path to input file (.xml)
	 * @param output_dir_path Path to output directory
	 * */
	Standard_Ipt(string input_path, string output_dir_path){
		this->p = (char*)input_path.c_str();
		xml_parse_result result = doc.load_file(p); // load xml file
		this->output_dir_path = output_dir_path;
		rg = new Uni_Rnd();
	}
	/** @brief Destructor */
	~Standard_Ipt();
	/** @brief Implementation of the _Input::get_System() method
	 * @return Gillespie_Sys
	 *
	 * Create Gillespie_Sys from input .xml file*/
	_System* get_System();

private:
/**	@brief Structure with attributes of the _Event class*/
	struct xml_evt{
		long xml_id;
		string xml_name;
		double xml_k;
		vector<long> xml_vxl;
	};
/**	@brief Structure with attributes of the Reaction_Evt class*/
	struct xml_Rct_Evt:xml_evt{
		vector<long> xml_educts;
		vector<long> xml_products;
	};
/**	@brief Structure with attributes of the Diffusion_Evt class*/
	struct xml_Dif_Evt:xml_evt{
		long xml_diff_spc;
	};
/**	@brief Structure with attributes of the SubUnitSwitch_Rct_Evt class*/
	struct xml_SuSwitch_Rct_Evt:xml_Rct_Evt{
		long xml_actSu_id;
		long xml_channel_id;
	};
/**	@brief Structure with attributes of the ChFlux_Rct_Evt class*/
	struct xml_ChFlux_Rct_Evt:xml_Rct_Evt{
		long xml_channel_id;
	};
/**	@brief Structure with attributes of the _Voxel class*/
	struct xml_vxl{
		long xml_id;
		vector<long> xml_init_state;
		vector<long> xml_vxl_neighbours;
	};
/**	@brief Structure with attributes of the _Species class*/
	struct xml_spc{
		long xml_id;
		string xml_name;
		double xml_init_conc;
	};
/**	@brief Structure with attributes of the Channel_Spc class*/
	struct xml_Ch_Spc:xml_spc{
		long xml_n_subunits;
		long xml_n_suToOpen;
	};
/**	@brief Structure with attributes representing general data necessary for a system*/
	struct xml_gen_data{
		string 	xml_simulation_type;
		string 	xml_border_condition;
		long 	xml_output_mode;
		double 	xml_max_sim_steps;
		double 	xml_max_sim_time;
		double 	xml_sample_rate;
		double 	xml_box_lenght;
		vector<long> xml_output_Spc;
		vector<long> xml_output_Vxl;
		vector<xml_Rct_Evt*> xml_rct_evt_list;
		vector<xml_Dif_Evt*> xml_dif_evt_list;
		vector<xml_SuSwitch_Rct_Evt*> xml_suSwitch_rct_evt_list;
		vector<xml_ChFlux_Rct_Evt*> xml_chFlux_rct_evt_list;
		vector<xml_vxl*> xml_vxl_list;
		vector<xml_spc*> xml_spc_list;
		vector<xml_Ch_Spc*> xml_ch_spc_list;
	};

//	------------------------------------------

/** @brief Extract general simulation data and store it in the xgd struct*/
	void read_Sim_Data();
/** @brief Read events from parse result
 * @param nod XML node <EM>event_List</EM>*/
	void read_Events(pugi::xml_node const& nod);
/** @brief Read voxels from parse result
 * @param nod XML node <EM>voxel_List</EM>*/
	void read_Voxel(pugi::xml_node const& nod);
/** @brief Read species from parse result
 * @param nod XML node <EM>species_list</EM>*/
	void read_Species(pugi::xml_node const& nod);
/** @brief Create _Event objects*/
	void alloc_Events();
/** @brief Create _Voxel objects*/
	void alloc_Voxel();
/** @brief Create _Species objects*/
	void alloc_Species();
/** @brief Build _Voxel vector for _Event objects
 * @param evt xml_evt structure
 * @return _Event specific voxel vector
 *
 * The event definition inside the .xml file includes a list of _Voxel ids that represent
 * the subset of _Voxel where the respective _Event can occur.<BR>
 * If the id list only contains one element with a value of -1, the event is can occur in
 * all existing _Voxel.
 * */
	vector<_Voxel*> build_vxl_vec(xml_evt* const& evt);
/** @brief Build state change vector for Reaction_Evt objects
 * @ param cxe xml_Rct_Evt
 * @ return Reaction_Evt specific state change vector
 * */
	vector<long> build_sc_vector(xml_Rct_Evt* const& cxe);
/** @brief Clone _Species vector
 * @return new instance of the _Species vector*/
	vector<_Species*> clone_svc();
/** @brief Builds console output header */
	string build_header ();

	string output_dir_path; 	//!< Output directory path
	xml_gen_data xgd; 			//!< Structure holding general simulation data
	pugi::xml_document doc; 	//!< DOM tree root (pugixml)
	char * p;
	ofstream info_path; 		//!< Off-stream for sim_info file summarizing parsed xml data
	Uni_Rnd* rg; 				//!< Uniform random number generator
	vector<_Event*> evc; 		//!< _Event vector
	vector<_Voxel*> vvc; 		//!< _Voxel vector
	vector<_Species*> svc; 		//!< _Specied vector
};
} // namespace nw
#endif /* STANDARD_IPT_H_ */
