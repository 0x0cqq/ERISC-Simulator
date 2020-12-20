#include "status.h"
#include "output_bmp.h"
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>

// build status from null
Status::Status(/* args */) {
    reset();
}

// reset whole object
void Status::reset() {
    for(int i = 0; i < REGISTER_NUM; i++)
        x[i] = 0;
    for(int i = 0; i < MEMORY_SIZE; i++)
        memory[i] = 0;
    for(int i = 0; i < STACK_SIZE; i++)
        stack[i] = 0;
    stack_ptr = stack + STACK_SIZE;
    draw_time = 0;
}

Status::~Status() {}

// print `cnt` bytes using hex, start from `scr+_start`, with beautiful syntax
void print_bytes(unsigned char *scr, unsigned int _start, unsigned int cnt) {
    for(unsigned int i = 0; i < cnt; i++) {
        if(i % 16 == 0) { // header
            std::cout << std::endl;
            std::cout << std::setiosflags(std::ios::uppercase) << std::setw(6)
                      << std::setfill('0') << std::setprecision(8) << std::hex
                      << (unsigned int)(_start + i) << ": ";
        }
        std::cout << std::setiosflags(std::ios::uppercase) << std::setw(2)
                  << std::setfill('0') << std::setprecision(8) << std::hex
                  << (unsigned int)*(scr + _start + i) << " ";
    }
}

// print `cnt` bytes in array memory[] from memory[start_pos]
int Status::print_memory(unsigned int start_pos, unsigned int cnt) {
    if(start_pos > MEMORY_SIZE || start_pos + cnt > MEMORY_SIZE) {
        return -1; // overflow
    }
    else {
        std::cout << "start_pos:" << start_pos << " cnt:" << cnt << std::endl;
        std::cout << "--[little--->big]--";
        print_bytes(memory, start_pos, cnt);
        std::cout << std::endl;
        return 0;
    }
}

// print top `cnt` bytes in stack
int Status::print_stack(unsigned int cnt) {
    std::cout << "stack size: " << stack_ptr - stack;

    if((long long)stack_ptr + (long long)cnt >
       (long long)stack + (long long)STACK_SIZE) {
        std::cout << std::endl;
        return -1; // overflow
    }
    else {
        std::cout << "--[top--->bottom]--";
        print_bytes(stack, stack_ptr - stack, cnt);
        std::cout << std::endl;
        return 0;
    }
}

