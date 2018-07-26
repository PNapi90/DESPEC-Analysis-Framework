#include "First_Test.h"  // Defines Histograms //
#include "Riostream.h"

// Root Includes //
#include "TROOT.h"
#include "TH1.h"
#include "TF1.h"
#include "TH2.h"
#include "TCutG.h"
#include "TArc.h"
#include "TTree.h"

#include <time.h>
#include <math.h>
#include <iomanip>

// Go4 Includes //
#include "TGo4UserException.h"
#include "TGo4Picture.h"
#include "TGo4MbsEvent.h"

// General Includes //
#include <fstream>
#include <vector>


#include "TSCNUnpackEvent.h"

#include "Detector_System.cxx"
#include "AIDA_Detector_System.h"
#include "FATIMA_Detector_System.h"
#include "PLASTIC_Detector_System.h"
#include "GALILEO_Detector_System_TEST.h"
#include "BS_Detector_System.h"

#include "Data_Stream.cxx"
#include "White_Rabbit.h"

#include <string>

#define FAT_MAX_DET 60
#define FAT_REF_DET 0

using namespace std;


//----------------------------------------------------------
TSCNUnpackProc::TSCNUnpackProc() :
TGo4EventProcessor()
{
	ffill = kTRUE;
	fshift = 6;
}
//----------------------------------------------------------

		    int not_same = 0;
		    int equal_2 = 0;

