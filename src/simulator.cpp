#include "simulator.h"
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

// clang-format off
// some define to make code more elegant
#define add_unfound unfound_line[unfound_index] = current_line;strcpy(unfound_scpt[unfound_index], script);++unfound_index
// some define to make get register's val and reference more easy
// get reference of register from argument
#define _ref(i) (status.get_reg_ref(_arg[i].val))
// get value of arg[i] (immediate number or register)
#define _val(i) (_arg[i].type ? _arg[i].val : status.get_reg_val(_arg[i].val))
#define _raw(i) (_arg[i].type ? -1 : _arg[i].val)
// the type of the line
#define lt line.get_type
// clang-format on

// reset the type
void Simulator::reset() {
    debug_mode    = 0;
    unfound_index = 0;
    for(int i = 0; i < MAX_INSTRUCTION; i++) {
        unfound_line[i] = 0;
        for(int j = 0; j < MAX_LINE_COL; j++) {
            unfound_scpt[i][j] = 0;
        }
    }
    lines_num = now_line = unfound_index = 0;
    for(int i = 0; i < MAX_INSTRUCTION; i++) {
        lines[i].reset();
    }
    status.reset();
    breakpoints.clear();
}

Simulator::Simulator(/* args */) {
    reset();
}

Simulator::~Simulator() {}

void Simulator::set_debug_mode(int x) {
    debug_mode = x;
}

