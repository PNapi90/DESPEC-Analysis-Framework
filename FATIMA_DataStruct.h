#ifndef FATIMA_DATA_STRUCT_H
#define FATIMA_DATA_STRUCT_H


#include <vector>

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

    FATIMA_DataStruct() : FAT_DET_FIRED(0) , FAT_id(100,0),FAT_E(100,0),FAT_ratio(100,0),FAT_t(100,0),FAT_t_qdc(100,0),
                          FAT_QDCs_FIRED(0) , FAT_QDC_id(100,0),FAT_QLong(100,0),FAT_QLong_Raw(100,0),FAT_QShort_Raw(100,0),
                          FAT_QDC_t_coarse(100,0),FAT_QDC_t_fine(100,0),FAT_TDCs_FIRED(0),FAT_TDC_id(100,0),FAT_TDC_timestamp(100,0)
    {}



    SetDATA(int QDC_FIRED,int TDC_FIRED,std::vector<double> &Ql_Raw,std::vector<double> &Qs_Raw,
            std::vector<double> &Ql,std::vector<ULong64_t> &TDC,std::vector<double> &TDC_ns,
            std::vector<ULong64_t> &QDC_c,std::vector<double> &QDC_f,std::vector<int> &det_ids_QDC,
            std::vector<int> &det_ids_TDC)
    {
        FAT_QDCs_FIRED = QDC_FIRED;
        FAT_TDCs_FIRED = TDC_FIRED;
        int dets_fired = 0;
        for (int i=0; i<QDC_FIRED; i++) {
            FAT_QDC_id[i] = det_ids_QDC[i];
            if (det_ids_QDC[i] == 35) cout<<"I am in QDC"<<endl;
            FAT_QLong[i]  = Ql[i];
            FAT_QLong_Raw[i]  = Ql_Raw[i];
            FAT_QShort_Raw[i] = Qs_Raw[i];
            FAT_QDC_t_coarse[i] = QDC_c[i];
            FAT_QDC_t_fine[i] = QDC_f[i];
            for (int j=0; j<TDC_FIRED; j++) {
                if (det_ids_QDC[i] == det_ids_TDC[j]) {
                    FAT_id[dets_fired] = det_ids_QDC[i];
                    if (det_ids_TDC[j] == 35) cout<<"I am in TDC"<<endl;

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