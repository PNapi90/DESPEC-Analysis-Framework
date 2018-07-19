#include "TXRSAnlProc.h"

#include "TH1.h"
#include "TH2.h"
#include "TGo4WinCond.h"
#include "TGo4PolyCond.h"
#include "TGo4CondArray.h"

#include "TXRSAnlEvent.h"
#include "TXRSSortEvent.h"
#include "TXRSCalibrEvent.h"

#include "TXRSParameter.h"

#include <stdlib.h>

//#include "Go4Analysis/TGo4Analysis.h"


TXRSAnlProc::TXRSAnlProc() : TXRSBasicProc("FRSAnlProc")
{
  music = dynamic_cast<TMUSICParameter*>(GetParameter("MUSICPar"));
  sci   = dynamic_cast<TSCIParameter*>(GetParameter("SCIPar"));
  frs   = dynamic_cast<TXRSParameter*>(GetParameter("FRSPar"));
  id    = dynamic_cast<TIDParameter*>(GetParameter("IDPar"));
  mrtof = dynamic_cast<TMRTOFMSParameter*>(GetParameter("MRTOFMSPar"));
}

TXRSAnlProc::TXRSAnlProc(const char* name): TXRSBasicProc(name)
{ 

  music = dynamic_cast<TMUSICParameter*> (GetParameter("MUSICPar"));
  sci   = dynamic_cast<TSCIParameter*>(GetParameter("SCIPar"));
  frs   = dynamic_cast<TXRSParameter*>(GetParameter("FRSPar"));
  id    = dynamic_cast<TIDParameter*>(GetParameter("IDPar"));
  mrtof = dynamic_cast<TMRTOFMSParameter*>(GetParameter("MRTOFMSPar"));

  bDrawHist=kTRUE;
  //     printf("Start MUSIC \n");
  Create_MUSIC_Hist();
  //     printf("Start SCI \n");
  Create_SCI_Hist();
  //     printf("Start ID \n"); 
  Create_ID_Hist();
  //     printf("Start SI \n"); 
  //Create_SI_Hist();
  //     printf("Start CT \n"); 
  //Create_CT_Hist();
  Create_MRTOF_Hist();


  TGo4CondArray* conarr = MakeCondArray(0, "Integral", 5);
  if (ObjWasCreated())
    {
      ((*conarr)[0])->SetValues(0,10);
      ((*conarr)[1])->SetValues(20,30);
      ((*conarr)[2])->SetValues(40,50);
      ((*conarr)[3])->SetValues(60,70);
      ((*conarr)[4])->SetValues(80,90);
    }

}

TXRSAnlProc::~TXRSAnlProc()
{ }

//void TXRSAnlProc::FRSEventAnalysis(TXRSAnlEvent* poutevt)
Bool_t TXRSAnlProc::BuildEvent(TGo4EventElement* output)
{

  TXRSAnlEvent* poutevt = dynamic_cast<TXRSAnlEvent*>(output);
  if(poutevt==nullptr)
    return kFALSE;

  poutevt->SetValid(kTRUE);  // events always accepted

  bDrawHist=kTRUE;

  TXRSSortEvent *srt = dynamic_cast<TXRSSortEvent*> (GetInputEvent("Calibr"));
  TXRSCalibrEvent *clb = dynamic_cast<TXRSCalibrEvent*> (GetInputEvent());
  
  if ((srt==nullptr) || (clb==nullptr))
    return kFALSE;

  if(srt->EventFlag != clb->EventFlag)
    std::cout<<"E> AnlProc BuildEvent : EventFlag miss matched between Sort and Calib ! "<<srt->EventFlag <<" != "<< clb->EventFlag <<"\n";

  poutevt->EventFlag = srt->EventFlag;

  Procceed_MUSIC_Analysis(*srt, *clb, *poutevt);
  Procceed_SCI_Analysis(*srt, *clb, *poutevt);
  Procceed_ID_Analysis(*srt, *clb, *poutevt);
  Procceed_MRTOF_Analysis(*srt, *clb, *poutevt); 

  return kTRUE;
}

void TXRSAnlProc::Create_MRTOF_Hist()
{

  h_MRtof_Start = MakeH1F("MRTOF/Time","mrtof_start",1000,0,50,"microsec",2,6);
  h_MRtof_StopDelay = MakeH1F("MRTOF/Time","mrtof_stopDelay",1000,0,50,"microsec",2,6);
  h_MRtof_Stop = MakeH1F("MRTOF/Time","mrtof_stop",1000,0,50,"microsec",2,6);
  h_MRtof_tof = MakeH1F("MRTOF/Time","mrtof_tof",200000,0,200,"microsec",2,6);
  h_MRtof_status = MakeH1F("MRTOF/Time","mrtof_TimeStatus",10,0,10,"",2,6);
  
}

