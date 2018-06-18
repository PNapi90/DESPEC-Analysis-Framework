#include <map>


channels::channels(int number_of_channels)
{
		 	 
	/*QShort_Raw[number_of_channels];		
	QShort_Cal[number_of_channels];		
	QLong_Raw[number_of_channels];		
	QLong_Cal[number_of_channels];
	
	QDC_Time[number_of_channels];
	QDC_Fine_Time[number_of_channels];
	QDC_Ext_Time[number_of_channels];
	
	TDC_Time[number_of_channels];
		
	QDC_Fired[number_of_channels];
	TDC_Fired[number_of_channels];*/

	
	for(int i=0; i<number_of_channels; i++)
	{
		
		QShort_Raw[i] = 0;		
		QShort_Cal[i] = 0;		
		QLong_Raw[i] = 0;		
		QLong_Cal[i] = 0;
	
		QDC_Time[i] = 0;
		QDC_Fine_Time[i] = 0;
		QDC_Ext_Time[i] = 0;
	
		TDC_Time[i] = 0;
	
		QDC_Fired[i] = 0;		
		TDC_Fired[i] = 0;
		
	}
	
}

allocation::allocation(std::string filename)
{
	QDC_map.clear();
	TDC_map.clear();

	std::ifstream infile(filename.c_str());
	
    
	while (infile >> chNo >> QDC_geo >> QDC_chan >> TDC_geo >>TDC_chan >> active)
	{
		
		QDC_map[std::make_pair(QDC_geo,QDC_chan)] = chNo;
		TDC_map[std::make_pair(TDC_geo,TDC_chan)] = chNo;
		
		det_active_QDC[std::make_pair(QDC_geo,QDC_chan)] = active;
		det_active_TDC[std::make_pair(QDC_geo,QDC_chan)] = active;

	}
	
}

setup::setup(std::string filename)
{

	std::ifstream infile(filename.c_str());
	
    
		infile.ignore(256,':');
		infile >> online;

		infile.ignore(256,':');
		infile >> extras;

		infile.ignore(256,':');
		infile >> traces;
		
		infile.ignore(256,':');
		infile >> time_calib;
}


void channels::reset_checks(int number_of_channels)
{
	
	for(int i=0; i<number_of_channels; i++)
	{
		
		QDC_Fired[i] = 0;		
		TDC_Fired[i] = 0;		
		
	}
	
}



void channels::reset_channel(int channel_number)
{
	
	
	QShort_Raw[channel_number] = 0;		
	QShort_Cal[channel_number] = 0;		
	QLong_Raw[channel_number] = 0;		
	QLong_Cal[channel_number] = 0;
	
	QDC_Time[channel_number] = 0;
	QDC_Fine_Time[channel_number] = 0;
	QDC_Ext_Time[channel_number] = 0;
	
	TDC_Time[channel_number] = 0;
	
	QDC_Fired[channel_number] = 0;		
	TDC_Fired[channel_number] = 0;
	
}


int channels::QDC_checker(int number_of_channels)
{
	
	int check = 0;
	
	for(int i = 0; i<number_of_channels; i++)
	{
			
			if (QDC_Fired[i] == 1) check++;
			
	}
		
	return check;
		
}

int channels::TDC_checker(int number_of_channels)
{
	
	int check = 0;
	
	for(int i = 0; i<number_of_channels; i++)
	{
			
			if (TDC_Fired[i] == 1) check++;
			
	}
	
	return check;
		
}
// ###################################################### //
// ------------------- SET VARIABLES -------------------- //
// ###################################################### //



void channels::set_QShort_Raw(int channel_number, Int_t input)
{
	
	QShort_Raw[channel_number] = input;		
	
}	

void channels::set_QShort_Cal(int channel_number, Int_t input)
{
	
	QShort_Cal[channel_number] = input;		
	
}	

void channels::set_QLong_Raw(int channel_number, Int_t input)
{
	
	QLong_Raw[channel_number] = input;		
	
}	

void channels::set_QLong_Cal(int channel_number, Int_t input)
{
	
	QLong_Cal[channel_number] = input;		
	
}	

void channels::set_QDC_Time(int channel_number, unsigned long input)
{
	
	QDC_Time[channel_number] = input;		
	
}	

void channels::set_QDC_Fine_Time(int channel_number, Double_t input)
{
	
	QDC_Fine_Time[channel_number] = input;		
	
}	

void channels::set_QDC_Ext_Time(int channel_number, unsigned long input)
{
	
	QDC_Ext_Time[channel_number] = input;		
	
}	

void channels::set_TDC_Time(int channel_number, Double_t input)
{
	
	TDC_Time[channel_number] = input;		
	
}

void channels::set_QDC_Fired(int channel_number)
{
	
	QDC_Fired[channel_number] = 1;		
	
}

void channels::set_TDC_Fired(int channel_number)
{
	
	TDC_Fired[channel_number] = 1;		
	
}

void channels::set_WR_Time(ULong64_t white_rabbit)
{
	
	WR_Time = white_rabbit;		
	
}



// ###################################################### //
// ------------------- GET VARIABLES -------------------- //
// ###################################################### //


Int_t channels::get_QShort_Raw(int channel_number)
{
	
	return 	QShort_Raw[channel_number];		
	
}	

Int_t channels::get_QShort_Cal(int channel_number)
{
	
	return 	QShort_Cal[channel_number];		
	
}	

Int_t channels::get_QLong_Raw(int channel_number)
{
	
	return 	QLong_Raw[channel_number];		
	
}	

Int_t channels::get_QLong_Cal(int channel_number)
{
	
	return 	QLong_Cal[channel_number];		
	
}	

unsigned long channels::get_QDC_Time(int channel_number)
{
	
	return 	QDC_Time[channel_number];		
	
}

Double_t channels::get_QDC_Fine_Time(int channel_number)
{
	
	return QDC_Fine_Time[channel_number];		
	
}	

unsigned long channels::get_QDC_Ext_Time(int channel_number)
{
	
	return QDC_Ext_Time[channel_number];		
	
}

Double_t channels::get_TDC_Time(int channel_number)
{
	
	return 	TDC_Time[channel_number];		
	
}	

int channels::get_QDC_Fired(int channel_number)
{
	
	return QDC_Fired[channel_number];		
	
}

int channels::get_TDC_Fired(int channel_number)
{
	
	return TDC_Fired[channel_number];	
	
}

ULong64_t channels::get_WR_Time()
{
	
	return WR_Time;	
	
}




int allocation::get_QDC_Chan(int geo_ad, int channel_number)
{
	
	if (det_active_QDC[std::make_pair(geo_ad, channel_number)] != 0)
	{
		return QDC_map[std::make_pair(geo_ad, channel_number)];	
	}
	else return -1;
	
}

int allocation::get_TDC_Chan(int geo_ad, int channel_number)
{
	if (det_active_TDC[std::make_pair(geo_ad, channel_number)] != 0)
	{
		return TDC_map[std::make_pair(geo_ad, channel_number)];	
	}
	else return -1;
}


int setup::get_online()
{
	
	return online;	
	
}
int setup::get_extras()
{
	
	return extras;	
	
}
int setup::get_traces()
{
	
	return traces;	
	
}
int setup::get_time_calib()
{
	
	return time_calib;
	
}
	




