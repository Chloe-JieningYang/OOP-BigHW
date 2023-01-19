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

//批量移动方块
void BlockMove(int g2048[][10], const CONSOLE_GRAPHICS_INFO CGI, const BLOCK_DISPLAY_INFO *bdi_normal,const int type,bool& no_use)
{
	const int row = CGI.row_num;
	const int col = CGI.col_num;

	int copyg2048[8][10];//复制游戏数组
	int mark[8][10];//标记合成后下面有多少0的数组
	for (int i = 0; i <= 7; i++)
	{
		for (int j = 0; j <= 9; j++)
		{
			copyg2048[i][j] = g2048[i][j];
			mark[i][j] = 0;
		}
	}
	int fakescore = 0;//提前合成的假分数

	RemoveZero(copyg2048, CGI, type);//提前演练去0
	CombineSame(copyg2048, mark, CGI, type, fakescore);//填充mark数组，mark数组用来记合成后还要移几个0
	RemoveZero(copyg2048, CGI, type);

	if (JudgeSame(g2048, copyg2048) == true)
	{
		no_use = true;
		return;
	}

	if (type == UP_TO_DOWN)
	{
		for (int j = 0; j <= col - 1; j++)
		{
			int count0 = 0;
			for (int i = row - 1; i >= 0; i--)
			{
				if (g2048[i][j] == 0)
					count0++;
				else if (count0 + mark[i + count0][j] != 0)
				{
					int totalmove = count0 + mark[i + count0][j];
					gmw_move_block(&CGI, i, j, g2048[i][j], 0, bdi_normal, UP_TO_DOWN, totalmove);
					if (FindSameAhead(g2048, i, j, row, col, type) == false)
						gmw_draw_block(&CGI, i + totalmove, j, copyg2048[i + totalmove][j], bdi_normal);
				}
			}
		}
	}
	else if (type == DOWN_TO_UP)
	{
		for (int j = 0; j <= col - 1; j++)
		{
			int count0 = 0;
			for (int i = 0; i <= row - 1; i++)
			{
				if (g2048[i][j] == 0)
					count0++;
				else if (count0 + mark[i - count0][j] != 0)
				{
					int totalmove = count0 + mark[i - count0][j];
					gmw_move_block(&CGI, i, j, g2048[i][j], 0, bdi_normal, DOWN_TO_UP, totalmove);
					if (FindSameAhead(g2048, i, j, row, col, type) == false)
						gmw_draw_block(&CGI, i - totalmove, j, copyg2048[i - totalmove][j], bdi_normal);
				}
			}
		}
	}
	else if (type == RIGHT_TO_LEFT)
	{
		for (int i = 0; i <= row - 1; i++)
		{
			int count0 = 0;
			for (int j = 0; j <= col - 1; j++)
			{
				if (g2048[i][j] == 0)
					count0++;
				else if (count0 + mark[i][j - count0] != 0)
				{
					int totalmove = count0 + mark[i][j - count0];
					gmw_move_block(&CGI, i, j, g2048[i][j], 0, bdi_normal, RIGHT_TO_LEFT, totalmove);
					if (FindSameAhead(g2048, i, j, row, col, type) == false)
						gmw_draw_block(&CGI, i, j - totalmove, copyg2048[i][j - totalmove], bdi_normal);
				}
			}
		}
	}
	else if (type == LEFT_TO_RIGHT)
	{
		for (int i = 0; i <= row - 1; i++)
		{
			int count0 = 0;
			for (int j = col - 1; j >= 0; j--)
			{
				if (g2048[i][j] == 0)
					count0++;
				else if (count0 + mark[i][j + count0] != 0)
				{
					int totalmove= count0 + mark[i][j + count0];
					gmw_move_block(&CGI, i, j, g2048[i][j], 0, bdi_normal, LEFT_TO_RIGHT, totalmove);
					if (FindSameAhead(g2048, i, j, row, col, type) == false)
						gmw_draw_block(&CGI, i, j + totalmove, copyg2048[i][j + totalmove], bdi_normal);
				}
			}
		}
	}
	
	return;
}


void OneMove(int g2048[][10], const CONSOLE_GRAPHICS_INFO CGI, const BLOCK_DISPLAY_INFO *bdi_normal, int& score,bool &quit,bool &restart)
{
	int mrow, mcol, mac, k1, k2, direction;
	while (1)//移动有效或游戏失败才结束
	{
		if (JudgeEnd(g2048, CGI) == true)
			return;
		while (1)//读到方向键合qr键才结束
		{
			int event = gmw_read_keyboard_and_mouse(&CGI, mac, mrow, mcol, k1, k2, false);
			if (event == CCT_KEYBOARD_EVENT)
			{
				if (k1 == 0xe0 && k2 == KB_ARROW_UP)
				{
					direction = DOWN_TO_UP;
					break;
				}
				else if (k1 == 0xe0 && k2 == KB_ARROW_DOWN)
				{
					direction = UP_TO_DOWN;
					break;
				}
				else if (k1 == 0xe0 && k2 == KB_ARROW_LEFT)
				{
					direction = RIGHT_TO_LEFT;
					break;
				}
				else if (k1 == 0xe0 && k2 == KB_ARROW_RIGHT)
				{
					direction = LEFT_TO_RIGHT;
					break;
				}
				else if (k1 == 'Q' || k1 == 'q')
				{
					quit = 1;
					return;
				}
				else if (k1 == 'R' || k1 == 'r')
				{
					restart = 1;
					return;
				}
			}
		}

		bool no_use = 0;//判断此次移动有没有效果，比如方块全在最下面还在按下移
		BlockMove(g2048, CGI, bdi_normal, direction, no_use);//图形移动
		if (!no_use)
			break;
	}
	int mark_no_use[8][10];
	for (int i = 0; i <= 7; i++)
	{
		for (int j = 0; j <= 9; j++)
			mark_no_use[i][j] = 0;
	}
	//数组变化
	RemoveZero(g2048, CGI, direction);
	CombineSame(g2048, mark_no_use, CGI, direction, score);
	RemoveZero(g2048, CGI, direction);
	
	return;
}