void TXRSAnlProc::Create_MUSIC_Hist()
{
   char name[80], xtitle[80]; 
   for(int i=0;i<8;i++)
     {
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


   for(int i=0;i<4;i++)
     {
       sprintf(name,"MUSIC3_E(%d)",i);  
       sprintf(xtitle,"dE MUSIC3(%d)",i);
       hMUSIC3_E[i] = MakeH1I("MUSIC/MUSIC(3)/E",name,4096,0,4096,xtitle,2,6);
       
       sprintf(name,"MUSIC3_dEdx_betagamma(%d)",i);  
       h_dEdx_betagamma[i] = MakeH2I("MUSIC/MUSIC(3)/BT",name,1000,0.4,1.4,1000,0,100,"betagamma","dE/dx (ch/cm)",1);

       sprintf(name,"MUSIC3_DiffdEdx_betagamma(%d)",i);  
       h_DiffdEdx_betagamma[i] = MakeH2I("MUSIC/MUSIC(3)/BT",name,1000,0.4,1.4,1000,-10,10,"betagamma","DiffdE/dx (ch/cm)",1);

       for(int k=0;k<5;++k)
	 {
	   
	   sprintf(name,"MUSIC3_z_AoQ_E(%d)gate(%d)",i,k);  
	   sprintf(xtitle,"dE MUSIC3(%d) zAoQgate(%d)",i,k);
	   hMUSIC3_z_AoQ_E[i][k] = MakeH1I("MUSIC/MUSIC(3)/E",name,4096,0,4096,xtitle,2,6);

	   sprintf(name,"MUSIC3_music_z_E(%d)gate(%d)",i,k);  
	   sprintf(xtitle,"dE MUSIC3(%d) gate(%d)",i,k);
	   hMUSIC3_music_z_E[i][k] = MakeH1I("MUSIC/MUSIC(3)/E",name,4096,0,4096,xtitle,2,6); 
	 }
       for(int k=0;k<6;++k)
	 {
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

    
   //   hMUSIC_pres[0] = MakeH1I("MUSIC/Pres","MUSIC_pres_0",3000,0,1500,"Pressure MUSIC 1 [mbar]",2,6);
   //   hMUSIC_pres[1] = MakeH1I("MUSIC/Pres","MUSIC_pres_1",3000,0,1500,"Pressure MUSIC 2 [mbar]",2,6);
        
   hMUSIC3_dEx = MakeH2I("MUSIC/MUSIC(3)/E","MUSIC3_dEx",100,-100,100,200,0,4096,
                         "Average x position in MUSIC3","dE MUSIC3 [channels]",2);

   h_dEdx_betagammaAll = MakeH2I("MUSIC/MUSIC(3)","MUSIC3_dEdx_betagammaAll",1000,0.4,1.4,1000,0,100,"betagamma","dE/dx (ch/cm)",1);
   h_dEdx_betagammaAllZoom = MakeH2I("MUSIC/MUSIC(3)","MUSIC3_dEdx_betagammaAllZoom",1000,0.4,1.4,1000,0,1,"betagamma","dE/dx (ch/cm)",1);
  
   hMUSIC3_dECOR = MakeH1I("MUSIC/MUSIC(3)/E","MUSIC3_dECOR",4000,0.5,4000.5,"dE MUSIC3 corrected for position",2,6);
   
   hMUSIC3_dExc = MakeH2I("MUSIC/MUSIC(3)","MUSIC3_dExc",100,-100,+100,200,0,4096,
			  "Average x position in MUSIC3", "dE MUSIC3 (3)  [channels]", 2);


   for(int i=0;i<8;i++)
     {
       sprintf(name,"Music1_E(%d)",i);
       cMusic1_E[i] = MakeWindowCond("MUSIC/MUSIC(1)/E",name, 10, 4086, hMUSIC1_E[i]->GetName());

        sprintf(name,"Music1_T(%d)",i);
       cMusic1_T[i] = MakeWindowCond("MUSIC/MUSIC(1)/T",name, 10, 4086, hMUSIC1_T[i]->GetName());

       sprintf(name,"Music2_E(%d)",i);
       cMusic2_E[i] = MakeWindowCond("MUSIC/MUSIC(2)/E",name, 10, 4086, hMUSIC2_E[i]->GetName());

       sprintf(name,"Music2_T(%d)",i);
       cMusic2_T[i] = MakeWindowCond("MUSIC/MUSIC(2)/T",name, 10, 4086, hMUSIC2_T[i]->GetName());
     }

   for(int i=0;i<4;i++)
     {
       sprintf(name,"Music3_E(%d)",i);
       cMusic3_E[i] = MakeWindowCond("MUSIC/MUSIC(3)/E",name, 10, 4086, hMUSIC3_E[i]->GetName());

       sprintf(name,"Music3_T(%d)",i);
       cMusic3_T[i] = MakeWindowCond("MUSIC/MUSIC(3)/T",name, 10, 4086, hMUSIC3_T[i]->GetName());
     }
   
   
   cMusic3_dec = MakeWindowCond("MUSIC/MUSIC 3","Music3_dec", 10., 4000., hMUSIC3_dECOR->GetName());
   
}

void TXRSAnlProc::Create_SCI_Hist()
{
  char fname[50], name[50], title[60];//, title2[60];
  
  const char *count_title1[12]={"(0:1)", "(1:1)", "(2:1)",
				"(2:2)", "(3:1)", "(4:1)",
				"(4:2)", "(4:3)", "(6:1)",
				"(6:2)", "(8:1)"};
  const char *fext1[12]={"0", "1", "2", "2", "3", "4", "4", "4", "6", "6", "8", "8"};
  const char *fext2[12]={"01", "11", "21", "22","31", "41",
			 "42", "43", "61",
			 "62", "81", "82"};
  
  for (int cnt = 0; cnt<6; cnt++) //changed from 3 to 6 04.07.2018
    {
      int index = 0;
      switch(cnt)
	{
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
     
     
      sprintf(name, "cSCI%s_L", count_title1[index]);
      cSCI_L[index] = MakeWindowCond(fname,name, 10, 4000, hSCI_L[index]->GetName());
     
      sprintf(name, "cSCI%s_R", count_title1[index]);
      cSCI_R[index] = MakeWindowCond(fname,name, 10, 4000, hSCI_R[index]->GetName());
     
      sprintf(name, "cSCI%s_E", count_title1[index]);
      cSCI_E[index] = MakeWindowCond(fname,name, 10, 4000, hSCI_E[index]->GetName());
     
      sprintf(name, "cSCI%s_Tx", count_title1[index]);
      cSCI_Tx[index] = MakeWindowCond(fname,name,200,4000, hSCI_Tx[index]->GetName());
     
      sprintf(name, "cSCI%s_X", count_title1[index]);
      cSCI_X[index] = MakeWindowCond(fname,name,-100,100, hSCI_X[index]->GetName());
     
    }

             
  // ToF SC21-SC41
  sprintf(fname,"SCI/TOF/TOF(%d)",2);
  sprintf(name,"SCI_21_41_TofLL");
  hSCI_TofLL2 = MakeH1I(fname,name,1500,0,62000,"TAC SC41L-SC21L [ps]",2,3);

  sprintf(name,"SCI_21_41_TofRR");
  hSCI_TofRR2 = MakeH1I(fname,name,1500,0,62000,"TAC SC41R-SC21R [ps]",2,3);

  sprintf(name,"SCI_21_41_Tof2");
  hSCI_Tof2 = MakeH1I(fname,name,1000,0,62000,"TAC SC41-SC21 [ps] (pos.corr.)",2,3);

  sprintf(name, "cSCI%d_TofLL", 2);
  cSCI_TofLL2 = MakeWindowCond(fname,name, 2500, 80000, hSCI_TofLL2->GetName());

  sprintf(name, "cSCI%d_TofRR", 2);
  cSCI_TofRR2 = MakeWindowCond(fname,name, 2500, 80000, hSCI_TofRR2->GetName());

  // ToF SC21-SC42 changed on 03.07.2018 SB
  sprintf(fname,"SCI/TOF/TOF(%d)",3);
  sprintf(name,"SCI_21_42_TofLL");
  hSCI_TofLL3 = MakeH1I(fname,name,1500,0,62000,"TAC SC42L-SC21L [ps]",2,3);

  sprintf(name,"SCI_21_42_TofRR");
  hSCI_TofRR3 = MakeH1I(fname,name,1500,0,62000,"TAC SC42R-SC21R [ps]",2,3);

  sprintf(name,"SCI_21_42_Tof3");
  hSCI_Tof3 = MakeH1I(fname,name,1000,0,62000,"TAC SC42-SC21 [ps] (pos.corr.)",2,3);

  sprintf(name, "cSCI%d_TofLL", 3);
  cSCI_TofLL3 = MakeWindowCond(fname,name, 2500, 80000, hSCI_TofLL3->GetName());

  sprintf(name, "cSCI%d_TofRR", 3);
  cSCI_TofRR3 = MakeWindowCond(fname,name, 2500, 80000, hSCI_TofRR3->GetName());


  // ToF SC21-SC81 changed on 03.07.2018 SB
  sprintf(fname,"SCI/TOF/TOF(%d)",4);
  sprintf(name,"SCI_21_81_TofLL");
  hSCI_TofLL4 = MakeH1I(fname,name,1500,0,62000,"TAC SC81L-SC21L [ps]",2,3);

  sprintf(name,"SCI_21_81_TofRR");
  hSCI_TofRR4 = MakeH1I(fname,name,1500,0,62000,"TAC SC81R-SC21R [ps]",2,3);

  sprintf(name,"SCI_21_81_Tof4");
  hSCI_Tof4 = MakeH1I(fname,name,1000,0,62000,"TAC SC81-SC21 [ps] (pos.corr.)",2,3);

  sprintf(name, "cSCI%d_TofLL", 4);
  cSCI_TofLL4 = MakeWindowCond(fname,name, 2500, 80000, hSCI_TofLL4->GetName());

  sprintf(name, "cSCI%d_TofRR", 4);
  cSCI_TofRR4 = MakeWindowCond(fname,name, 2500, 80000, hSCI_TofRR4->GetName());


  
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
  
  Float_t cSCI_detof_points[4][2] = 
    {{30000.,     0.},
     {30000.,  4096.},
     {40000.,  4096.},
     {40000.,     0.}};
  cSCI_detof = MakePolyCond("SCI","SCI_detof",4,cSCI_detof_points, hSCI_dETof2->GetName());
  
}

void TXRSAnlProc::Create_ID_Hist()
{
  //TMUSICParameter* music = (TMUSICParameter*) GetParameter("MUSICPar");
  //TXRSParameter* frs = (TXRSParameter*) GetParameter("FRSPar");    
  
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
  
  cID_x2 = MakeWindowCond("ID","cID_x2", -100.0, 100.0);
  cID_x4 = MakeWindowCond("ID","cID_x4", -100.0, 100.0);
  
  Float_t cID_dEToF_points[4][2] = 
    {{    0.,    0.},
     {    0., 4000.},
     {40000., 4000.},
     {40000.,    0.}}; 
  cID_dEToF = MakePolyCond("ID","cID_dEToF",4, cID_dEToF_points, hID_dEToF->GetName());
  
  int num_ID_x2AoQ[6] = {5, 5, 5, 5, 5, 5};
  Float_t init_ID_x2AoQ[6][5][2] =
     {
       {{  2.009 ,   87.121 },
	{  2.0113,  -82.0475},
	{  2.3872,  -82.3590},
	{  2.3872,  -82.3590},
	{  2.3886,   85.8756}
       },
       {{  2.009 ,   87.121 },
	{  2.0113,  -82.0475},
	{  2.3872,  -82.3590},
	{  2.3872,  -82.3590},
	{  2.3886,   85.8756}
       },
       {{  2.009 ,   87.121 },
	{  2.0113,  -82.0475},
	{  2.3872,  -82.3590},
	{  2.3872,  -82.3590},
	{  2.3886,   85.8756}
       },
       {{  2.009 ,   87.121 },
	{  2.0113,  -82.0475},
	{  2.3872,  -82.3590},
	{  2.3872,  -82.3590},
	{  2.3886,   85.8756}
       },
       {{  2.009 ,   87.121 },
	{  2.0113,  -82.0475},
	{  2.3872,  -82.3590},
	{  2.3872,  -82.3590},
	{  2.3886,   85.8756}
       },
       {{  2.009 ,   87.121 },
	{  2.0113,  -82.0475},
	{  2.3872,  -82.3590},
	{  2.3872,  -82.3590},
	{  2.3886,   85.8756}
       }
     };
  

  // Float_t cID_x2AoQ_points[4][2] = 
  //   {{  2.009 ,   87.121 },
  //    {  2.0113,  -82.0475},
  //    {  2.3872,  -82.3590},
  //    {  2.3886,   85.8756}};
  char name[50], title[100];
  for(int i=0;i<6;++i)
    {
      sprintf(name,"cID_x2AoQ%d",i);
      cID_x2AoQ[i] = MakePolyCond("ID",name,num_ID_x2AoQ[i],init_ID_x2AoQ[i], hID_x2AoQ->GetName());
      //cID_Z_AoQ[i] = MakePolyCond("ID", name, num_ID_Z_AoQ[i], init_ID_Z_AoQ[i], hID_Z_AoQ->GetName());
      sprintf(name,"ID_x4AoQ_x2AoQgate%d",i);
      hID_x4AoQ_x2AoQgate[i] = MakeH2I("ID", name, 300,1.,2.4, 200,-100.,100.,"A/Q s2-s4", "gate on Z    X at S4 [mm]", 2);

      sprintf(name,"ID_ZAoQ_x2AoQgate%d",i);
      hID_ZAoQ_x2AoQgate[i] = MakeH2I("ID", name, 300,1.,2.4, 400,30.,90.,"A/Q s2-s4", " Z music", 2);
      //hID_Z_AoQ = MakeH2I("ID","ID_Z_AoQ", 300,1.2,2.8, 400,40.,95.,"A/Q s2-s4", "Z s2-s4", 2); 
    }


  int num_ID_Z_AoQ[5] = {5, 5, 5, 5, 5};
  Float_t init_ID_Z_AoQ[5][5][2] =
     {
       // ID_Z_AOQ(1)
       /* 213Fr setting */
       {{2.45205,        86.7875},
        {2.46124,        86.7458},
        {2.46202,        88.1625},
        {2.4496,        88.1417},
        {2.45205,        86.7875}},
       /* 223Th setting */
       //      {{2.48025,        89.3017},
       //      {2.49183,        89.2808},
       //      {2.49122,        90.7808},
       //      {2.48005,        90.8122},
       //      {2.48025,        89.3017}},
       /* 70 Ni @ 70Ni setting shifted by 10*/
       {{2.482,        38.0000},
        {2.508,        38.5000},
        {2.522,        38.0000},
        {2.489,        37.5000},
        {2.488,        37.5000}},
       // ID_Z_AOQ(3) 
       {{2.25029,      51.22740},
        {2.27401,      50.88124},
        {2.30980,      51.80895},
        {2.28475,      51.93358},
        {2.25208,      52.07204}},
       //ID_Z_AOQ(4) 
       {{2.25222,      49.96984},
        {2.24048,      49.85215},
        {2.24764,      49.28444},
        {2.25967,      49.38137},
        {2.26654,      49.68599}},
       // ID_Z_AOQ(5)
       {{2.55977,      52.6315},
        {2.55247,      52.293},
        {2.57298,      51.8529},
        {2.58529,      52.1914},
        {2.57936,      52.6315}} };
  
  for(int i=0;i<5;i++)
    {
      
      sprintf(name,"ID_Z_AoQgate%d",i);  
      hID_Z_AoQgate[i] = MakeH2I("ID",name, 300,1.2,2.5, 400,20.,60.,"A/Q s2-s4", "Z s2-s4", 2); 
     
      sprintf(name,"ID_Z3_gate%d",i);
      char name_x_title[256];
      sprintf(name_x_title,"Z3 gate%d",i);
      hID_Z3_gate[i] = MakeH1I("ID",name,  500,1.,20.,name_x_title, 2, 6); 

      sprintf(name,"ID_dE3_gate%d",i);
      sprintf(name_x_title,"Average dE MUSIC3 gate%d",i);
      hID_dE3_gate[i] = MakeH1I("ID",name,4000,0.0,4000.0,name_x_title, 2, 6);
    /* 
       sprintf(name,"ID_x_silicon1c%d",i);  
       sprintf(title,"X at Si tracker 1 [mm] gated by poly ID_Z_AoQ%d",i);
       hID_x_target1c[i] = MakeH1I("ID",name,200,-100,100,title,2,6);  
  
      sprintf(name,"ID_x_OTPCc%d",i);  
      sprintf(title,"X at OTPC [mm] gated by poly ID_Z_AoQ%d",i);
      hID_x_target2c[i] = MakeH1I("ID",name,200,-100,100,title,2,6);  
        
      sprintf(name,"ID_y_silicon1c%d",i);  
      sprintf(title,"Y at Si tracker 1 [mm] gated by poly ID_Z_AoQ%d",i);
      hID_y_target1c[i] = MakeH1I("ID",name,200,-100,100,title,2,6);  
    
      sprintf(name,"ID_y_OTPCc%d",i);  
      sprintf(title,"Y at OTPC [mm] gated by poly ID_Z_AoQ%d",i);
      hID_y_target2c[i] = MakeH1I("ID",name,200,-100,100,title,2,6);  
    */

      sprintf(name,"ID_X4_gated%d",i);  
      sprintf(title,"X at S4 [mm] gated by poly ID_Z_AoQ%d",i);
      hID_x4c[i] = MakeH1I("ID",name,200,-100,100,title,2,6);
      
      sprintf(name,"ID_X2_gated%d",i);  
      sprintf(title,"X at S2 [mm] gated by poly ID_Z_AoQ%d",i);
      hID_x2c[i] = MakeH1I("ID",name,200,-100,100,title,2,6);

      //      sprintf(name,"ID_target2c%d",i);  
      //      sprintf(title,"X at target2 [mm] gated by poly ID_Z_AoQ%d",i);
      //      hID_target2c[i] = MakeH1I("ID",name,200,-100,100,title,2,6); 
      
 
      sprintf(name,"ID_beta_gated%d",i);  
      sprintf(title,"beta at S4  gated by poly ID_Z_AoQ%d",i);
      hID_betac[i] = MakeH1I("ID",name,500,0.3,0.9,title,2,6);

      sprintf(name,"ID_brho_gated%d",i);  
      sprintf(title,"brho at S4  gated by poly ID_Z_AoQ%d",i);
      hID_brhoc[i] = MakeH1I("ID",name,500,3,9,title,2,6);

      sprintf(name,"cID_Z_AoQ%d",i);
      cID_Z_AoQ[i] = MakePolyCond("ID", name, num_ID_Z_AoQ[i], init_ID_Z_AoQ[i], hID_Z_AoQ->GetName());
  

      sprintf(name,"cID_x4AoQ_Z(%d)",i);
      Float_t cID_x4AoQ_Z_points[5][2] = 
	{{2.1, -90},
	 {2.1,  90},
	 {2.5,  90}, 
	 {2.5, -90},
	 {2.1, -90},
	};
      cID_x4AoQ_Z[i] = MakePolyCond("ID",name, 5, cID_x4AoQ_Z_points, hID_x4AoQ->GetName());
     
      sprintf(name,"cID_Z_Z%d",i);
      cID_Z_Z[i] = MakeWindowCond("ID",name,2.,40.,hID_Z->GetName());
    }
   
 }
// void TXRSAnlProc::Create_SI_Hist()
// { }
// void TXRSAnlProc::Create_CT_Hist()
//{ }

void TXRSAnlProc::Procceed_MUSIC_Analysis(TXRSSortEvent& srt, TXRSCalibrEvent& clb, TXRSAnlEvent& tgt) 
{
  
  tgt.music1_anodes_cnt = 0;  
  tgt.music2_anodes_cnt = 0;  
  tgt.music3_anodes_cnt = 0;


   // Munich MUSIC 
  
  for (int i=0;i<8;i++)
    {
      /* 8 anodes of TUM MUSIC */		
      /****** first MUSIC ***** threshold changed to 5, 9/8/2012 **/
      if ( srt.music_e1[i] > 5)
	{ 
	  if(bDrawHist) 
	    hMUSIC1_E[i]->Fill(srt.music_e1[i]);

	  tgt.music_b_e1[i] = cMusic1_E[i]->Test(srt.music_e1[i]);

	  if (tgt.music_b_e1[i])
	    tgt.music1_anodes_cnt++;
	}

        if (srt.music_t1[i] > 0)
	{ 
	  if(bDrawHist) 
	    hMUSIC1_T[i]->Fill(srt.music_t1[i]);     
	  tgt.music_b_t1[i] = cMusic1_T[i]->Test(srt.music_t1[i]);
	}

       
      hMUSIC1_dE1dE2->Fill(srt.music_e1[0],srt.music_e1[1]);

      //       /****** second MUSIC *****/
      
      if ( srt.music_e2[i] > 5)
	{ 
	  if(bDrawHist) 
	    hMUSIC2_E[i]->Fill(srt.music_e2[i]);
	  tgt.music_b_e2[i] = cMusic2_E[i]->Test(srt.music_e2[i]);
	  if (tgt.music_b_e2[i]) tgt.music2_anodes_cnt++;
	}

      if (srt.music_t2[i] > 0)
	{ 
	  if(bDrawHist) 
	    hMUSIC2_T[i]->Fill(srt.music_t2[i]);     
	  tgt.music_b_t2[i] = cMusic2_T[i]->Test(srt.music_t2[i]);
	}
    }

  for (int i=0;i<4;i++)
    {
      /* 4 anodes of MUSIC OLD */		
      /****** third MUSIC *****/
      if ( srt.music_e3[i] > 0)
	{ 
	  if(bDrawHist) 
	    hMUSIC3_E[i]->Fill(srt.music_e3[i]);

	  tgt.music_b_e3[i] = cMusic3_E[i]->Test(srt.music_e3[i]);
	  if (tgt.music_b_e3[i])
	    tgt.music3_anodes_cnt++;
	}
      if (srt.music_t3[i] > 0)
	{ 
	  if(bDrawHist) 
	    hMUSIC3_T[i]->Fill(srt.music_t3[i]);     
	  tgt.music_b_t3[i] = cMusic3_T[i]->Test(srt.music_t3[i]);
	}
    }
  
  // calculate truncated dE from 8 anodes, Munich MUSIC 
  
  if (tgt.music1_anodes_cnt == 8)
    {
      /* "quick-n-dirty solution according to Klaus:   */
      //      Float_t r1 = (srt.music_e1[0] - music->e1_off[0])*(srt.music_e1[1] - music->e1_off[1]);
      //      Float_t r2 = (srt.music_e1[2] - music->e1_off[2])*(srt.music_e1[3] - music->e1_off[3]);
      //      Float_t r3 = (srt.music_e1[4] - music->e1_off[4])*(srt.music_e1[5] - music->e1_off[5]);
      //      Float_t r4 = (srt.music_e1[6] - music->e1_off[6])*(srt.music_e1[7] - music->e1_off[7]);

      Float_t r1 = ((srt.music_e1[0])*music->e1_gain[0] + music->e1_off[0])*((srt.music_e1[1])*music->e1_gain[1] + music->e1_off[1]);
      Float_t r2 = ((srt.music_e1[2])*music->e1_gain[2] + music->e1_off[2])*((srt.music_e1[3])*music->e1_gain[3] + music->e1_off[3]);
      Float_t r3 = ((srt.music_e1[4])*music->e1_gain[4] + music->e1_off[4])*((srt.music_e1[5])*music->e1_gain[5] + music->e1_off[5]);
      Float_t r4 = ((srt.music_e1[6])*music->e1_gain[6] + music->e1_off[6])*((srt.music_e1[7])*music->e1_gain[7] + music->e1_off[7]);

      if ( (r1 > 0) && (r2 > 0) && (r3 > 0) && (r4 > 0) )
	{
	  tgt.b_de1 = kTRUE;
	  tgt.de[0] = sqrt( sqrt( sqrt(r1) * sqrt(r2) ) * sqrt( sqrt(r3) * sqrt(r4) ) );
	  tgt.de_cor[0] = tgt.de[0];
	  if(bDrawHist) 
	    hMUSIC1_dE->Fill(tgt.de[0]);
	}  
    }

       
  if (tgt.music2_anodes_cnt == 8)
    {
      
      Float_t r1 = ((srt.music_e2[0])*music->e2_gain[0] + music->e2_off[0])*((srt.music_e2[1])*music->e2_gain[1] + music->e2_off[1]);
      Float_t r2 = ((srt.music_e2[2])*music->e2_gain[2] + music->e2_off[2])*((srt.music_e2[3])*music->e2_gain[3] + music->e2_off[3]);
      Float_t r3 = ((srt.music_e2[4])*music->e2_gain[4] + music->e2_off[4])*((srt.music_e2[5])*music->e2_gain[5] + music->e2_off[5]);
      Float_t r4 = ((srt.music_e2[6])*music->e2_gain[6] + music->e2_off[6])*((srt.music_e2[7])*music->e2_gain[7] + music->e2_off[7]);

      if ( (r1 > 0) && (r2 > 0) && (r3 > 0) && (r4 > 0) )
	{
	  tgt.b_de2 = kTRUE;
	  tgt.de[1] = sqrt( sqrt( sqrt(r1) * sqrt(r2) ) * sqrt( sqrt(r3) * sqrt(r4) ) );
	  tgt.de_cor[1] = tgt.de[1];
	  if(bDrawHist) 
	    hMUSIC2_dE->Fill(tgt.de[1]);
	}  
    }
       

  if (tgt.music3_anodes_cnt == 4)
    {         // OLD MUSIC
      
      Float_t r1 = ((srt.music_e3[0])*music->e3_gain[0] + music->e3_off[0])*((srt.music_e3[1])*music->e3_gain[1] + music->e3_off[1]);
      Float_t r2 = ((srt.music_e3[2])*music->e3_gain[2] + music->e3_off[2])*((srt.music_e3[3])*music->e3_gain[3] + music->e3_off[3]);
      //Float_t r3 = ((srt.music_e3[4])*music->e3_gain[4] + music->e3_off[4])*((srt.music_e3[5])*music->e3_gain[5] + music->e3_off[5]);  //added on 22.05.2018 SB
      //Float_t r4 = ((srt.music_e3[6])*music->e3_gain[6] + music->e3_off[6])*((srt.music_e3[7])*music->e3_gain[7] + music->e3_off[7]);  //added on 22.05.2018 SB
      
      if ( (r1 > 0) && (r2 > 0) )
	{
	  tgt.b_de3 = kTRUE;
	  tgt.de[2] = sqrt( sqrt(r1) * sqrt(r2) ) ;  // corrrected JSW 19.03.2010
	  //tgt.de[2] = sqrt( sqrt( sqrt(r1) * sqrt(r2) ) * sqrt( sqrt(r3) * sqrt(r4) ) );   //changed on 22.05.2018 SB
	  tgt.de_cor[2] = tgt.de[2];
	  if(bDrawHist)
	    hMUSIC3_dE->Fill(tgt.de[2]);
	} 

      if (tgt.music_b_t3[0] && tgt.music_b_t3[1] && tgt.music_b_t3[2] && tgt.music_b_t3[3]) 
	tgt.b_dt3 = kTRUE;
    


	hMUSIC1_MUSIC2->Fill(tgt.de[0],tgt.de[1]);

      /* Position (X) correction by TPC */       //TO DO!!!
    


      //if(!music->b_selfcorr1 && tgt.b_de3) {
      //if(clb.b_mw_xsum[4] && clb.b_mw_xsum[5] && tgt.b_de3) {
      if(tgt.b_de3 && clb.b_tpc_xy[4]&&clb.b_tpc_xy[5])
	{
	  Float_t p1 = clb.music1_x1;
	  Float_t p2 = clb.music1_x2;
	  Float_t p3 = clb.music1_x3;
	  Float_t p4 = clb.music1_x4;

	  //Float_t p5 = clb.music1_x5; //added on 22.05.2018 SB
	  //Float_t p6 = clb.music1_x6; //added on 22.05.2018 SB
	  //Float_t p7 = clb.music1_x7; //added on 22.05.2018 SB
	  //Float_t p8 = clb.music1_x8; //added on 22.05.2018 SB
	  
	  tgt.x1_mean = (p1+p2+p3+p4)/4.;	// Mean position
	  //tgt.x1_mean = (p1+p2+p3+p4+p5+p6+p7+p8)/8.;	// Mean position  //added on 22.05.2018 SB
 
	  if(bDrawHist) 
	    hMUSIC3_dEx->Fill(tgt.x1_mean, tgt.de[2]);
	  
	  Float_t power = 1., Corr = 0.;
	  for(int i=0;i<4;i++) {
	    Corr += music->pos_a1[i] * power;
	    power *= tgt.x1_mean;  
	  }
      
	  if (Corr!=0) {
	    Corr = music->pos_a1[0] / Corr;
	    tgt.de_cor[2] = tgt.de[2] * Corr;
	  }
	  
	  if(bDrawHist) 
	    hMUSIC3_dExc->Fill(tgt.x1_mean, tgt.de_cor[2]);
	}
	//}
      

      /* Special gate on corrected music for cleaning x2 vs. AoQ spectrum */
      tgt.b_decor = cMusic3_dec->Test(tgt.de_cor[2]);
      
      if(tgt.b_de3)
	if(bDrawHist) 
       	hMUSIC3_dECOR->Fill(tgt.de_cor[2]);
    
    }

 }


Float_t rand0_5() 
{
  return rand()*1./RAND_MAX;// - 0.5;   
}

void TXRSAnlProc::Procceed_SCI_Analysis(TXRSSortEvent& srt, TXRSCalibrEvent& clb, TXRSAnlEvent& tgt) 
{
        /*-------------------------------------------------------------------------*/
 	/* focus index: detector number                  tof index  tof path       */
 	/*       0:     Sc01                                0:     TA - S1         */
 	/*       1:     Sc11                                1:     S1 - S2         */
 	/*       2:     Sc21                                2:     S2 - S41        */
 	/*       3:     Sc21                                3:     S2 - S42        */
 	/*       4:     Sc31                                4:     S2 - 81         */
 	/*       5:     Sc41                                5:     S2 - E1         */
	/*                                                                         */
 	/*       6:     Sc42                              tof index not used up to */
 	/*       7:     Sc43 (previously Sc51)             now, only separate      */
 	/*       8:     Sc61                              variables for S2-S41 and */
 	/*       9:     ScE1 (ESR)                                S2-S42           */
 	/*      10:     Sc81                                                       */
 	/*      11:     Sc82                                                       */
 	/*-------------------------------------------------------------------------*/
    
    

  /*  Raw data  */
   tgt.sci_l[2] = srt.de_21l;  /* 21L         */
   tgt.sci_r[2] = srt.de_21r;  /* 21R         */
   tgt.sci_tx[2] = srt.dt_21l_21r + rand0_5();

   tgt.sci_l[5] = srt.de_41l;  /* 41L         */
   tgt.sci_r[5] = srt.de_41r;  /* 41R         */
   tgt.sci_tx[5] = srt.dt_41l_41r + rand0_5();
  
   tgt.sci_l[6] = srt.de_42l;  /* 42L         */
   tgt.sci_r[6] = srt.de_42r;  /* 42R         */
   tgt.sci_tx[6] = srt.dt_42l_42r + rand0_5();

   tgt.sci_l[7] = srt.de_43l;  /* 43L         */
   tgt.sci_r[7] = srt.de_43r;  /* 43R         */
   tgt.sci_tx[7] = srt.dt_43l_43r + rand0_5();

   tgt.sci_l[10] = srt.de_81l; /* 81L         */
   tgt.sci_r[10] = srt.de_81r; /* 81R         */
   tgt.sci_tx[10] = srt.dt_81l_81r + rand0_5();
   

   for (int cnt=0;cnt<6;cnt++) // 
     {
       int idx = 0 ;
       //int mw_idx = 0;
       //Float_t mwx = 0;
       switch(cnt)
	 {
	 case 0:        /* SC21 */
	   idx = 2; 
	   //mw_idx = 2;
	   //mwx = clb.sc21_x;
	   break;    
	 case 1:        /* SC21 delayed */
	   idx = 3; 
	   //mw_idx = 2;
	   //mwx = clb.sc21_x;
	   break;    
	 case 2:        /* SC41 */
	   idx = 5; 
	   //mw_idx = 5;
	   //mwx = clb.tpc_sc41_x;
	   break;    
	 case 3:        /* SC42 */
           idx = 6;
	   break;
	 case 4:
	   idx = 7;     /* SC43 */
	   break;
	 case 5:
	   idx = 10;    /* SC81 */
	   break;
	 default: idx = 2;
	 }   
     
       // raw spectra 
       tgt.sci_b_l[idx] = cSCI_L[idx]->Test(tgt.sci_l[idx]);
       tgt.sci_b_r[idx] = cSCI_R[idx]->Test(tgt.sci_r[idx]);

       if(bDrawHist)
	 {
	   hSCI_L[idx]->Fill(tgt.sci_l[idx]);   
	   hSCI_R[idx]->Fill(tgt.sci_r[idx]);  
	 }
     
       if(tgt.sci_b_l[idx] && tgt.sci_b_r[idx])
	 {     
	   tgt.sci_e[idx] = sqrt( (tgt.sci_l[idx] - sci->le_a[0][idx]) * sci->le_a[1][idx] 
				  * (tgt.sci_r[idx] - sci->re_a[0][idx]) * sci->re_a[1][idx]);
	   
	   tgt.sci_b_e[idx] = cSCI_E[idx]->Test(tgt.sci_e[idx]);
	   if(bDrawHist) 
	     hSCI_E[idx]->Fill(tgt.sci_e[idx]);
	 }


       /*   Position in X direction:   */
       tgt.sci_b_tx[idx] = cSCI_Tx[idx]->Test(tgt.sci_tx[idx]);
       if (tgt.sci_b_tx[idx])
	 {
	   if(bDrawHist)
	     hSCI_Tx[idx]->Fill(tgt.sci_tx[idx]);    

	   /* mm-calibrated     */
	   Float_t R = tgt.sci_tx[idx] ;//+ rand0_5(); 
	   
	   Float_t power = 1., sum = 0.;
	   for(int i=0;i<7;i++)
	     {
	       sum += sci->x_a[i][idx] * power;
	       power *= R;  
	     }
	   
	   tgt.sci_x[idx] = sum;
	   
	   tgt.sci_b_x[idx] = cSCI_X[idx]->Test(tgt.sci_x[idx]);
	   if(bDrawHist) 
	     hSCI_X[idx]->Fill(tgt.sci_x[idx]);
	 }

     } // end of loop for indices 2,3,5,6,7,10   


   /***  Scintillator Tof  spectra ***/

   /* S21 - S41 */ 

   /*  Calibrated tof  */
   tgt.sci_tofll2 = srt.dt_21l_41l*sci->tac_factor[2] - sci->tac_off[2] ;   /* S41L- S21L */
   tgt.sci_tofrr2 = srt.dt_21r_41r*sci->tac_factor[3] - sci->tac_off[3] ;   /* S41R- S21R */
   //        tgt.sci_tofll2 = srt.dt_21l_41l;  
   //        tgt.sci_tofrr2 = srt.dt_21r_41r;
   //     std::cout <<" tac_factor2 "<<sci->tac_factor[2]<<std::endl; 
   //     std::cout <<" tac_factor3 "<<sci->tac_factor[3]<<std::endl;  
   if(bDrawHist)
     {
       //     std::cout <<" TofLL2 "<<tgt.sci_tofll2<<std::endl;
       //     std::cout <<" TofRR2 "<<tgt.sci_tofrr2<<std::endl;
       hSCI_TofLL2->Fill(tgt.sci_tofll2);
       hSCI_TofRR2->Fill(tgt.sci_tofrr2);
     }
  
   tgt.sci_b_tofll2 = cSCI_TofLL2->Test(tgt.sci_tofll2);
   tgt.sci_b_tofrr2 = cSCI_TofRR2->Test(tgt.sci_tofrr2);

   /* sum of Tof_LL and Tof_RR corrects for position in stop/start scint.      */
   if (tgt.sci_b_tofll2 && tgt.sci_b_tofrr2)
     {      /* TOF SC41 - SC21 [ps]  */
       tgt.sci_tof2 =  (sci->tof_bll2 * tgt.sci_tofll2 + sci->tof_a2 
			+ sci->tof_brr2 * tgt.sci_tofrr2)/2.0 ;
      
       if(bDrawHist) 
	 hSCI_Tof2->Fill(tgt.sci_tof2);
     }
 
   /*  
    else if (tgt.sci_b_tofll2) {
    tgt.sci_tof2 =  (sci->tof_bll2 * tgt.sci_tofll2 + sci->tof_a2) ;
    
    hSCI_Tof2->Fill(tgt.sci_tof2);
    }
    else if (tgt.sci_b_tofrr2) {
    tgt.sci_tof2 =  (sci->tof_a2 + sci->tof_brr2 * tgt.sci_tofrr2);
    
    hSCI_Tof2->Fill(tgt.sci_tof2);
    }
   */


   /***  Scintillator Tof  spectra with SC21-SC42 (as a backup) ***/ //added on 03.07.2018 SB

   //  Calibrated tof  
   tgt.sci_tofll3 = srt.dt_42l_21l*sci->tac_factor[5] - sci->tac_off[5] ;   // S42L- S21L 
   tgt.sci_tofrr3 = srt.dt_42r_21r*sci->tac_factor[6] - sci->tac_off[6] ;   // S42R- S21R
   //     std::cout <<" tac_factor5 "<<sci->tac_factor[5]<<std::endl; 
   //     std::cout <<" tac_factor6 "<<sci->tac_factor[6]<<std::endl;  
   if(bDrawHist)
     {
       //     std::cout <<" TofLL3 "<<tgt.sci_tofll3<<std::endl;
       //     std::cout <<" TofRR3 "<<tgt.sci_tofrr3<<std::endl;
       hSCI_TofLL3->Fill(tgt.sci_tofll3);
       hSCI_TofRR3->Fill(tgt.sci_tofrr3);
     }
  
   tgt.sci_b_tofll3 = cSCI_TofLL3->Test(tgt.sci_tofll3);
   tgt.sci_b_tofrr3 = cSCI_TofRR3->Test(tgt.sci_tofrr3);

   // sum of Tof_LL and Tof_RR corrects for position in stop/start scint.      
   if (tgt.sci_b_tofll3 && tgt.sci_b_tofrr3)
     {      // TOF SC42 - SC21 [ps]
       tgt.sci_tof3 =  (sci->tof_bll3 * tgt.sci_tofll3 + sci->tof_a3 
			+ sci->tof_brr3 * tgt.sci_tofrr3)/2.0 ;
      
       if(bDrawHist) 
	 hSCI_Tof3->Fill(tgt.sci_tof3);
     }


   /***  Scintillator Tof  spectra with SC21-SC81 ***/ //added on 03.07.2018 SB

   //  Calibrated tof  
   tgt.sci_tofll4 = srt.dt_21l_81l*sci->tac_factor[9] - sci->tac_off[9] ;   // S81L- S21L 
   tgt.sci_tofrr4 = srt.dt_21r_81r*sci->tac_factor[10] - sci->tac_off[10] ;   // S81R- S21R
   if(bDrawHist)
     {
       hSCI_TofLL4->Fill(tgt.sci_tofll4);
       hSCI_TofRR4->Fill(tgt.sci_tofrr4);
     }
  
   tgt.sci_b_tofll4 = cSCI_TofLL4->Test(tgt.sci_tofll4);
   tgt.sci_b_tofrr4 = cSCI_TofRR4->Test(tgt.sci_tofrr4);

   // sum of Tof_LL and Tof_RR corrects for position in stop/start scint.      
   if (tgt.sci_b_tofll4 && tgt.sci_b_tofrr4)
     {      // TOF SC81 - SC21 [ps]
       tgt.sci_tof4 =  (sci->tof_bll4 * tgt.sci_tofll4 + sci->tof_a4 
			+ sci->tof_brr4 * tgt.sci_tofrr4)/2.0 ;
      
       if(bDrawHist) 
	 hSCI_Tof4->Fill(tgt.sci_tof4);
     }
   

   /* check for polygon in raw detof spectrum of SC41 */
   tgt.sci_b_detof = cSCI_detof->Test(tgt.sci_tof2, tgt.sci_e[5]);
 
   if(bDrawHist)
     {
       /* check sc21 energy loss vs. sc21 position   */
       //     hSCI_dEx2->Fill(tgt.sci_x[2],tgt.sci_e[2]);
       hSCI_dEx2->Fill(clb.tpc_x[2],sqrt(tgt.sci_e[2]));
       /* check sc41 energy loss vs. sc21 position   */
       hSCI_dEx5->Fill(tgt.sci_x[5],tgt.sci_e[5]);
       /* check sc21 (delayed) energy loss vs. sc21 position   */
       hSCI_dEx2del->Fill(tgt.sci_x[2], tgt.sci_e[3]);
       /* compare dE at SC21 and SC41  */
       hSCI_dE24->Fill(tgt.sci_e[2], tgt.sci_e[5]);
     }
}

void TXRSAnlProc::Procceed_ID_Analysis(TXRSSortEvent& srt, TXRSCalibrEvent& clb, TXRSAnlEvent& tgt) 
{

  tgt.id_trigger=srt.trigger;
  if(tgt.id_trigger!=1)
    return;
  /* accumulate raw detof spectrum  */
  if(bDrawHist) 
    //  hID_dEToF->Fill(tgt.sci_tof2, tgt.de[2]);// changed to Music1. YT. 2016Jun.13 19:00 
    hID_dEToF->Fill(tgt.sci_tof2, tgt.de[0]);// changed to Music1. YT. 2016Jun.13 19:00 

  // tgt.sci_e[idx] = sqrt( (tgt.sci_l[idx] - sci->le_a[0][idx]) * sci->le_a[1][idx] 
  //                   * (tgt.sci_r[idx] - sci->re_a[0][idx]) * sci->re_a[1][idx]);
  
  
  /* check for polygon in raw detof spectrum  */
  //  tgt.id_b_detof2 = cID_dEToF->Test(tgt.sci_tof2, tgt.de[0]);
    
  /*  select by what means S2 positions are to be derived:         */
  /*  ID.X2_select =0: SC21;  =1: S2 MWs                           */
  tgt.id_x2 = -9999;
  if(id->x2_select == 0) 
    {
      if(tgt.sci_b_x[2])
	tgt.id_x2 = tgt.sci_x[2]; /* pos index 2 = SC21 */
    }

  /* For old tracking with old nomenclature (06.06.18)
  if(id->x2_select == 1)
    {
     
      tgt.id_x2=999;
      if(clb.b_tpc_xy[2] && clb.b_tpc_xy[3])
	tgt.id_x2 = clb.tpc_x_s2_foc;
      if(clb.b_tpc_xy[2] && !clb.b_tpc_xy[3])
	tgt.id_x2 = clb.tpc_x[2];
      if(!clb.b_tpc_xy[2] && clb.b_tpc_xy[3])
	tgt.id_x2 = clb.tpc_x[3];
      if(!clb.b_tpc_xy[2] && !clb.b_tpc_xy[3] && tgt.sci_b_x[2])
	tgt.id_x2 = tgt.sci_x[2];
     

     tgt.id_a2 = clb.tpc_angle_x_s2_foc;
     tgt.id_y2 = clb.tpc_y_s2_foc;
     tgt.id_b2 = clb.tpc_angle_y_s2_foc;
  }
  */

   if(id->x2_select == 1)
    {
     
      tgt.id_x2=999;
      tgt.id_y2=999;
      tgt.id_a2=999;
      tgt.id_b2=999;
      
      if(clb.b_tpc_xy[0] && clb.b_tpc_xy[1] && !clb.b_tpc_xy[2] && !clb.b_tpc_xy[3] )
	{
	tgt.id_x2 = clb.tpc_x_s2_foc_21_22;
        tgt.id_y2 = clb.tpc_y_s2_foc_21_22;
	tgt.id_a2 = clb.tpc_angle_x_s2_foc_21_22;
	tgt.id_b2 = clb.tpc_angle_y_s2_foc_21_22;
	}

      if(!clb.b_tpc_xy[0] && !clb.b_tpc_xy[1] && clb.b_tpc_xy[2] && clb.b_tpc_xy[3] )
	{
	tgt.id_x2 = clb.tpc_x_s2_foc_23_24;
        tgt.id_y2 = clb.tpc_y_s2_foc_23_24;
	tgt.id_a2 = clb.tpc_angle_x_s2_foc_23_24;
	tgt.id_b2 = clb.tpc_angle_y_s2_foc_23_24;
	}

      if(!clb.b_tpc_xy[0] && clb.b_tpc_xy[1] && !clb.b_tpc_xy[2] && clb.b_tpc_xy[3] )
	{
	tgt.id_x2 = clb.tpc_x_s2_foc_22_24;
        tgt.id_y2 = clb.tpc_y_s2_foc_22_24;
	tgt.id_a2 = clb.tpc_angle_x_s2_foc_22_24;
	tgt.id_b2 = clb.tpc_angle_y_s2_foc_22_24;
	}

       if(clb.b_tpc_xy[0] && clb.b_tpc_xy[1] && clb.b_tpc_xy[2] && clb.b_tpc_xy[3] )
	{
	tgt.id_x2 = clb.tpc_x_s2_foc_22_24;
        tgt.id_y2 = clb.tpc_y_s2_foc_22_24;
	tgt.id_a2 = clb.tpc_angle_x_s2_foc_22_24;
	tgt.id_b2 = clb.tpc_angle_y_s2_foc_22_24;
	}

        if(!clb.b_tpc_xy[0] && !clb.b_tpc_xy[1] && !clb.b_tpc_xy[2] && !clb.b_tpc_xy[3] && tgt.sci_b_x[2] )
	{
	tgt.id_x2 = tgt.sci_x[2];
        tgt.id_y2 = 0.0;
	tgt.id_a2 = 0.0;
	tgt.id_b2 = 0.0;
	}
	
  }
                            
  /*  select by what means S4 positions are to be derived:         */
  /*  ID.X4_select =0: SC41;  =1: S4 MWs;                          */
  if(id->x4_select == 0) 
    {
      //    if(tgt.sci_b_x[5]) tgt.id_x4 = tgt.sci_x[5];  /* pos index 5 = SC41 */
      tgt.id_x4 = 0;  /* pos index 5 = SC41 */
    }
  
  if(id->x4_select == 1)
    {
      tgt.id_x4 = clb.tpc_x_s4;
      tgt.id_a4 = clb.tpc_angle_x_s4;
      tgt.id_y4 = clb.tpc_y_s4;
      tgt.id_b4 = clb.tpc_angle_y_s4;
    }

  
  /*  check that the positions are OK   */
  tgt.id_b_x2 = cID_x2->Test(tgt.id_x2);
  tgt.id_b_x4 = cID_x4->Test(tgt.id_x4);

  /*
  /// conditions for the 12C measurement

  // for 12N at S4
  if(tgt.de[0]>2200. && tgt.de[0]<4000.)hID_x4z55->Fill(tgt.id_x4);// added 2016Jun.16
  // for 12N at S2
   if(tgt.de[0]>2200. && tgt.de[0]<4000.)hID_x2z55->Fill(tgt.id_x2);// added 2016Jun.16

   //for 12B at s4 and s2
  if(tgt.de[0]>900. && tgt.de[0]<1200. )hID_x4z53->Fill(tgt.id_x4);
  if(tgt.de[0]>900. && tgt.de[0]<1200. )hID_x2z53->Fill(tgt.id_x2);
  */

  // This ones are for all nuclei as a funtion of the energy loss

  hID_E_Xs4->Fill(tgt.id_x4,tgt.de[0]);// added by 2016Jun.16

  hID_E_Xs2->Fill(tgt.id_x2,tgt.de[0]);// added by 2016Jun.16

  hID_x2a2->Fill(tgt.id_x2,tgt.id_a2);
  hID_y2b2->Fill(tgt.id_y2,tgt.id_b2);
  hID_x4a4->Fill(tgt.id_x4,tgt.id_a4);
  hID_y4b4->Fill(tgt.id_y4,tgt.id_b4); 


  /****  A/Q calculations for S41-S21 or S42-S41 (tof index 2+3) *************/ 
  Float_t f = 931.4940 / 299.792458 ;    /* the u/(c*10^-6) factor  */

  /*----------------------------------------------------------*/
  /* Determination of beta                                    */
  /* ID.TofOff(i)                   Flight time offset [ps]   */
  /* ID.Path(i)                     Flight path/c [ps]        */
  /* TOF(i)        BIN FLOAT(24),   Flight time  [ps]         */
  /*----------------------------------------------------------*/
  /* from ToF S41-S21 */
  if (tgt.sci_b_tofll2 && tgt.sci_b_tofrr2)
    {
      tgt.id_beta = id->id_path2 / (id->id_tofoff2 - tgt.sci_tof2);
      if(bDrawHist) 
	hID_beta->Fill(tgt.id_beta*1000.);
    }
  
  /*------------------------------------------------------*/
  /* Determination of Brho                                */
  /* Dispersion and magnification are still the same      */
  /* variable for S41-S21 and S42-S41, adjust in setup.C  */
  /*------------------------------------------------------*/

  /* check S2 valid conditions */
  //if (tgt.id_b_x2 && tgt.id_b_x4) {
  if (tgt.id_b_x2)
    {   
      // first half of FRS
      tgt.id_rho[0] = frs->rho0[0] * (1. - tgt.id_x2/1000./frs->dispersion[0]);   
      // second half of FRS
      tgt.id_rho[1] = frs->rho0[1] * (1. - (tgt.id_x4 - frs->magnification[1] * tgt.id_x2) / 1000. / frs->dispersion[1]) ; 
      //    tgt.id_rho[1] = frs->rho0[1] * (1. - (frs->magnification[1] * tgt.id_x2) / 1000. / frs->dispersion[1]) ; 
      
      for(int i=0;i<2;i++)
	{
	  tgt.id_brho[i] = (fabs(frs->bfield[2*i]) + fabs(frs->bfield[2*i+1]))/ 2. * tgt.id_rho[i];
	  if(bDrawHist) 
	    hID_BRho[i]->Fill(tgt.id_brho[i]);
	}
    }   
  //}


  /*--------------------------------------------------------------*/
  /* Determination of A/Q                                         */
  /*--------------------------------------------------------------*/
  /* Beta(i)       BIN FLOAT(24),   Beta = v/c                    */
  /* Gamma(i)      BIN FLOAT(24),   Gamma= sqrt(1/1-beta**2)      */
  /*--------------------------------------------------------------*/
  /* for S2-S4 */
  // if (tgt.sci_b_tofll2 && tgt.sci_b_tofrr2 && tgt.id_b_x2 && tgt.id_b_x4) {
  if (tgt.sci_b_tofll2 && tgt.sci_b_tofrr2 &&  tgt.id_b_x2)
    {
      if ((tgt.id_beta>0.0) && (tgt.id_beta<1.0))
	{
	  tgt.id_gamma = sqrt( 1. /(1. - tgt.id_beta * tgt.id_beta));
	  //tgt.id_AoQ = tgt.id_brho[1]/tgt.id_beta/tgt.id_gamma/ f - id->id_tofcorr2 * tgt.id_x4;
	  tgt.id_AoQ = tgt.id_brho[1]/tgt.id_beta/tgt.id_gamma/ f ;
	  //      std::cout <<" id_AoQ "<<tgt.id_AoQ<<std::endl; 
	  
	  //correction for id_a2, JK 16.9.11


	  tgt.id_AoQ_corr = tgt.id_AoQ - frs->a2AoQCorr * tgt.id_a2;
	  if (!clb.b_tpc_xy[4] || !clb.b_tpc_xy[5])
	    tgt.id_AoQ_corr = tgt.id_AoQ - frs->a4AoQCorr * tgt.id_a4;


	  if(bDrawHist)
	    {
	      hID_AoQ->Fill(tgt.id_AoQ);
	      hID_AoQ_corr->Fill(tgt.id_AoQ_corr);
	    }
	  tgt.id_b_AoQ = kTRUE;
	}
    }

  if(tgt.id_b_AoQ)
    {
      if(bDrawHist)
	{
	  hID_DeltaBrho_AoQ->Fill(tgt.id_AoQ,tgt.id_brho[0]-tgt.id_brho[1]);
	  hID_DeltaBrho_AoQzoom->Fill(tgt.id_AoQ,tgt.id_brho[0]-tgt.id_brho[1]);
	}
    }
  
  // }

  /*------------------------------------------------*/
  /* Determination of Z                             */
  /*------------------------------------------------*/
  /****  S4  (MUSIC 1)   */
  //  if((tgt.de_cor[0]>0.0) && (tgt.id_beta>0.0) && (tgt.id_beta<1.0)) {
  
  if((tgt.de[0]>0.0) && (tgt.id_beta>0.0) && (tgt.id_beta<1.0))
    {
      Double_t power = 1., sum = 0.;
      for (int i=0;i<4;i++)
	{
	  sum += power * id->vel_a[i];
	  power *= tgt.id_beta;
	}
      tgt.id_v_cor = sum;
      
      if (tgt.id_v_cor > 0.0)
	tgt.id_z = frs->primary_z * sqrt(tgt.de[0]/tgt.id_v_cor) + frs->offset_z;
      
    if ((tgt.id_z>0.0) && (tgt.id_z<100.0))
      {
	tgt.id_b_z = kTRUE;
	if(bDrawHist) 
	  {
	    hID_Z->Fill(tgt.id_z);
	  }
      }
    }

  /****  S4  (MUSIC 2)   */
  
  //  if((tgt.de_cor[0]>0.0) && (tgt.id_beta>0.0) && (tgt.id_beta<1.0)) {
  if((tgt.de[1]>0.0) && (tgt.id_beta>0.0) && (tgt.id_beta<1.0))
    {
      Double_t power = 1., sum = 0.;
      for (int i=0;i<4;i++)
	{
	  sum += power * id->vel_a2[i];
	  power *= tgt.id_beta;
	}
      tgt.id_v_cor2 = sum;
      
      if (tgt.id_v_cor2 > 0.0)
	tgt.id_z2 = frs->primary_z * sqrt(tgt.de[1]/tgt.id_v_cor2) + frs->offset_z2;
      //std::cout<<"id_z2="<<tgt.id_z2<<std::endl;
      
      if ((tgt.id_z2>0.0) && (tgt.id_z2<100.0))
	{
	  tgt.id_b_z2 = kTRUE;
	  if(bDrawHist) 
	    hID_Z2->Fill(tgt.id_z2);
	}
      if(bDrawHist)
	{
	  hID_Z_Z2->Fill(tgt.id_z,tgt.id_z2);
	  hID_Z_dE2->Fill(tgt.id_z,tgt.de[1]);
	}
    }
 

  /****  S4  (MUSIC OLD)   */
  //  if((tgt.de_cor[0]>0.0) && (tgt.id_beta>0.0) && (tgt.id_beta<1.0)) {
  
  if((tgt.de[2]>0.0) && (tgt.id_beta>0.0) && (tgt.id_beta<1.0))
    {
      Double_t power = 1., sum = 0.;
      for (int i=0;i<4;i++)
	{
	  sum += power * id->vel_a3[i];
	  power *= tgt.id_beta;
	}
      tgt.id_v_cor3 = sum;
      
      if (tgt.id_v_cor3 > 0.0)
	tgt.id_z3 = frs->primary_z * sqrt(tgt.de[2]/tgt.id_v_cor3) + frs->offset_z3;
      //std::cout<<"id_z2="<<tgt.id_z2<<std::endl;
      
      if ((tgt.id_z3>0.0) && (tgt.id_z3<100.0))
	{
	  tgt.id_b_z3 = kTRUE;
	  if(bDrawHist)
	    {
	      hID_Z3->Fill(tgt.id_z3);
	      hID_ZmaxRange->Fill(tgt.id_z3);
	    }
	}
      if(bDrawHist)
	hID_Z_Z3->Fill(tgt.id_z,tgt.id_z3);


      static const double anode_width = 10.;//cm
      double music_dX = anode_width*sqrt(tgt.id_a4*tgt.id_a4+tgt.id_b4*tgt.id_b4+1.);
      h_dEdx_betagammaAll->Fill(tgt.id_beta*tgt.id_gamma,tgt.de[2]/music_dX);
      h_dEdx_betagammaAllZoom->Fill(tgt.id_beta*tgt.id_gamma,tgt.de[2]/music_dX);
      double music_dEtemp0 = srt.music_e3[0]*music->e3_gain[0] + music->e3_off[0];
      for(int i=0;i<4;++i)
	{
	  double music_dEtemp = srt.music_e3[i]*music->e3_gain[i] + music->e3_off[i];
	  h_dEdx_betagamma[i]->Fill(tgt.id_beta*tgt.id_gamma,music_dEtemp/music_dX);
	  if(i>0)
	    h_DiffdEdx_betagamma[i]->Fill(tgt.id_beta*tgt.id_gamma,(music_dEtemp-music_dEtemp0)/music_dX);
	}
    }
  /*------------------------------------------------*/
  /* Identification Plots                           */
  /*------------------------------------------------*/

  /****  for S2-S4  ****/

  //Float_t my_dist_TPC6= 1075; //position TPC6 in s388?
  //Float_t x4env=0;
  //Float_t y4env=0;
  //  Float_t my_dist_TPC65;
  if(tgt.id_b_AoQ)
    {
      if(bDrawHist)
	{ 
	  hID_x2AoQ->Fill(tgt.id_AoQ, tgt.id_x2);  
	  hID_x4AoQ->Fill(tgt.id_AoQ, tgt.id_x4);
	  //    hID_Z2_AoQ->Fill(tgt.id_AoQ, tgt.id_z2);
	  // hID_Z3_AoQ->Fill(tgt.id_AoQ, tgt.id_z3);
	  hID_x2x4->Fill(tgt.id_x2,tgt.id_x4);
	  hID_SC41dE_AoQ->Fill(tgt.id_AoQ, tgt.sci_e[5]);
	}

      

      
      for (int i=0;i<5;i++)
	{
	tgt.id_b_x4AoQ_Z[i] = cID_x4AoQ_Z[i]->Test(tgt.id_AoQ, tgt.id_x4); 
        }
      
      tgt.id_b_x2AoQ = cID_x2AoQ[0]->Test(tgt.id_AoQ, tgt.id_x2);
      for(int i=0;i<6;++i)
	{
	  if(cID_x2AoQ[i]->Test(tgt.id_AoQ, tgt.id_x2)==true)
	    {
	      hMUSIC3_x2AoQ_E[0][i]->Fill(srt.music_e3[0]);
	      hMUSIC3_x2AoQ_E[1][i]->Fill(srt.music_e3[1]);
	      hMUSIC3_x2AoQ_E[2][i]->Fill(srt.music_e3[2]);
	      hMUSIC3_x2AoQ_E[3][i]->Fill(srt.music_e3[3]);	      
	      hID_x4AoQ_x2AoQgate[i]->Fill(tgt.id_AoQ, tgt.id_x4);
	      if (tgt.id_b_z3)
		hID_ZAoQ_x2AoQgate[i]->Fill(tgt.id_AoQ, tgt.id_z3);
	    }
	}
	       
      //      if (tgt.id_b_z3) // changed to Music1. YT. 2016Jun.13 19:00
      if (tgt.id_b_z) // changed to Music1. YT. 2016Jun.13 19:00   
	{   
	  if(bDrawHist)
	    { 

	      hID_Z_Q->Fill(tgt.id_z);
	       
	      hID_Z_AoQ->Fill(tgt.id_AoQ, tgt.id_z);// changed to Music1. YT. 2016Jun.13 19:00 
	      hID_Z_AoQv2->Fill(tgt.id_AoQ, tgt.id_z);// changed to Music1. YT. 2016Jun.13 19:00 
	      hID_Z_AoQ_corr->Fill(tgt.id_AoQ_corr, tgt.id_z); // changed to Music1. YT. 2016Jun.13 19:00 
	      //hID_Z_AoQ_corrZoom->Fill(tgt.id_AoQ_corr,tgt.id_z);

	      hID_x2z->Fill(tgt.id_z, tgt.id_x2);// MUSIC1
	      hID_x4z->Fill(tgt.id_z, tgt.id_x4);// MUSIC1
	      
	      
	      if(tgt.id_z>= id->zgate_low && tgt.id_z<= id->zgate_high) // changed to Music1. YT. 2016Jun.13 19:00
		{
		  hID_x4AoQ_zgate->Fill(tgt.id_AoQ_corr,tgt.id_x4);
		}
	      
	      hID_Z_Sc21E->Fill(tgt.id_z, sqrt(tgt.sci_l[2]*tgt.sci_r[2])); // changed to Music1. YT. 2016Jun.13 19:00

	      if(tgt.id_b_z2)
		{
		  hID_Z2_AoQ->Fill(tgt.id_AoQ, tgt.id_z2);
		  if(TMath::Abs(tgt.id_z-tgt.id_z2-0.3)<0.6)
		    {
		      hID_Z_AoQ_zsame->Fill(tgt.id_AoQ, tgt.id_z);
		    }
		}
	      
	      
	      // S395 envelop plots, tracking from position(TPC5) and angle (TPC5, TPC4)
	      /*
		if( tgt.id_b_x4) {
		for(Int_t zz=0; zz<=2600; zz+=100) {
		x4env = tgt.id_a4 * (zz-my_dist_TPC6)/1000. + clb.tpc_x[5];
		y4env = tgt.id_b4 * (zz-my_dist_TPC6)/1000. + clb.tpc_y[5];
		hID_xz->Fill(zz,x4env);
		hID_yz->Fill(zz,y4env);
		// hID_xzc->Fill(zz,x4env);
		//  hID_yzc->Fill(zz,y4env);
		}
		}
	      */	      


	    }
	
	  for(int i=0;i<5;i++)
	    {
	      tgt.id_b_music_z[i] = cID_Z_Z[i]->Test(tgt.id_z);   
	      if(cID_Z_AoQ[i]==nullptr)
		std::cout<<"E> cID_Z_AoQ["<<i<<"] nullptr "<<cID_Z_AoQ[i]<<" | "<<id->ID_Z_AoverQ_num[i]<<std::endl;
	      
	       tgt.id_b_z_AoQ[i] = cID_Z_AoQ[i]->Test(tgt.id_AoQ, tgt.id_z);      
	      
		// if(i==0)
		//   { 
		//     if (tgt.id_b_z_AoQ[0])
		//       { 
		// 	hMUSIC3_z_AoQ_E[0][i]->Fill(srt.music_e3[0]);
		// 	hMUSIC3_z_AoQ_E[1][i]->Fill(srt.music_e3[1]);
		// 	hMUSIC3_z_AoQ_E[2][i]->Fill(srt.music_e3[2]);
		// 	hMUSIC3_z_AoQ_E[3][i]->Fill(srt.music_e3[3]);
		// 	hID_x4c[0]->Fill(tgt.id_x4);
		// 	hID_x2c[0]->Fill(tgt.id_x2);
		      
		// 	hMUSIC3_music_z_E[0][i]->Fill(srt.music_e3[0]);
		// 	hMUSIC3_music_z_E[1][i]->Fill(srt.music_e3[1]);
		// 	hMUSIC3_music_z_E[2][i]->Fill(srt.music_e3[2]);
		// 	hMUSIC3_music_z_E[3][i]->Fill(srt.music_e3[3]);
		//       }
		//   }
	      if (tgt.id_b_z_AoQ[i])
		  {
		    if(bDrawHist)
		      { 
			
			hID_Z_AoQgate[i]->Fill(tgt.id_AoQ, tgt.id_z);;
			hID_Z3_gate[i]->Fill(tgt.id_z3);			
			hID_dE3_gate[i]->Fill(tgt.de[2]);

			hMUSIC3_z_AoQ_E[0][i]->Fill(srt.music_e3[0]);
			hMUSIC3_z_AoQ_E[1][i]->Fill(srt.music_e3[1]);
			hMUSIC3_z_AoQ_E[2][i]->Fill(srt.music_e3[2]);
			hMUSIC3_z_AoQ_E[3][i]->Fill(srt.music_e3[3]);

			
			hID_x4c[i]->Fill(tgt.id_x4);
			hID_x2c[i]->Fill(tgt.id_x2);
			hMUSIC3_music_z_E[0][i]->Fill(srt.music_e3[0]);
			hMUSIC3_music_z_E[1][i]->Fill(srt.music_e3[1]);
			hMUSIC3_music_z_E[2][i]->Fill(srt.music_e3[2]);
			hMUSIC3_music_z_E[3][i]->Fill(srt.music_e3[3]);

			// S395 envelop plots, tracking from position(TPC6) and angle (TPC5, TPC6)
			/*
			  if( tgt.id_b_x4 && i==0) {
			  for(Int_t zz=0; zz<=2600; zz+=100) {
			  x4env =  tgt.id_a4 * (zz-my_dist_TPC6)/1000. + clb.tpc_x[5];
			  y4env =  tgt.id_b4 * (zz-my_dist_TPC6)/1000. + clb.tpc_y[5];
			  //hID_xz->Fill(zz,x4env);
			  //hID_yz->Fill(zz,y4env);
			  hID_xzc->Fill(zz,x4env);
			  hID_yzc->Fill(zz,y4env);
			  }
			  }
			*/
			 
			  
			  hID_betac[i]->Fill(tgt.id_beta);
			  hID_brhoc[i]->Fill(tgt.id_brho[1]);
			
			  // hID_x_target1c[i]->Fill(clb.tpc_x_s2_target1);
			  // hID_x_target2c[i]->Fill(clb.tpc_x_s4_target2);
			  // hID_y_target1c[i]->Fill(clb.tpc_y_s2_target1);
			  // hID_y_target2c[i]->Fill(clb.tpc_y_s4_target2);
			
		      
			// if(i==1){
			//if(clb.coin[10]==1)
			//	hID_x4c[i]->Fill(tgt.id_x4);
			//}else{
			//hID_x4c[i]->Fill(tgt.id_x4);
			//}
		      }
		  }
	      }
	}
    }
}

void TXRSAnlProc::Procceed_MRTOF_Analysis(TXRSSortEvent& srt, TXRSCalibrEvent& clb, TXRSAnlEvent& tgt) 
{
  
  if(srt.EventFlag==0x200 && srt.trigger==1)
    {
      tgt.mrtof_start = srt.mrtof_start*25./1000000.; // ch to microsec
      
      double tempStop1 = srt.mrtof_stopDelay*25./1000000. ;
      double tempStop2 = srt.mrtof_stop*25./1000000. ;
      
      h_MRtof_Start->Fill(tgt.mrtof_start);
      h_MRtof_StopDelay->Fill(tempStop1);
      h_MRtof_Stop->Fill(tempStop2);
      
      int stop_status=0;
      
      if(tempStop1<=0.)
	{
	  if(tempStop2>0.)
	    {
	      tgt.mrtof_stop  = tempStop2;
	      h_MRtof_status->Fill("GoodStop2",1);
	      stop_status=1;
	    }
	  else
	    {
	      tgt.mrtof_stop = -9999.;
	      //std::cout<<"E> MRTOF Anal : both stop signal "<<
	      if(tgt.mrtof_start>1)
		h_MRtof_status->Fill("NoGoodStop",1);
	      else
		h_MRtof_status->Fill("NoData",1);
	    }
	}
      else
	{
	  if(tempStop2<=0.)
	    {
	      tgt.mrtof_stop = tempStop1-45.;
	      h_MRtof_status->Fill("GoodStop1",1);
	      stop_status=1;
	    }
	  else
	    {
	      double diff_stop = tempStop1-tempStop2;
	      if(TMath::Abs(diff_stop-45)<1.)
		{
		  tgt.mrtof_stop = tempStop1-45.;
		  h_MRtof_status->Fill("GoodStop1&2",1);
		  stop_status=1;
		}
	      else
		{
		  h_MRtof_status->Fill("Stop1&2BadOverlap",1);
		  std::cout<<"E> MRtof diff stop too big from 45 microsec !"<<diff_stop<< "sorted : D="<<srt.mrtof_stopDelay <<" "<< srt.mrtof_stop <<" | in microsec: D="<< tempStop1 <<" "<<tempStop2<<"\n";
		}
	    }
	}

      //int diff_stopdelay = 1891482-54830;

      if(stop_status==1)
	{
	  tgt.mrtof_tof = 100 - ( tgt.mrtof_start - tgt.mrtof_stop); // add 100 microsec from mrtof trigger system 
	  h_MRtof_tof->Fill(tgt.mrtof_tof);
	  h_MRtof_status->Fill("GoodTOF",1);
	}
      else
	h_MRtof_status->Fill("BadTOF",1);


    }

  tgt.mrtof_si_e1=clb.si_e1;
  tgt.mrtof_si_e2=clb.si_e2;
  tgt.mrtof_si_e3=clb.si_e3;
  tgt.mrtof_si_e4=clb.si_e4;
  tgt.mrtof_si_e5=clb.si_e5;
  
}

ClassImp(TXRSAnlProc)
