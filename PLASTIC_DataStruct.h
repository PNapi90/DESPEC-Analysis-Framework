#ifndef PLASTIC_DATA_STRUCT_H
#define PLASTIC_DATA_STRUCT_H

typedef unsigned int UInt;


#include <vector>


struct PLASTIC_DataStruct{

    PLASTIC_DataStruct() : amount_hit_tamex(0) , iterator(4,0) , trigger_coarse(100,0) , trigger_fine(100,0) ,
                           leading_hits(4,0) , trailing_hits(4,0) , phys_channel(4,std::vector<int>(17,0)) ,
                           leading_hits_ch(4,std::vector<int>(17,0)) , trailing_hits_ch(4,std::vector<int>(17,0)) ,
                           Time_Lead(4,std::vector<double>(17,0)) , Time_Trail(4,std::vector<double>(17,0)) ,
                           coarse_T_edge_lead(4,std::vector<double>(100,0)) , coarse_T_edge_trail(4,std::vector<double>(100,0)) , 
                           fine_T_edge_lead(4,std::vector<double>(100,0)) , fine_T_edge_trail(4,std::vector<double>(100,0)) , 
                           ch_ID(4,std::vector<UInt>(100,0)) , fired_tamex(4,false),PADI_OR_PADIWA(true){}

    ~PLASTIC_DataStruct(){}
    
    int amount_hit_tamex = 0;

    std::vector<int> iterator = {};
    
    std::vector<double> trigger_coarse = {};
    std::vector<double> trigger_fine = {};

    std::vector<int> leading_hits = {};
    std::vector<int> trailing_hits = {};
    std::vector<std::vector<int> > phys_channel = {};
    std::vector<std::vector<int> > leading_hits_ch = {};
    std::vector<std::vector<int> > trailing_hits_ch = {};

    std::vector<std::vector<double> > Time_Lead = {};
    std::vector<std::vector<double> > Time_Trail = {};
    

    std::vector<std::vector<double> > coarse_T_edge_lead = {};
    std::vector<std::vector<double> > coarse_T_edge_trail = {};
    std::vector<std::vector<double> > fine_T_edge_lead = {};
    std::vector<std::vector<double> > fine_T_edge_trail = {};
    std::vector<std::vector<UInt> > ch_ID = {};
    
    std::vector<bool> fired_tamex = {};
    
    bool PADI_OR_PADIWA = true;



    void SetDATA_Directly(int amount_hit_tamex,std::vector<int> &iterator,std::vector<double> &trigger_coarse,
                          std::vector<double> &trigger_fine,std::vector<int> &leading_hits,std::vector<int> &trailing_hits,
                          std::vector<std::vector<int> > &phys_channel,std::vector<std::vector<int> > &leading_hits_ch,
                          std::vector<std::vector<int> > &trailing_hits_ch,std::vector<std::vector<double> > &Time_Lead,
                          std::vector<std::vector<double> > &Time_Trail,std::vector<std::vector<double> > &coarse_T_edge_lead,
                          std::vector<std::vector<double> > &coarse_T_edge_trail,std::vector<std::vector<double> > &fine_T_edge_lead,
                          std::vector<std::vector<double> > &fine_T_edge_trail,std::vector<std::vector<UInt> > &ch_ID,
                          std::vector<bool> &fired_tamex,bool PADI_OR_PADIWA)
    {   
        this->amount_hit_tamex = amount_hit_tamex;
        this->iterator = iterator;
        this->trigger_coarse = trigger_coarse;
        this->trigger_fine = trigger_fine;
        this->leading_hits = leading_hits;
        this->trailing_hits = trailing_hits;
        this->phys_channel = phys_channel;
        this->leading_hits_ch = leading_hits_ch;
        this->trailing_hits_ch = trailing_hits_ch;
        this->Time_Lead = Time_Lead;
        this->Time_Trail = Time_Trail;
        this->coarse_T_edge_lead = coarse_T_edge_lead;
        this->coarse_T_edge_trail = coarse_T_edge_trail;
        this->fine_T_edge_lead = fine_T_edge_lead;
        this->fine_T_edge_trail = fine_T_edge_trail;
        this->ch_ID = ch_ID;
        this->fired_tamex = fired_tamex;
        this->PADI_OR_PADIWA = PADI_OR_PADIWA;
    }

    void SetDATA(std::vector<int> &it,std::vector<std::vector<double> > &Edge_Coarse,
                 std::vector<std::vector<double> > &Edge_fine, std::vector<std::vector<UInt> > &ch_ed,
                 std::vector<double> &Coarse_Trigger,std::vector<double> &Fine_Trigger,int amount_hit_tamex)
    {
        this->amount_hit_tamex = amount_hit_tamex;
        //reset lead and trail hits
        for(int i = 0;i < amount_hit_tamex;++i){
            for(int j = 0;j < 17;++j){
                leading_hits_ch[i][j] = 0;
                trailing_hits_ch[i][j] = 0;
            }
        }

        //loop over all 4 tamex modules
        for(int i = 0;i < amount_hit_tamex;++i){
            iterator[i] = it[i];
            trigger_coarse[i] = Coarse_Trigger[i];
            trigger_fine[i] = Fine_Trigger[i];
            fired_tamex[i] = (iterator[i] > 0);
            leading_hits[i] = 0;
            trailing_hits[i] = 0;
            for(int j = 0;j < iterator[i];++j){
                ch_ID[i][j] = ch_ed[i][j];
                if(ch_ID[i][j] % 2 == PADI_OR_PADIWA){
                    coarse_T_edge_lead[i][j] = Edge_Coarse[i][j]*5.0;
                    fine_T_edge_lead[i][j] = Edge_fine[i][j]*5.0;
                
                    phys_channel[i][j] = PADI_OR_PADIWA ? (ch_ID[i][j]+1)/2 : (ch_ID[i][j])/2;
                
                    Time_Lead[phys_channel[i][j]][leading_hits_ch[i][phys_channel[i][j]]] = coarse_T_edge_lead[i][j] - fine_T_edge_lead[i][j];
                    
                    ++leading_hits[i];
                    ++leading_hits_ch[i][phys_channel[i][j]];
                }
                else{
                    coarse_T_edge_trail[i][j] = Edge_Coarse[i][j]*5.0;
                    fine_T_edge_trail[i][j] = Edge_fine[i][j]*5.0;
                
                    phys_channel[i][j] = PADI_OR_PADIWA ? (ch_ID[i][j])/2 : (ch_ID[i][j]+1)/2;
                
                
                    Time_Trail[phys_channel[i][j]][trailing_hits_ch[i][phys_channel[i][j]]] = coarse_T_edge_trail[i][j] - fine_T_edge_trail[i][j];
                
                    ++trailing_hits[i];
                    ++trailing_hits_ch[i][phys_channel[i][j]];
                }
            }
        }
    }
};

#endif