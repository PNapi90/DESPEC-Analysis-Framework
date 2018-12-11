#include "FATIMA_Event.h"

using namespace std;

//---------------------------------------------------------------

FATIMA_Event::FATIMA_Event(int* positions_tmp,int length,Raw_Event* RAW) : Events(positions_tmp,length){
    set_DATA(RAW);
}

//---------------------------------------------------------------

FATIMA_Event::~FATIMA_Event(){}

//---------------------------------------------------------------

inline void FATIMA_Event::set_DATA(Raw_Event* RAW){

    FATIMA_DataStruct* P_Tmp = RAW->PassFATIMA();
    //set event data
    DATA.SetDATA_Directly(P_Tmp->FAT_DET_FIRED,P_Tmp->FAT_id,P_Tmp->FAT_E,P_Tmp->FAT_ratio,P_Tmp->FAT_t,
                 P_Tmp->FAT_t_qdc,P_Tmp->FAT_QDCs_FIRED,P_Tmp->FAT_QDC_id,P_Tmp->FAT_QLong,
                 P_Tmp->FAT_QLong_Raw,P_Tmp->FAT_QShort_Raw,P_Tmp->FAT_QDC_t_coarse,
                 P_Tmp->FAT_QDC_t_fine,P_Tmp->FAT_TDCs_FIRED,P_Tmp->FAT_TDC_id,P_Tmp->FAT_TDC_timestamp);
    P_Tmp = nullptr;

}

//---------------------------------------------------------------

double FATIMA_Event::get_energy(){return 0;}

//---------------------------------------------------------------

void FATIMA_Event::Write_Event(Tree_Creator* Tree){
	//User defined what variables should be written
}

//---------------------------------------------------------------

FATIMA_DataStruct* FATIMA_Event::GET_FATIMA(){
    return &DATA;
}

//---------------------------------------------------------------