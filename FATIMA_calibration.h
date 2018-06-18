#ifndef calibration_h
#define calibration_h

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include "TROOT.h"
#include "Riostream.h"


class energy_calibration
{

	public : 

			energy_calibration(std::string filename);
		
			std::vector < std::vector <Double_t> > get_E_calib();
			
			Double_t do_E_calib(Double_t x, int channel);
			
				
	private : 
	  
			Double_t a, b, c, d, e;							 	
			int i, E_ch;									 	

			Double_t E_calibrated;

			std::vector < std::vector <Double_t> > E_calib;
			std::vector <Double_t> dummy;

};

class time_calibration
{

	public : 

			time_calibration(std::string filename);
		
			std::vector < std::vector <Double_t> > get_T_calib();
			
			Double_t do_T_calib(Double_t x, int channel);
			
				
	private : 
	  
			Double_t shift;							 	
			int i, T_ch;									 	

			Double_t T_calibrated;

			std::vector < std::vector <Double_t> > T_calib;
			std::vector <Double_t> dummy_2;

};

#endif
