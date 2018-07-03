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

	hit_hist = MakeTH1('D',"hits","hits",37,0,36);
	am_hits = MakeTH1('D',"hits_a","hits_a",51,0,50);

	C_t = MakeTH1('D',"pl","pl",1001,0,1000);

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

}


//----------------------------------------------------------


TSCNUnpackProc::~TSCNUnpackProc()
{

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
	count++;
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
	

	
	while ((psubevt = inp_evt->NextSubEvent()) != 0) // subevent loop //
	{
		subevent_iter++;

		Int_t* pdata=psubevt->GetDataField();

		Int_t lwords = psubevt->GetIntLen();
	
		Int_t PrcID=psubevt->GetProcid();

		Int_t PrcID_Conv = get_Conversion(PrcID);


		WR_tmp[iterator] = WR->get_White_Rabbit(pdata);

		
		called[iterator] = PrcID_Conv;
		pdata += WR->get_increase();

		//cout << WR_tmp[iterator] << " " << iterator << endl;

		

		//continue;

		if(PrcID_Conv == 2 && false){
			cout << "---------------------\n";
			for(int i = 0;i < lwords;++i){
				cout << hex <<*(pdata + i) << " ";
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
			am_hits->Fill(am_FATIMA_hits);

			double sum = 0;
			double tmpE[2];
			for(int i = 0;i < am_FATIMA_hits;++i){
				//e,g, sum spectrum
				tmpE[i] = RAW->get_FATIMA_E(i);
				sum += tmpE[i];
				hit_hist->Fill(RAW->get_FATIMA_det_id(i));
			}
			if(am_FATIMA_hits == 2) FAT_MAT->Fill(tmpE[0],tmpE[1]);
			if(am_FATIMA_hits > 0 && sum > 0) FAT_E->Fill(sum);
		}

		if(PrcID_Conv == 2){
			//do something here

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
				WR_tmp[0] = WR_tmp[1];
				called[0] = called[1];
				iterator = 1;
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
			WR_tmp[0] = WR_tmp[1];
			called[0] = called[1];
			iterator = 1;
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
	cerr << "ProcID not known!" << endl;
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













