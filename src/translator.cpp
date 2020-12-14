#include "Translator.h"
#include <cctype>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
/*
-----The EEC++ Language writing document-----
PART0 Introduction
    This language is used as a middle layer to generate code in ERISC language
from C++ alike code, which aims to save time in finishing Task 3 & 4 of this
Homework.
    This `Translator.cpp` is a translator to Translator EEC++ to the ERISC
language.

PART1 Basic Language rules
1. BASIC GRAMMAR RULES ARE C++ ALIKE

PART2 What you can do
1. write comments, which will be ignored
    a. multi-line comment
    b. single-line comment
2. declare a variable
    a. integer: short(int16_t), int(int32_t), long long(int64_t)
    b. floating number: float(32bit), double(64bit)
3. calculate
    a. basic operator:
        i. algebra: +, -, *, /
        ii. logical: && , || , ~


PART3 What you cannot do
1. YOU CANNOT USE C++ STANDARD LIBRARIES(only part functions are supported)
2. These types are not supported

*/

#define INF 0x3f3f3f3f

Translator::Translator(/* args */) {}
Translator::~Translator() {}

// read from "FILENAME"
int Translator::read(const char *FILENAME) {
    std::memset(scr, 0, sizeof(scr));
    std::ifstream f(FILENAME, std::ios::in);
    f.seekg(0, std::ios::end);
    int S = f.tellg();
    f.seekg(0, std::ios::beg);
    if(S > MAXLEN)
        throw std::runtime_error("script too long");
    f.read(scr, S);
    return S;
}

int split_to_parts(const char *scr, char **st_pos, char **ed_pos) {
    // return number of functions / declaration.
    // read script from `*scr`
    // store partition at `st_pos[i]` & `ed_pos[i]`
    // 第 i 个部分 的位置 [st_pos[i],ed_pos[i])
    // 1. 先找到 '{'
    // 1.1 如果没有找到 '{' ，找到 ";" ，直接 cut。
    // 1.2 如果找到 ‘}’ ，直接 cut
    // 没有考虑到注释，#define 等等（可以考虑加一个预编译阶段）
    int i = 0, cnt = 0, flag = 0, _from = 0;
    while(scr[i] != 0) {
        if(scr[i] == '{') flag = 1;
        if(scr[i] == '{') cnt++;
        if(scr[i] == '}') cnt--;
        if(cnt < 0) return -INF;
        if(scr[i] == ';' && flag == 0) {
            (*st_pos) = (char *)(scr);
            (*ed_pos) = (char *)(scr + i + 1);
            return 1 + split_to_parts(scr + i + 1, st_pos + 1, ed_pos + 1);
        }
        if(scr[i] == '}'){
            if(flag == 0) return -INF;
            if(flag == 1 && cnt == 0) {
                (*st_pos) = (char *)(scr);
                (*ed_pos) = (char *)(scr + i + 1);
                return 1 + split_to_parts(scr + i + 1, st_pos + 1, ed_pos + 1);
            }
        }
        i++;
    }
    return 0;
}


void Translator::print() {
    std::cout << scr;
}

void print_str(char *st,char *ed){
    while(st != ed) putchar(*(st++));
}

void Translator::parse() {
    // 整体思想：
    // 先预编译：处理注释、 #include
    //  
    #define MAXN 123123
    char **st = new char*[MAXN], **ed = new char*[MAXN];
    int t = split_to_parts(scr, st,ed);

    // printf("number:%d\n",t);
    // if(t < 0){

    // }
    // else{
    //     for(int i = 0;i<t;i++){
    //         printf("  translate:%d\n",i);
    //         print_str(st[i],ed[i]);
    //     }
    // }
}