TSCNUnpackProc::TSCNUnpackProc(const char* name) :
TGo4EventProcessor(name) // Histograms defined here //
{
	FAT_make_raw_histograms = true;
	
	cout << "**** TSCNUnpackProc: Create" << endl;
	
	
	// ######################################################### //
	
	hsci_tofll2 = MakeTH1('D',"hsci_tofll2","hsci_tofll2",1500,0.,62000.);
	hsci_tofll3 = MakeTH1('D',"hsci_tofll3","hsci_tofll3",1500,0.,62000.);
	hsci_tof2 = MakeTH1('D',"hsci_tof2","hsci_tof2",1000,0.,62000.);
	hsci_tofrr2 = MakeTH1('D',"hsci_tofrr2","hsci_tofrr2",1500,0.,62000.);
	hsci_tofrr3 = MakeTH1('D',"hsci_tofrr3","hsci_tofrr3",1500,0.,62000.);
	hsci_tof3 = MakeTH1('D',"hsci_tof3","hsci_tof3",1000,0.,62000.);
	
	/*hID_x2 = MakeTH1('D',"ID_x2","ID_x2",3000,0,300);
	hID_y2 = MakeTH1('D',"ID_y2","ID_y2",3000,0,300);
	hID_a2 = MakeTH1('D',"ID_a2","ID_a2",3000,0,300);
	hID_b2 = MakeTH1('D',"ID_b2","ID_b2",3000,0,300);
	
	hID_x4 = MakeTH1('D',"ID_x4","ID_x4",3000,0,300);
	hID_y4 = MakeTH1('D',"ID_y4","ID_y4",3000,0,300);
	hID_a4 = MakeTH1('D',"ID_a4","ID_a4",3000,0,300);
	hID_b4 = MakeTH1('D',"ID_b4","ID_b4",3000,0,300);*/
	
	hbeta = MakeTH1('D',"beta","beta",100,0.,1.);
	hbeta3 = MakeTH1('D',"beta3","beta3",100,0.,1.);
	hgamma = MakeTH1('D',"gamma","gamma",100,0.,1.);
	
	hAoQ = MakeTH1('D',"AoQ","AoQ",200,1.4,3.0); // 200,1.4,3.0
	hAoQ_corr = MakeTH1('D',"AoQ_corr","AoQ_corr",200,1.4,3.0); // 200,1.4,3.0
	
	hz = MakeTH1('D',"z","z",100,0.,93.);
	hz2 = MakeTH1('D',"z2","z2",100,0.,93.);
	hz3 = MakeTH1('D',"z3","z3",100,10.,93.);
	
	htimestamp = MakeTH1('D',"timestamp","timestamp",30,0.,300.);
	hts = MakeTH1('D',"ts","ts",30,0.,300.);
	hts2 = MakeTH1('D',"ts2","ts2",30,0.,300.);
	
	
	
	cout<<"Helleurrr"<<endl;
	
	// ######################################################### //

	//*****************
	//Fatima histograms
	FATgate1_low  = 1172.;
	FATgate1_high = 1182.;
	FATgate2_low  = 1328.;
	FATgate2_high = 1338.;
	float E_gate1 = FATgate1_low + (FATgate1_high - FATgate1_low)/2;
	float E_gate2 = FATgate2_low + (FATgate2_high - FATgate2_low)/2;
	
	cout<<"Helleurrr"<<endl;

		
	FAT_Esum  = MakeTH1('D', "FATIMA/ESum", "LaBr Energy (all detectors)",4001,0,4000);
	FAT_gg    = MakeTH2('D', "FATIMA/gg", "FATIMA Gamma-Gamma (all detectors)",2001,0,2000, 2001,0,2000);
	FAT_TDCdtsum = MakeTH1('D', "FATIMA/TDCdtSum", "TDC dt (all detectors)", 3201,-40,40);
	FAT_QDCdtsum = MakeTH1('D', "FATIMA/QDCdtSum", "QDC dt (all detectors)", 3201,-40,40);
	FAT_TDCdtsum_ref_gated = MakeTH1('D', "FATIMA/TDCdt_ref_gated",
							Form("TDC dt gates on %5.2f keV and %5.2f keV (all detectors)", E_gate1, E_gate2), 3201,-40,40);
	FAT_QDCdtsum_ref_gated = MakeTH1('D', "FATIMA/QDCdt_ref_gated",
							Form("QDC dt gates on %5.2f keV and %5.2f keV (all detectors)", E_gate1, E_gate2), 3201,-40,40);				
	
	cout<<"Helleurrr"<<endl;

	
	//statistics
	FAT_hits = MakeTH1('D', "FATIMA/Stat/det_hits", "FATIMA detector statistics",40,0,40);
	FAT_hits_QDC = MakeTH1('D', "FATIMA/Stat/QDC_hits", "FATIMA QDC statistics",40,0,40);
	FAT_hits_TDC = MakeTH1('D', "FATIMA/Stat/TDC_hits", "FATIMA TDC statistics",40,0,40);
	FAT_QDC_TDC_hitmap = MakeTH2('D', "FATIMA/Stat/QDC_TDC_hitmap", "FATIMA QDC-TDC hit map",40,0,40, 40,0,40);
	FAT_correlations = MakeTH2('D', "FATIMA/Stat/det_det_correlations", "FATIMA det-det correlations",40,0,40, 40,0,40);
	
	cout<<"Helleurrr"<<endl;
	
	//energy
	FAT_E = new TH1*[FAT_MAX_DET];
	FAT_Eraw = new TH1*[FAT_MAX_DET];
	FAT_E_ratio = new TH2*[FAT_MAX_DET];
	FAT_gg_ref = new TH2*[FAT_MAX_DET];
	
	cout<<"Helleurrr"<<endl;
	
	//timing
	FAT_TDCdt_ref = new TH1*[FAT_MAX_DET];
	FAT_QDCdt_ref = new TH1*[FAT_MAX_DET];
	FAT_TDC_QDC_dt = new TH2*[FAT_MAX_DET];
	FAT_TDCdt_ref_gated = new TH1*[FAT_MAX_DET];
	FAT_E_TDCdt_ref_gated = new TH2*[FAT_MAX_DET];
	
	cout<<"Helleurrr"<<endl;
	
	for (int det = 0;  det< FAT_MAX_DET; det++) {
		FAT_E[det] = MakeTH1('D', Form("FATIMA/Energy/E_LaBr%02d", det),
		                          Form("LaBr%02d energy", det),4001,0,4000);
		FAT_Eraw[det] = MakeTH1('D', Form("FATIMA/Energy/E_Raw_LaBr%02d", det),
		                             Form("LaBr%02d energy (raw)", det),2000,0,40000);
		FAT_E_ratio[det] = MakeTH2('D', Form("FATIMA/Energy/EvsRatio_LaBr%02d", det),
		                           Form("LaBr%02d energy vs QShort/QLong", det),4001,0,4000, 200,0,1);
		FAT_gg_ref[det] = MakeTH2('D', Form("FATIMA/Energy/gg_LaBr%02d_LaBr%02d", FAT_REF_DET, det),
		                          Form("Gamma-Gamma coincidences LaBr%02d-LaBr%02d", FAT_REF_DET, det),4001,0,4000, 200,0,1);
		FAT_TDCdt_ref[det] = MakeTH1('D', Form("FATIMA/Timing/TDCdt_LaBr%02d_LaBr%02d", FAT_REF_DET, det),
		                             Form("TDC dt LaBr%02d LaBr%02d", FAT_REF_DET, det),3201,-40,40);		                             
		FAT_QDCdt_ref[det] = MakeTH1('D', Form("FATIMA/Timing/QDCdt_LaBr%02d_LaBr%02d", FAT_REF_DET, det),
		                             Form("TDC dt LaBr%02d LaBr%02d", FAT_REF_DET, det),3201,-40,40);
		FAT_TDC_QDC_dt[det] = MakeTH2('D', Form("FATIMA/Timing/TDCdt_QDCdt_LaBr%02d", det),
		                              Form("TDCdt vs QDCdt LaBr%02d", det),3201,-40,40, 3201,-40,40);
		FAT_TDCdt_ref_gated[det] = MakeTH1('D', Form("FATIMA/Timing/Gated/TDCdt_gated_LaBr%02d_LaBr%02d", FAT_REF_DET, det),
		                                   Form("TDC dt LaBr%02d (on %4.2f keV) - LaBr%02d (on %4.2f keV)",
																		FAT_REF_DET, E_gate1, det, E_gate2),
                                           3201,-40,40);
		FAT_E_TDCdt_ref_gated[det] = MakeTH2('D', Form("FATIMA/Timing/Gated/TDCdt_gated_LaBr%02d_E_LaBr%02d", FAT_REF_DET, det),
		                                     Form("TDC dt LaBr%02d (on %4.2f keV) - LaBr%02d (E)",
																		FAT_REF_DET, E_gate1, det),
                                             2001, 0, 2000, 3201,-40,40);
	}

	cout<<"Helleurrr"<<endl;

	//*****************
	
	C_t = MakeTH1('D',"pl","pl",1001,0,1000);

	tamex_Mult_lead = new TH1*[50];
	tamex_Mult_trail = new TH1*[50];

	tamex_Mult_Ch_lead = new TH1**[50];
	tamex_Mult_Ch_trail = new TH1**[50];

	tamex_mult_mat_lead = new TH2*[50];
	tamex_mult_mat_trail = new TH2*[50];

	LEAD_LEAD = new TH1***[50];

	for(int i = 0;i < 50;++i){
		
		tamex_Mult_lead[i] = NULL;
		tamex_Mult_trail[i] = NULL;

		tamex_mult_mat_lead[i] = NULL;
		tamex_mult_mat_trail[i] = NULL;

		tamex_Mult_Ch_lead[i] = new TH1*[50];
		tamex_Mult_Ch_trail[i] = new TH1*[50];
		LEAD_LEAD[i] = new TH1**[50];
		for(int j = 0;j < 50;++j){
			tamex_Mult_Ch_lead[i][j] = NULL;//MakeTH1('D',Form("tamex_channels_hists/tamex_lead_ch_%d_%d",i,j),Form("tamex_lead_ch_%d_%d",i,j),100,0,100);
			tamex_Mult_Ch_trail[i][j] = NULL;//MakeTH1('D',Form("tamex_channels_hists/tamex_trail_ch_%d_%d",i,j),Form("tamex_trail_ch_%d_%d",i,j),100,0,100);
			LEAD_LEAD[i][j] = new TH1*[50];
			for(int k = 0;k < 50;++k) LEAD_LEAD[i][j][k] = NULL;
		}
	}


	DIFF_ARR = new TH1*[36];
	for(int i = 0;i < 36;++i) DIFF_ARR[i] = NULL;//MakeTH1('D',Form("TDC_DIFF_CH_6_to_%d",i),Form("TDC_DIFF_CH_6_to_%d",i),300,-30000,0);

	WR_used = false;

	//used_systems
	get_used_Systems();
	get_WR_Config();

	//create White Rabbit obj
	WR = WR_used ? new White_Rabbit() : NULL;

	WR_HIST = MakeTH1('D',"WR","WR",2001,-1,40);
	WR_HIST2 = MakeTH1('D',"WR2","WR2",2001,-10,4000);
	WR_F = MakeTH1('D',"WRf","WRf",2001,-10,4000);
	
	// GALILEO Histograms //
	
	//GAL_Evt_Time = MakeTH1('D',"GALILEO_E","GALILEO_E",20001,0,800000);
	GAL_Pileup = MakeTH1('I',"GALILEO_Pileup","GALILEO Pileup",16,0,16);
	GAL_Hit_Pat = MakeTH1('I',"GALILEO_Hit_Pat","GALILEO Hit Pattern",16,0,16);
	GAL_Chan_Time_Diff = MakeTH1('D',"GALILEO_Chan_Time_DIff","GALILEO Channel Time Difference",21,-100,100);
	GAL_Chan_E_Mat = MakeTH2('D',"GALILEO_E_Mat","GALILEO Channel Energy Matrix",10001,0,800000,10001,0,800000);

	for(int i=0; i < 5; ++i){
	    GAL_Chan_E[i] = MakeTH1('D',Form("GALILEO_Energy_Spectra/GALILEO_E%2d",i),Form("GALILEO Channel Energy Channel %2d",i),80001,0,800000);
	    GAL_Time_Diff_vs_Energy[i] = MakeTH2('D',Form("GALILEO_dT_vs_Energy_Spectra/GALILEO_dT_vs_E%2d",i),Form("GALILEO Time Difference Vs Channel Energy Channel %2d",i),21,-100,100,10001,0,800000);
	}


	Trail_LEAD = new TH1**[50];
	Coarse = new TH1**[50];
	//lead_lead = new TH1**[4];
	for(int i = 0;i < 50;++i){
		Trail_LEAD[i] = new TH1*[50];
		Coarse[i] = new TH1*[50];
		//lead_lead[i] = new TH1*[17];
		for(int j = 0;j < 50;++j){
			Trail_LEAD[i][j] = NULL;
			Coarse[i][j] = NULL;
		//	lead_lead[i][j] = NULL;
		
		}
	}


	//create Detector Systems
	Detector_Systems = new Detector_System*[6];

	// all non used systems intialized as NULL 
	//-> calling uninitialized system will cause an error !
	Detector_Systems[0] = !Used_Systems[0] ? NULL : new BS_Detector_System();
	//Detector_Systems[1] = !Used_Systems[1] ? NULL : new AIDA_Detector_System();
	Detector_Systems[2] = !Used_Systems[2] ? NULL : new PLASTIC_Detector_System();
	Detector_Systems[3] = !Used_Systems[3] ? NULL : new FATIMA_Detector_System();
	Detector_Systems[4] = !Used_Systems[4] ? NULL : new GALILEO_Detector_System();

	for(int i = 0;i < 6;++i) if(!Used_Systems[i]) Detector_Systems[i] = NULL;

	get_interest_arrays();

	/*
	if(!SKIP_EVT_BUILDING){
		EvtBuilder = new EventBuilder*[2];
		EvtBuilder[0] = new Time_EventBuilder(amount_interest,length_interest,interest_array);
		//EvtBuilder[1] = new Space_EventBuilder();
	}
	*/
	//Raw_Event object to handle data
	RAW = new Raw_Event();

	load_PrcID_File();

	White_Rabbbit_old = 0;
	count = 0;
	iterator = 0;
	cals_done = false;
	val_it = 0;
	
	//Fix this in Detector_System.cxx either remove the
	// gain_match_used initialisation in FATIMA_Detector_System
	//and add a ::set_bool_gain_match(bool) or set FAT_gain_match_used
	//by the value in FATIMA_Detector_System constructor. Former
	//seems to make more sense.
	//FAT_gain_match_used = Detector_Systems[3]->do_gain_matching();
	FAT_gain_match_used = true;
	//Having an initialisation (below) and a user setting (above)
	//like this is probably bad.
	FAT_gain_match_done = false;

}


