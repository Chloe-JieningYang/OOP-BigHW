/* 2050598 通信 杨洁宁 */
#define _CRT_SECURE_NO_WARNINGS
#define LENGTH 6
#define HEIGHT 3
#include <iostream>
#include "../include/cmd_console_tools.h"
#include <iomanip>
#include <conio.h>
#include <ctime>
#include <cstring>
#include "90-02-b1.h"
#include "..\include\common-menu.h"
#include "..\include\common-judge-and-seek.h"
#include "..\include\common-move-matrix.h"
#include "..\include\common-print-matrix.h"
#include "..\include\common-console.h"
using namespace std;

void InputRowCol(int& c, int& r)//输入总行总列
{
	while (1)
	{
		cout << "请输入行数(8-10):" << endl;
		cin >> r;
		if (cin.good() && r >= 8 && r <= 10)
			break;
	}
	while (1)
	{
		cout << "请输入列数(8-10):" << endl;
		cin >> c;
		if (cin.good() && c >= 8 && c <= 10)
			break;
	}
}

void InputSelection(int m[12][12], int& c, int& r, int i, int j)//选中某一格
{
	char ch;
	int x1, y1;
	cout << endl;
	while (1) {
		cout << "请以字母+数字形式[例：c2]输入矩阵坐标:";
		cin >> ch >> c;
		if (ch >= 'a' && ch <= 'z')
			ch -= 32;
		while (!cin.good() || !(ch >= 'A' && ch <= 'A' + i - 1 ) || !(c >= 0 && c <= j-1)||m[int(ch - 'A' + 1)][c+1]==0)
		{
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "输入错误，请重新输入";
			cct_getxy(x1, y1);
			cct_showstr(0, y1 - 1, "请以字母+数字形式[例：c2]输入矩阵坐标:          ");
			cct_getxy(x1, y1);
			cct_gotoxy(x1 - 10, y1);
			cin >> ch >> c;
			if (ch >= 'a' && ch <= 'z')
				ch -= 32;
		}
		
		if (!JudgeRound(m, ch - 'A' + 1, c + 1))
		{
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "输入为" << ch << "行" << c << "列" << endl;
			cout << "输入的矩阵坐标位置处无连续相同值，请重新输入" << endl;
		}
		else
			break;
	}
	r = int(ch - 'A' + 1);
	c++;
}

char Confirm(int j0, int i0)
{
	char conf;
	cout << "请确认是否把" << char('A' + i0 - 1) << j0 - 1 << "及周围的相同值消除(Y/N/Q):";
	while (1)
	{
		cin >> conf;
		if (cin.good() && (conf == 'Y' || conf == 'N' || conf == 'Q' || conf == 'y' || conf == 'n' || conf == 'q'))
			break;
	}
	if (conf == 'y' || conf == 'n' || conf == 'q')
		conf -= 32;
	return conf;
}

void End(int height, int i, bool splitline, bool fakefigure)
{
	char e[100] = { '\0' };
	int x1, y1;
	cct_setcolor();
	if (fakefigure)
	{
		if (splitline)
			cct_gotoxy(0, UPVACROW + (height + 1) * i + 1);
		else
			cct_gotoxy(0, 2 + height * i + UPVACROW);
	}
	else
		cout << endl;
	cout << "本小题结束，请输入End继续...";
	cin >> e;
	int i1;
	for (i1 = 0; i1 <= 2; i1++)
	{
		if (e[i1] == 'E' || e[i1] == 'N' || e[i1] == 'D')
			e[i1] += 32;
	}
	while (strcmp(e, "end"))
	{
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "输入错误，请重新输入";
		cct_getxy(x1, y1);
		cct_showstr(0, y1 - 1, "本小题结束，请输入End继续...          ");
		cct_getxy(x1, y1);
		cct_gotoxy(x1 - 10, y1);
		cin >> e;
	}
}

