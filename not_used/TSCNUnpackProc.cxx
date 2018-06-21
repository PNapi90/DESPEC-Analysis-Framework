// $Id: TSCNUnpackProc.cxx 755 2011-05-20 08:04:11Z linev $
//-----------------------------------------------------------------------
//
//       The GSI Online Offline Object Oriented (Go4) Project
//         Experiment Data Processing at EE department, GSI
//-----------------------------------------------------------------------
// Copyright (C) 2000- GSI Helmholtzzentrum für Schwerionenforschung GmbH
//                     Planckstr. 1, 64291 Darmstadt, Germany
// Contact:            http://go4.gsi.de
//-----------------------------------------------------------------------
// This software can be used under the license agreements as stated
// in Go4License.txt file which is part of the distribution.
//-----------------------------------------------------------------------

#include "TSCNUnpackProc.h"
#include "TGo4UserException.h"

#include "Riostream.h"
#include <time.h>
#include "TROOT.h"

#include "TH1.h"
//#include "TH1D.h"
#include "TF1.h"
#include "TH2.h"
#include "TCutG.h"
#include "TArc.h"

#include<math.h>

#include "TGo4Picture.h"

#include "TGo4MbsEvent.h"
#include "TSCNUnpackEvent.h"
#include "QDC.h"

//#include "calibration.h"



#include<string>

#define FT_1           200    // one fast trigger,
#define PI_FLG         400    // two (or more) fast
#define RT_FLG         600    // retrigger flag set
#define NO_FT          800    // no fast trigger
#define MAX_CHA_INPUT 35
//-----------------------------------------------------------


//calibration cal("cal.root");
//vector < vector< Double_t> > calib = cal.getcalib();

