# DEspec-Analysis-Framework (DEAF)
C++ analysis framwork for the DESPEC campaign @ GSI/FAIR

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
  - for each physical event -> create "interesting" histograms and matrices

EVENTBUILDER:
  - get important informations from RAW_EVENT class obj:
  - FATIMA: E[ ], T[ ], hitID[ ]
  - GALILEO: E[ ], T[ ], hitID[ ]
  - PLASTIC: T,... (what else necessary ?)
  - AIDA: X[ ], Y[ ], E[ ], T[ ], GRID_ADRESS[ ]
  - FRS: ?
  - buffer events for few seconds (experimental seconds) => in Raw_Event_Storage class

=======================================================================

EXPECTED DATA FORMATS BY double** data_stream (White Rabbit TS already known by EVENTBUILDER):
  - FATIMA: data_stream[i] = {E[i],T[i],detID[i],am_hits}
  - GALILEO: data_stream[i] = {E[i],T[i],detID[i],am_hits}
  - AIDA: data_stream[i] = {E[i],T[i],hit_grid_id[i],strip_x_ID[i],strip_y_ID[i],am_hits}
  - FRS: data_stream[i] = {?,?,?,...,am_hits}
  - PLASTIC: data_stream[i] = {E[i],T[i],...,am_hits}

where i represents the index over the amount of hits am_hits (different for each system)  

=======================================================================

=======================================================================

To do (UNPACKING):
  - FRS
  - AIDA
  - GALILEO 

=======================================================================

To do (EVENTBUILDING):
  - Write EventBuilder class
  - How to build physical events?

=======================================================================