int main()
{
	srand((unsigned)time(NULL));
	int i, j;//总行总列
	int i0, j0;//选中的某一格

	while (1)
	{
		cct_setconsoleborder(69, 21, 120, 9000);
		cct_setcolor();
		cct_cls();
		cct_setcursor(CURSOR_VISIBLE_NORMAL);
		struct matrix_sign mat;
		for (int p = 0; p <= 11; p++)
		{
			for (int q = 0; q <= 11; q++) {
				mat.matrix[p][q] = 0;
				mat.sign[p][q] = 0;
			}
		}
		int score = 0;
		const char instruction[][100] = { "A.命令行找出可消除项并标识","B.命令行完成一次消除（分步骤显示）","C.命令行完成一关（分步骤显示）",
			"D.伪图形界面下用鼠标选择一个色块（无分隔线）","E.伪图形界面下用鼠标选择一个色块（有分割线）","F.伪图形界面完成一次消除（分步骤）",
			"G.伪图形界面完整版","Q.退出","\0" };
		char choice = menu(instruction);
		if (choice != 'Q' && choice != 'q')
		{
			cct_cls();
			InputRowCol(j, i);
			CreatMatrix(i, j, mat.matrix);
		}
		if (choice == 'A' || choice == 'a')
		{
			cout << endl;
			cout << "当前数组：" << endl;
			OutputMatrixArray(i, j, mat.matrix, mat.sign, 0);
			InputSelection(mat.matrix, j0, i0, i, j);
			FindSameNoRecursion(mat.matrix, mat.sign, i0, j0, i, j);
			cout << endl;
			cout << "当前数组(不同色标识):" << endl;
			OutputMatrixArray(i, j, mat.matrix, mat.sign, 1);
			cout << endl;
			End(3, i, 0, 0);
		}
		else if (choice == 'B' || choice == 'b')
		{
			OrderLineOneErase(i, j, mat.matrix, mat.sign, score);
			End(3, i, 0, 0);
		}
		else if (choice == 'C' || choice == 'c')
		{
			while (1)
			{
				if (OrderLineOneErase(i, j, mat.matrix, mat.sign, score) == -1)
					break;
				cout << endl << "本次消除结束，按回车键继续新一次的消除" << endl;
				while (char ch = _getch() != '\r');
				if (JudgeAll(mat.matrix, j, i)) {
					int x, y;
					cct_getxy(x, y);
					cct_gotoxy(0, y + 1);
					cct_setcolor(COLOR_HYELLOW, COLOR_HRED);
					int sum = 0;
					for (int p = 1; p <= i; p++)
					{
						for (int q = 1; q <= j; q++)
						{
							if (mat.matrix[p][q] != 0)
								sum++;
						}
					}
					cout << "剩余" << sum << "个不可消除项，本关结束" << endl;
					cct_setcolor();					
					break;
				}
			}
			End(3, i, 0, 0);
		}
		else if (choice == 'D' || choice == 'd')
		{
			bool quit = 0;
			DrawCompleteMatrix(LENGTH, HEIGHT, 0, "erasestar", j, i, 0, mat.matrix, "★");
			cct_gotoxy(4, 3);
			DrawOneBlock(LENGTH, HEIGHT, mat.matrix[1][1], COLOR_HWHITE, 0, "★");
			ChooseOneBlock(mat.matrix, i, j, LENGTH, HEIGHT, i0, j0, 'D', 0, 0, "erasestar", quit);
			End(HEIGHT, i, 0, 1);
		}
		else if (choice == 'E' || choice == 'e')
		{
			bool quit = 0;
			DrawCompleteMatrix(LENGTH, HEIGHT, 0, "erasestar", j, i, 1, mat.matrix, "★");
			cct_gotoxy(4, 3);
			DrawOneBlock(LENGTH, HEIGHT, mat.matrix[1][1], COLOR_HWHITE, 0, "★");
			ChooseOneBlock(mat.matrix, i, j, LENGTH, HEIGHT, i0, j0, 'E', 1, 0, "erasestar", quit);
			End(HEIGHT, i, 1, 1);
		}
		else if (choice == 'F' || choice == 'f')
		{
			bool quit = 0;
			DrawCompleteMatrix(LENGTH, HEIGHT, 0, "erasestar", j, i, 0, mat.matrix, "★");
			cct_gotoxy(4, 3);
			DrawOneBlock(LENGTH, HEIGHT, mat.matrix[1][1], COLOR_HWHITE, 0, "★");
			OneErase(i, j, LENGTH, HEIGHT, mat.matrix, mat.sign, 'F', 0, score, quit);
			End(HEIGHT, i, 0, 1);
		}
		else if (choice == 'G' || choice == 'g')
		{
			while (1)
			{
				cct_setcolor();
				cct_cls();
				cct_setcursor(CURSOR_VISIBLE_NORMAL);
				for (int p = 0; p <= 11; p++)
				{
					for (int q = 0; q <= 11; q++) {
						mat.matrix[p][q] = 0;
						mat.sign[p][q] = 0;
					}
				}
				score = 0;
				CreatMatrix(i, j, mat.matrix);
				DrawCompleteMatrix(LENGTH, HEIGHT, 0, "erasestar", j, i, 1, mat.matrix, "★");
				bool quit = 0;
				cct_gotoxy(4, 3);
				DrawOneBlock(LENGTH, HEIGHT, mat.matrix[1][1], COLOR_HWHITE, 0, "★");
				CompleteGame(i, j, LENGTH, HEIGHT, mat.matrix, mat.sign, 1, 'G', score, quit);
				if (quit == 1)
					break;
			}
			End(HEIGHT, i, 1, 1);
		}
		else if (choice == 'Q' || choice == 'q')
			break;
	}
	return 0;
}
