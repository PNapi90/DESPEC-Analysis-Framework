//Changes, 24.07.18 MR

//b) changed allocation file name from allocation.txt to
//	 FATIMA_allocation.txt

#include "TH1.h"
#include "TH2.h"

#include "FRS_Histograms.h"
#include "TGo4EventProcessor.h"
#include "TXRSBasicProc.h"

#include <cstdlib>

using namespace std;

//---------------------------------------------------------------

FRS_Histograms::FRS_Histograms() : TXRSBasicProc("FRSHistos"){


    
}

void FRS_Histograms::Create_Unpack_Hist(){
    
      hTrigger = MakeH1I("Raw_data", "newTrigger", 16, 0.5, 16.5); 

  bool remove_histos = true; //(frs!=nullptr) && (!frs->fill_raw_histos);
  
  for(int n=0;n<32;n++){

      
      hVME0_8[n]  = MakeH1ISeries("Raw_data/VME0",  8, 0, n, remove_histos);
      hVME0_9[n]  = MakeH1ISeries("Raw_data/VME0",  9, 0, n, remove_histos);
      hVME0_11[n] = MakeH1ISeries("Raw_data/VME0", 11, 0, n, remove_histos);
      hVME0_12[n] = MakeH1ISeries("Raw_data/VME0", 12, 0, n, remove_histos);
      //    hVME0_13[n] = MakeH1ISeries("Raw data/VME0", 13, 0, n, remove_histos);
      //    hVME0_14[n] = MakeH1ISeries("Raw data/VME0", 14, 0, n, remove_histos);

      hVME1_8[n]  = MakeH1ISeries("Raw_data/VME1", 8, 1, n, remove_histos);
      hVME1_9[n]  = MakeH1ISeries("Raw_data/VME1", 9, 1, n, remove_histos);
      hVME1_3[n]  = MakeH1ISeries("Raw_data/VME1", 3, 1, n, remove_histos);
      hVME1_15[n] = MakeH1ISeries("Raw_data/VME1", 15, 1, n, remove_histos);

      hVME5_11[n] = MakeH1ISeries("Raw_data/VME5", 11, 5, n, remove_histos);
      
      //hVME1_16[n] = MakeH1ISeries("Raw data/VME1", 16, 1, n, remove_histos);
      //hVME1_17[n] = MakeH1ISeries("Raw data/VME1", 17, 1, n, remove_histos);

      hVME2_TDC[n] = MakeH1ISeries3("Raw_data/VME2", 1, 2, n, remove_histos);
 
      hVME3_TDC[n] = MakeH1ISeries3("Raw_data/VME3", 2, 3, n, remove_histos);
      //hVME2_12[n] = MakeH1ISeries("Raw data/VME2", 12, 2, n, remove_histos);
      
    }

  h_VME0_8All  = MakeH2I("Raw_data/VME0","V0_Module_geo8AllCh",32,0,32,512,0,4096,"#Ch","",1);
  h_VME0_9All  = MakeH2I("Raw_data/VME0","V0_Module_geo9AllCh",32,0,32,512,0,4096,"#Ch","",1);
  h_VME0_11All = MakeH2I("Raw_data/VME0","V0_Module_geo11AllCh",32,0,32,512,0,4096,"#Ch","",1);
  h_VME0_12All = MakeH2I("Raw_data/VME0","V0_Module_geo12AllCh",32,0,32,512,0,4096,"#Ch","",1);
			     
  h_VME1_8All  = MakeH2I("Raw_data/VME1","V1_Module_geo8AllCh",32,0,32,512,0,4096,"#Ch","",1);
  h_VME1_9All  = MakeH2I("Raw_data/VME1","V1_Module_geo9AllCh",32,0,32,512,0,4096,"#Ch","",1);
  h_VME1_3All  = MakeH2I("Raw_data/VME1","V1_Module_geo3AllCh",32,0,32,512,0,4096,"#Ch","",1);
  h_VME1_15All = MakeH2I("Raw_data/VME1","V1_Module_geo15AllCh",32,0,32,512,0,4096,"#Ch","",1);
			     
  hVME2_TDCAll = MakeH2I("Raw_data/VME2","V2_Module_AllCh",32,0,32,1024,0,1048576,"#Ch","",1);
  hVME3_TDCAll = MakeH2I("Raw_data/VME3","V3_Module_AllCh",32,0,32,1024,0,1048576,"#Ch","",1);

  
  
  h_UnpackStatus = MakeH2I("Unpack","Status",2*21*32,0.,2*21*32,10,0.,10.,"#Ch","Status",1);
  h_TSFlagStatus = MakeH2I("Unpack","TS_flagStatus",1000,0,1000,10,0,10,"Diff_TS","Status",1);
  
  h_Multiplicity = MakeH2I("Unpack","mTDC_multiplicity",200,0,200,10,0,10,"ch mTDC","Multiplicity",1);
    
    
    
}


