#include "Event_Storage.h"

using namespace std;


//---------------------------------------------------------------

Event_Storage::Event_Storage(){

    //number of stripes of AIDA in x and y
    N_X = 1024;
    N_Y = 1024;

    //create raw Event array and iterator array
    Events = new Events****[3];
    AIDA_iterator = new int**[3];
    for(int i = 0;i < 3;++i){
        Events[i] = new Events***[N_X];
        AIDA_iterator[i] = new int*[N_X];
        for(int j = 0;j < N_X;++j){
            Events[i][j] = new Events**[N_Y];
            AIDA_iterator[i][j] = new int[N_Y];
            for(int k = 0;k < N_Y;++k){
                Events[i][j][k] = new Events*[200];
                AIDA_iterator[i][j][k] = 0;
                for(int o = 0;o < 200;++o) Events[i][j][k][o] = NULL;
            }
        }
    }
}

//---------------------------------------------------------------


Event_Storage::~Event_Storage(){
    for(int i = 0;i < 3;++i){
        for(int j = 0;j < N_X;++j){
            for(int k = 0;k < N_Y;++k){
                for(int o = 0;o < 200;++o) if(Events[i][j][k][o]) delete Events[i][j][k][o];
                delete[] Events[i][j][k];
            }
            delete[] AIDA_iterator[i][j];
            delete[] Events[i][j];
        }
        delete[] AIDA_iterator[i];
        delete[] Events[i];
    }
    delete[] AIDA_iterator;
    delete[] Events;
}

//---------------------------------------------------------------

void Event_Storage::save_Raw_Event(Raw_Event* RAW,int type,ULong64_t Time){
    //set type of Event(0 = FRS,1 = AIDA,2 = PLASTIC,3 = FATIMA,4 = GALILEO)
    this->type = type;

    if(type > 4){
        cerr << "In Event_Storage::save_Raw_Event  ";
        cerr << "-> Detector system type " << type << " not known" << endl;
        exit(0);
    }
    else if(type == 1){
        cerr << "AIDA data in ";
        cerr << "wrong Event_Storage::save_Raw_Event function! Overload error!" << endl;
        exit(0);
    }

    
    
    Time_Array[type][type_iter[type]] = Time;


}

//---------------------------------------------------------------

void Event_Storage::save_Raw_Event(Raw_Event* RAW,int type){
    //set type of Event(0 = FRS,1 = AIDA,2 = PLASTIC,3 = FATIMA,4 = GALILEO)
    this->type = type;
    if(type > 4){
        cerr << "In Event_Storage::save_Raw_Event  ";
        cerr << "-> Detector system type " << type << " not known" << endl;
        exit(0);
    }
    else if(type != 1){
        cerr << "Non AIDA data in ";
        cerr << "wrong Event_Storage::save_Raw_Event function! Overload error!" << endl;
        exit(0);
    }

    //loop over all AIDA events
    int AIDA_Event_Num = RAW->get_AIDA_amount_of_events();
    
    ULong64_t time_tmp = 0;
    int iter_tmp = 0;

    int* AIDA_XYZ = new int[3];
    for(int i = 0;i < 3;++i) AIDA_XYZ[i] = -1;

    for(int i = 0;i < AIDA_Event_Num;++i){
        //get X,Y and Z coordinates, time and type of AIDA event
        RAW->get_AIDA_XYZ(AIDA_XYZ,i);
        time_tmp = RAW->get_AIDA_time(i);
        type = RAW->get_AIDA_type(i);

        //iterator of hit AIDA grid (if multiple hits on same XYZ coordinates)
        iter_tmp = AIDA_iterator[AIDA_XYZ[2]][AIDA_XYZ[0]][AIDA_XYZ[1]];
        
        //if grid point not hit -> create new hit
        if(!Events[AIDA_XYZ[2]][AIDA_XYZ[0]][AIDA_XYZ[1]][iter_tmp]) create_Event(iter_tmp,AIDA_XYZ);
        
        //save data of AIDA event in Event object
        Events[AIDA_XYZ[2]][AIDA_XYZ[0]][AIDA_XYZ[1]][iter_tmp]->save_DATA();

        //increase iterator
        AIDA_iterator[AIDA_XYZ[2]][AIDA_XYZ[0]][AIDA_XYZ[1]]++;
    }
    delete[] AIDA_XYZ;
    AIDA_XYZ = NULL;
}

//---------------------------------------------------------------

void Event_Storage::create_Event(int iter,int* AIDA_XYZ){
    Events[AIDA_XYZ[2]][AIDA_XYZ[0]][AIDA_XYZ[1]][iter_tmp] = new Event(AIDA_XYZ);
}

//---------------------------------------------------------------