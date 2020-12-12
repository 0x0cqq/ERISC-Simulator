#ifndef __TRANSLATE_H_
#define __TRANSLATE_H_
#include "program.h"

class Function {
    
};

class Translate {
  private:
    static const int MAXLEN = (1<<20);
    char scr[MAXLEN];
  public:
    int read(const char *FILENAME);
    void print();
    void parse();
    Translate(/* args */);
    ~Translate();
};
#endif