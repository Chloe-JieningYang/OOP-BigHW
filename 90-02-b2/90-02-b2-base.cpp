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

//游戏开始前构建数组
void CreateMatrix(int g2048[][10], const CONSOLE_GRAPHICS_INFO CGI)//最大8行10列
{
	for (int i = 0; i <= 7; i++)
	{
		for (int j = 0; j <= 9; j++)
			g2048[i][j] = 0;
	}
	int row1, row2, col1, col2;//从0开始数
	row1 = rand() % CGI.row_num;
	col1 = rand() % CGI.col_num;

	while (1)
	{
		row2 = rand() % CGI.row_num;
		col2 = rand() % CGI.col_num;
		if (row2 != row1 || col2 != col1)
			break;
	}

	g2048[row1][col1] = 2;
	g2048[row2][col2] = 2;

	return;
}

//合并相同数并加分，type为移动方向
void CombineSame(int g2048[][10],int mark[][10], const CONSOLE_GRAPHICS_INFO CGI, const int type, int &score)
{
	const int row = CGI.row_num;
	const int col = CGI.col_num;
	if (type == UP_TO_DOWN)
	{
		for (int j = 0; j <= col - 1; j++)
		{
			for (int i = row - 1; i >= 1; i--)
			{
				if (g2048[i][j] == g2048[i - 1][j])
				{
					score += 2 * g2048[i][j];					
					for (int i1 = i - 1; i1 >= 0; i1--)
					{
						if (g2048[i1][j] != 0)
							mark[i1][j]++;
					}
					g2048[i][j] *= 2;
					g2048[i - 1][j] = 0;
				}
			}
		}
	}
	else if (type == DOWN_TO_UP)
	{
		for (int j = 0; j <= col - 1; j++)
		{
			for (int i = 0; i <= row - 1 - 1; i++)
			{
				if (g2048[i][j] == g2048[i + 1][j])
				{
					score += 2 * g2048[i][j];					
					for (int i1 = i + 1; i1 <= row - 1; i1++)
					{
						if (g2048[i1][j] != 0)
							mark[i1][j]++;
					}
					g2048[i][j] *= 2;
					g2048[i + 1][j] = 0;
				}
			}
		}
	}
	else if (type == RIGHT_TO_LEFT)
	{
		for (int i = 0; i <= row - 1; i++)
		{
			for (int j = 0; j <= col - 1 - 1; j++)
			{
				if (g2048[i][j] == g2048[i][j + 1])
				{
					score += 2 * g2048[i][j];					
					for (int j1 = j + 1; j1 <= col - 1; j1++)
					{
						if (g2048[i][j1] != 0)
							mark[i][j1]++;
					}
					g2048[i][j] *= 2;
					g2048[i][j + 1] = 0;
				}
			}
		}
	}
	else if (type == LEFT_TO_RIGHT)
	{
		for (int i = 0; i <= row - 1; i++)
		{
			for (int j = col - 1; j >= 1; j--)
			{
				if (g2048[i][j] == g2048[i][j - 1])
				{
					score += 2 * g2048[i][j];					
					for (int j1 = j - 1; j1 >= 0; j1--)
					{
						if (g2048[i][j1] != 0)
							mark[i][j1]++;
					}
					g2048[i][j] *= 2;
					g2048[i][j - 1] = 0;
				}
			}
		}
	}

	return;
}

//移动0
void RemoveZero(int g2048[][10], const CONSOLE_GRAPHICS_INFO CGI, const int type)
{
	const int row = CGI.row_num;
	const int col = CGI.col_num;
	if (type == UP_TO_DOWN)
	{
		for (int j = 0; j <= col - 1; j++)
		{
			for (int i = 1; i <= row - 1; i++)
			{
				if (g2048[i][j] == 0)
				{
					for (int i1 = i; i1 >= 1; i1--)					
						g2048[i1][j] = g2048[i1 - 1][j];							
					g2048[0][j] = 0;
				}
			}
		}
	}
	else if (type == DOWN_TO_UP)
	{
		for (int j = 0; j <= col - 1; j++)
		{
			for (int i = row - 1 - 1; i >= 0; i--)
			{
				if (g2048[i][j] == 0)
				{
					for (int i1 = i; i1 <= row - 1 - 1; i1++)
						g2048[i1][j] = g2048[i1 + 1][j];
					g2048[row - 1][j] = 0;
				}
				
			}
		}
	}
	else if (type == RIGHT_TO_LEFT)
	{
		for (int i = 0; i <= row - 1; i++)
		{
			for (int j = col - 1 - 1; j >= 0; j--)
			{
				if (g2048[i][j] == 0)
				{
					for (int j1 = j; j1 <= col - 1 - 1; j1++)
						g2048[i][j1] = g2048[i][j1 + 1];
					g2048[i][col - 1] = 0;
				}
				
			}
		}
	}
	else if (type == LEFT_TO_RIGHT)
	{
		for (int i = 0; i <= row - 1; i++)
		{
			for (int j = 1; j <= col - 1; j++)
			{
				if (g2048[i][j] == 0)
				{
					for (int j1 = j; j1 >= 1; j1--)
						g2048[i][j1] = g2048[i][j1 - 1];
					g2048[i][0] = 0;
				}
				
			}
		}
	}
	
	return;
}

