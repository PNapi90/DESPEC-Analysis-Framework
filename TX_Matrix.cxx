#include "TX_Matrix.h"

using namespace std;

//---------------------------------------------------------------

TX_Matrix::TX_Matrix(int strip_iterator,int am_threads,int* lens_sent){

    cout << "Initialize TX_Matrix " << strip_iterator << endl;
    max_len = lens_sent[3];
    //load_thread_file();
    save_iter = 0;
    amount_of_data_points = 0;
    this->am_threads = am_threads;
    this->strip_iterator = strip_iterator;
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
        
        //try -- catch statement since T_Rows consumes most 
        //part of needed memory
        //----------------------------------------------------
        try{
            T_Rows[i] = new T_Matrix_Row(max_len,lens_sent[2]);
        }
        catch(const std::bad_alloc&){
            process_mem_usage(i,lens_sent[2]);
            exit(0);
        }
        //----------------------------------------------------
        
        Time_sent[i] = 0;
        Energy_sent[i] = 0;
        Energies_sent[i] = new double[128];

        relevant_for_x[i] = nullptr;

        for(int j = 0;j < 128;++j) Energies_sent[i][j] = 0;

        Cluster_IDs[i] = new int[2];
        for(int j = 0;j < 2;++j) Cluster_IDs[i][j] = 0;

        Time_Arr_Save[i] = 0;
        Energy_Arr_Save[i] = 0;
        X_Arr_Save[i] = 0;
    }
    

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

void TX_Matrix::process_mem_usage(int iter,int len_2){
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
    cerr << "==================================================================" << endl;
    cerr << "MEMORY USED @ iteration " << iter << " -> ";
    cerr << "VM: " << vm_usage << " MB" << endl;
    cerr << "std::bad_alloc occured!" << endl;
    cerr << "Problem occured in T_Rows" << endl;
    cerr << "max_len is set to " << max_len;
    double needed_mem = (max_len*len_2)/pow(2.,18.)*6.;
    cerr << " -> " << needed_mem << " MB of RAM needed for T-Matrix" << endl;
    cerr << "----------------------------------------------------------------" << endl;
    cerr << "***HINT***" << endl;
    cerr << "If VM is 8 GB, check maximum allowed virtual memory via ulimit -a" << endl;
    cerr << "Decrease max_len (if possible) or ask System Admin @ GSI for more" << endl;
    cerr << "possible memory per user" << endl;
    cerr << "==================================================================" << endl;
    
}

//---------------------------------------------------------------

