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
    
    double* Etmp = nullptr;
    int* x_tmp = nullptr;
    ULong64_t* T_tmp = nullptr;
    bool xy_b = false;
    int hits_tmp = 0;

    //Cluster x/y strips by time, energy and position
    for(int i = 0;i < amount_thr;++i){
        //x or y strip ? 
        xy_b = (i % 2 == 1);
        
        //data for each strip type
        Etmp = Stream->get_Energy(xy_b,i);
        x_tmp = Stream->get_coordinates(xy_b,i);
        T_tmp = Stream->get_Time(xy_b,i);
        hits_tmp = Stream->get_amount_of_hits(xy_b,i);

        //processing thread t[i]
        t[i] = thread(TX[i]->Process(xy_b,x_tmp,T_tmp,Etmp,hits_tmp,i),i);
    }

    for(int i = 0;i < amount_thr;++i) t[i].join();

    //reset (for safety)
    Etmp = nullptr;
    x_tmp = nullptr;
    T_tmp = nullptr;
    
    int thread_iterator = 0;

    //Cluster X and Y of each z plane to possible beta clusters
    for(int i = 0;i < amount_z_strips;++i){
        t[i] = thread(XY[i]->Process(TX[thread_iterator],TX[thread_iterator + 1]),i);
        thread_iterator += 2;
    }
    
    for(int i = 0;i < amount_z_strips;++i) t[i].join();
}

//---------------------------------------------------------------

void AIDA_Processor::get_DATA(Raw_Event* RAW){
    //get data
}

//---------------------------------------------------------------