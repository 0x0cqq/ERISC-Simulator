#include "simulator.h"
#include <fstream>
#include <cstring>
#include <map>

std::map<std::string, int> REGISTER = {
    {"x0", 0}, {"zero", 0},
    {"x1", 1}, {"ra", 1},
    {"x2", 2}, {"sp", 2},
    {"x3", 3}, {"gp", 3},
    {"x4", 4}, {"tp", 4},
    {"x5", 5}, {"t0", 5},
    {"x6", 6}, {"t1", 6},
    {"x7", 7}, {"t2", 7},
    {"x8", 8}, {"fp", 8},
    {"x9", 9}, {"s1", 9},
    {"x10", 10}, {"a0", 10},
    {"x11", 11}, {"a1", 11},
    {"x12", 12}, {"a2", 12},
    {"x13", 13}, {"a3", 13},
    {"x14", 14}, {"a4", 14},
    {"x15", 15}, {"a5", 15},
    {"x16", 16}, {"a6", 16},
    {"x17", 17}, {"a7", 17},
    {"x18", 18}, {"s2", 18},
    {"x19", 19}, {"s3", 19},
    {"x20", 20}, {"s4", 20},
    {"x21", 21}, {"s5", 21},
    {"x22", 22}, {"s6", 22},
    {"x23", 23}, {"s7", 23},
    {"x24", 24}, {"s8", 24},
    {"x25", 25}, {"s9", 25},
    {"x26", 26}, {"s10", 26},
    {"x27", 27}, {"s11", 27},
    {"x28", 28}, {"t3", 28},
    {"x29", 29}, {"t4", 29},
    {"x30", 30}, {"t5", 30},
    {"x31", 31}, {"t6", 31}
};

std::map<std::string, short> TYPE = {
    {"load", 1},
    {"store", 2},
    {"push", 3},
    {"pop", 4},
    {"mov", 10},
    {"add", 20},
    {"sub", 21},
    {"mul", 22},
    {"div", 23},
    {"rem", 24},
    {"and", 25},
    {"or", 26},
    {"jal", 30},
    {"beq", 31},
    {"bne", 32},
    {"blt", 33},
    {"bge", 34},
    {"call", 41},
    {"ret", 42}
};

std::map<std::string, int> JUMP_LINE;

Line::Line(const short &t, const Num *args, const int &arg_n) {
    type = t;
    for (int i = 0; i < arg_n; ++i) arg[i] = args[i];
}

Line::~Line() {}

Simulator::Simulator(/* args */) {}

Simulator::~Simulator() {}

void Simulator::parse_file(const char *FILENAME) {
    std::ifstream risc_file(FILENAME, std::ios::in);
    char line[Simulator::MAX_LINE_COL];  // instruction line

    int current_line = 0;  // current line index (0-index)

    while (risc_file.getline(line, sizeof(line))) {
        // setup an instruction line
        parse(line, lines[current_line++], current_line);
        // TODO: deal with the overflow of lines
    }
    risc_file.close();
    return;
}

void Simulator::parse(const char *script, Line &line, const int &current_line) {
    char name[Simulator::MAX_LINE_COL];  // instruction name
    char arg[11];  // single argument
    int args[3];  // arguments
    int arg_i = 0;  // index of `arg`
    int args_i = 0;  // index of `args`
    for (int j = 0; j < 11; ++j) args[j] = 0;  // initiate `arg`
    int line_len = strlen(script);  // length of `line`
    int i = 0;  // index of `line`

    // receive instruction name
    for (; i < line_len; ++i) {
        if (script[i] == ' ') {  // instrction name end
            while (script[i] == ' ') ++i;  // skip spaces
            break;
        }
        if (script[i] == ':') {  // is line_ID
            JUMP_LINE[name] = current_line;
            break;
        }
        name[i] = script[i];  // add a character to `name`
    }

    // TODO: receive arguments



    // line = Line(name, args);
    return;
}