std::unordered_map<std::string, int> REGISTER = {
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

std::unordered_map<std::string, short> TYPE = {
    {"UNDEFINED", UNDEF}, {"end", END},   {"draw", DRAW}, {"load", LOAD},
    {"store", STORE},     {"push", PUSH}, {"pop", POP},   {"mov", MOV},
    {"add", ADD},         {"sub", SUB},   {"mul", MUL},   {"div", DIV},
    {"rem", REM},         {"and", AND},   {"or", OR},     {"jal", JAL},
    {"beq", BEQ},         {"bne", BNE},   {"blt", BLT},   {"bge", BGE},
    {"call", CALL},       {"ret", RET},   {"read", READ}, {"write", WRITE}};

// a hash map from the name of line_id to its line number
std::unordered_map<std::string, unsigned int> jump_line;

// get the next argument in the string `args_str` and store it in `arg`
// the next arg appears at `args_str + get_arg()` (if there is one)
// spaces will be ignored
inline int get_arg(const char *args_str, char *arg) {
    int len   = strlen(args_str);
    int i     = 0;  // index of `arg_str`
    int index = 0;  // index of `arg`
    while(i < len &&
          (args_str[i] == ' ' || args_str[i] == ',' || args_str[i] == '\n' || args_str[i] == '\r' || 
           args_str[i] == '*'))
        ++i;
    while(i < len && args_str[i] != ' ' && args_str[i] != ',' &&  args_str[i] != '\r' &&
          args_str[i] != '\n' && args_str[i] != '*')
        arg[index++] = args_str[i++];
    arg[index] = '\0';
    return i + 1;
}

// print a line in a beautiful syntax
void print_line(Line &line) {
    std::cout << "    type:" << line.get_type() << " arg:";
    // usleep(10000);
    for(int i = 0; i < 3; i++) {
        std::cout << "(" << int(line.get_arg(i).type) << ","
                  << int(line.get_arg(i).val) << ")";
        if(i < 2)
            std::cout << ",";
    }
    std::cout << std::endl;
}

// convert a string into a number (hex or dec)
inline int strtoi(const char *s) {
    int num;
    // judge by '0x' ? hex : dec
    s[1] == 'x' ? sscanf(s, "%x", &num) : sscanf(s, "%d", &num);
    return num;
}

// type = 0 --> reg/imm; type = 1 --> lid
// modify args[i];
inline int add_arg(const char *args_str, Num &arg, int type) {
    int  ans = 0;
    char r[101];
    ans = get_arg(args_str, r);
    // std::cout << "arg: \"";
    // std::cout << args_str << "\"" << std::endl;
    if(type == 0) {
        if('a' <= r[0] && r[0] <= 'z' && !REGISTER.count(r)) {
            throw std::runtime_error("parsing error: wrong register name.");
        }
        arg = (('a' <= r[0] && r[0] <= 'z') ?
                   Num{false, (unsigned int)REGISTER[r]} :
                   Num{true, (unsigned int)strtoi(r)});
    }
    else if(type == 1) {
        arg = Num{true, jump_line.count(r) ? jump_line[r] : -1};
    }
    else {
        std::cerr << "?" << std::endl;
    }
    return ans;
}

// Execute the program from now_line, and ended to stop_line(not execute)
// (-1 means to the `end` symbol)
void Simulator::execute(const char *OUTPUT_PATH, unsigned int stop_line) {
    if(debug_mode)
        print_debug_note();
    static int cnt = -1;
    if(cnt == -1)
        cnt = 0;
    static clock_t total_runtime = 0, last_clock = 0, t1 = 0;
    last_clock = clock();
    while(true) {
        if(cnt % (debug_mode ? 100 : 1000000) == 0)
            std::cout << "Current executed Lines: " << cnt << std::endl;
        if(now_line == lines_num) { // last line
            break;
        }
        if(debug_mode && breakpoints.count(now_line)) { // meet a breakpoint
            t1 = clock();
            total_runtime += t1 - last_clock;
            last_clock = t1;
            debug_watch();
            last_clock = clock(); // exclude these time 
        }
        int end_flag = do_line(now_line, lines[now_line], OUTPUT_PATH);
        cnt++;
        if(end_flag != 0) {
            break;
        }
    }
    t1 = clock();
    total_runtime += t1 - last_clock;
    last_clock = t1;
    std::cout << "--Total executed Lines: " << cnt << " --" << std::endl;
    std::cout << "--Total runtime: " << std::setprecision(6)
              << double(total_runtime) / CLOCKS_PER_SEC << "s.--" << std::endl;
}

// Do a line, in "now_line", with Line Struction "line"
int Simulator::do_line(unsigned int &now_line,
                       Line          line,
                       const char *  output_path) {
    Num _arg[3];
    // get a copy of line's arg
    for(int i = 0; i < 3; i++) {
        _arg[i] = line.get_arg(i);
    }
    // test part
    if(debug_mode) {
        std::cout << "  Current line: " << now_line;
        print_line(line);
    }
    // sort the lines to different types
    static char filename[1024];
    // clang-format off
    switch(lt()) {
        case UNDEF:case LINE_SYMBOL: 
            break;
        // DRAW, END
        case DRAW: case END:
            // get_print_name from status,  -2 -> 0 , -1 -> 1
            status.get_print_filename((bool)(lt() + 2), filename,output_path);
            switch(lt()) {
                case DRAW: 
                    status.print_to_bmp(filename);
                    break;
                case END: 
                    status.print_to_txt(filename);
                    break;
                default: break;
            }
            break;
        case LOAD:
            status.load(_ref(0), _val(1));
            status.set_reg_status(_raw(0), 1);
            status.set_reg_status(_raw(1), 0);
            status.set_memory_status(_val(1));
            break;
        case STORE:
            status.store(_val(0), _val(1));
            status.set_reg_status(_raw(0), 0);
            status.set_reg_status(_raw(1), 0);
            status.set_memory_status(_val(1));
            break;
        case READ:
            status.read(_ref(0));
            status.set_reg_status(_raw(0),1);
            break;
        case WRITE:
            status.write(_val(0));
            status.set_reg_status(_raw(0),0);
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
        case ADD: case SUB: case MUL: case DIV: case REM: case AND: case OR:
            status.op(_ref(0), _val(1), _val(2), lt() - 20); 
            status.set_reg_status(_raw(0), 1);
            status.set_reg_status(_raw(1), 0);
            status.set_reg_status(_raw(2), 0);
            break;
        // JAL, BEQ, BNE, BLT, BGE
        case JAL: 
            now_line = _val(0); 
            break;
        case BEQ: case BNE: case BLT: case BGE:
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
    // clang-format on
    if(debug_mode >= 2) {  // 2
        static int  cnt = 0;
        static char tmpfilename[1024];
        std::sprintf(tmpfilename, "%d.txt",
                     cnt);  // to be done: move it to output_path
        status.print_raw(tmpfilename);
        cnt++;
    }
    now_line++;  // jump to the next line
    if(lt() == END)
        return -1;  // set an end signal
    else
        return 0;  // next line!
}

// parse input file `FILENAME`(.risc)
// and store the instuctions in `Simulator.lines`
void Simulator::parse_file(const char *FILENAME) {
    // index of `UNFOUND_LINE` and `UNFOUND_LINE_ID`
    std::ifstream risc_file(FILENAME, std::ios::in);
    static char   line_str[Simulator::MAX_LINE_COL];  // instruction line
    std::cout << "---Parsing result:---" << std::endl;
    int current_line = 0;  // current line index (0-index)
    // parse the input file line by line
    while(risc_file.getline(line_str, sizeof(line_str))) {
        // setup an instruction line
        // std::sscanf(tmp_str, "%[^\n/]", line_str);
        for(int i = 0;; i++)
            if(line_str[i] == 0 || line_str[i] == '/' || line_str[i] == '\r' || line_str[i] == '\n') {
                line_str[i] = 0;
                break;
            }
        parse(line_str, lines[current_line], current_line, 0);
        // std::cout << line_str << std::endl;
        current_line++;
        if(current_line == MAX_INSTRUCTION) {
            std::cout << "Maximum instructions limit reached.\n"
                      << "Instructions after line" << MAX_INSTRUCTION
                      << "will be ignored." << std::endl;
            // maybe: raise exception?
            break;
        }
    }
    // deal with the instuctions whose argument [line_id]
    // appeared before its declaration
    while(unfound_index--) {
        parse(unfound_scpt[unfound_index], lines[unfound_line[unfound_index]],
              unfound_line[unfound_index], 1);  // re-parse the scripts
    }
    // record line_num into the simulator, to end the execute
    lines_num = current_line;
    risc_file.close();
    std::cout << "---Finish parsing.---" << std::endl;
    return;
}

// parse a single line `script` (the `current_line`th) and store it in `line`
void Simulator::parse(const char *script,
                      Line &      line,
                      int         current_line,
                      bool        unfounded) {
    static char name[Simulator::MAX_LINE_COL];  // instruction name
    std::sprintf(name, "UNDEFINED");
    Num args[3];                    // arguments
    int i        = 0;               // index of `line`
    int line_len = strlen(script);  // length of `line`

    std::cout << "  line: \"" << script << "\"" << std::endl;

    // receive instruction name

    if(line_len) {  // not blank line
        for(; i < line_len; ++i) {
            if(script[i] == ' ') {  // instrction name end
                name[i] = '\0';
                break;
            }
            if(script[i] == ':') {  // is line_ID
                name[i] = '\0';
                if(jump_line.count(name)) {
                    throw std::runtime_error("duplicate symbols:  ");
                    return;
                }
                jump_line[name] = current_line;  // the next line index // no!
                break;
            }
            name[i] = script[i];  // add a character to `name`
        }
        name[i] = '\0';
    }
    // `line_len` == 0, which means `script` is a blank line, `name` would stay
    // as its default value "UNDEF" receive arguments and setup a new `Line`
    int   s       = i;
    short type_id = TYPE.count(name) ? TYPE[name] : 0;
    // clang-format off
    switch(type_id) {
        // read/write [rd] 屏幕读写
        // 只写最后一个字节，但会全部覆盖
        // 只输出最低的字节
        case READ: case WRITE:
            s += add_arg(script+s, args[0], 0);
            line = Line(type_id, args, 1);
            break;
        // push/pop [r]
        case PUSH: case POP:
            s += add_arg(script + s, args[0], 0);
            line = Line(type_id, args, 1);
            break;
        // load/store/mov [r],[r]
        case LOAD: case STORE: case MOV:
            s += add_arg(script + s, args[0], 0);
            s += add_arg(script + s, args[1], 0);
            line = Line(type_id, args, 2);
            break;
        // add/sub/mul/rem/div [rd],[rs1],[rs2/imm]
        case ADD: case SUB: case MUL: case DIV: case REM: case AND: case OR:
            s += add_arg(script + s, args[0], 0);
            s += add_arg(script + s, args[1], 0);
            s += add_arg(script + s, args[2], 0);
            line = Line(type_id, args, 3);
            break;
        // jal [line_id] // call [line_id]
        case JAL: case CALL:
            s += add_arg(script + s, args[0], 1);
            if(~args[0].val){
                line = Line(type_id, args, 1);
            }
            else {
                if(unfounded) throw std::runtime_error("parsing error: missing symbols:  ");
                add_unfound;
                return;
            }
            break;
        // beq/bne/blt/bge [rs1],[rs2],[line_id]
        case BEQ: case BNE: case BLT: case BGE:
            s += add_arg(script + s, args[0], 0);
            s += add_arg(script + s, args[1], 0);
            s += add_arg(script + s, args[2], 1);
            if(~args[2].val){
                line = Line(type_id, args, 3);
            }
            else {
                if(unfounded) throw std::runtime_error("parsing error: missing symbols:  ");
                add_unfound;
                return;
            }
            break;
        // ret/end/draw/line_symbol/undef
        case RET: case END: case DRAW: case LINE_SYMBOL: case UNDEF:  
            line = Line(type_id, args, 0);
            break;
        default: 
            throw std::runtime_error("parsing error: no this operation!");        
            break;
    }
    if(script[line_len-1] == '*'){
        breakpoints.insert(current_line);
    }
    print_line(line);
    // clang-format on
    return;
}

void Simulator::print_debug_note() {
    std::cout << "Debug Mode on." << std::endl;
}
#define print_error                                                            \
    std::cout << "Please try again, or type `?` for help." << std::endl

// debug mode, watch varibles at breakpoints.
void Simulator::debug_watch() {
    std::cout << "Program stops at line " << now_line << "." << std::endl;
    while(true) {
        // clang-format off
        std::cout << "***Give a command: (`?` for help or `!` to quit), press enter to submit: ";
        // clang-format on
        static char op[MAX_LINE_COL];  // string to store operation
        std::cin.getline(op, sizeof(op));
        if(op[0] == '!') {  // quit
            break;
        }
        else if(op[0] == '?') {  // help
            // clang-format off
            std::cout << "---------BEGIN HELP---------" << std::endl;
            std::cout << "0. To add or delete a breakpoint: `add 3`/`del 33`" << std::endl;
            std::cout << "1. To get the value of a register: `a1`/`fp`/`zero`/`x31`" << std::endl;
            std::cout << "2. To get some bytes start from a memory address: `0x3f 123`/`100000 1`" << std::endl;
            std::cout << "3. To get top bytes of the stack: `stack 1`/`stack 13`" << std::endl;
            std::cout << "----------END HELP----------" << std::endl;
            // clang-format on
        }
        else if(op[0] == 'a' && op[1] == 'd' && op[2] == 'd') {  // add [x]
            std::istringstream is_op(op);
            std::string        tmp;
            unsigned int       pos;
            is_op >> tmp >> pos;
            breakpoints.insert(pos);
            std::cout << "Added breakpoint at line " << pos << "." << std::endl;
        }
        else if(op[0] == 'd' && op[1] == 'e' && op[2] == 'l') {  // del[x]
            std::istringstream is_op(op);
            std::string        tmp;
            unsigned int       pos;
            is_op >> tmp >> pos;
            if(!breakpoints.count(pos)) {
                std::cout << "ERROR: Breakpoint not found at line " << pos
                          << ". ";
                print_error;
            }
            else {
                breakpoints.erase(pos);
                std::cout << "Deleted breakpoint at line " << pos << "."
                          << std::endl;
            }
        }
        else if(op[0] == 's' && op[1] == 't' && op[2] == 'a') {  // stack [123]
            std::istringstream is_op(op);
            std::string        tmp;
            unsigned int       pos;
            is_op >> tmp;
            tmp.resize(0);
            is_op >> tmp;
            if(tmp[0] != 0) {
                is_op = std::istringstream(tmp);
                is_op >> pos;
                if(status.print_stack(pos) == -1)
                    std::cout << "ERROR: overflow.", print_error;
            }
            else {
                std::cout
                    << "ERROR: Stack operations must be followed by numbers.";
                print_error;
            }
        }
        else if(op[0] == 'd' && op[1] == 'i' && op[2] == 's') {  // disable
            debug_mode = 0;
        }
        else if(op[0] >= 'a' && op[0] <= 'z') {  // [register_name]
            if(REGISTER.count(op) == 0) {        // name not found
                std::cout << "ERROR: no register is named \"" << op << "\". ";
                print_error;
            }
            else {
                std::cout << "The value of register \"" << op << "\" is \""
                          << status.get_reg_val(REGISTER[op]) << "\"."
                          << std::endl;
            }
        }
        else if(op[0] >= '0' && op[0] <= '9') {  // memory
            std::istringstream is_op(op);
            unsigned int       a = -1, b = 1;
            // is_op >> a;
            if(op[1] == 'x') {
                is_op >> std::hex >> a;
                is_op >> std::dec >> b;
            }
            else {
                is_op >> std::dec >> a;
                is_op >> std::dec >> a;
            }
            if(status.print_memory(a, b) == -1)
                std::cout << "ERROR: overflow.", print_error;
        }
        else {  // what the fuck?
            std::cout << "Didn't understand. " << std::endl;
            print_error;
        }
    }
}
