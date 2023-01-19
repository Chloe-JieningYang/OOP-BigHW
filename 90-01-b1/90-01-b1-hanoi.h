/* 通信工程 2050598 杨洁宁 */
#pragma once

/* ------------------------------------------------------------------------------------------------------

     本文件功能：
	1、为了保证 hanoi_main.cpp/hanoi_menu.cpp/hanoi_multiple_solutions.cpp 能相互访问函数的函数声明
	2、一个以上的cpp中用到的宏定义（#define）或全局只读（const）变量，个数不限
	3、可以参考 cmd_console_tools.h 的写法（认真阅读并体会）
   ------------------------------------------------------------------------------------------------------ */

#define x_1 1 //第一个柱子底座最左坐标
#define x_2 33
#define x_3 65

//有关输入和初始化的函数
//char menu();
void input(int* num, char* i, char* a, char* m, char choice);
void init_store(int n, char init);
void initialize(int n, char src, char dst, char choice);

//有关画图形的函数
void draw_three_pole();
void draw_six(int n, char src);
void draw_seven(int n, char src, char dst);
void nine(int nn,char aim);
void hanoi(int n, char src, char tmp, char dst, char choice);

//结束一次游戏函数
void end_(char choice);

