#include "program.h"

Num::Num(){
    reset();
}
Num::Num(bool _type, unsigned int _val) {
    type = _type, val = _val;
}
void Num::reset(){
    type = 0,val = 0;
}
Num::~Num() {}



Line::Line() {
    reset();
}

Line::Line(const short &t, const Num *args, const int &arg_n) {
    type = t;
    for(int i = 0; i < arg_n; ++i)
        arg[i] = args[i];
}

void Line::reset(){
    type = UNDEF;
    for(int i = 0;i<3;i++){
        arg[i].reset();
    }
}

Line::~Line() {}