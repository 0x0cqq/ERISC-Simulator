#ifndef __STATUS_H_
#define __STATUS_H_

class Status {
  private:
    static const int REGISTER_NUM = 32;
    static const int MEMORY_SIZE  = 4 * (1 << 20);
    static const int STACK_SIZE   = 4 * (1 << 20);
    unsigned int     x[REGISTER_NUM];
    unsigned char    memory[MEMORY_SIZE];
    unsigned char    stack[STACK_SIZE];
    unsigned char *  stack_ptr; // top of the stack operator
    unsigned int     draw_time;
    unsigned int  read_4_byte(unsigned char *ptr);
    void          write_4_byte(unsigned char *ptr, unsigned int x);
  public:
    Status(/* args */);
    ~Status();
    void load(unsigned int &rd, unsigned int ptr);
    void store(unsigned int rs, unsigned int ptr);
    void push(unsigned int rs);
    void pop(unsigned int &rd);
    void mov(unsigned int &rd, unsigned int rs);
    void op(unsigned int &rd, unsigned int rs1, unsigned int rs2, short type);
    void get_print_filename(bool op, char *FILENAME);
    void print_to_bmp(const char *FILENAME);  // op = 0
    void print_to_txt(const char *FILENAME);  // op = 1
    unsigned int  get_reg_val(unsigned short pos);
    unsigned int &get_reg_ref(unsigned short pos);
};

#endif
