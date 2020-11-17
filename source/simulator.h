#include "status.h"

#ifndef __SIMULATOR_H_
#    define __SIMULATOR_H_

struct Num {
    bool         type;
    unsigned int val;
};

class Line {
  private:
#    define UNDEFINED   -1
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
};

class Simulator {
  private:
    static const int MAX_INSTRUCTION = 10000;
    Line             lines[MAX_INSTRUCTION];
    Status           status;
    unsigned int     now_line;

  public:
    Simulator(/* args */);
    ~Simulator();
    void parse(const char *FILENAME);
    void parse(const char *script);
    void execute(unsigned int stop_line);
    void do_line(Line line);
};

#endif