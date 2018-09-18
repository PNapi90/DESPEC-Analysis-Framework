#include "XY_Matrix.h"

using namespace std;

//---------------------------------------------------------------

XY_Matrix::XY_Matrix(int am_threads,int max_len){
    
    len_X = 0;
    len_Y = 0;
    this->am_threads = am_threads;
    this->max_len = max_len;
    
    am_threads_d = (double) am_threads;

    amount_of_deleted_events = 0;
    amount_of_events = 0;

    deleted_events = new int[max_len];
    for(int i = 0;i < max_len;++i) deleted_events[i] = -1;

    NULL_Arrays();

}

//---------------------------------------------------------------

XY_Matrix::~XY_Matrix(){
    Cluster_X = nullptr;
    Cluster_Y = nullptr;

    delete[] deleted_events;
}

//---------------------------------------------------------------

void XY_Matrix::Process(TX_Matrix* Cluster_X,TX_Matrix* Cluster_Y){
    this->Cluster_X = Cluster_X;
    this->Cluster_Y = Cluster_Y;

    //set data arrays
    get_Arrays();

    //get amount of points per thread
    get_amount_points_per_thr();

    amount_of_deleted_events = 0;
    
    //do parallel processing
    thread t[am_threads];
    for(int i = 0;i < am_threads;++i) t[i] = threading(i);
    for(int i = 0;i < am_threads;++i) t[i].join();

    //reset data arrays
    NULL_Arrays();

}

//---------------------------------------------------------------

void XY_Matrix::get_Arrays(){

    //amount of Clusters for X and Y (1d array)
    len_X = Cluster_X->get_len();
    len_Y = Cluster_Y->get_len();

    //amount of fired strips in each cluster (2d array)
    Cluster_X_len = Cluster_X->get_len_array();
    Cluster_Y_len = Cluster_Y->get_len_array();
    
    //Time (mean) of each Cluster (1d array)
    Cluster_X_Time = Cluster_X->get_Time();
    Cluster_Y_Time = Cluster_Y->get_Time();

    //Summed energy of each Cluster (1d array)
    Cluster_X_Energy = Cluster_X->get_Energy();
    Cluster_Y_Energy = Cluster_X->get_Energy();

    //Energies for each strip (2d array)
    Cluster_X_Energies = Cluster_X->get_Energies();
    Cluster_Y_Energies = Cluster_Y->get_Energies();
}

//---------------------------------------------------------------

void XY_Matrix::NULL_Arrays(){
    Cluster_X = nullptr;
    Cluster_Y = nullptr;
    
    Cluster_X_Time = nullptr;
    Cluster_Y_Time = nullptr;

    Cluster_X_len = nullptr;
    Cluster_Y_len = nullptr;

    Cluster_X_Energy = nullptr;
    Cluster_Y_Energy = nullptr;

    Cluster_X_Energies = nullptr;
    Cluster_Y_Energies = nullptr;
}

//---------------------------------------------------------------

void XY_Matrix::get_DATA(Raw_Event* RAW){
    //get data for xy at plane "local_z"
}

void XY_Matrix::get_amount_points_per_thr(){
    data_points_per_thr_x = len_X/am_threads;
    double amount_of_data_points_d = (double) data_points_per_thr_x;
    double remaining = amount_of_data_points_d/am_threads_d - data_points_per_thr_x;
    data_points_per_thr_last_x = ((int) remaining*am_threads) + data_points_per_thr_x;

    data_points_per_thr_y = len_Y/am_threads;
    amount_of_data_points_d = (double) data_points_per_thr_y;
    remaining = amount_of_data_points_d/am_threads_d - data_points_per_thr_y;
    data_points_per_thr_last_y = ((int) remaining*am_threads) + data_points_per_thr_y;
}

//---------------------------------------------------------------

