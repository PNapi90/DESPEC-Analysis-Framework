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
#include "FRS_Detector_System.h"

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
	FAT_make_raw_histograms = true;
	
	cout << "**** TSCNUnpackProc: Create" << endl;
	
	input_data_path_old = "old";
	
	Make_FRS_Histos();
	
	Make_FATIMA_Histos();

	Make_Plastic_Histos();
	
	Make_GALILEO_Histos();
	
	WR_used = false;

	//used_systems
	get_used_Systems();
	get_WR_Config();

	//create White Rabbit obj
	WR = WR_used ? new White_Rabbit() : NULL;

	WR_HIST = MakeTH1('D',"WR","WR",2001,-1,40);
	WR_HIST2 = MakeTH1('D',"WR2","WR2",2001,-10,4000);
	WR_F = MakeTH1('D',"WRf","WRf",2001,-10,4000);
	
	//create Detector Systems
	Detector_Systems = new Detector_System*[6];

	// all non used systems intialized as NULL 
	//-> calling uninitialized system will cause an error !
	Detector_Systems[0] = !Used_Systems[0] ? NULL : new FRS_Detector_System();
	Detector_Systems[1] = !Used_Systems[1] ? NULL : new AIDA_Detector_System();
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
		
	read_setup_parameters();
	
	FAT_det_pos_setup();
	
	White_Rabbbit_old = 0;
	count = 0;
	iterator = 0;
	cals_done = false;
	val_it = 0;
	
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
		
		data_file_number++;
	    
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
		
	if (count % 100000 == 0){
	    cout << "Event " << count << " Reached!!!"<<"    Data File Number : "<<data_file_number;
	    cout << "\r";
	}
	
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
	
	//bool WHITE_RABBIT_USED = true;
	
	while ((psubevt = inp_evt->NextSubEvent()) != 0) // subevent loop //
	{
		subevent_iter++;

		Int_t* pdata=psubevt->GetDataField();

		Int_t lwords = psubevt->GetIntLen();
	
		Int_t PrcID=psubevt->GetProcid();
		
		//cout<<"Proc_ID is : "<<PrcID<<endl;

		Int_t PrcID_Conv = get_Conversion(PrcID);
		
		Int_t sub_evt_length  = (psubevt->GetDlen() - 2) / 2;
		
				
		if(PrcID_Conv == 0){
			
		    Detector_Systems[PrcID_Conv]->Process_FRS(psubevt);
		    
		    Detector_Systems[PrcID_Conv]->get_Event_data(RAW);
		    		    
		    Fill_FRS_Histos();
		    
		    continue;
		    
		}
			    
		    
		if(WHITE_RABBIT_USED){
			sub_evt_length = sub_evt_length - 5;
			WR_tmp[iterator] = WR->get_White_Rabbit(pdata);
			pdata += WR->get_increase();
						
		}
		called[iterator] = PrcID_Conv;
		

		//cout << WR_tmp[iterator] << " " << iterator << endl;

		if(PrcID_Conv == 1 && sub_evt_length != 0){
		    
		    Detector_Systems[PrcID_Conv]->Process_AIDA(psubevt);
		    
		}
		
		

		//continue;

		if(PrcID_Conv == 1 && false){
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
		
		
		//PLASTIC CASE
		if(PrcID_Conv == 2) Fill_Plastic_Histos();
		
		//FATIMA CASE
		if(PrcID_Conv == 3) Fill_FATIMA_Histos();
		
		// GALILEO CASE //
		if(PrcID_Conv == 4) Fill_GALILEO_Histos();
		
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

void TSCNUnpackProc::read_setup_parameters(){

    const char* format = "%s %d";

    ifstream file("Configuration_Files/Detector_System_Setup_File.txt");

    if(file.fail()){
        cerr << "Could not find File for setup parameters!" << endl;
        exit(0);
    }

    string line;
    string var_name;
    int dummy_var;
    //file.ignore(256,'GENERAL_CONFIGURATION');


    file.ignore(256,':');
    file >> WHITE_RABBIT_USED;//dummy_var;

    file.ignore(256,':');
    file >> FAT_gain_match_used;//dummy_var;

    file.ignore(256,':');
    file >> FAT_exclusion_dist;//dummy_var;


    cout<<endl;
    cout<<endl;
    cout<<"////////////////////////////////////"<<endl;
    cout<<"Setup Parameters List: "<<endl;
    if(WHITE_RABBIT_USED) cout<<"White Rabbit: Enabled"<<endl;
    else if(!WHITE_RABBIT_USED) cout<<"White Rabbit: Disabled"<<endl;
    if(FAT_gain_match_used) cout<<"FATIMA Gain Matching: Enabled"<<endl;
    else if(!FAT_gain_match_used) cout<<"FATIMA Gain Matching: Disabled"<<endl;
    if(FAT_exclusion_dist > 0) cout<<"FATIMA Detectors Excluded if Linear Difference Exceeds "<<FAT_exclusion_dist<<" mm"<<endl;
    else if(FAT_exclusion_dist == 0) cout<<"'Nearest Neighbour Exclusion': Disabled (Distance set to 0)"<<endl;
    cout<<"////////////////////////////////////"<<endl;
    cout<<endl;
    cout<<endl;
    /*while(file.good()){
        getline(file,line,'\n');
        if(line[0] == '#') continue;
        sscanf(line.c_str(),format,&var_name,&dummy_var);
	
	    cout<<"Hello Again?"<<endl;

		if (var_name == "White_Rabbit_Enabled:" && dummy_var == 1)  WHITE_RABBIT_USED = true;
		else if (var_name == "White_Rabbit_Enabled:" && dummy_var == 0)  WHITE_RABBIT_USED = false;
		
		if (var_name == "FATIMA_Gain_Match_Enabled:" && dummy_var == 1)  FAT_gain_match_used = true;
		else if (var_name == "FATIMA_Gain_Match_Enabled:" && dummy_var == 0) FAT_gain_match_used  = false;
		
    }*/
        
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
			if(tmp_values[1] == -1){
				cerr << endl;
				cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
				cerr << "SINGLE COINCIDENCE DETECTED! => EDIT Coincidences_of_Interest.txt FILE!" << endl;
				cerr << "-> EXITING PROGRAM" << endl;
				cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
				cerr << endl;
				exit(0);
			}
			
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

void TSCNUnpackProc::FAT_det_pos_setup(){
    
    bool output_position_matrix = false;

    FAT_positions 	= new double*[36];
    FAT_neighbour_check = new bool*[36];

    for(int i = 0; i < 36; ++i){
	FAT_positions[i] = new double[3];
	FAT_neighbour_check[i] = new bool[36];
	for (int j = 0; j < 3; ++j) FAT_positions[i][j] = -1;
	for (int k = 0; k < 36; ++k) FAT_neighbour_check[i][k] = false;
    }



    const char* format = "%d %lf %lf %lf";

    ifstream file("Configuration_Files/FATIMA_Detector_Positions.txt");

    if(file.fail()){
        cerr << "Could not find FATIMA Detector Positions File!" << endl;
        exit(0);
    }

    string line;
    int pos_num;
    double r, theta, phi;

    while(file.good()){
        getline(file,line,'\n');
        if(line[0] == '#') continue;
        sscanf(line.c_str(),format, &pos_num, &r, &theta, &phi);
		
		FAT_positions[pos_num][0] = r;
		FAT_positions[pos_num][1] = theta;
		FAT_positions[pos_num][2] = phi;

	}
    
    
    if (output_position_matrix) cout<<"        "<<"0 "<<"1 "<<"2 "<<"3 "<<"4 "<<"5 "<<"6 "<<"7 "<<"8 "
		    <<"9 "<<"10 "<<"11 "<<"12 "<<"13 "<<"14 "<<"15 "<<"16 "<<"17 "
		    <<"18 "<<"19 "<<"20 "<<"21 "<<"22 "<<"23 "<<"24 "<<"25 "<<"26 "
		    <<"27 "<<"28 "<<"29 "<<"30 "<<"31 "<<"32 "<<"33 "<<"34 "<<"35 "<<endl;
    
    for(int i = 0; i < 36; ++i){
	
	if (i >= 10 && output_position_matrix) cout<<"Det "<<i<<": ";
	if (i < 10  && output_position_matrix) cout<<"Det "<<i<<" : ";

	for (int k = 0; k < 36; ++k){
	    
	    if(k > 9 && output_position_matrix) cout<<" ";
	    
	    double dist = distance_between_detectors( FAT_positions[i][0],  FAT_positions[i][1],  FAT_positions[i][2],
						      FAT_positions[k][0],  FAT_positions[k][1],  FAT_positions[k][2]);
	    
	    if(dist > FAT_exclusion_dist && i != k){
		
		
		 FAT_neighbour_check[i][k] = true;
		 
		 if (output_position_matrix) cout<<"0 ";
	    
	    }
	    else if(output_position_matrix) cout<<"X ";
	    
	}
	
	if (output_position_matrix) cout<<endl;
	
    }
    
    
    
    
}
double TSCNUnpackProc::distance_between_detectors(double _r, double _theta, double _phi, double r_, double theta_, double phi_){

    _theta = _theta * M_PI/180.0; 
    theta_ = theta_ * M_PI/180.0; 

    _phi = _phi * M_PI/180.0; 
    phi_ = phi_ * M_PI/180.0; 

    double dist = sqrt(_r*_r + r_*r_ - 2.0*_r*r_*(sin(_theta)*sin(theta_)*cos(_phi - phi_) + cos(_theta)*cos(theta_)));
    
    return dist;


}

// ################################################################## //
// ################################################################## //
// ################# Histogram Filling Section ###################### //
// ################################################################## //
// ################################################################## //


void TSCNUnpackProc::Make_FRS_Histos(){
    
    hsci_tofll2 = MakeTH1('D',"FRS/hsci_tofll2","hsci_tofll2",1500,0.,62000.);
    hsci_tofll3 = MakeTH1('D',"FRS/hsci_tofll3","hsci_tofll3",1500,0.,62000.);
    hsci_tof2 = MakeTH1('D',"FRS/hsci_tof2","hsci_tof2",1000,0.,62000.);
    hsci_tofrr2 = MakeTH1('D',"FRS/hsci_tofrr2","hsci_tofrr2",1500,0.,62000.);
    hsci_tofrr3 = MakeTH1('D',"FRS/hsci_tofrr3","hsci_tofrr3",1500,0.,62000.);
    hsci_tof3 = MakeTH1('D',"FRS/hsci_tof3","hsci_tof3",1000,0.,62000.);
    
    hsci_dt_21l_21r = MakeTH1('D',"FRS/hsci_dt_21l_21r","hsci_dt_21l_21r",5001,0,5000);
    hsci_dt_41l_41r = MakeTH1('D',"FRS/hsci_dt_41l_41r","hsci_dt_41l_41r",5001,0,5000);
    hsci_dt_42l_42r = MakeTH1('D',"FRS/hsci_dt_42l_42r","hsci_dt_42l_42r",5001,0,5000);
    hsci_dt_43l_43r = MakeTH1('D',"FRS/hsci_dt_43l_43r","hsci_dt_42l_43r",5001,0,5000);
    hsci_dt_81l_81r = MakeTH1('D',"FRS/hsci_dt_81l_81r","hsci_dt_81l_81r",5001,0,5000);
    
    
    hsci_dt_21l_41l = MakeTH1('D',"FRS/hsci_dt_21l_41l","hsci_dt_21l_41l",5001,0,5000);
    hsci_dt_21r_41r = MakeTH1('D',"FRS/hsci_dt_21r_41r","hsci_dt_21r_41r",5001,0,5000);
    
    hsci_dt_21l_42l = MakeTH1('D',"FRS/hsci_dt_21l_42l","hsci_dt_21l_42l",5001,0,5000);
    hsci_dt_21r_42r = MakeTH1('D',"FRS/hsci_dt_21r_42r","hsci_dt_21r_42r",5001,0,5000);

    hsci_dt_21l_81l = MakeTH1('D',"FRS/hsci_dt_21l_81l","hsci_dt_21l_81l",5001,0,5000);
    hsci_dt_21r_81r = MakeTH1('D',"FRS/hsci_dt_21r_81r","hsci_dt_21r_81r",5001,0,5000);

    /*hID_x2 = MakeTH1('D',"ID_x2","ID_x2",3000,0,5000);
    hID_y2 = MakeTH1('D',"ID_y2","ID_y2",3000,0,5000);
    hID_a2 = MakeTH1('D',"ID_a2","ID_a2",3000,0,5000);
    hID_b2 = MakeTH1('D',"ID_b2","ID_b2",3000,0,5000);
    
    hID_x4 = MakeTH1('D',"ID_x4","ID_x4",3000,0,5000);
    hID_y4 = MakeTH1('D',"ID_y4","ID_y4",3000,0,5000);
    hID_a4 = MakeTH1('D',"ID_a4","ID_a4",3000,0,5000);
    hID_b4 = MakeTH1('D',"ID_b4","ID_b4",3000,0,5000);*/
    
    hbeta = MakeTH1('D',"FRS/beta","beta",100,0.,1.);
    hbeta3 = MakeTH1('D',"FRS/beta3","beta3",100,0.,1.);
    hgamma = MakeTH1('D',"FRS/gamma","gamma",100,0.,1.);
    
    hAoQ = MakeTH1('D',"FRS/AoQ","AoQ",200,1.4,3.0); // 200,1.4,3.0
    hAoQ_corr = MakeTH1('D',"FRS/AoQ_corr","AoQ_corr",200,1.4,3.0); // 200,1.4,3.0
    
    hz = MakeTH1('D',"FRS/z","z",100,0.,93.);
    hz2 = MakeTH1('D',"FRS/z2","z2",100,0.,93.);
    hz3 = MakeTH1('D',"FRS/z3","z3",100,10.,93.);
    
    htimestamp = MakeTH1('D',"FRS/timestamp","timestamp",30,0.,300.);
    hts = MakeTH1('D',"FRS/ts","ts",30,0.,300.);
    hts2 = MakeTH1('D',"FRS/ts2","ts2",30,0.,300.);
	
}
void TSCNUnpackProc::Fill_FRS_Histos(){
    
    // Float_t dE, dE_cor;

    // Float_t sci_l, sci_r, sci_e, sci_tx, sci_x;

    Float_t sci_tofll2, sci_tofll3, sci_tof2, sci_tofrr2, sci_tofrr3, sci_tof3;

    Float_t ID_x2, ID_y2, ID_a2, ID_b2;
    
    Float_t ID_x4, ID_y4, ID_a4, ID_b4;

    
    Int_t sci_dt_21l_21r, sci_dt_41l_41r, sci_dt_42l_42r, sci_dt_43l_43r, sci_dt_81l_81r;
    
    Int_t sci_dt_21l_41l, sci_dt_21r_41r, sci_dt_21l_42l, sci_dt_21r_42r, sci_dt_21l_81l, sci_dt_21r_81r;
    
    // Float_t ID_brho, ID_rho;
    
    Float_t beta, beta3, gamma;
    
    Float_t AoQ, AoQ_corr;
    
    Float_t z, z2, z3;
    
    Float_t timestamp, ts, ts2;
    
    
    /*dE = RAW->get_FRS_dE(int i);
    dE_cor = RAW->get_FRS_dE_corr(int i);

    sci_l = RAW->get_FRS_sci_l(int i);
    sci_r = RAW->get_FRS_sci_r(int i);
    sci_e = RAW->get_FRS_sci_e(int i);
    sci_tx = RAW->get_FRS_sci_tx(int i);
    sci_x = RAW->get_FRS_sci_x(int i);*/
    
    sci_tofll2 = RAW->get_FRS_tofll2();
    sci_tofll3 = RAW->get_FRS_tofll3();
    sci_tof2 = RAW->get_FRS_tof2();
    sci_tofrr2 = RAW->get_FRS_tofrr2();
    sci_tofrr3 = RAW->get_FRS_tofrr3();
    sci_tof3 = RAW->get_FRS_tof3();

    ID_x2 = RAW->get_FRS_x2();
    ID_y2 = RAW->get_FRS_y2();
    ID_a2 = RAW->get_FRS_a2();
    ID_b2 = RAW->get_FRS_b2();
    
    ID_x4 = RAW->get_FRS_x4();
    ID_y4 = RAW->get_FRS_y4();
    ID_a4 = RAW->get_FRS_a4();
    ID_b4 = RAW->get_FRS_b4();
    
    sci_dt_21l_21r = RAW->get_FRS_dt_21l_21r();
    sci_dt_41l_41r = RAW->get_FRS_dt_41l_41r();
    sci_dt_42l_42r = RAW->get_FRS_dt_42l_42r();
    sci_dt_43l_43r = RAW->get_FRS_dt_43l_43r();
    sci_dt_81l_81r = RAW->get_FRS_dt_81l_81r();
    
    sci_dt_21l_41l = RAW->get_FRS_dt_21l_41l();
    sci_dt_21r_41r = RAW->get_FRS_dt_21r_41r();
    
    sci_dt_21l_42l = RAW->get_FRS_dt_21l_42l();
    sci_dt_21r_42r = RAW->get_FRS_dt_21r_42r();

    sci_dt_21l_81l = RAW->get_FRS_dt_21l_81l();
    sci_dt_21r_81r = RAW->get_FRS_dt_21r_81r();
    
    /*ID_brho = RAW->get_FRS_brho(int i);
    ID_rho = RAW->get_FRS_rho(int i);*/
    
    beta = RAW->get_FRS_beta();
    beta3 = RAW->get_FRS_beta3();
    gamma = RAW->get_FRS_gamma();
    
    AoQ = RAW->get_FRS_AoQ();
    AoQ_corr = RAW->get_FRS_AoQ_corr();
    
    z = RAW->get_FRS_z();
    z2 = RAW->get_FRS_z2();
    z3 = RAW->get_FRS_z3();
    
    timestamp = RAW->get_FRS_timestamp();
    ts = RAW->get_FRS_ts();
    ts2 = RAW->get_FRS_ts2();		    
    
    // ---------------------------------------- //
	    
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
    
    if(sci_dt_21l_21r) hsci_dt_21l_21r->Fill(sci_dt_21l_21r);
    if(sci_dt_41l_41r) hsci_dt_41l_41r->Fill(sci_dt_41l_41r);
    if(sci_dt_42l_42r) hsci_dt_42l_42r->Fill(sci_dt_42l_42r);
    if(sci_dt_43l_43r) hsci_dt_43l_43r->Fill(sci_dt_43l_43r);
    if(sci_dt_81l_81r) hsci_dt_81l_81r->Fill(sci_dt_81l_81r);

    if(sci_dt_21l_41l) hsci_dt_21l_41l->Fill(sci_dt_21l_41l);
    if(sci_dt_21r_41r) hsci_dt_21r_41r->Fill(sci_dt_21r_41r);
    
    if(sci_dt_21l_42l) hsci_dt_21l_42l->Fill(sci_dt_21l_42l);
    if(sci_dt_21r_42r) hsci_dt_21r_42r->Fill(sci_dt_21r_42r);

    if(sci_dt_21l_81l) hsci_dt_21l_81l->Fill(sci_dt_21l_81l);
    if(sci_dt_21r_81r) hsci_dt_21r_81r->Fill(sci_dt_21r_81r);
    
    
    if(beta) hbeta->Fill(beta);
    if(beta3) hbeta3->Fill(beta3);
    if(gamma) hgamma->Fill(gamma);
		    
    if(AoQ) hAoQ->Fill(AoQ);
    if(AoQ_corr) hAoQ_corr->Fill(AoQ_corr);
    
    if(z) hz->Fill(z);
    if(z2) hz2->Fill(z2);
    if(z3) hz3->Fill(z3);
    
    if(timestamp) htimestamp->Fill(timestamp);
    if(ts) hts->Fill(ts);
    if(ts2) hts2->Fill(ts2);

}

void TSCNUnpackProc::Make_Plastic_Histos(){
    
    
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
}

void TSCNUnpackProc::Fill_Plastic_Histos(){
    
    int pl_iter = 0;
    int sum_l = 0;
    int sum_t = 0;

    int phys_ch = 0;
    int phys_ch_tmp = 0;
    int sum_phys_l[17];
    int sum_phys_t[17];
    int called_channels[17];
    for(int i = 0; i < 17; ++i){
	
	    sum_phys_t[i] = 0;
	    sum_phys_l[i] = 0;
	    called_channels[i] = 0;
    }


    for(int i = 0; i < 4; ++i){
    
	sum_l = 0;
	sum_t = 0;
	pl_iter = RAW->get_PLASTIC_am_Fired(i);
	
	if(pl_iter > 200) pl_iter = 0;
	
	for(int j = 0; j < pl_iter; ++j){

	    //cout << "i " << i << " j " << j << " ";// << endl; 
	    phys_ch = RAW->get_PLASTIC_physical_channel(i,j);
	    //cout << "-> phys_ch " << phys_ch << endl;
	    called_channels[j] = phys_ch;

	    sum_phys_l[phys_ch] += RAW->get_PLASTIC_physical_lead_hits(i,phys_ch);
	    sum_phys_t[phys_ch] += RAW->get_PLASTIC_physical_trail_hits(i,phys_ch);
	    

	    sum_l += RAW->get_PLASTIC_lead_hits(i);
	    sum_t += RAW->get_PLASTIC_trail_hits(i);
	    
	    
	    if(!Trail_LEAD[i][phys_ch]) Trail_LEAD[i][phys_ch] = MakeTH1('D',Form("trail_minus_lead/trail_minus_lead_board%d_ch%d",i,phys_ch),
								    Form("trail_minus_lead_board%d_ch%d",i,phys_ch),500,0,500);
	    
	    if(j % 2 == 0){
		
		    Trail_LEAD[i][phys_ch]->Fill(RAW->get_PLASTIC_trail_T(i,j+1)-RAW->get_PLASTIC_lead_T(i,j));
		    
		    for(int k = 0;k < pl_iter;++k){
			
			    phys_ch_tmp = RAW->get_PLASTIC_physical_channel(i,k);
			    
			    if(k % 2 == 0 && k != j){
				
				    //cout << "i " << i << " " << j << " " << k << " " << phys_ch << " " << phys_ch_tmp << " " << pl_iter<< endl;
				    if(!LEAD_LEAD[i][phys_ch][phys_ch_tmp]) LEAD_LEAD[i][phys_ch][phys_ch_tmp] = MakeTH1('D',Form("lead_minus_lead/lead_minus_lead_board_%d_from_ch%d_to_%d",i,phys_ch,phys_ch_tmp),
														    Form("lead_minus_lead_board%d_from_ch%d_to_%d",i,phys_ch,phys_ch_tmp),500,-1000,1000);
				    LEAD_LEAD[i][phys_ch][phys_ch_tmp]->Fill(RAW->get_PLASTIC_lead_T(i,j) - RAW->get_PLASTIC_lead_T(i,k));
			    }
		    }
	    }

	    
	    if(!Coarse[i][phys_ch]) Coarse[i][phys_ch] = MakeTH1('D',Form("coarse_%d_%d",i,phys_ch),
							    Form("coarse_%d_%d",i,phys_ch),500,0,5000);

	    if(j % 2 == 0) Coarse[i][phys_ch]->Fill(RAW->get_PLASTIC_coarse_lead(i,j));


	}
	for(int j = 0;j < pl_iter;++j){

	    if(sum_phys_l[called_channels[j]] > 0){

		    if(!tamex_Mult_Ch_lead[i][called_channels[j]]) tamex_Mult_Ch_lead[i][called_channels[j]] = MakeTH1('D',Form("tamex_channels_hists/tamex_lead_ch_%d_%d",i,called_channels[j]),
														Form("tamex_lead_ch_%d_%d",i,called_channels[j]),30,0,30);
		    if(!tamex_mult_mat_lead[i]) tamex_mult_mat_lead[i] = MakeTH2('D',Form("tamex_mat_lead_%d",i),
									    Form("tamex_mat_lead_%d",i),20,0,20,30,0,30);

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
	    if(!tamex_Mult_lead[i]) tamex_Mult_lead[i] = MakeTH1('D',Form("tamex_lead_%d",i),
							    Form("tamex_lead_%d",i),100,0,100);		
	    tamex_Mult_lead[i]->Fill(sum_l);
	}
	if(sum_t > 0){
	    if(!tamex_Mult_trail[i]) tamex_Mult_trail[i] = MakeTH1('D',Form("tamex_trail_%d",i),
							    Form("tamex_trail_%d",i),100,0,100);
	    tamex_Mult_trail[i]->Fill(sum_t);
	}
    }
}


void TSCNUnpackProc::Make_FATIMA_Histos(){
    
    FAT_REF_DET = 0;

    //*****************
    //Fatima histograms
    FATgate1_low  = 1172.;
    FATgate1_high = 1182.;
    FATgate2_low  = 1328.;
    FATgate2_high = 1338.;
    E_gate1 = FATgate1_low + (FATgate1_high - FATgate1_low)/2.;
    E_gate2 = FATgate2_low + (FATgate2_high - FATgate2_low)/2.;
    
    
    FAT_Esum  		= MakeTH1('D', "FATIMA/ESum", "LaBr Energy (all detectors)",4001,0,4000);
    FAT_gg    		= MakeTH2('D', "FATIMA/gg", "FATIMA Gamma-Gamma (all detectors)",2001,0,2000, 2001,0,2000);
    FAT_TDCdtsum 		= MakeTH1('D', "FATIMA/TDCdtSum", "TDC dt (all detectors)", 3201,-40,40);
    FAT_QDCdtsum 		= MakeTH1('D', "FATIMA/QDCdtSum", "QDC dt (all detectors)", 3201,-40,40);
    FAT_TDCdtsum_ref_gated 	= MakeTH1('D', "FATIMA/TDCdt_ref_gated",
				Form("TDC dt gates on %5.2f keV and %5.2f keV (all detectors)", E_gate1, E_gate2), 3201,-40,40);
    FAT_QDCdtsum_ref_gated 	= MakeTH1('D', "FATIMA/QDCdt_ref_gated",
				Form("QDC dt gates on %5.2f keV and %5.2f keV (all detectors)", E_gate1, E_gate2), 3201,-40,40);				
    
    

    //statistics
    FAT_hits 		= MakeTH1('D', "FATIMA/Stat/det_hits", "FATIMA detector statistics",40,0,40);
    FAT_hits_QDC 		= MakeTH1('D', "FATIMA/Stat/QDC_hits", "FATIMA QDC statistics",40,0,40);
    FAT_hits_TDC 		= MakeTH1('D', "FATIMA/Stat/TDC_hits", "FATIMA TDC statistics",40,0,40);
    FAT_QDC_TDC_hitmap 	= MakeTH2('D', "FATIMA/Stat/QDC_TDC_hitmap", "FATIMA QDC-TDC hit map",40,0,40, 40,0,40);
    FAT_correlations 	= MakeTH2('D', "FATIMA/Stat/det_det_correlations", "FATIMA det-det correlations",40,0,40, 40,0,40);
    
    /*** FATIMA Histogram Arrays ***/

    //energy
    FAT_E = new TH1*[FAT_MAX_DET];
    FAT_Eraw = new TH1*[FAT_MAX_DET];
    FAT_E_ratio = new TH2*[FAT_MAX_DET];
    FAT_gg_ref = new TH2*[FAT_MAX_DET];
    
    //timing
    FAT_TDCdt_ref = new TH1*[FAT_MAX_DET];
    FAT_QDCdt_ref = new TH1*[FAT_MAX_DET];
    FAT_TDC_QDC_dt = new TH2*[FAT_MAX_DET];
    FAT_TDCdt_ref_gated = new TH1*[FAT_MAX_DET];
    FAT_E_TDCdt_ref_gated = new TH2*[FAT_MAX_DET];
    

    for (int det = 0;  det< FAT_MAX_DET; det++) {

	    FAT_E[det] = nullptr;
	    FAT_Eraw[det] = nullptr;
	    FAT_E_ratio[det] = nullptr;
	    FAT_gg_ref[det] = nullptr;
	    FAT_TDCdt_ref[det] = nullptr;
	    FAT_QDCdt_ref[det] = nullptr;
	    FAT_TDC_QDC_dt[det] = nullptr;
	    FAT_TDCdt_ref_gated[det] = nullptr;
	    FAT_E_TDCdt_ref_gated[det] = nullptr;
	    
    }

}
void TSCNUnpackProc::Fill_FATIMA_Histos(){
    
    
    int deti, detj;
    double dt1, dt2;
    double En_i, En_j;
    double FAT_ratio_i;
    
    
    for (int i=0; i<RAW->get_FAT_QDCs_fired(); i++){ /** Loops over only channels in the QDC **/

	deti = RAW->get_FAT_QDC_id(i);
	En_i = RAW->get_FAT_QLong_Raw(i);

	
	if(!FAT_Eraw[deti]) FAT_Eraw[deti] = MakeTH1('D', Form("FATIMA/Energy/E_Raw_LaBr%02d", deti),
						Form("LaBr%02d energy (raw)", deti),2000,0,40000);
    
	FAT_Eraw[deti]->Fill(En_i);
	
    }
    
    int dets_fired = RAW->get_FAT_det_fired();
    
    for (int i=0; i<dets_fired; i++) { /** Loops over only channels in both QDC and TDC **/
    
	deti 	    = RAW->get_FAT_id(i);
	En_i 	    = RAW->get_FAT_E(i);
	FAT_ratio_i = RAW->get_FAT_ratio(i);
    
	FAT_hits->Fill(deti);
	FAT_Esum->Fill(En_i);
	
	if(!FAT_E[deti]) FAT_E[deti] = MakeTH1('D', Form("FATIMA/Energy/E_LaBr%02d", deti),
					Form("LaBr%02d energy", deti),4001,0,4000);
	
	if(!FAT_E_ratio[deti]) FAT_E_ratio[deti] = MakeTH2('D', Form("FATIMA/Energy/EvsRatio_LaBr%02d", deti),
						    Form("LaBr%02d energy vs QShort/QLong", deti),4001,0,4000, 200,0,1);
	
	FAT_ratio_i = RAW->get_FAT_ratio(i);
	
	FAT_hits->Fill(deti);
	FAT_Esum->Fill(En_i);
	FAT_E[deti]->Fill(En_i);
	FAT_E_ratio[deti]->Fill(En_i, FAT_ratio_i);
    
	for (int j=0; j<dets_fired; j++) { /** Loops over only channels in both QDC and TDC **/

	    detj = RAW->get_FAT_id(j);
	    
	    if (FAT_neighbour_check[deti][detj]){ /** Excludes nearest neighbour **/
	    
		if (deti < detj) { /** Avoids the same detector pairing being counted twice **/
		
		    En_j = RAW->get_FAT_E(j);
		    
		    dt1 = RAW->get_FAT_t(i) - RAW->get_FAT_t(j);
		    dt2 = RAW->get_FAT_t_qdc(i) - RAW->get_FAT_t_qdc(j);
		    
		    FAT_gg->Fill(En_i, En_j);
		    
		    FAT_TDCdtsum->Fill(dt1);
		    FAT_QDCdtsum->Fill(dt2);
		    
		    FAT_TDCdtsum_ref_gated->Fill(dt1);
		    FAT_QDCdtsum_ref_gated->Fill(dt2);
		
		}
   
		if (deti == FAT_REF_DET) { /** Only occurs for the Reference Detector **/
    
		    if(!FAT_gg_ref[detj]) FAT_gg_ref[detj] =  MakeTH2('D', Form("FATIMA/Energy/gg_LaBr%02d_LaBr%02d", FAT_REF_DET, detj),
								Form("Gamma-Gamma coincidences LaBr%02d-LaBr%02d", FAT_REF_DET, detj),2000,0,2000, 2000,0,2000);
		    
		    if(!FAT_TDCdt_ref[detj]) FAT_TDCdt_ref[detj] = MakeTH1('D', Form("FATIMA/Timing/TDCdt_LaBr%02d_LaBr%02d", FAT_REF_DET, detj),
								    Form("TDC dt LaBr%02d LaBr%02d", FAT_REF_DET, detj),3201,-40,40);	
		    
		    if(!FAT_QDCdt_ref[detj]) FAT_QDCdt_ref[detj] = MakeTH1('D', Form("FATIMA/Timing/QDCdt_LaBr%02d_LaBr%02d", FAT_REF_DET, detj),
								    Form("QDC dt LaBr%02d LaBr%02d", FAT_REF_DET, detj),3201,-40,40);
		    
		    if(!FAT_TDC_QDC_dt[detj]) FAT_TDC_QDC_dt[detj] =  MakeTH2('D', Form("FATIMA/Timing/TDCdt_QDCdt_LaBr%02d", detj),
									Form("TDCdt vs QDCdt LaBr%02d", detj),3201,-40,40, 3201,-40,40);
		    
		    FAT_gg_ref[detj]->Fill(En_i, En_j);
		    FAT_TDCdt_ref[detj]->Fill(dt1);
		    FAT_QDCdt_ref[detj]->Fill(dt2);
		    FAT_TDC_QDC_dt[detj]->Fill(dt1, dt2);

    
    
		    if (RAW->get_FAT_E(i) > FATgate1_low && RAW->get_FAT_E(i) < FATgate1_high) { /** Only if the energies are withing the energy gate **/
		    
			if(!FAT_E_TDCdt_ref_gated[detj]) FAT_E_TDCdt_ref_gated[detj] = MakeTH2('D', Form("FATIMA/Timing/Gated/TDCdt_gated_LaBr%02d_E_LaBr%02d", FAT_REF_DET, detj),
											Form("TDC dt LaBr%02d (on %4.2f keV) - LaBr%02d (E)",FAT_REF_DET, E_gate1, detj), 2001, 0, 2000, 3201,-40,40);
		    
			FAT_E_TDCdt_ref_gated[detj]->Fill(En_j, dt1);
    
    
			if (RAW->get_FAT_E(j) > FATgate2_low && RAW->get_FAT_E(j) < FATgate2_high) { /** Only if the energies of the secodn detector are withing the energy gate **/
			
			    if(!FAT_TDCdt_ref_gated[detj]) FAT_TDCdt_ref_gated[detj] = MakeTH1('D', Form("FATIMA/Timing/Gated/TDCdt_gated_LaBr%02d_LaBr%02d", FAT_REF_DET, detj),
											Form("TDC dt LaBr%02d (on %4.2f keV) - LaBr%02d (on %4.2f keV)", FAT_REF_DET, E_gate1, detj, E_gate2),3201,-40,40);
    
			    FAT_TDCdt_ref_gated[detj]->Fill(dt1);
			    printf("hit gates\n");
       
    									
			}
		    }
		}
	    }
	}
    }
}

void TSCNUnpackProc::Make_GALILEO_Histos(){
    
    GAL_REF_DET = 0;

       
    GAL_Pileup = MakeTH1('I',"GALILEO/GALILEO_Pileup","GALILEO Pileup",16,0,16);
    GAL_Hit_Pat = MakeTH1('I',"GALILEO/GALILEO_Hit_Pat","GALILEO Hit Pattern",16,0,16);
    GAL_Chan_E_Mat = MakeTH2('D',"GALILEO/GALILEO_E_Mat","GALILEO Channel Energy Matrix",10001,0,800000,10001,0,800000);

    GAL_Chan_E = new TH1*[32];
    GAL_Time_Diff_vs_Energy = new TH2*[32];
    GAL_Chan_Time_Diff = new TH1*[32];

    for(int i = 0; i < 32; ++i){
	
	GAL_Chan_E[i] = NULL;
	GAL_Time_Diff_vs_Energy[i] = NULL;
	GAL_Chan_Time_Diff[i] = NULL;

    }
}
void TSCNUnpackProc::Fill_GALILEO_Histos(){
        
    am_GALILEO_hits = RAW->get_GALILEO_am_Fired();
				    
    double tmpGAL[32];
    double time_1, time_2;
    double GAL_chan_time_diff;
    
    
    for(int i = 0; i < am_GALILEO_hits;  ++i){
				
	tmpGAL[i] = RAW->get_GALILEO_Chan_E(i);
	time_1 = RAW->get_GALILEO_Chan_T(i);

	
	if(tmpGAL[i] > 0){
	    if(!GAL_Chan_E[i]) GAL_Chan_E[i] = MakeTH1('D',Form("GALILEO/GALILEO_Energy_Spectra/GALILEO_E%2d",i),
						    Form("GALILEO Channel Energy Channel %2d",i),80001,0,800000);
	
	    GAL_Chan_E[i]->Fill(tmpGAL[i]);
	}
	
	GAL_Pileup->Fill(RAW->get_GALILEO_Pileup(i));
	GAL_Hit_Pat->Fill(RAW->get_GALILEO_Hit_Pattern(i));
	
	for(int j = 0; j < am_GALILEO_hits; ++j){
	    
	    if(i != j && i == GAL_REF_DET){
		
		tmpGAL[j] = RAW->get_GALILEO_Chan_E(j);

		time_2 = RAW->get_GALILEO_Chan_T(j);

		GAL_chan_time_diff = time_1 - time_2;
		

		if(!GAL_Chan_Time_Diff[j]) GAL_Chan_Time_Diff[j] = MakeTH1('D',Form("GALILEO/GALILEO_Chan_Time_DIff%2d",j),
								    Form("GALILEO Channel Time Difference",j),201,-1000,1000);

		if(!GAL_Time_Diff_vs_Energy[j]) GAL_Time_Diff_vs_Energy[j] = MakeTH2('D',Form("GALILEO/GALILEO_dT_vs_Energy_Spectra/GALILEO_dT_vs_E%2d",j),
										Form("GALILEO Time Difference Vs Channel Energy Channel %2d",j),201,-1000,1000,10001,0,800000);
		
		
		GAL_Chan_Time_Diff[j]->Fill(GAL_chan_time_diff);

		GAL_Time_Diff_vs_Energy[j]->Fill(GAL_chan_time_diff,tmpGAL[j]);
		
		GAL_Chan_E_Mat->Fill(tmpGAL[i],tmpGAL[j]);
		
	    }
	}
    }    
}
