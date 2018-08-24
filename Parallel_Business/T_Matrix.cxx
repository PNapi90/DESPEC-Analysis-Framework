#include "T_Matrix.h"

using namespace std;

//---------------------------------------------------------------

T_Matrix::T_Matrix(){
    load_thread_file();
    amount_of_data_points = 0;

    T_Rows = new T_Matrix_Row*[max_len];
    skip_arr = new bool[max_len];
    relevant_for_x = new int*[max_len];
    len_line_X = new int[max_len];
    for(int i = 0;i < max_len;++i){
        len_line_X[i] = 0;
        skip_arr[i] = false;
        T_Rows[i] = new T_Matrix_Row();
        relevant_for_x[i] = new int[max_len];
        for(int j = 0;j < max_len;++j) relevant_for_x[i][j] = -1;
    }

    Thr_Time_Array = new ULong64_t*[am_threads];
    for(int i = 0;i < am_threads;++i){
        Thr_Time_Array[i] = new ULong64_t[max_len];
        for(int j = 0;j < max_len;++j) Thr_Time_Array[i][j] = 0;
    }

    

}

//---------------------------------------------------------------

T_Matrix::~T_Matrix(){
    for(int i = 0;i < am_threads;++i){
        delete  T_Rows[i];
        delete[] Thr_Time_Array[i];
    }
    for(int i = 0;i < max_len;++i) delete[] relevant_for_x[i];
    delete[] relevant_for_x;
    delete[] Thr_Time_Array;
    delete[] T_Rows;
    delete[] skip_arr;
    delete[] len_line_X;
}

//---------------------------------------------------------------

void T_Matrix::set_data(Data_Class_Obj* DATA){
    amount_of_data_points = DATA->get_LEN();
    Time_Arr = DATA->get_Time_Array();

    data_points_per_thr = amount_of_data_points/am_threads;

    amount_of_data_points_d = (double) amount_of_data_points;

    double remaining = amount_of_data_points_d/am_threads_d - data_points_per_thr;
    data_points_per_thr_last = ((int) remaining*am_threads) + data_points_per_thr;

    int t_counter = 0;
    int tmp_len = 0;

    thread t[am_threads];

    for(int i = 0;i < am_threads;++i) t[i] = threading(0,i);
    for(int i = 0;i < am_threads;++i) t[i].join();


    int* deleteable_rows = nullptr;

    for(int i = 0;i < amount_of_data_points;++i){
        
        if(skip_arr[i]) continue;
        
        len_line_X[i] = T_Rows[i]->get_Relevant_amount();
        deleteable_rows = T_Rows[i]->get_Relevant_Evts();
        for(int j = 0;j < len_line_X[i];++j){
            skip_arr[deleteable_rows[j]] = true;
            relevant_for_x[i][j] = deleteable_rows[j];
        }

    }

    X_Arr = DATA->get_X_Array();

    for(int i = 0;i < am_threads;++i) t[i] = threading(1,i);



}

//---------------------------------------------------------------

void T_Matrix::Thread_T(int thr_num){
    int row_start = thr_num*data_points_per_thr;
    for(int i = row_start;i < data_points_per_thr+row_start;++i){
        T_Rows[i]->set_Row(Time_Arr,Time_Arr[i],i,amount_of_data_points); 
    }
}

//---------------------------------------------------------------

void T_Matrix::Thread_X(int thr_num){
    cluster_counter[thr_num] = 0;

    int xy_for_sort[1000][2];
    int row_start = thr_num*data_points_per_thr;
    auto sort_ptr = (pair<int,int>*) xy_for_sort;

    int tmp_cluster[10][2];
    for(int i = 0;i < 10;++i){
        tmp_cluster[i][0] = 0;
        tmp_cluster[i][1] = 0;
    }

    int delta_c = 0;
    int c_counter = -1;
    bool active_cluster = false;
    int cluster_of_interest = 0;

    for(int i = row_start;i < data_points_per_thr+row_start;++i){
        if(skip_arr[i]) continue;
        for(int j = 0;j < len_line_X[i];++j){
            xy_for_sort[j][0] = X_Arr[relevant_for_x[i][j]];
            xy_for_sort[j][1] = relevant_for_x[i][j];
        }
        xy_for_sort[len_line_X[i]][0] = X_Arr[i];
        xy_for_sort[len_line_X[i]][1] = i;
        
        sort_ptr = (pair<int,int>*) xy_for_sort;
        sort(sort_ptr,sort_ptr + len_line_X[i] + 1);

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
        
        Cluster_IDs[cluster_counter[thr_num]+thr_offset][0] = tmp_cluster[c_counter][0];
        Cluster_IDs[cluster_counter[thr_num]+thr_offset][1] = tmp_cluster[c_counter][1];
        cluster_counter[thr_num]++;

        sort_ptr = nullptr;
    }
}

//---------------------------------------------------------------

thread T_Matrix::threading(int i,int j){
    if(i == 0) return thread([=] {Thread_T(j);});
    else if(i == 1) return thread([=] {Thread_X(j);});
    else cout << "Error: i " << i << " not known" << endl;
}

//---------------------------------------------------------------

void T_Matrix::load_thread_file(){

    string line;
    char dummy_str[100];

    cout << "\n-------------------------------------" << endl;

    ifstream thr_file("Configuration_Files/THREAD_FILE.txt");
    if(thr_file.fail()){
        cout << "No THREAD_FILE found!" << endl;
        cout << "Using 5 threads for each AIDA xyz plane -> total of 30 threads" << endl;
        am_threads = 5;
    }
    else{
        while(thr_file.good()){
            getline(thr_file,line,'\n');
            if(line[0] == "#") continue;
            sscanf(line.c_str(),"%s %d",dummy_str,&am_threads);
            cout << "Using " << am_threads << " threads for each AIDA xyz plane -> total of " << am_threads*6 << " threads" << endl;
        }
    }
    cout << "-------------------------------------\n" << endl;
    am_threads_d = (double) am_threads;
}

//---------------------------------------------------------------