//----------------------------------------------------------


TSCNUnpackProc::~TSCNUnpackProc()
{
    
	Detector_Systems[3]->write();
	double mean = 0;
	for(int i = 0;i < val_it;++i) mean += vals[i];
	mean /= (double) val_it;
	double sq = 0;
	for(int i = 0;i < val_it;++i) sq += pow(mean - vals[i],2);
	sq = sqrt(sq)/((double) val_it);
	cout << "MEAN " << mean << " +- " << sq << endl;


	for(int i = 0;i < 6;++i){
		if(Detector_Systems[i]) delete Detector_Systems[i];
		delete[] interest_array[i];
	}
	delete[] interest_array;
	delete[] length_interest;
	delete[] Detector_Systems;
	
	delete RAW;
	cout << "**** TSCNUnpackProc: Delete" << endl;
}

//----------------------------------------------------------


Bool_t TSCNUnpackProc::BuildEvent(TGo4EventElement* dest)
{
    
    if(Used_Systems[3] && FAT_gain_match_used){

	    TGo4MbsEvent       *fMbsEvent = dynamic_cast<TGo4MbsEvent*>    (GetInputEvent("Unpack"));   // of step "Unpack";
	    s_filhe* fileheader=fMbsEvent->GetMbsSourceHeader();

	    input_data_path = fileheader->filhe_file;
	    
	    if(input_data_path == input_data_path_old) FAT_gain_match_done = true;
	    else if(input_data_path != input_data_path_old) FAT_gain_match_done = false;
	    
	    if(!FAT_gain_match_done){
	    
		input_data_path_old = input_data_path;
		
		cout<<"Filename = "<<input_data_path<<endl;
			
		for(unsigned int i = 0; i<input_data_path.length(); ++i){
			    if(input_data_path[i] == '/') file_pwd = i;
			    else if(input_data_path[i] == '.'){
			    file_end = i;
				gain_match_filename = "Configuration_Files/FATIMA_Gain_Match_Files/" + input_data_path.substr((file_pwd+1),(file_end - file_pwd -1)) + ".gm";
				    cout<<"gain_match_filename = "<<gain_match_filename<<endl;
			    }
		}
		Detector_Systems[3]->set_Gain_Match_Filename(gain_match_filename);
		FAT_gain_match_done = true;
		
	    }
	    
	}
	
	count++;
		
	if (count % 100000 == 0) cout << "Event " << count << " Reached!!!";
	cout << "\r";
	    
	
	if(cals_done) return kTRUE; //BAD!!!!
	//if(count > 500000) return kTRUE;
	Bool_t isValid=kFALSE; // validity of output event //
  
	TGo4MbsEvent* inp_evt = (TGo4MbsEvent* ) GetInputEvent(); // from this //
  
	TSCNUnpackEvent* out_evt = (TSCNUnpackEvent*) dest;
	

	if (inp_evt==0) // Ensures that there is data in the event //
	{
		isValid = kFALSE;	// default calling Fill method will set validity of out_evt to return value! //
		cout << "SCNUnpackProc: no input event !"<< endl;
		out_evt->SetValid(isValid);
		
		return isValid;
	}
	
	
	inp_evt->ResetIterator(); // ASK SUDIPTA //
	TGo4MbsSubEvent* psubevt(0); // ASK SUDIPTA //
	
	
	// ------------------------------------------------------ //
	// |                                                    | //
	// |               START OF EVENT ANALYSIS              | //
	// |                                                    | //
	// ------------------------------------------------------ //
	

	int subevent_iter = 0;

	bool used[5];
	for(int i = 0;i < 5;++i) used[i] = false;
	
	bool WHITE_RABBIT_USED = false;
	
	while ((psubevt = inp_evt->NextSubEvent()) != 0) // subevent loop //
	{
		subevent_iter++;

		Int_t* pdata=psubevt->GetDataField();

		Int_t lwords = psubevt->GetIntLen();
	
		Int_t PrcID=psubevt->GetProcid();
		
		//cout<<"Proc_ID is : "<<PrcID<<endl;

		Int_t PrcID_Conv = get_Conversion(PrcID);
				
		if(PrcID_Conv == 0){
			
		    Detector_Systems[PrcID_Conv]->Process_FRS(psubevt);
		    
		    Detector_Systems[PrcID_Conv]->get_Event_data(RAW);
		    		    
		    /*Float_t dE = RAW->get_FRS_dE(int i);
		    Float_t dE_cor = RAW->get_FRS_dE_corr(int i);

		    Float_t sci_l = RAW->get_FRS_sci_l(int i);
		    Float_t sci_r = RAW->get_FRS_sci_r(int i);
		    Float_t sci_e = RAW->get_FRS_sci_e(int i);
		    Float_t sci_tx = RAW->get_FRS_sci_tx(int i);
		    Float_t sci_x = RAW->get_FRS_sci_x(int i);*/
		    
		    Float_t sci_tofll2 = RAW->get_FRS_tofll2();
		    Float_t sci_tofll3 = RAW->get_FRS_tofll3();
		    Float_t sci_tof2 = RAW->get_FRS_tof2();
		    Float_t sci_tofrr2 = RAW->get_FRS_tofrr2();
		    Float_t sci_tofrr3 = RAW->get_FRS_tofrr3();
		    Float_t sci_tof3 = RAW->get_FRS_tof3();
		
		    Float_t ID_x2 = RAW->get_FRS_x2();
		    Float_t ID_y2 = RAW->get_FRS_y2();
		    Float_t ID_a2 = RAW->get_FRS_a2();
		    Float_t ID_b2 = RAW->get_FRS_b2();
		    
		    Float_t ID_x4 = RAW->get_FRS_x4();
		    Float_t ID_y4 = RAW->get_FRS_y4();
		    Float_t ID_a4 = RAW->get_FRS_a4();
		    Float_t ID_b4 = RAW->get_FRS_b4();
		    
		    /*Float_t ID_brho = RAW->get_FRS_brho(int i);
		    Float_t ID_rho = RAW->get_FRS_rho(int i);*/
		    
		    Float_t beta = RAW->get_FRS_beta();
		    Float_t beta3 = RAW->get_FRS_beta3();
		    Float_t gamma = RAW->get_FRS_gamma();
		    
		    Float_t AoQ = RAW->get_FRS_AoQ();
		    Float_t AoQ_corr = RAW->get_FRS_AoQ_corr();
		    
		    Float_t z = RAW->get_FRS_z();
		    Float_t z2 = RAW->get_FRS_z2();
		    Float_t z3 = RAW->get_FRS_z3();
		    
		    Float_t timestamp = RAW->get_FRS_timestamp();
		    Float_t ts = RAW->get_FRS_ts();
		    Float_t ts2 = RAW->get_FRS_ts2();		    
		    
		    /*cout<<"hsci_tofll2 "<<sci_tofll2<<endl;
		    cout<<"hsci_tofll3 "<<sci_tofll3<<endl;
		    cout<<"hsci_tof2 "<<sci_tof2<<endl;
		    cout<<"hsci_tofrr2 "<<sci_tofrr2<<endl;
		    cout<<"hsci_tofrr3 "<<sci_tofrr3<<endl;
		    cout<<"hsci_tof3 "<<sci_tof3<<endl;
		
		    cout<<"hID_x2 "<<ID_x2<<endl;
		    cout<<"hID_y2 "<<ID_y2<<endl;
		    cout<<"hID_a2 "<<ID_a2<<endl;
		    cout<<"hID_b2 "<<ID_b2<<endl;
		    
		    cout<<"hID_x4 "<<ID_x4<<endl;
		    cout<<"hID_y4 "<<ID_y4<<endl;
		    cout<<"hID_a4 "<<ID_a4<<endl;
		    cout<<"hID_b4 "<<ID_b4<<endl;
		    
		    
		    cout<<"hbeta "<<beta<<endl;
		    cout<<"hbeta3 "<<beta3<<endl;
		    cout<<"hgamma "<<gamma<<endl;
		    
		    cout<<"hAoQ "<<AoQ<<endl;
		    cout<<"hAoQ_corr "<<AoQ_corr<<endl;
		    
		    cout<<"hz "<<z<<endl;
		    cout<<"hz2 "<<z2<<endl;
		    cout<<"hz3 "<<z3<<endl;
		    
		    cout<<"htimestamp "<<timestamp<<endl;
		    cout<<"hts "<<ts<<endl;
		    cout<<"hts2 "<<ts2<<endl;*/
		    
		    	    
		    if(sci_tofll2) hsci_tofll2->Fill(sci_tofll2);
		    if(sci_tofll3) hsci_tofll3->Fill(sci_tofll3);
		    if(sci_tof2) hsci_tof2->Fill(sci_tof2);;
		    if(sci_tofrr2) hsci_tofrr2->Fill(sci_tofrr2);
		    if(sci_tofrr3) hsci_tofrr3->Fill(sci_tofrr3);
		    if(sci_tof3) hsci_tof3->Fill(sci_tof3);
		
		    /*if(ID_x2) hID_x2->Fill(ID_x2);
		    if(ID_y2) hID_y2->Fill(ID_y2);
		    if(ID_a2) hID_a2->Fill(ID_a2);
		    if(ID_b2) hID_b2->Fill(ID_b2);
		    
		    if(ID_x4) hID_x4->Fill(ID_x4);
		    if(ID_y4) hID_y4->Fill(ID_y4);
		    if(ID_a4) hID_a4->Fill(ID_a4);
		    if(ID_b4) hID_b4->Fill(ID_b4);*/
		    
		    
		    if(beta) hbeta->Fill(beta);
		    if(beta3) hbeta3->Fill(beta3);
		    if(gamma) hgamma->Fill(gamma);
		    
		    cout<<"AoQ I Guess?? "<<AoQ<<endl;
		    
		    if(AoQ) hAoQ->Fill(AoQ);
		    if(AoQ_corr) hAoQ_corr->Fill(AoQ_corr);
		    
		    if(z) hz->Fill(z);
		    if(z2) hz2->Fill(z2);
		    if(z3) hz3->Fill(z3);
		    
		    if(timestamp) htimestamp->Fill(timestamp);
		    if(ts) hts->Fill(ts);
		    if(ts2) hts2->Fill(ts2);
		    
		    
		    continue;
		    
		}
			    
		    
		if(WHITE_RABBIT_USED){
			WR_tmp[iterator] = WR->get_White_Rabbit(pdata);
			pdata += WR->get_increase();
		}
		called[iterator] = PrcID_Conv;
		

		//cout << WR_tmp[iterator] << " " << iterator << endl;

		

		//continue;

		if(PrcID_Conv == 3 && false){
			cout << "---------------------\n";
			for(int i = 0;i < lwords;++i){
				cout << hex << *(pdata + i) << " ";
				if(i % 5 == 0 && i > 0) cout << endl;
			}
			cout << "\n---------------------\n";
		}
		


		//send subevent to respective unpacker
		Detector_Systems[PrcID_Conv]->Process_MBS(pdata);
		
		
		
		//get mbs stream data from unpacker (pointer copy solution)
		pdata = Detector_Systems[PrcID_Conv]->get_pdata();
		
		
		//get data from subevent
		Detector_Systems[PrcID_Conv]->get_Event_data(RAW);
		

		//if(PrcID_Conv == 3) Detector_Systems[PrcID_Conv]->get_Event_data(data_stream[PrcID_Conv]);

		cals_done = Detector_Systems[PrcID_Conv]->calibration_done();
		
		/*	
		if(!SKIP_EVT_BUILDING){
			EvtBuilder[0]->set_Event(RAW);
			//EvtBuilder[1]->set_Event(RAW);
		}
		*/
		
		if(cals_done) break;
		//continue;
		//temporary
		used[PrcID_Conv] = true;

		//FATIMA CASE
		if(PrcID_Conv == 3){
			double dt1, dt2;
			for (int i=0; i<RAW->get_FAT_QDCs_fired(); i++) {
				FAT_Eraw[RAW->get_FAT_QDC_id(i)]->Fill(RAW->get_FAT_QLong_Raw(i));
			}
			
			int dets_fired = RAW->get_FAT_det_fired();
			for (int i=0; i<dets_fired; i++) {
				int deti = RAW->get_FAT_id(i);
				
				FAT_hits->Fill(deti);
				FAT_Esum->Fill(RAW->get_FAT_E(i));
				FAT_E[deti]->Fill(RAW->get_FAT_E(i));
				FAT_E_ratio[deti]->Fill(RAW->get_FAT_E(i), RAW->get_FAT_ratio(i));
				for (int j=0; j<dets_fired; j++) {
					int detj = RAW->get_FAT_id(j);
					if ( deti==detj )
						continue;
					
					FAT_gg->Fill(RAW->get_FAT_E(i), RAW->get_FAT_E(j));
					if (deti < detj) {
						dt1 = RAW->get_FAT_t(i) - RAW->get_FAT_t(j);
						dt2 = RAW->get_FAT_t_qdc(i) - RAW->get_FAT_t_qdc(j);
						FAT_TDCdtsum->Fill(dt1);
						FAT_QDCdtsum->Fill(dt2);
						FAT_TDCdtsum_ref_gated->Fill(dt1);
						FAT_QDCdtsum_ref_gated->Fill(dt2);
					}else{
						dt1 = RAW->get_FAT_t(j) - RAW->get_FAT_t(i);
						dt2 = RAW->get_FAT_t_qdc(j) - RAW->get_FAT_t_qdc(i);
					}	
						
						//printf("Gates %4.2lf-%4.2lf  %4.2lf-%4.2lf\n", FATgate1_low, FATgate1_high,
						//										     FATgate1_low, FATgate1_high);
						//printf("Energies %4.2lf %4.2lf\n", RAW->get_FAT_E(i), RAW->get_FAT_E(j));
						
					
					if (deti == FAT_REF_DET) {
						FAT_gg_ref[detj]->Fill(RAW->get_FAT_E(i), RAW->get_FAT_E(j));
						FAT_gg_ref[detj]->Fill(RAW->get_FAT_E(j), RAW->get_FAT_E(i));
						FAT_TDCdt_ref[detj]->Fill(dt1);
						FAT_QDCdt_ref[detj]->Fill(dt2);
						FAT_TDC_QDC_dt[detj]->Fill(dt1, dt2);
						if (RAW->get_FAT_E(i) > FATgate1_low 
								&& RAW->get_FAT_E(i) < FATgate1_high) {
							FAT_E_TDCdt_ref_gated[detj]->Fill(RAW->get_FAT_E(j), dt1);
							if (RAW->get_FAT_E(j) > FATgate2_low 
									&& RAW->get_FAT_E(j) < FATgate2_high) {
								FAT_TDCdt_ref_gated[detj]->Fill(dt1);
								printf("hit gates\n");									
							}
						}
					}
				}

			}
			
			//if(am_FATIMA_hits == 2) FAT_MAT->Fill(RAW->get_FATIMA_E(0),RAW->get_FATIMA_E(1));
		}
		//PLASTIC CASE
		if(PrcID_Conv == 2){
			//do something here
			int pl_iter = 0;
			int sum_l = 0;
			int sum_t = 0;

			int phys_ch = 0;
			int phys_ch_tmp = 0;
			int sum_phys_l[17];
			int sum_phys_t[17];
			int called_channels[17];
			for(int i = 0;i < 17;++i){
				sum_phys_t[i] = 0;
				sum_phys_l[i] = 0;
				called_channels[i] = 0;
			}


			for(int i = 0;i < 4;++i){
			    
				sum_l = 0;
				sum_t = 0;
				pl_iter = RAW->get_PLASTIC_am_Fired(i);
				if(pl_iter > 200) pl_iter = 0;
				for(int j = 0;j < pl_iter;++j){

					//cout << "i " << i << " j " << j << " ";// << endl; 
					phys_ch = RAW->get_PLASTIC_physical_channel(i,j);
					//cout << "-> phys_ch " << phys_ch << endl;
					called_channels[j] = phys_ch;

					sum_phys_l[phys_ch] += RAW->get_PLASTIC_physical_lead_hits(i,phys_ch);
					sum_phys_t[phys_ch] += RAW->get_PLASTIC_physical_trail_hits(i,phys_ch);
					

					sum_l += RAW->get_PLASTIC_lead_hits(i);
					sum_t += RAW->get_PLASTIC_trail_hits(i);
					
					
					if(!Trail_LEAD[i][phys_ch]) Trail_LEAD[i][phys_ch] = MakeTH1('D',Form("trail_minus_lead/trail_minus_lead_board%d_ch%d",i,phys_ch),Form("trail_minus_lead_board%d_ch%d",i,phys_ch),500,0,500);
					
					if(j % 2 == 0){
						Trail_LEAD[i][phys_ch]->Fill(RAW->get_PLASTIC_trail_T(i,j+1)-RAW->get_PLASTIC_lead_T(i,j));
						for(int k = 0;k < pl_iter;++k){
							phys_ch_tmp = RAW->get_PLASTIC_physical_channel(i,k);
							if(k % 2 == 0 && k != j){
								//cout << "i " << i << " " << j << " " << k << " " << phys_ch << " " << phys_ch_tmp << " " << pl_iter<< endl;
								if(!LEAD_LEAD[i][phys_ch][phys_ch_tmp]) LEAD_LEAD[i][phys_ch][phys_ch_tmp] = MakeTH1('D',Form("lead_minus_lead/lead_minus_lead_board_%d_from_ch%d_to_%d",i,phys_ch,phys_ch_tmp),Form("lead_minus_lead_board%d_from_ch%d_to_%d",i,phys_ch,phys_ch_tmp),500,-1000,1000);
								LEAD_LEAD[i][phys_ch][phys_ch_tmp]->Fill(RAW->get_PLASTIC_lead_T(i,j) - RAW->get_PLASTIC_lead_T(i,k));
							}
						}
					}

					
					if(!Coarse[i][phys_ch]) Coarse[i][phys_ch] = MakeTH1('D',Form("coarse_%d_%d",i,phys_ch),Form("coarse_%d_%d",i,phys_ch),500,0,5000);

					if(j % 2 == 0) Coarse[i][phys_ch]->Fill(RAW->get_PLASTIC_coarse_lead(i,j));


				}
				for(int j = 0;j < pl_iter;++j){
					if(sum_phys_l[called_channels[j]] > 0){
						if(!tamex_Mult_Ch_lead[i][called_channels[j]]) tamex_Mult_Ch_lead[i][called_channels[j]] = MakeTH1('D',Form("tamex_channels_hists/tamex_lead_ch_%d_%d",i,called_channels[j]),Form("tamex_lead_ch_%d_%d",i,called_channels[j]),30,0,30);
						if(!tamex_mult_mat_lead[i]) tamex_mult_mat_lead[i] = MakeTH2('D',Form("tamex_mat_lead_%d",i),Form("tamex_mat_lead_%d",i),20,0,20,30,0,30);
						tamex_Mult_Ch_lead[i][called_channels[j]]->Fill(sum_phys_l[called_channels[j]]);
						tamex_mult_mat_lead[i]->Fill(called_channels[j],sum_phys_l[called_channels[j]]);
					}
					if(sum_phys_t[called_channels[j]] > 0){
						if(!tamex_Mult_Ch_trail[i][called_channels[j]]) tamex_Mult_Ch_trail[i][called_channels[j]] = MakeTH1('D',Form("tamex_channels_hists/tamex_trail_ch_%d_%d",i,called_channels[j]),Form("tamex_trail_ch_%d_%d",i,called_channels[j]),30,0,30);
						if(!tamex_mult_mat_trail[i]) tamex_mult_mat_trail[i] = MakeTH2('D',Form("tamex_mat_trail_%d",i),Form("tamex_mat_trail_%d",i),20,0,20,30,0,30);
						tamex_Mult_Ch_trail[i][called_channels[j]]->Fill(sum_phys_t[called_channels[j]]);
						tamex_mult_mat_trail[i]->Fill(called_channels[j],sum_phys_t[called_channels[j]]);
					}
					sum_phys_l[called_channels[j]] = 0;
					sum_phys_t[called_channels[j]] = 0;
				}
				if(sum_l > 0){
					if(!tamex_Mult_lead[i]) tamex_Mult_lead[i] = MakeTH1('D',Form("tamex_lead_%d",i),Form("tamex_lead_%d",i),100,0,100);		
					tamex_Mult_lead[i]->Fill(sum_l);
				}
				if(sum_t > 0){
					if(!tamex_Mult_trail[i]) tamex_Mult_trail[i] = MakeTH1('D',Form("tamex_trail_%d",i),Form("tamex_trail_%d",i),100,0,100);
					tamex_Mult_trail[i]->Fill(sum_t);
				}
			}
		}
		
		// GALILEO CASE //
		if(PrcID_Conv == 4){
		    
		    //cout<<"Something = "<<RAW->get_GALILEO_Chan_E(0)<<endl;
		    
		    am_GALILEO_hits = RAW->get_GALILEO_am_Fired();
		    		    		    
		    
		    double tmpGAL[2];

		    for(int i = 0;i < am_GALILEO_hits;++i){
			//e,g, sum spectrum
						
			tmpGAL[i] = RAW->get_GALILEO_Chan_E(i);
			
			GAL_Chan_E[i]->Fill(tmpGAL[i]);
			GAL_Pileup->Fill(RAW->get_GALILEO_Pileup(i));
			GAL_Hit_Pat->Fill(RAW->get_GALILEO_Hit_Pattern(i));
			
			for(int j = 0;j < am_GALILEO_hits;++j){
			    
			    if(i != j){
				
				double time_1 = RAW->get_GALILEO_Chan_T(i);
				double time_2 = RAW->get_GALILEO_Chan_T(j);
				
				double GAL_chan_time_diff = time_1 - time_2;

				GAL_Chan_Time_Diff->Fill(GAL_chan_time_diff);
								
				GAL_Time_Diff_vs_Energy[i]->Fill(GAL_chan_time_diff,tmpGAL[i]);
				
			    }
			    
			}

			
		    }
		    
		    //cout<<"Channel 0 Energy "<<tmpGAL[0]<<"    Channel 1 Energy "<<tmpGAL[1]<<endl;
		    
		    if(am_GALILEO_hits >= 2){GAL_Chan_E_Mat->Fill(tmpGAL[0],tmpGAL[1]);}
			
		}
		
		

		iterator++;
	}

	

	//========================================================
	//	the White rabbit time difference of coincident events
	//	in FATIMA and PLASTIC is roughly (222 +- 8) ns
	//========================================================

	//rudimentary event builder
	if(iterator == 2){
		if(called[0] != called[1]){
			//White rabbit histograms
			if((WR_tmp[1] - WR_tmp[0])/1000.  <= 10){
				if(called[0] == 3) WR_HIST->Fill( ((double)(WR_tmp[1] - WR_tmp[0]))/1000. );
				else WR_HIST->Fill( ((double)(WR_tmp[0] - WR_tmp[1]))/1000. );
			}

			if(called[0] == 3) WR_HIST2->Fill( ((double)(WR_tmp[1] - WR_tmp[0]))/1000. );
			else WR_HIST2->Fill( ((double)(WR_tmp[0] - WR_tmp[1]))/1000.);

			//========================================================
			//	"EVENTBUILDER" IS HERE!!!
			//========================================================

			//"coincident" events -> reset iterator
			if(abs((WR_tmp[1] - WR_tmp[0]) - 222 ) <= 8){
				//do all things related to coincidence analysis here


				iterator = 0;
			}
			else{
				//WR_tmp[0] = WR_tmp[1];
				//called[0] = called[1];
				iterator = 0;
			}
		}
		else if(called[0] == 3){
			WR_F->Fill(((double)(WR_tmp[1] - WR_tmp[0]))/1000.);
		}
		if(called[0] == called[1]){
			//sets second fired system to first (for time differences)

			//========================================================
			//change of variables (e.g. x[0] = x[1] also needed!)
			//========================================================
			//WR_tmp[0] = WR_tmp[1];
			//called[0] = called[1];
			iterator = 0;
		}
				
	}
	if(iterator > 2) iterator = 0;


	out_evt->SetValid(isValid);
	
	return isValid;
	
}



