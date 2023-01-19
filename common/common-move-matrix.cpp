/* 2050598 杨洁宁 通信 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "..\include\cmd_console_tools.h"
#include "..\include\common-print-matrix.h"
#include <iomanip>
#include <cstring>
#include <Windows.h>
#include <ctime>
#include <conio.h>
#include <cstdio>
#include <stdio.h>

using namespace std;

/***************************************************************************
  函数名称：
  功    能：做数组中的移动
  输入参数：行，列，数组，标记数组，游戏名
  返 回 值：
  说    明：
***************************************************************************/
void RemoveZero(int i, int j, int m[][12], int s[][12],const char game[])//数组中的变化
{
	int j1, i1;
	for (j1 = 1; j1 <= j; j1++)
	{
		for (i1 = 2; i1 <= i; i1++)
		{
			if (m[i1][j1] == 0)
			{
				int i2;
				for (i2 = i1; i2 >= 2; i2--)//下落填充0
				{
					m[i2][j1] = m[i2 - 1][j1];
				}
				m[1][j1] = 0;
			}
		}
	}
	for (i1 = 0; i1 <= i; i1++)//将标记矩阵置零
	{
		for (j1 = 0; j1 <= 11; j1++)
			s[i1][j1] = 0;
	}
	for (i1 = 1; i1 <= i; i1++)//标记矩阵中0的位置
	{
		for (j1 = 1; j1 <= j; j1++)
		{
			if (m[i1][j1] == 0)
				s[i1][j1] = 1;
		}
	}
}

/***************************************************************************
  函数名称：
  功    能：做数组中的全0列的移动
  输入参数：行，列，数组，标记数组
  返 回 值：
  说    明：
***************************************************************************/
void ZeroColMoveLeft(int i, int j, int m[][12], int s[][12]) {
	int leftzerocol[12];
	int signzerocol[12];
	for (int p = 0; p <= 11; p++)
	{
		leftzerocol[p] = 0;
		signzerocol[p] = 0;
	}
	for (int j1 = 1; j1 <= j; j1++)//将全0列标记为1
	{
		bool allzero = 1;
		for (int i1 = 1; i1 <= i; i1++)
		{
			if (m[i1][j1] != 0)
			{
				allzero = 0;
				break;
			}
		}
		if (allzero)
			signzerocol[j1] = 1;//全0列标记为1
	}
	for (int j1 = 2; j1 <= j; j1++)//一列一列看左侧一共多少全0列
	{
		if (signzerocol[j1] != 1)
		{
			for (int j2 = 1; j2 < j1; j2++)
			{
				leftzerocol[j1] += signzerocol[j2];
			}
		}
	}
	for (int j2 = 2; j2 <= j; j2++)
	{
		if (signzerocol[j2] == 1)
			continue;
		else if (leftzerocol[j2] == 0)
			continue;
		else
		{
			for (int i1 = 1; i1 <= i; i1++)
			{
				m[i1][j2 - leftzerocol[j2]] = m[i1][j2];
				m[i1][j2] = 0;
			}
		}
	}
	for (int i1 = 0; i1 <= i; i1++)//将标记矩阵置零
	{
		for (int j1 = 0; j1 <= j; j1++)
			s[i1][j1] = 0;
	}
	for (int i1 = 1; i1 <= i; i1++)//标记矩阵中0的位置
	{
		for (int j1 = 1; j1 <= j; j1++)
		{
			if (m[i1][j1] == 0)
				s[i1][j1] = 1;
		}
	}
}

/***************************************************************************
  函数名称：
  功    能：先做伪图形的移动，最后做数组中的移动
  输入参数：数组，标记数组，行，列，里面方块的长，高，有无间隔线，游戏名（mkten/erasestar）
  返 回 值：
  说    明：
***************************************************************************/
void BlockFallDown2(int m[][12], int s[][12], int i, int j, int length, int height,bool splitline, const char game[])
{
	int i1, j1;
	for (j1 = 1; j1 <= j; j1++)
	{
		int count0 = 0;
		for (i1 = i; i1 >= 1; i1--)
		{
			if (m[i1][j1] == 0)
			{
				count0++;
				continue;
			}
			else
			{
				if (count0 != 0)//如果不是第一排为0
				{
					if (splitline)
						cct_gotoxy(4 + (length + 2) * (j1 - 1), 3 + (height + 1) * (i1 - 1));
					else
						cct_gotoxy(4 + length * (j1 - 1), 3 + height * (i1 - 1));//到方块左上角
					for (int t = 1; t <= count0; t++)
					{
						int x, y;
						cct_getxy(x, y);
						int y1;
						if (splitline)//如果有分隔线
						{
							for (y1 = y; y1 <= y + height+1; y1++)
							{
								int x2, y2;
								cct_gotoxy(4 + (length + 2) * (j1 - 1), y1);
								cct_getxy(x2, y2);
								if (!strcmp(game, "mkten"))
								{
									char content[] = { char(m[i1][j1] + '0'),'\0' };
									DrawOneBlock(length, height, m[i1][j1], COLOR_BLACK, 1, content);
								}
								else if (!strcmp(game, "erasestar"))
									DrawOneBlock(length, height, m[i1][j1], COLOR_BLACK, 0, "★");
								Sleep(1);
								if (y1 < y + height) {									
									cct_gotoxy(x, y2);
									if (!strcmp(game, "mkten"))
										DrawOneBlock(length, height, COLOR_HWHITE, COLOR_HWHITE, -1, " ");
									else if(!strcmp(game,"erasestar"))
										DrawOneBlock(length, height, COLOR_HWHITE, COLOR_HWHITE, -1, " ");
								}
							}
						}
						else//没有分隔线
						{
							for (y1 = y; y1 <= y + height; y1++)
							{
								int x2, y2;
								cct_gotoxy(4 + length * (j1 - 1), y1);//方块左上角定位
								cct_getxy(x2, y2);
								if (!strcmp(game, "mkten"))
								{
									char content[] = { char(m[i1][j1] + '0'),'\0' };
									DrawOneBlock(length, height, m[i1][j1], COLOR_BLACK, 1, content);
								}
								else if (!strcmp(game, "erasestar"))
									DrawOneBlock(length, height, m[i1][j1], COLOR_BLACK, 0, "★");
								Sleep(1);
								if (y1 < y + height)
								{
									cct_gotoxy(x, y2);
									DrawOneBlock(length, height, COLOR_HWHITE, COLOR_HWHITE, -1, " ");									
								}							
							}
						}
						if (splitline)
						{
							cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
							cct_getxy(x, y);
							cct_gotoxy(x - length, y - height);
							for (int n = 1; n <= length / 2; n++)
							{
								if (!strcmp(game, "mkten"))
									cout << "━";
								else if (!strcmp(game, "erasestar"))
									cout << "═";
							}
							cct_getxy(x, y);
							cct_gotoxy(x - length, y + 1);
						}
						else
						{
							cct_getxy(x, y);//方块右下角
							cct_gotoxy(x - length, y - (height - 1));
						}
					}
				}
			}
		}

	}
	RemoveZero(i, j, m, s, game);
}

