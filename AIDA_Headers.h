#ifndef AIDA_HEADERS_H
#define AIDA_HEADERS_H


#include <iostream>
#include <string>
#include <sstream>
using namespace std;

typedef unsigned long long ULong64_t;


struct AIDA_t0_Header // Contains bits 0->15 of 64 bit t0 timestamp
{  

	ULong64_t t0_1st		: 16;
	unsigned int empty_2		: 4;
	unsigned int second_check	: 4; // Should be = 5
	unsigned int empty		: 6;
	unsigned int check		: 2; // Should be = 2

};

struct AIDA_t0_Header_2 // Contains bits 36->63 of 64 bit t0 timestamp
{

	ULong64_t t0_3rd		: 28;
	unsigned int empty		: 4;

};

struct AIDA_t0_Header_3 // Contains bits 16->35 of 64 bit t0 timestamp
{

	ULong64_t t0_2nd		: 20;
	unsigned int empty		: 12;

};

struct AIDA_t0_Header_4
{

	unsigned int empty		: 32;

};

//-------------------------------------------------

struct AIDA_ADC_Times
{

	unsigned long empty 		: 30;
	unsigned int check 		: 2; // Should be = 3

};

struct AIDA_ADC_Times_2 // Time must be added to the base of WR timestamp
{

	ULong64_t ADC_time		: 28;
	unsigned int empty		: 4;

};

//-------------------------------------------------

struct AIDA_1st_Disc
{

	unsigned int empty_2		: 20;
	unsigned int second_check	: 4; // Should be = 6
	unsigned int empty		: 4;	
	unsigned int check		: 4; // Should be = 8
	
};

struct AIDA_1st_Disc_2 // Time must be added to the base of WR timestamp
{

	ULong64_t first_disc_time	: 28;
	unsigned int empty		: 4;
	
};

//-------------------------------------------------

#endif
