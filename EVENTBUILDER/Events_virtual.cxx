#ifndef EVENTS_H
#define EVENTS_H

class Events{

public:
    
    virtual ~Events(){};

    virtual void set_Match_ID_address(int*) = 0;
    virtual void set_Match_ID_address_to_NULL_ALL() = 0;
    
    virtual int get_iterator() = 0;

    virtual bool Permission(int) = 0;

    virtual int* get_Match_ID_address(int) = 0;
    
};

#endif
