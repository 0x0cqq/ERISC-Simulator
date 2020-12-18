#include "simulator.h"
#include <cstring>
#include <iostream>
using namespace std;

int main(int argc, char const *argv[]) {
    char *type        = new char[1231];
    char *filename    = new char[1231];
    char *output_path = new char[1231];
    cout << "argc: " << argc << ", ";
    if(argc < 3) {
        cout << "fatal: you have given wrong numbers of parameters." << endl;
        return 42;
    }
    else {
        Simulator *a = new Simulator;
        cout << "type: " << argv[1] << ", ";
        cout << "filename: " << argv[2] << endl;
        sscanf(argv[1], "%s", type);
        sscanf(argv[2], "%s", filename);
        if(type[0] == 'e' || type[0] == 'd') {
            a->parse_file(filename);
            if(argc > 3)
                strcpy(output_path, argv[3]);
            else {
                strcpy(output_path, filename);
                int l        = strlen(filename);
                int last_dot = l;
                for(int i = 0; i < l; i++)
                    if(output_path[i] == '.')
                        last_dot = i;
                output_path[last_dot] = 0;
            }
            char spell1[1231] = "mkdir ";
            char spell2[1231] = "rm -r ";
            strcat(output_path, "_out\0");
            strcat(spell2, output_path), strcat(spell1, output_path);
            cout << spell2 << " | " << spell1 << endl;
            system(spell2);
            system(spell1);
            if(type[0] == 'e') {
                a->set_debug_mode(0);
                a->execute(output_path);
            }
            else {
            }
        }
        else if(type[0] == 't') {
            // Translator *a = new Translator;
            // a->read(filename);
            // a->parse();
            // delete a;
        }
        else {
            cout << "fatal: no such a option in parameter 1." << endl;
            return 84;
        }
        delete a;
    }
    delete[] type;
    delete[] filename;
    delete[] output_path;
    return 0;
}