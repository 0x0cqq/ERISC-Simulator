#ifndef __OUTPUT_BMP_H_
#define __OUTPUT_BMP_H_

struct RGB {
    unsigned char r, g, b;
};

// template<size_t SIZ> ?
class BMP {
  private:
  public:
    void print(const char *FILENAME);
};

#endif
