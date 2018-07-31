#include "Time_EventBuilder.h"

using namespace std;

//---------------------------------------------------------------

Time_EventBuilder::Time_EventBuilder(int amount_interest,int* length_interest_tmp,int** interest_array_tmp){

    get_used_Systems();

    //get coincidences between Systems of interest defined by interest_array
    this->amount_interest = amount_interest;
    length_interest = new int[amount_interest];
    interest_array = new int*[amount_interest];
    for(int i = 0;i < amount_interest;++i){
    length_interest[i] = length_interest_tmp[i];
        interest_array[i] = new int[length_interest[i]];
        for(int j = 0;j < length_interest[i];++j) interest_array[i][j] = interest_array_tmp[i][j];
    }
    
    Matches = new Match**[amount_interest];
    for(int i = 0;i < amount_interest;++i){
        Matches[i] = new Match*[MEMORY_LIMIT];
        for(int j = 0;j < MEMORY_LIMIT;++j) Matches[i][j] = nullptr;
    }

    found_matches = 0;
    for(int i = 0;i < 100;++i) match_amount[i] = 0;

    create_relevance_array();

    //set event storage
    Event_Storage = new Event_Store(amount_interest,length_interest,interest_array);

    expired_counter = 0;
    first_event = true;

}

//---------------------------------------------------------------

