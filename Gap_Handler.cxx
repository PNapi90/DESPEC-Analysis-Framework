#include "Gap_Handler.h"

using namespace std;

//---------------------------------------------------------------

Gap_Handler::Gap_Handler(){
    
    GAPS_Beta_Ev = new int[MAX_GAPS];
    GAPS_Beta_Cluster = new int[MAX_GAPS];

    for(int i = 0;i < MAX_GAPS;++i){
        GAPS_Beta_Ev[i] = -1;
        GAPS_Beta_Cluster[i] = -1;
    }

    GAPS_Strip = new int**[2];
    GAPS_Strip_amount = new int*[2];
    for(int i = 0;i < 2;++i){
        GAPS_Strip[i] = new int*[3];
        GAPS_Strip_amount[i] = new int[3];
        for(int j = 0;j < 3;++j){
            GAPS_Strip_amount[i][j] = 0;
            GAPS_Strip[i][j] = new int[MAX_GAPS];
            for(int k = 0;k < MAX_GAPS;++k) GAPS_Strip[i][j][k] = -1;
        }
    }
    GAPS_Beta_Ev_amount = 0;
    GAPS_Beta_Cluster_amount = 0;
}

//---------------------------------------------------------------

Gap_Handler::~Gap_Handler(){
    for(int i = 0;i < 2;++i){
        for(int j = 0;j < 3;++j) delete[] GAPS_Strip[i][j];
        delete[] GAPS_Strip[i];
        delete[] GAPS_Strip_amount[i];
    }
    delete[] GAPS_Strip_amount;
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

int Gap_Handler::get_Strip_GAP(int i,int j){
    if(GAPS_Strip_amount[i][j] == 0) return -1;

    int value = GAPS_Strip[i][j][GAPS_Strip_amount[i][j]-1];
    GAPS_Strip_amount[i][j]--;

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

void Gap_Handler::set_Strip_Event_GAP(int i,int o,int j){
    GAPS_Strip[i][o][GAPS_Strip_amount[i][o]] = j;
    GAPS_Strip_amount[i][o]++;
}

//---------------------------------------------------------------