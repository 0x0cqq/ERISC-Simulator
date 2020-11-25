#include "translate.h"

/*
-----The EEC++ Language writing document-----
PART0 Introduction
    This language is used as a middle layer to generate code in ERISC language
from C++ alike code, which aims to save time in finishing Task 3 & 4 of this
Homework.
    This `translate.cpp` is a translator to translate EEC++ to the ERISC language.

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

Translate::Translate(/* args */) {}

Translate::~Translate() {}
