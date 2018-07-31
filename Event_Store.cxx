#include "Event_Store.h"

using namespace std;

//---------------------------------------------------------------

Event_Store::Event_Store(int amount_interest,int* length_interest,int** interest_array){

    this->amount_interest = amount_interest;
    this->length_interest = length_interest;
    this->interest_array = interest_array;

    set_Det_Sys_specific_coincidences();

    tmp_pos = nullptr;

    Event = new Events**[6];
    Event_WR = new ULong64_t*[6];
    Event_position = new int*[6];
    for(int i = 0;i < 6;++i){
        Event[i] = new Events*[MEMORY_LIMIT];
        Event_WR[i] = new ULong64_t[MEMORY_LIMIT];
        Event_position[i] = new int[MEMORY_LIMIT];
        for(int j = 0;j < MEMORY_LIMIT;++j) Event[i][j] = nullptr;
        event_counter[i] = 0;
    }
}

//---------------------------------------------------------------

Event_Store::~Event_Store(){

    //set to nullptr since they will be deleted in Time_EvtBuilder
    length_interest = nullptr;
    interest_array = nullptr;

	int sum_event = 0;
    for(int i = 0;i < 6;++i){
        for(int j = 0;j < MEMORY_LIMIT;++j) if(Event[i][j]) delete Event[i][j];
        delete[] Event[i];
        delete[] Event_WR[i];
        delete[] Event_position[i];

        delete[] sys_interest[i];

        sum_event += event_counter[i];
    }
    cout << "*** Event_Store: from " << MEMORY_LIMIT*6 << " possible objects " << sum_event << " were used ***" << endl;

    delete[] Event;
    delete[] Event_WR;
    delete[] Event_position;
    delete[] sys_interest;
} 

//---------------------------------------------------------------


void Event_Store::store(Raw_Event* RAW){
	int sum_event = 0;
	for(int i = 0;i < 6;++i) sum_event += event_counter[i];

    if(sum_event >= MEMORY_LIMIT*6){
        cerr << "Event_Store MEMORY_LIMIT of " << MEMORY_LIMIT*6.*2./1024. << "MB reached !" << endl;
        cerr << "Check if delete process is working correctly!" << endl;
        cerr << "If it does, you can change the MEMORY_LIMIT by hand in Event_Store.cxx directly (but be careful!)" << endl;
        exit(0);
    }
    int event_type = RAW->get_Event_type();
    create_Event(event_type,RAW);
}

//---------------------------------------------------------------

void Event_Store::show_all_addresses(int type,int p){
    string v;
    for(int i = 0;i < event_counter[type];++i){
        v = (i == p) ? " <-" : " ";
        cout << i << " " << Event[type][i] << " " << Event_position[type][i] << " " << type << v <<  endl;
        cout.flush();
    }
    cout << endl;
}

//---------------------------------------------------------------

void Event_Store::purge(int type,int i){

    //delete event
    cout << "In delete" << endl;
    if(Event[type][i]){

        delete Event[type][i];
        
        Event[type][i] = nullptr;
        Event_WR[type][i] = 0;
        Event_position[type][i] = i;

        //shift last event in list to free memory slot
        //nullify pointer of last event
        if(i < event_counter[type]-1){
            Event[type][i] = Event[type][event_counter[type]-1];
            Event_WR[type][i] = Event_WR[type][event_counter[type]-1];
            Event_position[type][i] = i;//Event_position[type][event_counter[type]-1];
        }
        
        Event[type][event_counter[type]-1] = nullptr;
        Event_WR[type][event_counter[type]-1] = 0;
        Event_position[type][event_counter[type]-1] = i;

        event_counter[type]--;

    }
    else{
        cerr << "Event " << i << " of type " << type << " already nullptr -> undefined behavior!" << endl;
        exit(0);
    }
}

//---------------------------------------------------------------

int Event_Store::Time_Comparison(int type,ULong64_t WR){
    
    int return_value = -1;

    double delta = 0;
    double WR_d = (double) WR;
    
    //cout << "-----------------" << endl;
    //search through event data for smallest time difference
    for(int i = 0;i < event_counter[type];++i){
        delta = (double) abs(WR - Event_WR[type][i]);
        
        //cout << delta/1000. << " ";
        //if(i % 10 == 0 && i > 0) cout << endl;
        
        if(in_time_windows(delta)){
            return_value = i;
            break;
        }
    }
    //cout << endl;
    //cout << "-----------------" << endl;
    return return_value;
}

