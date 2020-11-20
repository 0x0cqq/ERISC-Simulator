#include <cstdio>
#include "simulator.h"
using namespace std;

Simulator a;



int main(int argc, char const *argv[]){
    char * filename = new char[1231];
    printf("argc:%d\n",argc);
    printf("filename:%s\n",argv[0]);

    if(argc == 2){
        sscanf(argv[1],"%s",filename);
        a.parse_file(filename);
        a.execute();   
    }
    return 0;
}