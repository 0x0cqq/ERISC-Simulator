#include "status.h"

#ifndef __SIMULATOR_H_
#    define __SIMULATOR_H_

struct Num {
    bool         type;
    unsigned int val;
    Num();
    ~Num();
};

class Line {
  private:
#    define UNDEFINED   -10
#    define END         -1
#    define DRAW        -2
#    define LINE_SYMBOL 0
#    define LOAD        1
#    define STORE       2
#    define PUSH        3
#    define POP         4
#    define MOV         10
#    define ADD         20
#    define SUB         21
#    define MUL         22
#    define DIV         23
#    define REM         24
#    define AND         25
#    define OR          26
#    define JAL         30
#    define BEQ         31
#    define BNE         32
#    define BLT         33
#    define BGE         34
#    define CALL        41
#    define RET         42

    short type;
    Num   arg[3];

  public:
    Line(/* args */);
    ~Line();
    short get_type() {
        return type;
    }
    Num get_arg(unsigned int i) {
        return arg[i];
    }
};

class Simulator {
  private:
    static const int MAX_INSTRUCTION = 10000;
    Line             lines[MAX_INSTRUCTION];
    Status           status;
    unsigned int     now_line;
    unsigned int     lines_num;

  public:
    Simulator(/* args */);
    ~Simulator();
    // void parse(const char *FILENAME);
    void parse(const char *script);
    void execute(unsigned int stop_line);
    void do_line(unsigned int &now_line, Line line);
    void jal(unsigned int symbol);
};

#endif