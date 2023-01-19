/* 2050598 杨洁宁 通信 */
#include <iostream>
#include <iomanip>
#include <conio.h>
#include "..\include\cmd_console_tools.h"
#include "90-01-b2.h"
#include "..\include\common-judge-and-seek.h"
#include "..\include\common-move-matrix.h"
#include "..\include\common-print-matrix.h"
#include "..\include\common-menu.h"
using namespace std;



int Choice3(int aim,int i,int j,int m[10][12],int s[10][12],int &sc)
{
	int j0, i0;
	cout << endl;
	cout << "当前数组:" << endl;
	OutputMatrixArray(i, j, m, s, 0);
	char conf;
	while (1) {
		int i1, j1;
		for (i1 = 0; i1 <= 9; i1++)
		{
			for (j1 = 0; j1 <= 11; j1++)
				s[i1][j1] = 0;
		}
		InputSelection(m, j0, i0, i, j);
		FindSameWithRecursion(m, s, i0, j0);
		cout << endl;
		cout << "当前数组(不同色标识):" << endl;
		OutputMatrixArray(i, j, m, s, 1);
		conf = Confirm(j0, i0);
		if (conf == 'Y' || conf == 'Q')
			break;
	}
	if (conf == 'Y')
	{
		CombineSame(i0, j0, m, s);
		cout << endl;
		cout << "相同值归并后的数组(不同色标识):" << endl;
		OutputMatrixArray(i, j, m, s, 1);
		cout << endl;
		cout << endl;
		sc += AddScore(s, m[i0][j0]);
		cout << "本次得分：" << AddScore(s, m[i0][j0]) << " 总得分：" << sc << " 合成目标：" << aim << endl;
		cout << endl << "按回车进行数组下落除0操作" << endl;
		while (_getch() != '\r');
		RemoveZero(i, j, m, s, "mkten");
		cout << endl << "除0后的数组(不同色标识):" << endl;//标识的是上层0
		OutputMatrixArray(i, j, m, s, 1);
		cout << endl;
		cout << "按回车键进行新值补充" << endl;
		while (_getch() != '\r');
		AddNewNum(i, j, m, SearchForMax(m));
		cout << endl << "新值填充后的数组(不同色标识):" << endl;
		OutputMatrixArray(i, j, m, s, 1);
		cout << endl;
	}
	else if (conf == 'Q')
	{
		return -1;
	}
	return 0;
}
