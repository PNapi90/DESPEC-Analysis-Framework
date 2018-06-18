#include "FATIMA_gating.h"


#include <iostream> 
#include <fstream>
#include <math.h>
#include <vector>
#include "TROOT.h"
#include "Riostream.h"


gating::gating(std::string filename)
{
	gates.clear();

	std::ifstream infile(filename.c_str());
	
	i = 0;
	    
	while (infile >> low >> high >> low_coinc >> high_coinc)
	{
		
		dummy.push_back(low);
		dummy.push_back(high);
		dummy.push_back(low_coinc);
		dummy.push_back(high_coinc);
		
		gates.push_back(dummy);
		
		dummy.clear();
		
		i++;

	}
	
}
	
	
std::vector < std::vector <Double_t> > gating::get_gates() 
{
		
	return gates;
		
}

int gating::get_num_gates() 
{
		
	return i;
		
}

