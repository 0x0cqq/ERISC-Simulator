#include "simulator.h"
#include <iostream>
using namespace std;

Simulator a;

int main(int argc, char const *argv[]) {
    char *filename = new char[1231];
    cout << "argc: " << argc << endl;
    cout << "filename: " << argv[1] << endl;

    if(argc == 2) {
        sscanf(argv[1], "%s", filename);
        a.parse_file(filename);
        a.execute();
    }
    return 0;
}