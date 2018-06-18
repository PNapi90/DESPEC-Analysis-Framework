#ifndef DETECTOR_SYSTEM_H
#define DETECTOR_SYSTEM_H 

class Detector_System{

private:

public:
    Detector_System();

    virtual ~Detector_System();
    
    virtual void get_Event_data() = 0;

};

#endif