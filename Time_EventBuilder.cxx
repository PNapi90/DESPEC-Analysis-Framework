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
    
    FileSystem = nullptr;
    Trees = nullptr;

    Matches = new Match**[amount_interest];
    for(int i = 0;i < amount_interest;++i){
        Matches[i] = new Match*[MEMORY_LIMIT];
        for(int j = 0;j < MEMORY_LIMIT;++j) Matches[i][j] = nullptr;
    }

    found_matches = 0;
    for(int i = 0;i < 100;++i) match_amount[i] = 0;

    create_relevance_array();

    for(int i = 0;i < 6;++i) hits[i] = -1;
    for(int i = 0;i < 50;++i) match_ID[i] = -1;

    //set event storage
    Event_Storage = new Event_Store(amount_interest,length_interest,interest_array);

    expired_counter = 0;
    first_event = true;

    //create Root Trees for all coincidences of interest
    create_used_Trees();


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
    cout << "Time_EventBuilder deleted!" << endl;

}

//---------------------------------------------------------------

void Time_EventBuilder::create_relevance_array(){

    std::vector<std::string> relevance_name_tmp(amount_interest,"Root_Trees/");
    std::string name_list[6] = {"FRS","AIDA","PLASTIC","FATIMA","GALILEO","FINGER"};

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
                    relevance_name_tmp[j] += name_list[i] + "_";
                    break;
                }
            }
        }
    }
    //.root ending for files
    for(int i = 0;i < amount_interest;++i) relevance_name_tmp[i] += ".root";



    //Root files with user-defined histograms for
    //each type of coincidences of interest
    FileSystem = new TFile*[amount_interest];
    for(int i = 0;i < amount_interest;++i){
        FileSystem[i] = new TFile(relevance_name_tmp[i].c_str(),"RECREATE");
    }

}

//---------------------------------------------------------------

