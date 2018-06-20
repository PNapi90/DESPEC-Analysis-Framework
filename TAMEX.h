#include <iostream>
#include <string>
#include <sstream>
using namespace std;

// ############################################## //

struct TRIGGER_WINDOW
{  
    unsigned int PRE_TRIGG     : 16;
    unsigned int POST_TRIGG    : 16;
};

struct PADDING
{
    unsigned int  add        : 12;
    unsigned long empty      : 20;
};

struct TAMEX_FIRED
{
    unsigned int am_fired   : 8;
    unsigned long empty     : 24;
};

struct TAMEX_CHANNEL_HEADER
{
    unsigned int empty1     : 8;
    unsigned int trigger_t  : 4;
    unsigned int sfp_id     : 4;
    unsigned int Tamex_id   : 8;
    unsigned int empty2     : 8;
};

struct TAMEX_BEGIN
{
    unsigned long empty     : 24;
    unsigned int aa         : 8;
};
/*
struct TAMEX_FIRED
{
    unsigned long empty     : 24;
    unsigned int start      : 8;
};
*/
struct PLACE_HOLDER
{
    unsigned long empty     : 24;
    unsigned int six_eight  : 8;
};

struct TAMEX_DATA
{
    unsigned long coarse_T  : 11;
    unsigned int leading_E  : 1;
    unsigned long fine_T    : 10;
    unsigned int ch_ID      : 6;
    unsigned int TDC        : 4;

};

struct TAMEX_ERROR
{
    unsigned long err_code  : 24;
    unsigned int error      : 8;

};

struct TAMEX_TRAILER
{
    unsigned long trailer   : 32;
};