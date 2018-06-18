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