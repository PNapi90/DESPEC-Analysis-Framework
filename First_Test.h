// $Id: TSCNUnpackProc.h 755 2011-05-20 08:04:11Z linev $
//-----------------------------------------------------------------------
//       The GSI Online Offline Object Oriented (Go4) Project
//         Experiment Data Processing at EE department, GSI
//-----------------------------------------------------------------------
// Copyright (C) 2000- GSI Helmholtzzentrum für Schwerionenforschung GmbH
//                     Planckstr. 1, 64291 Darmstadt, Germany
// Contact:            http://go4.gsi.de
//-----------------------------------------------------------------------
// This software can be used under the license agreements as stated
// in Go4License.txt file which is part of the distribution.
//-----------------------------------------------------------------------

#ifndef FIRST_TEST_H
#define FIRST_TEST_H

#include "Riostream.h"

// Root Includes //
#include "TROOT.h"
#include "TH1.h"
#include "TF1.h"
#include "TH2.h"
#include "TCutG.h"
#include "TArc.h"
#include "TTree.h"



// Go4 Includes //
#include "TGo4UserException.h"
#include "TGo4Picture.h"
#include "TGo4MbsEvent.h"


#include "TSCNUnpackEvent.h"

#include "Detector_System.cxx"
#include "FATIMA_Detector_System.h"
#include "PLASTIC_Detector_System.h"
#include "GALILEO_Detector_System_TEST.h"
//#include "FRS_Detector_System.h"

#include "Data_Stream.cxx"
#include "White_Rabbit.h"

#include "PLASTIC_Data_Stream.h"

#include "Raw_Event.h"

#include <string>


using namespace std;

#include "TGo4EventProcessor.h"

	class TSCNUnpackEvent;

	#include "TSCNUnpackEvent.h"

	class TSCNUnpackProc : public TGo4EventProcessor
	{
		public:
			TSCNUnpackProc();
			TSCNUnpackProc(const char* name);
			virtual ~TSCNUnpackProc();

			virtual Bool_t BuildEvent(TGo4EventElement* dest);

		protected:
		
	
			TH1* FAT_E;
			TH2* FAT_MAT;
			TH2* FAT_MAT_2;
			TH2* hit_mat;
			
			TH1* FAT_TDC_dT[36];
			TH1* Energy_Singles[36];
			TH1* Energy_Coinc[36];
			TH2* FAT_En_vs_dT[36];
			
			
			TH1* WR_HIST;
			TH1* WR_HIST2;
			TH1* C_t;
			TH1** tamex_Mult_lead;
			TH1** tamex_Mult_trail;

			TH1*** mat;
			TH1* all;
			TH1* all2;
			TH1* hit_hist;
			TH1* am_hits;
			TH1* tdc_hist;
			TH1* WR_F;
			TH1*** Coarse;
			TH1** DIFF_ARR;
			TH1*** tamex_Mult_Ch_lead;
			TH1*** tamex_Mult_Ch_trail;
			
			TH1* FAT_TDC_Diff; // ****NEWLY ADDED****
			

			TH2** tamex_mult_mat_lead;
			TH2** tamex_mult_mat_trail;

			TH1*** Trail_LEAD;
			TH1**** LEAD_LEAD;
			//for the SIS modules

			
			
			// GALILEO Histograms //
			
			//TH1* GAL_Evt_Time;
			TH1* GAL_Pileup;
			TH1* GAL_Hit_Pat;
			TH1* GAL_Chan_Time_Diff;
			TH1* GAL_Chan_E[5];
			TH2* GAL_Chan_E_Mat;
			TH2* GAL_Time_Diff_vs_Energy[5];

			

		private:
		
			int FATIMA_reference_det = 1;
		
			Bool_t ffill;
			Int_t fshift;
			ULong64_t White_Rabbbit_old;

			Int_t PrcID_Array[10][5];
			bool Used_Systems[10];

			bool SKIP_EVT_BUILDING;

			double vals[100000];
			int val_it;

			bool cals_done,WR_used;
			bool FAT_gain_match_used;
			bool FAT_gain_match_done;
			int file_pwd, file_end;
			std::string gain_match_filename;

			Detector_System** Detector_Systems;
			Data_Stream** data_stream;
			White_Rabbit* WR;
			Raw_Event* RAW;

			int amount_interest;
			int* length_interest;
			int** interest_array;

			//Event_Builder** EvtBuilder;

			double fatima_E_save[4];
			int am_FATIMA_hits;
			int num_full_FAT_evts;
			int am_GALILEO_hits;

			Int_t get_Conversion(Int_t);
			void get_used_Systems();
			void get_WR_Config();

			void load_PrcID_File();
			void get_interest_arrays();

			int count;
			int called[2];
			int iterator;

			ULong64_t WR_tmp[2];

			ClassDef(TSCNUnpackProc,1)
	};

#endif //Analysis_v5_H



