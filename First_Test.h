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
			//FATIMA variables
			double FATgate1_low, FATgate1_high;
			double FATgate2_low, FATgate2_high;
			
			TSCNUnpackProc();
			TSCNUnpackProc(const char* name);
			virtual ~TSCNUnpackProc();

			virtual Bool_t BuildEvent(TGo4EventElement* dest);

		protected:
		


			TH1* hsci_tofll2;
			
			TH1* hsci_tofll3;
			TH1* hsci_tof2;
			TH1* hsci_tofrr2;
			TH1* hsci_tofrr3;
			TH1* hsci_tof3;
			
			TH1* hID_x2;
			TH1* hID_y2;
			TH1* hID_a2;
			TH1* hID_b2;
			
			TH1* hID_x4;
			TH1* hID_y4;
			TH1* hID_a4;
			TH1* hID_b4;
			
			TH1* hsci_dt_21l_21r;
			TH1* hsci_dt_41l_41r;
			TH1* hsci_dt_42l_42r;
			TH1* hsci_dt_43l_43r;
			TH1* hsci_dt_81l_81r;
	
			TH1* hsci_dt_21l_41l;
			TH1* hsci_dt_21r_41r;
	
			TH1* hsci_dt_21l_42l;
			TH1* hsci_dt_21r_42r;
		
			TH1* hsci_dt_21l_81l;
			TH1* hsci_dt_21r_81r;
			
			
			TH1* hbeta;
			TH1* hbeta3;
			TH1* hgamma;
			
			TH1* hAoQ;
			TH1* hAoQ_corr;
			
			TH1* hz;
			TH1* hz2;
			TH1* hz3;
			
			TH1* htimestamp;
			TH1* hts;
			TH1* hts2;
			
		
			//Fatima histograms
			//-general
			TH1* FAT_Esum;
			TH2* FAT_gg;
			TH1* FAT_TDCdtsum;
			TH1* FAT_QDCdtsum;
			TH1* FAT_TDCdtsum_ref_gated;		//gates are hard coded
			TH1* FAT_QDCdtsum_ref_gated;   //for now...
			TH1* FAT_Angular_Diff_ref_gated; // Histogram of Gated Angular Differences
			//-statistics
			TH1* FAT_hits;		     //number of hits per detector id
			TH1* FAT_hits_QDC;
			TH1* FAT_hits_TDC;
			TH2* FAT_QDC_TDC_hitmap; //hits of qdc and tdc in same event
			TH2* FAT_correlations;   //det-det coincidence map
			//-energy
			TH1** FAT_E;
			TH1** FAT_Eraw;
			TH2** FAT_E_ratio;
			TH2** FAT_gg_ref;
			//-timing
			TH1** FAT_TDCdt_ref;
			TH1** FAT_QDCdt_ref;
			TH2** FAT_TDC_QDC_dt;
			TH1** FAT_TDCdt_ref_gated;
			TH2** FAT_E_TDCdt_ref_gated;

			//Other histograms			 
			TH1* WR_HIST;
			TH1* WR_HIST2;
			TH1* C_t;
			TH1** tamex_Mult_lead;
			TH1** tamex_Mult_trail;

			TH1*** mat;
			TH1* all;
			TH1* all2;
			
			TH1* WR_F;
			
			
			//Plastic histograms
			TH1*** tamex_Mult_Ch_lead;
			TH1*** tamex_Mult_Ch_trail;
			TH2** tamex_mult_mat_lead;
			TH2** tamex_mult_mat_trail;
			TH1*** Trail_LEAD;
			TH1**** LEAD_LEAD;
			TH1*** Coarse;
			TH1** DIFF_ARR;
			
			
			TH1* FAT_TDC_Diff; // ****NEWLY ADDED****
			

			

			
			//for the SIS modules

			
			
			// GALILEO Histograms //
			
			//TH1* GAL_Evt_Time;
			TH1* GAL_Pileup;
			TH1* GAL_Hit_Pat;
			TH1** GAL_Chan_Time_Diff;
			TH1** GAL_Chan_E;
			TH2* GAL_Chan_E_Mat;
			TH2** GAL_Time_Diff_vs_Energy;

			

		private:
		
			const int FATIMA_reference_det = 0;
			const int FAT_MAX_DET = 36;

			int FAT_REF_DET;
			int GAL_REF_DET;

			float E_gate1,E_gate2;

			Bool_t ffill;
			Int_t fshift;
			ULong64_t White_Rabbbit_old;

			Int_t PrcID_Array[10][5];
			bool Used_Systems[10];

			bool SKIP_EVT_BUILDING;

			double vals[100000];
			int val_it;

			string input_data_path;
			string input_data_path_old;

			bool cals_done,WR_used;
			bool WHITE_RABBIT_USED; // Read from General Setup File
			bool FAT_make_raw_histograms;
			bool FAT_gain_match_used; // Read from General Setup File
			bool FAT_gain_match_done;
			bool FAT_dist_corr_used; // Read from General Setup File
			int FAT_exclusion_dist; // Read from General Setup File
			int FAT_num_TDC_modules; // Read from General Setup File
			bool same_ring_exclusion; // Read from General Setup File
			bool output_position_matrix; // Read from General Setup File
			int file_pwd, file_end;
			std::string gain_match_filename;
			int data_file_number = 0;

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
			
			void read_setup_parameters();

			void load_PrcID_File();
			void get_interest_arrays();
			
			void Make_FRS_Histos();
			void Fill_FRS_Histos();
			
			void Make_Plastic_Histos();
			void Fill_Plastic_Histos();
			
			void Make_FATIMA_Histos();
			void Fill_FATIMA_Histos();
			
			void Make_GALILEO_Histos();
			void Fill_GALILEO_Histos();
			
			
			bool Check_Cal_Plastic();
			
			bool PLASTIC_CALIBRATION;
			
			double** FAT_positions;
			double** FAT_angle_diffs;
			bool** FAT_neighbour_check;
			
			void FAT_det_pos_setup();
			double distance_between_detectors(double, double, double, double, double, double);
			double angle_between_detectors(double, double, double);


			int count;
			int called[2];
			int iterator;
			int Cout_counter;

			ULong64_t WR_tmp[2];

			ClassDef(TSCNUnpackProc,1)
	};

#endif //Analysis_v5_H



