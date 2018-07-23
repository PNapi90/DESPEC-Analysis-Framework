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
	
	cout << "**** TSCNUnpackProc: Create" << endl;
	
	
	all =  MakeTH1('D', "je","hey", 2000, 0.,10000);
	
	
	all2 = MakeTH1('D',"2","2",60,-30,30);

	FAT_E = MakeTH1('D',"FATIMA_E","FATIMA_E",2001,0,8000);
	FAT_MAT = MakeTH2('D',"FAT_MAT","FAT_MAT",1001,0,4000,1001,0,4000);
	FAT_MAT_2 = MakeTH2('D',"FAT_MAT_2","FAT_MAT_2",1001,0,4000,1001,0,4000);
	
	FAT_TDC_Diff = MakeTH1('D',"FATIMA_TDC","FATIMA_TDC",3200,-40,40);
	
	for(int i = 0; i<36; ++i){

	    FAT_TDC_dT[i] = MakeTH1('D',Form("TDC_Diff_Hists/TDC_Time_Diff%2d",i),Form("TDC Time Difference%2d",i),3201,-40,40);
	    Energy_Singles[i] = MakeTH1('D',Form("Energy_Singles/Energy_Singles%2d",i),Form("Energy Singles%2d",i),8000,0,8000);
	    Energy_Coinc[i] = MakeTH1('D',Form("Energy_Coincidences/Energy_Coinc%2d",i),Form("Energy Coincidences%2d",i),8000,0,8000);
	    FAT_En_vs_dT[i] = MakeTH2('D',Form("Energy_vs_dT/Energy_vs_dT%2d",i),Form("Energy Vs dT%2d",i),1001,0,4000,3201,-40,40);

	}

	hit_mat = MakeTH2('D',"hitmat","hitmat",37,0,36,37,0,36);

	hit_hist = MakeTH1('D',"hits","hits",60,0,60);
	am_hits = MakeTH1('D',"hits_a","hits_a",51,0,50);

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
	tdc_hist = MakeTH1('D',"tdc","tdc",1000,-60,1000);

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
	FAT_gain_match_used = false;
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
    
    if(Used_Systems[3] && FAT_gain_match_used && !FAT_gain_match_done){

		TGo4MbsEvent       *fMbsEvent = dynamic_cast<TGo4MbsEvent*>    (GetInputEvent("Unpack"));   // of step "Unpack";
		s_filhe* fileheader=fMbsEvent->GetMbsSourceHeader();

	    string input_data_path = fileheader->filhe_file;
	
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
		
		//cout<<"ProcID is "<<PrcID<<endl;

		Int_t PrcID_Conv = get_Conversion(PrcID);
				
		if(PrcID_Conv == 0){
		    		    	    
		   Detector_Systems[PrcID_Conv]->Process_FRS(psubevt);
		    		    
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
			//to get histograms (e.g.)
			am_FATIMA_hits = RAW->get_FATIMA_am_Fired();
			int tdc_hits = RAW->get_FATIMA_am_Fired_TDC();
			if(am_hits > 0) am_hits->Fill(am_FATIMA_hits);
					    
			double sum = 0;
			double tmpE[2];
			double TDC_times[2] = {0,0};
			double TDC_time_6[2];
			int det_iter = 0;
			bool called_link = false;
			
			num_full_FAT_evts = RAW->get_am_FATIMA_Both(); 
			
			for(int i = 0; i < num_full_FAT_evts; ++i){
			    
			    int Data_Ref_i = RAW->get_FATIMA_Both(i);
			    int Det_ID_i = RAW->get_FATIMA_det_id(Data_Ref_i);
			    //int Data_Ref_i = RAW->get_FATIMA_Both(i);

			    tmpE[0] = RAW->get_FATIMA_E(Data_Ref_i);
			    
			    if(num_full_FAT_evts == 1){
				
				 Energy_Singles[Det_ID_i]->Fill(tmpE[0]);

			    }
			    if(num_full_FAT_evts == 2){
				
				 Energy_Coinc[0]->Fill(tmpE[0]);



			    }
			    for(int j = 0; j < num_full_FAT_evts; ++j){
				
				int Data_Ref_j = RAW->get_FATIMA_Both(j);		    		    
				int Det_ID_j = RAW->get_FATIMA_det_id(Data_Ref_j);
				
				if(Det_ID_i != Det_ID_j) hit_mat->Fill(Det_ID_i,Det_ID_j);
				
				tmpE[0] = RAW->get_FATIMA_E(Data_Ref_i);
				tmpE[1] = RAW->get_FATIMA_E(Data_Ref_j);

				if(Det_ID_i > Det_ID_j && num_full_FAT_evts > 1) FAT_MAT->Fill(tmpE[0], tmpE[1]);
				if(Det_ID_i != Det_ID_j && num_full_FAT_evts > 1) FAT_MAT_2->Fill(tmpE[0],tmpE[1]);

				
				if(Det_ID_i != Det_ID_j /*&& Det_ID_j == 0*/){
				    
				    
				    
				    //int Data_Ref_j = RAW->get_FATIMA_Both(j);		    		    
				
				    double TDC_Time_1 = RAW->get_FATIMA_TDC_T(Data_Ref_i);
				    double TDC_Time_2 = RAW->get_FATIMA_TDC_T(Data_Ref_j);
				    
				    if(Det_ID_j == 0) FAT_TDC_dT[Det_ID_i]->Fill((TDC_Time_1 - TDC_Time_2));
				    
				    FAT_En_vs_dT[Det_ID_i]->Fill(tmpE[0],(TDC_Time_1 - TDC_Time_2));
			
				}
			
			
			    }
			    
			}
			
			if(RAW->CH_51_FIRED() && tdc_hits == 3){
				int id_tmp = RAW->get_FATIMA_det_id(0);
				double tdiff = RAW->get_FATIMA_Time_Diff();
				if(!DIFF_ARR[id_tmp]) DIFF_ARR[id_tmp] = MakeTH1('D',Form("TDC_DIFF_CH_6_to_%d",id_tmp),Form("TDC_DIFF_CH_6_to_%d",id_tmp),300,-30000,0);
				DIFF_ARR[id_tmp]->Fill(tdiff);
			}
			int tdc_iter = 0;
			for(int i = 0;i < tdc_hits;++i){
				//hit_hist->Fill(RAW->get_FATIMA_det_id(i));
				if(RAW->get_FATIMA_QDC_TDC_LINKED(i)){
					//e,g, sum spectrum
					tmpE[i] = RAW->get_FATIMA_E(i);
					sum += tmpE[i];
					hit_hist->Fill(RAW->get_FATIMA_det_id(i));
					tdc_hist->Fill(RAW->get_FATIMA_TDC_T(i)*1e-6);
					if(RAW->get_FATIMA_det_id(i) < 50){
						det_iter = RAW->get_FATIMA_det_id(i);
					}
					called_link = true;
				}
				else{
					if(RAW->get_FATIMA_det_id(i) >= 17 && called_link && false){
						TDC_time_6[tdc_iter] = (double) RAW->get_FATIMA_TDC_T(i);
						tdc_iter++;
					}
				}
				
				for(int j = 0; j < tdc_hits; ++j){


				    if (i != j ){
					 
					 
					//cout<<"First timestamp "<<RAW->get_FATIMA_TDC_T(i)<<"  Second timestamp  "<<RAW->get_FATIMA_TDC_T(j)<<endl;

					TDC_times[0] = RAW->get_FATIMA_TDC_T(i);
					TDC_times[1] = RAW->get_FATIMA_TDC_T(j);
					 
					//cout<<(TDC_times[0] - TDC_times[1])*1e-3<<endl;

					FAT_TDC_Diff->Fill(((TDC_times[0] - TDC_times[1])*1e-3));
				    
				    }
				}
			}
			
			//if(am_FATIMA_hits == 2) FAT_MAT->Fill(RAW->get_FATIMA_E(0),RAW->get_FATIMA_E(1));
			if(am_FATIMA_hits > 0 && sum > 0) FAT_E->Fill(sum);
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









