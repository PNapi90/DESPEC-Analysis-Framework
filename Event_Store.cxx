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
    Event_position = new int**[6];

    Address_arr = new int[MEMORY_LIMIT];

    Fill_at = new int*[6];
    Fill_am = new int[6];

    Max_Fill = new int[6];
	tgal = 0;
	tfat = 0;
    for(int i = 0;i < 6;++i){
        Event[i] = new Events*[MEMORY_LIMIT];
        Event_WR[i] = new ULong64_t[MEMORY_LIMIT];
        Event_position[i] = new int*[MEMORY_LIMIT];

        Fill_at[i] = new int[MEMORY_LIMIT];
        Fill_am[i] = 0;

        Max_Fill[i] = 0;

        for(int j = 0;j < MEMORY_LIMIT;++j){
            Event[i][j] = nullptr;
            Event_position[i][j] = nullptr;
            Fill_at[i][j] = -1;
        }
        event_counter[i] = 0;
    }

    for(int i = 0;i < MEMORY_LIMIT;++i) Address_arr[i] = i;

    Verbose_Write = true;
    if(Verbose_Write){
        T_DIFF = new TH1D("FAT_GAL_WR","FAT_GAL_WR",1000,0,1000);
        T_DIFF_Fine = new TH1D("FAT_GAL_WR_f","FAT_GAL_WR_f",50000,0,100000);
        Fine_T2 = new TH1D("t2","t2",50000,0,100000);
        TFILE = new TFile("Root_Trees/Verbose_root.root","RECREATE");
        
        Emat = new TH2D("mat","FAT-GAL Gamma-Gamma",2000,0,4000,500,0,4000);
        Efat = new TH1D("fat","fat",500,0,4000);
        Egal = new TH1D("gal","gal",2000,0,4000);
        Efat_C = new TH1D("fatc","fatc",500,0,2000);
        Egal_o_t = new TH2D("E_T Gal","E_T Gal",5000,0,10000,1000,0,2000);
        e_over_t = new TH2D**[3];
        det_vs_t = new TH2D*[3];
        char name[100];
        for(int i = 0;i < 3;++i){
			e_over_t[i] = new TH2D*[3];
			sprintf(name,"GAL_t_%d",i);
			det_vs_t[i] = new TH2D(name,name,2000,-5000,5000,10,0,10);
			TFILE->Add(det_vs_t[i]);
			for(int j = 0;j < 3;++j){
				if(j <= i) e_over_t[i][j] = nullptr;
				else{
					sprintf(name,"GAL_Det_%d_%d",i,j);
					e_over_t[i][j] = new TH2D(name,name,2000,-1000,1000,2000,0,4000);
					TFILE->Add(e_over_t[i][j]);
				}
			}
			
		}
        
        TFILE->Add(T_DIFF);
        TFILE->Add(T_DIFF_Fine);
        TFILE->Add(Emat);
        TFILE->Add(Efat);
        TFILE->Add(Egal);
        TFILE->Add(Efat_C);
        TFILE->Add(Egal_o_t);
        TFILE->Add(Fine_T2);
    }
    iter_tmp = 0;
    internal_iter = 0;
}

//---------------------------------------------------------------

