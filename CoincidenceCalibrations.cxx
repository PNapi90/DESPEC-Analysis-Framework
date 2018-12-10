#include "CoincidenceCalibrations.h"

//---------------------------------------------------------------

CoincidenceCalibrations::CoincidenceCalibrations(){
	LoadCalibrationTypes();
}

//---------------------------------------------------------------

CoincidenceCalibrations::~CoincidenceCalibrations(){
	
	
}

//---------------------------------------------------------------

void CoincidenceCalibrations::LoadCalibrationTypes(){

	std::string filename = "Configuration_Files/CoincidenceCalibrations.txt";
	std::ifstream CoincFile(filename);
	
	if(CoincFile.fail()){
		std::cerr << "Could not find " << filename << std::endl;
		exit(1);
	}

}

//---------------------------------------------------------------