Time_EventBuilder::~Time_EventBuilder(){

    delete Event_Storage;

    for(int i = 0;i < amount_interest;++i){
        delete[] interest_array[i];
        delete[] relevance_array[i];
        for(int j = 0;j < MEMORY_LIMIT;++j) if(Matches[i][j]) delete Matches[i][j];
        delete[] Matches[i];
    }
    delete[] Matches;
    delete[] relevance_array;
    delete[] interest_array;
    delete[] length_interest;

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
    int tmp_type = RAW->get_Event_type();

    if(!relevance_system[tmp_type]) return;

    //PrcID check
    if(tmp_type > 5 || tmp_type < 0){
        cerr << "Unknown PrcID in Time_EventBuilder. Raw_Event sent wrong ID" << endl;
        exit(0);
    }

    ULong64_t WR = RAW->get_WR();
    if(first_event){
        WR_old = WR;
        first_event = false;
    }

    //save event in respective store
    Event_Storage->store(RAW);

    //pass by pointer to allow dynamic change of position
    int* tmp_data_pos = Event_Storage->get_position(tmp_type);

    //hits[i] != -1 if hit in system i
    int hits[6];
    int match_ID[50];

    int found_matches = 0;
    int* match_id_ptr = nullptr;

    int match_hits = 0;
    bool expired = false;
    int** hit_addresses = nullptr;
    int* hit_types = nullptr;
    int* filled_types = nullptr;

    //get coincidences (self coincidence not possible)
    for(int i = 0;i < 6;++i){
        hits[i] = -1;
        match_ID[i] = -1;
        continue;
        if(i != tmp_type && relevance_system[i]){
            //hit id of smallest WR difference of system tmp_type to i
            //if -1 -> no value within threshold window found
            hits[i] = Event_Storage->Time_Comparison(i,WR);

            //check if coincidence and system relevant for user analysis
            if(hits[i] == -1) continue;

            //check Match objects for each relevance array row
            for(int j = 0;j < amount_interest;++j){
                if(relevance_array[i][j]){
                    //get Match id of coincident event
                    cout << "get match id " << i << " " << hits[i] << " " << j << endl;
                    match_ID[j] = Event_Storage->get_Match_ID(i,hits[i],j);
                    cout << "-> " << match_ID[j] << endl;
                    //pointer on MatchID (allows for dynamic change of Match address)
                    match_id_ptr = Matches[j][match_ID[j]]->get_Address();
                    cout << "-> ID " << match_id_ptr << endl;

                    //set Match id to new event
                    Event_Storage->set_Match_ID_address(tmp_type,tmp_data_pos,match_id_ptr);
                    
                    //set data address in respective event store of tmp_type event
                    Matches[j][match_ID[j]]->set_Data(tmp_type,tmp_data_pos);

                    //check if match is filled
                    if(Matches[j][match_ID[j]]->Full()){
                        cout << "Match filled" << endl;
                        //write and delete data
                        Matches[j][match_ID[j]]->Write();

                        //get delete permission for Event_Store data
                        get_DELETE_Permission(j,match_ID[j]);
                        cout << "mmm " << match_ID[j] << " " << match_amount[j] << endl;
                        
                        delete Matches[j][match_ID[j]];
                        Matches[j][match_ID[j]] = nullptr;

                        //fill empty hole in Match data and reset address variables
                        if(match_ID[j] < match_amount[j]-1){
                            Matches[j][match_ID[j]] = Matches[j][match_amount[j]-1];
                            Matches[j][match_ID[j]]->set_Address(match_ID[j]);
                        }

                        //last event pointing to nullptr
                        Matches[j][match_amount[j]-1] = nullptr;
                        match_amount[j]--;

                        cout << "new " << match_amount[j] << " " << Matches[j][match_ID[j]] << " " << Matches[j][match_amount[j]] << endl;
                        for(int p = 0;p < match_amount[j];++p) cout << Matches[j][p]->get_Address() << " ";
                        cout << endl;
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
                //create new Match object
                Matches[j][match_amount[j]] = new Match(match_amount[j],tmp_type,tmp_data_pos,
                                                        interest_array[j],length_interest[j],WR);

                //get its address and send it to Event_Storage
                match_id_ptr = Matches[j][match_amount[j]]->get_Address();
                cout << j <<" -> Data Address " << *tmp_data_pos << " match am " << match_amount[j] << " " << *match_id_ptr<< endl;
                Event_Storage->set_Match_ID_address(tmp_type,tmp_data_pos,match_id_ptr);

                hit_addresses = Matches[j][match_amount[j]]->get_Address_Array();
                match_hits = Matches[j][match_amount[j]]->get_amount_Hits();
                filled_types = Matches[j][match_amount[j]]->get_filled_types();
                cout << "Jj " << j << " Kj " << match_amount[j] << endl;
                for(int o = 0;o < match_hits;++o) cout << filled_types[o]<< " " << *hit_addresses[filled_types[o]] << endl;
                cout << endl;

                match_amount[j]++;
            }
        }
    }

    //too old data has to be deleted
    

    int k = 0;

    ULong64_t max_WR_Difference = 10000*1e3;

    if(expired_counter == 5){//WR - WR_old > max_WR_Difference){

        WR_old = ULONG64_MAX;
        ULong64_t WR_tmp = 0;
        for(int j = 0;j < amount_interest;++j){
            k = 0;
            while(k < match_amount[j]){
                //check if Match event is already expired
                //=> difference of WR of Match to current WR too large
                Event_Storage->show_all_addresses(interest_array[j][0],k);
                Event_Storage->show_all_addresses(interest_array[j][1],k);
                Event_Storage->show_positions(interest_array[j][1]);

                hit_addresses = Matches[j][k]->get_Address_Array();
                match_hits = Matches[j][k]->get_amount_Hits();
                filled_types = Matches[j][k]->get_filled_types();
                cout << "J " << j << " K " << k << endl;
                for(int o = 0;o < match_hits;++o) cout << filled_types[o]<< " " << *hit_addresses[filled_types[o]] << endl;
                cout << endl;

                cout << "--------" << endl;
                for(int o = 0;o < match_amount[j];++o){
                    cout << o << " " << Matches[j][o]->get_Address() << endl;
                }
                cout << "--------" << endl;
                expired = Matches[j][k]->Check_Time(WR,interest_array[j][k]);
                if(expired){
                    //get amount of hits and types in Match
                    match_hits = Matches[j][k]->get_amount_Hits();
                    hit_addresses = Matches[j][k]->get_Address_Array();
                    hit_types = Matches[j][k]->get_hit_types();
                    filled_types = Matches[j][k]->get_filled_types();
                    match_id_ptr = Matches[j][k]->get_Address();

                    for(int o = 0;o < 6;++o){
                        cout << hit_addresses[o] << " ";
                    }
                    cout << " -> " << hit_types[0] << " " << hit_types[1] << endl;
                    cout << match_hits << " " << match_id_ptr << endl;
                    cout <<"f-> ";
                    for(int o = 0;o < match_hits;++o) cout << filled_types[o]<< " " << *hit_addresses[filled_types[o]] << endl;
                    cout << endl;

                    //loop over all events in Match
                    for(int o = 0;o < match_hits;++o){
                        cout << "fff " << filled_types[o] << " " << match_id_ptr << " " << *match_id_ptr << " " << hit_addresses[filled_types[o]]  << " " << *hit_addresses[filled_types[o]] << endl;
                        if(filled_types[o] == -1) continue;
                        //get match_id pointer to compare, if Event already deleted
                        if(Event_Storage->compare_match_ID(filled_types[o],match_id_ptr,*hit_addresses[filled_types[o]])){
                            Event_Storage->Full_Permission(filled_types[o],*hit_addresses[filled_types[o]]);
                        }
                    }
                    cout << "MATCH " << Matches[j][k]->get_Address() << " NULLED; total " << match_amount[j]-1 << endl;
                    cout << Matches[j][k]->get_Address() << " ";
                    delete Matches[j][k];
                    Matches[j][k] = nullptr;

                    //cout << "-> delete " << k << " " << j << " " << match_amount[j]<<endl;
                    //fill empty hole in Match data and reset address variables
                    if(k < match_amount[j]-1){
                        Matches[j][k] = Matches[j][match_amount[j]-1];
                        Matches[j][k]->set_Address(k);
                        for(int o = 0;o < 6;++o){
                            if(hit_addresses[o]) cout << o << " " <<*hit_addresses[o] << endl;
                        }
                        cout << "bbb" << endl;
                    }
                    
                    //last event pointing to NULL
                    Matches[j][match_amount[j]-1] = nullptr;
                    //decrease amount of current Matches
                    match_amount[j]--;
                    expired_counter--;
                }
                //only increase if no expiration
                else{
                    Matches[j][k]->set_Address(k);
                    match_hits = Matches[j][k]->get_amount_Hits();
                    hit_addresses = Matches[j][k]->get_Address_Array();
                    hit_types = Matches[j][k]->get_hit_types();
                    filled_types = Matches[j][k]->get_filled_types();
                    match_id_ptr = Matches[j][k]->get_Address();
                    cout << "accepted!" << endl;
                    cout <<"af-> ";
                    for(int o = 0;o < match_hits;++o) cout << filled_types[o]<< " " << *hit_addresses[filled_types[o]] << endl;
                    cout << endl;

                    k++;
                }
                cout << "\n1\n-a-a-a-a-a-a-a-" << endl;
                Event_Storage->show_all_addresses(interest_array[j][0],k);
                Event_Storage->show_all_addresses(interest_array[j][1],k);
                cout << "-a-a-a-a-a-a-a-\n2" << endl;
                
            }

        }
        for(int j = 0;j < amount_interest;++j){
            for(int o = 0;o < k;++o){
                WR_tmp = Matches[j][o]->get_WR();
                WR_old = (WR_tmp < WR_old) ? WR_tmp : WR_old;
            }
        }

    }
    expired_counter++;
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
		Used_Systems[i] = (id == 1);
		i++;
	}
}

//---------------------------------------------------------------

void Time_EventBuilder::get_DELETE_Permission(int j,int match_ID){
    //type of detector system
    int type = -1;

    //get all addresses of data in Matches[j][match_ID] stored in 
    //respective Event_Storage ()
    int** event_address_array = Matches[j][match_ID]->get_Address_Array();
    for(int i = 0;i < 6;++i) cout << event_address_array[i] << " ";// << event_address_array[1] << endl;
    cout << " <--> "<< length_interest[j]  << endl;

    //loop over all elements in interest array
    for(int i = 0;i < length_interest[j];++i){
        //get Detector system type
        type = interest_array[j][i];
        //set delete permission from Match with id {j,match_ID}
        Event_Storage->set_permission(type,event_address_array[type],j);
    }
}

//---------------------------------------------------------------
