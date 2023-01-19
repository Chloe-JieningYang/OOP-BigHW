/* 2050598 杨洁宁 通信 */
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <conio.h>
#include <cstring>
#include "..\include\cmd_console_tools.h"
#include "90-02-b1.h"
#include "..\include\common-menu.h"
#include "..\include\common-judge-and-seek.h"
#include "..\include\common-move-matrix.h"
#include "..\include\common-print-matrix.h"
#include "..\include\common-console.h"
using namespace std;

void OneErase(int i, int j, int length, int height, int m[][12], int s[][12], char choice,bool splitline, int& score, bool& qui)
{
	int i0, j0;
	bool brk = 0;
	while (1)
	{
		for (int p = 0; p <= 11; p++)
			for (int q = 0; q <= 11; q++)
				s[p][q] = 0;
		ChooseOneBlock(m, i, j, length, height, i0, j0, choice, splitline, 0, "erasestar", qui);
		if (qui == 1)
			return;
		if (!JudgeRound(m, i0, j0))
		{
			if (splitline)
				cct_gotoxy(0, UPVACROW + (height + 1) * i);
			else
				cct_gotoxy(0, UPVACROW + height * i + 1);
			cct_setcolor(COLOR_BLACK, COLOR_HYELLOW);
			cout << "周围无相同值";
			cct_setcolor();
			cout << "箭头键/鼠标移动，回车键/单击左键选择";
			while (1)
			{
				int I, J;
				int event_ = MousePositionAndKeyboardToIJ(m, I, J, i, j, length, height, splitline, 1);
				if (I != i0 || J != j0 || event_ == 0)
				{					
					if (splitline)
						cct_gotoxy(LEFTVACCOL + (length + 2) * (j0 - 1), UPVACROW + (height + 1) * (i0 - 1));
					else
						cct_gotoxy(LEFTVACCOL + length * (j0 - 1), UPVACROW + height * (i0 - 1));
					DrawOneBlock(length, height, m[i0][j0], COLOR_BLACK, 0, "★");
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
					if (s[i1][j1] == 1 && (i1 != i0 || j1 != j0))
					{
						if (splitline)
							cct_gotoxy(LEFTVACCOL + (length + 2) * (j1 - 1), UPVACROW + (height + 1) * (i1 - 1));
						else
							cct_gotoxy(LEFTVACCOL + length * (j1 - 1), UPVACROW + height * (i1 - 1));
						DrawOneBlock(length, height, m[i1][j1], COLOR_WHITE, 0, "★");
					}
				}
			}
			if (splitline)
				cct_gotoxy(0, UPVACROW + (height + 1) * i);
			else
				cct_gotoxy(0, UPVACROW + height * i + 1);
			cct_setcolor();
			cout << "箭头键/鼠标移动取消当前选择，回车键/单击左键合成";
			int J, I;
			while (1)
			{
				int event_ = MousePositionAndKeyboardToIJ(m, I, J, i, j, length, height, splitline, 1);
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
									if (splitline)
										cct_gotoxy(LEFTVACCOL + (length + 2) * (j1 - 1), UPVACROW + (height + 1) * (i1 - 1));
									else
										cct_gotoxy(LEFTVACCOL + length * (j1 - 1), UPVACROW + height * (i1 - 1));
									DrawOneBlock(length, height, m[i1][j1], COLOR_BLACK, 0, "★");
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
	EraseSame(i0, j0, m, s);
	for (int i1 = 1; i1 <= i; i1++)
	{
		for (int j1 = 1; j1 <= j; j1++)
		{
			if (m[i1][j1] == 0)
			{
				if (splitline)
					cct_gotoxy(LEFTVACCOL + (length + 2) * (j1 - 1), UPVACROW + (height + 1) * (i1 - 1));
				else
					cct_gotoxy(LEFTVACCOL + length * (j1 - 1), UPVACROW + height * (i1 - 1));
				DrawOneBlock(length, height, COLOR_HWHITE, COLOR_HWHITE, 0, " ");
			}
		}
	}
	score += AddScore(s, m[i0][j0]);
	cct_setcolor();
	cct_gotoxy(0, 0);
	cout << setfill(' ') << setw(50) << " ";
	cct_gotoxy(0, 0);
	cout << "本次得分：" << AddScore(s, m[i0][j0]) << " 总分：" << score;
	if (choice == 'F' || choice == 'f')
	{
		cct_setcolor();
		if (splitline)
			cct_gotoxy(0, UPVACROW + (height + 1) * i);
		else
			cct_gotoxy(0, UPVACROW + height * i + 1);
		cout << "合成完成，回车/单击左键下落0                    ";
		while (1)
		{
			int mx2, my2, ma2, kc12, kc22;
			int event2 = cct_read_keyboard_and_mouse(mx2, my2, ma2, kc12, kc22);
			if (ma2 == MOUSE_LEFT_BUTTON_CLICK || kc12 == 13)
				break;
		}
	}
	BlockFallDown2(m, s, i, j, length, height, splitline, "erasestar");
	ColBlockMoveLeft(m, s, i, j, length, height, splitline, 0);
	if (choice == 'F'||choice=='f') {
		cct_setcolor();
		if (splitline)
			cct_gotoxy(0, UPVACROW + (height + 1) * i);
		else
			cct_gotoxy(0, UPVACROW + height * i + 1);
		cout << "本次合成结束，按C/单击左键继续新一次的合成" << endl;
		while (1)
		{
			int mx2, my2, ma2, kc12, kc22;
			int event2 = cct_read_keyboard_and_mouse(mx2, my2, ma2, kc12, kc22);
			if (ma2 == MOUSE_LEFT_BUTTON_CLICK || kc12 == 'C' || kc12 == 'c')
				break;
		}
	}
}

void CompleteGame(int i, int j, int length, int height, int m[][12], int s[][12],bool splitline, char choice, int& score, bool& quit)
{
	while (1)
	{
		OneErase(i, j, length, height, m, s, choice, splitline, score, quit);
		if (quit == 1)
			return;
		if (JudgeAll(m, j, i))
		{
			cct_setcolor();
			cct_gotoxy(0, 0);
			int awardscore = AwardScore(m, i, j);
			score += awardscore;
			cout << "奖励得分:" << awardscore << " 总分:" << score << "      ";
			if (splitline)
				cct_gotoxy(0, UPVACROW + (height + 1) * i);
			else
				cct_gotoxy(0, UPVACROW + height * i + 1);
			cct_setcolor();
			cout << "                                                  " << endl;
			cout << "                                 ";
			if (splitline)
				cct_gotoxy(0, UPVACROW + (height + 1) * i);
			else
				cct_gotoxy(0, UPVACROW + height * i + 1);
			cct_setcolor(COLOR_BLACK, COLOR_HYELLOW);
			cout << "无可消除的项，游戏结束";
			cct_setcolor();
			cout << "回车继续下一关";
			while (1)
			{
				int mx2, my2, ma2, kc12, kc22;
				int event2 = cct_read_keyboard_and_mouse(mx2, my2, ma2, kc12, kc22);
				if (kc12 == 13)
				{										
					break;
				}
			}
			break;
		}
	}
}