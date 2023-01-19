#pragma once
/* 通信 2050598 杨洁宁 */

#define UPVACROW 3
#define LEFTVACCOL 4

void OutputMatrixArray(int i, int j, int m[][12], int s[][12], bool sign);//用数组形式显示

//画一个格子的函数，style=1为实线，style为0为虚线
void DrawOneBlock(int length, int height, int backcolor, int frontcolor, int style, const char content[]);

//数字转换为字符
void IntToChar(int number, char content[]);


//画外框架的函数
void DrawOutLine(int length, int height, int j, int i, int style, bool splitline);


//画外框架加上内部数组，可以选择有无框线，虚线实线，打印色块的内容
void DrawCompleteMatrix(int length, int height, int style, const char game[], int j, int i, bool splitline, int m[][12], const char content[]);
