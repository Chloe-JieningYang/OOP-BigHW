/* 2050598 ����� ͨ�� */
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
	cout << "��ǰ����:" << endl;
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
		cout << "��ǰ����(��ͬɫ��ʶ):" << endl;
		OutputMatrixArray(i, j, m, s, 1);
		conf = Confirm(j0, i0);
		if (conf == 'Y' || conf == 'Q')
			break;
	}
	if (conf == 'Y')
	{
		CombineSame(i0, j0, m, s);
		cout << endl;
		cout << "��ֵͬ�鲢�������(��ͬɫ��ʶ):" << endl;
		OutputMatrixArray(i, j, m, s, 1);
		cout << endl;
		cout << endl;
		sc += AddScore(s, m[i0][j0]);
		cout << "���ε÷֣�" << AddScore(s, m[i0][j0]) << " �ܵ÷֣�" << sc << " �ϳ�Ŀ�꣺" << aim << endl;
		cout << endl << "���س��������������0����" << endl;
		while (_getch() != '\r');
		RemoveZero(i, j, m, s, "mkten");
		cout << endl << "��0�������(��ͬɫ��ʶ):" << endl;//��ʶ�����ϲ�0
		OutputMatrixArray(i, j, m, s, 1);
		cout << endl;
		cout << "���س���������ֵ����" << endl;
		while (_getch() != '\r');
		AddNewNum(i, j, m, SearchForMax(m));
		cout << endl << "��ֵ���������(��ͬɫ��ʶ):" << endl;
		OutputMatrixArray(i, j, m, s, 1);
		cout << endl;
	}
	else if (conf == 'Q')
	{
		return -1;
	}
	return 0;
}
