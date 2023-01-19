#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <stdio.h>  
#include <stdlib.h>  
#include <conio.h>  
#include <time.h>
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
#include "90-02-b2.h"
using namespace std;

void InputRowCol(int& c, int& r)//输入总行总列
{
	cout << "请输入行数(4-8):";
	while (1)
	{
		int x, y;
		cct_getxy(x, y);
		cin >> r;
		if (cin.good() && r >= 4 && r <= 8)
			break;
		else
		{
			cct_showstr(x, y, " ", COLOR_HWHITE, COLOR_BLACK, 10);
			cct_gotoxy(x, y);
		}
	}
	int x1, y1;
	cct_getxy(x1, y1);
	cct_gotoxy(3, y1 + 1);
	cout << "请输入列数(4-10):";
	while (1)
	{
		int x, y;
		cct_getxy(x, y);
		cin >> c;
		if (cin.good() && c >= 4 && c <= 10)
			break;
		else
		{
			cct_showstr(x, y, " ", COLOR_HWHITE, COLOR_BLACK, 10);
			cct_gotoxy(x, y);
		}
	}
}


void InputAim(int& a, const int row, const int col)
{
	int x1, y1;
	cct_getxy(x1, y1);
	cct_gotoxy(3, y1 + 1);
	int minaim;
	if (row + col == 8)
		minaim = 512;
	else if (row + col <= 10)
		minaim = 1024;
	else if (row + col <= 14)
		minaim = 2048;
	else if (row + col <= 16)
		minaim = 4096;
	else
		minaim = 8192;
	char temp[256] = { '\0' };
	sprintf(temp, "请输入合成目标[%d/%d/%d/%d]:", minaim, 2 * minaim, 4 * minaim, 8 * minaim);
	cout << temp;
	while (1)
	{
		int x, y;
		cct_getxy(x, y);
		cin >> a;
		if (cin.good() && (a == minaim || a == 2 * minaim || a == 4 * minaim || a == 8 * minaim))
			break;
		else
		{
			cct_showstr(x, y, " ", COLOR_HWHITE, COLOR_BLACK, 10);
			cct_gotoxy(x, y);
		}
	}
}


void InputDelayTime(int& time)
{
	int x1, y1;
	cct_getxy(x1, y1);
	cct_gotoxy(3, y1 + 1);
	cout << "请输入游戏延时[0-5]:";
	while (1)
	{
		int x, y;
		cct_getxy(x, y);
		cin >> time;
		if (cin.good() && (time >= 0 && time <= 5))
			break;
		else
		{
			cct_showstr(x, y, " ", COLOR_HWHITE, COLOR_BLACK, 10);
			cct_gotoxy(x, y);
		}
	}
}

void BeginPage(int& col, int& row, int& aim, int& delaytime)
{
	CONSOLE_GRAPHICS_INFO begin_CGI;

	gmw_init(&begin_CGI);

	gmw_set_status_line_switch(&begin_CGI, TOP_STATUS_LINE, false);
	gmw_set_status_line_switch(&begin_CGI, LOWER_STATUS_LINE, false);
	gmw_set_frame_style(&begin_CGI, 6, 3, false);
	gmw_set_frame_color(&begin_CGI, COLOR_HWHITE, COLOR_BLACK);

	gmw_draw_frame(&begin_CGI);
	cct_gotoxy(3, 2);
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);

	InputRowCol(col, row);
	
	InputAim(aim, row, col);
	
	InputDelayTime(delaytime);
}

int main()
{
	bool quit = 0;
	bool restart = 0;
	int aim;
	int highest = 0;
	srand((unsigned)time(NULL));
	CONSOLE_GRAPHICS_INFO g2048_CGI;
	while (1)
	{
		restart = 0;
		int score = 0;
		int row, col, delaytime;
		int counttime = 0;
		gmw_init(&g2048_CGI);
		BeginPage(col, row, aim, delaytime);

		CompleteGame(g2048_CGI, score, aim, highest, col, row, delaytime, quit, restart);

		if (quit == 1)
			break;
		if (restart == 1)
			continue;
	}
	
	return 0;
}