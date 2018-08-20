#include "Beta_Sorter.h"

using namespace std;

//---------------------------------------------------------------

Beta_Sorter::Beta_Sorter(){
    Beta_Event_Store = new Beta_Store();
}

//---------------------------------------------------------------

Beta_Sorter::~Beta_Sorter(){
    delete Beta_Event_Store;
}

//---------------------------------------------------------------

void Beta_Sorter::set_Strip_Event(int* evt_ids,double Energy,ULong64_t Time){
    bool x_or_y = (evt_ids[0] == 1);
    int z_strip = evt_ids[2];

    //what is empty_spot[evt_ids[0]]?

    //create new Strip Event
    Strip_Events[evt_ids[0]][empty_spot[evt_ids[0]]] = new Strip_Event(x_or_y,evt_ids[1],z_strip,Energy,Time);
    
    //check if x y match of two strip events
    //if yes -> create Beta Event and check for beta decay matches
    if(check_Strip_Events()) check_Beta_Events();

    //check if expired events are present & delete them
    check_Expired();

}

//---------------------------------------------------------------

bool Beta_Sorter::check_Strip_Events(){

    bool new_event = false;
    int beta_Ev_gap = 0;

    //check all previous events of opposite type for matching time and energy difference
    for(int i = 0;i < Strip_Ev_amount[1-evt_ids[0]];++i){
        //events not on same z strip? -> skip
        if(z_strip != Strip_Events[1-evt_ids[0]][i]->get_z_strip()) continue;
        
        //x and y events in predefined time and energy difference?
        if(Strip_Events[1-evt_ids[0]][i]->get_Energy_Time_Difference(Time,Energy)){

            //variables for possible new Beta event
            double Energy_shared = (!x_or_y) ? Energy : 0;
            ULong64_t Time_shared = (!x_or_y) ? Time : 0;
            int x_strip = (!x_or_y) ? evt_ids[1] : -1;

            int* xy = new int[2];

            Time_shared = (Time_shared > 0) ? Time_shared : Strip_Events[0][i]->get_Time();
            Energy_shared = (Energy_shared > 0) ? Energy_shared : Strip_Events[0][i]->get_Energy();
            
            //Beta events are defined by x strip values
            if(x_strip != -1){
                xy[0] = x_strip;
                xy[1] = Strip_Events[1][i]->get_strip();
            }
            else{
                xy[0] = Strip_Events[0][i]->get_strip();
                xy[1] = evt_ids[1];
            }

            //create new Beta event 
            beta_Ev_gap = GAP->get_Beta_Ev_GAP();
            beta_Ev_gap = (beta_Ev_gap != -1) ? beta_Ev_gap : amount_Beta_Events;

            Beta_Events[beta_Ev_gap] = new Beta_Event(xy,Time_shared,Energy_shared);
            Beta_Event_pos = beta_Ev_gap;
            
            if(beta_Ev_gap == amount_Beta_Events) amount_Beta_Events++;

            delete[] xy;
            xy = nullptr;

            delete Strip_Events[0][i];//HERE!!
            delete Strip_Events[]

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
        delta =  (double) (Time_current - Beta_Cluster[i]->get_Time());
        if(abs(delta - offset) < width){
            if(Beta_Cluster[i]->check_XY_Difference(xy)){
                Beta_Events[Beta_Event_pos]->set_Cluster_ID(Beta_Cluster[i]->get_ID());
                cluster_found = true;
                break;
            }
        }
    }
    //no cluster found for Beta_Event -> create new cluster
    if(!cluster_found){
        
        int beta_Cl_gap = GAP->set_Beta_Cluster_GAP();
        beta_Cl_gap = (beta_Cl_gap != -1) ? beta_Cl_gap : amount_Beta_Clusters;
        Beta_Cluster[beta_Cl_gap] = new Beta_Cluster();
        
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
            delta =  (double) (Time_current - Beta_Cluster[i]->get_Time());
            if(delta > TMAX_cluster){
                //save cluster if possible beta decay
                if(Beta_Cluster[i]->Possible_Beta_Decay()) Beta_Event_Store->SAVE(Beta_Cluster[i]);
                
                //delete all related Beta Events in Cluster
                cluster_size = Beta_Cluster[i]->get_cluster_size();
                for(int c = 0;c < cluster_size;++c){
                    xy_ids = Beta_Cluster[i]->get_XY_ID(c);
                    
                    GAP->set_Beta_Ev_GAP(xy_ids);
                    delete Beta_Events[xy_ids];
                    Beta_Events[xy_ids[k]] = nullptr;
                }
                

                GAP->set_Beta_Cluster_GAP(i);

                delete Beta_Cluster[i];
                Beta_Cluster[i] = nullptr;
                xy_ids = nullptr;

                //hole!

            }
        }

        //delete stray Strip_Events
        for(int o = 0;o < 2;++o){
            for(int i = 0;i < Strip_Ev_amount;++i){
                delta =  (double) (Time_current - Beta_Cluster[i]->get_Time());
                if(delta > TMAX_strip){
                    delete Strip_Events[o][];
                    Strip_Events[o][] = nullptr;
                    //hole!

                }
            }
        }
    }
}

//---------------------------------------------------------------