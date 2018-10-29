#ifndef BRANCH_CREATOR_H
#define BRANCH_CREATOR_H

//#include "FRS_Data_Struct.h"
//#include "AIDA_Data_Struct.h"
//#include "PLASTIC_Data_Struct.h"
#include "FATIMA_Data_Struct.h"
//#include "GALILEO_Data_Struct.h"
//#include "FINGER_Data_Struct.h"

#include <TTree.h>

class Branch_Creator{

    
public:
    
    virtual ~Branch_Creator(){};

    //virtual void WriteData(FRS_Data*) = 0;
    //virtual void WriteData(AIDA_Data*) = 0;
    //virtual void WriteData(PLASTIC_Data*) = 0;
    virtual void WriteData(FATIMA_Data*) = 0;
    //virtual void WriteData(GALILEO_Data*) = 0;
    //virtual void WriteData(FINGER_Data*) = 0;
    
};



#endif
