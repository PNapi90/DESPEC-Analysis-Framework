#ifndef GAP_HANDLER_H
#define GAP_HANDLER_H

class Gap_Handler{

private:

    const int MAX_GAPS = 100000;

    int** GAPS_Strip_amount;
    int*** GAPS_Strip;

    int GAPS_Beta_Ev_amount;
    int* GAPS_Beta_Ev;

    int GAPS_Beta_Cluster_amount;
    int* GAPS_Beta_Cluster;


public:
    Gap_Handler();
    ~Gap_Handler();

    int get_Beta_Ev_GAP();
    int get_Beta_Cluster_GAP();
    int get_Strip_GAP(int,int);

    void set_Beta_Ev_GAP(int);
    void set_Beta_Cluster_GAP(int);
    void set_Strip_Event_GAP(int,int,int);
    
};

#endif