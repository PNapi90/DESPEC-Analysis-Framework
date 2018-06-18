# DEspec-Analysis-Framework (DEAF)
C++ analysis framwork of the DESPEC campaign @ GSI/FAIR

=======================================================================

All used detector systems:
  - FRS
  - AIDA
  - beta-Plastic
  - FATIMA
  - GALILEO

=======================================================================

The general idea of DEAF:
  - inside Go4 loop -> get MBS stream event pdata
  - extract White Rabbit Time Stamp of subevent
  - identify Detector System (FRS,AIDA,...) -> each system gets id: i 
  - Process MBS stream in corresponding detector system i (QDC,TDC,... analysis)
  - format output data stream in each system
  - send output data stream to corresponding data stream object i
  - build physical events inside eventbuilder object (for isomeric decay and beta decay)






=======================================================================

To do (UNPACKING):
  - FRS
  - AIDA
  - beta-Plastic
  - GALILEO 

=======================================================================

To do (EVENTBUILDING):
  - Define general data stream structure of each detector
  - Write abstract data stream class and inheriting detector system classes
  - Write EventBuilder class
  - How to build physical events?


!!! Go4 linking (access to data via Int_t* pdata) still missing !!!