Event_Store::~Event_Store(){
    
    if(Verbose_Write){
        TFILE->Write();
        //TFILE->Close();
        cout << "WR diff Histograms written" << endl;
    }

    //set to nullptr since they will be deleted in Time_EvtBuilder
    length_interest = nullptr;
    interest_array = nullptr;

	int sum_event = 0;
    for(int i = 0;i < 6;++i){
        for(int j = 0;j < MEMORY_LIMIT;++j){
            if(Event[i][j]) delete Event[i][j];
        }
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
    delete[] Address_arr;
    delete[] Max_Fill;
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
    if(event_counter[type] == 0) return;
    cout << "======================" << endl;
    cout << event_counter[type] << endl;
    cout << "----------------------" << endl;
    for(int i = 0;i < event_counter[type];++i){
        v = (i == p) ? " <-" : " ";
        if(Event[type][i]) cout << i << " " << Event[type][i] << " " << *Event_position[type][i] << " " << Event_position[type][i] << " " << type << v <<  endl;
        else cout << i << " -------------" << endl;
        cout.flush();
    }
    cout << "======================" << endl;
    
}

//---------------------------------------------------------------

void Event_Store::purge(int type,int i){

    //delete event
    if(Event[type][i]){

        delete Event[type][i];
        
        Event[type][i] = nullptr;
        Event_WR[type][i] = 0;
        Event_position[type][i] = nullptr;

        if(i < event_counter[type] - 1){
            Fill_at[type][Fill_am[type]] = i;
            Fill_am[type]++;            
        }

        else event_counter[type]--;

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
    double delta_min = 1000000;

    //search through event data for smallest time difference
    for(int i = 0;i < event_counter[type];++i){
        delta = (double)(WR - Event_WR[type][i]);
        //delta = (WR > Event_WR[type][i]) ? (double)(WR - Event_WR[type][i]) : (double)(Event_WR[type][i] - WR);
        //delta = abs(delta);
        delta_min = (delta_min > abs(delta)) ? delta : delta_min;
        if(in_time_windows(delta)){
            return_value = i;
            break;
        }
    }
    for(int i = 0;i < event_counter[type];++i){
        delta = (double)(WR - Event_WR[type][i]);
        //delta = (WR > Event_WR[type][i]) ? (double)(WR - Event_WR[type][i]) : (double)(Event_WR[type][i] - WR);
        //delta = abs(delta);
        delta_min = (delta_min > abs(delta)) ? delta : delta_min;
    }
    if(Verbose_Write){
         T_DIFF->Fill(delta_min/1000.);
         T_DIFF_Fine->Fill(delta_min);
    }
    return return_value;
}

//---------------------------------------------------------------

inline bool Event_Store::in_time_windows(double delta){
    double offset = 205;
    double width = 30;
    return (abs(delta - offset) < width);
}

//---------------------------------------------------------------

void Event_Store::set_Match_ID_address(int type,int* primary_addr,int* match_id_addr){
    int pos = *primary_addr;
    Event[type][pos]->set_Match_ID_address(match_id_addr);
}

//---------------------------------------------------------------

int* Event_Store::get_position(int type){
    return Event_position[type][ev_pos];
}

//---------------------------------------------------------------

void Event_Store::show_positions(int type){
    cout <<"*** Evt POS ***" << endl;
    for(int i = 0;i < event_counter[type];++i) cout << i << " " << &Event_position[type][i] << " " << Event_position[type][i] << endl;
    cout << "*** *** " << endl;    
}

//---------------------------------------------------------------

void Event_Store::set_permission(int type,int* event_addr,int interest_pos){
    
    int position = *event_addr;
    bool full_permission = Event[type][position]->Permission(interest_pos);

    //if all events of interest happend -> delete data
    if(full_permission) purge(type,position);
}

//---------------------------------------------------------------

void Event_Store::Full_Permission(int type,int event_addr){
    int position = *Event_position[type][event_addr];
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
        //check if match_id still known (should be the case!)
        for(int i = 0;i < internal_counter;++i){
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
    
    bool filled = false;

    int val = 0;
    if(Fill_am[type] == 0) val = event_counter[type];

    else{
        val = Fill_at[type][0];
        Fill_at[type][0] = Fill_at[type][Fill_am[type]-1];
        Fill_am[type]--;

        filled = true;
    }

    switch(type){
        case 0:
            //Event[0][event_counter[0]] = new FRS_Event(sys_interest[0],iter[0],RAW);
            break;
        case 1:
           // Event[1][event_counter[1]] = new AIDA_Event(sys_interest[1],iter[1],RAW);
            break;
        case 2:
            Event[2][val] = new PLASTIC_Event(sys_interest[2],iter[2],RAW);
            break;
        case 3:
            Event[3][val] = new FATIMA_Event(sys_interest[3],iter[3],RAW);
            break;
        case 4:
            Event[4][val] = new GALILEO_Event(sys_interest[4],iter[4],RAW);
            break;
        case 5:
            //Event[5][event_counter[5]] = new FINGER_Event(sys_interest[5],iter[5],RAW);
            break;
        default:
            cerr << "Default error in Event_Store switch!" << endl;
            exit(0);
    }
    
    ev_pos = val;

    Event_WR[type][val] = RAW->get_WR();

    Event_position[type][val] = &Address_arr[val];
    
    if(type == 4){
		unsigned long ttt,ttt2;
		double ttt3,etmp;
		for(int i = 0;i < 3;++i){
			etmp = Event[type][val]->get_energies(i);
			ttt = Event[type][val]->get_time(i);
			for(int j = i+1;j < 3;++j){
				ttt2 = Event[type][val]->get_time(j);
				if(ttt2 != 1337 && ttt != 1337){
					etmp = Event[type][val]->get_energies(i) + Event[type][val]->get_energies(j);
					ttt3 = (double)(ttt2-ttt);
					e_over_t[i][j]->Fill(ttt3,etmp);
					det_vs_t[i]->Fill(ttt3,j);
					break;
				}
			}
		}
	}
    
    if(Fill_am[type] == 0 && !filled) event_counter[type]++;
    
    

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

void Event_Store::reset_Iter(){internal_iter = 0;}

void Event_Store::Write_Energies(int type,int evt_addr){
    double EEE = Event[type][evt_addr]->get_energy();
    if(type == 3 && EEE > 0){
		fat_e = EEE;
		Efat->Fill(EEE);
		tfat = Event_WR[type][evt_addr];
	}
    if(type == 4 && EEE > 0){
		e_gali = EEE;
		Egal->Fill(EEE);
		tgal = Event_WR[type][evt_addr];
		unsigned long ttt,ttt2;
		double ttt3 = 0;
		double etmp = 0;
		for(int i = 0;i < 0;++i){
			etmp = Event[type][evt_addr]->get_energies(i);
			ttt = Event[type][evt_addr]->get_time(i);
			for(int j = i+1;j < 3;++j){
				ttt2 = Event[type][evt_addr]->get_time(j);
				if(ttt2 != 1337 && ttt != 1337){
					etmp = Event[type][evt_addr]->get_energies(i) + Event[type][evt_addr]->get_energies(j);
					ttt3 = (double)(ttt2-ttt);
					e_over_t[i][j]->Fill(ttt3,etmp);
					det_vs_t[i]->Fill(ttt3,j);
					break;
				}
			}
		}
	}
    internal_iter++;
    if(internal_iter == 2){
		if(fat_e > 50) Emat->Fill(e_gali,fat_e);
		if(abs(e_gali - 1332.5) < 5.) Efat_C->Fill(fat_e);
		double del = (double)(tgal - tfat);
		Fine_T2->Fill(del);
		internal_iter = 0;
	}    
}
