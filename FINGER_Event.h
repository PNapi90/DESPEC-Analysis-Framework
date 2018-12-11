#ifndef FINGER_EVENT_H_
#define FINGER_EVENT_H_

#include <TFile.h>

#include "Events.h"
#include "Raw_Event.h"

#include "FATIMA_DataStruct.h"
#include "PLASTIC_DataStruct.h"
#include "PLASTIC_VME_DataStruct.h"

class FINGER_Event: public Events {

private:

	int am_fired;
	double energy_tot;
	inline void set_DATA(Raw_Event*);


public:
	FINGER_Event(int* positions_tmp,int length,Raw_Event* RAW);
	virtual ~FINGER_Event();

	double get_energy();
	void Write_Event(Tree_Creator*);

    PLASTIC_DataStruct* GET_PLASTIC(){return nullptr;};
    PLASTIC_VME_DataStruct* GET_PLASTIC_VME(){return nullptr;};

    FATIMA_DataStruct* GET_FATIMA(){return nullptr;};
};

#endif /* FINGER_EVENT_H_ */
