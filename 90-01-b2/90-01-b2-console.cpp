/* 2050598 ����� ͨ�� */
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <conio.h>
#include <cstring>
#include "..\include\cmd_console_tools.h"
#include "90-01-b2.h"
#include "..\include\common-menu.h"
#include "..\include\common-judge-and-seek.h"
#include "..\include\common-move-matrix.h"
#include "..\include\common-print-matrix.h"
#include "..\include\common-console.h"
using namespace std;

void OneCombine(int i, int j,int length,int height, int m[][12], int s[][12], char choice, int &score,bool &qui, int aim)
{
	int i0, j0;
	bool brk = 0;
	while (1)
	{
		for (int p = 0; p <= 9; p++)
			for (int q = 0; q <= 11; q++)
				s[p][q] = 0;
		ChooseOneBlock(m, i, j, length, height, i0, j0, choice, 1, 1, "mkten", qui);
		if (qui == 1)
			return;
		if (!JudgeRound(m, i0, j0))
		{
			cct_gotoxy(0, 3 + (height + 1) * i);
			cct_setcolor(COLOR_BLACK, COLOR_HYELLOW);
			cout << "��Χ����ֵͬ";
			cct_setcolor();
			cout << "��ͷ��/����ƶ����س���/�������ѡ��";
			while (1)
			{
				int I, J;
				int event_ = MousePositionAndKeyboardToIJ(m, I, J, i, j, length, height, 1, 1);
				if (I != i0 || J != j0 || event_ == 0)
				{					
					cct_gotoxy(4 + (length + 2) * (j0 - 1), 3 + (height + 1) * (i0 - 1));
					char content[10] = { '\0' };
					IntToChar(m[i0][j0], content);
					DrawOneBlock(length, height, m[i0][j0], COLOR_BLACK, 1, content);
					break;
				}
			}
		}
		else
		{
			FindSameWithRecursion(m, s, i0, j0);
			int i1, j1;
			for (i1 = 1; i1 <= i; i1++)
			{
				for (j1 = 1; j1 <= j; j1++)
				{
					if (s[i1][j1] == 1&&(i1!=i0||j1!=j0))
					{
						cct_gotoxy(4 + (length + 2) * (j1 - 1), 3 + (height + 1) * (i1 - 1));
						char content[10] = { '\0' };
						IntToChar(m[i1][j1], content);
						DrawOneBlock(length, height, m[i1][j1], COLOR_WHITE, 1, content);						
					}
				}
			}
			cct_gotoxy(0, 3 + (height + 1) * i);
			cct_setcolor();
			cout << "��ͷ��/����ƶ�ȡ����ǰѡ�񣬻س���/��������ϳ�";			
			int I, J;
			while (1)
			{
				int event_ = MousePositionAndKeyboardToIJ(m, I, J, i, j, length, height, 1, 1);
				if (event_ == 2)
				{
					brk = 1;
					break;
				}
				else
				{
					if (I != i0 || J != j0 || event_ == 0) {
						for (int i1 = 1; i1 <= i; i1++)
						{
							for (int j1 = 1; j1 <= j; j1++)
							{
								if (s[i1][j1] == 1)
								{
									cct_gotoxy(4 + (length + 2) * (j1 - 1), 3 + (height + 1) * (i1 - 1));
									char content[10] = { '\0' };
									IntToChar(m[i1][j1], content);
									DrawOneBlock(length, height, m[i1][j1], COLOR_BLACK, 1, content);
								}
							}
						}
						break;
					}
				}
			}
			if (brk == 1)
				break;
		}	
	}
	CombineSame(i0, j0, m, s);
	for (int i1 = 1; i1 <= i; i1++)
	{
		for (int j1 = 1; j1 <= j; j1++)
		{
			if (m[i1][j1] == 0)
			{
				cct_gotoxy(4 + (length + 2) * (j1 - 1), 3 + (height + 1) * (i1 - 1));
				DrawOneBlock(length, height, COLOR_HWHITE, COLOR_HWHITE, 1, " ");				
			}
		}
	}
	for(int n=1;n<=3;n++)
	{
		cct_gotoxy(4 + (length + 2) * (j0 - 1), 3 + (height + 1) * (i0 - 1));
		char content[10] = { '\0' };
		IntToChar(m[i0][j0], content);
		DrawOneBlock(length, height, m[i0][j0], COLOR_HWHITE, 1, content);

		cct_gotoxy(4 + (length + 2) * (j0 - 1), 3 + (height + 1) * (i0 - 1));
		DrawOneBlock(length, height, m[i0][j0], COLOR_BLACK, 1, content);
	}
	score += AddScore(s, m[i0][j0]);
	cct_setcolor();
	cct_gotoxy(0, 0);
	cout << setfill(' ') << setw(50) << " ";
	cct_gotoxy(0, 0);
	cout << "���ε÷֣�" << AddScore(s, m[i0][j0]) << " �ܵ÷֣�" << score << " �ϳ�Ŀ�꣺" << aim << endl;
	if (choice == '8') {
		cct_setcolor();
		cct_gotoxy(0, 3 + (height + 1) * i);
		cout << "�ϳ���ɣ��س�/�����������0                    ";
		while (1)
		{
			int mx2, my2, ma2, kc12, kc22;
			int event2 = cct_read_keyboard_and_mouse(mx2, my2, ma2, kc12, kc22);
			if (ma2 == MOUSE_LEFT_BUTTON_CLICK || kc12 == 13)
				break;
		}
	}
	BlockFallDown2(m, s, i, j, length, height, 1, "mkten");
	if (choice == '8') {
		cct_setcolor();
		cct_gotoxy(0, 3 + (height + 1) * i);
		cout << "����0��ɣ��������/�س��������ֵ";

		while (1)
		{
			int mx2, my2, ma2, kc12, kc22;
			int event2 = cct_read_keyboard_and_mouse(mx2, my2, ma2, kc12, kc22);
			if (ma2 == MOUSE_LEFT_BUTTON_CLICK || kc12 == 13)
				break;
		}
	}
	int max=SearchForMax(m);
	AddNewNum(i, j, m, max);
	for (int i1 = 1; i1 <= i; i1++)
	{
		for (int j1 = 1; j1 <= j; j1++)
		{
			if (s[i1][j1] == 1)
			{
				cct_gotoxy(4 + (length + 2) * (j1 - 1), 3 + (height + 1) * (i1 - 1));
				char content[10] = { '\0' };
				IntToChar(m[i1][j1], content);
				DrawOneBlock(length, height, m[i1][j1], COLOR_BLACK, 1, content);
			}
		}
	}
	if (choice == '8') {
		cct_setcolor();
		cct_gotoxy(0, 3 + (height + 1) * i);
		cout << "���κϳɽ�������C/�������������һ�εĺϳ�" << endl;
		while (1)
		{
			int mx2, my2, ma2, kc12, kc22;
			int event2 = cct_read_keyboard_and_mouse(mx2, my2, ma2, kc12, kc22);
			if (ma2 == MOUSE_LEFT_BUTTON_CLICK || kc12 == 'C' || kc12 == 'c')
				break;
		}
	}
}

