#include "simulator.h"
#include <cstring>
#include <fstream>
#include <map>

Num::Num(bool _type = 1, unsigned int _val = 0) {
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

Simulator::Simulator(/* args */) {
    now_line = 0;
}

Simulator::~Simulator() {}

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
    {"UNDEFINED", UNDEF}, {"end", END},   {"draw", DRAW}, {"load", LOAD},
    {"store", STORE},     {"push", PUSH}, {"pop", POP},   {"mov", MOV},
    {"add", ADD},         {"sub", SUB},   {"mul", MUL},   {"div", DIV},
    {"rem", REM},         {"and", AND},   {"or", OR},     {"jal", JAL},
    {"beq", BEQ},         {"bne", BNE},   {"blt", BLT},   {"bge", BGE},
    {"call", CALL},       {"ret", RET}};

// a hash map from the name of line_id to its line number
std::map<std::string, int> JUMP_LINE;

// get the next argument in the string `args_str` and store it in `arg`
// the next arg appears at `args_str + get_arg()` (if there is one)
// spaces will be ignored
inline int get_arg(const char *args_str, char *arg) {
    int len   = strlen(args_str);
    int i     = 0;  // index of `arg_str`
    int index = 0;  // index of `arg`
    while(i < len && args_str[i] == ' ')
        ++i;
    while(i < len && args_str[i] != ' ' && args_str[i] != ',')
        arg[index++] = args_str[i++];
    arg[index] = '\0';
    return i + 1;
}

// convert a string into a number (hex or dec)
inline int strtoi(const char *s) {
    int num;
    // judge by '0x' ? hex : dec
    s[1] == 'x' ? sscanf(s, "%x", &num) : sscanf(s, "%d", &num);
    return num;
}

// receive args `[rd],[rs]` from `args_str` and store them in `args`
inline void set_args_rd_rs(const char *args_str, Num *args) {
    char rd[5], rs[5];
    int  a = get_arg(args_str, rd);  // shift backwards `a` chars
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
    'a' <= rs[0] && rs[0] <= 'z' ?
        args[1] = Num{false, (unsigned int)REGISTER[rs]}  // is register name
        :
        args[1] = Num{true, (unsigned int)strtoi(rs)};  // is immediate number
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
    'a' <= rs2[0] && rs2[0] <= 'z' ?
        args[2] = Num{false, (unsigned int)REGISTER[rs2]}  // is register name
        :
        args[2] = Num{true, (unsigned int)strtoi(rs2)};  // is immediate number
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
    int  a = get_arg(args_str, rs1);
    int  b = get_arg(args_str + a, rs2);
    get_arg(args_str + a + b, line_id);
    args[0] = Num{false, (unsigned int)REGISTER[rs1]};
    args[1] = Num{false, (unsigned int)REGISTER[rs2]};
    args[2] = Num{false, (unsigned int)JUMP_LINE[line_id]};
    return;
}

// Execute the program from now_line, and ended to stop_line(not execute)
// (-1 means to the `end` symbol)
void Simulator::execute(unsigned int stop_line = -1) {
    while(true) {
        if(now_line == stop_line || now_line == lines_num)
            break;
        do_line(now_line, lines[now_line]);
    }
}

// some define to make get register's val and reference more easy
// get reference of register from argument
#define _ref(i) (status.get_reg_ref(_arg[i].val))
// get value of arg[i] (immediate number or register)
#define _val(i) (_arg[i].type ? _arg[i].val : status.get_reg_val(_arg[i].val))
// the type of the line
#define lt line.get_type

