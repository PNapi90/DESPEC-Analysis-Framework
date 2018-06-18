#ifndef WHITE_RABBIT_H
#define WHITE_RABBIT_H

#include <fstream>
#include <string>
#include <iostream>

#include "WR_Structure.h"


//define ULong64_t (used e.g. in Root or Go4)
typedef unsigned long long ULong64_t;

class White_Rabbit{

private:

    const char* format = "%d %d %d %d %d";

    int DETECTORS[5];

    int detector_id;
    ULong64_t WR_Time;

    void load_config_file();
    int get_Detector_type();

    void set_triggered_detector(int);
    void process_White_Rabbit(int*);



public:
    White_Rabbit();
    ~White_Rabbit();

    ULong64_t get_White_Rabbit(int*);
    int get_Detector_id();

    
};


#endif