void TSCNUnpackProc::load_PrcID_File(){
	ifstream data("Configuration_Files/PrcID_to_Det_Sys.txt");
	if(data.fail()){
		cerr << "Could not find PrcID config file!" << endl;
		exit(0);
	}
	int id[5] = {0,0,0,0,0};
	int i = 0;
	string line;
	char s_tmp[100];
	while(data.good()){
		getline(data,line,'\n');
		if(line[0] == '#') continue;
		sscanf(line.c_str(),"%s %d %d %d %d %d",s_tmp,&id[0],&id[1],&id[2],&id[3],&id[4]);
		for(int j = 0; j < 5; ++j) PrcID_Array[i][j] = id[j];
		i++;
	}
}


Int_t TSCNUnpackProc::get_Conversion(Int_t PrcID){

	for(int i = 0;i < 6;++i){
	    for(int j = 0;j < 5;++j){
	     if(PrcID == PrcID_Array[i][j]) return i;
	    }
	}
	cerr << "ProcID " << PrcID << " not known!" << endl;
	exit(0);
}

void TSCNUnpackProc::get_used_Systems(){
	ifstream data("Configuration_Files/Used_Systems.txt");
	if(data.fail()){
		cerr << "Could not find Used_Systems config file!" << endl;
		exit(0);
	}
	int i = 0;
	int id = 0;
	string line;
	char s_tmp[100];
	while(data.good()){
		getline(data,line,'\n');
		if(line[0] == '#') continue;
		sscanf(line.c_str(),"%s %d",s_tmp,&id);
		Used_Systems[i] = (id == 1);
		i++;
	}
}




