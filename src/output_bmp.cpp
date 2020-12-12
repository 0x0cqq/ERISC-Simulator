#include "output_bmp.h"

void BMP::wr(int x, int s = 1) {
    lst[++pct][0] = x, lst[pct][1] = s;
    sz += s;
}

void BMP::upd() {
    std::fwrite("BM", 2, 1, fout);
    std::fwrite(&sz, 4, 1, fout);
    for(int i = 1; i <= pct; i++)
        std::fwrite(&lst[i][0], lst[i][1], 1, fout);
}

void BMP::val(unsigned char a[3], unsigned char b[3]) {
    for(int i = 0; i < 3; ++i)
        a[i] = b[i];
    return;
}
void BMP::colorr(coor a, unsigned char csign[3]) {
    for(int i = x + a.lx - 1; i >= x + a.rx - 1; --i) {
        for(int j = y + a.ly; j <= y + a.ry; ++j) {
            val(col[i][j], csign);
        }
    }
}
void BMP::color(coor a, int sign) {
    if(sign == 0 or sign == 4 or sign == 6) {
        colorr(a, cwh);
    }
    if(sign == 1) {
        colorr(a, cblu);
    }
    if(sign == 2) {
        colorr(a, cre);
    }
    if(sign == 3) {
        colorr(a, cpur);
    }
    if(sign == 5) {
        colorr(a, cgr);
    }
    if(sign == 7) {
        colorr(a, cye);
    }
}

void BMP::print(const char *   FILENAME,
                unsigned short reg_rw[32],
                bool           mem_rw[16],
                bool           stack_rw) {
    for(int i = 0; i < 32; ++i) {
        ro       = i / 8;
        co       = i % 8;
        re[i].lx = hh - ((ro + 1) * fr + ro * h);
        re[i].ly = (co + 1) * fr + co * l1;
        re[i].rx = hh - ((ro + 1) * (fr + h) - 1);
        re[i].ry = (co + 1) * (fr + l1) - 1;
    }
    int add = 8 * (l1 + fr) + fr;
    for(int i = 0; i < 16; ++i) {
        ro       = i / 4;
        co       = i % 4;
        me[i].lx = hh - ((ro + 1) * fr + ro * h);
        me[i].ly = add + (co + 1) * fr + co * l2;
        me[i].rx = hh - ((ro + 1) * (fr + h) - 1);
        me[i].ry = add + (co + 1) * (fr + l2) - 1;
    }
    add += 4 * (l2 + fr) + fr;
    sta.lx = hh - fr;
    sta.ly = add + fr;
    sta.rx = hh - (4 * (h + fr) - 1);
    sta.ry = add + fr + l3 - 1;

    for(int i = 0; i < H; ++i) {
        for(int j = 0; j < W; ++j) {
            val(col[i][j], cwh);
        }
    }

    // draw frame--horizontal line
    for(int i = x; i <= x + 4 * (h + fr); i += h + fr) {
        for(int j = 0; j < fr; ++j) {
            for(int k = y; k < y + 8 * (l1 + fr) + fr + 4 * (l2 + fr) + fr;
                ++k) {
                val(col[i + j][k], cbla);
            }
        }
    }
    // draw frame--horizontal line--extend upper&bottom line
    for(int i = x; i <= x + 4 * (h + fr); i += 4 * (h + fr)) {
        for(int j = 0; j < fr; ++j) {
            for(int k = y + 8 * (l1 + fr) + fr + 4 * (l2 + fr) + fr;
                k < y + 8 * (l1 + fr) + fr + 4 * (l2 + fr) + fr + l3 + fr;
                ++k) {
                val(col[i + j][k], cbla);
            }
        }
    }
    // draw frame--vertical line--register
    for(int i = y; i <= y + 8 * (l1 + fr); i += l1 + fr) {
        for(int j = 0; j < fr; ++j) {
            for(int k = x; k < x + 4 * (h + fr) + fr; ++k) {
                val(col[k][i + j], cbla);
            }
        }
    }
    // draw frame--vertical line--memory
    add = y + 8 * (l1 + fr) + fr;
    for(int i = 0; i <= 4 * (l2 + fr); i += l2 + fr) {
        for(int j = 0; j < fr; ++j) {
            for(int k = x; k < x + 4 * (h + fr) + fr; ++k) {
                val(col[k][i + j + add], cbla);
            }
        }
    }
    // draw frame--vertical line--stack
    add += 4 * (l2 + fr) + fr;
    for(int i = 0; i <= l3 + fr; i += l3 + fr) {
        for(int j = 0; j < fr; ++j) {
            for(int k = x; k < x + 4 * (h + fr) + fr; ++k) {
                val(col[k][i + j + add], cbla);
            }
        }
    }

    // fill color for register n

    for(int i = 0; i <= 31; i++) {
        color(re[i], (int)reg_rw[i]);
    }

    // fill color for memory n
    for(int i = 0; i < 16; ++i) {
        color(me[i], (int)mem_rw[i] + 4);
    }

    // fill color for stack
    color(sta, (int)stack_rw + 6);

    fout = std::fopen(FILENAME, "wb");
    wr(0, 4), wr(54, 4);                                 //- 数据起始地址
    wr(40, 4), wr(W, 4), wr(H, 4), wr(1, 2), wr(24, 2);  //- 宽 高 - 色深
    wr(0, 4), wr(0, 4), wr(1, 4), wr(1, 4), wr(0, 4), wr(0, 4);
    for(int i = 0; i < H; i++) {
        for(int j = 0; j < W; j++) {
            for(int k = 0; k < 3; k++)
                wr(col[i][j][k], 1);
        }
        if((W * 3) % 4 != 0)
            wr(0, 4 - W * 3 % 4);
    }

    upd();
    std::fclose(fout);
}