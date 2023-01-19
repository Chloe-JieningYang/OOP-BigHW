/*2050598 Ñî½àÄþ Í¨ÐÅ*/
#pragma once
#include "..\include\cmd_gmw_tools.h"

struct MATRIX_SIGN {
	int matrix[10][12];
	int sign[10][12];
};

//toolsÄÚÈÝ£º
void CreatMatrix(int row_num, int col_num, int m[10][12]);

void CombineSame(int i0, int j0, MATRIX_SIGN* mat);

int SearchForMax(int m[10][12]);

void AddNewNum(int i, int j, int m[10][12], int max);

int AddScore(int s[10][12], int zhi);

void RemoveZeroGMW(const CONSOLE_GRAPHICS_INFO* pCGI, MATRIX_SIGN* mat);

void CompleteGame(const CONSOLE_GRAPHICS_INFO* pCGI, MATRIX_SIGN* mat, int& score, int aim, bool& quit);
