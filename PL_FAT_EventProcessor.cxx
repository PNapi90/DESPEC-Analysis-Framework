#include "PL_FAT_EventProcessor.h"

//---------------------------------------------------------------

PL_FAT_EventProcessor::PL_FAT_EventProcessor() : PLASTIC_Passed(false) , 
                                                 FATIMA_Passed(false)
{
    FILE = new TFile("PL_FAT_EventBuild.root","RECREATE");

    char Name[50];

    DATA_F = nullptr;
    DATA_P = nullptr;
    DATA_VME_P = nullptr;

    TDiff = new TH1D**[MAX_FAT];
    for(int i = 0;i < MAX_FAT;++i){
        TDiff[i] = new TH1D*[MAX_PL];
        for(int j = 0;j < MAX_PL;++j){
            TDiff[i][j] = nullptr;
            //FILE->Add(TDiff[i][j]);
        }
    }

}

//---------------------------------------------------------------

PL_FAT_EventProcessor::~PL_FAT_EventProcessor(){
    FILE->Write();
}

//---------------------------------------------------------------

void PL_FAT_EventProcessor::PassEvent_PLASTIC(PLASTIC_DataStruct* DATA_P){

    this->DATA_P = DATA_P;

    PLASTIC_Passed = true;
}

//---------------------------------------------------------------

void PL_FAT_EventProcessor::PassEvent_PLASTIC_VME(PLASTIC_VME_DataStruct* DATA_VME_P){

    this->DATA_VME_P = DATA_VME_P;

    PLASTIC_Passed = true;
}

//---------------------------------------------------------------

void PL_FAT_EventProcessor::PassEvent_FATIMA(FATIMA_DataStruct* DATA_F){

    this->DATA_F = DATA_F;

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
    int FAT_Fired = DATA_F->FAT_DET_FIRED;
    int PL_Fired = PLASTIC_VME ? DATA_VME_P->TDC_iterator : DATA_P->iterator[0];

    if(PL_Fired >= MAX_PL || FAT_Fired >= MAX_FAT) return;

    double Diff_FAT_PL = 0;
    

    int FAT_Ch = 0, PL_Ch = 0;
    int tmpIter = 0;


    //loop over FATIMA
    for(int i = 0;i < FAT_Fired;++i){
        FAT_Ch = DATA_F->FAT_id[i];
        for(int j = 0;j < PL_Fired;++j){

            if(PLASTIC_VME){
                PL_Ch = DATA_VME_P->VME_TDC_Channels[j];
                Diff_FAT_PL = DATA_F->FAT_t[i] - DATA_VME_P->VME_TDC_Data[j];
            }
            else{
                PL_Ch = DATA_P->ch_ID[0][j];
                tmpIter = DATA_P->trailing_hits_ch[0][PL_Ch];
                Diff_FAT_PL = DATA_F->FAT_t[i] - DATA_P->Time_Trail[PL_Ch][tmpIter];
            }
            Diff_FAT_PL /= 1000.;

			if(abs(FAT_Ch) <= MAX_FAT && abs(PL_Ch) <= MAX_PL){
                if(!TDiff[FAT_Ch][PL_Ch]){
                    char Name[50];
                    sprintf(Name,"Time_Difference_FAT_%d_PL_%d",FAT_Ch,PL_Ch);
                    TDiff[FAT_Ch][PL_Ch] = new TH1D(Name,Name,4000,-2000,2000);
                }
                TDiff[FAT_Ch][PL_Ch]->Fill(Diff_FAT_PL);
            }
            //TWalk_MAT[FAT_Ch][PL_Ch]->Fill(ToT,Diff_FAT_PL);
        }
    }
    DATA_F = nullptr;
    DATA_P = nullptr;
    DATA_VME_P = nullptr;
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
