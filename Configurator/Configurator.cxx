#include "Configurator.h"

using namespace std;

//---------------------------------------------------------------


Configurator::Configurator(){
	configuration_called = false;
	det_enabled = new int[5];

	det_names[0] = "FRS";
	det_names[1] = "AIDA";
	det_names[2] = "PLASTIC";
	det_names[3] = "FATIMA";
	det_names[4] = "GALILEO";

}

//---------------------------------------------------------------

Configurator::~Configurator(){
	delete[] det_enabled;
}

//---------------------------------------------------------------

void Configurator::configurate(){

	int input;

	cout << "=================================" << endl;
	cout << "***Configurator Called***" << endl;
	cout << endl;
	cout << "Enable White Rabbit? (0 -> no, 1 -> yes)\t";
	cin >> input;

	if(input != 1 && input != 0) format_error(input);
	
	White_Rabbit_value = input;
	
	cout << endl;
	cout << "Which of the following Detector Systems would you like to enable? (0 -> no, 1 -> yes)" << endl;
	cout << endl;
	for(int i = 0;i < 5;++i){
		cout << det_names[i] << "\t\t\t";
		cin >> input;
		if(input != 1 && input != 0) format_error(input);
		det_enabled[i] = input;
		cout << endl;
	}
	cout << "\n=================================" << endl;

	write_config_file();
	configuration_called = true;
}


//---------------------------------------------------------------


void Configurator::load_configuration(){
	if(!configuration_called){
		ifstream data("Configuration_Files/System_Arrangement.txt");
		
		const char* format = "%s %d";
		char name[100];
		int enabled;
		string line;

		if(data.fail()){
			cerr << "Could not find System_Arrangement.txt file!" << endl;
			exit(0);
		}

		int counter = 0;

		while(data.good()){
			getline(data,line,'\n');
			if(line[0] == '#') continue;
			sscanf(line.c_str(),format,name,enabled);
	
			if(counter == 0) White_Rabbit_value = enabled;
			else det_enabled[counter-1] = enabled;
			counter++;
		}
	}

	cout << "=========================================" << endl;
	cout << "USED SYSTEMS" << endl;
	cout << "-----------------------------------------" << endl;
	cout << "White Rabbit\t\t" << White_Rabbit_value << endl;
	for(int i = 0;i < 5;++i) cout << det_names[i] << "\t\t" << det_enabled[i] << endl;
	cout << "-----------------------------------------" << endl; 
	cout << "=========================================" << endl;

}

//---------------------------------------------------------------

void Configurator::format_error(){
	cout << endl <<"Input format " << input << " not known!" << endl;
	exit(0);
}

//---------------------------------------------------------------

void Configurator::write_config_file(){
	
	ofstream data("Configuration_Files/System_Arrangement.txt");

	data << "#Configuration File for used Detector Systems and White Rabbit" << endl;
	data << "#This file is generated automatically by the Configurator class" << endl;
	data << "#System name\t\t used (0 -> no, 1 -> yes)" << endl;

	data << "White Rabbit\t\t" << White_Rabbit_value << endl;

	for(int i = 0;i < 5;++i) data << det_names[i] << "\t\t" << det_enabled[i] << endl;

	data.close();
}

//---------------------------------------------------------------