void TX_Matrix::Process(int* X_Arr,ULong64_t* Time_Arr,double* Energy_Arr,int len,int thr_it){

    if(len == 0) return;

    iterator_mutex = 0;

    //set latest measured time for time comparison
    Time_Last = Time_Arr[len-1];

    //array of energies for energy comparison
    this->Energy_Arr = Energy_Arr;

    amount_of_data_points = len;
    
    //array of WR times
    this->Time_Arr = Time_Arr;

    //positions on respective plane (x/y coordinate)
    this->X_Arr = X_Arr;

    int dataPosition = 0;

    //g++ version >= 4.9
    #ifdef GPP_FLAG
        //data point splitting for threading
        set_data_points_per_thread();
    
        //sub threads for each xyz plane (standard = 1)
        thread t[am_threads];

        //check time differences between all events using threads
        for(int i = 0;i < amount_of_data_points;++i){
            for(int j = 0;j < am_threads;++j){
                t[j] = threading(true,dataPosition,j);
                ++dataPosition;
            }
            for(int j = 0;j < am_threads;++j) t[j].join();
        }

        //print_COINC_MAT();
    
        //check if coincident events are neighbors (using threads)
        for(int i = 0;i < am_threads;++i) t[i] = threading(false,0,i);
        for(int i = 0;i < am_threads;++i) t[i].join();
    
    //g++ version < 4.9
    #else
        //"thread 1" processes all the data
        data_points_per_thr = amount_of_data_points;
        data_points_per_thr_last = amount_of_data_points;
        
        //processing time differences in serial
        Thread_T(0);

        //processing position differences in serial
        Thread_X(0);


    #endif

    //set times
    set_Time_and_Energy();

    //reset data to prevent memory leaks
    this->X_Arr = nullptr;
    this->Time_Arr = nullptr;
    this->Energy_Arr = nullptr;


    for(int i = 0;i < amount_of_data_points;++i){
        skip_arr[i] = false;
        if(relevant_for_x[i]){
            delete[] relevant_for_x[i];
            relevant_for_x[i] = nullptr;
        }
    }
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

void TX_Matrix::set_data_points_per_thread(){
    data_points_per_thr = amount_of_data_points/am_threads;
    amount_of_data_points_d = (double) amount_of_data_points;
    double am_threads_d = (double) am_threads;
    double remaining = amount_of_data_points_d/am_threads_d - data_points_per_thr;
    data_points_per_thr_last = ((int) remaining*am_threads) + data_points_per_thr;
}

//---------------------------------------------------------------

inline bool TX_Matrix::keep_Event(int i){
    bool empty = (len_line_X[i] == 0);
    bool late_enough = (Time_Last - Time_Arr[i] < Time_tolerance);

    if(empty && !late_enough) skip_arr[i] = true;

    return (empty && late_enough);
}

//---------------------------------------------------------------

inline void TX_Matrix::Save_Matrix_Row(int i){
    
    #ifdef GPP_FLAG
        lock_guard<mutex> lockGuard(MUTEX);
    #endif

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

    int* deleteable_rows = nullptr;
    for(int i = row_start;i < data_points_per_thr_tmp+row_start;++i){
        //skip data points that already exist in events before
        if(check_relevant(i)){
            relevant_for_x[i] = nullptr;
            len_line_X[i] = 0;
            continue;
        }
        T_Rows[i]->set_Row(Time_Arr,Time_Arr[i],i,amount_of_data_points);

        //create coincidence matrix without 0 values
        len_line_X[i] = T_Rows[i]->get_Relevant_amount();
        deleteable_rows = T_Rows[i]->get_Relevant_Evts();

        //set relevant events for row i
        set_relevant(i,len_line_X[i],deleteable_rows);

        deleteable_rows = nullptr;
    }
}

//---------------------------------------------------------------

void TX_Matrix::Thread_T(int thr_num,int rowNumber){

    if(check_relevant(rowNumber)){
        relevant_for_x[rowNumber] = nullptr;
        len_line_X[rowNumber] = 0;
        return;
    }

    T_Rows[rowNumber]->set_Row(Time_Arr,Time_Arr[rowNumber],rowNumber,amount_of_data_points);

    //create coincidence matrix without 0 values
    len_line_X[rowNumber] = T_Rows[rowNumber]->get_Relevant_amount();
    int* deleteable_rows = T_Rows[rowNumber]->get_Relevant_Evts();

    //set relevant events for row rowNumber
    set_relevant(rowNumber,len_line_X[rowNumber],deleteable_rows);

    deleteable_rows = nullptr;
}


//---------------------------------------------------------------

inline bool TX_Matrix::check_relevant(int i){
    
    #ifdef GPP_FLAG
        lock_guard<mutex> lockGuard(MUTEX);
    #endif
    
    return skip_arr[i];
}

//---------------------------------------------------------------

inline void TX_Matrix::set_relevant(int i,int lenX,int* deleteable_rows){
    
    //set array of strips relevant for event i
    relevant_for_x[i] = (lenX > 0) ? new int[lenX] : nullptr;
    
    //loop over coincident events of line i
    //-> if event j is present in line i, line j is ignored
    for(int j = 0;j < lenX;++j) set_skip_array_element(deleteable_rows[j],i,j);
    
    //if event i has no coincidence and time difference to
    //latest AIDA event is short enough, event is saved
    if(keep_Event(i)) Save_Matrix_Row(i);

}

//---------------------------------------------------------------

inline void TX_Matrix::set_skip_array_element(int delete_j,int i,int j){
    
    #ifdef GPP_FLAG
        lock_guard<mutex> lockGuard(MUTEX);
    #endif

    skip_arr[delete_j] = (delete_j != i);
    relevant_for_x[i][j] = delete_j;
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
    vector<vector<int> > xy_for_sort(2000,vector<int>(2,10000));
    
    bool check_bool = (thr_num == am_threads - 1);
    
    int data_points_per_thr_tmp = (thr_num == am_threads - 1) ? data_points_per_thr_last : data_points_per_thr;
    int row_start = thr_num*data_points_per_thr_tmp;

    //temporary cluster for sorting
    int **tmp_cluster = new int*[200];
    for(int i = 0;i < 100;++i){
        tmp_cluster[i] = new int[2];
        for(int j = 0;j < 2;++j) tmp_cluster[i][j] = 0;
    }

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
        xy_for_sort[len_line_X[i]][0] = X_Arr[i];
        xy_for_sort[len_line_X[i]][1] = i;
        
        //sort values by increasing position value
        sort(xy_for_sort.begin(),xy_for_sort.begin()+len_line_X[i]+1,
             [this](const vector<int>& p1, const vector<int>& p2) {return sortFunc(p1, p2);});
        
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
        //===========================================================
        Set_Next_Cluster(tmp_cluster,c_counter,thr_num);
        //===========================================================
        
        c_counter = 0;
    }

    for(int i = 0;i < 100;++i) delete[] tmp_cluster[i];
    delete[] tmp_cluster;
    tmp_cluster = nullptr;

}

//---------------------------------------------------------------

#ifdef GPP_FLAG
    thread TX_Matrix::threading(bool T_or_X,int i,int j){
        if(T_or_X) return thread([=] {Thread_T(i,j);});
        else return thread([=] {Thread_X(j);});
    }
#endif

//---------------------------------------------------------------

void TX_Matrix::Set_Next_Cluster(int** tmp_cluster,int c_counter,int thr_num){

    #ifdef GPP_FLAG
        lock_guard<mutex> lockGuard(MUTEX);
    #endif

    Cluster_IDs[iterator_mutex][0] = tmp_cluster[c_counter][0];
    Cluster_IDs[iterator_mutex][1] = tmp_cluster[c_counter][1];
    cluster_counter[thr_num]++;
    iterator_mutex++;

}

//---------------------------------------------------------------

void TX_Matrix::set_Time_and_Energy(){
    int tmp_diff = 0;
    double tmp_sum = 0;
    int tmp_pos = 0;
    for(int i = 0;i < iterator_mutex;++i){
        Time_sent[i] = Time_Arr[Cluster_IDs[i][0]];
        
        tmp_sum = 0;
        tmp_diff = Cluster_IDs[i][1] - Cluster_IDs[i][0];
        tmp_pos = Cluster_IDs[i][0];
        for(int j = 0;j < tmp_diff;++j){
            Energies_sent[i][j] = Energy_Arr[tmp_pos];
            tmp_sum += Energies_sent[i][j];
            tmp_pos++;
        }
        Energy_sent[i] = tmp_sum;
    }
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

