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
#include <time.h>
#include <math.h>
#include <iomanip>




#include "TSCNUnpackEvent.h"

#include "Detector_System.cxx"
#include "FATIMA_Detector_System.h"
#include "PLASTIC_Detector_System.h"
#include "GALILEO_Detector_System_TEST.h"

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



TSCNUnpackProc::TSCNUnpackProc(const char* name) :
TGo4EventProcessor(name) // Histograms defined here //
{
		
	cout << "**** TSCNUnpackProc: Create" << endl;
	
	
	all =  MakeTH1('D', "je","hey", 2000, 0.,10000);
	
	
	all2 = MakeTH1('D',"2","2",60,-30,30);

	FAT_E = MakeTH1('D',"FATIMA_E","FATIMA_E",2001,0,8000);
	FAT_MAT = MakeTH2('D',"FAT_MAT","FAT_MAT",1001,0,4000,1001,0,4000);

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
	GAL_Pileup = MakeTH1('D',"GALILEO_Pileup","GALILEO Pileup",101,0,100);
	GAL_Hit_Pat = MakeTH1('D',"GALILEO_Hit_Pat","GALILEO Hit Pattern",101,0,100);
	GAL_Chan_Time_Diff = MakeTH1('D',"GALILEO_Chan_Time_DIff","GALILEO Channel Time Difference",20001,-5000,5000);
	GAL_Chan_E = MakeTH1('D',"GALILEO_E","GALILEO Channel Energy",80001,0,800000);
	GAL_Chan_E_Mat = MakeTH2('D',"GALILEO_E_Mat","GALILEO Chanel Energy Matrix",10001,0,800000,10001,0,800000);

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
	//Detector_Systems[0] = !Used_Systems[0] ? NULL : new FRS_Detector_System();
	//Detector_Systems[1] = !Used_Systems[1] ? NULL : new AIDA_Detector_System();
	Detector_Systems[2] = !Used_Systems[2] ? NULL : new PLASTIC_Detector_System();
	Detector_Systems[3] = !Used_Systems[3] ? NULL : new FATIMA_Detector_System();
	Detector_Systems[4] = !Used_Systems[4] ? NULL : new GALILEO_Detector_System();

	for(int i = 0;i < 6;++i) if(!Used_Systems[i]) Detector_Systems[i] = NULL;


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
	}
	delete[] Detector_Systems;
	
	delete RAW;
	cout << "**** TSCNUnpackProc: Delete" << endl;
}

//----------------------------------------------------------


