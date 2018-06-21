#ifndef WR_HEADER_H
#define WR_HEADER_H

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

// ############################################## //
// ######## White Rabbit Data Structure ######### //
// ############################################## //

struct WR_Header
{  

    unsigned int check_wr   : 32;

};

struct WR_Data
{  
    
    unsigned int timestamp  : 16;
    unsigned int nothing    : 16;

};

// ############################################## //

#endif
