#ifndef __SIMULATOR_H_
#define __SIMULATOR_H_

#include "program.h"
#include "status.h"

class Simulator {
  private:
    static const int MAX_INSTRUCTION = 10000;
    static const int MAX_LINE_COL    = 100;
    // index of lines whose argument [line_id] appeared before its declaration
    int unfound_index;
    int unfound_line[MAX_INSTRUCTION];
    // script of instuctions above
    char unfound_scpt[MAX_INSTRUCTION][Simulator::MAX_LINE_COL];
    Line             lines[MAX_INSTRUCTION];
    Status           status;
    unsigned int     now_line;
    unsigned int     lines_num;
    void parse(const char *script, Line &line, const int current_line,bool unfounded);
    int do_line(unsigned int &now_line, Line line);

  public:
    Simulator(/* args */);
    ~Simulator();
    void reset();
    void parse_file(const char *FILENAME);
    void execute(unsigned int stop_line = -1);
};

#endif