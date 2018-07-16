#include "TFRSUnpackEvent.h"

#include "Riostream.h"
#include "TFRSUnpackProc.h"
#include "Go4EventServer/TGo4FileSource.h"


TFRSUnpackEvent::TFRSUnpackEvent() : TGo4EventElement("FRSUnpackEvent")
				     //  fxProc(0),fxFileSrc(0)
{  }

TFRSUnpackEvent::TFRSUnpackEvent(const char* name) 
  :TGo4EventElement(name)
   //fxProc(0), fxFileSrc(0)
{  }

TFRSUnpackEvent::~TFRSUnpackEvent()
{  }

Int_t TFRSUnpackEvent::Init() 
{ 
  Int_t rev=0;
  Clear();
  // if(CheckEventSource("TFRSUnpackProc")){
  //   fxProc = (TFRSUnpackProc*)GetEventSource();
  //   std::cout << "**** " << GetName() << " will be filled by " << fxProc->GetName() << " ****" << std::endl;
  // } else
  // if(CheckEventSource("TGo4FileSource")) {
  //   fxFileSrc = (TGo4FileSource*)GetEventSource();
  //   std::cout << "**** " << GetName() << " will be filled by File Source ****"<< std::endl;
  // } else rev=1;
  return rev;
}

// Int_t TFRSUnpackEvent::Fill()
// { 
//   Int_t rev=0;
//   Clear();
//   if(fxProc)fxProc->FRSUnpack(this); else  // user event processing method
//   if(fxFileSrc)fxFileSrc->BuildEvent(this); // method from framework to restore event from file
//   return rev;
// }

void TFRSUnpackEvent::Clear(Option_t *t)
{
  EventFlag = -1;
  
  for(int i=0;i<21;i++)
    {
      for(int j=0;j<32;j++)
	{
	  vme0[i][j] = 0;  
	  vme1[i][j] = 0;	       
	  vme3[i][j] = 0;	       
	  vme4[i][j] = 0;
	  vme5[i][j] = 0;
	}
    }
  for(int j=0;j<32;j++)
    {
      vme2scaler[j]=0;
      vme3scaler[j]=0;
    }
  
  for (int i = 0; i < 32; ++i)
    {
      for (int j = 0; j < 10; ++j)
	{
	  vme2s[i][j] = -10;
	  vme2s_trailing[i][j] = -10;
	  vme3s_MT[i][j] = -10;
	  vme3s_MT_trailing[i][j] = -10;
	}    
      nhit5[i][1] = 0;
      nhit5[i][0] = 0;
     
      vme3_MT_nhit5[i][0] = 0;
      vme3_MT_nhit5[i][1] = 0;
    }  

   qlength = 0;
   qtype = 0;
   qsubtype = 0;
   qdummy = 0;
   qtrigger = 0;
   qevent_nr = 0;
}

ClassImp(TFRSUnpackEvent)
