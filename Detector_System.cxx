//#include "Detector_System.h"

using namespace std;

#ifndef DETECTOR_SYSTEM_H
#define DETECTOR_SYSTEM_H 

class Detector_System{

private:

public:
    //Detector_System();

    //virtual ~Detector_System();
    
    virtual double** get_Event_data() = 0;

};

#endif


//---------------------------------------------------------------

//Detector_System::Detector_System(){}

//---------------------------------------------------------------
