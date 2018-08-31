#include "AIDA_Processor.h"

using namespace std;

//---------------------------------------------------------------

AIDA_Processor::AIDA_Processor(int amount_z_strips){
    this->amount_z_strips = amount_z_strips;
    
    USE_THREADS = false;
    am_sub_threads = 1;

    check_Thread_Use();

    if(USE_THREADS) amount_thr = amount_z_strips*2;
    else amount_thr = 1;

    Stream = new AIDA_Data_Streamer();

    TX = new TX_Matrix*[amount_z_strips*2];
    for(int i = 0;i < amount_z_strips*2;++i) TX[i] = new TX_Matrix(i,am_sub_threads);

    XY = new XY_Matrix*[amount_z_strips];
    for(int i = 0;i < amount_z_strips;++i) XY[i] = new XY_Matrix(am_sub_threads);

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

void AIDA_Processor::PROCESSING(AIDA_Decay_Event_Store* Store){
    //seperate data into xyz of AIDA
    Stream->set_DATA(Store);

    //Threading active ?
    if(USE_THREADS){

        //create threads for parallel calculations
        thread t[amount_thr];
        
        //Cluster x/y strips by time, energy and position
        for(int i = 0;i < amount_thr;++i) t[i] = threading(TX_CALC,i);
        for(int i = 0;i < amount_thr;++i) t[i].join();


        int thread_iterator = 0;

        //Cluster X and Y of each z plane to possible beta clusters
        for(int i = 0;i < amount_z_strips;++i){
            t[i] = threading(XY_CALC,thread_iterator);
            thread_iterator += 2;
        }
        for(int i = 0;i < amount_z_strips;++i) t[i].join();
    }
    //single thread mode
    else{
        //Cluster x/y strips by time, energy and position
        for(int i = 0;i < amount_z_strips*2;++i) non_threading(TX_CALC,i);
            
        //Cluster X and Y of each z plane to possible beta clusters
        for(int i = 0;i < amount_z_strips;++i) non_threading(XY_CALC,i);
    }
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

        return thread([=] {TX[thr_it]->Process(xy_b,x_tmp,T_tmp,Etmp,hits_tmp,thr_it);});
    }
    else return thread([=] {XY[(thr_it/((int) 2))]->Process(TX[thr_it],TX[thr_it + 1]);});
}

//---------------------------------------------------------------


inline void AIDA_Processor::non_threading(bool type,int iterator){
    bool xy_b = (iterator % 2 == 1);
    if(type){
        double* Etmp = Stream->get_Energy(xy_b,iterator);
        int* x_tmp = Stream->get_coordinates(xy_b,iterator);
        ULong64_t T_tmp = Stream->get_Time(xy_b,iterator);
        int hits_tmp = Stream->get_amount_of_hits(xy_b,iterator);

        TX[iterator]->Process(xy_b,x_tmp,T_tmp,Etmp,hits_tmp,iterator);
    }
    else XY[(iterator/((int) 2))]->Process(TX[iterator],TX[iterator + 1]);
}

//---------------------------------------------------------------

void AIDA_Processor::check_Thread_Use(){

    string line;
    char dummy_str[100];
    int thr_n = 0;

    cout << "\n-------------------------------------" << endl;

    ifstream thr_file("Configuration_Files/THREAD_FILE.txt");
    if(thr_file.fail()){
        if(cout_checker){
            cout << "No THREAD_FILE found!" << endl;
            cout << "Using 1 thread in total" << endl;
        }
        am_threads = 1;
        USE_THREADS = false;
        cout << "-------------------------------------\n" << endl;
        return;
    }
    else{
        while(thr_file.good()){
            getline(thr_file,line,'\n');
            if(line[0] == "#") continue;
            sscanf(line.c_str(),"%s %d",dummy_str,&thr_n);
            if(string(dummy_str) == "PRIMARY") USE_THREADS = (thr_n == 1);
            if(string(dummy_str) == "SUB") am_sub_threads = thr_n;
        }
    }
    if(USE_THREADS){
        cout << "**AIDA THREADING ENABLED**" << endl;
        cout << "Using a total number of " << amount_z_strips*2*am_sub_threads << " threads" << endl;
    }
    else{
        cout << "**AIDA THREADING DISABLED**" << endl;
        cout << "Using a total number of 1 thread" << endl;
    }
    cout << "-------------------------------------\n" << endl;
    
}

//---------------------------------------------------------------