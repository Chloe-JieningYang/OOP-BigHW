/* 2050598 杨洁宁 通信 */
#include <iostream>
#include <iomanip>
#include <conio.h>
#include "..\include\cmd_console_tools.h"
#include "90-02-b1.h"
#include "..\include\common-judge-and-seek.h"
#include "..\include\common-move-matrix.h"
#include "..\include\common-print-matrix.h"
#include "..\include\common-menu.h"
using namespace std;

int OrderLineOneErase(int i, int j, int m[12][12], int s[12][12], int& sc)
{
	int j0, i0;
	cout << endl;
	cout << "当前数组:" << endl;
	OutputMatrixArray(i, j, m, s, 1);
	char conf;
	while (1) {
		int i1, j1;
		for (i1 = 0; i1 <= 11; i1++)
		{
			for (j1 = 0; j1 <= 11; j1++)
				s[i1][j1] = 0;
		}
		InputSelection(m, j0, i0, i, j);
		FindSameWithRecursion(m, s, i0, j0);
		cout << endl;
		cout << "查找结果数组:" << endl;
		cout << setfill(' ') << setw(3) << "|";
		int c;
		for (c = 0; c <= j - 1; c++)
			cout << setw(3) << c;
		cout << endl;
		cout << "--+";
		cout << setfill('-') << setw(3 * j + 1) << "-" << endl;		
		int n;
		for (n = 1; n <= i; n++)
		{
			cout << char(n - 1 + 'A') << " |";
			for (int c = 1; c <= j; c++)
			{
				if (s[n][c] == 1)
					cout << setfill(' ') << setw(3) << '*';
				else
					cout << setfill(' ') << setw(3) << '0';
			}
			cout << endl;
		}
		cout << endl;
		cout << "当前数组(不同色标识):" << endl;
		OutputMatrixArray(i, j, m, s, 1);
		conf = Confirm(j0, i0);
		if (conf == 'Y' || conf == 'Q')
			break;
	}
	if (conf == 'Y')
	{
		EraseSame(i0, j0, m, s);
		cout << endl;
		cout << "相同值归并后的数组(不同色标识):" << endl;
		OutputMatrixArray(i, j, m, s, 1);
		cout << endl;
		cout << endl;
		sc += AddScore(s, m[i0][j0]);
		cout << "本次得分：" << AddScore(s, m[i0][j0]) << " 总得分：" << sc << endl;
		cout << endl << "按回车进行数组下落除0操作" << endl;
		while (_getch() != '\r');
		RemoveZero(i, j, m, s, "erasestar");
		ZeroColMoveLeft(i, j, m, s);
		cout << endl << "下落后的数组:" << endl;
		OutputMatrixArray(i, j, m, s, 1);
		cout << endl;
	}
	else if (conf == 'Q')
	{
		return -1;
	}
	return 0;
}