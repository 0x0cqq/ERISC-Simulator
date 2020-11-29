#ifndef __TRANSLATE_H_
#define __TRANSLATE_H_
class Translate {
  private:
    static const int MAXLEN = (1<<20);
    char script[MAXLEN];
  public:
    void read(const char *FILENAME);
    // void Translate::parse();
    Translate(/* args */);
    ~Translate();
};
#endif