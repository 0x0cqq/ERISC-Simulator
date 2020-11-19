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
    unsigned int     stack_ptr;
    unsigned int     draw_time;

  public:
    Status(/* args */);
    ~Status();
    void load(unsigned int &rd, unsigned int ptr);
    void store(unsigned int rs, unsigned int ptr);
    void push(unsigned int rs);
    void pop(unsigned int &rd);
    void mov(unsigned int &rd, unsigned int rs);
    void op(unsigned int &rd, unsigned int rs1, unsigned int rs2, short type);
    void get_print_filename(bool op,char *FILENAME); 
    void print_to_bmp(const char *FILENAME); // op = 0
    void print_to_txt(const char *FILENAME); // op = 1
    unsigned int  get_reg_val(unsigned int pos);
    unsigned int &get_reg_ref(unsigned int pos);
};

#endif  // __STATUS_H_
