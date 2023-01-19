/* 2050598 Í¨ÐÅ Ñî½àÄþ */
#include <iostream>
#include "90-02-b1.h"
#include "..\include\common-judge-and-seek.h"
#include "..\include\common-move-matrix.h"
#include "..\include\common-print-matrix.h"
#include "..\include\common-menu.h"
using namespace std;

void CreatMatrix(int i, int j, int m[12][12])
{
	int i1, j1;
	for (i1 = 1; i1 <= i; i1++)
	{
		for (j1 = 1; j1 <= j; j1++)
			m[i1][j1] = rand() % (5 - 1 + 1) + 1;
	}
}

void EraseSame(int i0, int j0, int m[12][12], int s[12][12])
{
	m[i0][j0] = 0;
	int i1, j1;
	for (i1 = 1; i1 <= 10; i1++)
	{
		for (j1 = 1; j1 <= 10; j1++)
		{
			if (s[i1][j1] == 1 )
			{
				m[i1][j1] = 0;
			}
		}
	}
}

int AddScore(int s[12][12], int zhi)
{
	int sum = 0;
	int i, j;
	for (i = 1; i <= 10; i++)
	{
		for (j = 1; j <= 10; j++)
		{
			if (s[i][j] == 1)
				sum++;
		}
	}
	int benci = sum * sum * 5;
	return benci;
}

int AwardScore(int m[12][12], int i, int j)
{
	int numofstar = 0;
	int awardscore = 0;
	for (int i1 = 1; i1 <= i; i1++)
	{
		for (int j1 = 1; j1 <= j; j1++)
		{
			if (m[i1][j1] != 0)
				numofstar++;
		}
	}
	if (numofstar < 10)
	{
		awardscore = (10 - numofstar) * 180;
	}
	return awardscore;
}