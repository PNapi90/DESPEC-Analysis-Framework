#include "FATIMA_calibration.h"


#include <iostream> 
#include <fstream>
#include <math.h>
#include <vector>
#include "TROOT.h"
#include "Riostream.h"


energy_calibration::energy_calibration(std::string filename)
{
	E_calib.clear();

	std::ifstream infile(filename.c_str());
	    
	while (infile >> E_ch >> a >> b >> c >> d >> e)
	{
		
		dummy.push_back(E_ch);
		dummy.push_back(a);
		dummy.push_back(b);
		dummy.push_back(c);	
		dummy.push_back(d);	
		dummy.push_back(e);	
		
		E_calib.push_back(dummy);
		
		dummy.clear();

	}
	
}
	
	
std::vector < std::vector <Double_t> > energy_calibration::get_E_calib() // Gets array of calibration factors //
{
		
	return E_calib;
		
}

Double_t energy_calibration::do_E_calib(Double_t x, int channel) // Carries out calibration //
{
	
	
	E_calibrated = E_calib[channel][1]*x*x*x*x + E_calib[channel][2]*x*x*x + E_calib[channel][3]*x*x + E_calib[channel][4]*x + E_calib[channel][5];


	return E_calibrated;	

}

// ################################################### //
// ################################################### //
// ################################################### //
// ################################################### //


time_calibration::time_calibration(std::string filename)
{
	T_calib.clear();

	std::ifstream infile(filename.c_str());
	    
	while (infile >> T_ch >> shift)
	{
		
		dummy_2.push_back(T_ch);
		dummy_2.push_back(shift);
		
		T_calib.push_back(dummy_2);
		
		dummy_2.clear();

	}
	
}
	
	
std::vector < std::vector <Double_t> > time_calibration::get_T_calib() // Gets array of calibration factors //
{
		
	return T_calib;
		
}

Double_t time_calibration::do_T_calib(Double_t x, int channel) // Carries out calibration //
{
	
	
	T_calibrated = x + T_calib[channel][1];


	return T_calibrated;

}

