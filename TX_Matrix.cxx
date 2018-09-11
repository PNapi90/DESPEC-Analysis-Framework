#include "TX_Matrix.h"

using namespace std;

//---------------------------------------------------------------

TX_Matrix::TX_Matrix(int strip_iterator,int am_threads){

    cout << "Initialize TX_Matrix " << strip_iterator << endl;

    //load_thread_file();
    save_iter = 0;
    amount_of_data_points = 0;
    this->am_threads = am_threads;
    cluster_counter = new int[am_threads];

    x_or_y = (strip_iterator % 2 == 1);
    z_strip_number = strip_iterator/((int) 2);

    T_Rows = new T_Matrix_Row*[max_len];
    skip_arr = new bool[max_len];
    relevant_for_x = new int*[max_len];
    len_line_X = new int[max_len];
    
    Time_sent = new ULong64_t[max_len];
    Energy_sent = new double[max_len];
    Energies_sent = new double*[max_len];

    Time_Arr_Save = new ULong64_t[max_len];
    Energy_Arr_Save = new double[max_len];
    X_Arr_Save = new int[max_len];

    Cluster_IDs = new int*[max_len];
   
    for(int i = 0;i < max_len;++i){

        len_line_X[i] = 0;
        skip_arr[i] = false;
        //process_mem_usage(i);
        try{
            T_Rows[i] = new T_Matrix_Row();
        }
        catch(const std::bad_alloc&){
            cerr << "std::bad_alloc occured!" << endl;
            cerr << "Problem occured in T_Rows" << endl;
            exit(0);
        }
        Time_sent[i] = 0;
        Energy_sent[i] = 0;
        Energies_sent[i] = new double[128];

        for(int j = 0;j < 128;++j) Energies_sent[i][j] = 0;

        Cluster_IDs[i] = new int[2];
        for(int j = 0;j < 2;++j) Cluster_IDs[i][j] = 0;

        Time_Arr_Save[i] = 0;
        Energy_Arr_Save[i] = 0;
        X_Arr_Save[i] = 0;
    }
    cout << endl;

    Thr_Time_Array = new ULong64_t*[am_threads];
    for(int i = 0;i < am_threads;++i){
        cluster_counter[i] = 0;
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
    for(int i = 0;i < max_len;++i){
        if(relevant_for_x[i]) delete[] relevant_for_x[i];
        delete[] Cluster_IDs[i];
        delete[] Energies_sent[i];
    }
    delete[] Energies_sent;
    delete[] Energy_sent;
    delete[] Cluster_IDs;
    delete[] relevant_for_x;
    delete[] Thr_Time_Array;
    delete[] T_Rows;
    delete[] skip_arr;
    delete[] len_line_X;

    delete[] Time_Arr_Save;
    delete[] Energy_Arr_Save;
    delete[] X_Arr_Save;
    delete[] cluster_counter;
    delete[] Time_sent;
}

//---------------------------------------------------------------

void TX_Matrix::process_mem_usage(int iter){
    double vm_usage     = 0.0;
    double resident_set = 0.0;

    // the two fields we want
    unsigned long vsize;
    long rss;
    {
        std::string ignore;
        std::ifstream ifs("/proc/self/stat", std::ios_base::in);
        ifs >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> vsize >> rss;
    }

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    vm_usage = vsize / (1024.0*1024.0);
    resident_set = rss * page_size_kb;
    cout << "\r";
    cout << "MEMORY USED @ iteration " << iter << " -> ";
    cout << "VM: " << vm_usage << "; RSS: " << resident_set << "\t\t\t";
    cout.flush();
}

//---------------------------------------------------------------

void TX_Matrix::Process(int* X_Arr,ULong64_t* Time_Arr,double* Energy_Arr,int len,int thr_it){

    iterator_mutex = 0;

    cout << "LEN of Process evt " << len << endl;

    //set latest measured time for time comparison
    Time_Last = Time_Arr[len-1];

    //array of energies for energy comparison
    this->Energy_Arr = Energy_Arr;

    amount_of_data_points = len;
    
    //array of WR times
    this->Time_Arr = Time_Arr;

    //positions on respective plane (x/y coordinate)
    this->X_Arr = X_Arr;

    //data point splitting for threading
    data_points_per_thr = amount_of_data_points/am_threads;
    amount_of_data_points_d = (double) amount_of_data_points;
    double am_threads_d = (double) am_threads;
    double remaining = amount_of_data_points_d/am_threads_d - data_points_per_thr;
    data_points_per_thr_last = ((int) remaining*am_threads) + data_points_per_thr;
    
    cout << "THREADS" << endl;
    cout << data_points_per_thr << " " << remaining << " " << data_points_per_thr_last << endl;
    
    //sub threads for each xyz plane (standard = 1)
    thread t[am_threads];

    //check time differences between all events using threads
    cout << "Starting threads with " << am_threads << endl; 
    for(int i = 0;i < am_threads;++i) t[i] = threading(true,i);
    for(int i = 0;i < am_threads;++i) t[i].join();

    int* deleteable_rows = nullptr;

    int rel_counter = 0;

    for(int i = 0;i < amount_of_data_points;++i){
        //skip data points that already exist in events before
        if(skip_arr[i]) continue;
        
        //create coincidence matrix without 0 values
        len_line_X[i] = T_Rows[i]->get_Relevant_amount();
        deleteable_rows = T_Rows[i]->get_Relevant_Evts();

        if(len_line_X[i] > 0) relevant_for_x[i] = new int[len_line_X[i]];
        
        //loop over coincident events of line i
        for(int j = 0;j < len_line_X[i];++j){
            //if event j is present in line i, line j is ignored
            skip_arr[deleteable_rows[j]] = (deleteable_rows[j] != i);
            relevant_for_x[i][j] = deleteable_rows[j];
        }
        
        //if event i has no coincidence and time difference to
        //latest AIDA event is short enough, event is saved
        if(keep_Event(i)) Save_Matrix_Row(i);
        
        deleteable_rows = nullptr;
    }
    //print_COINC_MAT();

    //check if coincident events are neighbors (using threads)
    for(int i = 0;i < am_threads;++i) t[i] = threading(false,i);
    for(int i = 0;i < am_threads;++i) t[i].join();
    
    //set times
    set_Time_and_Energy();

    //reset data to prevent memory leaks
    this->X_Arr = nullptr;
    this->Time_Arr = nullptr;
    this->Energy_Arr = nullptr;

    cout << "nullptr set" << endl;

    for(int i = 0;i < amount_of_data_points;++i){
        skip_arr[i] = false;
        if(relevant_for_x[i]) delete[] relevant_for_x[i];
        cout << "\r";
        cout << "Row " << i << " deleted\t\t\t\t";
        cout.flush();
    }
    cout << endl;
    cout << "Event done!" << endl;
}

//---------------------------------------------------------------

void TX_Matrix::print_COINC_MAT(){
    cout << "-----MATRIX-----" << endl;
    for(int i = 0;i < amount_of_data_points;++i){
        if(relevant_for_x[i]){
            if(X_Arr[i] > 99) cout << X_Arr[i] << " | ";
            else if(X_Arr[i] > 9) cout << "0" << X_Arr[i] << " | ";
            else cout << "00" << X_Arr[i] << " | ";
            for(int j = 0;j < len_line_X[i];++j){
                cout << setw(4) << X_Arr[relevant_for_x[i][j]];
                cout.flush();
            }
            cout << endl;
        }
    }
    cout << "----------------" << endl;
}

//---------------------------------------------------------------

inline bool TX_Matrix::keep_Event(int i){
    bool empty = (len_line_X[i] == 0);
    bool late_enough = (Time_Last - Time_Arr[i] < Time_tolerance);

    if(empty && !late_enough) skip_arr[i] = true;

    return (empty && late_enough);
}

//---------------------------------------------------------------

void TX_Matrix::Save_Matrix_Row(int i){
    Time_Arr_Save[save_iter] = Time_Arr[i];
    X_Arr_Save[save_iter] = X_Arr[i];
    Energy_Arr_Save[save_iter] = Energy_Arr[i];

    skip_arr[i] = true;
    
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
    cout << "Thread_T " << row_start << " -> " << data_points_per_thr+row_start << endl;
    for(int i = row_start;i < data_points_per_thr_tmp+row_start;++i){
        T_Rows[i]->set_Row(Time_Arr,Time_Arr[i],i,amount_of_data_points); 
    }
}

//---------------------------------------------------------------

bool TX_Matrix::sortFunc(const vector<int>& p1,const vector<int>& p2){
    return p1[0] < p2[0];
}

//---------------------------------------------------------------

void TX_Matrix::Thread_X(int thr_num){
    //counter of formed clusters set to 0
    cluster_counter[thr_num] = 0;
    
    //temporary sort array for position sorting
    vector<vector<int> > xy_for_sort(1000,vector<int>(2,10000));
    
    bool check_bool = (thr_num == am_threads - 1);
    cout << check_bool << " " << thr_num << " " << am_threads << endl;
    int data_points_per_thr_tmp = (thr_num == am_threads - 1) ? data_points_per_thr_last : data_points_per_thr;
    int row_start = thr_num*data_points_per_thr_tmp;

    //temporary cluster for sorting
    int tmp_cluster[200][2] = {0};

    int delta_c = 0;
    int c_counter = -1;
    bool active_cluster = false;
    int cluster_of_interest = 0;
    int cluster_of_interest_len = 0;
    
    //loop over all events in thread
    for(int i = row_start;i < data_points_per_thr_tmp+row_start;++i){
        //skip if event not of interest (see Process(...))
        if(skip_arr[i]) continue;
        if(!relevant_for_x[i]){
            cerr << "Skip array pointer exception in TX_Matrix" << endl;
            exit(0);
        }
        //save (tmp) coordinates in xy_for_sort
        for(int j = 0;j < len_line_X[i];++j){
            xy_for_sort[j][0] = X_Arr[relevant_for_x[i][j]];
            xy_for_sort[j][1] = relevant_for_x[i][j];
        }

        //...[0] sorting value, ...[1] position in array
        cout << "-------------------" << endl;
        cout << "before " << i+1 << " " << row_start << " " << data_points_per_thr_tmp+row_start << endl;
        xy_for_sort[len_line_X[i]][0] = X_Arr[i];
        xy_for_sort[len_line_X[i]][1] = i;
        for(int k = 0;k < len_line_X[i]+1;++k) cout << xy_for_sort[k][0] << " " << xy_for_sort[k][1] << endl;
        //sort values by increasing position value
        sort(xy_for_sort.begin(),xy_for_sort.begin()+len_line_X[i]+1,
             [this](const vector<int>& p1, const vector<int>& p2) {return sortFunc(p1, p2);});
        cout << "Sort array" << endl;
        for(int k = 0;k < len_line_X[i]+1;++k) cout << xy_for_sort[k][0] << " " << xy_for_sort[k][1] << endl;

        //check if points are neighbors (and how long a list of neighbors is)
        for(int j = 0;j <= len_line_X[i];++j){

            active_cluster = (xy_for_sort[j][1] == i);

            delta_c = (j == 0) ? -1 : xy_for_sort[j][0] - xy_for_sort[j-1][0];
                
            if(delta_c == 1) tmp_cluster[c_counter][1]++;
            else{
                cout << "counter: " << c_counter << endl;
                c_counter++;
                tmp_cluster[c_counter][0] = xy_for_sort[j][0];
                tmp_cluster[c_counter][1] = xy_for_sort[j][0];
            }
            if(active_cluster) cluster_of_interest = c_counter;
        }

        cluster_of_interest_len = tmp_cluster[cluster_of_interest][1] - tmp_cluster[cluster_of_interest][0];
        
        //save clusters -- those clusters are directly used as output events of AIDA as 
        //possible beta decay events
        cout << "next " << i+1 << " " << row_start << " " << data_points_per_thr_tmp+row_start << endl;
        //===========================================================
        MUTEX.lock();

        Cluster_IDs[iterator_mutex][0] = tmp_cluster[c_counter][0];
        Cluster_IDs[iterator_mutex][1] = tmp_cluster[c_counter][1];
        cluster_counter[thr_num]++;
        iterator_mutex++;

        MUTEX.unlock();
        //===========================================================
        c_counter = 0;
        cout << "next " << i+1 << " " << row_start << " " << data_points_per_thr_tmp+row_start << endl;
    }
    cout << "THREAD_X done" << endl;
}

//---------------------------------------------------------------

thread TX_Matrix::threading(bool i,int j){
    if(i) return thread([=] {Thread_T(j);});
    else return thread([=] {Thread_X(j);});
}

//---------------------------------------------------------------

void TX_Matrix::set_Time_and_Energy(){
    cout << "Setting E and T" << endl;
    int tmp_diff = 0;
    double tmp_sum = 0;
    int tmp_pos = 0;
    for(int i = 0;i < iterator_mutex;++i){
        Time_sent[i] = Time_Arr[Cluster_IDs[i][0]];
        
        tmp_sum = 0;
        tmp_diff = Cluster_IDs[i][1] - Cluster_IDs[i][0];
        tmp_pos = Cluster_IDs[i][0];
        cout << "For " << i << " ";
        for(int j = 0;j < tmp_diff;++j){
            Energies_sent[i][j] = Energy_Arr[tmp_pos];
            tmp_sum += Energies_sent[i][j];
            tmp_pos++;
        }
        Energy_sent[i] = tmp_sum;
        cout << Energy_sent[i] << " " << Time_sent[i] << endl;
    }
    cout << "DONE" << endl;
}

//---------------------------------------------------------------

ULong64_t* TX_Matrix::get_Time(){
    return Time_sent;
}

//---------------------------------------------------------------

int TX_Matrix::get_len(){
    return iterator_mutex;
}

//---------------------------------------------------------------

int** TX_Matrix::get_len_array(){
    return Cluster_IDs;
}

//---------------------------------------------------------------

double* TX_Matrix::get_Energy(){
    return Energy_sent;
}

//---------------------------------------------------------------

double** TX_Matrix::get_Energies(){
    return Energies_sent;
}

//---------------------------------------------------------------

