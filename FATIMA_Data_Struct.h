#ifndef FATIMA_DATA_H
#define FATIMA_DATA_H

#include <vector>

struct FATIMA_Data{
    int AmountFired = 0;
    std::vector<double> Energies(50,0),Times(50,0);
    std::vector<int> IDs(50,-1);
};


#endif
