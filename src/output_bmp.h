#include <cstdio>
#ifndef __OUTPUT_BMP_H_
#    define __OUTPUT_BMP_H_

// template<size_t SIZ> ?
class BMP {
  private:
    static const int maxn = 5e3 + 10;
    struct coor {
        int lx;
        int ly;
        int rx;
        int ry;
    };
    coor          re[32];
    coor          me[16];
    coor          sta;
    int           ro, co;
    int           W = 300, H = 200;
    unsigned char col[maxn][maxn][3];
    int           h = 7, fr = 1, l1 = 10, l2 = 9, l3 = 20;
    int           hh = 4 * (fr + h) + fr;
    int           ls = 8 * (fr + l1) + fr + 4 * (fr + l2) + fr + 2 * fr + l3;
    int           x  = (H - hh) / 2;
    int y = (W - (8 * (fr + l1) + fr + 4 * (fr + l2) + fr + 2 * fr + l3)) / 2;
    // lower left corner of the diagram
    unsigned char cbla[3] = {0, 0, 0};
    unsigned char cwh[3]  = {255, 255, 255};
    unsigned char cye[3]  = {0, 192, 255};
    unsigned char cre[3]  = {0, 0, 255};
    unsigned char cgr[3]  = {0, 255, 0};
    unsigned char cblu[3] = {255, 0, 0};
    unsigned char cpur[3] = {255, 0, 255};

    int   lst[maxn * maxn][2], pct, sz = 6;
    FILE *fout;

  public:
    void print(const char *   FILENAME,
               unsigned short reg_rw[32],
               bool           mem_rw[16],
               bool           stack_rw);
    void wr(int x, int s);
    void upd();
    void val(unsigned char a[3], unsigned char b[3]);
    void colorr(coor a, unsigned char csign[3]);
    void color(coor a, int sign);
};

#endif
