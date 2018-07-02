#include "Raw_Event_Storage.h"

using namespace std;

//---------------------------------------------------------------

Raw_Event_Storage::Raw_Event_Storage(){

	//Maximum event capacity of storage
	capacity = 1000;

	White_Rabbit_Storage = new ULong64_t[capacity];
	sub_event_type = new int[capacity];

}

//---------------------------------------------------------------

Raw_Event_Storage::~Raw_Event_Storage(){
	delete[] White_Rabbit_Storage;
}

//---------------------------------------------------------------

