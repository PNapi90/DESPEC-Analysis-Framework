#ifndef FATIMA_DATA_STRUCT_H
#define FATIMA_DATA_STRUCT_H

#include <iostream>
#include <vector>

typedef unsigned long ULong;
typedef unsigned long long ULong64_t;

struct FATIMA_DataStruct{

    int    FAT_DET_FIRED;         //number of completed detectors in evt
    std::vector<int>    FAT_id;           //id according to allocation file
    std::vector<double> FAT_E;            //energy (calibrated nad gain matched)
    std::vector<double> FAT_ratio;        //Qshort/Qlong
    std::vector<double> FAT_t;            //tdc time  (ns, shifted)
    std::vector<double> FAT_t_qdc;        //qdc time  (ns, shifted)
    //For trouble debugging:
    int    FAT_QDCs_FIRED;
    std::vector<int>    FAT_QDC_id;
    std::vector<double> FAT_QLong;        //calibrated
    std::vector<double> FAT_QLong_Raw;
    std::vector<double> FAT_QShort_Raw;
    std::vector<ULong>  FAT_QDC_t_coarse;
    std::vector<double> FAT_QDC_t_fine;  //qdc time (ns)
    //
    int    FAT_TDCs_FIRED;
    std::vector<int>    FAT_TDC_id;
    std::vector<double> FAT_TDC_timestamp;//tdc time raw

    FATIMA_DataStruct() : FAT_DET_FIRED(0),FAT_id(100,0),FAT_E(100,0),FAT_ratio(100,0),FAT_t(100,0),FAT_t_qdc(100,0),
                          FAT_QDCs_FIRED(0),FAT_QDC_id(100,0),FAT_QLong(100,0),FAT_QLong_Raw(100,0),FAT_QShort_Raw(100,0),
                          FAT_QDC_t_coarse(100,0),FAT_QDC_t_fine(100,0),FAT_TDCs_FIRED(0),FAT_TDC_id(100,0),FAT_TDC_timestamp(100,0)
    {}

    void SetDATA_Directly(int _FAT_DET_FIRED,std::vector<int> &_FAT_id,std::vector<double> &_FAT_E,std::vector<double> &_FAT_ratio,
                     std::vector<double> &_FAT_t,std::vector<double> &_FAT_t_qdc,int _FAT_QDCs_FIRED,std::vector<int> &_FAT_QDC_id,
                     std::vector<double> &_FAT_QLong,std::vector<double> &_FAT_QLong_Raw,std::vector<double> &_FAT_QShort_Raw,
                     std::vector<ULong> &_FAT_QDC_t_coarse,std::vector<double> &_FAT_QDC_t_fine,int _FAT_TDCs_FIRED,
                     std::vector<int> &_FAT_TDC_id,std::vector<double> &_FAT_TDC_timestamp)
    {
        FAT_DET_FIRED = _FAT_DET_FIRED;
        FAT_TDCs_FIRED = _FAT_TDCs_FIRED;
        FAT_QDCs_FIRED = _FAT_QDCs_FIRED;

        for(int i = 0;i < 100;++i){
            FAT_id[i] = _FAT_id[i];
            FAT_E[i] = _FAT_E[i];
            FAT_ratio[i] = _FAT_ratio[i];
            FAT_t[i] = _FAT_t[i];
            FAT_t_qdc[i] = _FAT_t_qdc[i];
            
            FAT_QDC_id[i] = _FAT_QDC_id[i];
            FAT_QLong[i] = _FAT_QLong[i];
            FAT_QLong_Raw[i] = _FAT_QLong[i];
            FAT_QShort_Raw[i] = _FAT_QShort_Raw[i];
            FAT_QDC_t_coarse[i] = _FAT_QDC_t_coarse[i];
            FAT_QDC_t_fine[i] = _FAT_QDC_t_fine[i];
            FAT_TDC_id[i] = _FAT_TDC_id[i]; 
            FAT_TDC_timestamp[i] = _FAT_TDC_timestamp[i];
        }
    }


    void SetDATA(int QDC_FIRED,int TDC_FIRED,std::vector<double> &Ql_Raw,std::vector<double> &Qs_Raw,
            std::vector<double> &Ql,std::vector<ULong64_t> &TDC,std::vector<double> &TDC_ns,
            std::vector<ULong64_t> &QDC_c,std::vector<double> &QDC_f,std::vector<int> &det_ids_QDC,
            std::vector<int> &det_ids_TDC)
    {
        FAT_QDCs_FIRED = QDC_FIRED;
        FAT_TDCs_FIRED = TDC_FIRED;
        int dets_fired = 0;
        for (int i=0; i<QDC_FIRED; i++) {
            FAT_QDC_id[i] = det_ids_QDC[i];
            if (det_ids_QDC[i] == 35) std::cout<<"I am in QDC"<<std::endl;
            FAT_QLong[i]  = Ql[i];
            FAT_QLong_Raw[i]  = Ql_Raw[i];
            FAT_QShort_Raw[i] = Qs_Raw[i];
            FAT_QDC_t_coarse[i] = QDC_c[i];
            FAT_QDC_t_fine[i] = QDC_f[i];
            for (int j=0; j<TDC_FIRED; j++) {
                if (det_ids_QDC[i] == det_ids_TDC[j]) {
                    FAT_id[dets_fired] = det_ids_QDC[i];
                    if (det_ids_TDC[j] == 35) std::cout<<"I am in TDC"<<std::endl;

                    FAT_E[dets_fired] = Ql[i];
                    FAT_ratio[dets_fired] = (double) Qs_Raw[i]/Ql_Raw[i];
                    FAT_t[dets_fired] = TDC_ns[j];
                    FAT_t_qdc[dets_fired] = QDC_c[i];
                    dets_fired++;
                }
            }
        }
        FAT_DET_FIRED = dets_fired;
    
        for (int i=0; i<TDC_FIRED; i++) {
            FAT_TDC_id[i] = det_ids_TDC[i];
            FAT_TDC_timestamp[i] = TDC[i];
        }
    }

};



#endif