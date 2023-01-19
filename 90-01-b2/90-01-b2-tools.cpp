/* 2050598 Ñî½àÄþ Í¨ÐÅ */
#include <iostream>
#include "90-01-b2.h"
#include "..\include\common-judge-and-seek.h"
#include "..\include\common-move-matrix.h"
#include "..\include\common-print-matrix.h"
#include "..\include\common-menu.h"
using namespace std;

void CreatMatrix(int i, int j, int m[10][12])
{
	int i1, j1;
	for (i1 = 1; i1 <= i; i1++)
	{
		for (j1 = 1; j1 <= j; j1++)
			m[i1][j1] = rand() % (3 - 1 + 1) + 1;
	}
}


void CombineSame(int i0, int j0, int m[10][12],int s[10][12])
{
	m[i0][j0]++;
	int i1, j1;
	for (i1 = 1; i1 <= 8; i1++)
	{
		for (j1 = 1; j1 <= 10; j1++)
		{
			if (s[i1][j1] == 1 && (i1 != i0 || j1 != j0))
			{
				m[i1][j1] = 0;
			}
		}
	}
}


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

void AddNewNum(int i, int j, int m[10][12],int max)
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
				else if (max == 6){				
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
				else{				
					bool b = 0;
					while (1){					
						int r = rand() % (100 - 1 + 1) + 1;
						int i;
						if (r <= 80) {
							for (i = 1; i <= max - 3; i++){							
								if (r > (i - 1) * (80 / (max - 3))&&r<=i* (80 / (max - 3))){
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

int AddScore(int s[10][12],int zhi)
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
	int benci= sum * zhi * 3;
	return benci;
}