#!/bin/bash

#use script via . run_DESPEC.sh <input_file_name> <output_file_name>

#copy head of PLASTIC_CALIB_FILE into tmp
head -n 4 Configuration_Files/PLASTIC_CALIB_FILE.txt > tmp

#set Calibration ONLINE
echo "ONLINE		1" >> tmp
echo "FORCE		0" >> tmp

mv tmp Configuration_Files/PLASTIC_CALIB_FILE.txt

#do Online calibration run
rm -f ${2}
go4analysis -file ${1} -asf ${2} | tee showOut.txt

#do Offline calibration run
rm -f ${2}
go4analysis -file ${1} -asf ${2} | tee showOut.txt
