#ifndef AIDA_EVENT_H_
#define AIDA_EVENT_H_

#include "Events.h"
#include "Raw_Event.h"

class AIDA_Event: public Events {

private:

    int am_fired;
    double energy_tot;
    inline void set_DATA(Raw_Event*);


public:
	AIDA_Event(int* positions_tmp,int length,Raw_Event* RAW);
	virtual ~AIDA_Event();

	double get_energy();
	void Write_Event(Tree_Creator* Tree);
};

#endif /* AIDA_EVENT_H_ */
