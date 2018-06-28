#ifndef QDC_TDC_FATIMA_H
#define QDC_TDC_FATIMA_H

//#include "Setup_Specifier.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include "QDC_751.h"
#include "TDC_1290.h"

#include "QDC_FATIMA_Channel.h"
#include "TDC_FATIMA_Channel.h"
#include "FATIMA_DETECTOR.h"

#include "FATIMA_Energy_Calibration.h"
#include "FATIMA_Time_Calibration.h"

#include "FATIMA_Data_Stream.h"
#include "Raw_Event.h"

typedef unsigned long long ULong64_t;

class QDC_TDC_FATIMA{

private:

    //Setup_Specifier* setup;

    int amount_of_FAT_DETS;
    int increase;
    bool exiter;
    int* pdata;

    bool no_data;

    int QDC_amount,TDC_amount;

    int det_num[100][100];
    int det_num_tdc[100][100];


    int fired_Detectors_QDC[100];
    int fired_Detectors_TDC[100];

    int Fired_QDC_Channels[100][2];
    int Fired_TDC_Channels[100][2];

    int fired_Dets;

    int fired_QDC_amount,fired_TDC_amount;

    FATIMA_Time_Calibration* FATIMA_T_CALIB;
    FATIMA_Energy_Calibration* FATIMA_E_CALIB;

    FATIMA_DETECTOR** FATIMA_Detectors;

    QDC_FATIMA_Channel*** QDC_Channels;
    TDC_FATIMA_Channel*** TDC_Channels;

    void load_board_channel_file();
    void Check_QDC_DATA(QDC_Header*);
    void Check_TDC_DATA();
    void reset_fired_channels();
    void Merge_To_Detectors();

    bool wired_QDC(int,int);
    bool wired_TDC(int,int);

    void print_match_error(int);

public:
    QDC_TDC_FATIMA(int,int);
    ~QDC_TDC_FATIMA();
    
    void Run_FATIMA_MBS(int*);

    int get_amount_of_fired_Dets();
    void get_Detector_Data(FATIMA_Data_Stream*);
    void get_Detector_Data(Raw_Event*);

    int get_increase();

    int* get_pdata();
};


#endif
