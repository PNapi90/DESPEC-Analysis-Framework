#ifndef RAW_EVENT_H
#define RAW_EVENT_H


typedef unsigned long ULong;
typedef unsigned int UInt;

class Raw_Event{

private:

	//FATIMA
	int FAT_FIRED,TDC_FIRED;
	int Det_Nums[100];
	double E[100];
	double QShort[100];
	ULong TDC_timestamp[100];
	ULong QDC_t_coarse[100];
	ULong QDC_t_fine[100];
	bool used_for_QDC[100];
	double time_difference;

	//PLASTIC
	int iterator[4];
	double trigger_coarse[100];
	double trigger_fine[100];

	int leading_hits[4];
	int trailing_hits[4];
	int phys_channel[4][17];
	int leading_hits_ch[4][17];
	int trailing_hits_ch[4][17];
	

	double coarse_T_edge_lead[4][100];
	double coarse_T_edge_trail[4][100];
	double fine_T_edge_lead[4][100];
	double fine_T_edge_trail[4][100];
	UInt ch_ID[4][100];
	bool fired_tamex[4];

	
	//GALILEO
	int GAL_FIRED;
	int GALILEO_Det_Nums[36];
	ULong GALILEO_sum_time[36];
	int GALILEO_pileup[36];
	int GALILEO_hit_pattern[36];
	ULong GALILEO_chan_time[36];
	double GALILEO_chan_energy[36];


	bool ch51;



public:
	Raw_Event();
	~Raw_Event();




	void set_DATA_GALILEO(int,ULong*,int*,int*,ULong*,double*,int*);

	void set_DATA_FATIMA(int,int,double*,double*,ULong*,ULong*,ULong*,int*,int*);
	void set_DATA_PLASTIC(int*,double**,double**,UInt**,double*,double*);


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
	double get_FATIMA_Time_Diff();

	//temporary PLASTIC getters
	int get_PLASTIC_am_Fired(int);
	double get_PLASTIC_trigger_T(int);
	int get_PLASTIC_CH_ID(int,int);
	double get_PLASTIC_lead_T(int,int);
	double get_PLASTIC_trail_T(int,int);

	int get_PLASTIC_trail_hits(int);
	int get_PLASTIC_lead_hits(int);
	int get_PLASTIC_physical_channel(int,int);
	int get_PLASTIC_physical_lead_hits(int,int);
	int get_PLASTIC_physical_trail_hits(int,int);
	double get_PLASTIC_coarse_lead(int,int);
	
	//temporary GALILEO getters
	int get_GALILEO_am_Fired();
	ULong get_GALILEO_Sum_T(int);
	int get_GALILEO_Pileup(int);
	int get_GALILEO_Hit_Pattern(int);
	ULong get_GALILEO_Chan_T(int);
	double get_GALILEO_Chan_E(int);
};


#endif