void TSCNUnpackProc::get_WR_Config(){
	ifstream data("Configuration_Files/White_Rabbit.txt");
	if(data.fail()){
		cerr << "Could not find White_Rabbit config file!" << endl;
		exit(0);
	}
	
	int id = 0;
	string line;
	char s_tmp[100];
	while(data.good()){
		getline(data,line,'\n');
		if(line[0] == '#') continue;
		sscanf(line.c_str(),"%s %d",s_tmp,&id);
		WR_used = (id == 1);
	}
}



void TSCNUnpackProc::get_interest_arrays(){
	SKIP_EVT_BUILDING = false;
	amount_interest = 0;

	length_interest = new int[10];
	interest_array = new int*[10];
	for(int i = 0;i < 10;++i){
		length_interest[i] = 0;
		interest_array[i] = new int[6];
		for(int j = 0;j < 6;++j) interest_array[i][j] = -1;
	}

	string DET_NAME[6] = {"FRS","AIDA","PLASTIC","FATIMA","GALILEO","FINGER"};

	const char* format = "%d %d %d %d %d %d";

	int tmp_values[6];

	ifstream data("Configuration_Files/Coincidences_of_Interest.txt");

	//Print statements if loading of data fails
	if(data.fail()){
		string input_string;
		cerr << endl;
		cerr << "No Detector_System coincidence file found!" << endl;
		cerr << "Do you want to omit the Time_EventBuilding?  (y/n)\t\t ";
		getline(cin,input_string);
		cerr << endl;
		if(input_string == "y"){
			cerr << "------------------------------------------------------" << endl;
			cerr << "!Time_EventBuilding will be skipped! ONLY SINGLES" << endl;
			cerr << "------------------------------------------------------" << endl;
			SKIP_EVT_BUILDING = true;
			return;
		}
		else{
			cerr << "Not skipping Time_EventBuilding.\n";
			cerr << "PLEASE CREATE Configuration_Files/Coincidences_of_Interest.txt file!" << endl;
			cerr << "\nEXITING PROGRAM NOW" << endl;
			exit(0);
		} 
	}

	string line;

	cout << "\n=====================================================" << endl;
	cout << "Coincidences of interest are: " << endl;
	cout << "-----------------------------------------------------" << endl;
	while(data.good()){
		getline(data,line,'\n');
		if(line[0] == '#') continue;
		for(int i = 0;i < 6;++i) tmp_values[i] = -1;

		sscanf(line.c_str(),format,&tmp_values[0],&tmp_values[1]
								  ,&tmp_values[2],&tmp_values[3]
								  ,&tmp_values[4],&tmp_values[5]);
		
		if(tmp_values[0] != -1){
			for(int i = 0;i < 6;++i){
				if(tmp_values[i] != -1){
					interest_array[amount_interest][length_interest[amount_interest]] = tmp_values[i];
					length_interest[amount_interest]++;
				}
			}
			cout << "-> ";
			for(int i = 0;i < length_interest[amount_interest]-1;++i) cout << DET_NAME[interest_array[amount_interest][i]] << " + ";
			cout << DET_NAME[interest_array[amount_interest][length_interest[amount_interest]-1]];
			cout << endl;
			amount_interest++;
		}
	}
	cout << "=====================================================" << endl;
}









