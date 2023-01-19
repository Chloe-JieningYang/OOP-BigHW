/* 2050598 ����� ͨ�� */
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

void InputRowCol(int& c, int& r)//������������
{
	while (1)
	{
		cout << "����������(5-8):" << endl;
		cin >> r;
		if (cin.good() && r >= 5 && r <= 8)
			break;
	}
	while (1)
	{
		cout << "����������(5-10):" << endl;
		cin >> c;
		if (cin.good() && c >= 5 && c <= 10)
			break;
	}
}

void InputAim(int& a)
{
	while (1)
	{
		cout << "������ϳ�Ŀ��(5-20):" << endl;
		cin >> a;
		if (cin.good() && a >= 5 && a <= 20)
			break;
	}
}

void InputSelection(int m[10][12],int& c, int& r,int i,int j)//ѡ��ĳһ��
{
	char ch;
	int x1, y1;
	cout << endl;
	while (1) {
		cout << "������ĸ+������ʽ[����c2]�����������:";
		cin >> ch >> c;
		while (!cin.good() || !(ch >= 'A' && ch <= 'A' + i - 1 || ch >= 'a' && ch <= 'a' + i - 1) || !(c >= 0 && c <= j))
		{
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "�����������������";
			cct_getxy(x1, y1);
			cct_showstr(0, y1 - 1, "������ĸ+������ʽ[����c2]�����������:          ");
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
			cout << "����Ϊ" << ch << "��" << c << "��" << endl;
			cout << "����ľ�������λ�ô���������ֵͬ������������" << endl;
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
	cout << "��ȷ���Ƿ�����ڵ���ֵͬ�ϲ���" << char('A' + i0 - 1) << j0 - 1 << "��(Y/N/Q):";
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
	cout << "��С�������������End����...";
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
		cout << "�����������������";
		cct_getxy(x1, y1);
		cct_showstr(0, y1 - 1, "��С�������������End����...          ");
		cct_getxy(x1, y1);
		cct_gotoxy(x1 - 10, y1);
		cin >> e;
	}
}

int main()
{
	srand((unsigned)time(NULL));
	int i, j;//��������
	int i0, j0;//ѡ�е�ĳһ��
	
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
		const char instruction[][100] = { "1.�������ҳ��ɺϳ����ʶ���ǵݹ飩","2.�������ҳ��ɺϳ����ʶ���ݹ飩",
		"3.���������һ�κϳɣ��ֲ�����ʾ��","4.�����������棨�ֲ�����ʾ��","5.αͼ�ν�����ʾ��ʼ���飨�޷ָ��ߣ�",
		"6.αͼ�ν�����ʾ��ʼ���飨�зָ��ߣ�","7.αͼ�ν������ü�ͷ��/���ѡ��ǰɫ��","8.αͼ�ν������һ�κϳɣ��ֲ��裩",
		"9.αͼ�ν��������棨֧����꣩","0.�˳�","\0" };
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
			cout << "��ǰ���飺" << endl;
			OutputMatrixArray(i, j, mat.matrix, mat.sign, 0);
			InputSelection(mat.matrix, j0, i0, i, j);
			if (choice == '1')
				FindSameNoRecursion(mat.matrix, mat.sign, i0, j0, i, j);
			else
				FindSameWithRecursion(mat.matrix, mat.sign, i0, j0);
			cout << endl;
			cout << "��ǰ����(��ͬɫ��ʶ):" << endl;
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
				cout << endl << "���κϳɽ��������س���������һ�εĺϳ�" << endl;
				while (char ch = _getch() != '\r');
				if (SearchForMax(mat.matrix) > max) {					
					max = SearchForMax(mat.matrix);						
					cct_setcolor(COLOR_HYELLOW, COLOR_RED);
					cout << "�Ѿ��ϳɵ�" << SearchForMax(mat.matrix);
					cct_setcolor();
					cout << " ���س������������Ŀ�����" << endl;
					while (char ch = _getch() != '\r');	
					
				}
				if (JudgeAll(mat.matrix,j,i)){
					int x, y;
					cct_getxy(x, y);
					cct_showstr(0, y + 1, "�޿ɺϲ������Ϸ����", 0, COLOR_HYELLOW);
					cct_setcolor();
					cout << "��Q�˳�";
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
			cout << "������";
			cct_getxy(x1, y1);
			cct_gotoxy(x1 - 6, y1 + 1);
			cout << "��";
			cout << resetiosflags(ios::right);
			cout << setiosflags(ios::left) << setw(2) << mat.matrix[1][1];
			cout << "��";
			cct_getxy(x1, y1);
			cct_gotoxy(x1 - 6, y1 + 1);
			cout << "������";
			CompleteGame(i, j, 6, 3, mat.matrix, mat.sign, choice, score, quit, aim);
			End(3, i, 1, 1);
		}
		else if(choice=='0')
			break;
	}
	return 0;
}