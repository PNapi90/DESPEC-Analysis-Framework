#include "PLASTIC_Event.h"

using namespace std;

//---------------------------------------------------------------

PLASTIC_Event::PLASTIC_Event(int* positions_tmp,int length,Raw_Event* RAW) : Events(positions_tmp,length),DATA(),DATA_VME(){
    set_DATA(RAW);
}

//---------------------------------------------------------------

PLASTIC_Event::~PLASTIC_Event(){}

//---------------------------------------------------------------

inline void PLASTIC_Event::set_DATA(Raw_Event* RAW){

    VME_Event = RAW->PLASTIC_CheckVME();

    //Plastic read from VME
    if(VME_Event){
        PLASTIC_VME_DataStruct* P_Tmp = RAW->PassPLASTIC_VME();
        //set event data
        DATA_VME.SetDATA(P_Tmp->TDC_iterator,
                         P_Tmp->VME_QDC_Data,P_Tmp->VME_QDC_Channels,
                         P_Tmp->VME_TDC_Data,P_Tmp->VME_TDC_Channels);
        P_Tmp = nullptr;
    }
    //Plastic read from TAMEX
    else{
        PLASTIC_DataStruct* P_Tmp = RAW->PassPLASTIC();
        
        //set event data
        DATA.SetDATA_Directly(P_Tmp->amount_hit_tamex,P_Tmp->iterator,P_Tmp->trigger_coarse,P_Tmp->trigger_fine,
                     P_Tmp->leading_hits,P_Tmp->trailing_hits,P_Tmp->phys_channel,
                     P_Tmp->leading_hits_ch,P_Tmp->trailing_hits_ch,P_Tmp->Time_Lead,
                     P_Tmp->Time_Trail,P_Tmp->coarse_T_edge_lead,P_Tmp->coarse_T_edge_trail,
                     P_Tmp->fine_T_edge_lead,P_Tmp->fine_T_edge_trail,P_Tmp->ch_ID,P_Tmp->fired_tamex,P_Tmp->PADI_OR_PADIWA);
        P_Tmp = nullptr;
    }
    

}

//---------------------------------------------------------------

void PLASTIC_Event::Write_Event(Tree_Creator* Tree){
	//User defined what variables should be written
}

//---------------------------------------------------------------

PLASTIC_DataStruct* PLASTIC_Event::GET_PLASTIC(){
    return &DATA;
}

//---------------------------------------------------------------

PLASTIC_VME_DataStruct* PLASTIC_Event::GET_PLASTIC_VME(){
    return &DATA_VME;
}

//---------------------------------------------------------------
