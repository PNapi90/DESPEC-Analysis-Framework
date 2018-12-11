#include "PL_FAT_EventProcessor.h"

//---------------------------------------------------------------

PL_FAT_EventProcessor::PL_FAT_EventProcessor() : PLASTIC_Passed(false) , 
                                                 FATIMA_Passed(false)
{
    FILE = new TFile("PL_FAT_EventBuild","RECREATE");

    char Name[50];

    TDiff = new TH1D**[MAX_FAT];
    for(int i = 0;i < MAX_FAT;++i){
        TDiff[i] = new TH1D*[MAX_PL];
        for(int j = 0;j < MAX_PL;++j){
            sprintf(Name,"Time_Difference_FAT_%d_PL_%d",i,j);
            TDiff[i][j] = new TH1D(Name,Name,500,0,500,500,0,500);
            FILE->Add(TDiff[i][j]);
        }
    }

}

//---------------------------------------------------------------

PL_FAT_EventProcessor::~PL_FAT_EventProcessor(){
    FILE->Write();
}

//---------------------------------------------------------------

void PL_FAT_EventProcessor::PassEvent(PLASTIC_Event* Event){

    if(PLASTIC_VME) DATA_VME_P = Event->GET_PLASTIC_VME();
    else DATA_P = Event->GET_PLASTIC();

    PLASTIC_Passed = true;
}

//---------------------------------------------------------------

void PL_FAT_EventProcessor::PassEvent(FATIMA_Event* Event){

    DATA_F = Event->GET_FATIMA();

    FATIMA_Passed = true;
}

//---------------------------------------------------------------

bool PL_FAT_EventProcessor::AllPassed(){
    //check if all necessary events were passed
    if(PLASTIC_Passed && FATIMA_Passed){
        PLASTIC_Passed = false;
        FATIMA_Passed = false;

        ProcessEvent();

        return true;
    }
    std::cerr << "-----------------------------------------------\n";
    std::cerr << "*** PL_FAT_EventProcessor ***\n";
    std::cerr << "AllPassed called before all elements passed!\n";
    std::cerr << "-----------------------------------------------" << std::endl;
    
    return false;

}

//---------------------------------------------------------------

void PL_FAT_EventProcessor::ProcessEvent(){
    int FAT_Fired = DATA_F->AmountFired;
    int PL_Fired = PLASTIC_VME ? DATA_VME_P->TDC_iterator : DATA_P->AmountFired;

    double Diff_FAT_PL = 0;
    

    int FAT_Ch = 0, PL_Ch = 0;

    //loop over FATIMA
    for(int i = 0;i < FAT_Fired;++i){
        FAT_Ch = DATA_FFiredChannel[i];
        for(int j = 0;j < PL_Fired;++j){

            if(PLASTIC_VME){
                PL_Ch = DATA_VME_P->VME_TDC_Channels[j];
                Diff_FAT_PL = DATA_F->Time[i] - DATA_VME_P->Time_Trail[j];
            }
            else{
                PL_Ch = DATA_P->FiredChannel[j];
                Diff_FAT_PL = DATA_F->Time[i] - DATA_P->Time_Trail[j];
            }

            TDiff[FAT_Ch][PL_Ch]->Fill(Diff_FAT_PL);
            //TWalk_MAT[FAT_Ch][PL_Ch]->Fill(ToT,Diff_FAT_PL);
        }
    }
}

//---------------------------------------------------------------
/*
void PL_FAT_EventProcessor::ProcessEvent(){
    int FAT_Fired = DATA_F.AmountFired;
    int PL_Fired = DATA_P.AmountFired;

    double Diff_FAT_PL = 0;
    double ToT = 0;

    int FAT_Ch = 0, PL_Ch = 0;

    //loop over FATIMA
    for(int i = 0;i < FAT_Fired;++i){
        FAT_Ch = DATA_F.FiredChannel[i];
        for(int j = 0;j < PL_Fired;++j){
            PL_Ch = DATA_P.FiredChannel[j];
            Diff_FAT_PL = DATA_F.Time[i] - DATA_P.Time_Trail[j];
            ToT = DATA_P.TOT[j];

            TWalk_MAT[FAT_Ch][PL_Ch]->Fill(ToT,Diff_FAT_PL);
        }
    }
}
*/
//---------------------------------------------------------------