// read 4 byte from pointer `ptr`
unsigned int Status::read_4_byte(unsigned char *ptr) {
    unsigned int ans = 0;
    for(int i = 0; i < 4; i++) {
        ans = (ans << 8) | (unsigned int)(ptr[4 - i - 1]);
    }
    return ans;
}
// write 4 byte from pointer `ptr`
void Status::write_4_byte(unsigned char *ptr, unsigned int x) {
    for(int i = 0; i < 4; i++) {
        ptr[i] = (unsigned char)(x & ((1 << 8) - 1));
        x >>= 8;
    }
}
// read one char from cin, to rd's low 8 bit
void Status::read(unsigned int &rd) {
    rd = (~((~rd) & ((1 << 8) - 1))) | (unsigned int)(std::cin.get());
}
// write one char to cout, from rs's high 8 bit
void Status::write(unsigned int rs) {
    std::cout.put((rs & ((1 << 8) - 1)));
}
// load data from memory[ptr] to register rd
void Status::load(unsigned int &rd, unsigned int ptr) {
    if(ptr + 4 > MEMORY_SIZE)
        throw std::runtime_error("memory overread");
    rd = read_4_byte(memory + ptr);
}
// store data from register rs to memory[ptr]
void Status::store(unsigned int rs, unsigned int ptr) {
    if(ptr + 4 > MEMORY_SIZE)
        throw std::runtime_error("memory overstore");
    write_4_byte(memory + ptr, rs);
}
// push register rs into stack
void Status::push(unsigned int rs) {
    if(stack_ptr == stack)
        throw std::runtime_error("stack overpush");
    stack_ptr -= 4;
    write_4_byte(stack_ptr, rs);
}
// pop stack and store it into register
void Status::pop(unsigned int &rd) {
    if(stack_ptr == stack + STACK_SIZE) {
        throw std::runtime_error("stack overpop");
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
// do arithmetic operations
void Status::op(unsigned int &rd,
                unsigned int  rs1,
                unsigned int  rs2,
                short         type) {
    // type = operation_type - 20
    // for example , add is 0,  sub = 1 , ...
    switch(type) {
        case 0: rd = rs1 + rs2; break;
        case 1: rd = rs1 - rs2; break;
        case 2: rd = rs1 * rs2; break;
        case 3: rd = rs1 / rs2; break;
        case 4: rd = rs1 % rs2; break;
        case 5: rd = rs1 & rs2; break;
        case 6: rd = rs1 | rs2; break;
        default: break;
    }
}
// get output file's name
void Status::get_print_filename(bool        op,
                                char *      FILENAME,
                                const char *STORE_PATH) {
    if(op == 0) {  // `draw` action
        std::sprintf(FILENAME, "%s/%d.bmp", STORE_PATH, ++draw_time);
    }
    else if(op == 1) {  // `end` action
        std::sprintf(FILENAME, "%s/result.txt", STORE_PATH);
    }
}
// output current state to `FILENAME`
void Status::print_to_bmp(const char *FILENAME) {
    BMP *bmp = new BMP;
    bmp->print(FILENAME, output_status.reg_rw, output_status.mem_rw,
               output_status.stack_rw);
    delete bmp;
}
// output current state to `FILENAME`
void Status::print_to_txt(const char *FILENAME) {
    std::ofstream f_out(FILENAME);
    for(int i = 0; i < REGISTER_NUM; i++) {
        f_out << std::setiosflags(std::ios::uppercase) << std::setw(8)
              << std::setfill('0') << std::setprecision(8) << std::hex << x[i];
        f_out << (i == REGISTER_NUM - 1 ? "\n" : " ");
    }
    const int LINE_LEN = 64;
    for(int i = 0; i < MEMORY_SIZE; i++) {
        f_out << std::setiosflags(std::ios::uppercase) << std::setw(2)
              << std::setfill('0') << std::setprecision(8) << std::hex
              << (unsigned short)memory[i];
        f_out << (i % LINE_LEN == LINE_LEN - 1 ? "\n" : " "); // no trailing space at line's end
    }
    f_out.close();
    return;
}
// print raw(binary) data to `FILENAME`
void Status::print_raw(const char *FILENAME) {
    std::ofstream f_out(FILENAME, std::ios::binary);
    f_out.write((char *)(x), sizeof(x));
    f_out.write((char *)(memory), sizeof(memory));
    f_out.write((char *)(stack), sizeof(stack));
    // f_out.writeline("stack_ptr:%d\n",int(stack_ptr - stack));
    f_out.close();
}
// set register output status
// op == 0 read, op == 1 write
// status == 3 --> r&w; status == 0 --> x; status == 1 --> r; status == 2 --> w;
void Status::set_reg_status(unsigned short pos, bool op) {
    if(pos == (unsigned short)(-1))
        return;
    output_status.reg_rw[pos] |= (1 << op);
}
// set memory output status
void Status::set_memory_status(unsigned int ptr) {
    if(ptr == (unsigned int)(-1))
        return;
    output_status.mem_rw[ptr / output_status.MEMORY_BLOCK_SIZE] = true;
}
// set stack output status
void Status::set_stack_status() {
    output_status.stack_rw = true;
}
// get register's value
unsigned int Status::get_reg_val(unsigned short pos) {
    if(pos > REGISTER_NUM)
        throw std::runtime_error("register_val out of range");
    return x[pos];
}
// get register's reference
unsigned int &Status::get_reg_ref(unsigned short pos) {
    if(pos > REGISTER_NUM)
        throw std::runtime_error("register_val out of range");
    return x[pos];
}