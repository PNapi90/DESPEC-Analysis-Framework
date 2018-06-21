#include <iostream>
//#include "White_Rabbit.h"
#include "Detector_System.cxx"
#include "FATIMA_Detector_System.h"

using namespace std;

int main(){
    Detector_System** Det = new Detector_System*[1];
    Det[0] = new FATIMA_Detector_System();

    cout << "DONE" << endl;
     return 0;
}