void FRS_Histograms::Create_MRTOF_Hist(){

  h_MRtof_Start = MakeH1F("MRTOF/Time","mrtof_start",1000,0,50,"microsec",2,6);
  h_MRtof_StopDelay = MakeH1F("MRTOF/Time","mrtof_stopDelay",1000,0,50,"microsec",2,6);
  h_MRtof_Stop = MakeH1F("MRTOF/Time","mrtof_stop",1000,0,50,"microsec",2,6);
  h_MRtof_tof = MakeH1F("MRTOF/Time","mrtof_tof",200000,0,200,"microsec",2,6);
  h_MRtof_status = MakeH1F("MRTOF/Time","mrtof_TimeStatus",10,0,10,"",2,6);
  
}

void FRS_Histograms::Create_MUSIC_Hist(){
    char name[80], xtitle[80]; 
    for(int i=0;i<8;i++){
    
	sprintf(name,"MUSIC1_E(%d)",i);  
	sprintf(xtitle,"dE MUSIC1(%d)",i);
	hMUSIC1_E[i] = MakeH1I("MUSIC/MUSIC(1)/E",name,4096,0,4096,xtitle,2,6);
	
	sprintf(name,"MUSIC2_E(%d)",i);  
	sprintf(xtitle,"dE MUSIC2(%d)",i);
	hMUSIC2_E[i] = MakeH1I("MUSIC/MUSIC(2)/E",name,4096,0,4096,xtitle,2,6);
	
	sprintf(name,"MUSIC1_T(%d)",i);  
	sprintf(xtitle,"dT MUSIC1(%d) ",i);
	hMUSIC1_T[i] = MakeH1I("MUSIC/MUSIC(1)/T",name,4096,0,4096,xtitle,2,6);
	
	sprintf(name,"MUSIC2_T(%d)",i);  
	sprintf(xtitle,"dT MUSIC2(%d) ",i);
	hMUSIC2_T[i] = MakeH1I("MUSIC/MUSIC(2)/T",name,4096,0,4096,xtitle,2,6);
	
    }
    
    hMUSIC1_dE1dE2 = MakeH2I("MUSIC/MUSIC(1)/E","dE1_dE2",1024,0,4096,1024,0,4096,"dE1","dE2",2);
    hMUSIC1_MUSIC2 = MakeH2I("MUSIC/MUSIC1_MUSIC2","dE1_dE2",1024,0,4096,1024,0,4096,"dE1","dE2",2);
    
    
    for(int i=0;i<4;i++){
	
	sprintf(name,"MUSIC3_E(%d)",i);  
	sprintf(xtitle,"dE MUSIC3(%d)",i);
	hMUSIC3_E[i] = MakeH1I("MUSIC/MUSIC(3)/E",name,4096,0,4096,xtitle,2,6);
	
	sprintf(name,"MUSIC3_dEdx_betagamma(%d)",i);  
	h_dEdx_betagamma[i] = MakeH2I("MUSIC/MUSIC(3)/BT",name,1000,0.4,1.4,1000,0,100,"betagamma","dE/dx (ch/cm)",1);
	
	sprintf(name,"MUSIC3_DiffdEdx_betagamma(%d)",i);  
	h_DiffdEdx_betagamma[i] = MakeH2I("MUSIC/MUSIC(3)/BT",name,1000,0.4,1.4,1000,-10,10,"betagamma","DiffdE/dx (ch/cm)",1);
    
	for(int k=0;k<5;++k){
	
	    sprintf(name,"MUSIC3_z_AoQ_E(%d)gate(%d)",i,k);  
	    sprintf(xtitle,"dE MUSIC3(%d) zAoQgate(%d)",i,k);
	    hMUSIC3_z_AoQ_E[i][k] = MakeH1I("MUSIC/MUSIC(3)/E",name,4096,0,4096,xtitle,2,6);
	    
	    sprintf(name,"MUSIC3_music_z_E(%d)gate(%d)",i,k);  
	    sprintf(xtitle,"dE MUSIC3(%d) gate(%d)",i,k);
	    hMUSIC3_music_z_E[i][k] = MakeH1I("MUSIC/MUSIC(3)/E",name,4096,0,4096,xtitle,2,6); 

	}

	for(int k=0;k<6;++k){
	    
	    sprintf(name,"MUSIC3_x2AoQ_E(%d)gate(%d)",i,k);  
	    sprintf(xtitle,"dE MUSIC3(%d) x2AoQgate(%d)",i,k);
	    hMUSIC3_x2AoQ_E[i][k] = MakeH1I("MUSIC/MUSIC(3)/E",name,4096,0,4096,xtitle,2,6);
	
	}
	
	sprintf(name,"MUSIC3_T(%d)",i);  
	sprintf(xtitle,"dT MUSIC3(%d) ",i);
	hMUSIC3_T[i] = MakeH1I("MUSIC/MUSIC(3)/T",name,4096,0,4096,xtitle,2,6);
	
    }
    
    
    hMUSIC1_dE = MakeH1I("MUSIC/MUSIC(1)/E","MUSIC1_dE",4000,0.0,4000.0,"Average dE MUSIC1 (root)",2,6);
    hMUSIC2_dE = MakeH1I("MUSIC/MUSIC(2)/E","MUSIC2_dE",4000,0.0,4000.0,"Average dE MUSIC2 (root)",2,6);
    hMUSIC3_dE = MakeH1I("MUSIC/MUSIC(3)/E","MUSIC3_dE",4000,0.0,4000.0,"Average dE MUSIC3 (root)",2,6);
    
    
    hMUSIC3_dEx = MakeH2I("MUSIC/MUSIC(3)/E","MUSIC3_dEx",100,-100,100,200,0,4096,
    "Average x position in MUSIC3","dE MUSIC3 [channels]",2);
    
    h_dEdx_betagammaAll = MakeH2I("MUSIC/MUSIC(3)","MUSIC3_dEdx_betagammaAll",1000,0.4,1.4,1000,0,100,"betagamma","dE/dx (ch/cm)",1);
    h_dEdx_betagammaAllZoom = MakeH2I("MUSIC/MUSIC(3)","MUSIC3_dEdx_betagammaAllZoom",1000,0.4,1.4,1000,0,1,"betagamma","dE/dx (ch/cm)",1);
    
    hMUSIC3_dECOR = MakeH1I("MUSIC/MUSIC(3)/E","MUSIC3_dECOR",4000,0.5,4000.5,"dE MUSIC3 corrected for position",2,6);
    
    hMUSIC3_dExc = MakeH2I("MUSIC/MUSIC(3)","MUSIC3_dExc",100,-100,+100,200,0,4096,
    "Average x position in MUSIC3", "dE MUSIC3 (3)  [channels]", 2);

}

