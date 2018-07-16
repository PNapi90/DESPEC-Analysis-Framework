#ifndef RAW_EVENT_H
#define RAW_EVENT_H


typedef unsigned long ULong;
typedef unsigned int UInt;
typedef unsigned long long ULong64_t;

class Raw_Event{

private:


	/* ##########################################################

	//FRS
	// MUSIC PARAMETERS //
	  
	Float_t dE[3];      // set_DATA_MUSIC
	Float_t dE_cor[3];  // set_DATA_MUSIC
	
	// SCINTILLATOR PARAMETERS //
	  
	Float_t sci_l[12];  // set_DATA_SCI
	Float_t sci_r[12];  // set_DATA_SCI
	Float_t sci_e[12];  // set_DATA_SCI
	Float_t sci_tx[12]; // set_DATA_SCI
	Float_t sci_x[12];  // set_DATA_SCI
	  
	  
	Float_t sci_tofll2; // set_DATA_SCI_ToF
	Float_t sci_tofll3; // set_DATA_SCI_ToF
	Float_t sci_tof2;   // set_DATA_SCI_ToF
	Float_t sci_tofrr2; // set_DATA_SCI_ToF
	Float_t sci_tofrr3; // set_DATA_SCI_ToF
	Float_t sci_tof3;   // set_DATA_SCI_ToF
	  
	// ID PARAMETERS //
	  
	Float_t ID_x2;      // set_DATA_ID_2_4
	Float_t ID_y2;      // set_DATA_ID_2_4
	Float_t ID_a2;      // set_DATA_ID_2_4
	Float_t ID_b2;      // set_DATA_ID_2_4
	  
	Float_t ID_x4;      // set_DATA_ID_2_4
	Float_t ID_y4;      // set_DATA_ID_2_4
	Float_t ID_a4;      // set_DATA_ID_2_4
	Float_t ID_b4;      // set_DATA_ID_2_4
	  
	Float_t ID_brho[2]; // set_DATA_ID_Beta_Rho
	Float_t ID_rho[2];  // set_DATA_ID_Beta_Rho
	  
	Float_t beta;       // set_DATA_ID_Beta_Rho
	Float_t beta3;      // set_DATA_ID_Beta_Rho
	Float_t gamma;      // set_DATA_ID_Beta_Rho

	Float_t AoQ;        // set_DATA_ID_Z_AoQ
	Float_t AoQ_corr;   // set_DATA_ID_Z_AoQ
	  
	Float_t z;          // set_DATA_ID_Z_AoQ
	Float_t z2;         // set_DATA_ID_Z_AoQ
	Float_t z3;         // set_DATA_ID_Z_AoQ
	  
	Float_t timestamp;  // set_DATA_ID_Timestamp
	Float_t ts;         // set_DATA_ID_Timestamp
	Float_t ts2;        // set_DATA_ID_Timestamp

	
	########################################################## */

	//FATIMA
	int FAT_FIRED,TDC_FIRED;
	int Det_Nums[100];
	double E[100];
	double QShort[100];
	double E_Raw[100];
	double QShort_Raw[100];
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


	/* ##########################################################

	// FRS STUFF //
	void set_DATA_MUSIC(Float_t*,Float_t*);
	void set_DATA_SCI(Float_t*,Float_t*,Float_t*,Float_t*,Float_t*);
	void set_DATA_SCI_ToF(Float_t,Float_t,Float_t,Float_t,Float_t,Float_t);
	void set_DATA_ID_2_4(Float_t,Float_t,Float_t,Float_t,Float_t,Float_t,Float_t,Float_t);
	void set_DATA_ID_Beta_Rho(Float_t*,Float_t*,Float_t,Float_t,Float_t);
	void set_DATA_ID_Z_AoQ(Float_t,Float_t,Float_t,Float_t,Float_t);
	void set_DATA_ID_Timestamp(Float_t,Float_t,Float_t);
	// FRS STUFF //

	########################################################## */

	void set_DATA_FATIMA(int,int,double*,double*,double*,double*,ULong64_t*,ULong64_t*,ULong64_t*,int*,int*);
	void set_DATA_PLASTIC(int*,double**,double**,UInt**,double*,double*);
	void set_DATA_GALILEO(int,ULong64_t*,int*,int*,ULong64_t*,double*,int*);


/* ####################################################

	//temporary FRS getters
	Float_t get_FRS_dE(int);
	Float_t get_FRS_dE_corr(int);
	
	Float_t get_FRS_sci_l(int);
	Float_t get_FRS_sci_r(int);
	Float_t get_FRS_sci_e(int);
	Float_t get_FRS_sci_tx(int);
	Float_t get_FRS_sci_x(int);
	
	Float_t get_FRS_tofll2();
	Float_t get_FRS_tofll3();
	Float_t get_FRS_tof2();
	Float_t get_FRS_tofrr2();
	Float_t get_FRS_tofrr3();
	Float_t get_FRS_tof3();

	Float_t get_FRS_x2();
	Float_t get_FRS_y2();
	Float_t get_FRS_a2();
	Float_t get_FRS_b2();
	
	Float_t get_FRS_x4();
	Float_t get_FRS_y4();
	Float_t get_FRS_a4();
	Float_t get_FRS_b4();

	Float_t get_FRS_brho(int);
	Float_t get_FRS_rho(int);
	
	Float_t get_FRS_beta();
	Float_t get_FRS_beta3();
	Float_t get_FRS_gamma();
	
	Float_t get_AoQ();
	Float_t get_AoQ_corr();
	Float_t get_z();
	Float_t get_z2();
	Float_t get_z3();
	
	Float_t get_timestamp();	
	Float_t get_ts();	
	Float_t get_ts2();	

 #################################################### */


	//temporary FATIMA getters
	int get_FATIMA_am_Fired();
	double get_FATIMA_E(int);
	double get_FATIMA_QShort(int);
	double get_FATIMA_E_Raw(int);
	double get_FATIMA_QShort_Raw(int);
	ULong64_t get_FATIMA_TDC_T(int);
	ULong64_t get_FATIMA_QDC_T_Coarse(int);
	ULong64_t get_FATIMA_QDC_T_Fine(int);
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
	double get_PLASTIC_TOT(int,int);

	//temporary GALILEO getters
	int get_GALILEO_am_Fired();
	ULong64_t get_GALILEO_Sum_T(int);
	int get_GALILEO_Pileup(int);
	int get_GALILEO_Hit_Pattern(int);
	ULong64_t get_GALILEO_Chan_T(int);
	double get_GALILEO_Chan_E(int);
	
	
	
};


#endif