void CompleteGame(int i, int j,int length,int height, int m[10][12], int s[10][12], char choice, int &score,bool &quit, int aim)
{
	int max = aim - 1;
	while (1)
	{
		OneCombine(i, j, length, height, m, s, choice, score, quit, aim);
		if (quit == 1)
			return;
		if (SearchForMax(m) > max) {
			max = SearchForMax(m);
			cct_gotoxy(0, 3 + (height + 1) * i);
			cct_setcolor(COLOR_HYELLOW, COLOR_RED);
			cout << "�Ѿ��ϳɵ�" << SearchForMax(m);
			cct_setcolor();
			cout << ",���س���/����������������Ŀ�����    ";
			while (1)
			{
				int mx2, my2, ma2, kc12, kc22;
				int event2 = cct_read_keyboard_and_mouse(mx2, my2, ma2, kc12, kc22);
				if (ma2 == MOUSE_LEFT_BUTTON_CLICK || kc12 == 13)
					break;
			}
			cct_setcolor();
			cct_gotoxy(0, 3 + (height + 1) * i);
			cout << "                               " << endl;
			cout << "                                 ";
		}
		if (JudgeAll(m,j,i))
		{
			cct_gotoxy(0, 3 + (height + 1) * i);
			cct_setcolor();
			cout << "                                                  " << endl;
			cout << "                                 ";
			cct_gotoxy(0, 3 + (height + 1) * i);
			cct_setcolor(COLOR_BLACK, COLOR_HYELLOW);
			cout << "�޿ɺϲ������Ϸ����";
			cct_setcolor();
			cout << "��Q�˳�";			
			while (1)
			{
				int mx2, my2, ma2, kc12, kc22;
				int event2 = cct_read_keyboard_and_mouse(mx2, my2, ma2, kc12, kc22);
				if (kc12 == 'Q' || kc12 == 'q')
					break;
			}
			break;
		}
	}
}