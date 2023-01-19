/* 2050598 杨洁宁 通信 */
#include <iostream>
#include "90-01-b2-gmw.h"
#include "..\include\common-judge-and-seek.h"
#include "..\include\common-move-matrix.h"
#include "..\include\cmd_gmw_tools.h"

using namespace std;

/***************************************************************************
  函数名称：
  功    能：随机给初始游戏数组赋值
  输入参数：行数，列数，数组
  返 回 值：
  说    明：
***************************************************************************/
void CreatMatrix(int row_num, int col_num, int m[10][12])
{
	int i1, j1;
	for (i1 = 1; i1 <= row_num; i1++)
	{
		for (j1 = 1; j1 <= col_num; j1++)
			m[i1][j1] = rand() % (3 - 1 + 1) + 1;
	}
}


/***************************************************************************
  函数名称：
  功    能：在数组中合并选中的一众方块
  输入参数：被选中方块的行数（从1开始），列数（从1开始），存值数组，标记数组
  返 回 值：
  说    明：
***************************************************************************/
void CombineSame(int i0, int j0, MATRIX_SIGN *mat)
{
	mat->matrix[i0][j0]++;
	int i1, j1;
	for (i1 = 1; i1 <= 8; i1++)
	{
		for (j1 = 1; j1 <= 10; j1++)
		{
			if (mat->sign[i1][j1] == 1 && (i1 != i0 || j1 != j0))
			{
				mat->matrix[i1][j1] = 0;
			}
		}
	}
}


/***************************************************************************
  函数名称：
  功    能：在数组中寻找当前数组中的最大值
  输入参数：被选中方块的行数（从1开始），列数（从1开始），存值数组，标记数组
  返 回 值：
  说    明：
***************************************************************************/
int SearchForMax(int m[10][12])
{
	int max = 0;
	int i1, j1;
	for (i1 = 1; i1 <= 8; i1++)
	{
		for (j1 = 1; j1 <= 10; j1++)
		{
			if (m[i1][j1] > max)
				max = m[i1][j1];
		}
	}
	return max;
}


/***************************************************************************
  函数名称：
  功    能：数组中添加新的数字
  输入参数：总行，总列，存值数组，当前数组中最大值
  返 回 值：
  说    明：
***************************************************************************/
void AddNewNum(int i, int j, int m[10][12], int max)
{
	int i1, j1;
	for (i1 = 1; i1 <= i; i1++)
	{
		for (j1 = 1; j1 <= j; j1++)
		{
			if (m[i1][j1] == 0)
			{
				if (max == 3)
				{
					m[i1][j1] = rand() % (3 - 1 + 1) + 1;
				}
				else if (max == 4)
				{
					int r = rand() % (10 - 1 + 1) + 1;
					if (r <= 3)
						m[i1][j1] = 1;
					else if (r <= 6)
						m[i1][j1] = 2;
					else if (r <= 9)
						m[i1][j1] = 3;
					else
						m[i1][j1] = 4;
				}
				else if (max == 5)
				{
					int r = rand() % (100 - 1 + 1) + 1;
					if (r <= 25)
						m[i1][j1] = 1;
					else if (r <= 50)
						m[i1][j1] = 2;
					else if (r <= 75)
						m[i1][j1] = 3;
					else if (r <= 90)
						m[i1][j1] = 4;
					else
						m[i1][j1] = 5;
				}
				else if (max == 6) {
					int r = rand() % (100 - 1 + 1) + 1;
					if (r <= 20)
						m[i1][j1] = 1;
					else if (r <= 40)
						m[i1][j1] = 2;
					else if (r <= 60)
						m[i1][j1] = 3;
					else if (r <= 80)
						m[i1][j1] = 4;
					else if (r <= 95)
						m[i1][j1] = 5;
					else
						m[i1][j1] = 6;
				}
				else {
					bool b = 0;
					while (1) {
						int r = rand() % (100 - 1 + 1) + 1;
						int i;
						if (r <= 80) {
							for (i = 1; i <= max - 3; i++) {
								if (r > (i - 1) * (80 / (max - 3)) && r <= i * (80 / (max - 3))) {
									m[i1][j1] = i;
									b = 1;
									break;
								}
							}
						}
						else if (r <= 90) {
							m[i1][j1] = max - 2;
							b = 1;
						}
						else if (r <= 95) {
							m[i1][j1] = max - 1;
							b = 1;
						}
						else {
							m[i1][j1] = max;
							b = 1;
						}
						if (b == 1)
							break;
					}
				}
			}
		}
	}
}


/***************************************************************************
  函数名称：
  功    能：加分数
  输入参数：标记数组，本次选择方块的值
  返 回 值：
  说    明：
***************************************************************************/
int AddScore(int s[10][12], int zhi)
{
	int sum = 0;
	int i, j;
	for (i = 1; i <= 8; i++)
	{
		for (j = 1; j <= 10; j++)
		{
			if (s[i][j] == 1)
				sum++;
		}
	}
	int benci = sum * zhi * 3;
	return benci;
}

/***************************************************************************
  函数名称：
  功    能：数组下落0
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void RemoveZeroGMW(const CONSOLE_GRAPHICS_INFO* pCGI, MATRIX_SIGN* mat)
{
	int j1, i1;
	for (j1 = 1; j1 <= pCGI->col_num; j1++)
	{
		for (i1 = 2; i1 <= pCGI->row_num; i1++)
		{
			if (mat->matrix[i1][j1] == 0)
			{
				int i2;
				for (i2 = i1; i2 >= 2; i2--)//下落填充0
				{
					mat->matrix[i2][j1] = mat->matrix[i2 - 1][j1];
				}
				mat->matrix[1][j1] = 0;
			}
		}
	}
	for (i1 = 0; i1 <= pCGI->row_num; i1++)//将标记矩阵置零
	{
		for (j1 = 0; j1 <= 11; j1++)
			mat->sign[i1][j1] = 0;
	}
	for (i1 = 1; i1 <= pCGI->row_num; i1++)//标记矩阵中0的位置
	{
		for (j1 = 1; j1 <= pCGI->col_num; j1++)
		{
			if (mat->matrix[i1][j1] == 0)
				mat->sign[i1][j1] = 1;
		}
	}
}