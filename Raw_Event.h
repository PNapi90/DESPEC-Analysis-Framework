#ifndef RAW_EVENT_H
#define RAW_EVENT_H


typedef unsigned long ULong;
typedef unsigned int UInt;

class Raw_Event{

private:

	//FATIMA
	int FAT_FIRED;
	double E[36];
	double QShort[36];
	ULong TDC_timestamp[36];
	ULong QDC_t_coarse[36];
	ULong QDC_t_fine[36];

	//PLASTIC
	int iterator[4];
	ULong trigger_coarse[100];
	ULong trigger_fine[100];

	ULong coarse_T_edge_lead[4][100];
	ULong coarse_T_edge_trail[4][100];
	ULong fine_T_edge_lead[4][100];
	ULong fine_T_edge_trail[4][100];
	UInt ch_ID[4][100];
	bool fired_tamex[4];



public:
	Raw_Event();
	~Raw_Event();

	void set_DATA_FATIMA(int,double*,double*,ULong*,ULong*,ULong*);
	void set_DATA_PLASTIC(int*,ULong***,ULong***,UInt***,ULong*,ULong*);
};


#endif