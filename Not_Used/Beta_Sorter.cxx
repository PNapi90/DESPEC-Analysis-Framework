#include "Beta_Sorter.h"

using namespace std;

//---------------------------------------------------------------

Beta_Sorter::Beta_Sorter(){
    Beta_Event_Store = new Beta_Store();

    Strip_Events = new Strip_Event***[2];
    for(int i = 0;i < 2;++i){
        Strip_Events[i] = new Strip_Events**[3];
        for(int o = 0;o < 3;++o){
            Strip_Events[i][o] = new Strip_Event*[MAX_AMOUNT];
            for(int j = 0;j < MAX_AMOUNT;++j) Strip_Events[i][o][j] = nullptr;
        }
    }
    
    Beta_Events = new Beta_Event*[MAX_AMOUNT];
    Beta_Clusters = new Beta_Cluster*[MAX_AMOUNT];
    for(int i = 0;i < MAX_AMOUNT;++i){
        Beta_Events[i] = nullptr;
        Beta_Clusters[i] = nullptr;
    }
}

//---------------------------------------------------------------

Beta_Sorter::~Beta_Sorter(){
    delete Beta_Event_Store;
    for(int i = 0;i < 3;++i){
        for(int o = 0;o < 2;++o){
            for(int j = 0;j < MAX_AMOUNT;++j){
                if(Strip_Events[i][o][j]) delete Strip_Events[i][o][j];
                if(i == 0 && o == 0){
                    if(Beta_Events[j]) delete Beta_Events[j];
                    if(Beta_Clusters[j]) delete Beta_Clusters[j];
                }
            }
            delete[] Strip_Events[i][o];
        }
        delete[] Strip_Events[i];
    }
    delete[] Strip_Events;
    delete[] Beta_Events;
    delete[] Beta_Clusters;
}

//---------------------------------------------------------------

void Beta_Sorter::set_Strip_Event(int* evt_ids,double Energy,ULong64_t Time){
    bool x_or_y = (evt_ids[0] == 1);
    z_strip = evt_ids[2];

    //what is empty_spot[evt_ids[0]]?
    strip_gap = GAP->get_Strip_GAP(evt_ids[0],z_strip);
    strip_gap = (strip_gap != -1) ? strip_gap : Strip_Ev_amount[evt_ids[0]];
    //create new Strip Event
    Strip_Events[evt_ids[0]][z_strip][strip_gap] = new Strip_Event(x_or_y,evt_ids[1],z_strip,Energy,Time);

    if(strip_gap == Strip_Ev_amount[evt_ids[0]]) Strip_Ev_amount[evt_ids[0]][z_strip]++;
    
    //check if x y match of two strip events
    //if yes -> create Beta Event and check for beta decay matches
    if(check_Strip_Events(evt_ids)) check_Beta_Events();

    //check if expired events are present & delete them
    check_Expired();

}

//---------------------------------------------------------------

bool Beta_Sorter::check_Strip_Events(int* evt_ids){

    bool new_event = false;
    int beta_Ev_gap = 0;

    //check all previous events of opposite type for matching time and energy difference
    for(int i = 0;i < Strip_Ev_amount[1-evt_ids[0]];++i){
        if(!Strip_Events[1-evt_ids[0]][z_strip][i]) continue;
        
        //x and y events in predefined time and energy difference?
        if(Strip_Events[1-evt_ids[0]][z_strip][i]->get_Energy_Time_Difference(Time,Energy)){

            //variables for possible new Beta event
            double Energy_shared = (!x_or_y) ? Energy : 0;
            ULong64_t Time_shared = (!x_or_y) ? Time : 0;
            int x_strip = (!x_or_y) ? evt_ids[1] : -1;

            int* xy = new int[3];

            Time_shared = (Time_shared > 0) ? Time_shared : Strip_Events[0][z_strip][i]->get_Time();
            Energy_shared = (Energy_shared > 0) ? Energy_shared : Strip_Events[0][z_strip][i]->get_Energy();
            
            //Beta events are defined by x strip values
            if(x_strip != -1){
                xy[0] = x_strip;
                xy[1] = Strip_Events[1][z_strip][i]->get_strip();
            }
            else{
                xy[0] = Strip_Events[0][z_strip][i]->get_strip();
                xy[1] = evt_ids[1];
            }
            xy[2] = z_strip;

            //create new Beta event 
            beta_Ev_gap = GAP->get_Beta_Ev_GAP();
            beta_Ev_gap = (beta_Ev_gap != -1) ? beta_Ev_gap : amount_Beta_Events;

            Beta_Events[beta_Ev_gap] = new Beta_Event(xy,Time_shared,Energy_shared);
            Beta_Event_pos = beta_Ev_gap;
            
            if(beta_Ev_gap == amount_Beta_Events) amount_Beta_Events++;

            delete[] xy;
            xy = nullptr;

            delete_Strip_Events(evt_ids[0],i);

            new_event = true;

            break;
        }
    }

    return new_event;

}

