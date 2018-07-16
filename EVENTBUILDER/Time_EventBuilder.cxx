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
	
    found_matches = 0;
    for(int i = 0;i < 100;++i) match_amount[i] = 0;

    create_relevance_array();

	check_kinds_overlap();

}

//---------------------------------------------------------------

Time_EventBuilder::~Time_EventBuilder(){

	for(int i = 0;i < amount_interest;++i){
        delete[] interest_array[i];
        delete[] relevance_array[i];
    }
    delete[] relevance_array;
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

void Time_EventBuilder::create_relevance_array(){
    relevance_array = new bool*[6];
    //loop over all 6 systems (FRS,AIDA,...)
    for(int i = 0;i < 6;++i){
        relevance_system[i] = false;
        relevance_array[i] = new bool[amount_interest];
        //loop over amount of user defined coincidence arrays
        for(int j = 0;j < amount_interest;++j){
            relevance_array[i][j] = false;
            //loop over elements in coincidence arrays
            for(int k = 0;k < length_interest[j];++k){
                //detector system i found in coincidence array
                if(i == interest_array[j][k]){
                    //system i is relevant for coincidence analysis
                    relevance_system[i] = true;
                    //system i can be found in coincidence array j
                    relevance_array[i][j] = true;
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

    //pass by pointer to allow dynamic change of position
    int* tmp_data_pos = Event_Stores[tmp_type]->get_position();

    //hits[i] != -1 if hit in system i
	int hits[6];
    int match_ID[6];

    int found_matches = 0;

	//get coincidences (self coincidence not possible)
	for(int i = 0;i < 6;++i){
        hits[i] = -1;
        match_ID[i] = -1;
        if(i != tmp_type){
            //hit id of smallest WR difference of system tmp_type to i
            //if -1 -> no value within threshold window found
            hits[i] = Event_Stores[i]->Time_Comparison(WR);

            //check if coincidence and system relevant for user analysis
            if(hits[i] == -1 || !relevance_system[i]) continue;

            //check Match objects for each relevance array row
            for(int j = 0;j < amount_interest;++j){
                if(relevance_array[i][j]){
                    //get Match id of coincident event
                    match_ID[j] = Event_Stores[i]->get_Match_ID(hits[i],j);
                    
                    //set Match id to new event
                    Event_Stores[tmp_type]->set_Match_ID(tmp_data_pos,match_ID[j]);
                    
                    //set data address in respective event store of tmp_type event
                    Matches[j][match_ID[j]]->set_Data(tmp_type,tmp_data_pos);

                    //check if match is filled
                    if(Matches[j][match_ID[j]]->Full()){

                        //write and delete data
                        Matches[j][match_ID[j]]->Write();

                        


                        delete Matches[j][match_ID[j]];
                        Matches[j][match_ID[j]] = Matches[j][match_amount[j]];
                        Matches[j][match_amount[j]] = NULL;
                    }
                }
            }
            found_matches++;
        }
    }

    //no matches in data found -> create new match object
    if(found_matches == 0){
        //loop over user defined coincidence list
        for(int j = 0;j < amount_interest;++j){
            //if new event's system relevant for analysis
            if(relevance_array[tmp_type][j]){
                //create new Match object with 
                Matches[j][match_amount[j]] = new Match(match_amount,j,tmp_data_pos,interest_array[j]);
                match_amount[j]++;
            }
        }
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