void FRS_Histograms::Create_SCI_Hist(){
    char fname[50], name[50], title[60];//, title2[60];
    
    const char *count_title1[12]={"(0:1)", "(1:1)", "(2:1)",
    "(2:2)", "(3:1)", "(4:1)",
    "(4:2)", "(4:3)", "(6:1)",
    "(6:2)", "(8:1)"};
    const char *fext1[12]={"0", "1", "2", "2", "3", "4", "4", "4", "6", "6", "8", "8"};
    const char *fext2[12]={"01", "11", "21", "22","31", "41",
    "42", "43", "61",
    "62", "81", "82"};
    
    for (int cnt = 0; cnt<6; cnt++){
    
	int index = 0;
	switch(cnt){
	    
	    case 0: index = 2; break;
	    case 1: index = 3; break;
	    case 2: index = 5; break;
	    case 3: index = 6; break;
	    case 4: index = 7; break;
	    case 5: index = 10; break;
	}   
	sprintf(fname,"SCI/SCI%s/SCI%s",fext1[index],fext2[index]);
	sprintf(name, "SCI%s_L", count_title1[index]);  
	sprintf(title, "Sc%s L dE [ch]", count_title1[index]);
	hSCI_L[index] = MakeH1I(fname,name,4096,0,4096,title,2,3);
	
	sprintf(name, "SCI%s_R", count_title1[index]);  
	sprintf(title, "Sc%s R dE [ch]", count_title1[index]);
	hSCI_R[index] = MakeH1I(fname,name,4096,0,4096,title,2,3);
	
	sprintf(name, "SCI%s_E", count_title1[index]);  
	sprintf(title, "Sc%s Energy [ch]", count_title1[index]);
	hSCI_E[index] = MakeH1I(fname,name,4096,0,4096,title,2,3);
	
	sprintf(name, "SCI%s_Tx", count_title1[index]);  
	sprintf(title, "Sc%s t_lr [ch] TAC", count_title1[index]);
	hSCI_Tx[index] = MakeH1I(fname,name,4096,0,4096,title,2,3);
	
	sprintf(name, "SCI%s_X", count_title1[index]);  
	sprintf(title, "Sc%s x-pos [mm]", count_title1[index]);
	hSCI_X[index] = MakeH1I(fname,name,240,-120,120,title,2,3);
    
    }
    
    
    // ToF SC21-SC41
    sprintf(fname,"SCI/TOF/TOF(%d)",2);
    sprintf(name,"SCI_21_41_TofLL");
    hSCI_TofLL2 = MakeH1I(fname,name,1500,0,62000,"TAC SC41L-SC21L [ps]",2,3);
    
    sprintf(name,"SCI_21_41_TofRR");
    hSCI_TofRR2 = MakeH1I(fname,name,1500,0,62000,"TAC SC41R-SC21R [ps]",2,3);
    
    sprintf(name,"SCI_21_41_Tof2");
    hSCI_Tof2 = MakeH1I(fname,name,1000,0,62000,"TAC SC41-SC21 [ps] (pos.corr.)",2,3);
    
    
    // ToF SC21-SC42 changed on 03.07.2018 SB
    sprintf(fname,"SCI/TOF/TOF(%d)",3);
    sprintf(name,"SCI_21_42_TofLL");
    hSCI_TofLL3 = MakeH1I(fname,name,1500,0,62000,"TAC SC42L-SC21L [ps]",2,3);
    
    sprintf(name,"SCI_21_42_TofRR");
    hSCI_TofRR3 = MakeH1I(fname,name,1500,0,62000,"TAC SC42R-SC21R [ps]",2,3);
    
    sprintf(name,"SCI_21_42_Tof3");
    hSCI_Tof3 = MakeH1I(fname,name,1000,0,62000,"TAC SC42-SC21 [ps] (pos.corr.)",2,3);
    
    
    // ToF SC21-SC81 changed on 03.07.2018 SB
    sprintf(fname,"SCI/TOF/TOF(%d)",4);
    sprintf(name,"SCI_21_81_TofLL");
    hSCI_TofLL4 = MakeH1I(fname,name,1500,0,62000,"TAC SC81L-SC21L [ps]",2,3);
    
    sprintf(name,"SCI_21_81_TofRR");
    hSCI_TofRR4 = MakeH1I(fname,name,1500,0,62000,"TAC SC81R-SC21R [ps]",2,3);
    
    sprintf(name,"SCI_21_81_Tof4");
    hSCI_Tof4 = MakeH1I(fname,name,1000,0,62000,"TAC SC81-SC21 [ps] (pos.corr.)",2,3);
    
    sprintf(fname,"SCI/SCI%s/SCI%s",fext1[2],fext2[2]);
    sprintf(name, "SCI%s_dEx", count_title1[2]);  
    hSCI_dEx2 = MakeH2I(fname,name, 200,-100.,100.,200,0,100,
    "SC21 dE [ch]","SC21 x [mm]",2);
    
    sprintf(fname,"SCI/SCI%s/SCI%s",fext1[3],fext2[3]);
    sprintf(name, "SCI%s_dEx2del", count_title1[3]);  
    hSCI_dEx2del = MakeH2I(fname,name, 200,10,4000,200,-100,100,
    "SC21del dE [ch]","SC21 x [mm]",2);
    
    sprintf(fname,"SCI/SCI%s/SCI%s",fext1[5],fext2[5]);
    sprintf(name, "SCI%s_dETof2", count_title1[5]);  
    hSCI_dETof2 = MakeH2I(fname, name, 200,10,4000,200,0,62000,
    "SC41 dE [ch]", "ToF S41-S21 [ps]",2);
    
    sprintf(fname,"SCI/SCI%s/SCI%s",fext1[6],fext2[6]);
    sprintf(name, "SCI%s_dETof3", count_title1[6]);  
    hSCI_dETof3 = MakeH2I(fname, name, 200,10,4000,200,0,62000,
    "SC42 dE [ch]", "ToF S42-S21 [ps]",2);
    
    sprintf(fname,"SCI/SCI%s/SCI%s",fext1[10],fext2[10]);
    sprintf(name, "SCI%s_dETof4", count_title1[10]);  
    hSCI_dETof4 = MakeH2I(fname, name, 200,10,4000,200,0,62000,
    "SC81 dE [ch]", "ToF S81-S21 [ps]",2);
    
    sprintf(name, "SCI%s_dEx", count_title1[5]);  
    hSCI_dEx5 = MakeH2I(fname,name, 200,10,4000,200,-100,100,
    "SC41 dE [ch]","SC41 x [mm]",2);
    
    hSCI_dE24 = MakeH2I("SCI","SCI_dE21-41", 100,10,4000,100,10,4000,
    "SC21 dE","SC41 dE",2);   

}

