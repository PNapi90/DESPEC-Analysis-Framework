#ifndef RAW_EVENT_H
#define RAW_EVENT_H


typedef unsigned long ULong;
typedef unsigned int UInt;

class Raw_Event{

private:

	//FATIMA
	int FAT_FIRED,TDC_FIRED;
	int Det_Nums[50];
	double E[50];
	double QShort[50];
	ULong TDC_timestamp[50];
	ULong QDC_t_coarse[50];
	ULong QDC_t_fine[50];
	bool used_for_QDC[50];

	//PLASTIC
	int iterator[4];
	ULong trigger_coarse[100];
	ULong trigger_fine[100];

	int leading_hits[4];
	int trailing_hits[4];

	ULong coarse_T_edge_lead[4][100];
	ULong coarse_T_edge_trail[4][100];
	ULong fine_T_edge_lead[4][100];
	ULong fine_T_edge_trail[4][100];
	UInt ch_ID[4][100];
	bool fired_tamex[4];
	bool ch51;


public:
	Raw_Event();
	~Raw_Event();

	void set_DATA_FATIMA(int,int,double*,double*,ULong*,ULong*,ULong*,int*,int*);
	void set_DATA_PLASTIC(int*,ULong**,ULong**,UInt**,ULong*,ULong*);


	//temporary FATIMA getters
	int get_FATIMA_am_Fired();
	double get_FATIMA_E(int);
	ULong get_FATIMA_TDC_T(int);
	ULong get_FATIMA_QDC_T_Coarse(int);
	ULong get_FATIMA_QDC_T_Fine(int);
	int get_FATIMA_det_id(int);
	int get_FATIMA_am_Fired_TDC();
	bool get_FATIMA_QDC_TDC_LINKED(int);
	bool CH_51_FIRED();

	//temporary PLASTIC getters
	int get_PLASTIC_am_Fired(int);
	double get_PLASTIC_trigger_T(int);
	int get_PLASTIC_CH_ID(int,int);
	double get_PLASTIC_lead_T(int,int);
	double get_PLASTIC_trail_T(int,int);
	int get_PLASTIC_trail_hits(int);
	int get_PLASTIC_lead_hits(int);
};


#endif