TSCNUnpackProc::TSCNUnpackProc() :
   TGo4EventProcessor()
{
  ffill = kTRUE;
  fshift = 6;
}
//-----------------------------------------------------------
TSCNUnpackProc::TSCNUnpackProc(const char* name) :
   TGo4EventProcessor(name)
{
  cout << "**** TSCNUnpackProc: Create" << endl;


 for(int i=0;i<33;i++)
 {

      fCr1Ch[i] = 0;
      fCr2Ch[i] = 0;
      fCr3Ch[i] = 0;
      fCr4Ch[i] = 0;
      fCr5Ch[i] = 0;
      fCr6Ch[i] = 0;
 }


 for(int i=0;i<33;i++)
    {
      fCr1Ch[i] = MakeTH1('D',Form("PADI_Coarse/CoarseCh%02d",i),Form("Coarse%2d",i), 1001, -500., 500.);
      fCr2Ch[i] = MakeTH1('D',Form("PADI_Fine/FineCh%02d",i),Form("Fine%2d",i), 1001, -500., 500.);
//    fCr3Ch[i] = MakeTH1('D',Form("PADI_diff/diffCh%02d",i),Form("diff Ch(%2d)-Ch(%2d)",i+1,i), 10000, -10000., 10000.);
      fCr3Ch[i] = MakeTH1('D',Form("TimeDiffLeading/LeadingDiffCh%02d",i),Form("leading Ch(%2d)-Ch(%2d)",i,i+1), 200001, -2500., 2500.);
      fCr4Ch[i] = MakeTH1('D',Form("TimeDiffTrailing/TrailingDiffCh%02d",i),Form("trailing Ch(%2d)-Ch(%2d)",i,i+1), 20001, -100., 100.);
      fCr5Ch[i] = MakeTH1('D',Form("TOT/TOTCh%02d",i),Form("TOT Ch(%2d)-Ch(%2d)",i,i+1), 20001, -100., 100.);
      fCr6Ch[i] = MakeTH1('D',Form("TOTAND/TOTandCh%02d",i),Form("TOTand Ch(%2d)-Ch(%2d)",i,i+1), 20001, -100., 100.);
      fCr8Ch[i] = MakeTH1('D',Form("TOTANDdiff/TOTandDiffCh%02d",i),Form("TOTandDiff Ch(%2d)-Ch(%2d)",i,i+1), 20001, -100., 100.);
      fCr9Ch[i] = MakeTH1('D',Form("PADI_Time_minusTrigg/TimeCh%02d",i),Form("TimeCh%2d",i), 10001, -500., 500.); 
      fCr10Ch[i] = MakeTH1('D',Form("PADI_FineTimeCal/CalCh%02d",i),Form("CalCh%2d",i), 1001, -500., 500.);
      fCr11Ch[i] = MakeTH1('D',Form("PADI_FineTimeAfterCalib/Cal1Ch%02d",i),Form("Cal1Ch%2d",i), 1001, -500., 500.);
      fCr7Ch[i] = MakeTH2('D',Form("PADI_2D/StatCh%02d",i),Form("STAT%02d",i), 2000, 0., 20, 2000, -20., 20.);
      fCr12Ch[i] = MakeTH1('D',Form("TimeDiffLeadinggated/LeadingDiffChgated%02d",i),Form("leading Ch1(%2d)-Ch1(%2d)",i,i+1), 20001, -100., 100.);


  }


//	fCr4Ch[1] = MakeTH2('D',Form("PADI_2D/diffCh%02d",1),Form("diff Ch(%2d)-Ch(%2d)",1,1), 2000, 0., 20., 2000, 0., 20.);

    tamexdata.ChannelNo=0;
    tamexdata.CoarseTime=0;
    tamexdata.FineTime=0;
    tamexdata.LeadingEdge=false;
    tamexdata.TrailingEdge=false;
 //   tamexdata.NHits=0;

    tamexheader.PreTriggerWindow=0;
    tamexheader.PostTriggerWindow=0;
    tamexheader.SFPID=0;
    tamexheader.TamexID=0;
    tamexheader.TDCHeader=0x00;
    tamexheader.TDCTrailer=0x00;


for(int i=0;i<33;i++){
B[i]=1.0;
}

count_event=0;


  /*  fCoincQ1A1 = MakeTH2('D',"Coinc","CoincQDC1ADC1" ,200,0.,2000.,200,0.,5000.);
   */
//  picPMTraw = GetPicture("picPMTraw");
//  picPMTraw = new TGo4Picture("pmtRaw","photomultiplier Raw",1,2);
//  picPMTraw -> SetDrawHeader();
//  picPMTraw -> Pic(0,0) -> SetDivision(SCN_NUM_CHAN/2,1);// X anodes
//  picPMTraw -> Pic(0,1) -> SetDivision(SCN_NUM_CHAN/2,1);// Y anodes
/*
  for(int i=0;i<SCN_NUM_CHAN;i++)
    {
      if(i<16)
	{
	  picPMTraw -> Pic(0,0) -> Pic(i,0) -> AddH1(fCr1Ch[i]);// HistRaw X
	  picPMTraw -> Pic(0,0) -> Pic(i,0) -> SetRangeX(0,8000);
	}
      else
	{
	  picPMTraw -> Pic(0,1) -> Pic(i-SCN_NUM_CHAN/2,0) -> AddH1(fCr1Ch[i]);// HistRaw X
	  picPMTraw -> Pic(0,1) -> Pic(i-SCN_NUM_CHAN/2,0) -> SetRangeX(0,8000);
	}
    }
  AddPicture(picPMTraw);
*/
  //setup SIS histosgrams
/*
  for(int i=0;i<FADC_CHAN;i++)
    {
      hph[i]=MakeTH1('F',Form("Cluster/Energy/energySISchan%2d",i),Form("E SIS channel %2d",i+1),8192.,0,8192.);
      htrace[i]=MakeTH1('I',Form("Cluster/Traces/traceSISchan%2d",i),Form("T SIS channel %2d",i+1),RAW_DATA_LENGTH,0,RAW_DATA_LENGTH-1);
    }

*/

  // create histograms
}
//-----------------------------------------------------------
TSCNUnpackProc::~TSCNUnpackProc()
{
  cout << "**** TSCNUnpackProc: Delete" << endl;
}
//-----------------------------------------------------------
Bool_t TSCNUnpackProc::BuildEvent(TGo4EventElement* dest)
{
  Bool_t isValid=kFALSE; // validity of output event

  TGo4MbsEvent* inp_evt = (TGo4MbsEvent* ) GetInputEvent(); // from this

  TSCNUnpackEvent* out_evt = (TSCNUnpackEvent*) dest;


// CAEN variables


unsigned long  caen_header,
               geo,                  /* geograph. address */
               type;                 /* [2,6] */

long           //g1=0,
               d_cnt;                /* number of data words */
unsigned long  data,
               chan1;

/* some masks for the CAEN data words */

#define  OV_MASK 0x00001000  /* overflow bit */
#define  UN_MASK 0x00002000  /* underthreshold bit */
#define   V_MASK 0x00004000  /* data valid bit */
#define FCH_MASK 0x00003F00  /* number of fired channels mask */
#define  CH_MASK 0x003F0000  /* channel mask */
#define  DA_MASK 0x00000FFF  /* data mask */
#define  TY_MASK 0x07000000  /* type mask */
#define GEO_MASK 0xF8000000  /* geo address mask */

#define CH_MASK2 0x03E00000  // channel mask for V1290
#define DA_MASK2 0x001FFFFF  // data mask for V1290











  if (inp_evt==0) {
    cout << "SCNUnpackProc: no input event !"<< endl;
    out_evt->SetValid(isValid); // to store or not to store
    // default calling Fill method will set validity of out_evt to return value!
    return isValid;
  }
  isValid=kTRUE;
//  cout<<"found an event!!!!"<<inp_evt->GetTrigger()<<endl ; //$$
  inp_evt->ResetIterator();
  TGo4MbsSubEvent* psubevt(0);

  /******* for the SIS module ****/

  const Int_t fthr = 50; //100

  Int_t *pl_data,diff,first_e_sample;
  char exit_flg=0;
  long raw_data=0,
    energy_data=0,
    first_lw;
  long card=0,chan,rchan,samples, energy=0,trg_cnt,
    ft_cnt,
    trailer;
  long hit_pattern, nof_hits=0,i;
  static long raw_data_old=0;
  unsigned short *pl_data16,
    first_sample;
  char index_pat;
  long base_sum,
    base2_sum,
    top_sum;
 char pi_flg,
    rt_flg;

  

  Int_t value = 0;
  Int_t value_t = 0;
  Int_t chNo = 0;


 UInt_t  PreTriggerWin, PostTriggerWin, tmp, tmp1, TamexChannel, HitCounts;


  while ((psubevt = inp_evt->NextSubEvent()) != 0) // subevent loop
    {
		
//		cout << "count_subevent" << count_subevent <<endl;
      Int_t* pdata=psubevt->GetDataField();
      Int_t lwords = psubevt->GetIntLen();

      Int_t PrcID=psubevt->GetProcid();

      /******************************* FINGER Readout Unpacker---Tamex ****************************/
          if (psubevt->GetIntLen()>2){

            data = *pdata;
            tamexheader.PreTriggerWindow = (data & 0xffff0000) >> 16;
            tamexheader.PostTriggerWindow = (data & 0xffff);



           for(int ix=0 ; ix< 100; ix++)
           {
           *pdata++;
           data=*pdata;
           tmp = (data & 0xfff00000) >> 20;
           //    cout << tmp << endl;
           if( ((data &  0xfff00000) >> 20) != 0xadd) {break;}
           }

             data = *pdata;

 //            if(((data & 0xff) != 0x34))
 //            {cout << "wrong channel header" << endl; break;}

            if(((data & 0xff) != 0x34))
             {cout << "wrong channel header" << endl; break;}
             tamexheader.SFPID = (data & 0xf000) >> 12;
             tamexheader.TamexID  = (data & 0xf0000) >> 16;
             tamexheader.UnusedChannelId  = (data & 0xff000000) >> 24;

            // Size of TDC data in words --(0x00000034)/4 -2

             *pdata++; data = *pdata;

             tmp = data & 0xff;

             tamexheader.SizeOfDataInWords = (tmp/4)-2;



             *pdata++; data = *pdata;
             tamexheader.TDCHeader = (data & 0xff000000) >> 24;



//	cout << "SizeOfDataInWords: "<< tamexheader.SizeOfDataInWords << endl;

//              tamexdata.NHits=0;

              for (int i =0; i< tamexheader.SizeOfDataInWords ; i++)
              {
              	 *pdata++; data = *pdata;
 /*              	if((data & 0xe0000000)>> 29 == 4)
              {
                if(((data & 0x800) >> 11) == 1)
               {
               tamexdata.ChannelNo = (data & 0x1fc00000) >> 22;
               value = (data & 0x3ff000) >> 12;
               tamexdata.FineTime=value;
               value = (data & 0x7ff);
               tamexdata.CoarseTime=value;

              for( unsigned int i=0;i<33;i++){
			  if(tamexdata.ChannelNo==i){
              fCr1Ch[i]->Fill(tamexdata.CoarseTime); //fine time (needs calibration * 5000 ps)
              fCr2Ch[i]->Fill(tamexdata.FineTime); //coarse time (ns)
              }
			  }
//     cout << tamexdata.FineTime/500.0*5.0 <<endl;
                tamexdata.LeadingEdge = true;
                tamexdata.TrailingEdge= false;
                fCoarse[tamexdata.ChannelNo] =  tamexdata.CoarseTime + tamexdata.FineTime/500.0*5.0;
//              fCoarse[tamexdata.ChannelNo] =  tamexdata.FineTime/500.0*5.0;
                

              }else{
                  cout <<"AHHHHHA"<< endl;
               tamexdata.ChannelNo = (data & 0x1fc00000) >> 22;
               value = (data & 0x3ff000) >> 12;
               tamexdata.FineTime=value;
               value = (data & 0x7ff);
               tamexdata.CoarseTime=value;
               tamexdata.LeadingEdge = false;
               tamexdata.TrailingEdge=true;
              }
*/
 //             PrintCheckTamexTDCData();
/*
            for(int i=0;i<32;i+2){
		    if((fCoarse[i]>0) && (fCoarse[i+1]>0)){
                  	fCr3Ch[i]->Fill(fCoarse[i+1]- fCoarse[i]);//-fCoarse[i]);
			}
*/
 //                 fCr3Ch[23]->Fill(fCoarse[23]- fCoarse[24]);

//              }
  
 //          } //End If
              } //End for // Time Information here


                     *pdata++; data = *pdata;


                   tamexheader.TDCTrailer = (data & 0xff000000) >> 24;

//cout << tamexheader.TDCTrailer << endl;
/*****************Fill Histograms Here******************************/

/// Here we read the second tamex module///

*pdata++;
             data = *pdata;

 //            if(((data & 0xff) != 0x34))
 //            {cout << "wrong channel header" << endl; break;}

            if(((data & 0xff) != 0x34))
             {cout << "wrong channel header" << endl; break;}
             tamexheader.SFPID = (data & 0xf000) >> 12;
             tamexheader.TamexID  = (data & 0xf0000) >> 16;
             tamexheader.UnusedChannelId  = (data & 0xff000000) >> 24;

            // Size of TDC data in words --(0x00000034)/4 -2

             *pdata++; data = *pdata;

             tmp = data & 0xff;

             tamexheader.SizeOfDataInWords = (tmp/4)-2;

//cout << tamexheader.SizeOfDataInWords << endl;

             *pdata++; data = *pdata;
             tamexheader.TDCHeader = (data & 0xff000000) >> 24;

 //cout << "TDC header Starts here:" << tamexheader.TDCHeader <<"   Should be: "<< 0xaa << endl;



//			  tamexdata.NHits=0;



//			  tamexdata.NHits=0;

for(int i=0;i<33;i++){
fCoarse[i]=0;
tamexdata.ChannelNo=0;
tamexdata.FineTime=0;
tamexdata.CoarseTime=0;
tamexdata.NHits[i]=0;
}





               






              for (int i =0; i< tamexheader.SizeOfDataInWords ; i++)
              {
              	 *pdata++; data = *pdata;
               	if((data & 0xf0000000)>> 28 == 8)
              {
                if(((data & 0x800) >> 11) == 1)
               {
               tamexdata.ChannelNo = (data & 0x1fc00000) >> 22;
               value = (data & 0x3ff000) >> 12;
               tamexdata.FineTime=value;
               value = (data & 0x7ff);
               tamexdata.CoarseTime=value;
               tamexdata.NHits[tamexdata.ChannelNo]++;
               
               
               
               
              for( unsigned int k=0;k<33;k++){
			  if(tamexdata.ChannelNo==k){
              fCr1Ch[k]->Fill(tamexdata.CoarseTime); //coarse time (ns) 
              fCr2Ch[k]->Fill(tamexdata.FineTime);   //fine time (needs calibration * 5000 ps)
              }
			  }
//     cout << tamexdata.FineTime/500.0*5.0 <<endl;
                tamexdata.LeadingEdge = true;
                tamexdata.TrailingEdge= false;


               count_event++;
                 
//               cout << count_event << endl;






                if(tamexdata.NHits[tamexdata.ChannelNo]==1){

                fCoarse[tamexdata.ChannelNo] = tamexdata.CoarseTime;
                 fFine[tamexdata.ChannelNo]  = tamexdata.FineTime;

  
               
               if(tamexdata.NHits[tamexdata.ChannelNo]>99)
               {cout << "Lot of noise in channel:"<<	tamexdata.ChannelNo <<"NHits > 100; Reset to zero"<<endl;
               tamexdata.NHits[tamexdata.ChannelNo]=0;}
               
             
		     }

              
              }else{
                  cout <<"AHHHHHA"<< endl;
               tamexdata.ChannelNo = (data & 0x1fc00000) >> 22;
               value = (data & 0x3ff000) >> 12;
               tamexdata.FineTime=value;
               value = (data & 0x7ff);
               tamexdata.CoarseTime=value;
               tamexdata.LeadingEdge = false;
               tamexdata.TrailingEdge=true;
              }


 
              
              
              } //End If
              } //End for // Time Information here



                     *pdata++; data = *pdata;


                   tamexheader.TDCTrailer = (data & 0xff000000) >> 24;


//  cout << "TDC trailer here:" << tamexheader.TDCTrailer <<"   Should be: "<< 0xbb << endl;





////////////   Calibration    //////////////////////////////

if(count_event>2000  && count_event<2005 ){

   for( unsigned int l=0;l<33;l++){	
	
    B[l]=fCr2Ch[l]-> GetEntries();
    
		if(B[l]>0){ cout <<"done"<<endl;
			       for(int ii=0; ii< fCr2Ch[l]->GetNbinsX(); ii++)
			       { 
			       	    double sum=0;
			       	    double cal_factor; 
			       	    for(int kk=0; kk< ii; kk++){
			       	    sum=sum + fCr2Ch[l]->GetBinContent(kk);
			       	    }
				        if(sum >0 && B[l] >0)
				        {
							fCr10Ch[l]-> SetBinContent(ii,sum);
							
							cal_factor = ((double) sum)/((double) B[l]);       	        

                  
				

					SetFineTimeFactor( l, (int) fCr2Ch[l]->GetXaxis()->GetBinCenter(ii), cal_factor);	
//					cout <<  cal_factor << endl;
//				fCr11Ch[l]->SetBinContent(ii, cal_factor*5000.0 ); //GetFineTimeFactor( tamexdata.ChannelNo, (int) tamexdata.FineTime )*5.0
                
				   fCr11Ch[l]->SetBinContent(ii, GetFineTimeFactor(l, ii ) *5000.0 );			
	     //       cout <<  (int) fCr2Ch[l]->GetXaxis()->GetBinCenter(ii) << "		"<< GetFineTimeFactor(l, (int) fCr2Ch[l]->GetXaxis()->GetBinCenter(ii) ) << endl;						    
				        }
				            
			       	//cout << A[k][ii] <<endl;
			       }
			   }
		   }


}


if(count_event > 2005)
{
 for( unsigned int k=0;k<33;k++){
			  if(fFine[k]>0){
				  
			  fCoarse[k] = fCoarse[k] + GetFineTimeFactor( k, fFine[k] )*5.0;
//			  cout << k<<"   Fine Time"<< fFine[k] << "Fine Time Factor" <<GetFineTimeFactor( k, (int) fFine[k] )<< endl;
			  }  
}
}






//////////////////////////////////////////////



/*****************Fill Histograms Here******************************/

double Ch0minusChI[33];


for(int i=0;i<33;i++){	
	Ch0minusChI[i]= fCoarse[0] - fCoarse[i];
	if((fCoarse[0] >0) && (fCoarse[i]>0)){ fCr9Ch[i] ->Fill(Ch0minusChI[i] );}// Time data wrt trigger time
}
	
//		    if((fCoarse[i]>0) && (fCoarse[i+2]>0)){
 int fLeadingLoop=0;
 int fTimeDiffLoop=0; 
 double TOT_value[32]; for (int i=0;i<32;i++){TOT_value[i]=0.0;}
 double LeadingLeading[32]; for (int i=0;i<32;i++){LeadingLeading[i]=0.0;}
 double TrailingTrailing[32];for (int i=0;i<32;i++){LeadingLeading[i]=0.0;}
 
 
 
for( fTimeDiffLoop = 0; fTimeDiffLoop < 18; fTimeDiffLoop++)
{

int i = fLeadingLoop;


//if((fCoarse[i+1] <  fCoarse[i+2])&& (fCoarse[i+1] > 0) && (fCoarse[i+3] < fCoarse[i+4]) && (fCoarse[i+3] > 0))
//if((fCoarse[i+1] > 0) && (fCoarse[i+2] >0)  && (fCoarse[i+3] > 0)  && (fCoarse[i+4] > 0))
//{
	
	
	
	LeadingLeading[fTimeDiffLoop]= fCoarse[i+1] - fCoarse[i+3];
	TrailingTrailing[fTimeDiffLoop]= fCoarse[i+2] - fCoarse[i+4];
	TOT_value[fTimeDiffLoop]= fCoarse[i+2] - fCoarse[i+1];

fLeadingLoop = fLeadingLoop+2;


}


for( int kk = 0; kk< 18; kk++)
{


if (TOT_value[kk] >3.5  &&  TOT_value[kk+1] >3.5 ){
fCr3Ch[kk] ->Fill(LeadingLeading[kk] );          // Leading -Leading 
fCr4Ch[kk] ->Fill(TrailingTrailing[kk]);      // Trailing - Trailing
fCr5Ch[kk] ->Fill(TOT_value[kk]);             // TOT	
}
//fCr7Ch[kk] -> Fill(TOT_value[kk],LeadingLeading[kk]);
fCr7Ch[kk] -> Fill(TOT_value[kk],TOT_value[kk]);



}



// if ((TOT_value[0] >4.63)&&(TOT_value[0]<4.75)){ // 207 Bi
 if ((TOT_value[0] >3.8)&&(TOT_value[0]<5.2)){
 

fCr12Ch[0] ->Fill(LeadingLeading[0] +0.78*TOT_value[0]);    // Leading -Leading 

}



//fCr7Ch[0] ->Fill(TOT_value[0],LeadingLeading[0] );
//fCr7Ch[1] ->Fill(TOT_value[1],LeadingLeading[0] );
//fCr7Ch[2] ->Fill(TOT_value[14],LeadingLeading[14] );
//fCr7Ch[3] ->Fill(TOT_value[15],LeadingLeading[14] );

//if((fCoarse[i+1] >0) && (fCoarse[i+3]>0) && TOT_value[fTimeDiffLoop]>0){ fCr3Ch[fTimeDiffLoop] ->Fill(fCoarse[i+1] - fCoarse[i+3] );}// Leading -Leading
//if((fCoarse[i+2] >0) && (fCoarse[i+4]>0) && TOT_value[fTimeDiffLoop]>0){ fCr4Ch[fTimeDiffLoop] ->Fill(fCoarse[i+2] - fCoarse[i+4] );}// Trailing - Trailing
//if((fCoarse[i+1] >0) && (fCoarse[i+2]>0)){ fCr5Ch[fTimeDiffLoop] ->Fill(fCoarse[i+2] - fCoarse[i+1] );}// TOT





for( int i= 0; i < 16; i++)
{
//if(( TOT_value[i+1]>0) && (TOT_value[i]>0)) 
//{
if((TOT_value[14] >1)&&(TOT_value[15]>1)){
//	fCr8Ch[i]->Fill(TOT_value[i+1]-TOT_value[i]);
fCr12Ch[14]-> Fill(LeadingLeading[14]); 

}
//}
}




fLeadingLoop=0;
fTimeDiffLoop=0;
















//PrintCheckTamexHeaderData(); // Check using varbose output

// cout << "I am here 1" << endl;

}//end of Finger Subevent
/********************************End unpacker FINGER*****************************/
// cout << "I am here 2" << endl;
   


	
			    

 } //subevt while
  out_evt->SetValid(isValid);

  return isValid;
}


