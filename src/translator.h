#ifndef __Translator_H_
#define __Translator_H_
#include "program.h"

class Function {
    
};

class Translator {
  private:
    static const int MAXLEN = (1<<20);
    char scr[MAXLEN];
  public:
    int read(const char *FILENAME);
    void print();
    void parse();
    Translator();
    ~Translator();
};
#endif