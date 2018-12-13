#ifndef PLASTIC_VME_DATA_STRUCT_H
#define PLASTIC_VME_DATA_STRUCT_H

typedef unsigned int UInt;


#include <vector>


struct PLASTIC_VME_DataStruct{

    int TDC_iterator;
    
    std::vector<double> VME_QDC_Data;
    std::vector<double> VME_QDC_Channels;
    std::vector<double> VME_TDC_Data;
    std::vector<double> VME_TDC_Channels;
    
    bool ValidEvent;

    PLASTIC_VME_DataStruct() : TDC_iterator(0),VME_QDC_Data(100,0) , VME_QDC_Channels(100,0) , 
                               VME_TDC_Data(100,0) , VME_TDC_Channels(100,0) , ValidEvent(true) {}

    ~PLASTIC_VME_DataStruct(){}

    

    void SetDATA(int _TDC_iterator,std::vector<double> &_VME_QDC_Data,std::vector<double> &_VME_QDC_Channels,
                 std::vector<double> &_VME_TDC_Data,std::vector<double> &_VME_TDC_Channels)
    {
        if(_TDC_iterator > 100){
            TDC_iterator = 0;
            ValidEvent = false;
            return;
        }
        ValidEvent = true;

        for(int i = 0;i < 100;++i){
            VME_QDC_Data[i] = _VME_QDC_Data[i];
            VME_QDC_Channels[i] = _VME_QDC_Channels[i];
            VME_TDC_Data[i] = _VME_TDC_Data[i];
            VME_TDC_Channels[i] = _VME_TDC_Channels[i];
        }

        TDC_iterator = _TDC_iterator;
    }
};

#endif