void Time_EventBuilder::set_Event(Raw_Event* RAW){
    //get PrcID from RAW
    int tmp_type = RAW->get_Event_type();

    if(!relevance_system[tmp_type]) return;

    //PrcID check
    if(tmp_type > 5 || tmp_type < 0){
        cerr << "Unknown PrcID in Time_EventBuilder. Raw_Event sent wrong ID: " << tmp_type << endl;
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

    for(int i = 0;i < 50;++i) match_ID[i] = -1;

    int found_matches = 0;
    int* match_id_ptr = nullptr;

    int match_hits = 0;
    bool expired = false;
    int** hit_addresses = nullptr;
    int* hit_types = nullptr;
    int* filled_types = nullptr;

    //get coincidences (self coincidence not possible)
    for(int i = 0;i < 6;++i){
        //hits[i] != -1 if hit in system i
        hits[i] = -1;
        match_ID[i] = -1;

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
                    match_ID[j] = Event_Storage->get_Match_ID(i,hits[i],j);

                    //pointer on MatchID (allows for dynamic change of Match address)
                    match_id_ptr = Matches[j][match_ID[j]]->get_Address();

                    //set Match id to new event
                    Event_Storage->set_Match_ID_address(tmp_type,tmp_data_pos,match_id_ptr);

                    //set data address in respective event store of tmp_type event
                    Matches[j][match_ID[j]]->set_Data(tmp_type,tmp_data_pos);

                    //check if match is filled
                    if(Matches[j][match_ID[j]]->Full()){
                        //write data (directly to histogram or Root Tree)
                        Event_Storage->Write(Matches[j][match_ID[j]],FileSystem[j]);

                        //get delete permission for Event_Store data
                        get_DELETE_Permission(j,match_ID[j]);
                        
                        //delete coincident Match
                        delete Matches[j][match_ID[j]];
                        Matches[j][match_ID[j]] = nullptr;

                        //fill empty hole in Match data and reset address variables
                        if(match_ID[j] < match_amount[j]-1){
                            Matches[j][match_ID[j]] = Matches[j][match_amount[j]-1];
                            Matches[j][match_ID[j]]->set_Address(match_ID[j]);
                        }

                        //last event pointing to NULL
                        Matches[j][match_amount[j]-1] = nullptr;
                        match_amount[j]--;
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
                Event_Storage->set_Match_ID_address(tmp_type,tmp_data_pos,match_id_ptr);

                hit_addresses = Matches[j][match_amount[j]]->get_Address_Array();
                match_hits = Matches[j][match_amount[j]]->get_amount_Hits();
                filled_types = Matches[j][match_amount[j]]->get_filled_types();
                
                match_amount[j]++;
            }
        }
    }

    //too old data has to be deleted
    int k = 0;

    ULong64_t max_WR_Difference = 10000*1e3;

    if(WR - WR_old > max_WR_Difference){

        WR_old = ULONG64_MAX;
        ULong64_t WR_tmp = 0;
        for(int j = 0;j < amount_interest;++j){
            k = 0;
            while(k < match_amount[j]){
                //check if Match event is already expired               
                expired = Matches[j][k]->Check_Time(WR,interest_array[j][k]);
                if(expired){
                    //get amount of hits and types in Match
                    match_hits = Matches[j][k]->get_amount_Hits();
                    hit_addresses = Matches[j][k]->get_Address_Array();
                    hit_types = Matches[j][k]->get_hit_types();
                    filled_types = Matches[j][k]->get_filled_types();
                    match_id_ptr = Matches[j][k]->get_Address();

                    //loop over all events in Match
                    for(int o = 0;o < match_hits;++o){
                        if(filled_types[o] == -1) continue;
                        //get match_id pointer to compare, if Event already deleted
                        if(Event_Storage->compare_match_ID(filled_types[o],match_id_ptr,*hit_addresses[filled_types[o]])){
                            Event_Storage->Full_Permission(filled_types[o],*hit_addresses[filled_types[o]]);
                        }
                    }
                    delete Matches[j][k];
                    Matches[j][k] = nullptr;
                    
                    //fill empty hole in Match data and reset address variables
                    if(k < match_amount[j]-1){
                        Matches[j][k] = Matches[j][match_amount[j]-1];
                        Matches[j][k]->set_Address(k);
                    }
                    
                    //last event pointing to NULL
                    Matches[j][match_amount[j]-1] = nullptr;
                    //decrease amount of current Matches
                    match_amount[j]--;
                    expired_counter--;
                }
                //only increase if no expiration
                else k++;
            }
        }
        //get "oldest" event defined by WR (smallest WR)
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
        cerr << "Could not find Used_Systems configuration file!" << endl;
        exit(0);
    }
    int i = 0;
    int id = 0;
    string line;
    char s_tmp[100];
    while(getline(data,line,'\n')){
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
    
    //loop over all elements in interest array
    for(int i = 0;i < length_interest[j];++i){
        //get Detector system type
        type = interest_array[j][i];
        //set delete permission from Match with id {j,match_ID}
        Event_Storage->set_permission(type,event_address_array[type],j);
    }
}

//---------------------------------------------------------------

void Time_EventBuilder::create_used_Trees(){
    
    //temporary type of coincidence
    int type = 0;

    //create n = amount_interest Tree arrays for all coincidences
    Trees = new TTree*[amount_interest];
    for(int i = 0;i < amount_interest;++i){
        Trees[i] = new TTree(InterestNames[i].c_str(),InterestNames[i].c_str());
        for(int j = 0;j < length_interest[i];++i){
            //get detector system type of new tree
            type = interest_array[i][j];
            //add relevant branches to Tree
            CreateBranches(type,i);
        }
    }
}

//---------------------------------------------------------------

inline void Time_EventBuilder::CreateBranches(int type,int i){
    switch(type){
        case 0:
            FRS_Branch_Creator Branch(Trees[i]);
            break;
        case 1:
            AIDA_Branch_Creator Branch(Trees[i]);
            break;
        case 2:
            PLASTIC_Branch_Creator Branch(Trees[i]);
            break;
        case 3:
            FATIMA_Branch_Creator Branch(Trees[i]);
            break;
        case 4:
            GALILEO_Branch_Creator Branch(Trees[i]);
            break;
        case 5:
            FINGER_Branch_Creator Branch(Trees[i]);
            break;
        default:
            cerr << "Unknown Branch_Creator type " << type << " in Time_EventBuilder" << endl;
            exit(1);
    }
}

//---------------------------------------------------------------
