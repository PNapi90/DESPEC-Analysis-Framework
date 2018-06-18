#ifndef channels_h
#define channels_h

#include <stdint.h> 

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include "TROOT.h"
#include "Riostream.h"

#include <map>

class channels
{

	public : 

		channels(int number_of_channels); // Sets up all detector channels
		
		void reset_channel(int channel_number); // resets all values
		void reset_checks(int number_of_channels); // resets all checks
		
		int QDC_checker(int number_of_channels);
		int TDC_checker(int number_of_channels);
		
		// ----- Set Channels ----- //
	
		void set_QShort_Raw(int channel_number, Int_t input);
		void set_QShort_Cal(int channel_number, Int_t input);
		void set_QLong_Raw(int channel_number, Int_t input);
		void set_QLong_Cal(int channel_number, Int_t input);
		
		void set_QDC_Time(int channel_number, unsigned long input);
		void set_QDC_Fine_Time(int channel_number, Double_t input);
		void set_QDC_Ext_Time(int channel_number, unsigned long input);

		void set_TDC_Time(int channel_number, Double_t input);
		
		void set_QDC_Fired(int channel_number);
		void set_TDC_Fired(int channel_number);
		
		void set_WR_Time(ULong64_t white_rabbit);


		// ----- Get Channels ----- //

		Int_t get_QShort_Raw(int channel_number);
		Int_t get_QShort_Cal(int channel_number);
		Int_t get_QLong_Raw(int channel_number);
		Int_t get_QLong_Cal(int channel_number);
		
		unsigned long get_QDC_Time(int channel_number);
		Double_t get_QDC_Fine_Time(int channel_number);
		unsigned long get_QDC_Ext_Time(int channel_number);

		Double_t get_TDC_Time(int channel_number);
		
		int get_QDC_Fired(int channel_number);
		int get_TDC_Fired(int channel_number);
		
		ULong64_t get_WR_Time();

	private : 
	
		
		Int_t QShort_Raw[36];		
		Int_t QShort_Cal[36];		
		Int_t QLong_Raw[36];		
		Int_t QLong_Cal[36];
	
		unsigned long QDC_Time[36];
		Double_t QDC_Fine_Time[36];
		unsigned long QDC_Ext_Time[36];
	
		Double_t TDC_Time[36];
		
		int QDC_Fired[36];
		int TDC_Fired[36];
		
		ULong64_t WR_Time;
		
		
};

class allocation
{
	
		public : 

			allocation(std::string filename);

			int get_QDC_Chan(int geo_ad, int channel_number);
			int get_TDC_Chan(int geo_ad, int channel_number);
	
		private : 

			int chNo, QDC_geo, TDC_geo, QDC_chan, TDC_chan, active;

			std::map<std::pair<int,int>, int> QDC_map;
			std::map<std::pair<int,int>, int> TDC_map;
			std::map<std::pair<int,int>, int> det_active_QDC;
			std::map<std::pair<int,int>, int> det_active_TDC;
	
};

class setup
{
	
		public : 

			setup(std::string filename);

			int get_online();
			int get_extras();
			int get_traces();
			int get_time_calib();
	
		private : 

			int online = 0;
			int extras = 0;
			int traces = 0;
			int time_calib = 0;
	
};


#endif

