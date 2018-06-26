#ifndef DATA_STREAM_H
#define DATA_STREAM_H

class Data_Stream{

public:
    
    virtual ~Data_Stream() = 0;

    virtual void reset() = 0;

    virtual bool get_mismatch() = 0;

    virtual int get_amount_Hits() = 0;

    virtual double get_E(int) = 0;

};

#endif