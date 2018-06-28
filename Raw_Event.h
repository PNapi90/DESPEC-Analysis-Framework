#ifndef RAW_EVENT_H
#define RAW_EVENT_H


typedef unsigned long ULong;
typedef unsigned int UInt;

class Raw_Event{

private:

	//FATIMA
	int FAT_FIRED;
	int Det_Nums[36];
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

	void set_DATA_FATIMA(int,double*,ULong*,ULong*,ULong*,int*);
	void set_DATA_PLASTIC(int*,ULong***,ULong***,UInt***,ULong*,ULong*);


	//temporary FATIMA getters
	int get_FATIMA_am_Fired();
	double get_FATIMA_E(int);
	ULong get_FATIMA_TDC_T(int);
	ULong get_FATIMA_QDC_T_Coarse(int);
	ULong get_FATIMA_QDC_T_Fine(int);
	int get_FATIMA_det_id(int);

	//temporary PLASTIC getters
	int get_PLASTIC_am_Fired(int);
	double get_PLASTIC_trigger_T(int);
	int get_PLASTIC_CH_ID(int,int);
	double get_PLASTIC_lead_T(int,int);
	double get_PLASTIC_trail_T(int,int);
};


#endif