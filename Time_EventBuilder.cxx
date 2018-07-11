#include "Time_EventBuilder.h"

using namespace std;

//---------------------------------------------------------------

Time_EventBuilder::Time_EventBuilder(int type_of_interest){
	this->type_of_interest = type_of_interest;
}

//---------------------------------------------------------------

Time_EventBuilder::~Time_EventBuilder(){

}

//---------------------------------------------------------------


void Time_EventBuilder::set_Event(Raw_Event* RAW){
	//get PrcID from RAW
	int tmp_type = RAW->get_Type();

	if(tmp_type > 4 || tmp_type < 0){
		cerr << "Unknown PrcID in Time_EventBuilder. Raw_Event sent wrong ID" << endl;
		exit(0);
	}

	//check if type_of_interest
	if(tmp_type == type_of_interest){
		Main_Event[main_iterator] = new (RAW);
		
		new_Comparison();

		main_iterator++;
	}
	else{
		Non_Main_Event[tmp_type][non_main_iterator[tmp_type]] = new (RAW);
		
		existing_Comparison(tmp_type);

		non_main_iterator[tmp_type]++;
	}
}

//---------------------------------------------------------------

void Time_EventBuilder::existing_Comparison(int tmp_type){

	//
	int tmp_iter = non_main_iterator[tmp_type];

	//tmp value
	double delta_T = 100000;

	//get White Rabbit of non main event
	double Time_of_Non_Main = Non_Main_Event[tmp_type][tmp_iter]->get_T();
	
	//loop over all main events
	for(int i = 0;i < main_iterator;++i){
		delta_T = Main_Event[i]->get_T() - Time_of_Non_Main;
		time_difference[i][tmp_type][tmp_iter] = delta_T;
		
		//check if new time difference is smallest
		if(delta_T < Minimal_T_Diffs[i][tmp_type]){
			min_pos[i][tmp_type] = tmp_iter;
			if(delta_T < threshold){
				write_and_clear(i,tmp_type,tmp_iter);
				break;
			}
		}
	}
}

//---------------------------------------------------------------

void Time_EventBuilder::new_Comparison(){
	
	//Time of Main Event
	double main_time = Main_Event[main_iterator]->get_T();
	double non_main_time = 0;

	//loop over all Detector System unequal to type_of_interest
	for(int i = 0;i < 5;++i){
		if(i != type_of_interest){
			//loop over all recorded events
			for(int j = 0;j < non_main_iterator[i];++j){
				non_main_time = Non_Main_Event[i][j]->get_T();
				time_difference[type_of_interest][i][j] = main_time - non_main_time;
				if(time_difference[type_of_interest][i][j] < threshold){
					write_and_clear(type_of_interest,tmp_type,tmp_iter);
					break;
				}
			}
		}
	}
}