void TSCNUnpackProc::PrintCheckTamexHeaderData()
{

   cout << "PreTriggerWin:" << tamexheader.PreTriggerWindow << "PostTriggerWin:" << tamexheader.PostTriggerWindow << endl;
   cout << "SFP ID: " << tamexheader.SFPID << endl;
   cout << "TAMEX ID: " << tamexheader.TamexID << endl;
   cout << "Unused Channel ID: " << tamexheader.UnusedChannelId << "(must be 0)"<<endl;
   cout << "Size in words: 	" <<  tamexheader.SizeOfDataInWords << endl;
   cout << "TDC header Starts here:" << tamexheader.TDCHeader <<"   Should be: "<< 0xaa << endl;
   cout << "TDC trailer here:" << tamexheader.TDCTrailer <<"   Should be: "<< 0xbb << endl;
}

void TSCNUnpackProc::PrintCheckTamexTDCData()
{
              cout << "TDC Channel Fired. Channel Id" << tamexdata.ChannelNo << endl;
 //             tamexdata.ChannelNo=0;
               if(tamexdata.LeadingEdge==true){cout << "Leading edge" << endl;}
               tamexdata.LeadingEdge==false;
               if(tamexdata.TrailingEdge==true){cout << "Trailing edge" << endl;}
               tamexdata.TrailingEdge==false;
}


void  TSCNUnpackProc::SetFineTimeFactor( int id, int channel, double  factor)
{
A[id][channel]= factor;

}

double TSCNUnpackProc::GetFineTimeFactor(unsigned int id, int channel) 
{
	return A[id][channel]; 
}


/*
void TSCNUnpackProc::FillTamexHist()
{
 //if(tamexdata.NHits==1){
              fCr1Ch[tamexdata.ChannelNo]->Fill(tamexdata.CoarseTime); //fine time (needs calibration * 5000 ps)
              fCr2Ch[tamexdata.ChannelNo]->Fill(tamexdata.FineTime); //coarse time (ns)
              fCr3Ch[tamexdata.ChannelNo]->Fill(tamexdata.CoarseTime+tamexdata.FineTime);
//}
}

*/
