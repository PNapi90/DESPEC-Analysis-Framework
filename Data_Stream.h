#ifndef DATA_STREAM_H
#define DATA_STREAM_H

#include <iostream>

typedef unsigned long ULong;
typedef unsigned int UInt;


class Data_Stream{

private:

	std::string Known_ID[5];



public:
	Data_Stream();
	~Data_Stream();
	
	void set_ID(std::string);
	void set_amount_of_Events(int);
	void set_event_data(double**);
    void set_event_data(ULong*,ULong*,UInt*,ULong,ULong);
}


};


#endif