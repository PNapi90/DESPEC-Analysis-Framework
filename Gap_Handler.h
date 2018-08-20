#ifndef GAP_HANDLER_H
#define GAP_HANDLER_H

class Gap_Handler{

private:

    const int MAX_GAPS = 100000;

    int GAPS_Strip_amount;
    int* GAPS_Strip;

    int GAPS_Beta_Ev_amount;
    int* GAPS_Beta_Ev;

    int GAPS_Beta_Cluster_amount;
    int* GAPS_Beta_Cluster;


public:
    Gap_Handler();
    ~Gap_Handler();
    
};

#endif