void XY_Matrix::Thread_XY(int thr_num){
    //check bools for pairing
    bool in_Energy = false;
    bool in_time = false;

    int data_points_per_thr_x_tmp = (thr_num == am_threads - 1) ? data_points_per_thr_last_x : data_points_per_thr_x;
    int data_points_per_thr_y_tmp = (thr_num == am_threads - 1) ? data_points_per_thr_last_y : data_points_per_thr_y;
    
    //get start values for loops based on thread number
    int start_x = thr_num*data_points_per_thr_x_tmp;
    int start_y = thr_num*data_points_per_thr_y_tmp;
    double* pos_tmp = new double[2];
    
    //arrays for already paired events (bools initialized as false)
    bool used_X[data_points_per_thr_x_tmp];
    bool used_Y[data_points_per_thr_y_tmp];

    for(int i = 0;i < data_points_per_thr_x_tmp;++i) used_X[i] = false;
    for(int i = 0;i < data_points_per_thr_y_tmp;++i) used_Y[i] = false;
    
    int XY_counter = 0;

    //loop over x strips and y strips
    for(int x_counter = start_x;x_counter < data_points_per_thr_x_tmp + start_x;++x_counter){
        if(used_X[x_counter]) continue;
        for(int y_counter = start_y;y_counter < data_points_per_thr_y_tmp + start_y;++y_counter){
            if(used_Y[y_counter]) continue;
            //compare energies and times of x and y clusters for pairing
            in_time = abs(Cluster_X_Time[x_counter] - Cluster_Y_Time[y_counter]) < max_T;
            
            //WRONG!!!
            in_Energy = abs(Cluster_X_Energy[x_counter] - Cluster_Y_Energy[y_counter]) < max_E;
            
            //if pairable x and y, create new XY cluster as possible beta decay
            if(in_time && in_Energy){
                Form_XY(pos_tmp,x_counter,y_counter);
                XY_counter = get_XY_Counter();
                //XY_Clusters[XY_counter] = new XY_Cluster(Cluster_X[x_counter],Cluster_Y[y_counter]);
                //pair found -> continue with next cluster
                used_X[x_counter] = true;
                used_Y[y_counter] = true;
                break;
            }
        }
    }

    delete[] pos_tmp;
    pos_tmp = nullptr;
}

//---------------------------------------------------------------

inline int XY_Matrix::get_XY_Counter(){
    int fill_value = 0;

    //takes care of possible overlaps in increments
    lock_guard<mutex> lockGuard(MUTEX);

    if(amount_of_deleted_events == 0){
        fill_value = amount_of_events;
        amount_of_events++;
    }
    else{
        fill_value = deleted_events[amount_of_deleted_events-1];
        amount_of_deleted_events--;
    }

    return fill_value;
}

//---------------------------------------------------------------

thread XY_Matrix::threading(int j){
    return thread([=] {Thread_XY(j);});
}

//---------------------------------------------------------------

inline void XY_Matrix::Form_XY(double* pos,int x_counter,int y_counter){
    //set amount of relevant x and y strips for cluster
    int x_len = Cluster_X_len[x_counter][1] - Cluster_X_len[x_counter][0];
    int y_len = Cluster_Y_len[y_counter][1] - Cluster_Y_len[y_counter][0];
    double mu_x = 0;
    double mu_y = 0;

    int pos_tmp = Cluster_X_len[x_counter][0];

    //Form center of energies for x and y
    for(int i = 0;i < x_len;++i){
        mu_x += Cluster_X_Energies[x_counter][i]*pos_tmp;
        pos_tmp++;
    }

    pos_tmp = Cluster_Y_len[y_counter][0];
    for(int i = 0;i < y_len;++i){
        mu_y += Cluster_Y_Energies[y_counter][i]*pos_tmp;
        pos_tmp++;
    }

    mu_x /= Cluster_X_Energy[x_counter];
    mu_y /= Cluster_Y_Energy[y_counter];

    //position of beta cluster set to (mu_x,mu_y)
    pos[0] = mu_x;
    pos[1] = mu_y;
}

//---------------------------------------------------------------
