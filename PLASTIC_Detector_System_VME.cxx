#include "PLASTIC_Detector_System_VME.h"

using namespace std;

//---------------------------------------------------------------

PLASTIC_VME_Detector_System::PLASTIC_VME_Detector_System(){

    // ================= THE SUDIPTA VME VARIABLES ================== //
    
    
    fthr = 50; //100
    
    exit_flg=0;

    raw_data=0;
    energy_data=0;

    card=0;
    energy=0;

    nof_hits=0;

    raw_data_old=0;

    value = 0;	
    value_t = 0;
	
    chNo = 0;
    
    // ===== THE ONE'S I ADDED ======== //
    
    VME_QDC_Data = std::vector<double>(100,0);
    VME_QDC_Channels = std::vector<double>(100,0);
    VME_TDC_Data = std::vector<double>(100,0);
    VME_TDC_Channels = std::vector<double>(100,0);


}

//---------------------------------------------------------------

PLASTIC_VME_Detector_System::~PLASTIC_VME_Detector_System(){

}


//---------------------------------------------------------------

void PLASTIC_VME_Detector_System::get_Event_data(Raw_Event* RAW){
    RAW->set_DATA_PLASTIC_VME(VME_QDC_Data,VME_QDC_Channels,VME_TDC_Data,VME_TDC_Channels,TDC_iterator);
}

//---------------------------------------------------------------


void PLASTIC_VME_Detector_System::Process_MBS(TGo4MbsSubEvent* psubevt){

      
    //get pdata from psubevt (and amount of workds)
    int* pdata = psubevt->GetDataField();
    int lwords = psubevt->GetIntLen();
    

    int EvCounter = 0;
    int geo_ad = 0;
    int Channels = 0;



    pl_data = pdata;

    //***********************************
    //***  CAEN V785 or V775 or V792  ***
    //***********************************

    // module loop
    while(true){

        diff = pl_data - pdata;

        // end of subevent
        if((diff >= lwords) || exit_flg) break;
        
        // read header word
        caen_header = *pl_data++;              

        // get type [2,6] 
        type = (caen_header & TY_MASK) >> 24;    

           // card with data, else =6 
        if (type == 2){

            // get geographical address 
            geo  = (caen_header & GEO_MASK) >> 27;      

            // get number of fired channels
            d_cnt= (caen_header & FCH_MASK) >> 8;        

            /***USELESS VARIABLES WHY ARE THEY HERE??***/
            //unsigned long  data1=0, data2=0, data3; 
            
            for(int jj = 0;jj < d_cnt;++jj){
                data = *pl_data++;
                chan = (data & CH_MASK) >> 16;     // [0..31] 
                data = data & DA_MASK;
                
                VME_QDC_Channels[jj] = chan;
                VME_QDC_Data[jj] = data;

            } // end for(jj)

 
            pl_data++;   // skip EOB word 

            for(int jj = 0;jj < d_cnt;++jj){
                data = *pl_data++;
                chan = (data & CH_MASK) >> 16;     // [0..31] 
                data = data & DA_MASK;

                VME_QDC_Channels[jj] = chan;
                VME_QDC_Data[jj] = data;

            } // end for(jj)
            pl_data++;   // skip EOB word 
        }   // end if(type)
    }   // end module loop while(true)

    //************************
    //***  CAEN V1290 TDC  ***
    //************************

    caen_header = *pdata++;     // read header word 

    type = (caen_header >> 27) & 0x1F;   // get global header ID 

    geo  = caen_header & 0x1F;            // get geographical address 

    TDC_iterator = 0; 

    // scan data until global Trailer
    while(true){   

        data = *pdata++;

        // trigger time tag
        if ((data & 0xF8000000) == 0x88000000) data = *pdata++;

        // global trailer
        if ((data & 0xF8000000) == 0x80000000){
            if (data & 0x07000000) printf("V1290 Global Trailer contains Status Info: 0x%08x\n",(unsigned int)data);
            
            break;  // exit loop
        }

        chan1 = (data & CH_MASK2) >> 21;     // [0..31] 
        data = data & DA_MASK2;

        VME_TDC_Channels[TDC_iterator] = chan1;
        VME_TDC_Data[TDC_iterator] = data;



        ++TDC_iterator;

    } // end while(true)
}
//---------------------------------------------------------------

int* PLASTIC_VME_Detector_System::get_pdata(){
    return pdata;
}

//---------------------------------------------------------------
