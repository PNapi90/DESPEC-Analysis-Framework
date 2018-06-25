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
#include "Data_Stream.h"
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
	
	/*for( int i=0; i<SCN_NUM_CHAN+1; i++ ) // Makes histogram for each channel  //
	{
		
		// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ //
		//                   FOR ONLINE ONLY                      //
		// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ //
		
		if (setup.get_online() == 1 && false)
		{
		
		
			QShort_Raw[i] = MakeTH1('D', Form("QShort_Raw/QShort_Raw%2d",i), Form("QDC Short Raw %2d",i+1), 15000, 1., 15001.);
			QLong_Raw[i]  = MakeTH1('D', Form("QLong_Raw/QLong_Raw%2d",i), Form("QDC Long Raw %2d",i+1), 15000, 1., 15001.);

			QDC_T[i]  = MakeTH1('D', Form("QDC_T/QDC_T%2d",i), Form("QDC Timestamp channel %2d",i+1), 2000, 1., 100000.);

			TDC_T[i]  = MakeTH1('D', Form("TDC_T/TDC_T%2d",i), Form("TDC Time channel %2d",i+1), 10000, 0., 100000.);

			TDC_dT_Trig[i]  = MakeTH1('D', Form("TDC_dT_Trig/TDC_dT_Trig%3d",i), Form("TDC dT for Trigger channel %2d",i+1), 4000, 50., 150.);
			TDC_dT_Misc[i]  = MakeTH1('D', Form("TDC_dT_Misc/TDC_dT_Misc%3d",i), Form("TDC dT for Miscellaneous channel %2d",i+1), 4000, 80., 180.);



			if(setup.get_time_calib() == 1)
			{

				for( int j=0; j<SCN_NUM_CHAN+1; j++ ) // Makes histogram for each channel  //
				{
					TDC_dT_All[i][j]  = MakeTH1('D', Form("TDC_dT_All/TDC_dT%3d/Time_dT_All%3d %3d",i,i,j), Form("TDC Diff Time channel %2d",j+1), 4000, -50., 50.);
				}
				
			}

			if(setup.get_time_calib() == 0)
			{
				 TDC_dT[i]  = MakeTH1('D', Form("TDC_dT/TDC_dT%3d",i), Form("TDC Diff Time channel %2d",i+1), 4000, -50., 50.);
			}

			TDC_dT_Vs_QDC_dT[i]  = MakeTH2('D', Form("TDC_dT_Vs_QDC_dT/TDC_dT_Vs_QDC_dT%2d",i), Form("QDC Diff Vs TDC Diff channels %2d", i+1), 400, -200., 200., 8000, -1000., 1000.);

			TDC_T_Vs_QDC_T[i]  = MakeTH2('D', Form("TDC_T_Vs_QDC_T/TDC_T_Vs_QDC_T%2d",i),Form("TDC Vs QDC Timestamp %2d",i+1), 320, -4000., 4000., 2000, -10000000., 10000000.);

		}
		// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ //
		// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ //
		// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ //
		
		
		QShortE[i] = MakeTH1('D', Form("QShortE/QShortE%2d",i), Form("QDC Short Calibrated %2d",i+1), 2000, 1., 2001.);

		QLongE[i]  = MakeTH1('D', Form("QLongE/QLongE%2d",i), Form("QDC Long Calibrated %2d",i+1), 2000, 1., 2001.);

		QDC_dT[i]  = MakeTH1('D', Form("QDC_dT/QDC_dT%2d",i), Form("QDC Timestamp Difference channel %2d",i+1), 400, -200., 200.);

		QLongE_Vs_TDC_dT[i]  = MakeTH2('D', Form("QLongE_Vs_TDC_dT/QLongE_Vs_TDC_dT channels%2d",i),Form("QLong Vs TDC Timestamp Difference %2d",i+1), 20, 0., 4000., 4000, -50., 50.);
		QLongE_Vs_QDC_dT[i]  = MakeTH2('D', Form("QLongE_Vs_QDC_dT/QLongE_Vs_QDC_dT channels%2d",i),Form("QLong Vs QDC Timestamp Difference %2d",i+1), 20, 0., 4000., 400, -200., 200.);

	}
	*/
	
	
	FAT_E = MakeTH1('D',"FATIMA_E","FATIMA_E",2001,0,4000);
	FAT_MAT = MakeTH2('D',"FAT_MAT","FAT_MAT",1001,0,4000,1001,0,4000);

	hit_mat = MakeTH2('D',"hitmat","hitmat",37,0,36,37,0,36);

	//used_systems
	get_used_Systems();

	//create White Rabbit obj
	WR = new White_Rabbit();


	//create Detector Systems
	Detector_Systems = new Detector_System*[6];

	// all non used systems intialized as NULL 
	//-> calling uninitialized system will cause an error !
	//Detector_Systems[0] = !Used_Systems[0] ? NULL : new FRS_Detector_System();
	//Detector_Systems[1] = !Used_Systems[1] ? NULL : new AIDA_Detector_System();
	Detector_Systems[2] = !Used_Systems[2] ? NULL : new PLASTIC_Detector_System();
	Detector_Systems[3] = !Used_Systems[3] ? NULL : new FATIMA_Detector_System();
	//Detector_Systems[4] = !Used_Systems[4] ? NULL : new GALILEO_Detector_System();

	for(int i = 0;i < 6;++i) if(!Used_Systems[i]) Detector_Systems[i] = NULL;

	//Create data streams
	data_stream = new Data_Stream*[6];

	// all non used data streams intialized as NULL 
	//-> calling uninitialized system will cause an error !
	//Detector_Systems[0] = !Used_Systems[0] ? NULL : new FRS_Detector_System();
	//Detector_Systems[1] = !Used_Systems[1] ? NULL : new AIDA_Detector_System();
	data_stream[2] = !Used_Systems[2] ? NULL : new PLASTIC_Data_Stream();
	data_stream[3] = !Used_Systems[3] ? NULL : new FATIMA_Data_Stream();
	//Detector_Systems[4] = !Used_Systems[4] ? NULL : new GALILEO_Detector_System();

	for(int i = 0;i < 6;++i) if(!Used_Systems[i]) data_stream[i] = NULL;

	load_PrcID_File();
	White_Rabbbit_old = 0;
	count = 0;
}