void CompleteGame(CONSOLE_GRAPHICS_INFO CGI, int& score, int aim, int& highest, const int col, const int row, const int delaytime, bool& quit, bool& restart)
{
	const BLOCK_DISPLAY_INFO bdi_normal[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0不显示，用空格填充即可
		{2, COLOR_HWHITE, COLOR_BLACK, NULL},
		{4, COLOR_HRED, COLOR_BLACK, NULL},
		{8, COLOR_GREEN, COLOR_BLACK, NULL},
		{16, COLOR_RED, COLOR_BLACK, NULL},
		{32, COLOR_BLUE, COLOR_BLACK, NULL},
		{64, COLOR_HYELLOW, COLOR_BLACK, NULL},
		{128, COLOR_CYAN, COLOR_BLACK, NULL},
		{256, COLOR_PINK, COLOR_BLACK, NULL},
		{512, COLOR_HBLACK, COLOR_BLACK, NULL},
		{1024, COLOR_HPINK, COLOR_BLACK, NULL},
		{2048, COLOR_WHITE, COLOR_BLACK, NULL},
		{4096, COLOR_YELLOW, COLOR_BLACK, NULL},
		{8192, COLOR_HBLUE, COLOR_BLACK, NULL},
		{16384,COLOR_GREEN, COLOR_BLACK, NULL},
		{32768, COLOR_HCYAN, COLOR_BLACK, NULL},
		{65536, COLOR_HGREEN, COLOR_BLACK, NULL},
		{131072, COLOR_HPINK, COLOR_BLACK, NULL}, 
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
	};

	int g2048[8][10];
	
	char temp[256];

	gmw_set_font(&CGI, "新宋体", 16, 0);
	gmw_set_block_border_switch(&CGI, true);
	gmw_set_block_default_linetype(&CGI, 1);
	gmw_set_delay(&CGI, DELAY_OF_BLOCK_MOVED, delaytime);
	gmw_set_status_line_color(&CGI, TOP_STATUS_LINE, COLOR_HWHITE, COLOR_BLACK);
	gmw_set_frame_color(&CGI, COLOR_HWHITE, COLOR_BLACK);
	gmw_set_frame_default_linetype(&CGI, 2);
	gmw_set_frame_style(&CGI, 12, 5);
	gmw_set_rowcol(&CGI, row, col);

	CreateMatrix(g2048, CGI);

	clock_t start, end;

	gmw_draw_frame(&CGI);
	//上显示栏
	sprintf(temp, "目标:%d 分数:%d 最高:%d 时间：%d (R:重玩 Q退出)", aim, score, highest, 0);
	gmw_status_line(&CGI, TOP_STATUS_LINE, temp);
	start = clock();

	for (int i = 0; i <= row - 1; i++)
	{
		for (int j = 0; j <= col - 1; j++)
			if (g2048[i][j] != 0)
				gmw_draw_block(&CGI, i, j, g2048[i][j], bdi_normal);
	}


	while (1)
	{

		OneMove(g2048, CGI, bdi_normal, score, quit, restart);
		
		
		if (JudgeEnd(g2048, CGI) == true)//如果游戏失败
		{
			gmw_status_line(&CGI, LOWER_STATUS_LINE, "按R重玩,按Q退出", "游戏失败");
			while (1)
			{
				int mx2, my2, ma2, kc12, kc22;
				int event2 = cct_read_keyboard_and_mouse(mx2, my2, ma2, kc12, kc22);
				if (kc12 == 'Q' || kc12 == 'q')
				{
					quit = 1;
					break;
				}
				else if (kc12 == 'R' || kc12 == 'r')
				{
					restart = 1; 
					break;
				}
			}
			break;
		}
		else if (SearchForMax(g2048) == aim)
		{
			gmw_status_line(&CGI, LOWER_STATUS_LINE, "按R重玩,按Q退出", "目标达成");
			while (1)
			{
				int mx2, my2, ma2, kc12, kc22;
				int event2 = cct_read_keyboard_and_mouse(mx2, my2, ma2, kc12, kc22);
				if (kc12 == 'Q' || kc12 == 'q')
				{
					quit = 1;
					break;
				}
				else if (kc12 == 'R' || kc12 == 'r')
				{
					restart = 1;
					break;
				}
			}
			break;
		}
		if (quit || restart)
			return;
		int i0, j0;
		AddNewNumber(CGI, g2048, i0, j0);
		if (highest < score)
			highest = score;
		gmw_draw_block(&CGI, i0, j0, g2048[i0][j0], bdi_normal);
		end = clock();
		int counttime = (int)(end - start) / CLOCKS_PER_SEC;
		sprintf(temp, "目标:%d 分数:%d 最高:%d 时间：%d (R:重玩 Q退出)", aim, score, highest, counttime);
		gmw_status_line(&CGI, TOP_STATUS_LINE, temp);
	}

	return;
}