#ifndef TXRSUNPACKEVENT_H
#define TXRSUNPACKEVENT_H


#include "TGo4EventElement.h"


class TXRSUnpackProc;
class TGo4FileSource;

class TXRSUnpackEvent : public TGo4EventElement {
public:
  TXRSUnpackEvent();
  TXRSUnpackEvent(const char* name);
  virtual ~TXRSUnpackEvent();
  // /**
  //  * Method called by the event owner (analysis step) to fill the
  //  * event element from the set event source. Event source can
  //  * be the source of the analysis step (if this is a raw event) 
  //  * or the event processor (if this is a reduced event).
  //  */
  // virtual Int_t Fill();

  /**
   * Method called by the event owner (analysis step) to clear the
   * event element.
   */
  virtual void Clear(Option_t *t="");  
      
  virtual Int_t Init();  

  Int_t EventFlag;
  
  UInt_t vme0[21][32];         // FRS crate                                
  UInt_t vme1[21][32];         // TPC crate 
  UInt_t vme3[21][32];         // Mtof crate

  Int_t vme5[21][32];          //Main Crate or User Crate

  Int_t vme2scaler[32];         // User Crate Messhure
  Int_t vme3scaler[32];         // User Crate Messhure

  Int_t vme2s[32][10];           // User TDC (V1290) 
  Int_t vme2s_trailing[32][10];  // User TDC (V1290) 
  Int_t nhit5[32][2];            // multiplicity (V1290)

  Int_t vme3s_MT[32][10];           // Mtof TDC (V1290) 
  Int_t vme3s_MT_trailing[32][10];  // Mtof TDC (V1290) 
  Int_t vme3_MT_nhit5[32][2];       // multiplicity (V1290)
  
  Int_t vme4[21][32] ; // SOFIA crate temp need multiplicity for both MDPP and vftx

 

  
  Int_t         qlength;         /* From event header     */
  Int_t         qtype;           /*                       */
  Int_t         qsubtype;        /*                       */
  Int_t         qdummy;          /*                       */
  Int_t         qtrigger;        /*                       */
  Int_t         qevent_nr;       /*                       */

private:
  //      TXRSUnpackProc *fxProc;     //! This is processor
  //      TGo4FileSource *fxFileSrc;  //! This is file source

  ClassDef(TXRSUnpackEvent,1)         
};
#endif //TXRSUNPACKEVENT_H
