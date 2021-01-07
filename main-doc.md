# ERISC-Simulator

[TOC]

## 简介

本程序是 ERISC（Extremely Reduced Instruction Set Computer, 极度精简指令集）的 C++ 模拟器，是清华大学2020年秋季学期“程序设计基础(30240233-2)”课程的期末大作业。

This is a C++ simulator of the ERISC (Extremely Reduced Instruction Set Computer), which is the final project of the course "Fundamentals of Programming(30240233-2)" in Tsinghua University.

## 亮点

总体上来说，本项目浅度使用了面向对象的编程技术，采用类和结构体对程序内容进行封装，以达到代码的易用性、易扩展性、易维护性。

具体来说，小组成员认为本项目有如下亮点：

+ 代码注释充分：绝大多数核心代码均有注释。
+ 代码风格一致：组内使用 VSCode 的 Clang-format 插件统一代码风格。
+ 程序结构清晰：我们先进行了总体框架的搭建，随后才进行具体代码工作。（具体可见开发日志）
+ 开发记录完整：我们使用 GitHub 管理代码，所有代码历史公开可查 ([ERISC-Simulator](https://github.com/ChenQiqian/ERISC-Simulator)) 。
+ 程序实现精巧：虽然有大量注释、头文件、附加功能，但本项目总体代码仅有约 1200 行。
  + 寄存器和立即数的处理：采用一个结构体同一了两者，简化了参数传递。
  + 巧用宏定义减少代码量：定义了 `_val` 、`_ref` 等宏定义，满足引用传参的要求。
+ 程序稳健性强：对于 `line_symbol` 、操作的操作数等易错的地方，进行了合法性检查并抛出异常。
+ 程序人性化好：输出了关键的过程信息，拥有调试模式（含帮助）助力汇编代码的编写与调试。

## 小组人员及分工

合影：

[See Photo](photo.jpg)

| 姓名   | 班级   | 学号 | 分工                                                         |
| ------ | ------ | ---- | ------------------------------------------------------------ |
| 陈启乾 | 软件03 | \*\*\*\*\*\*\*\*\*\* | （组长）<br/>总体框架搭建<br/>`simulator` 类的 `execute` 与 `do_line` 函数<br/>杂七杂八的工作 |
| 任自厚 | 软件02 |  \*\*\*\*\*\*\*\*\*\*    | 总体框架搭建<br/>`simulator` 类的 `parse` 相关函数                 |
| 武文静 | 计05   |  \*\*\*\*\*\*\*\*\*\*    | `output_bmp` 的相关函数、`status` 类的函数                   |
| 梅雨   | 计06   |  \*\*\*\*\*\*\*\*\*\*    | `output_bmp` 的相关函数、`status` 类的函数                   |

## 程序整体架构

本程序采用面向对象的方法编程，`src` 目录文件组织如下：

```
./src/
├── main.cpp
├── output_bmp.cpp
├── output_bmp.h
├── program.cpp
├── program.h
├── simulator.cpp
├── simulator.h
├── status.cpp
└── status.h
```

文件作用及构成介绍如下：

### `main.cpp`

调用 `simulator.h` 中的 `Simulator`  类。

主要程序，完成对 ERISC 源代码的接收，根据源代码对 `Simulator` 类的初始化，下达运行源代码的指令。

### `status.h` & `status.cpp`

调用 `output_bmp.h` 。

定义 `Status` 类，用来记录虚拟的“电脑”的寄存器、内存、栈的数值和使用状态，和输出可视化的结果。

### `program.h` & `program.cpp`

无调用。

定义 `Num` 类，用作多用途的“数“（既可以表示立即数，也可以表示某个寄存器中的数）。

定义 `Line` 类，用来存储 ERISC 程序中的某行，记录该行的类型、0～3 个参数。

### `simulator.h` & `simulator.cpp`

调用 `program.h` 与 `status.h`。

定义 `Simulator` 类，用来模拟程序的运行；内含一个 `Status` 对象，用来存储“虚拟机”的物理情况；内含若干 `Line` 对象，用来存储源代码。类中包含 `parse` 方法，用来将源代码结构化并存储到 `Line` 对象中；类中包含 `execute` 方法和 `do_line` 方法，用来执行代码，修改虚拟机中的物理情况。

### `output_bmp.h` & `output_bmp.cpp`

无调用。

定义 `BMP` 类，用于接收 `status` 类传递的空间使用状态，并输出可视化的BMP。

## 任务完成情况

### 必做任务

#### 命令模拟

全部完成。

| 命令名称      | 程序代码                                |
| ------------- | --------------------------------------- |
| 内存数据加载  | `load [rd],[rs]`                        |
| 内存数据存入  | `store [rs],[rd]`                       |
| 入栈          | `push [rs]`                             |
| 出栈          | `pop [rd]`                              |
| 寄存器赋值    | `mov [rd],[rs/imm]`                     |
| 寄存器加/减   | `add/sub [rd],[rs1],[rs2/imm]`          |
| 寄存器乘/除   | `mul/div [rd],[rs1],[rs2/imm]`          |
| 寄存器取余    | `rem [rd],[rs1],[rs2/imm]`              |
| 寄存器位与/或 | `and/or [rd],[rs1],[rs2/imm]`           |
| 行标识        | `[line_id]:`                            |
| 无条件跳转    | `jal [line_id]`                         |
| 条件跳转      | `beq/bne/blt/bge [rs1],[rs2],[line_id]` |

#### 结果可视化（bmp）输出

完成，结果大致如下图：

<img src="1.jpg" style="zoom: 400%;" />

#### 结果文本（txt）输出

完成。见对应输入文件的 `result.txt` 。

#### 必做程序编写、运行情况

必做任务1：成功运行，输出结果见 `./sample_output/test_1` 。

必做任务2：成功运行，输出结果见 `./sample_output/test_2` 。

必做任务3：成功编写并运行。代码输出结果见 `./sample_output/test_3` 。

必做任务4：成功编写 $O(n \log n)$ 的素数筛并运行，共计运行约 29336424 行命令，用时约 2.72536s（Dual-Core Intel Core M, 1.1GHz）得到最后的答案。代码见 `./input/test_4.risc` ，输出结果见 `./sample_output/test_4` 。

### 选做任务

#### 读写（io）命令增加

加入屏幕读写命令。

| 命令名称           | 程序代码     | 具体说明                                                    |
| ------------------ | ------------ | ----------------------------------------------------------- |
| 从屏幕读取到寄存器 | `read [rd]`  | 从屏幕(`cin`)读入一个字节，存在 `rd` 的最低 `8` 位。        |
| 从寄存器输出到屏幕 | `write [rs]` | 将 `rs` 的最低 `8` 位作为一个 `char` 输出到从屏幕(`cout`)。 |

#### 调试模式

> 这汇编，也太难调了吧！（来自写了2个多小时汇编素数筛，最后发现寄存器名字敲串了的同学）

于是，加入了 `debug` 模式。在 `debug` 模式中，程序输出更多信息至屏幕，并且可以在运行过程中指定的一些行暂停程序，进行变量的查看。具体使用方法见下文。

#### 空行、注释支持

空行会被忽略，但计入行数。

每一行中第一个 `/` 字符之后的所有字符均会被忽略，因此可以使用 `//some comments` 的类似 C++ 的方式来编写注释。调试和编码都因此方便多了！

#### 简单的代码合法性检查

1. 对未出现的行标识、重复出现的行标识符号进行了判断并抛出异常，防止出现一些不可预料的行为。
2. 对未被定义的命令进行了判断并抛出异常，帮助编码者在自己拼错命令的时候快速 debug。
3. 对可能出现的寄存器名称不存在的命令进行了判断并抛出异常，防止程序解析错了我们也不知道。

### 关于外部代码使用

本项目没有使用其他语言的任何代码。

本项目使用了一系列 C++ 标准库，包括 `cstring` ，`iostream`，`cstdio`  ，`ctime` ，`fstream` ，`iomanip` ，`sstream` ，`string` ，`unordered_map` 。

## 使用说明

### 关于项目编译

本项目配上了一个 `makefile` 文件，里面使用了 `findall` 命令，默认使用的编译器命令是 `g++-10` ，如需编译项目，可根据具体情况使用 `makefile` 文件。

`makefile` 编译出来的可执行文件放在 `./bin/main` 处。

或直接编译 `main.cpp` 亦可。

### 关于 `main.cpp` 的主函数使用

包括可执行文件本身，`main.cpp` 接收三至四个参数：

1. 第一个参数是可执行文件本身
2. 第二个参数是执行任务类型【只有第一个字母有意义】
   1. `e(xecute)` 代表模拟运行 ERISC 程序。
   2. `d(ebug)` 代表以调试模式运行 ERISC 程序。
3. 第三个参数给出输入文件路径。
4. 【可选】第四个参数给出输出文件存储路径。（如果不给出将在输入文件的同级文件夹下新建以该文件除去后缀名并加上 `_out` 为名的文件夹）

例如：

`$ ./bin/main execute input/test_1.risc` 将会运行第一个示例代码，输出在 `./test_1_out` 文件夹下 。

`$ ./bin/main execute input/test_1.risc sample_output_1` 将会运行第一个示例代码，输出在 `./sample_output_1` 文件夹下 。

`$ ./bin/main debug input/test_2.risc` 将会以**调试模式**运行第二个示例代码，输出在 `./test_2_out` 文件夹下 。

需要注意：

+ `main.cpp` 中使用 `mkdir` 和 `rm` 的命令行命令来创建输出文件存储路径，需要保证命令行有上述这两个命令。

### 关于 `debug` 模式的使用

`debug` 模式下，程序将会提供更为详尽的处理过程输出，并提供在程序运行过程中暂停以查看变量等功能。

如何进入 `debug` 模式请参见上文。

#### 如何设置断点

在某行最后放置一个 `*` 字符，后面**直接**连接  `\n` 或 `/ ...`。（注意，断点只有在 `debug` 模式下才能生效！）

#### 如何在断点处做一些事情

程序会在设置的断点处暂停，此时可以进行如下的操作：

| 操作       | 命令                | 说明                 |
| ---------- | ------------------- | -------------------- |
| 查看帮助   | `?`                 | 无                   |
| 退出查看 | `!`                 | 无                   |
| 查看寄存器 | `fp`/`zero`/`x31`   | 直接输入寄存器名即可 |
| 查看内存   | `0x3f 123`/`100000 1` | 第一个参数可以是16进制数，也可以是10进制数<br/>第二个参数代表字节数 |
| 查看栈 | `stack 1`/`stack 123` | 第一个参数必须是 `stack` <br/>第二个参数是从顶向下要查看的元素个数 |
| 增加断点 | `add x` | 在第 $x$ 行处增加断点（注意是以 0 为开始行） |
| 去除断点 | `del x` | 在第 $x$ 行处去掉断点（注意是以 0 为开始行） |
| 结束调试 | `disable` | 关闭 `debug` 模式，结束这次断点后将不会再进入任何断点。 |

### 其他注意事项

1. ERISC 源代码最大 10000 行，如果超出将只阅读前 10000 行。
2. ERISC 源代码只支持 ASCII 码。
3. ERISC 源代码每行不能超过 100 个字符。

## 具体实现解读

### 数据管理

#### “数”：Num 类

`Num` 类主要用来表示每行命令中的参数。

`Num` 类中包含 `bool type` 和 `unsigned int val` 两个变量；如果 `type` 是 1 代表这就是 “数”，否则代表这是一个寄存器的编号。

#### “行”：Line 类

`Line` 类主要用来表示每行的命令及参数。

`Line` 类中包含 `short type` ，表示命令类型；包含 `Num args[3]` ，表示可能出现的三个参数。

用宏定义的方法将命令类型映射到 `short`范围内的一个整数。

#### “计算机的状态”：Status 类

`Status` 类主要用来表示计算机的状态，包括寄存器，内存、栈的数值和使用情况。

 `Status` 类中包含数组 `unsigned int x[REGISTER_NUM]; unsigned char    memory[MEMORY_SIZE]; unsigned char stack[STACK_SIZE];`，分别用来表示寄存器、内存、栈的存储情况；包含结构 `Output_status` ，用来记录寄存器、内存、栈的使用情况。

 `Status` 类中包含 `void load(unsigned int &rd, unsigned int ptr)` 等方法来执行“命令”，其中需要修改的寄存器传参时使用引用，只需要值的寄存器传参时使用值；为此定义了 `unsigned int  get_reg_val(unsigned short pos);unsigned int &get_reg_ref(unsigned short pos);` 方法来获取寄存器的值和引用；还包含一系列修改 `output_status` 的方法，在执行命令时同步运行；还包含一系列输出函数，将寄存器、内存、栈的内容输出。

### 程序模拟运行

#### 封装：Simulator 类

`Simulator` 类主要用来模拟程序运行。程序运行主要分为两个阶段： `parse` 阶段中，将

#### 解释：parse 阶段

主要由两个函数组成：

+ `void parse_file(const char *FILENAME)` ，进行总体的解释，调用下面的函数。
  + 这里最难的是处理 `line_symbol` 出现在其首次调用之后出现的问题。我们建了一个数组 `unfounded_line` 来储存这样的调用行，在所有行均处理完之后再对这些行进行处理。
+ `void parse(const char *script, Line & line, const int current_line, bool unfounded)`，进行每一行具体的解释，存储到 `line` 变量中。
  + 其中使用 `unordered_map` 来解析名称至数字，通过 `get_arg, add_arg` 函数来从文本中读取并添加参数至 `Line` 中。

#### 运行：execute 阶段

主要由两个函数组成：

+ `void execute(const char *OUTPUT_PATH, unsigned int stop_line = -1)` 整体运行程序，调用下面的 `do_line` 函数。
+ `int do_line(unsigned int &now_line, Line line, const char *output_path)` ，运行具体的某一行，并修改 `now_line` 。

#### 调试：debug 模式

`Simulator` 结构体内有 `debug_mode` 变量，代表调试等级，默认为 0 ，开启 `debug` 模式后为 1。
主要在 `void debug_watch();` 模式中进行。

用户输入的命令经过条件判断后得到反馈。

## 一些不宜直接上传于 GitHub的信息

pdf版文档：https://cloud.tsinghua.edu.cn/f/4531f974c4654bf296c2/?dl=1

演示视频：https://cloud.tsinghua.edu.cn/f/43adc88c607e4762aa09/?dl=1

失效后如需要，请索取。
