#include "simulator.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
// #include <unistd.h>

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
std::map<std::string, unsigned int> jump_line;

// index of lines whose argument [line_id] appeared before its declaration
int unfound_line[1000];
// script of instuctions above
char unfound_scpt[1000][100];
// index of `UNFOUND_LINE` and `UNFOUND_LINE_ID`
int unfound_index = 0;

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

inline int add_arg(const char *args_str, Num &arg, int type) {
    // type = 0 --> reg/imm; type = 1 --> lid
    // modify args[i];
    int  ans = 0;
    char r[101];
    ans = get_arg(args_str, r);
    if(type == 0) {
        arg = (('a' <= r[0] && r[0] <= 'z') ?
                   Num{false, (unsigned int)REGISTER[r]} :
                   Num{true, (unsigned int)strtoi(r)});
    }
    else if(type == 1) {
        arg = Num{true, jump_line.count(r) ? jump_line[r] : -1};
    }
    else {
        std::printf("fuckfuck!!!!\n");
    }
    return ans;
}

// Execute the program from now_line, and ended to stop_line(not execute)
// (-1 means to the `end` symbol)
void Simulator::execute(unsigned int stop_line) {
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
#define _raw(i) (_arg[i].type ? -1 : _arg[i].val)
// the type of the line
#define lt line.get_type

// Do a line, in "now_line", with Line Struction "line"
void Simulator::do_line(unsigned int &now_line, Line line) {
    Num _arg[3];
    // get a copy of line's arg
    for(int i = 0; i < 3; i++) {
        _arg[i] = line.get_arg(i);
    }
    // test part
    std::printf("now_line:%d\n", now_line);
    std::printf("  type:%d arg:", line.get_type());
    // usleep(10000);
    for(int i = 0; i < 3; i++) {
        printf("(%d,%d)", int(_arg[i].type), _arg[i].val);
        if(i < 2)
            printf(",");
    }
    printf("\n");
    // sort the lines to different types
    char *filename = new char[1024];

    switch(lt()) {
        case UNDEF:  // UNDEFINED
            break;
        case DRAW:
        case END:  // DRAW, END
            // get_print_name from status,  -2 -> 0 , -1 -> 1
            status.get_print_filename((bool)(lt() + 2), filename);
            switch(lt()) {
                case DRAW: status.print_to_bmp(filename);
                case END: status.print_to_txt(filename);
                default: break;
            }
            break;
        case LINE_SYMBOL: break;
        case LOAD:
            status.load(_ref(0), _val(1));
            status.set_reg_status(_raw(0), 1);
            status.set_memory_status(_raw(1));
            break;
        case STORE:
            status.store(_val(0), _val(1));
            status.set_reg_status(_raw(0), 0);
            status.set_memory_status(_raw(1));
            break;
        case PUSH:
            status.push(_val(0));
            status.set_reg_status(_raw(0), 0);
            status.set_stack_status();
            break;
        case POP:
            status.pop(_ref(0));
            status.set_reg_status(_raw(0), 1);
            status.set_stack_status();
            break;
        case MOV:
            status.mov(_ref(0), _val(1));
            status.set_reg_status(_raw(0), 1);
            status.set_reg_status(_raw(1), 0);
            break;
        // ADD, SUB, MUL, DIV, REM, AND, OR
        case ADD:
        case SUB:
        case MUL:
        case DIV:
        case REM:
        case AND:
        case OR: status.op(_ref(0), _val(1), _val(2), lt() - 20); break;
        // JAL, BEQ, BNE, BLT, BGE
        case JAL: now_line = _val(0);
        case BEQ:
        case BNE:
        case BLT:
        case BGE:
            if((lt() == BEQ && _val(0) == _val(1)) ||
               (lt() == BNE && _val(0) != _val(1)) ||
               (lt() == BLT && _val(0) < _val(1)) ||
               (lt() == BGE && _val(0) > _val(1))) {
                now_line = _val(2);  // dst_line
                status.set_reg_status(_raw(0), 0);
                status.set_reg_status(_raw(1), 0);
            }
            break;
        case CALL:
            status.push(now_line);  // not according with document
            now_line = _val(0);
            break;
        case RET:
            status.pop(now_line);
            // printf("%d\n",(int)now_line);
            break;
        default: break;
    }
    delete[] filename;
    // test part2
    static int cnt         = 0;
    char *     tmpfilename = new char[1024];
    std::sprintf(tmpfilename, "%d.txt", cnt);
    status.print_raw(tmpfilename);
    cnt++;
    delete[] tmpfilename;
    // end test part 2
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

    // parse the input file line by line
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

    // deal with the instuctions whose argument [line_id]
    // appeared before its declaration
    while(unfound_index--) {
        // std::cout << "Unfound" << unfound_index << std::endl;
        parse(unfound_scpt[unfound_index], lines[unfound_line[unfound_index]],
              unfound_line[unfound_index]);  // re-parse the scripts
    }
    // record line_num into the simulator, to end the execute
    lines_num = current_line;
    risc_file.close();
    return;
}

// parse a single line `script` (the `current_line`th) and store it in `line`
void Simulator::parse(const char *script, Line &line, int current_line) {
    // std::cerr << current_line << std::endl;
    char name[Simulator::MAX_LINE_COL] = "UNDEF";  // instruction name
    Num  args[3];                                  // arguments
    int  i        = 0;                             // index of `line`
    int  line_len = strlen(script);                // length of `line`

    // receive instruction name
    if(line_len) {  // not blank line
        for(; i < line_len; ++i) {
            if(script[i] == ' ') {  // instrction name end
                name[i] = '\0';
                break;
            }
            if(script[i] == ':') {  // is line_ID
                // std::cout << "Line Symbol:" << current_line << std::endl;
                name[i]         = '\0';
                jump_line[name] = current_line;  // the next line index // no!
                // std::cout << "Line Symbol:" << jump_line[name] << std::endl;
                break;
            }
            name[i] = script[i];  // add a character to `name`
        }
        name[i] = '\0';
    }
// if `line_len` == 0, which means `script` is a blank line,
// `name` would stay as its default value "UNDEF"

// receive arguments and setup a new `Line`
#define add_unfound                                                            \
    unfound_line[unfound_index] = current_line;                                \
    strcpy(unfound_scpt[unfound_index], script);                               \
    ++unfound_index
    int   s       = i;
    short type_id = TYPE.count(name) ? TYPE[name] : 0;
    switch(type_id) {
        case PUSH:
        case POP:
            // push/pop [rs]
            s += add_arg(script + s, args[0], 0);
            line = Line(type_id, args, 1);
            break;
        case LOAD:
        case STORE:
        case MOV:
            // load/store/mov [r],[r]
            s += add_arg(script + s, args[0], 0);
            s += add_arg(script + s, args[1], 0);
            line = Line(type_id, args, 2);
            break;
        // add/sub/mul/div [rd],[rs1],[rs2/imm]
        case ADD:
        case SUB:
        case MUL:
        case DIV:
        case AND:
        case OR:
            s += add_arg(script + s, args[0], 0);
            s += add_arg(script + s, args[1], 0);
            s += add_arg(script + s, args[2], 0);
            line = Line(type_id, args, 3);
            break;
        case JAL:  // jal [line_id]
            s += add_arg(script + s, args[0], 1);
            if(~args[0].val)
                line = Line(type_id, args, 1);
            else {
                add_unfound;
            }
            break;
        // beq/bne/blt/bge [rs1],[rs2],[line_id]
        case BEQ:
        case BNE:
        case BLT:
        case BGE:
            s += add_arg(script + s, args[0], 0);
            s += add_arg(script + s, args[1], 0);
            s += add_arg(script + s, args[2], 1);
            if(~args[2].val)
                line = Line(BEQ, args, 3);
            else {
                add_unfound;
            }
            break;
        case CALL:  // call [line_id]
            s += add_arg(script + s, args[0], 1);
            if(~args[0].val)
                line = Line(CALL, args, 1);
            else {
                add_unfound;
            }
            break;
        case RET:
        case END:
        case DRAW:
        case LINE_SYMBOL:
        case UNDEF:  // ret/end/draw/line_symbol/undef
            line = Line(type_id, args, 0);
            break;
        default: break;
    }
    return;
}