void FRS_Histograms::Create_ID_Hist(){ 

    hID_dEToF = MakeH2I("ID","ID_dEToF", 2000, 00000.,70000.,400,0,4000,
    "tof S2-S4 Sci.Tof(2)", "Music_dE(1)", 2);
    
    
    hID_BRho[0] = MakeH1I("ID","ID_BRho0",5000,2.5,14.5,"BRho of 1. Stage [Tm]",2,6);
    hID_BRho[1] = MakeH1I("ID","ID_BRho1",5000,2.5,14.5,"BRho of 2. Stage [Tm]",2,6);
    
    hID_beta = MakeH1I("ID","ID_beta",2000,0,2000,"id.beta(2)*1000",2,6);
    
    //   hID_beta3 = MakeH1I("ID","ID_beta3",2000,0,2000,"id.beta(3)*1000",2,6);
    
    hID_AoQ = MakeH1I("ID","ID_AoQ",2000,1.4,3.0,"A/Q S2-S4",2,6);
    hID_AoQ_corr = MakeH1I("ID","ID_AoQ_corr",2000,1.4,3.0,"A/Q S2-S4",2,6);
    //   hID_Z = MakeH1I("ID",Form("ID_Z, gain=%f",music->e1_gain[0]),1000,10,93,"Z s2-s4",2,6);
    hID_Z = MakeH1I("ID","ID_Z",1000,0,93,"Z s2-s4",2,6);
    hID_ZmaxRange = MakeH1I("ID","ID_ZmaxRange",1000,0,400,"Z s2-s4",2,6);
    hID_Z2 = MakeH1I("ID","ID_Z2",1000,0,93,"Z2 s2-s4",2,6);
    hID_Z3 = MakeH1I("ID","ID_Z3",1000,10,93,"Z3 s2-s4",2,6);
    
    
    hID_DeltaBrho_AoQ = MakeH2I("ID","ID_DBrho_AoQ",300,1.4,2.5,500,0.,2.5,"A/Q s2-s4", "DeltaBrho ta-s2 s2-s4",2);
    hID_DeltaBrho_AoQzoom = MakeH2I("ID","ID_DBrho_AoQzoom",300,1.4,2.5,900,0.9,1.2,"A/Q s2-s4", "DeltaBrho ta-s2 s2-s4",2);
    
    hID_x2AoQ = MakeH2I("ID","ID_x2AoQ", 300,1.4,2.5, 200,-100.,100.,"A/Q s2-s4", "X at S2 [mm]", 2);
    
    hID_Z_AoQ = MakeH2I("ID","ID_Z_AoQ", 300,1.4,2.5, 400,1.,20.,"A/Q s2-s4", "Z s2-s4", 2); 
    
    
    hID_Z_AoQv2 = MakeH2I("ID","ID_Z_AoQv2", 300,1.4,2.5, 400,1.,20.,"A/Q s2-s4", "Z s2-s4", 2); 
    
    hID_Z_AoQ_zsame = MakeH2I("ID","ID_Z_AoQ_zsame", 600,1.4,2.5, 600,1.,30.,
    "Z1==Z2 A/Q s2-s4", "Z s2-s4", 2); 
    
    hID_x2x4 = MakeH2I("ID","ID_x2_x4",200,-100,100,200,-100,100,"x2 mm","x4 mm",2);
    
    hID_Z_AoQ_corr = MakeH2I("ID","ID_Z_AoQ_corr", 300,1.4,2.5, 300,1.,20.,
    "A/Q s2-s4", "Z s2-s4", 2); 
    
    //   hID_Z_AoQ_corrZoom = MakeH2I("ID","ID_Z_AoQ_corrZoom", 400,2.3,2.65, 300,75.,95.,
    //			"A/Q s2-s4", "Z s2-s4", 2);
    
    
    hID_Z2_AoQ = MakeH2I("ID","ID_Z2_AoQ", 250,1.4,2.5, 250,1.,20.,
    "A/Q s2-s4", "Z2 after SHT", 2);
    
    //   hID_Z3_AoQ = MakeH2I("ID","ID_Z3_AoQ", 250,1.8,2.8, 250,10.,93.,
    //			"A/Q s2-s4", "Z3 'old MUSIC'", 2);
    
    hID_Z_Z2 = MakeH2I("ID","ID_Z_Z2", 350,1,20, 350,1.,20.,
    "Z", "Z2", 2); 
    
    hID_Z_Z3 = MakeH2I("ID","ID_Z_Z3", 250,10,93, 250,10.,93.,
    "Z", "Z3 'old MUSIC'", 2); 
    
    hID_Z_dE2 = MakeH2I("ID","ID_Z_dE2", 250,1,30, 250,0.,4000.,
    "Z", "MUSIC2_dE", 2); 
    
    hID_Z_Sc21E = MakeH2I("ID","ID_Z_Sc21E", 300,0,25.,400,0,4000.,
    "Z s2-s4", "sqrt(Sc21_L*sC21_R)", 2); 
    
    hID_x4AoQ = MakeH2I("ID","ID_x4AoQ", 100,1.4,2.5, 100,-100.,100.,"A/Q s2-s4", "X at S4 [mm]", 2);
    
    hID_x4AoQ_zgate = MakeH2I("ID","ID_x4AoQ_zgate", 300,1.4,2.5, 200,-100.,100.,"A/Q s2-s4", "gate on Z    X at S4 [mm]", 2);
    
    hID_x2z = MakeH2I("ID","ID_x2z", 300,1.,30., 200,-100.,100., "Z s2-s4", "X at S2 [mm]", 2); 
    hID_x4z = MakeH2I("ID","ID_x4z", 300,1.,30., 200,-100.,100., "Z s2-s4", "X at S4 [mm]", 2); 
    
    hID_SC41dE_AoQ = MakeH2I("ID","ID_SC41dE_AoQ", 300,1.2,3.0, 800,0.,4000.,"A/Q s2-s4", "SC41 dE", 2);
    
    
    // For delta measurement
    hID_x4z55 = MakeH1I("ID_delta","ID_x4z7",600,-100,100,"x4 [mm]",2,6);
    hID_x4z53 = MakeH1I("ID_delta","ID_x4z5",600,-100,100,"x4 [mm]",2,6);
    hID_x2z55 = MakeH1I("ID_delta","ID_x2z7",600,-100,100,"x2 [mm]",2,6);
    hID_x2z53 = MakeH1I("ID_delta","ID_x2z5",600,-100,100,"x2 [mm]",2,6);
    
    
    hID_AoQ_ang_s4 = MakeH2I("ID","ID_AoQ_ang_s4", 500,-20.,20., 400,1.,3., "Angle s4", "AoQ", 2); 
    hID_AoQ_ang_s4corr = MakeH2I("ID","ID_AoQ_ang_s4corr", 500,-20.,20., 400,1.,3., "Angle s4", "AoQ", 2); 
    hID_Z_x_s4 = MakeH2I("ID","ID_Z_x_s4", 500,-100.,100., 300,0.,20., "X s4", "Z", 2); 
    hID_AoQ_Z_corr = MakeH2I("ID","ID_AoQ_Z_corr",600,1.6,2.8,600,40.,60., "AoQ", "Z", 2); 
    
    hID_E_Xs4 = MakeH2I("ID","ID_E_Xs4", 200,-100.,100., 400,0.,4000., "X s4 [mm]", "Delta E", 2); 
    
    hID_E_Xs2 = MakeH2I("ID","ID_E_Xs2", 200,-100.,100., 400,0.,4000., "X s2 [mm]", "Delta E", 2);
    
    hID_x2a2 = MakeH2I("ID", "ID_x2_a2", 200, -100., 100., 200, -100., 100., "X s2 [mm]", "AngleX s2 [mrad]", 2);
    hID_y2b2 = MakeH2I("ID", "ID_y2_b2", 200, -100., 100., 200, -100., 100., "Y s2 [mm]", "AngleY s2 [mrad]", 2);
    
    hID_x4a4 = MakeH2I("ID", "ID_x4_a4", 200, -100., 100., 200, -100., 100., "X s4 [mm]", "AngleX s4 [mrad]", 2);
    hID_y4b4 = MakeH2I("ID", "ID_y4_b4", 200, -100., 100., 200, -100., 100., "Y s4 [mm]", "AngleY s4 [mrad]", 2);
    
    //   hID_Z_Q = MakeH1I("ID","ID_Z_Q",2000,10.,93.,"Z s2-s4 gated on id_x2aoq(2)",2,6);
    
    hID_xz = MakeH2I("ID", "ID_x4z4", 52, 0., 5200., 200,-100.,100., "Z at S4 [mm]", "X at S4 [mm]", 2);   
    hID_yz = MakeH2I("ID", "ID_y4z4", 52, 0., 5200., 200,-100.,100., "Z at S4 [mm]", "Y at S4 [mm]", 2);   
    hID_xzc = MakeH2I("ID", "ID_x4z4c", 52, 0., 5200., 200,-100.,100., "Z at S4 [mm] cond", "X at S4 [mm]", 2);   
    hID_yzc = MakeH2I("ID", "ID_y4z4c", 52, 0., 5200., 200,-100.,100., "Z at S4 [mm] cond", "Y at S4 [mm]", 2);   
    
    char name[50], title[100];
    for(int i=0;i<6;++i)
    {
	sprintf(name,"ID_x4AoQ_x2AoQgate%d",i);
	hID_x4AoQ_x2AoQgate[i] = MakeH2I("ID", name, 300,1.,2.4, 200,-100.,100.,"A/Q s2-s4", "gate on Z    X at S4 [mm]", 2);
	
	sprintf(name,"ID_ZAoQ_x2AoQgate%d",i);
	hID_ZAoQ_x2AoQgate[i] = MakeH2I("ID", name, 300,1.,2.4, 400,30.,90.,"A/Q s2-s4", " Z music", 2);
    }
    
    
    for(int i=0;i<5;i++){
    
	sprintf(name,"ID_Z_AoQgate%d",i);  
	hID_Z_AoQgate[i] = MakeH2I("ID",name, 300,1.2,2.5, 400,20.,60.,"A/Q s2-s4", "Z s2-s4", 2); 
	
	sprintf(name,"ID_Z3_gate%d",i);
	char name_x_title[256];
	sprintf(name_x_title,"Z3 gate%d",i);
	hID_Z3_gate[i] = MakeH1I("ID",name,  500,1.,20.,name_x_title, 2, 6); 
	
	sprintf(name,"ID_dE3_gate%d",i);
	sprintf(name_x_title,"Average dE MUSIC3 gate%d",i);
	hID_dE3_gate[i] = MakeH1I("ID",name,4000,0.0,4000.0,name_x_title, 2, 6);
	
	
	sprintf(name,"ID_X4_gated%d",i);  
	sprintf(title,"X at S4 [mm] gated by poly ID_Z_AoQ%d",i);
	hID_x4c[i] = MakeH1I("ID",name,200,-100,100,title,2,6);
	
	sprintf(name,"ID_X2_gated%d",i);  
	sprintf(title,"X at S2 [mm] gated by poly ID_Z_AoQ%d",i);
	hID_x2c[i] = MakeH1I("ID",name,200,-100,100,title,2,6);
	
	sprintf(name,"ID_beta_gated%d",i);  
	sprintf(title,"beta at S4  gated by poly ID_Z_AoQ%d",i);
	hID_betac[i] = MakeH1I("ID",name,500,0.3,0.9,title,2,6);
	
	sprintf(name,"ID_brho_gated%d",i);  
	sprintf(title,"brho at S4  gated by poly ID_Z_AoQ%d",i);
	hID_brhoc[i] = MakeH1I("ID",name,500,3,9,title,2,6);
    
    }

}




//---------------------------------------------------------------
