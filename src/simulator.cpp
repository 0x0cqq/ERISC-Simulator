#include "simulator.h"
#include <cstring>
#include <fstream>
#include <map>

std::map<std::string, int> REGISTER = {
    {"x0", 0},   {"zero", 0}, {"x1", 1},   {"ra", 1},  {"x2", 2},   {"sp", 2},
    {"x3", 3},   {"gp", 3},   {"x4", 4},   {"tp", 4},  {"x5", 5},   {"t0", 5},
    {"x6", 6},   {"t1", 6},   {"x7", 7},   {"t2", 7},  {"x8", 8},   {"fp", 8},
    {"x9", 9},   {"s1", 9},   {"x10", 10}, {"a0", 10}, {"x11", 11}, {"a1", 11},
    {"x12", 12}, {"a2", 12},  {"x13", 13}, {"a3", 13}, {"x14", 14}, {"a4", 14},
    {"x15", 15}, {"a5", 15},  {"x16", 16}, {"a6", 16}, {"x17", 17}, {"a7", 17},
    {"x18", 18}, {"s2", 18},  {"x19", 19}, {"s3", 19}, {"x20", 20}, {"s4", 20},
    {"x21", 21}, {"s5", 21},  {"x22", 22}, {"s6", 22}, {"x23", 23}, {"s7", 23},
    {"x24", 24}, {"s8", 24},  {"x25", 25}, {"s9", 25}, {"x26", 26}, {"s10", 26},
    {"x27", 27}, {"s11", 27}, {"x28", 28}, {"t3", 28}, {"x29", 29}, {"t4", 29},
    {"x30", 30}, {"t5", 30},  {"x31", 31}, {"t6", 31}};

std::map<std::string, short> TYPE = {
    {"load", 1}, {"store", 2}, {"push", 3},  {"pop", 4},  {"mov", 10},
    {"add", 20}, {"sub", 21},  {"mul", 22},  {"div", 23}, {"rem", 24},
    {"and", 25}, {"or", 26},   {"jal", 30},  {"beq", 31}, {"bne", 32},
    {"blt", 33}, {"bge", 34},  {"call", 41}, {"ret", 42}};

// a hash map from the name of line_id to its line number
std::map<std::string, int> JUMP_LINE;

Num::Num(){
    type = 1,val = 0;
}
Num::~Num(){

}

Line::Line(/* args */) {}

Line::~Line() {}

Simulator::Simulator(/* args */) {}

Simulator::~Simulator() {}

// Execute the program from now_line, and ended to stop_line(not execute) (-1 means to the `end`)

void Simulator::execute(unsigned int stop_line = -1) {
    while(true) {
        if(now_line == stop_line || now_line == lines_num)
            break;
        do_line(now_line, lines[now_line]);
    }
}

#define _ref(i) (status.get_reg_ref(_arg[i].val))
#define _val(i) (_arg[i].type ? _arg[i].val : status.get_reg_val(_arg[i].val))

void Simulator::do_line(unsigned int &now_line, Line line) {
    Num  _arg[3];
    for(int i = 0; i < 3; i++) {
        _arg[i] = line.get_arg(i);
    }
    #define lt line.get_type
    short T = lt();
    T = (T >= 0 ? (T/10) : -((-T)/10) - 1);
    char *filename = new char[1024];
    switch(T) {
        case -2:
            break;
        case -1:
            
            status.get_print_filename((bool)(lt()+2),filename); // -2 -> 0 , -1 -> 1
            switch (lt()) {
                case DRAW:
                    status.print_to_bmp(filename);
                case END:
                    status.print_to_txt(filename);
                default:
                    break;
            }
            
            break;
        case 0:
            switch(lt()) {
                case LINE_SYMBOL:
                    break;
                case LOAD:
                    status.load(_ref(0), _val(1));
                    break;
                case STORE:
                    status.store(_ref(0), _val(1));
                    break;
                case PUSH:
                    status.push(_val(0));
                    break;
                case POP:
                    status.pop(_ref(0));
                    break;
                default:
                    break;
            }
            break;
        case 1:
            switch(lt()) {
                case MOV:
                    status.mov(_ref(0), _val(1));
                    break;
                default:
                    break;
            }
            break;
        case 2:  // ADD, SUB, MUL, DIV, REM, AND, OR
            status.op(_ref(0), _val(1), _val(2), lt() - 20);
            break;
        case 3:
            if(lt() == JAL || 
            (lt() == BEQ && _val(0) == _val(1))|| \
            (lt() == BNE && _val(0) != _val(1))|| \
            (lt() == BLT && _val(0) < _val(1)) || \
            (lt() == BGE && _val(0) > _val(1)) )
                now_line = _val(2); // dst_line
            break;
        case 4:
            switch(lt()) {
                case CALL:
                    status.push(now_line);// not according with document
                    now_line = _val(0);
                    break;
                case RET:
                    status.pop(now_line);
                    break;
            }
            break;
        default:
            break;
    }
    delete[] filename;
    now_line++;
    if(lt() == END) 
        now_line = -1;
}
