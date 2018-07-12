#include "Time_EventBuilder.h"

using namespace std;

//---------------------------------------------------------------

Time_EventBuilder::Time_EventBuilder(int amount_interest,int* length_interest_tmp,int** interest_array_tmp){

	get_used_Systems();

	//set event stores
	Event_Stores = new Event_Store*[6];

	// all non used systems intialized as NULL 
	//-> calling uninitialized system will cause an error !
	
	Event_Stores[2] = !Used_Systems[2] ? NULL : new PLASTIC_Event_Store();
	Event_Stores[3] = !Used_Systems[3] ? NULL : new FATIMA_Event_Store();
	//Event_Stores[4] = !Used_Systems[4] ? NULL : new GALILEO_Event_Store();

	for(int i = 0;i < 6;++i) if(!Used_Systems[i]) Event_Stores[i] = NULL;

	//get coincidences between Systems of interest defined by interest_array
	this->amount_interest = amount_interest;
	length_interest = new int[amount_interest];
	interest_array = new int*[amount_interest];
	for(int i = 0;i < amount_interest;++i){
		length_interest[i] = length_interest_tmp[i];
		interest_array[i] = new int[length_interest[i]];
		for(int j = 0;j < length_interest[i];++j) interest_array[i][j] = interest_array_tmp[i][j];
	}
	
	check_kinds_overlap();

}

//---------------------------------------------------------------

Time_EventBuilder::~Time_EventBuilder(){

	for(int i = 0;i < amount_interest;++i) delete[] interest_array[i];
	delete[] interest_array;
	delete[] length_interest;

}

//---------------------------------------------------------------

void Time_EventBuilder::check_kinds_overlap(){
	//loop over all possible Detector Systems
	for(int i = 0;i < 6;++i){
		iter[i] = 0;
		for(int j = 0;j < amount_interest;++j){
			for(int k = 0;k < length_interest[j];++k){
				if(interest_array[j][k] == i){
					overlap_arr[i][iter[i]] = j;
					iter[i]++;
					break;
				}
			}
		}
	}
}

//---------------------------------------------------------------

void Time_EventBuilder::set_Event(Raw_Event* RAW){
	//get PrcID from RAW
	int tmp_type = RAW->get_Type();
	
	//PrcID check
	if(tmp_type > 5 || tmp_type < 0){
		cerr << "Unknown PrcID in Time_EventBuilder. Raw_Event sent wrong ID" << endl;
		exit(0);
	}

	ULong64_t WR = RAW->get_WR();

	//save event in respective store
	Event_Stores[tmp_type]->store(RAW);

	int hits[6] = {-1,-1,-1,-1,-1,-1};

	//get coincidences
	for(int i = 0;i < 6;++i) if(i != tmp_type) hits[i] = Event_Stores[i]->Time_Comparison(WR);
	
	//check for shared event data and write data
	for(int i = 0;i < 6;++i){
		if(hits[i] != -1){
			
		}
	}





	//improve!!
	for(int i = 0;i < ){

		//check if WR time difference lower than set threshold
		if(){
			for(int k = 0;k < iter[tmp_type];++k){
				if(!Event[][]) Event = new Event;
				else Event[][]->Set_Data(DATA);
			}
		}
	}

	//improve!
	for(int i = 0;i < am_events;++i){

	}



	

	
}

//---------------------------------------------------------------

void Time_EventBuilder::get_used_Systems(){
	ifstream data("Configuration_Files/Used_Systems.txt");
	if(data.fail()){
		cerr << "Could not find Used_Systems config file!" << endl;
		exit(0);
	}
	int i = 0;
	int id = 0;
	string line;
	char s_tmp[100];
	while(data.good()){
		getline(data,line,'\n');
		if(line[0] == '#') continue;
		sscanf(line.c_str(),"%s %d",s_tmp,&id);
		Used_Systems[i] = (id == 1)
		i++;
	}
}

//---------------------------------------------------------------