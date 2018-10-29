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

    //virtual WriteData(FRS_Data*) = 0;
    //virtual WriteData(AIDA_Data*) = 0;
    //virtual WriteData(PLASTIC_Data*) = 0;
    virtual WriteData(FATIMA_Data*) = 0;
    //virtual WriteData(GALILEO_Data*) = 0;
    //virtual WriteData(FINGER_Data*) = 0;
    
};



#endif