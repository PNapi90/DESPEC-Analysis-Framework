#ifndef DATA_STREAM_H
#define DATA_STREAM_H

#include <iostream>
#include <cstdlib>



typedef unsigned long ULong;
typedef unsigned int UInt;


class Data_Stream{

private:

	std::string Known_ID[5];

	int FATIMA_Det_ID[100];
	double FATIMA_E[100];
	ULong FATIMA_T[100];
	ULong FATIMA_QDC_T[100];
	int FATIMA_hits_det[100];

	ULong PLASTIC_Coarse_T[100];
	ULong PLASTIC_Fine_T[100];
	ULong PLASTIC_ch_ID[100];

	ULong PLASTIC_Coarse_T_Trigger;
	ULong PLASTIC_Fine_T_Trigger;

	int fat_iter;
	int FATIMA_Event_Length;
	int ID_int;
	int amount_ID[100];
	

public:
	Data_Stream();
	~Data_Stream();
	
	void set_ID(std::string);
	void set_amount_of_Events(int);
	void set_event_data(double,ULong,ULong,int,int,int);
    void set_event_data(ULong*,ULong*,UInt*,ULong,ULong);

};


#endif