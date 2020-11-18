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

// a hash map from the name of line_id to its line number
std::map<std::string, int> JUMP_LINE;

// get the next argument in the string `args_str` and store it in `arg`
// the next arg appears at `args_str + get_arg()` (if there is one)
// spaces will be ignored
inline int get_arg(const char *args_str, char *arg) {
    int len = strlen(args_str);
    int i = 0;  // index of `arg_str`
    int index = 0;  // index of `arg`
    while (i < len && args_str[i] == ' ') ++i;
    while (i < len && args_str[i] != ' ' && args_str[i] != ',') 
        arg[index++] = args_str[i++];
    return i + 1;
}

// convert a string into a number (hex or dec)
inline int strtoi(const char *s) {
    int num;
    s[1] == 'x' ? sscanf(s, "%x", &num)  // hex
                : sscanf(s, "%d", &num);  // dec
    return num;
}

// receive args `[rd],[rs]` from `args_str` and store them in `args`
inline void set_args_rd_rs(const char *args_str, Num *args) {
    char rd[5], rs[5];
    int a = get_arg(args_str, rd);  // shift backwards `a` chars
    get_arg(args_str + a, rs);
    args[0] = Num{false, (unsigned int)REGISTER[rd]};
    args[1] = Num{false, (unsigned int)REGISTER[rs]};
    return;
}

// receive arg `[rd/rs]` from `args_str` and store it in `args`
inline void set_args_r(const char *args_str, Num *args) {
    char r[5];
    get_arg(args_str, r);
    args[0] = Num{false, (unsigned int)REGISTER[r]};
    return;
}

// receive args `[rd],[rs/imm]` from `args_str` and store them in `args`
inline void set_args_rd_imm(const char *args_str, Num *args) {
    char rd[5], rs[11];
    // `rs2` could be `imm`, so it has a maximun length of 10 (without '\0')
    int a = get_arg(args_str, rd);  // shift backwards `a` chars
    get_arg(args_str + a, rs);
    args[0] = Num{false, (unsigned int)REGISTER[rd]};
    'a' <= rs[0] && rs[0] <= 'z' 
        ? args[1] = Num{false, (unsigned int)REGISTER[rs]}  // is register name
        : args[1] = Num{true, (unsigned int)strtoi(rs)};  // is immediate number
    return;
}

// receive args `[rd],[rs1],[rs2/imm]` from `args_str` and store them in `args`
inline void set_args_rd_rs_imm(const char *args_str, Num *args) {
    char rd[5], rs[5], rs2[11];
    // `rs2` could be `imm`, so it has a maximun length of 10 (without '\0')
    int a = get_arg(args_str, rd);
    int b = get_arg(args_str + a, rs);
    get_arg(args_str + a + b, rs2);
    args[0] = Num{false, (unsigned int)REGISTER[rd]};
    args[1] = Num{false, (unsigned int)REGISTER[rs]};
    'a' <= rs2[0] && rs2[0] <= 'z' 
        ? args[2] = Num{false, (unsigned int)REGISTER[rs2]}  // is register name
        : args[2] = Num{true, (unsigned int)strtoi(rs2)};  // is immediate number
    return;
}

// receive arg `[line_id]` from `args_str` and store it in `args`
inline void set_args_lid(const char *args_str, Num *args) {
    char line_id[101];
    get_arg(args_str, line_id);
    args[0] = Num{false, (unsigned int)JUMP_LINE[line_id]};
    return;
}

// receive args `[rs1],[rs2],[line_id]` from `args_str`
// and store them in `args`
inline void set_args_rs_rs_lid(const char *args_str, Num *args) {
    char rs1[5], rs2[5], line_id[101];
    int a = get_arg(args_str, rs1);
    int b = get_arg(args_str + a, rs2);
    get_arg(args_str + a + b, line_id);
    args[0] = Num{false, (unsigned int)REGISTER[rs1]};
    args[1] = Num{false, (unsigned int)REGISTER[rs2]};
    args[2] = Num{false, (unsigned int)JUMP_LINE[line_id]};
    return;
}

Line::Line() {}

Line::Line(const short &t, const Num *args, const int &arg_n) {
    type = t;
    for (int i = 0; i < arg_n; ++i) arg[i] = args[i];
}

Line::~Line() {}

Simulator::Simulator(/* args */) {}

Simulator::~Simulator() {}