//---------------------------------------------------------------

void Beta_Sorter::check_Beta_Events(){
    
    double delta = 0;
    ULong64_t Time_current = Beta_Events[Beta_Event_pos]->get_Time();

    bool cluster_found = false;

    int* xy = Beta_Events[Beta_Event_pos]->get_xy();

    for(int i = 0;i < amount_Beta_Clusters;++i){
        if(!Beta_Events[i]) continue;
        delta =  (double) (Time_current - Beta_Clusters[i]->get_Time());
        if(abs(delta - offset) < width){
            if(Beta_Clusters[i]->check_XY_Difference(xy)){
                Beta_Events[Beta_Event_pos]->set_Cluster_ID(Beta_Clusters[i]->get_ID());
                cluster_found = true;
                break;
            }
        }
    }
    //no cluster found for Beta_Event -> create new cluster
    if(!cluster_found){
        
        int beta_Cl_gap = GAP->get_Beta_Cluster_GAP();
        beta_Cl_gap = (beta_Cl_gap != -1) ? beta_Cl_gap : amount_Beta_Clusters;
        Beta_Clusters[beta_Cl_gap] = new Beta_Cluster();
        
        if(beta_Cl_gap == amount_Beta_Clusters) amount_Beta_Clusters++;
    }


}

//---------------------------------------------------------------

void Beta_Sorter::check_Expired(){
    if(WR - WR_old > max_WR_Difference){
        WR_old = ULONG64_MAX;
        ULong64_t WR_tmp = 0;
        int xy_ids = 0;

        int cluster_size = 0;

        //delete all expired Beta clusters and respective Beta_Events
        for(int i = 0;i < amount_Beta_Clusters;++i){
            if(!Beta_Clusters[i]) continue;
            delta =  (double) (Time_current - Beta_Clusters[i]->get_Time());
            if(delta > TMAX_cluster){
                //save cluster if possible beta decay
                if(Beta_Clusters[i]->Possible_Beta_Decay()) Beta_Event_Store->SAVE(Beta_Clusters[i]);
                
                //delete all related Beta Events in Cluster
                cluster_size = Beta_Clusters[i]->get_cluster_size();
                for(int c = 0;c < cluster_size;++c){
                    xy_ids = Beta_Clusters[i]->get_XY_ID(c);
                    
                    delete Beta_Events[xy_ids];
                    Beta_Events[xy_ids[k]] = nullptr;
                    
                    if(xy_ids-1 < amount_Beta_Events) GAP->set_Beta_Ev_GAP(xy_ids);
                    else amount_Beta_Events--;
                }

                delete Beta_Clusters[i];
                Beta_Clusters[i] = nullptr;
                xy_ids = nullptr;
                
                if(i-1 < amount_Beta_Clusters) GAP->set_Beta_Cluster_GAP(i);
                else amount_Beta_Clusters--;
            }
        }

        //delete stray Strip_Events
        for(int i = 0;i < 2;++i){
            for(int o = 0;o < 3;++o){
                for(int j = 0;j < Strip_Ev_amount;++j){
                    delta =  (double) (Time_current - Strip_Events[i][o][j]->get_Time());
                    if(delta > TMAX_strip){
                        delete Strip_Events[i][o][j];
                        Strip_Events[i][o][j] = nullptr;
                        if(GAP->set_Strip_Event_GAP(i,o,j)) Strip_Ev_amount[i][o]--;
                    }
                }
            }
        }
    }
}

//---------------------------------------------------------------

void Beta_Sorter::delete_Strip_Events(int evt_id,int i){

    delete Strip_Events[1-evt_id][i];
    delete Strip_Events[evt_id][strip_gap];

    Strip_Events[1-evt_id][z_strip][i] = nullptr;
    Strip_Events[evt_id][z_strip][strip_gap] = nullptr;

    if(GAP->set_Strip_Event_GAP(1-evt_id,i)) Strip_Ev_amount[1-evt_id][z_strip]--;
    if(GAP->set_Strip_Event_GAP(evt_id,strip_gap)) Strip_Ev_amount[evt_id][z_strip]--;
}

//---------------------------------------------------------------