//----------------------------------------------------------


TSCNUnpackProc::~TSCNUnpackProc()
{

	for(int i = 0;i < 6;++i){
		if(Detector_Systems[i]) delete Detector_Systems[i];
		if(data_stream[i]) delete data_stream[i];
	}
	delete[] Detector_Systems;
	
	delete[] data_stream;

	cout << "**** TSCNUnpackProc: Delete" << endl;
}

//----------------------------------------------------------



Bool_t TSCNUnpackProc::BuildEvent(TGo4EventElement* dest)
{
	count++;
	if(count > 500000) return kTRUE;
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


		ULong64_t White_Rabbit = WR->get_White_Rabbit(pdata);
		pdata += WR->get_increase();

		if(PrcID_Conv == 2){
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
		Detector_Systems[PrcID_Conv]->get_Event_data(data_stream[PrcID_Conv]);
		
		//temporary
		used[PrcID_Conv] = true;

		if(PrcID_Conv == 3){
			//if FATIMA mismatch -> don't use data
			if(data_stream[PrcID_Conv]->get_mismatch()) continue;
			
			fat_hits = data_stream[PrcID_Conv]->get_FATIMA_Hits();
			E0 = data_stream[PrcID_Conv]->get_FATIMA_E(0);
			FAT_E->Fill(E0);
			if(fat_hits == 2){
				E1 = data_stream[PrcID_Conv]->get_FATIMA_E(1);
				FAT_MAT->Fill(E0,E1);

			}
		}

		if(used[2] && used[3]){

		}
	}

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

















