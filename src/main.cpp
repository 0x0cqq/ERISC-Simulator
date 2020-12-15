#include "simulator.h"
#include "translator.h"
#include <iostream>
using namespace std;



int main(int argc, char const *argv[]) {
    
    char *type = new char[1231];
    char *filename = new char[1231];
    cout << "argc: " << argc << ", ";
    if(argc < 3){
        cout << "fatal: you have given wrong numbers of parameters." << endl;
        return 1;
    }
    else{
        cout << "type: " << argv[1] << ", ";
        cout << "filename: " << argv[2] << endl;
        sscanf(argv[1], "%s", type);
        sscanf(argv[2], "%s", filename);
        if(type[0] == 'e'){
            Simulator *a = new Simulator;
            a->parse_file(filename);
            a->execute();
            delete a;
        }
        else if(type[0] == 't'){
            Translator *a = new Translator;
            a->read(filename);
            a->parse();
            delete a;
        }
        else{
            cout << "fatal: no such a option in parameter 1." << endl;
            return 2;
        }
    }
    delete[] type;
    delete[] filename;
    return 0;
}