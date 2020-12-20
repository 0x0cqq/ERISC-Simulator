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
    unsigned char *  stack_ptr;  // top of the stack operator
    unsigned int     draw_time;
    unsigned int     read_4_byte(unsigned char *ptr);
    void             write_4_byte(unsigned char *ptr, unsigned int x);
    // record status for output
    struct Output_status {
        // op == 0 read, op == 1 write
        // status == 3 --> r&w; status == 0 --> x;
        // status == 1 --> r; status == 2 --> w;
        static const int MEMORY_BLOCK_SIZE = ((1 << 8) << 10);
        unsigned short   reg_rw[Status::REGISTER_NUM];
        bool             mem_rw[Status::MEMORY_SIZE / MEMORY_BLOCK_SIZE];
        bool             stack_rw;
        Output_status() {
            for(int i = 0; i < Status::REGISTER_NUM; i++) {
                reg_rw[i] = 0;
            }
            for(int i = 0; i < Status::MEMORY_SIZE / MEMORY_BLOCK_SIZE; i++) {
                mem_rw[i] = 0;
            }
            stack_rw = 0;
        }
    } output_status;

  public:
    Status(/* args */);
    void reset();
    ~Status();

    // simulator functions
    void load(unsigned int &rd, unsigned int ptr);
    void store(unsigned int rs, unsigned int ptr);
    void read(unsigned int &rd);
    void write(unsigned int rs);
    void push(unsigned int rs);
    void pop(unsigned int &rd);
    void mov(unsigned int &rd, unsigned int rs);
    void op(unsigned int &rd, unsigned int rs1, unsigned int rs2, short type);

    // output functions
    void get_print_filename(bool op, char *FILENAME, const char *STORE_PATH);
    void print_to_bmp(const char *FILENAME);  // op = 0
    void print_to_txt(const char *FILENAME);  // op = 1
    void print_raw(const char *FILENAME);
    // op == 0 read, op == 1 write
    void set_reg_status(unsigned short pos, bool op);
    void set_memory_status(unsigned int ptr);
    void set_stack_status();
    
    // help functions
    unsigned int  get_reg_val(unsigned short pos);
    unsigned int &get_reg_ref(unsigned short pos);
    
    // debug functions
    int print_stack(unsigned int cnt);
    int print_memory(unsigned int start_pos, unsigned int cnt);
};

#endif
