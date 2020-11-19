#include "status.h"

Status::Status(/* args */) {}

Status::~Status() {}

void Status::load(unsigned int &rd, unsigned int ptr) {
    
}
void Status::store(unsigned int rs, unsigned int ptr) {}
void Status::push(unsigned int rs) {}
void Status::pop(unsigned int &rd) {}
void Status::mov(unsigned int &rd, unsigned int rs) {}
void Status::op(unsigned int &rd,
                unsigned int  rs1,
                unsigned int  rs2,
                short         type) {
    // type = operation_type - 20
    // for example , add is 0,  sub = 1 , ...
}
void         Status::get_print_filename(bool op, char *FILENAME) {}
void         Status::print_to_bmp(const char *FILENAME) {}
void         Status::print_to_txt(const char *FILENAME) {}
unsigned int Status::get_reg_val(unsigned int pos) {
    return 0;
}
unsigned int &Status::get_reg_ref(unsigned int pos) {}