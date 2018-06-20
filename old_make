CC      = g++
CFLAGS  = -g -O3 -std=c++11 -Wall
LIBFLAGS = #-I ~/eigen_dir/

OBJECTS = Detector_System.o FATIMA_DETECTOR.o FATIMA_Detector_System.o FATIMA_Energy_Calibration.o FATIMA_Time_Calibration.o QDC_FATIMA_Channel.o TDC_FATIMA_Channel.o QDC_TDC_FATIMA.o White_Rabbit.o

all: Program

%.o: %.cxx
	$(CC) -c $< $(CFLAGS) $(LIBFLAGS)
	
Program: $(OBJECTS) short_main.o
	$(CC) $(OBJECTS) short_main.o -o rough_test

clean: 
	rm -f *.o $(PROGRAM)

