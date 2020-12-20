#ifndef __SIMULATOR_H_
#define __SIMULATOR_H_

#include "program.h"
#include "status.h"
#include <unordered_set>

class Simulator {
  private:
    static const int MAX_INSTRUCTION = 10000;
    static const int MAX_LINE_COL    = 100;
    int              debug_mode;
    // index of lines whose argument [line_id] appeared before its declaration
    int unfound_index;
    int unfound_line[MAX_INSTRUCTION];
    // script of instuctions above
    char         unfound_scpt[MAX_INSTRUCTION][Simulator::MAX_LINE_COL];
    Line         lines[MAX_INSTRUCTION];
    Status       status;
    unsigned int now_line;
    unsigned int lines_num;

    void parse(const char *script,
               Line &      line,
               const int   current_line,
               bool        unfounded);
    int  do_line(unsigned int &now_line, Line line, const char *output_path);
    std::unordered_set<unsigned int> breakpoints;

  public:
    Simulator(/* args */);
    ~Simulator();
    void reset();
    // debug functions
    void set_debug_mode(int m);
    void print_debug_note();
    void debug_watch();
    // real functions
    void parse_file(const char *FILENAME);
    void execute(const char *OUTPUT_PATH, unsigned int stop_line = -1);
};

#endif