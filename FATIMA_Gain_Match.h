#ifndef FATIMA_GAIN_MATCH_H
#define FATIMA_GAIN_MATCH_H

#include <string>
#include <fstream>
#include <iostream>

class FATIMA_Gain_Match{

private:

    double gain_match_coeffs[100][5];

    std::string filename;

    void load_Gain_Match_File(std::string);
        

public:
    FATIMA_Gain_Match(std::string);
    ~FATIMA_Gain_Match();
        
    double Gain_Match(double,int);
};



#endif

