# ERISC-Simulator

[TOC]

## 简介

ERISC（Extremely Reduced Instruction Set Computer, 极度精简指令集）的 C++ 模拟器，是清华大学2020年秋季学期“程序设计基础(30240233-2)”课程的期末大作业。

A C++ simulator of the ERISC (Extremely Reduced Instruction Set Computer), which is the final project of the course "Fundamentals of Programming(30240233-2)" in Tsinghua University.

### 碎碎念

> 我觉得，这个项目里（绝大多数的）代码，还是很优雅的，对吗？

## 小组人员及分工

| 姓名   | 班级   | 学号 | 分工                                                         |
| ------ | ------ | ---- | ------------------------------------------------------------ |
| 陈启乾 | 软件03 | \*\*\*\*\*\*\*\*\*\* | （组长）<br/>总体框架搭建<br/>`simulator` 类的 `execute` 与 `do_line` 函数<br/>其他杂七杂八的函数 |
| 任自厚 | 软件02 |  \*\*\*\*\*\*\*\*\*\*    | `simulator` 类的 `parse` 相关函数                            |
| 武文静 | 计05   |  \*\*\*\*\*\*\*\*\*\*    | `output_bmp` 的相关函数、`status` 类的函数                   |
| 梅雨   | 计06   |  \*\*\*\*\*\*\*\*\*\*    | `output_bmp` 的相关函数、`status` 类的函数                   |

## 程序架构

本程序采用面向对象的方法编程，`src` 目录文件组织如下：

```
.
├── main.cpp
├── output_bmp.cpp
├── output_bmp.h
├── program.cpp
├── program.h
├── simulator.cpp
├── simulator.h
├── status.cpp
├── status.h
├── translate.cpp
└── translate.h
```

文件作用及构成介绍如下：

### `main.cpp`：

调用 `simulator.h` 中的 `Simulator`  类。

主要程序，完成对 ERISC 源代码的接收，根据源代码对 `Simulator` 类的初始化，下达运行源代码的指令。

### `status.h` & `status.cpp`

调用 `output_bmp.h` 。

定义 `Status` 类，用来记录虚拟的“电脑”的寄存器、内存、栈的数值和使用状态，和输出可视化的结果。

### `program.h` & `program.cpp`

无调用。

定义 `Num` 类，用作多用途的“数“（既可以表示立即数，也可以表示某个寄存器中的数）。

定义 `Line` 类，用来存储 ERISC 程序中的某行，记录该行的类型、1～3个参数。

### `simulator.h` & `simulator.cpp`

调用 `program.h` 与 `status.h`

定义 `Simulator` 类，用来模拟程序的运行；内含一个 `Status` 对象，用来存储“虚拟机”的物理情况；内含若干 `Line` 对象，用来存储源代码。类中包含 `parse` 方法，用来将源代码结构化并存储到 `Line` 对象中；类中包含 `execute` 方法和 `do_line` 方法，用来执行代码，修改虚拟机中的物理情况。

### `output_bmp.h` & `output_bmp.cpp`

无调用。

定义 `BMP` 类，用于接收 `status` 类传递的空间使用状态，并输出可视化的BMP。

### `translate.h` & `translate.cpp`

无调用。

定义 `Translate` 类型，用于翻译一种类 C++ 语言代码至 ERISC 代码。（还没写完）

## 任务完成情况

### 必做任务

程序模拟：完全完成

结果可视化（bmp）输出：完全完成

结果文本（txt）输出：完全完成

必做任务1：成功运行，输出结果见 `./output/test_1` 。

必做任务2：成功运行，输出结果见 `./output/test_2`。

必做任务3：（由 xxx 编写 C++ 代码，由该项目翻译为 ERISC 代码，成功运行。输出结果见 `./output/test_3` 。）

必做任务4：（由 xxx 编写 C++ 代码，由该项目翻译为 ERISC 代码，成功运行。输出结果见 `./output/test_4` 。）

### 选做任务

（选座任务0：在 ERISC 模拟器中加入文件读写与屏幕读写。）

（选做任务1：将类C++代码翻译为 ERISC 代码。）

（选做任务2：将该项目翻译为 ERISC 代码。）

## 使用说明

### 关于 `main.cpp` 主函数的参数情况

包括可执行文件本身，`main.cpp` 接收三个参数：

1. 第一个参数是可执行文件本身
2. 第二个参数是执行任务类型【只有第一个字母有意义】
   1. `e(xecute)` 代表运行 ERISC 程序
   2. `t(ranslate)` 代表翻译类 C++ 语言代码成为 ERISC 语言代码。
3. 第三个参数给出输入文件路径。

例如：

`$ ./bin/main execute ./input/test_1.risc` 将会运行第一个示例代码。

（`$ ./bin/main translate ./input/test_3.cpp` 将会把 `test_3.cpp` 翻译成为 ERISC 代码并输出。）

### 关于项目编译

本项目配上了一个 `makefile` 文件，里面使用了 `findall` 命令，默认使用的编译器命令是 `g++-10` ，如需编译项目，可根据具体情况使用 `makefile` 文件。`makefile` 编译出来的可执行文件放在 `./bin/main` 文件处

或者直接编译 `main.cpp` 亦可。

### 哪些undefined的行为被我们defined了

1. ERISC 源代码中可以出现空行，会被忽略。

### 哪些行为会出问题

1. ERISC 源代码最大 10000 行，如果超出将只阅读前 10000 行。
2. ERISC 源代码每行不能超过 100个字符。
3. ERISC 源代码只支持 ASCII码。

### 关于寄存器、内存限制的修改

1. 可以在 `status.h` 中修改 `MEMORY_SIZE` 、`STACK_SIZE` 、`REGISTER_NUM` 手动扩容寄存器、内存，但是这并没有什么用处，因为其他地方都没适配

### 关于 `translate.cpp` 的使用