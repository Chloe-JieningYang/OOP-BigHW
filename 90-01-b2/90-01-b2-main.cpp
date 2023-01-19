/* 2050598 杨洁宁 通信 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <ctime>
#include <cstring>
#include "..\include\cmd_console_tools.h"
#include "90-01-b2.h"
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
		cout << "请输入行数(5-8):" << endl;
		cin >> r;
		if (cin.good() && r >= 5 && r <= 8)
			break;
	}
	while (1)
	{
		cout << "请输入列数(5-10):" << endl;
		cin >> c;
		if (cin.good() && c >= 5 && c <= 10)
			break;
	}
}

void InputAim(int& a)
{
	while (1)
	{
		cout << "请输入合成目标(5-20):" << endl;
		cin >> a;
		if (cin.good() && a >= 5 && a <= 20)
			break;
	}
}

void InputSelection(int m[10][12],int& c, int& r,int i,int j)//选中某一格
{
	char ch;
	int x1, y1;
	cout << endl;
	while (1) {
		cout << "请以字母+数字形式[例：c2]输入矩阵坐标:";
		cin >> ch >> c;
		while (!cin.good() || !(ch >= 'A' && ch <= 'A' + i - 1 || ch >= 'a' && ch <= 'a' + i - 1) || !(c >= 0 && c <= j))
		{
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "输入错误，请重新输入";
			cct_getxy(x1, y1);
			cct_showstr(0, y1 - 1, "请以字母+数字形式[例：c2]输入矩阵坐标:          ");
			cct_getxy(x1, y1);
			cct_gotoxy(x1 - 10, y1);
			cin >> ch >> c;
		}
		if (ch >= 'a' && ch <= 'z')
			ch -= 32;
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

char Confirm(int j0,int i0)
{
	char conf;
	cout << "请确认是否把相邻的相同值合并到" << char('A' + i0 - 1) << j0 - 1 << "中(Y/N/Q):";
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

void End(int height,int i,bool splitline,bool fakefigure)
{
	char e[100] = { '\0' };
	int x1, y1;
	cct_setcolor();
	if (fakefigure)
	{
		if (splitline)
			cct_gotoxy(0, 2 + (height + 1) * i + 2);
		else
			cct_gotoxy(0, 2 + height * i + 3);
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
		for (int p = 0; p <= 9; p++)
		{
			for (int q = 0; q <= 11; q++) {
				mat.matrix[p][q] = 0;
				mat.sign[p][q] = 0;
			}
		}
		int aim = 5;
		int score = 0;
		const char instruction[][100] = { "1.命令行找出可合成项并标识（非递归）","2.命令行找出可合成项并标识（递归）",
		"3.命令行完成一次合成（分步骤显示）","4.命令行完整版（分步骤显示）","5.伪图形界面显示初始数组（无分隔线）",
		"6.伪图形界面显示初始数组（有分隔线）","7.伪图形界面下用箭头键/鼠标选择当前色块","8.伪图形界面完成一次合成（分步骤）",
		"9.伪图形界面完整版（支持鼠标）","0.退出","\0" };
		char choice = menu(instruction);
		if(choice!='0')
			cct_cls();
		if (choice != 'A' && choice != 'B' && choice != 'a' && choice != 'b'&& choice!='0') {
			InputRowCol(j, i);
			CreatMatrix(i, j, mat.matrix);
		}
		if (choice == '1'||choice=='2')
		{
			cout << endl;
			cout << "当前数组：" << endl;
			OutputMatrixArray(i, j, mat.matrix, mat.sign, 0);
			InputSelection(mat.matrix, j0, i0, i, j);
			if (choice == '1')
				FindSameNoRecursion(mat.matrix, mat.sign, i0, j0, i, j);
			else
				FindSameWithRecursion(mat.matrix, mat.sign, i0, j0);
			cout << endl;
			cout << "当前数组(不同色标识):" << endl;
			OutputMatrixArray(i, j, mat.matrix, mat.sign, 1);
			cout << endl;
			End(3, i, 0, 0);
		}
		else if (choice == '3')
		{
			InputAim(aim);
			Choice3(aim, i, j, mat.matrix, mat.sign, score);
			End(3, i, 0, 0);
		}
		else if (choice == '4')
		{
			InputAim(aim);			
			int max = aim - 1;
			while (1)
			{			
				if (Choice3(aim, i, j, mat.matrix, mat.sign, score) == -1)
					break;
				cout << endl << "本次合成结束，按回车键继续新一次的合成" << endl;
				while (char ch = _getch() != '\r');
				if (SearchForMax(mat.matrix) > max) {					
					max = SearchForMax(mat.matrix);						
					cct_setcolor(COLOR_HYELLOW, COLOR_RED);
					cout << "已经合成到" << SearchForMax(mat.matrix);
					cct_setcolor();
					cout << " 按回车键继续向更高目标进发" << endl;
					while (char ch = _getch() != '\r');	
					
				}
				if (JudgeAll(mat.matrix,j,i)){
					int x, y;
					cct_getxy(x, y);
					cct_showstr(0, y + 1, "无可合并的项，游戏结束", 0, COLOR_HYELLOW);
					cct_setcolor();
					cout << "按Q退出";
					while (char ch = _getch()){					
						if (ch == 'q' || ch == 'Q')
							break;
					}
					break;
				}
			}
			End(3, i, 0, 0);
		}
		else if (choice == '5') {
			DrawCompleteMatrix(6, 3, 1, "mkten", j, i, 0, mat.matrix,"www");
			End(3, i, 0, 1);
		}
		else if (choice == '6') {
			DrawCompleteMatrix(6, 3, 1, "mkten", j, i, 1, mat.matrix,"www");
			End(3, i, 1, 1);
		}
		else if (choice == '7') {
			bool quit = 0;
			DrawCompleteMatrix(6, 3, 1, "mkten", j, i, 1, mat.matrix, "does not matter");
			cct_gotoxy(4, 3);
			char content[10] = { '\0' };
			IntToChar(mat.matrix[1][1], content);
			DrawOneBlock(6, 3, mat.matrix[1][1], COLOR_HWHITE, 1, content);
			ChooseOneBlock(mat.matrix, i, j, 6, 3, i0, j0, choice, 1, 1, "mkten", quit);
			End(3, i, 1, 1);
		}
		else if (choice == '8') {
			bool quit = 0;
			DrawCompleteMatrix(6, 3, 1, "mkten", j, i, 1, mat.matrix,"www");
			cct_gotoxy(4, 3);
			char content[10] = { '\0' };
			IntToChar(mat.matrix[1][1], content);
			DrawOneBlock(6, 3, mat.matrix[1][1], COLOR_HWHITE, 1, content);
			OneCombine(i, j, 6, 3, mat.matrix, mat.sign, choice, score, quit, 3);
			End(3, i, 1, 1);
		}
		else if (choice == '9') {
			InputAim(aim);
			DrawCompleteMatrix(6, 3, 1, "mkten", j, i, 1, mat.matrix,"no matter");
			int x1, y1;
			bool quit = 0;
			cct_gotoxy(4, 3);
			cct_setcolor(mat.matrix[1][1], COLOR_HWHITE);
			cout << "┏━┓";
			cct_getxy(x1, y1);
			cct_gotoxy(x1 - 6, y1 + 1);
			cout << "┃";
			cout << resetiosflags(ios::right);
			cout << setiosflags(ios::left) << setw(2) << mat.matrix[1][1];
			cout << "┃";
			cct_getxy(x1, y1);
			cct_gotoxy(x1 - 6, y1 + 1);
			cout << "┗━┛";
			CompleteGame(i, j, 6, 3, mat.matrix, mat.sign, choice, score, quit, aim);
			End(3, i, 1, 1);
		}
		else if(choice=='0')
			break;
	}
	return 0;
}