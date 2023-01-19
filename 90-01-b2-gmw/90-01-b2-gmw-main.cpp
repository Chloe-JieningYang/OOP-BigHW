/* 2050598 杨洁宁 通信 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <stdio.h>  
#include <stdlib.h>  
#include <conio.h>  
#include <math.h>
#include <time.h>
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
#include "90-01-b2-gmw.h"
using namespace std;

void InputRowCol(int& c, int& r)//输入总行总列
{
	while (1)
	{
		cout << "请输入行数(5-8):" << endl;
		cin >> r;
		if (cin.good() && r >= 5 && r <= 8)
			break;
	}
	while (1)
	{
		cout << "请输入列数(5-10):" << endl;
		cin >> c;
		if (cin.good() && c >= 5 && c <= 10)
			break;
	}
}

void InputAim(int& a)
{
	while (1)
	{
		cout << "请输入合成目标(5-20):" << endl;
		cin >> a;
		if (cin.good() && a >= 5 && a <= 20)
			break;
	}
}

int main()
{
	srand((unsigned)time(NULL));
	CONSOLE_GRAPHICS_INFO Mto10_CGI;
	MATRIX_SIGN mat;
	int col_num, row_num, aim;
	while (1)
	{
		int score = 0;
		bool quit = 0;
		for (int p = 0; p <= 9; p++)
		{
			for (int q = 0; q <= 11; q++) {
				mat.matrix[p][q] = 0;
				mat.sign[p][q] = 0;
			}
		}//初始化mat数组 置零

		gmw_init(&Mto10_CGI);//初始化整个界面设置

		InputRowCol(col_num, row_num);//输入行列
		InputAim(aim);//输入目标

		gmw_set_rowcol(&Mto10_CGI, row_num, col_num);//设置行列
		gmw_set_frame_style(&Mto10_CGI, 6, 3, false);//设置框架
		gmw_set_block_border_switch(&Mto10_CGI, true);//色块边框
		gmw_set_block_default_linetype(&Mto10_CGI, 2);//色块边框线型
		gmw_set_colno_switch(&Mto10_CGI, true);//列号开
		gmw_set_rowno_switch(&Mto10_CGI, true);//行号开
		gmw_set_font(&Mto10_CGI, "新宋体", 20);//设置字体
		gmw_set_frame_color(&Mto10_CGI, COLOR_HWHITE, COLOR_BLACK);//设置游戏区颜色

		CreatMatrix(Mto10_CGI.row_num, Mto10_CGI.col_num, mat.matrix);

		gmw_draw_frame(&Mto10_CGI);//画游戏背景框架

		char temp[256];
		/* 上状态栏显示内容 */
		sprintf(temp, "屏幕当前设置为：%d行 %d列", Mto10_CGI.lines, Mto10_CGI.cols);
		gmw_status_line(&Mto10_CGI, TOP_STATUS_LINE, temp);

		CompleteGame(&Mto10_CGI, &mat, score, aim, quit);
		
		if (quit == 1)
			break;
	}

	return 0;
}