#ifndef PROCESSDEF_H
#define PROCESSDEF_H

#include <vector>

struct ProcessDef{
    
    ProcessDef(int _ID,int _Length, std::vector<int> _types) 
			  : ID(_ID) , Length(_Length) , types(_types){}
    
    int ID;
    int Length;
    std::vector<int> types;

};


#endif
