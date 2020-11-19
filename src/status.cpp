#include "status.h"
#include <cstdio>

Status::Status(/* args */) {
    stack_ptr = stack + STACK_SIZE;
    draw_time = 0;
}

Status::~Status() {}

unsigned int Status::read_4_byte(unsigned char *ptr) {
    unsigned int ans = 0;
    for(int i = 0; i < 4; i++) {
        ans = (ans << 8) | (unsigned int)(ptr[i]);
    }
    return ans;
}
void Status::write_4_byte(unsigned char *ptr, unsigned int x) {
    for(int i = 0; i < 4; i++) {
        ptr[i] = (unsigned char)(x & ((1 << 8) - 1));
        x >>= 8;
    }
}
// load data from memory[ptr] to register rd
void Status::load(unsigned int &rd, unsigned int ptr) {
    rd = read_4_byte(memory + ptr);
}
// store data from register rs to memory[ptr]
void Status::store(unsigned int rs, unsigned int ptr) {
    write_4_byte(memory + ptr, rs);
}
// push register rs into stack
void Status::push(unsigned int rs) {
    stack_ptr -= 4;
    write_4_byte(stack_ptr, rs);
}
// pop stack and store it into register
void Status::pop(unsigned int &rd) {
    if(stack_ptr == stack + STACK_SIZE) {
        // a exception
    }
    else {
        rd = read_4_byte(stack_ptr);
        stack_ptr += 4;
    }
}
// mov value of register rs to rd
void Status::mov(unsigned int &rd, unsigned int rs) {
    rd = rs;
}
void Status::op(unsigned int &rd,
                unsigned int  rs1,
                unsigned int  rs2,
                short         type) {
    // type = operation_type - 20
    // for example , add is 0,  sub = 1 , ...
    switch(type) {
        case 0:
            rd = rs1 + rs2;
            break;
        case 1:
            rd = rs1 - rs2;
            break;
        case 2:
            rd = rs1 * rs2;
            break;
        case 3:
            rd = rs1 / rs2;
            break;
        case 4:
            rd = rs1 % rs2;
            break;
        case 5:
            rd = rs1 & rs2;
            break;
        case 6:
            rd = rs1 | rs2;
            break;
        default:
            break;
    }
}
void Status::get_print_filename(bool op, char *FILENAME) {
    if(op == 0) {
        std::sprintf(FILENAME, "%d.bmp", ++draw_time);
    }
    else {
        std::sprintf(FILENAME, "result.txt");
    }
}
void Status::print_to_bmp(const char *FILENAME) {
    return;
}
void Status::print_to_txt(const char *FILENAME) {
    return;
}
unsigned int Status::get_reg_val(unsigned short pos) {
    return x[pos];
}
unsigned int &Status::get_reg_ref(unsigned short pos) {
    return x[pos];
}