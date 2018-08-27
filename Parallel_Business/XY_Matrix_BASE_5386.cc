#include "XY_Matrix.h"

using namespace std;

//---------------------------------------------------------------

XY_Matrix::XY_Matrix(int local_z){
    this->local_z = local_z;
    load_thread_file();
}

//---------------------------------------------------------------

XY_Matrix::~XY_Matrix(){

}

//---------------------------------------------------------------

void XY_Matrix::threading(int thr_num){
    bool in_Energy = false;
    bool in_time = false;

    int start_x = thr_num*data_points_per_thr_x;
    int start_y = thr_num*data_points_per_thr_y;
    double pos_tmp = new double[2];

    for(int x_counter = start_x;x_counter < data_points_per_thr_x + start_x;++x_counter){
        for(int y_counter = start_y;y_counter < data_points_per_thr_y + start_y;++y_counter){
            //compare energies and times of x and y clusters for pairing
            in_time = abs(Cluster_X_Time[x_counter] - Cluster_Y_Time[y_counter]) < max_T;
            in_Energy = abs(Cluster_X_Energy[x_counter] - Cluster_Y_Energy[y_counter]) < max_E;
            if(in_time && in_Energy){
                Form_XY(pos_tmp,x_counter,y_counter);
                XY_Clusters[XY_counter[thr_num]] = new XY_Cluster
            }
        }
    }

    delete[] pos_tmp;
}

//---------------------------------------------------------------

thread XY_Matrix::threading(int j){
    return thread([=] {Thread_XY(j);});
}

//---------------------------------------------------------------

void XY_Matrix::load_thread_file(){

    string line;
    char dummy_str[100];

    cout << "\n-------------------------------------" << endl;

    ifstream thr_file("Configuration_Files/THREAD_FILE.txt");
    if(thr_file.fail()) am_threads = 5;
    else{
        while(thr_file.good()){
            getline(thr_file,line,'\n');
            if(line[0] == "#") continue;
            sscanf(line.c_str(),"%s %d",dummy_str,&am_threads);
        }
    }
    am_threads_d = (double) am_threads;
}

//---------------------------------------------------------------

inline XY_Matrix::Form_XY(double* pos,int x_counter,int y_counter){
    int x_len = Cluster_X_len[x_counter];
    int y_len = Cluster_Y_len[y_counter];
    double mu_x = 0;
    double mu_y = 0;

    for(int i = 0;i < x_len;++i) mu_x += Cluster_X_Energies[x_counter][i]*Cluster_X_Pos[x_counter][i];
    for(int i = 0;i < y_len;++i) mu_y += Cluster_Y_Energies[y_counter][i]*Cluster_Y_Pos[y_counter][i];

    mu_x /= Cluster_X_Energy[x_counter];
    mu_y /= Cluster_Y_Energy[y_counter];

    pos[0] = mu_x;
    pos[1] = mu_y;
}

//---------------------------------------------------------------