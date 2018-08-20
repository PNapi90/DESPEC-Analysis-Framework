#include "Beta_Store.h"

using namespace std;

//---------------------------------------------------------------

Beta_Store::Beta_Store(){
    stored_counter = 0;

    TIMES = new ULong64_t[MAX_AMOUNT];
    ENERGIES = new double[MAX_AMOUNT];
    LENGTHS = new int[MAX_AMOUNT];

    for(int i = 0;i < MAX_AMOUNT;++i){
        TIMES[i] = 0;
        ENERGIES[i] = 0;
        LENGTHS[i] = 0;
    }

    //what else?
}

//---------------------------------------------------------------

Beta_Store::~Beta_Store(){
    delete[] TIMES;
    delete[] ENERGIES;
    delete[] LENGTHS;
}

//---------------------------------------------------------------

void Beta_Store::SAVE(Beta_Clusters* Cluster){
    
    //set Event relevant data
    LENGTHS[stored_counter] = Cluster->get_cluster_size();
    ENERGIES[stored_counter] = Cluster->get_cluster_energy();
    TIMES[stored_counter] = Cluster->get_cluster_time();
    
    //increment amount of stored clusters
    stored_counter++;

}

//---------------------------------------------------------------

void Beta_Store::GET(Raw_Event* RAW){
    //Raw specific stuff

    //Reset data
    for(int i = 0;i < stored_counter;++i){
        TIMES[i] = 0;
        ENERGIES[i] = 0;
        LENGTHS[i] = 0;
    }
    stored_counter = 0;
}

//---------------------------------------------------------------