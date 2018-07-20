#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(){

    ifstream data("Coincidences_of_Interest.txt");

    const char* format = "%d %d %d %d %d %d";
    string line;
    int x,y,z,a,b,c;

    while(data.good()){
        x = -1;
        y = -1;
        z = -1;
        a = -1;
        b = -1;
        c = -1;

        getline(data,line,'\n');
        cout << line << endl;
        if(line[0] == '#') continue;
        sscanf(line.c_str(),format,&x,&y,&z,&a,&b,&c);
        cout << x << " " << y << " " << z << " " << a << " " << b << " " << c <<endl;
    }
    return 0;
}