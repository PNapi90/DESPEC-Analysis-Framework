#include "T_Matrix_Row.h"

using namespace std;

//---------------------------------------------------------------

T_Matrix_Row::T_Matrix_Row(int max_len_TX,int max_len){

    this->max_len_TX = max_len_TX;
    this->max_len = max_len;
    len_of_row = 0;
    len_of_relevant = 0;
    len_old = 0;
    
    try{
        Relevant_for_i = new int[max_len];
    }
    catch(const std::bad_alloc&){
        process_mem_usage(0);
        exit(0);
    }
    for(int i = 0;i < max_len;++i) Relevant_for_i[i] = -1;
}

//---------------------------------------------------------------

T_Matrix_Row::~T_Matrix_Row(){
    delete[] Relevant_for_i;
}

//---------------------------------------------------------------

void T_Matrix_Row::process_mem_usage(int iter){
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
    cerr << "=======================================================================" << endl;
    cerr << "MEMORY USED @ iteration " << iter << " -> ";
    cerr << "VM: " << vm_usage << " MB" << endl;
    cerr << "std::bad_alloc occurred!" << endl;
    cerr << "Problem occurred in Relevant_for_i in T_Matrix_Rows" << endl;
    cerr << "max_len is set to " << max_len;
    double needed_mem = (max_len*max_len_TX)/pow(2.,18.)*6.;
    cerr << " -> " << needed_mem << " MB of RAM needed for T-Matrix" << endl;
    cerr << "-----------------------------------------------------------------------" << endl;
    cerr << "***HINT***" << endl;
    cerr << "If VM is 8 GB, check maximum allowed virtual memory via ulimit -a" << endl;
    cerr << "Decrease max_len (if possible) or ask System Admin @ GSI for more" << endl;
    cerr << "possible memory per user" << endl;
    cerr << "=======================================================================" << endl;
    
}

//---------------------------------------------------------------

void T_Matrix_Row::set_Row(ULong64_t* row,ULong64_t Time_i,int pos,int len_of_row){
    this->len_of_row = len_of_row;
    
    check_limits();

    len_of_relevant = 0;
    double delta = 0;
    bool ROW = false;

    for(int i = pos+1;i < len_of_row;++i){
        delta = (row[i] >= Time_i) ? (double) (row[i] - Time_i) : (double) (Time_i - row[i]);
        ROW = (abs(delta) < t_max);
        if(ROW){
            Relevant_for_i[len_of_relevant] = i;
            len_of_relevant++;
        }
    }
}

//---------------------------------------------------------------

int* T_Matrix_Row::get_Relevant_Evts(){
    return Relevant_for_i;
}

//---------------------------------------------------------------

int T_Matrix_Row::get_Relevant_amount(){
    return len_of_relevant;
}

//---------------------------------------------------------------

void T_Matrix_Row::Reset(){
    len_of_relevant = 0;
}

//---------------------------------------------------------------

inline void T_Matrix_Row::check_limits(){
    if(len_of_row > max_len){
        cerr << "ARRAY LENGTH LIMIT OF T_Matrix_Row REACHED!" << endl;
        exit(0);
    }
}

//---------------------------------------------------------------
