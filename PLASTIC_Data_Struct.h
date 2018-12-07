#ifndef PLASTIC_DATA_STRUCT
#define PLASTIC_DATA_STRUCT

#include <vector>

struct PLASTIC_Data{
	
	int TamexHits = 0;
	
    std::vector<std::vector<double> > LeadingT(10,std::vector<double>(20,0));
    std::vector<std::vector<double> > TrailingT(10,std::vector<double>(20,0));
    std::vector<std::vector<double> > TOT(10,std::vector<double>(20,0));
    std::vector<std::vector<int> > PhysicalChannels(10,std::vector<int>(20,0));
    std::vector<int> HitsPerTamex(10,0);
};

#endif
