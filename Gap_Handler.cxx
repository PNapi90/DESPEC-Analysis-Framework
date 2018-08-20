#include "Gap_Handler.h"

using namespace std;

//---------------------------------------------------------------

Gap_Handler::Gap_Handler(){
    GAPS_Strip = new int[MAX_GAPS];
    GAPS_Beta_Ev = new int[MAX_GAPS];
    GAPS_Beta_Cluster = new int[MAX_GAPS];

    for(int i = 0;i < MAX_GAPS;++i){
        GAPS_Strip[i] = -1;
        GAPS_Beta_Ev[i] = -1;
        GAPS_Beta_Cluster[i] = -1;
    }

    GAPS_Strip_amount = 0;
    GAPS_Beta_Ev_amount = 0;
    GAPS_Beta_Cluster_amount = 0;
}

//---------------------------------------------------------------

Gap_Handler::~Gap_Handler(){
    delete[] GAPS_Strip;
    delete[] GAPS_Beta_Ev;
    delete[] GAPS_Beta_Cluster;
}

//---------------------------------------------------------------

int Gap_Handler::get_Beta_Ev_GAP(){
    if(GAPS_Beta_Ev_amount == 0) return -1;
    
    int value = GAPS_Beta_Ev[GAPS_Beta_Ev_amount-1];

    GAPS_Beta_Ev[GAPS_Beta_Ev_amount-1] = -1;
    GAPS_Beta_Ev_amount--;

    return value;
}

//---------------------------------------------------------------

int Gap_Handler::get_Beta_Cluster_GAP(){
    if(GAPS_Beta_Cluster_amount == 0) return -1;

    int value = GAPS_Beta_Cluster[GAPS_Beta_Cluster_amount-1];

    GAPS_Beta_Cluster[GAPS_Beta_Cluster_amount-1] = -1;
    GAPS_Beta_Cluster_amount--;

    return value;
}

//---------------------------------------------------------------

void Gap_Handler::set_Beta_Ev_GAP(int pos){
    GAPS_Beta_Ev[GAPS_Beta_Ev_amount] = pos;
    GAPS_Beta_Ev_amount++;
}

//---------------------------------------------------------------

void Gap_Handler::set_Beta_Cluster_GAP(int pos){
    GAPS_Beta_Cluster[GAPS_Beta_Cluster_amount] = pos;
    GAPS_Beta_Cluster_amount++;
}

//---------------------------------------------------------------