//判断前后数组是否一样，一样返回true，不一样返回false
bool JudgeSame(const int g2048_old[][10], const int g2048[][10])
{
	bool same = true;
	for (int i = 0; i <= 7; i++)
	{
		for (int j = 0; j <= 9; j++)
		{
			if (g2048_old[i][j] != g2048[i][j])
			{
				same = false;
				return same;
			}
		}
	}
	return same;
}

//找最大值
int SearchForMax(int g2048[][10])
{
	int max = 0;
	int i1, j1;
	for (i1 = 0; i1 <= 7; i1++)
	{
		for (j1 = 0; j1 <= 9; j1++)
		{
			if (g2048[i1][j1] > max)
				max = g2048[i1][j1];
		}
	}
	return max;
}

//填充新数
void AddNewNumber(const CONSOLE_GRAPHICS_INFO CGI, int g2048[][10], int& i0, int& j0)
{
	int row = CGI.row_num;
	int col = CGI.col_num;

	int i, j;
	while (1)
	{
		i = rand() % row;
		j = rand() % col;
		if (g2048[i][j] == 0)
			break;
	}
	i0 = i;
	j0 = j;
	int random = rand() % 10;
	if (random >= 3)
		g2048[i][j] = 2;
	else
		g2048[i][j] = 4;
	return;
}

//找一个数组中一个数前面跟它相同的数是不是偶数个，是的话不用重画格子,返回true不用画，false要重画
bool FindSameAhead(const int g2048[][10], const int i0, const int j0, const int row, const int col, const int type)
{
	int sum = 0;
	if (type == UP_TO_DOWN)
	{
		for (int i1 = i0 + 1; i1 <= row - 1; i1++)
		{
			if (g2048[i1][j0] == 0)
				continue;
			else if (g2048[i1][j0] != g2048[i0][j0])
				break;
			else if (g2048[i1][j0] == g2048[i0][j0])
				sum++;
		}
	}
	else if (type == DOWN_TO_UP)
	{
		for (int i1 = i0 - 1; i1 >= 0; i1--)
		{
			if (g2048[i1][j0] == 0)
				continue;
			else if (g2048[i1][j0] != g2048[i0][j0])
				break;
			else if (g2048[i1][j0] == g2048[i0][j0])
				sum++;
		}
	}
	else if (type == RIGHT_TO_LEFT)
	{
		for (int j1 = j0 - 1; j1 >= 0; j1--)
		{
			if (g2048[i0][j1] == 0)
				continue;
			else if (g2048[i0][j1] != g2048[i0][j0])
				break;
			else if (g2048[i0][j1] == g2048[i0][j0])
				sum++;
		}
	}
	else if (type == LEFT_TO_RIGHT)
	{
		for (int j1 = j0 + 1; j1 <= col - 1; j1++)
		{
			if (g2048[i0][j1] == 0)
				continue;
			else if (g2048[i0][j1] != g2048[i0][j0])
				break;
			else if (g2048[i0][j1] == g2048[i0][j0])
				sum++;
		}
	}

	if (sum % 2 == 0)
		return true;
	else
		return false;
	
}

//判断合成有没有结束,返回true结束，false不结束
bool JudgeEnd(const int g2048[][10], const CONSOLE_GRAPHICS_INFO CGI)
{
	int row = CGI.row_num;
	int col = CGI.col_num;

	bool end = true;
	//查是不是全满了

	for (int i = 0; i <= row - 1; i++)
	{
		for (int j = 0; j <= col - 1; j++)
		{
			if (g2048[i][j] == 0)
				return false;
		}
	}

	for (int i = 1; i <= row - 2; i++)
	{
		for (int j = 1; j <= col - 2; j++)
		{
			if (g2048[i][j + 1] != g2048[i][j] && g2048[i][j - 1] != g2048[i][j] && g2048[i + 1][j] != g2048[i][j] && g2048[i - 1][j] != g2048[i][j])
				end = true;
			else
				return false;
		}
	}
	//开始找边边
	for (int j = 0; j <= col - 2; j++)
	{
		if (g2048[0][j] == g2048[0][j + 1])
			return false;
		if (g2048[row - 1][j] == g2048[row - 1][j + 1])
			return false;
	}

	for (int i = 0; i <= row - 2; i++)
	{
		if (g2048[i][0] == g2048[i + 1][0])
			return false;
		if (g2048[i][col - 1] == g2048[i + 1][col - 1])
			return false;
	}

	return true;	
}