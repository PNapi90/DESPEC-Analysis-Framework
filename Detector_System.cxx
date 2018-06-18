
#ifndef DETECTOR_SYSTEM_H
#define DETECTOR_SYSTEM_H 

class Detector_System{

public:

    virtual void Process_MBS(int*) = 0;
    
    virtual double** get_Event_data() = 0;

};

#endif
