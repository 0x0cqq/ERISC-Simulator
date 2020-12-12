#include "program.h"

Num::Num(){
    type = 0,val = 0;
}
Num::Num(bool _type, unsigned int _val) {
    type = _type, val = _val;
}
Num::~Num() {}

Line::Line() {
    type = UNDEF;
}

Line::Line(const short &t, const Num *args, const int &arg_n) {
    type = t;
    for(int i = 0; i < arg_n; ++i)
        arg[i] = args[i];
}

Line::~Line() {}