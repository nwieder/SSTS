#include "Standard_Ipt.h"

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>

namespace nw{

Standard_Ipt::~Standard_Ipt(){
	if(rg){delete rg;rg=NULL;}
	for (size_t i=0;i<xgd.xml_spc_list.size();++i){
		if(xgd.xml_spc_list[i]){delete xgd.xml_spc_list[i];xgd.xml_spc_list[i]=NULL;}}
	for (size_t i=0;i<xgd.xml_ch_spc_list.size();++i){
		if(xgd.xml_ch_spc_list[i]){delete xgd.xml_ch_spc_list[i];xgd.xml_ch_spc_list[i]=NULL;}}
	for (size_t i=0;i<xgd.xml_vxl_list.size();++i){if(xgd.xml_vxl_list[i]){
		delete xgd.xml_vxl_list[i];xgd.xml_vxl_list[i]=NULL;}}
	for (size_t i=0;i<xgd.xml_rct_evt_list.size();++i){
		if(xgd.xml_rct_evt_list[i]){delete xgd.xml_rct_evt_list[i];xgd.xml_rct_evt_list[i]=NULL;}}
	for (size_t i=0;i<xgd.xml_suSwitch_rct_evt_list.size();++i){
		if(xgd.xml_suSwitch_rct_evt_list[i]){delete xgd.xml_suSwitch_rct_evt_list[i];
		xgd.xml_suSwitch_rct_evt_list[i]=NULL;}}
	for (size_t i=0;i<xgd.xml_chFlux_rct_evt_list.size();++i){
		if(xgd.xml_chFlux_rct_evt_list[i]){delete xgd.xml_chFlux_rct_evt_list[i];
		xgd.xml_chFlux_rct_evt_list[i]=NULL;}}
	for (size_t i=0;i<xgd.xml_dif_evt_list.size();++i){if(xgd.xml_dif_evt_list[i]){
		delete xgd.xml_dif_evt_list[i];xgd.xml_dif_evt_list[i]=NULL;}}
	for (size_t i=0;i<svc.size();++i){if(svc[i]){delete svc[i];svc[i]=NULL;}}
	for (size_t i=0;i<vvc.size();++i){if(vvc[i]){delete vvc[i];vvc[i]=NULL;}}
	for (size_t i=0;i<evc.size();++i){if(evc[i]){delete evc[i];evc[i]=NULL;}}
	if(s){delete s;s=NULL;}
}

_System* Standard_Ipt::get_System(){
	read_Sim_Data();
//	create Gillespie_Sys
	s = new Gillespie_Sys(	xgd.xml_output_mode, xgd.xml_max_sim_steps, xgd.xml_max_sim_time,
							xgd.xml_sample_rate,&evc, &vvc,output_dir_path,xgd.xml_output_Spc,
							xgd.xml_output_Vxl);
	return s;
}

void Standard_Ipt::read_Sim_Data(){

	xml_node top = doc.child("Sim");
	xml_node_iterator it = doc.begin();
	xml_node_iterator li_it; // iterator for the outputSpeciesList

//	open output file stream
	string tmp = output_dir_path + "sim_info" + ".e";
	info_path.open(tmp.c_str());
	info_path << build_header() << endl;
	info_path << "############ GENERAL DATA ############" << endl;

//	read the general data and save to xgd
	xgd.xml_output_mode = atol(it->child_value("outputMode"));
	info_path << "output mode: " << xgd.xml_output_mode << endl;
	xgd.xml_max_sim_steps = atof(it->child_value("maxSimulationSteps"));
	info_path << "max simulation steps: " << xgd.xml_max_sim_steps << endl;
	xgd.xml_max_sim_time = atof(it->child_value("maxSimTime"));
	info_path<< "max simulation time: " << xgd.xml_max_sim_time << endl;
	xgd.xml_sample_rate = atof(it->child_value("sampleRate"));
	info_path<< "sample rate: " << xgd.xml_sample_rate << endl;
	xgd.xml_box_lenght = atof(it->child_value("voxelBoxLenght"));
	info_path<< "voxel box lenght: " << xgd.xml_box_lenght << endl;
	xgd.xml_border_condition = (string)it->child_value("borderCondition");
	info_path<< "border condition: " << xgd.xml_border_condition << endl;

//	print output _Species and _Voxel ids to sim_info.e
	info_path<< "output species: ";
	for (li_it = it->child("outputSpcList").begin(); li_it !=
			it->child("outputSpcList").end(); ++li_it){
		xgd.xml_output_Spc.push_back(atol(li_it->child_value()));
		info_path << li_it->child_value() << ",";
	}
	info_path << endl << "output voxel: " ;
	for (li_it = it->child("outputVxlList").begin(); li_it !=
			it->child("outputVxlList").end(); ++li_it){
		xgd.xml_output_Vxl.push_back(atol(li_it->child_value()));
		info_path<< li_it->child_value() << ",";
	}
	info_path << endl;

	read_Species(it->child("speciesList"));
	read_Voxel(it->child("voxelList"));
	read_Events(it->child("eventList"));

	alloc_Species();
	alloc_Voxel();
	alloc_Events();

//	close the file stream
	info_path << endl << endl << endl;
	info_path.close();
}

void Standard_Ipt::read_Species(xml_node const& nod){
	cout << "read_Species...",
	info_path << "############ SPECIES ############" << endl;

	xml_node_iterator spc_it;  //voxel iterator, voxel element iterator, list iterator

//	read all Standard Species
	for (spc_it = nod.child("standardSpcList").begin(); spc_it !=
			nod.child("standardSpcList").end(); ++spc_it){
// 		create new standard species struct
		Standard_Ipt::xml_spc* xspc = new Standard_Ipt::xml_spc;
		xspc->xml_id = atol(spc_it->attribute("id").value());
		info_path<< "id: " << xspc->xml_id;
		xspc->xml_name = (string)spc_it->child_value("name");
		info_path<< ", name: " << xspc->xml_name;
		xspc->xml_init_conc = atof(spc_it->child_value("initialConcentration"));
		info_path<< ", initConc:" << xspc->xml_init_conc;
		info_path<< endl;
		xgd.xml_spc_list.push_back(xspc);
	}

//	read all Channel Species
	for (spc_it = nod.child("channelSpcList").begin(); spc_it !=
			nod.child("channelSpcList").end(); ++spc_it){
// 		create new channel species struct
		Standard_Ipt::xml_Ch_Spc* xspc = new Standard_Ipt::xml_Ch_Spc;
		xspc->xml_id = atol(spc_it->attribute("id").value());
		info_path<< "id: " << xspc->xml_id;
		xspc->xml_name = (string)spc_it->child_value("name");
		info_path<< ", name: " << xspc->xml_name;
		xspc->xml_init_conc = atof(spc_it->child_value("initialConcentration"));
		info_path<< ", initConc:" << xspc->xml_init_conc;
		xspc->xml_n_subunits = atol(spc_it->child_value("nSubUnits"));
		info_path<< ", n_subunits:" << xspc->xml_n_subunits;
		xspc->xml_n_suToOpen = atol(spc_it->child_value("nSubUnitsToOpen"));
		info_path<< ", n_suToOpen:" << xspc->xml_n_suToOpen;
		info_path<< endl;
		xgd.xml_ch_spc_list.push_back(xspc);
	}

	cout << "DONE" << endl;
}

void Standard_Ipt::read_Voxel(xml_node const& nod){
	cout << "read_Voxel...";
	info_path << "############ VOXEL ############" << endl;
	xml_node_iterator vxl_it, li_it; //voxel iterator

//	read all Standard Voxel
	for(vxl_it = nod.child("standardVxlList").begin(); vxl_it !=
			nod.child("standardVxlList").end(); ++vxl_it){
		Standard_Ipt::xml_vxl* xvxl = new Standard_Ipt::xml_vxl;
		xvxl->xml_id = atol(vxl_it->attribute("id").value());
		info_path<< "id: " << xvxl->xml_id;
		info_path<< ", Initial State: ";
		for (li_it = vxl_it->child("initialState").begin(); li_it !=
				vxl_it->child("initialState").end(); ++li_it){
			xvxl->xml_init_state.push_back(atol(li_it->child_value()));
			info_path<< li_it->child_value();
		}
		info_path<< ", Neighbours: ";
		for (li_it = vxl_it->child("neighbours").begin(); li_it !=
				vxl_it->child("neighbours").end(); ++li_it){
			xvxl->xml_vxl_neighbours.push_back(atol(li_it->child_value()));
			info_path<< li_it->child_value();
		}
		info_path<< endl;
		xgd.xml_vxl_list.push_back(xvxl);
	}
	cout << "DONE" << endl;
}

void Standard_Ipt::read_Events(xml_node const& nod){
	cout << "read_Events... ";
	info_path << "############ EVENTS ############" << endl;
	xml_node_iterator evt_it, li_it; // event iterator, special event iterator, list iterator

//	runs through every item of the Reaction Event List and creates the corresponding struct
	for(evt_it = nod.child("reactionEvtList").begin();
			evt_it != nod.child("reactionEvtList").end(); ++evt_it){
		Standard_Ipt::xml_Rct_Evt* xevt = new Standard_Ipt::xml_Rct_Evt;
		xevt->xml_id = atol(evt_it->attribute("id").value());
		info_path<< "id: " << xevt->xml_id;
		xevt->xml_name = (string)evt_it->child("name").child_value();
		info_path<< ", name: " << xevt->xml_name;
		xevt->xml_k = atof(evt_it->child("k").child_value());
		info_path<< ", k: " <<xevt->xml_k;
		info_path<< ", Voxel: ";
		for (li_it = evt_it->child("voxelVector").begin();
				li_it != evt_it->child("voxelVector").end(); ++li_it){
			xevt->xml_vxl.push_back(atol(li_it->child_value()));
			info_path<< li_it->child_value();}
		info_path<< ", Educts: ";
		for (li_it = evt_it->child("educts").begin();
				li_it != evt_it->child("educts").end(); ++li_it){
			xevt->xml_educts.push_back(atol(li_it->child_value()));
			info_path<< li_it->child_value();}
		info_path<< ", Products: ";
		for (li_it = evt_it->child("products").begin();
				li_it != evt_it->child("products").end(); ++li_it){
			xevt->xml_products.push_back(atol(li_it->child_value()));
			info_path<< li_it->child_value();}
		info_path<< endl;
		xgd.xml_rct_evt_list.push_back(xevt);
	}

//	runs through every item of the Reaction Sub Unit Switch Reaction Event List and
//	creates the corresponding struct
	for(evt_it = nod.child("subUnitSwitchRctEvtList").begin();
			evt_it != nod.child("subUnitSwitchRctEvtList").end(); ++evt_it){
		Standard_Ipt::xml_SuSwitch_Rct_Evt* xevt = new Standard_Ipt::xml_SuSwitch_Rct_Evt;
		xevt->xml_id = atol(evt_it->attribute("id").value());
		info_path<< "id: " << xevt->xml_id;
		xevt->xml_name = (string)evt_it->child("name").child_value();
		info_path<< ", name: " << xevt->xml_name;
		xevt->xml_k = atof(evt_it->child("k").child_value());
		info_path<< ", k: " <<xevt->xml_k;
		xevt->xml_channel_id = atol(evt_it->child("channel").child_value());
		info_path<< ", channel; " << xevt->xml_channel_id;
		xevt->xml_actSu_id = atol(evt_it->child("actSubUnitID").child_value());
		info_path<< ", act su ID: " << xevt->xml_actSu_id;

		info_path<< ", Voxel: ";
		for (li_it = evt_it->child("voxelVector").begin();
				li_it != evt_it->child("voxelVector").end(); ++li_it){
			xevt->xml_vxl.push_back(atol(li_it->child_value()));
			info_path<< li_it->child_value();
		}
		info_path<< ", Educts: ";
		for (li_it = evt_it->child("educts").begin();
				li_it != evt_it->child("educts").end(); ++li_it){
			xevt->xml_educts.push_back(atol(li_it->child_value()));
			info_path<< li_it->child_value();
		}
		info_path<< ", Products: ";
		for (li_it = evt_it->child("products").begin();
				li_it != evt_it->child("products").end(); ++li_it){
			xevt->xml_products.push_back(atol(li_it->child_value()));
			info_path<< li_it->child_value();
		}
		info_path<< endl;
		xgd.xml_suSwitch_rct_evt_list.push_back(xevt);
	}

//	runs through every item of the Channel Flux Reaction Event List
//	and creates the corresponding struct
	for(evt_it = nod.child("channelFluxEvtList").begin();
		evt_it != nod.child("channelFluxEvtList").end(); ++evt_it){
		Standard_Ipt::xml_ChFlux_Rct_Evt* xevt = new Standard_Ipt::xml_ChFlux_Rct_Evt;
		xevt->xml_id = atol(evt_it->attribute("id").value());
		info_path<< "id: " << xevt->xml_id;
		xevt->xml_name = (string)evt_it->child("name").child_value();
		info_path<< ", name: " << xevt->xml_name;
		xevt->xml_k = atof(evt_it->child("k").child_value());
		info_path<< ", k: " <<xevt->xml_k;
		xevt->xml_channel_id = atol(evt_it->child("channel").child_value());
		info_path<< ", channel; " << xevt->xml_channel_id;

		info_path<< ", Voxel: ";
		for (li_it = evt_it->child("voxelVector").begin();
				li_it != evt_it->child("voxelVector").end(); ++li_it){
			xevt->xml_vxl.push_back(atol(li_it->child_value()));
			info_path<< li_it->child_value();
		}

		info_path<< ", Products: ";
		for (li_it = evt_it->child("products").begin();
				li_it != evt_it->child("products").end(); ++li_it){
			xevt->xml_products.push_back(atol(li_it->child_value()));
			info_path<< li_it->child_value();
		}
		info_path<< endl;
		xgd.xml_chFlux_rct_evt_list.push_back(xevt);
	}

//	runs through every item of the Diffusion Event List and creates the corresponding struct
	for(evt_it = nod.child("diffusionEvtList").begin();
			evt_it != nod.child("diffusionEvtList").end(); ++evt_it){
		Standard_Ipt::xml_Dif_Evt* xevt = new Standard_Ipt::xml_Dif_Evt;
		xevt->xml_id = atol(evt_it->attribute("id").value());
		info_path<< "id: " << xevt->xml_id;
		xevt->xml_name = (string)evt_it->child("name").child_value();
		info_path<< ", name: " << xevt->xml_name;
		xevt->xml_k = atof(evt_it->child("k").child_value());
		info_path<< ", k: " <<xevt->xml_k;
		xevt->xml_diff_spc = atol(evt_it->child("diffusionSpcID").child_value());
		info_path<< ", diffSpcID: " << xevt->xml_diff_spc;

		info_path<< ", Voxel: ";
		for (li_it = evt_it->child("voxelVector").begin();
				li_it != evt_it->child("voxelVector").end(); ++li_it){
			xevt->xml_vxl.push_back(atol(li_it->child_value()));
			info_path<< li_it->child_value();
		}
//		add the border voxel to the voxel vector of all diffusion events.
		if (xgd.xml_border_condition == "equilibrium"){
//		since the border voxel doesn't exist yet, xgd.xml_vxl_list.size() is the correct index.
			xevt->xml_vxl.push_back(xgd.xml_vxl_list.size());
			info_path << (xgd.xml_vxl_list.size());
		}
		info_path<< endl;

		xgd.xml_dif_evt_list.push_back(xevt);
	}
	cout << "DONE" << endl;
}

void Standard_Ipt::alloc_Species(){
	cout << "alloc Species... ";

	for (long i = 0; i < (long) xgd.xml_spc_list.size(); i++){
		_Species* ts = new Standard_Spc(xgd.xml_spc_list[i]->xml_id,
						xgd.xml_spc_list[i]->xml_name, xgd.xml_spc_list[i]->xml_init_conc);
		svc.push_back(ts);
	}
	for (long i = 0; i < (long) xgd.xml_ch_spc_list.size(); i++){
		_Species* tcs = new Channel_Spc(xgd.xml_ch_spc_list[i]->xml_id,
						xgd.xml_ch_spc_list[i]->xml_name, xgd.xml_ch_spc_list[i]->
						xml_init_conc,
						xgd.xml_ch_spc_list[i]->xml_n_subunits,	xgd.xml_ch_spc_list[i]->
						xml_n_suToOpen);
		svc.push_back(tcs);
	}

	cout << "DONE" << endl;
}

void Standard_Ipt::alloc_Voxel(){
	cout << "alloc Voxel... ";

//	border condition is equilibrium
	if(xgd.xml_border_condition == "equilibrium"){

// 		run through voxl list and create Standard_Voxel
		for (long i = 0; i < (long)xgd.xml_vxl_list.size(); i++){
			_Voxel* tvxl = 	new Standard_Vxl(xgd.xml_vxl_list[i]->xml_id,
							xgd.xml_box_lenght, clone_svc());
			vvc.push_back(tvxl);
		}

//		add border Voxel vvc.push_back(tbvxl);
		_Voxel* bv = new Border_Vxl(vvc.size(), xgd.xml_box_lenght, clone_svc());
		vvc.push_back(bv);

//		initialize the diffusion neighbors including the border voxel
		for (long i = 0; i < (long)xgd.xml_vxl_list.size(); i++){
			for (long j = 0; j < (long)xgd.xml_vxl_list[i]->xml_vxl_neighbours.size(); j++)
				vvc[i]->add_neighbour(vvc.at(xgd.xml_vxl_list[i]->xml_vxl_neighbours[j]));
			for (long j = vvc[i]->get_diff_vec()->size(); j < 6; j++){
//				fill every free spot of the diffusion neighbour vector with border voxel
//				(last element of the voxel vector)
				vvc[i]->add_neighbour(vvc.at(vvc.size()-1));
//				add current voxel to the diffusion neighbour vector of the border voxel
				vvc[vvc.size()-1]->add_neighbour(vvc[i]);
			}
	 	}

//	border condition is not equilibrium
	} else{
//		run through voxel list and create Standard_Voxel
		for (long i = 0; i < (long)xgd.xml_vxl_list.size(); i++){
			_Voxel* tvxl =	new Standard_Vxl(xgd.xml_vxl_list[i]->xml_id,
							xgd.xml_box_lenght, clone_svc());
			vvc.push_back(tvxl);
		}
//		initialize the diffusion neighbours including the border voxel
		for (long i = 0; i < (long)xgd.xml_vxl_list.size(); i++){
			for (long j = 0; j < (long)xgd.xml_vxl_list[i]->xml_vxl_neighbours.size(); j++){
				vvc[i]->add_neighbour(vvc.at(xgd.xml_vxl_list[i]->xml_vxl_neighbours[j]));
			}
		}
		cout << " *** NO BORDER VOXEL DEFINED *** ";
	}

	cout << "DONE" << endl;
}

void Standard_Ipt::alloc_Events(){
	cout << "alloc Events... ";

//	create Reaction Event Voxel Vector
	vector<_Voxel*> re_vvc = vvc; 	// reaction event voxel vector
	re_vvc.pop_back();				// doesn't contain the border voxel!

//	add all Reaction Events to the Event vector
	for (long i = 0; i < (long)xgd.xml_rct_evt_list.size(); i++){
		xml_Rct_Evt* cxe = xgd.xml_rct_evt_list[i];
		_Event* tre = 	new Reaction_Evt(cxe->xml_id, cxe->xml_name, cxe->xml_k,
						build_sc_vector(cxe), build_vxl_vec(cxe), rg);
		evc.push_back(tre);
	}

//	add all Subunit Switch Reaction Events to the Event vector
	for (long i = 0; i < (long)xgd.xml_suSwitch_rct_evt_list.size(); i++){
		xml_SuSwitch_Rct_Evt* cxe = xgd.xml_suSwitch_rct_evt_list[i];
		_Event* tsusre = 	new SubUnitSwitch_Rct_Evt(cxe->xml_id, cxe->xml_name, cxe->xml_k,
							build_sc_vector(cxe), cxe->xml_actSu_id, cxe->xml_channel_id,
							build_vxl_vec(cxe), rg);
		evc.push_back(tsusre);
	}

//	add all Channel Flux Reaction Events to the Event vector
	for (long i = 0; i < (long)xgd.xml_chFlux_rct_evt_list.size(); i++){
		xml_ChFlux_Rct_Evt* cxe = xgd.xml_chFlux_rct_evt_list[i];
		_Event* tcre = 	new ChFlux_Rct_Evt(cxe->xml_id, cxe->xml_name, cxe->xml_k,
						build_sc_vector(cxe), cxe->xml_channel_id, build_vxl_vec(cxe), rg);
		evc.push_back(tcre);
	}

//	add all Diffusion Events to the Event vector
	for (long i = 0; i < (long)xgd.xml_dif_evt_list.size(); i++){
		xml_Dif_Evt* cxe = xgd.xml_dif_evt_list[i];
		_Event* tde =	new Diffusion_Evt(cxe->xml_id, cxe->xml_name, cxe->xml_k,
						build_vxl_vec(cxe), cxe->xml_diff_spc,rg);
		evc.push_back(tde);
	}

//	output
	info_path << "*** State change vectors ***" << endl;
	for (long i = 0; i < (long)evc.size(); i++){
		info_path<< evc.at(i)->get_id() << " :" << evc.at(i)->get_name() << " sc_vec: \t";
		for (int j = 0; j < (int)evc.at(i)->get_sc_vec().size(); j++)
			info_path<< evc.at(i)->get_sc_vec().at(j) << "," ;
		info_path<< endl;
	}

	cout << "DONE" << endl;
}

vector<long> Standard_Ipt::build_sc_vector(xml_Rct_Evt* const& cxe){
	vector<long>* scv  = new vector<long>;
	scv->resize(vvc[cxe->xml_vxl[0]]->get_state_vec()->size());
	long i;

	for (i = 0; i < (long)cxe->xml_educts.size(); i++)
		scv->at(cxe->xml_educts.at(i)) = -1;
	for (i = 0; i < (long)cxe->xml_products.size(); i++)
		scv->at(cxe->xml_products.at(i)) = 1;

	return *scv;
}

vector<_Voxel*> Standard_Ipt::build_vxl_vec(xml_evt* const& evt){
	vector<_Voxel*> act_vvc;

	if(evt->xml_vxl[0] == -1){
		return vvc;
	}

	for(size_t i = 0; i < evt->xml_vxl.size(); ++i){
		act_vvc.push_back(vvc[evt->xml_vxl[i]]);
	}
	return act_vvc;
}

vector<_Species*> Standard_Ipt::clone_svc(){
	vector<_Species*> svcClone;
	svcClone.resize(svc.size());

	for (long i = 0; i < (long) svc.size(); i++)
		svcClone[i] = svc[i]->copy();
	return svcClone;
}

string Standard_Ipt::build_header(){
	time_t rawtime;
	time ( &rawtime );

	string tmp = 	"STOCHASTIC SIMULATION SOFTWARE \n ------------------"
					"--------------------------------\n developed by Nicolas Wieder and "
					"Frederic von Wegner \n Medical Biophysics Group, Institute of "
					"Physiology and Pathophysiology,\n University of Heidelberg, "
					"69120 Heidelberg, Germany\n----------------------------------"
					"----------------\n\n";
	tmp += ctime (&rawtime);
	return tmp;
}

} // namespace nw
