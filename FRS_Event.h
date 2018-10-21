#ifndef FRS_EVENT_H_
#define FRS_EVENT_H_

#include "Events.h"
#include "Raw_Event.h"

class FRS_Event : public Events {

private:

	int am_fired;
	double energy_tot;
	inline void set_DATA(Raw_Event*);

public:

	FRS_Event(int* positions_tmp,int length,Raw_Event* RAW);
	virtual ~FRS_Event();

	double get_energy();
	void Write_Event(TFile* File);
};

#endif /* FRS_EVENT_H_ */