// parse input file `FILENAME`(.risc) 
// and store the instuctions in `Simulator.lines`
void Simulator::parse_file(const char *FILENAME) {
    std::ifstream risc_file(FILENAME, std::ios::in);
    char line_str[Simulator::MAX_LINE_COL];  // instruction line
    int current_line = 0;  // current line index (0-index)

    while (risc_file.getline(line_str, sizeof(line_str))) {
        // setup an instruction line
        parse(line_str, lines[current_line++], current_line);
        if (current_line == MAX_INSTRUCTION) {
            printf("Maxinum instructions limit reached.\n"
                   "Instructions after line %d will be ignored.\n", MAX_INSTRUCTION);
            // TODO: raise exception?
            break;
        }
    }
    risc_file.close();
    return;
}

// parse a single line `script` (the `current_line`th) and store it in `line`
void Simulator::parse(const char *script, Line &line, const int &current_line) {
    char name[Simulator::MAX_LINE_COL];  // instruction name
    Num args[3];  // arguments
    int i = 0;  // index of `line`
    int line_len = strlen(script);  // length of `line`

    // receive instruction name
    for (; i < line_len; ++i) {
        if (script[i] == ' ') {  // instrction name end
            break;
        }
        if (script[i] == ':') {  // is line_ID
            JUMP_LINE[name] = current_line;
            break;
        }
        name[i] = script[i];  // add a character to `name`
    }

    // receive arguments and setup a new `Line`
    switch(TYPE[name]) {
    case 1:  // load [rd],[rs]
        set_args_rd_rs(script + i, args);
        line = Line(1, args, 2);
        break;
    case 2:  // store [rd],[rs]
        set_args_rd_rs(script + i, args);
        line = Line(2, args, 2);
        break;
    case 3:  // push [rs]
        set_args_r(script + i, args);
        line = Line(3, args, 1);
        break;
    case 4:  // pop [rd]
        set_args_r(script + i, args);
        line = Line(4, args, 1);
        break;
    case 10:  // mov [rd],[rs/imm]
        set_args_rd_imm(script + i, args);
        line = Line(10, args, 2);
        break;
    case 20:  // add [rd],[rs1],[rs2/imm]
        set_args_rd_rs_imm(script + i, args);
        line = Line(20, args, 3);      
        break;
    case 21:  // sub [rd],[rs1],[rs2/imm]
        set_args_rd_rs_imm(script + i, args);
        line = Line(21, args, 3);    
        break;
    case 22:  // mul [rd],[rs1],[rs2/imm]
        set_args_rd_rs_imm(script + i, args);
        line = Line(22, args, 3);
        break;
    case 23:  // div [rd],[rs1],[rs2/imm]
        set_args_rd_rs_imm(script + i, args);
        line = Line(23, args, 3);    
        break;
    case 24:  // rem [rd],[rs1],[rs2/imm]
        set_args_rd_rs_imm(script + i, args);
        line = Line(24, args, 3);
        break;
    case 25:  // and [rd],[rs1],[rs2/imm]
        set_args_rd_rs_imm(script + i, args);
        line = Line(25, args, 3);
        break;
    case 26:  // or [rd],[rs1],[rs2/imm]
        set_args_rd_rs_imm(script + i, args);
        line = Line(26, args, 3);
        break;
    case 30:  // jal [line_id]
        set_args_lid(script + i, args);
        line = Line(30, args, 1);
        break;
    case 31:  // beq [rs1],[rs2],[line_id]
        set_args_rs_rs_lid(script + i, args);
        line = Line(31, args, 3);
        break;
    case 32:  // bne [rs1],[rs2],[line_id]
        set_args_rs_rs_lid(script + i, args);
        line = Line(32, args, 3);
        break;
    case 33:  // blt [rs1],[rs2],[line_id]
        set_args_rs_rs_lid(script + i, args);
        line = Line(33, args, 3);
        break;
    case 34:  // bge [rs1],[rs2],[line_id]
        set_args_rs_rs_lid(script + i, args);
        line = Line(34, args, 3);
        break;
    case 41:  // call [line_id]
        set_args_lid(script + i, args);
        line = Line(41, args, 1);
        break;
    case 42:  // ret
        line = Line(42, args, 0);
        break;
    case 51:  // end
        line = Line(51, args, 0);
        break;
    case 52:  // draw
        line = Line(52, args, 0);
        break;
    default:
        break;
    }
    return;
}
