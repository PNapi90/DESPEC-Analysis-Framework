#include "TX_Matrix.h"

using namespace std;

//---------------------------------------------------------------

TX_Matrix::TX_Matrix(int strip_iterator,int am_threads){
    //load_thread_file();
    amount_of_data_points = 0;
    this->am_threads = am_threads;

    x_or_y = (strip_iterator % 2 == 1);
    z_strip_number = strip_iterator/((int) 2);

    T_Rows = new TX_Matrix_Row*[max_len];
    skip_arr = new bool[max_len];
    relevant_for_x = new int*[max_len];
    len_line_X = new int[max_len];

    Time_Arr_Save = new ULong64_t[max_len];
    Energy_Arr_Save = new double[max_len];
    X_Arr_Save = new int[max_len];

    for(int i = 0;i < max_len;++i){
        len_line_X[i] = 0;
        skip_arr[i] = false;
        T_Rows[i] = new TX_Matrix_Row();
        relevant_for_x[i] = new int[max_len];
        for(int j = 0;j < max_len;++j) relevant_for_x[i][j] = -1;

        Time_Arr_Save[i] = 0;
        Energy_Arr_Save[i] = 0;
        X_Arr_Save[i] = 0;
    }

    Thr_Time_Array = new ULong64_t*[am_threads];
    for(int i = 0;i < am_threads;++i){
        Thr_Time_Array[i] = new ULong64_t[max_len];
        for(int j = 0;j < max_len;++j) Thr_Time_Array[i][j] = 0;
    }

    Time_Arr = nullptr;
    Energy_Arr = nullptr;
    X_Arr = nullptr;

}

//---------------------------------------------------------------

TX_Matrix::~TX_Matrix(){
    for(int i = 0;i < am_threads;++i){
        delete T_Rows[i];
        delete[] Thr_Time_Array[i];
    }
    for(int i = 0;i < max_len;++i) delete[] relevant_for_x[i];
    delete[] relevant_for_x;
    delete[] Thr_Time_Array;
    delete[] T_Rows;
    delete[] skip_arr;
    delete[] len_line_X;

    delete[] Time_Arr_Save;
    delete[] Energy_Arr_Save;
    delete[] X_Arr_Save;
}

//---------------------------------------------------------------

void TX_Matrix::Process(int* X_Arr,ULong64_t* Time_Arr,double* Energy_Arr,int len,int thr_it){

    //set latest measured time for time comparison
    Time_Last = Time_Arr[len-1];

    //array of energies for energy comparison
    this->Energy_Arr = Energy_Arr;

    amount_of_data_points = len;
    
    //array of WR times
    this->Time_Arr = Time_Arr;

    //data point splitting for threading
    data_points_per_thr = amount_of_data_points/am_threads;
    amount_of_data_points_d = (double) amount_of_data_points;
    double am_threads_d = (double) am_threads;
    double remaining = amount_of_data_points_d/am_threads_d - data_points_per_thr;
    data_points_per_thr_last = ((int) remaining*am_threads) + data_points_per_thr;

    int t_counter = 0;
    int tmp_len = 0;

    //sub threads for each xyz plane (standard = 1)
    thread t[am_threads];

    //check time differences between all events using threads
    for(int i = 0;i < am_threads;++i) t[i] = threading(false,i);
    for(int i = 0;i < am_threads;++i) t[i].join();

    int* deleteable_rows = nullptr;

    for(int i = 0;i < amount_of_data_points;++i){
        //skip data points that already exist in events before
        if(skip_arr[i]) continue;
        
        //create coincidence matrix without 0 values
        len_line_X[i] = T_Rows[i]->get_Relevant_amount();
        deleteable_rows = T_Rows[i]->get_Relevant_Evts();
        
        //loop over coincident events of line i
        for(int j = 0;j < len_line_X[i];++j){
            //if event j is present in line i, line j is ignored
            skip_arr[deleteable_rows[j]] = true;
            relevant_for_x[i][j] = deleteable_rows[j];
        }
        
        //if event i has no coincidence and time difference to
        //latest AIDA event is short enough, event is saved
        if(keep_Event(i)) Save_Matrix_Row(i);
    }

    //positions on respective plane (x/y coordinate)
    this->X_Arr = X_Arr;

    //check if coincident events are neighbors (using threads)
    for(int i = 0;i < am_threads;++i) t[i] = threading(true,i);
    for(int i = 0;i < am_threads;++i) t[i].join();

    //reset data to prevent memory leaks
    this->X_Arr = nullptr;
    this->Time_Arr = nullptr;
    this->Energy_Arr = nullptr;

    deleteable_rows = nullptr;
}

