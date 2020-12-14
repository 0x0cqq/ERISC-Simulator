#ifndef __SIMULATOR_H_
#define __SIMULATOR_H_

#include "program.h"
#include "status.h"

class Simulator {
  private:
    static const int MAX_INSTRUCTION = 10000;
    static const int MAX_LINE_COL    = 100;
    Line             lines[MAX_INSTRUCTION];
    Status           status;
    unsigned int     now_line;
    unsigned int     lines_num;
    void parse(const char *script, Line &line, const int current_line);
    void do_line(unsigned int &now_line, Line line);

  public:
    Simulator(/* args */);
    ~Simulator();
    void parse_file(const char *FILENAME);
    void execute(unsigned int stop_line = -1);
};

#endif