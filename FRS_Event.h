#ifndef FRS_EVENT_H_
#define FRS_EVENT_H_

#include "Events.h"
#include "Raw_Event.h"

#include "FATIMA_DataStruct.h"
#include "PLASTIC_DataStruct.h"
#include "PLASTIC_VME_DataStruct.h"

class FRS_Event : public Events {

private:

	int am_fired;
	double energy_tot;
	inline void set_DATA(Raw_Event*);

public:

	FRS_Event(int* positions_tmp,int length,Raw_Event* RAW);
	virtual ~FRS_Event();

	double get_energy();
	void Write_Event(Tree_Creator*);

    PLASTIC_DataStruct* GET_PLASTIC(){return nullptr;};
    PLASTIC_VME_DataStruct* GET_PLASTIC_VME(){return nullptr;};

    FATIMA_DataStruct* GET_FATIMA(){return nullptr;};
};

#endif /* FRS_EVENT_H_ */