//---------------------------------------------------------------

inline bool TX_Matrix::keep_Event(int i){
    bool empty = (len_line_X[i] == 0);
    bool late_enough = (Time_Last - Time_Arr[i] < Time_tolerance);

    return (empty && late_enough);
}

//---------------------------------------------------------------

void TX_Matrix::Save_Matrix_Row(int i){
    Time_Arr_Save[save_iter] = Time_Arr[i];
    X_Arr_Save[save_iter] = X_Arr[i];
    Energy_Arr_Save[save_iter] = Energy_Arr[i];
    
    save_iter++;
}

//---------------------------------------------------------------

void TX_Matrix::set_Saved_To_Streamer(AIDA_Data_Streamer* Stream){
    Stream->Store_and_Purge(x_or_y,X_Arr_Save,Time_Arr_Save,Energy_Arr_Save,z_strip_number,save_iter);
    reset_Saved();
}

//---------------------------------------------------------------

void TX_Matrix::reset_Saved(){
    save_iter = 0;
}

//---------------------------------------------------------------

void TX_Matrix::Thread_T(int thr_num){
    int data_points_per_thr_tmp = (thr_num == am_threads - 1) ? data_points_per_thr_last : data_points_per_thr;
    int row_start = thr_num*data_points_per_thr_tmp;
    for(int i = row_start;i < data_points_per_thr_tmp+row_start;++i){
        T_Rows[i]->set_Row(Time_Arr,Time_Arr[i],i,amount_of_data_points); 
    }
}

//---------------------------------------------------------------

void TX_Matrix::Thread_X(int thr_num){
    //counter of formed clusters set to 0
    cluster_counter[thr_num] = 0;
    
    //temporary sort array for position sorting
    int xy_for_sort[1000][2];

    int data_points_per_thr_tmp = (thr_num == am_threads - 1) ? data_points_per_thr_last : data_points_per_thr;
    int row_start = thr_num*data_points_per_thr_tmp;
    auto sort_ptr = (pair<int,int>*) xy_for_sort;

    //temporary cluster for sorting
    int tmp_cluster[10][2] = {0};

    int delta_c = 0;
    int c_counter = -1;
    bool active_cluster = false;
    int cluster_of_interest = 0;

    //loop over all events in thread
    for(int i = row_start;i < data_points_per_thr_tmp+row_start;++i){
        //skip if event not of interest (see Process(...))
        if(skip_arr[i]) continue;

        //save (tmp) coordinates in xy_for_sort
        for(int j = 0;j < len_line_X[i];++j){
            xy_for_sort[j][0] = X_Arr[relevant_for_x[i][j]];
            xy_for_sort[j][1] = relevant_for_x[i][j];
        }

        //...[0] sorting value, ...[1] position in array
        xy_for_sort[len_line_X[i]][0] = X_Arr[i];
        xy_for_sort[len_line_X[i]][1] = i;
        
        //sort values by increasing position value
        sort_ptr = (pair<int,int>*) xy_for_sort;
        sort(sort_ptr,sort_ptr + len_line_X[i] + 1);

        //check if points are neighbors (and how long a list of neighbors is)
        for(int j = 0;j <= len_line_X[i];++j){

            active_cluster = (xy_for_sort[j][1] == i);

            delta_c = (j == 0) ? -1 : xy_for_sort[j][0] - xy_for_sort[j-1][0];
                
            if(delta_c == 1) tmp_cluster[c_counter][1]++;
            else{
                c_counter++;
                tmp_cluster[c_counter][0] = xy_for_sort[j][0];
                tmp_cluster[c_counter][1] = xy_for_sort[j][0];
            }
            if(active_cluster) cluster_of_interest = c_counter;
        }

        cluster_of_interest_len = tmp_cluster[cluster_of_interest][1] - tmp_cluster[cluster_of_interest][0];
        
        //save clusters -- those clusters are directly used as output events of AIDA as 
        //possible beta decay events
        Cluster_IDs[cluster_counter[thr_num]+thr_offset][0] = tmp_cluster[c_counter][0];
        Cluster_IDs[cluster_counter[thr_num]+thr_offset][1] = tmp_cluster[c_counter][1];
        cluster_counter[thr_num]++;

        sort_ptr = nullptr;
    }
}

//---------------------------------------------------------------

thread TX_Matrix::threading(bool i,int j){
    if(i) return thread([=] {Thread_T(j);});
    else return thread([=] {Thread_X(j);});
}

//---------------------------------------------------------------
