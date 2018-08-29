#include "AIDA_Processor.h"

using namespace std;

//---------------------------------------------------------------

AIDA_Processor::AIDA_Processor(int amount_z_strips){
    this->amount_z_strips = amount_z_strips;
    amount_thr = amount_z_strips*2;

    Stream = new AIDA_Data_Streamer();

    TX = new TX_Matrix*[amount_z_strips*2];
    for(int i = 0;i < amount_z_strips*2;++i) TX[i] = new TX_Matrix((i % 2 == 1),amount_z_strips,i);

    XY = new XY_Matrix*[amount_z_strips];
    for(int i = 0;i < amount_z_strips;++i) XY[i] = new XY_Matrix(i);

}

//---------------------------------------------------------------

AIDA_Processor::~AIDA_Processor(){
    delete Stream;
    for(int i = 0;i < amount_z_strips*2;++i){
        if(i < amount_z_strips) delete XY[i];
        delete TX[i];
    }
    delete[] XY;
    delete[] TX;
}

//---------------------------------------------------------------

void AIDA_Processor::PROCESSING(bool* x_or_y,int* x_coord,ULong64_t* Time,double* Energy,int* z_strip,int amount){
    //seperate data into xyz of AIDA
    Stream->set_DATA(x_or_y,x_coord,Time,Energy,z_strip,amount);

    thread t[amount_thr];

    //Cluster x/y strips by time, energy and position
    for(int i = 0;i < amount_thr;++i){
        //processing thread t[i]
        t[i] = threading(true,i);
    }

    for(int i = 0;i < amount_thr;++i) t[i].join();

    int thread_iterator = 0;

    //Cluster X and Y of each z plane to possible beta clusters
    for(int i = 0;i < amount_z_strips;++i){
        t[i] = threading(false,thread_iterator);
        thread_iterator += 2;
    }
    
    for(int i = 0;i < amount_z_strips;++i) t[i].join();
}

//---------------------------------------------------------------

void AIDA_Processor::get_DATA(Raw_Event* RAW){
    //get data
    for(int i = 0;i < amount_z_strips;++i) XY[i]->get_DATA(RAW);
}

//---------------------------------------------------------------

thread AIDA_Processor::threading(bool type,int thr_it){
    bool xy_b = (thr_it % 2 == 1);
    if(type){
        double* Etmp = Stream->get_Energy(xy_b,thr_it);
        int* x_tmp = Stream->get_coordinates(xy_b,thr_it);
        ULong64_t T_tmp = Stream->get_Time(xy_b,thr_it);
        int hits_tmp = Stream->get_amount_of_hits(xy_b,thr_it);

        return thread([=] {TX[i]->Process(xy_b,x_tmp,T_tmp,Etmp,hits_tmp,thr_it);});
    }
    else return thread([=] {XY[i]->Process(TX[thr_it],TX[thr_it + 1]);});
}

//---------------------------------------------------------------