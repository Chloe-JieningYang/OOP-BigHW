/* 2050598 杨洁宁 通信 */
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
  函数名称：
  功    能：将鼠标的位置转化成i j的值
  输入参数：获取i,j的变量，总行列数，方块长度，高度，有无分隔线，上一次操作类型（1表示移动鼠标或方向键，2表示按下左键或回车，0表示无效操作）
  返 回 值：
  说    明：
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
  函数名称：
  功    能：选中一格并反显,“返回”选择的行列数
  输入参数：数组m，总行，总列，每格长度，每格高度，接收选择格子行列的变量，选项，是否有分隔线，边框样式（1实心，0空心），游戏名字，是否退出
  返 回 值：
  说    明：
***************************************************************************/
void ChooseOneBlock(int m[][12], int i, int j, int length, int height, int& i0, int& j0, char choice, bool splitline, int style, const char game[], bool& quit)
{
	int i1, j1;//临时标记选中的方块
	int old_i1;
	int old_j1;
	for (int p = 1; p <= j; p++)//列
	{
		bool brk = 0;
		for (int q = 1; q <= i; q++)//行
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
		DrawOneBlock(length, height, m[old_i1][old_j1], COLOR_HWHITE, style, "★");
	int mk1, mk2;
	mk2 = 1;
	while (1)
	{
		cct_enable_mouse();
		mk1 = MousePositionAndKeyboardToIJ(m, i1, j1, i, j, length, height, splitline, mk2);
		if (mk1 == 0)//非法区域
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
				DrawOneBlock(length, height, m[old_i1][old_j1], COLOR_BLACK, style, "★");
			if (splitline)
				cct_gotoxy(0, UPVACROW + (height + 1) * i);
			else
				cct_gotoxy(0, UPVACROW + height * i + 1);
			cct_setcolor();
			cout << "[当前鼠标] 位置非法";
		}
		else if (mk1 == 1 && (i1 != old_i1 || j1 != old_j1 || mk2 == 0))//合法区域且与上次方块位置不同
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
				DrawOneBlock(length, height, m[old_i1][old_j1], COLOR_BLACK, style, "★");
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
				DrawOneBlock(length, height, m[i1][j1], COLOR_HWHITE, style, "★");
			if (splitline)
				cct_gotoxy(0, UPVACROW + (height + 1) * i);
			else
				cct_gotoxy(0, UPVACROW + height * i + 1);
			cct_setcolor();
			cout << "[当前鼠标] " << char('A' + i1 - 1) << "行" << j1 - 1 << "列                               ";
			old_j1 = j1;
			old_i1 = i1;
		}
		else if (mk1 == 2) {//选中
			i0 = i1;
			j0 = j1;
			break;
		}
		else if (mk1 == 3)//退出
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
			cout << "选中了" << char('A' + i0 - 1) << "行" << j0 - 1 << "列                    " << endl;
		}
	}
	else if (!strcmp(game, "erasestar"))
	{
		if ((choice == 'D' || choice == 'd' || choice == 'E' || choice == 'e') && quit == 0)
			cout << "选中了" << char('A' + i0 - 1) << "行" << j0 - 1 << "列                    " << endl;
	}
}