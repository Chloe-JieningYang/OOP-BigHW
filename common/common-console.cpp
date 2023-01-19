/* 2050598 ����� ͨ�� */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <conio.h>
#include <cstring>
#include "..\include\cmd_console_tools.h"
#include "..\include\common-menu.h"
#include "..\include\common-judge-and-seek.h"
#include "..\include\common-move-matrix.h"
#include "..\include\common-print-matrix.h"
using namespace std;

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������λ��ת����i j��ֵ
  �����������ȡi,j�ı������������������鳤�ȣ��߶ȣ����޷ָ��ߣ���һ�β������ͣ�1��ʾ�ƶ����������2��ʾ���������س���0��ʾ��Ч������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int MousePositionAndKeyboardToIJ(int m[][12], int& I, int& J, int i, int j, int length, int height, bool splitline, int last = 0)
{
	cct_setcursor(CURSOR_INVISIBLE);
	int hefa = 0;
	int mx, my, maction, keycode1, keycode2;
	int& mx1 = mx, & my1 = my, & mac = maction, & k1 = keycode1, & k2 = keycode2;
	int event_ = cct_read_keyboard_and_mouse(mx1, my1, mac, k1, k2);
	int ii, jj;
	bool findj = false;

	if (event_ == CCT_KEYBOARD_EVENT) {
		if (keycode1 == 0xe0 && keycode2 == KB_ARROW_UP && m[I - 1][J] != 0) {
			I--;
			hefa = 1;
		}
		else if (keycode1 == 0xe0 && keycode2 == KB_ARROW_UP && m[I - 1][J] == 0)
		{
			I = i;
			hefa = 1;
		}
		else if (keycode1 == 0xe0 && keycode2 == KB_ARROW_DOWN && m[I+1][J]!=0)
		{
			I++;
			hefa = 1;
		}
		else if (keycode1 == 0xe0 && keycode2 == KB_ARROW_DOWN && m[I + 1][J] == 0)
		{
			for (int i1 = 1; i1 <= i; i1++)
			{
				if (m[i1][J] != 0)
				{
					I = i1;
					break;
				}
			}
			hefa = 1;
		}
		else if (keycode1 == 0xe0 && keycode2 == KB_ARROW_LEFT && m[I][J-1]!=0)
		{			
			J--;
			hefa = 1;
		}
		else if (keycode1 == 0xe0 && keycode2 == KB_ARROW_LEFT && m[I][J - 1] == 0)
		{
			bool find = 0;
			for (int j1 = J-1; j1 >= 1; j1--)
			{
				if (m[I][j1] != 0)
				{
					J = j1;
					find = 1;
					break;
				}
			}
			if (find == 0)
			{
				for (int j1 = j; j1 >= J; j1--)
				{
					if (m[I][j1] != 0)
					{
						J = j1;
						break;
					}
				}
			}
			hefa = 1;
		}
		else if (keycode1 == 0xe0 && keycode2 == KB_ARROW_RIGHT && m[I][J+1]!=0)
		{
			J++;
			hefa = 1;
		}
		else if (keycode1 == 0xe0 && keycode2 == KB_ARROW_RIGHT && m[I][J + 1] == 0)
		{
			bool find = 0;
			for (int j1 = J + 1; j1 <= j; j1++)
			{
				if (m[I][j1] != 0)
				{
					J = j1;
					find = 1;
					break;
				}
			}
			if (find == 0)
			{
				for (int j1 = 1; j1 <= J; j1++)
				{
					if (m[I][j1] != 0)
					{
						J = j1;
						break;
					}
				}
			}
			hefa = 1;
		}
		else if (keycode1 == 'q' || keycode1 == 'Q')
			return 3;
	}
	else if (event_ == CCT_MOUSE_EVENT) {
		if (splitline)
		{
			if (mx >= LEFTVACCOL && mx <= LEFTVACCOL-1 + (length + 2) * j && (mx - LEFTVACCOL) % (length + 2) <= length - 1) {
				jj = (mx - LEFTVACCOL) / (length + 2) + 1;
				findj = true;
			}
			if (findj == true) {
				if (my >= UPVACROW && my <= UPVACROW-2 + (height + 1) * i && (my - UPVACROW) % (height + 1) <= height - 1) {
					ii = (my - UPVACROW) / (height + 1) + 1;
					if (m[ii][jj] != 0)
						hefa = 1;
				}
			}
		}
		else
		{
			if (mx >= LEFTVACCOL && mx <= LEFTVACCOL-1 + length * j && (mx - LEFTVACCOL) % length <= length - 1) {
				jj = (mx - LEFTVACCOL) / length + 1;
				findj = true;
			}
			if (findj == true) {
				if (my >= UPVACROW && my <= UPVACROW-2 + height * i && (my - UPVACROW) % height <= height - 1) {
					ii = (my - UPVACROW) / height + 1;
					if (m[ii][jj] != 0)
						hefa = 1;
				}
			}
		}
		if (hefa == 1) {
			I = ii;
			J = jj;
		}
	}
	if (maction == MOUSE_LEFT_BUTTON_CLICK && last == 1)
		return 2;
	if (maction == MOUSE_RIGHT_BUTTON_CLICK)
		return 3;
	if (keycode1 == 13 && last == 1)
		return 2;
	return hefa;
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ�ѡ��һ�񲢷���,�����ء�ѡ���������
  �������������m�����У����У�ÿ�񳤶ȣ�ÿ��߶ȣ�����ѡ��������еı�����ѡ��Ƿ��зָ��ߣ��߿���ʽ��1ʵ�ģ�0���ģ�����Ϸ���֣��Ƿ��˳�
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void ChooseOneBlock(int m[][12], int i, int j, int length, int height, int& i0, int& j0, char choice, bool splitline, int style, const char game[], bool& quit)
{
	int i1, j1;//��ʱ���ѡ�еķ���
	int old_i1;
	int old_j1;
	for (int p = 1; p <= j; p++)//��
	{
		bool brk = 0;
		for (int q = 1; q <= i; q++)//��
			if (m[q][p] != 0)
			{
				i1 = q;
				old_i1 = q;
				j1 = p;
				old_j1 = p;
				brk = 1;
				break;
			}
		if (brk)
			break;
	}
	if (splitline)
		cct_gotoxy(LEFTVACCOL + (length + 2) * (old_j1 - 1), UPVACROW + (height + 1) * (old_i1 - 1));
	else
		cct_gotoxy(LEFTVACCOL + length * (old_j1 - 1), UPVACROW + height * (old_i1 - 1));
	if (!strcmp(game, "mkten"))
	{
		char content[10] = { '\0' };
		IntToChar(m[old_i1][old_j1], content);
		DrawOneBlock(length, height, m[old_i1][old_j1], COLOR_HWHITE, style, content);
	}
	else if (!strcmp(game, "erasestar"))
		DrawOneBlock(length, height, m[old_i1][old_j1], COLOR_HWHITE, style, "��");
	int mk1, mk2;
	mk2 = 1;
	while (1)
	{
		cct_enable_mouse();
		mk1 = MousePositionAndKeyboardToIJ(m, i1, j1, i, j, length, height, splitline, mk2);
		if (mk1 == 0)//�Ƿ�����
		{
			if (splitline)
				cct_gotoxy(LEFTVACCOL + (length + 2) * (old_j1 - 1), UPVACROW + (height + 1) * (old_i1 - 1));
			else
				cct_gotoxy(LEFTVACCOL + length * (old_j1 - 1), UPVACROW + height * (old_i1 - 1));
			
			if (!strcmp(game, "mkten"))
			{
				char content[10] = { '\0' };
				IntToChar(m[old_i1][old_j1], content);
				DrawOneBlock(length, height, m[old_i1][old_j1], COLOR_BLACK, style, content);
			}
			else if (!strcmp(game, "erasestar"))
				DrawOneBlock(length, height, m[old_i1][old_j1], COLOR_BLACK, style, "��");
			if (splitline)
				cct_gotoxy(0, UPVACROW + (height + 1) * i);
			else
				cct_gotoxy(0, UPVACROW + height * i + 1);
			cct_setcolor();
			cout << "[��ǰ���] λ�÷Ƿ�";
		}
		else if (mk1 == 1 && (i1 != old_i1 || j1 != old_j1 || mk2 == 0))//�Ϸ����������ϴη���λ�ò�ͬ
		{
			if (splitline)
				cct_gotoxy(LEFTVACCOL + (length + 2) * (old_j1 - 1), UPVACROW + (height + 1) * (old_i1 - 1));
			else
				cct_gotoxy(LEFTVACCOL + length * (old_j1 - 1), UPVACROW + height * (old_i1 - 1));
			if (!strcmp(game, "mkten"))
			{
				char content[10] = { '\0' };
				IntToChar(m[old_i1][old_j1], content);
				DrawOneBlock(length, height, m[old_i1][old_j1], COLOR_BLACK, style, content);
			}
			else if (!strcmp(game, "erasestar"))
				DrawOneBlock(length, height, m[old_i1][old_j1], COLOR_BLACK, style, "��");
			if (splitline)
				cct_gotoxy(LEFTVACCOL + (length + 2) * (j1 - 1), UPVACROW + (height + 1) * (i1 - 1));
			else
				cct_gotoxy(LEFTVACCOL + length * (j1 - 1), UPVACROW + height * (i1 - 1));
			if (!strcmp(game, "mkten"))
			{
				char content[10] = { '\0' };
				IntToChar(m[i1][j1], content);
				DrawOneBlock(length, height, m[i1][j1], COLOR_HWHITE, style, content);
			}
			else if (!strcmp(game, "erasestar"))
				DrawOneBlock(length, height, m[i1][j1], COLOR_HWHITE, style, "��");
			if (splitline)
				cct_gotoxy(0, UPVACROW + (height + 1) * i);
			else
				cct_gotoxy(0, UPVACROW + height * i + 1);
			cct_setcolor();
			cout << "[��ǰ���] " << char('A' + i1 - 1) << "��" << j1 - 1 << "��                               ";
			old_j1 = j1;
			old_i1 = i1;
		}
		else if (mk1 == 2) {//ѡ��
			i0 = i1;
			j0 = j1;
			break;
		}
		else if (mk1 == 3)//�˳�
		{
			quit = 1;
			return;
		}
		mk2 = mk1;
	}
	cct_setcolor();
	if (splitline)
		cct_gotoxy(0, UPVACROW + (height + 1) * i);
	else
		cct_gotoxy(0, UPVACROW + height * i + 1);
	if (!strcmp(game, "mkten"))
	{
		if (choice == '7' && quit == 0)
		{
			cout << "ѡ����" << char('A' + i0 - 1) << "��" << j0 - 1 << "��                    " << endl;
		}
	}
	else if (!strcmp(game, "erasestar"))
	{
		if ((choice == 'D' || choice == 'd' || choice == 'E' || choice == 'e') && quit == 0)
			cout << "ѡ����" << char('A' + i0 - 1) << "��" << j0 - 1 << "��                    " << endl;
	}
}