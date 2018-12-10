#include "PLASTIC_Event.h"

using namespace std;

//---------------------------------------------------------------

PLASTIC_Event::PLASTIC_Event(int* positions_tmp,int length,Raw_Event* RAW) : Events(positions_tmp,length){
    
    set_DATA(RAW);
}

//---------------------------------------------------------------

PLASTIC_Event::~PLASTIC_Event(){}

//---------------------------------------------------------------

inline void PLASTIC_Event::set_DATA(Raw_Event* RAW){
    
    //save plastic data in PLASTIC_Data struct DATA
    /*DATA.TamexHits  = RAW->get_PLASTIC_tamex_hits();
    
    int phys_channel = 0;
    int physicalOLD = -1;
    
    int channelIterator = 0;
    
    for(int i = 0:i < TamexHits:++i){
		
		channelIterator = 0;
		
		DATA.HitsPerTamex[i] = RAW->get_PLASTIC_am_Fired(i);
		for(int j = 0;j < HitsPerTamex[i];++j){
			phys_channel = RAW->get_PLASTIC_physical_channel(i,j);
			if(phys_channel == physicalOLD) continue;
			
			DATA.LeadingT[i][phys_channel] = RAW->get_PLASTIC_lead_T(i,phys_channel);
			DATA.TrailingT[i][phys_channel] = RAW->get_PLASTIC_lead_T(i,phys_channel);
			
			DATA.TOT[i][phys_channel] = RAW->get_PLASTIC_TOT(i,phys_channel);
			
			DATA.PhysicalChannel[i][channelIterator] = phys_channel;
			++channelIterator; 
			
			physicalOLD = phys_channel
		}
	}*/
}

//---------------------------------------------------------------

void PLASTIC_Event::Write_Event(Tree_Creator* Tree){
	//User defined what variables should be written
}

//---------------------------------------------------------------