// Do a line, in "now_line", with Line Struction "line"
void Simulator::do_line(unsigned int &now_line, Line line) {
    Num _arg[3];
    // get a copy of line's arg
    for(int i = 0; i < 3; i++) {
        _arg[i] = line.get_arg(i);
    }
    // sort the lines to different types
    short T        = lt();
    T              = (T >= 0 ? (T / 10) : -((-T) / 10) - 1);
    char *filename = new char[1024];
    switch(T) {
        case -2:  // UNDEFINED
            break;
        case -1:  // DRAW, END
            // get_print_name from status
            status.get_print_filename((bool)(lt() + 2),
                                      filename);  // -2 -> 0 , -1 -> 1
            switch(lt()) {
                case DRAW:
                    status.print_to_bmp(filename);
                case END:
                    status.print_to_txt(filename);
                default:
                    break;
            }

            break;
        case 0:  // LINE_SYMBOL, LOAD, STORE, PUSH, POP
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
        case 1:  // MOV
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
        case 3:  // JAL, BEQ, BNE, BLT, BGE
            if(lt() == JAL || (lt() == BEQ && _val(0) == _val(1)) ||
               (lt() == BNE && _val(0) != _val(1)) ||
               (lt() == BLT && _val(0) < _val(1)) ||
               (lt() == BGE && _val(0) > _val(1)))
                now_line = _val(2);  // dst_line
            break;
        case 4:  // CALL, RET
            switch(lt()) {
                case CALL:
                    status.push(now_line);  // not according with document
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
    now_line++;  // jump to the next line
    if(lt() == END)
        now_line = -1;  // set an end signal
}

// parse input file `FILENAME`(.risc)
// and store the instuctions in `Simulator.lines`
void Simulator::parse_file(const char *FILENAME) {
    std::ifstream risc_file(FILENAME, std::ios::in);
    char          line_str[Simulator::MAX_LINE_COL];  // instruction line
    int           current_line = 0;  // current line index (0-index)

    while(risc_file.getline(line_str, sizeof(line_str))) {
        // setup an instruction line
        parse(line_str, lines[current_line], current_line);
        current_line++;
        if(current_line == MAX_INSTRUCTION) {
            printf("Maximum instructions limit reached.\n"
                   "Instructions after line %d will be ignored.\n",
                   MAX_INSTRUCTION);
            // TODO: raise exception?
            break;
        }
    }
    // record line_num into the simulator, to end the execute
    lines_num = current_line;
    risc_file.close();
    return;
}

// parse a single line `script` (the `current_line`th) and store it in `line`
void Simulator::parse(const char *script, Line &line, int current_line) {
    char name[Simulator::MAX_LINE_COL] = "UNDEF";  // instruction name
    Num  args[3];                                  // arguments
    int  i        = 0;                             // index of `line`
    int  line_len = strlen(script);                // length of `line`

    // receive instruction name
    if(line_len) {  // not blank line
        for(; i < line_len; ++i) {
            if(script[i] == ' ') {  // instrction name end
                break;
            }
            if(script[i] == ':') {  // is line_ID
                name[i]         = '\0';
                JUMP_LINE[name] = current_line;
                break;
            }
            name[i] = script[i];  // add a character to `name`
        }
        name[i] = '\0';
    }
    // if `line_len` == 0, which means `script` is a blank line,
    // `name` would stay as its default value "UNDEF"

    // receive arguments and setup a new `Line`
    switch(TYPE[name]) {
        case 1:  // load [rd],[rs]
            set_args_rd_rs(script + i, args);
            line = Line(LOAD, args, 2);
            break;
        case 2:  // store [rd],[rs]
            set_args_rd_rs(script + i, args);
            line = Line(STORE, args, 2);
            break;
        case 3:  // push [rs]
            set_args_r(script + i, args);
            line = Line(PUSH, args, 1);
            break;
        case 4:  // pop [rd]
            set_args_r(script + i, args);
            line = Line(POP, args, 1);
            break;
        case 10:  // mov [rd],[rs/imm]
            set_args_rd_imm(script + i, args);
            line = Line(MOV, args, 2);
            break;
        case 20:  // add [rd],[rs1],[rs2/imm]
            set_args_rd_rs_imm(script + i, args);
            line = Line(ADD, args, 3);
            break;
        case 21:  // sub [rd],[rs1],[rs2/imm]
            set_args_rd_rs_imm(script + i, args);
            line = Line(SUB, args, 3);
            break;
        case 22:  // mul [rd],[rs1],[rs2/imm]
            set_args_rd_rs_imm(script + i, args);
            line = Line(MUL, args, 3);
            break;
        case 23:  // div [rd],[rs1],[rs2/imm]
            set_args_rd_rs_imm(script + i, args);
            line = Line(DIV, args, 3);
            break;
        case 24:  // rem [rd],[rs1],[rs2/imm]
            set_args_rd_rs_imm(script + i, args);
            line = Line(REM, args, 3);
            break;
        case 25:  // and [rd],[rs1],[rs2/imm]
            set_args_rd_rs_imm(script + i, args);
            line = Line(AND, args, 3);
            break;
        case 26:  // or [rd],[rs1],[rs2/imm]
            set_args_rd_rs_imm(script + i, args);
            line = Line(OR, args, 3);
            break;
        case 30:  // jal [line_id]
            set_args_lid(script + i, args);
            line = Line(JAL, args, 1);
            break;
        case 31:  // beq [rs1],[rs2],[line_id]
            set_args_rs_rs_lid(script + i, args);
            line = Line(BEQ, args, 3);
            break;
        case 32:  // bne [rs1],[rs2],[line_id]
            set_args_rs_rs_lid(script + i, args);
            line = Line(BNE, args, 3);
            break;
        case 33:  // blt [rs1],[rs2],[line_id]
            set_args_rs_rs_lid(script + i, args);
            line = Line(BLT, args, 3);
            break;
        case 34:  // bge [rs1],[rs2],[line_id]
            set_args_rs_rs_lid(script + i, args);
            line = Line(BGE, args, 3);
            break;
        case 41:  // call [line_id]
            set_args_lid(script + i, args);
            line = Line(CALL, args, 1);
            break;
        case 42:  // ret
            line = Line(RET, args, 0);
            break;
        case -1:  // end
            line = Line(END, args, 0);
            break;
        case -2:  // draw
            line = Line(DRAW, args, 0);
            break;
        case 0:  // line symbol
            line = Line(LINE_SYMBOL, args, 0);
            break;
        case -10:  // UNDEF
            line = Line(UNDEF, args, 0);
            break;
    }
    return;
}
