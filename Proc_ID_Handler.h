#ifndef PROC_ID_HANDLER_H
#define PROC_ID_HANDLER_H

#include <fstream>
#include <iostream>
#include <string>

class Proc_ID_Handler{

private:

    const char* format = "%d %d %d %d %d";
    int Proc_ID_Arr[5];

    void load_Proc_ID_File();

public:
    Proc_ID_Handler();
    ~Proc_ID_Handler();

    int get_Detector_type(int);
    
};




#endif