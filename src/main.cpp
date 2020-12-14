#include "simulator.h"
#include "translator.h"
#include <iostream>
using namespace std;



int main(int argc, char const *argv[]) {
    
    char *type = new char[1231];
    char *filename = new char[1231];
    cout << "argc: " << argc << endl;
    cout << "type: " << argv[1] << endl;
    cout << "filename: " << argv[2] << endl;

    if(argc == 3) {
        sscanf(argv[1], "%s", type);
        sscanf(argv[2], "%s", filename);
        if(type[0] == 'e'){
            Simulator *a = new Simulator;
            a->parse_file(filename);
            a->execute();
        }
        else if(type[0] == 't'){
            Translator *a = new Translator;
            a->read(filename);
            a->parse();
        }
        else{
            cout << "fatal: no such a option in parameter 1." << endl;
            return 2;
        }
    }
    else{
        cout << "fatal: you have given wrong numbers of parameters." << endl;
        return 1;
    }
    return 0;
}