Bool_t TSCNUnpackProc::BuildEvent(TGo4EventElement* dest)
{
    
    	if (Used_Systems[3] && FAT_gain_match_used && !FAT_gain_match_done){
	    
	    
	    TGo4MbsEvent       *fMbsEvent = dynamic_cast<TGo4MbsEvent*>    (GetInputEvent("Unpack"));   // of step "Unpack";
	
	    s_filhe* fileheader=fMbsEvent->GetMbsSourceHeader();

	    string input_data_path = fileheader->filhe_file;
	
	    cout<<"Filename = "<<input_data_path<<endl;
	    	    
	    for(int i = 0; i<input_data_path.length(); ++i){
		
		
		if(input_data_path[i] == '/') file_pwd = i;
		else if(input_data_path[i] == '.'){
		    
		    file_end = i;
		
		    gain_match_filename = "FATIMA_Gain_Match_Files/" + input_data_path.substr((file_pwd+1),(file_end - file_pwd -1)) + ".gm";
		
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
	
	double E0,E1;
	int fat_hits = 0;
	
	
	int subevent_iter = 0;

	bool used[5];
	for(int i = 0;i < 5;++i) used[i] = false;
	
	bool WHITE_RABBIT_USED = true;
	
	while ((psubevt = inp_evt->NextSubEvent()) != 0) // subevent loop //
	{
		subevent_iter++;

		Int_t* pdata=psubevt->GetDataField();

		Int_t lwords = psubevt->GetIntLen();
	
		Int_t PrcID=psubevt->GetProcid();

		Int_t PrcID_Conv = get_Conversion(PrcID);

		if(WHITE_RABBIT_USED){
			WR_tmp[iterator] = WR->get_White_Rabbit(pdata);
			pdata += WR->get_increase();
		}
		called[iterator] = PrcID_Conv;
		

		//cout << WR_tmp[iterator] << " " << iterator << endl;

		

		//continue;

		if(PrcID_Conv == 2 && false){
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
						TDC_times[i] = (double) RAW->get_FATIMA_TDC_T(i);
					//cout << "YES! " << det_iter << " " << TDC_times[i] << endl;
					}
					called_link = true;
				}
				else{
					if(RAW->get_FATIMA_det_id(i) >= 17 && called_link && false){
						//cout << "Oh YEAH " <<  det_iter << " " << RAW->get_FATIMA_TDC_T(i) << " " << TDC_times[det_iter] << endl;
						TDC_time_6[tdc_iter] = (double) RAW->get_FATIMA_TDC_T(i);
						tdc_iter++;
					}
				}
			}
			for(int i = 0;i < tdc_iter*0;++i){
				//if(TDC_time_6[i] > 0 && TDC_times[0] > 0) DIFF_ARR[i]->Fill(TDC_time_6[i] - TDC_times[0]);
			}
			if(am_FATIMA_hits == 2) FAT_MAT->Fill(RAW->get_FATIMA_E(0),RAW->get_FATIMA_E(1));
			if(am_FATIMA_hits > 0 && sum > 0) FAT_E->Fill(sum);
		}
		//PLASTIC CASE
		if(PrcID_Conv == 2){
			//do something here
			int fired_pl[17];
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


			for(int i = 0;i < 17;++i){
			    
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
					
					
					if(!Trail_LEAD[i][phys_ch]) Trail_LEAD[i][phys_ch] = MakeTH1('D',Form("trail_minus_lead_board%d_ch%d",i,phys_ch),Form("trail_minus_lead_board%d_ch%d",i,phys_ch),500,-50,50);
					
					if(j % 2 == 0){
						Trail_LEAD[i][phys_ch]->Fill(RAW->get_PLASTIC_trail_T(i,j+1)-RAW->get_PLASTIC_lead_T(i,j));
						for(int k = 0;k < pl_iter;++k){
							phys_ch_tmp = RAW->get_PLASTIC_physical_channel(i,k);
							if(k % 2 == 0 && k != j){
								//cout << "i " << i << " " << j << " " << k << " " << phys_ch << " " << phys_ch_tmp << " " << pl_iter<< endl;
								if(!LEAD_LEAD[i][phys_ch][phys_ch_tmp]) LEAD_LEAD[i][phys_ch][phys_ch_tmp] = MakeTH1('D',Form("lead_minus_lead_board_%d_from_ch%d_to_%d",i,phys_ch,phys_ch_tmp),Form("lead_minus_lead_board%d_from_ch%d_to_%d",i,phys_ch,phys_ch_tmp),500,-1000,1000);
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
			
			//cout<<"Filled Channel Energy = "<<(RAW->get_GALILEO_Chan_E(i))<<endl;
			
			tmpGAL[i] = RAW->get_GALILEO_Chan_E(i);

			//cout<<"Something Else = "<<tmpGAL[i]<<endl;
			
			GAL_Chan_E->Fill(tmpGAL[i]);
			GAL_Pileup->Fill(RAW->get_GALILEO_Pileup(i));
			GAL_Hit_Pat->Fill(RAW->get_GALILEO_Hit_Pattern(i));
			
			for(int j = 0;j < am_GALILEO_hits;++j){
			    
			    if(i != j){
				
				double GAL_chan_time_diff = RAW->get_GALILEO_Chan_T(i) - RAW->get_GALILEO_Chan_T(j);

				GAL_Chan_Time_Diff->Fill(GAL_chan_time_diff);
				
			    }
			    
			}

			
		    }
		    
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
	int i = 0;
	int id = 0;
	string line;
	char s_tmp[100];
	while(data.good()){
		getline(data,line,'\n');
		if(line[0] == '#') continue;
		sscanf(line.c_str(),"%s %d",s_tmp,&id);
		PrcID_Array[i] = id;
		i++;
	}
}


Int_t TSCNUnpackProc::get_Conversion(Int_t PrcID){

	for(int i = 0;i < 6;++i) if(PrcID == PrcID_Array[i]) return i;
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
	int i = 0;
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