/***************************************************************************
  函数名称：
  功    能：先做伪图形的移动，最后做数组中的移动
  输入参数：数组，标记数组，行，列，里面方块的长，高，有无间隔线，线型
  返 回 值：
  说    明：
***************************************************************************/
void ColBlockMoveLeft(int m[][12], int s[][12], int i, int j, int length, int height, bool splitline,int style)
{
	int leftzerocol[12];
	int signzerocol[12];
	for (int p = 0; p <= 11; p++)
	{
		leftzerocol[p] = 0;
		signzerocol[p] = 0;
	}
	for (int j1 = 1; j1 <= j; j1++)//将全0列标记为1
	{
		bool allzero = 1;
		for (int i1 = 1; i1 <= i; i1++)
		{
			if (m[i1][j1] != 0)
			{
				allzero = 0;
				break;
			}
		}
		if (allzero)
			signzerocol[j1] = 1;//全0列标记为1
	}
	for (int j1 = 2; j1 <= j; j1++)//一列一列看左侧一共多少全0列
	{	
		if (signzerocol[j1] != 1)
		{
			for (int j2 = 1; j2 < j1; j2++)
			{
				leftzerocol[j1] += signzerocol[j2];
			}
		}
	}
	for (int j3 = 2; j3 <= j; j3++)//按列移动
	{
		if (signzerocol[j3] == 1)
			continue;
		else if (leftzerocol[j3] == 0)
			continue;
		else
		{
			int ii = 1;
			for (ii = 1; ii <= i; ii++)//找该列第一个不为0的数
			{
				if (m[ii][j3] != 0)
					break;
			}
			if (splitline)
				cct_gotoxy(LEFTVACCOL + (length + 2) * (j3 - 1), UPVACROW + (height + 1) * (ii - 1));
			else
				cct_gotoxy(LEFTVACCOL + length * (j3 - 1), UPVACROW + height * (ii - 1));
			int x, y;
			
			for (int i3 = ii; i3 <= i; i3++)
			{				
				cct_getxy(x, y);//获取这一列的x和当前方块左上角的y
				for (int t = 1; t <= leftzerocol[j3]; t++)
				{
					int xx, yy;
					cct_getxy(xx, yy);
					int x1;
					if (splitline)
					{
						for (x1 = xx; x1 >= xx - (length + 2); x1--)
						{
							cct_gotoxy(x1, y);
							int x0, y0;
							cct_getxy(x0, y0);
							DrawOneBlock(length, height, m[i3][j3], COLOR_BLACK, style, "★");
							Sleep(1);
							if (x1 > xx - (length + 2))
							{
								cct_gotoxy(x0, y0);
								DrawOneBlock(length, height, COLOR_HWHITE, COLOR_HWHITE, -1, " ");
							}
						}
					}
					else
					{
						for (x1 = xx; x1 >= xx - length; x1--)
						{
							cct_gotoxy(x1, y);
							int x0, y0;
							cct_getxy(x0, y0);
							DrawOneBlock(length, height, m[i3][j3], COLOR_BLACK, style, "★");
							Sleep(1);
							if (x1 > xx - length)
							{
								cct_gotoxy(x0, y0);
								DrawOneBlock(length, height, COLOR_HWHITE, COLOR_HWHITE, -1, " ");
							}

						}
					}
					if (splitline)
					{
						cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
						int x4, y4;
						cct_getxy(x4, y4);
						for (int y5 = y4 - (height - 1); y5 <= y4; y5++)
						{
							cct_gotoxy(x4, y5);
							cout << "║";
						}
					}
					if (splitline)
						cct_gotoxy(x - (length + 2), y);
					else
						cct_gotoxy(x - length, y);
				}
				if (splitline)
					y += height + 1;
				else
					y += height;
				cct_gotoxy(x, y);
			}

		}
	}
	ZeroColMoveLeft(i, j, m, s);
}