//---------------------------------------------------------------

inline bool Event_Store::in_time_windows(double delta){
    double offset = 0;
    double width = 1000;
    return (abs(delta - offset)/1000. < width);
}

//---------------------------------------------------------------

void Event_Store::set_Match_ID_address(int type,int* primary_addr,int* match_id_addr){
    int pos = *primary_addr;
    Event[type][pos]->set_Match_ID_address(match_id_addr);
}

//---------------------------------------------------------------

int* Event_Store::get_position(int type){
    return &Event_position[type][event_counter[type]-1];
}

void Event_Store::show_positions(int type){
    cout <<"*** Evt POS ***" << endl;
    for(int i = 0;i < event_counter[type];++i) cout << i << " " << &Event_position[type][i] << " " << Event_position[type][i] << endl;
    cout << "*** *** " << endl;    
}

//---------------------------------------------------------------

void Event_Store::set_permission(int type,int* event_addr,int interest_pos){
    int position = *event_addr;
    cout << "jjj " << type << " " << event_addr << " " << interest_pos << " " << position << endl;
    if(position == -9999){
        purge(type,position);
        return;
    }
    bool full_permission = Event[type][position]->Permission(interest_pos);

    //if all events of interest happend -> delete data
    if(full_permission) purge(type,position);
}

//---------------------------------------------------------------

void Event_Store::Full_Permission(int type,int event_addr){
    int position = Event_position[type][event_addr];
    Event[type][position]->set_Match_ID_address_NULL_ALL();
    purge(type,position);
}

//---------------------------------------------------------------

bool Event_Store::compare_match_ID(int type,int* match_ID,int event_addr){

    //check if Event still exists
    if(Event[type][event_addr]){
        bool correct_match = false;

        int internal_counter = Event[type][event_addr]->get_iterator();
        int** match_ID_Event = Event[type][event_addr]->get_Match_ID_address();
        cout << "COMPARING match_id" << endl;
        //check if match_id still known (should be the case!)
        for(int i = 0;i < internal_counter;++i){
            cout << match_ID << " " << match_ID_Event[i] << endl;
            correct_match = correct_match || (match_ID == match_ID_Event[i]);
        }
        if(!correct_match){
            cerr << "Possible pointer problem in Event_Store::compare_match_ID(...)" << endl;
        }

        match_ID_Event = nullptr;

        return correct_match;
    }
    return false;
}

//---------------------------------------------------------------

void Event_Store::create_Event(int type,Raw_Event* RAW){
    switch(type){
        case 0:
            //Event[0][event_counter[0]] = new FRS_Event(sys_interest[0],iter[0],RAW);
            break;
        case 1:
           // Event[1][event_counter[1]] = new AIDA_Event(sys_interest[1],iter[1],RAW);
            break;
        case 2:
            Event[2][event_counter[2]] = new PLASTIC_Event(sys_interest[2],iter[2],RAW);
            break;
        case 3:
            Event[3][event_counter[3]] = new FATIMA_Event(sys_interest[3],iter[3],RAW);
            break;
        case 4:
            //Event[4][event_counter[4]] = new GALILEO_Event(sys_interest[4],iter[4],RAW);
            break;
        case 5:
            //Event[5][event_counter[5]] = new FINGER_Event(sys_interest[5],iter[5],RAW);
            break;
        default:
            cerr << "Default error in Event_Store switch!" << endl;
            exit(0);
    }
    cout << "- - - - - - - -" << endl;
    cout << "created event " << type << " at " << event_counter[type] << " @ " << Event[type][event_counter[type]] << endl;
    
    ev_pos = event_counter[type];

    Event_WR[type][event_counter[type]] = RAW->get_WR();
    Event_position[type][event_counter[type]] = event_counter[type];
    event_counter[type]++;
}

//---------------------------------------------------------------

void Event_Store::set_Det_Sys_specific_coincidences(){

    sys_interest = new int*[6];

    for(int sys = 0;sys < 6;++sys){
        iter[sys] = 0;
        sys_interest[sys] = new int[10];
        for(int i = 0;i < amount_interest;++i){
            for(int j = 0;j < length_interest[i];++j){
                if(interest_array[i][j] == sys){
                    sys_interest[sys][iter[sys]] = j;
                    iter[sys]++;
                    break;
                }

            }
        }
    }
}
//---------------------------------------------------------------

int Event_Store::get_Match_ID(int type,int pos,int j){
    return Event[type][pos]->get_Match_ID(j);
}

//---------------------------------------------------------------