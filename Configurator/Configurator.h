#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

class Configurator{

private:

	bool configuration_called;

	std::string det_names[5];

	int White_Rabbit_value;
	int* det_enabled;

	void format_error(int);
	void write_config_file();

public:
	Configurator();
	~Configurator();

	void configurate();
	void load_configuration();



};



#endif