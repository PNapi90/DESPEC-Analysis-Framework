#ifndef gating_h
#define gating_h

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include "TROOT.h"
#include "Riostream.h"


class gating
{

	public : 

			gating(std::string filename);
		
			std::vector < std::vector <Double_t> > get_gates();
			
			int get_num_gates();
						
				
	private : 
	  
			Double_t low, high;							 	
			Double_t low_coinc, high_coinc;							 	
			int i;									 	

			std::vector < std::vector <Double_t> > gates;
			std::vector <Double_t> dummy;

};

#endif

