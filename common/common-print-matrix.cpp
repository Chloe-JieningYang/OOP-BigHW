/* 2050598 ����� ͨ�� */
#define _CRT_SECURE_NO_WARNINGS
#define MAX 10

#include <iostream>
#include "..\include\cmd_console_tools.h"
#include "..\include\common-print-matrix.h"
#include <cstring>
#include <iomanip>
#include <Windows.h>
#include <ctime>
#include <conio.h>
#include <cstdio>

using namespace std;

/***************************************************************************
  �������ƣ�
  ��    �ܣ������������ʽ������
  ������������У����У�m���飬������飬�Ƿ�Ҫ�û�ɫ��ʾ���
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void OutputMatrixArray(int i, int j, int m[][12], int s[][12], bool sign)
{
	cout << setfill(' ') << setw(3) << "|";
	int c;
	for (c = 0; c <= j - 1; c++)
		cout << setw(3) << c;
	cout << endl;
	cout << "--+";
	cout << setfill('-') << setw(3 * j + 1) << "-" << endl;
	int r;
	for (r = 1; r <= i; r++)
	{
		cout << char(r - 1 + 'A') << " |";
		for (c = 1; c <= j; c++)
		{
			if (sign)
			{
				cct_setcolor();
				cout << "  ";
			
				if (s[r][c] == 0)
				{
					cct_setcolor();
					cout << m[r][c];
				}
				else
				{
					cct_setcolor(COLOR_HYELLOW, COLOR_BLACK);
					cout << m[r][c];
				}			
			}
			else
				cout << setfill(' ') << setw(3) << m[r][c];
		}
		cct_setcolor();
		cout << endl;
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����αͼ����߿�
  ������������淽��ĳ����ߣ�����������������ʵ�ģ�1��or���ģ�0�������޼����
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void DrawOutLine(int length, int height, int j, int i, int style, bool splitline)
{
	if (splitline == 0)
	{
		cct_setconsoleborder(LEFTVACCOL +3 + length * j, height * i + UPVACROW + 5);//��һ��Ҫ�ĳ�+8
		cct_setcolor();
		cout << "��Ļ��ǰ����Ϊ��" << height * i + UPVACROW + 5 << "��" << LEFTVACCOL + 3 + length * j << "��" << endl;
		cout << " ";
		int j1, m1;
		cout << resetiosflags(ios::left);
		for (j1 = 0; j1 <= j - 1; j1++)
			cout << setiosflags(ios::right) << setfill(' ') << setw(length) << j1;
		cout << endl;
		cout << "  ";
		cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
		if (style == 1)
			cout << "��";
		else if (style == 0)
			cout << "�X";
		for (m1 = 1; m1 <= length * j / 2; m1++)
		{
			if (style == 1)
				cout << "��";
			else if (style == 0)
				cout << "�T";
		}
		if (style == 1)
			cout << "��";
		else if (style == 0)
			cout << "�[";
		cout << endl;
		for (int i2 = 1; i2 <= height * i; i2++)
		{
			cct_setcolor();
			if ((i2 - 2) % height == 0)
			{
				cout << resetiosflags(ios::right);
				cout << setiosflags(ios::left) << setw(2) << char((i2 - 2) / height + 'A');
			}
			else
				cout << "  ";
			cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
			if (style == 1)
				cout << "��";
			else if (style == 0)
				cout << "�U";
			for (int j2 = 1; j2 <= length * j; j2++)
			{
				cout << ' ';
			}
			if (style == 1)
				cout << "��" << endl;
			else if (style == 0)
				cout << "�U" << endl;
			Sleep(1);
		}
		cct_setcolor();
		cout << "  ";
		cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
		if (style == 1)
			cout << "��";
		else if (style == 0)
			cout << "�^";
		for (int j3 = 1; j3 <= length * j / 2; j3++)
		{
			if (style == 1)
				cout << "��";
			else if (style == 0)
				cout << "�T";
		}
		if (style == 1)
			cout << "��";
		else if (style == 0)
			cout << "�a";
		cout << endl;
		cct_setcolor();
		cout << endl;
	}
	else
	{
		cct_setconsoleborder(LEFTVACCOL+1+j*(length+2), UPVACROW+4 + (height+1) * i, LEFTVACCOL + 1 + j * (length + 2), UPVACROW + 4 + (height + 1) * i+ 1);
		cct_setcolor();
		cout << "��Ļ��ǰ����Ϊ��" << UPVACROW + 4 + (height + 1) * i << "��" << LEFTVACCOL + 1 + j * (length + 2) << "��" << endl;
		int j1;		
		cout << setfill(' ') << setw(LEFTVACCOL + 2 + (length - 4) / 2 - 1) << " ";
		cout << "0";
		cout << resetiosflags(ios::left);
		for (j1 = 1; j1 <= j - 1; j1++)
			cout << setiosflags(ios::right) << setfill(' ') << setw(length + 2) << j1;
		cout << endl;
		cout << "  ";
		cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
		if (style == 1)
			cout << "��";
		else if (style == 0)
			cout << "�X";
		for (j1 = 1; j1 <= j - 1; j1++)
		{
			if (style == 1)
			{
				for (int n1 = 1; n1 <= length / 2; n1++)
					cout << "��";
				cout << "��";
			}
			else if (style == 0)
			{
				for (int n1 = 1; n1 <= length / 2; n1++)
					cout << "�T";
				cout << "�j";
			}
		}
		if (style == 1)
		{
			for (int n1 = 1; n1 <= length / 2; n1++)
				cout << "��";
			cout << "��";
		}
		else if (style == 0)
		{
			for (int n1 = 1; n1 <= length / 2; n1++)
				cout << "�T";
			cout << "�[";
		}
		cout << endl;
		for (int i2 = 1; i2 <= i; i2++)
		{		
			for (int n2 = 1; n2 <= height ; n2++)
			{
				cct_setcolor();
				if (n2 == (height + 1) / 2)
				{
					cout << resetiosflags(ios::right);
					cout << setw(2) << setiosflags(ios::left) << char('A' + i2 - 1);
				}
				else
					cout << "  ";
				cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
				if (style == 1)
					cout << "��";
				else if (style == 0)
					cout << "�U";
				for (int j3 = 1; j3 <= j; j3++)
				{
					cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
					cout << setw(length) << " ";
					if (style == 1)
						cout << "��";
					else if (style == 0)
						cout << "�U";
				}
				cout << endl;
			}
			cct_setcolor();
			cout << "  ";
			cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
			if (i2 != i)
			{
				if (style == 1)
					cout << "��";
				else if (style == 0)
					cout << "�d";
			}
			else
			{
				if (style == 1)
					cout << "��";
				else if (style == 0)
					cout << "�^";
			}
			for (int j4 = 1; j4 <= j; j4++)
			{
				for (int n3 = 1; n3 <= length / 2; n3++)
				{
					if (style == 1)
						cout << "��";
					else if (style == 0)
						cout << "�T";
				}
				if (j4 != j)
				{
					if (i2 != i)
					{
						if (style == 1)
							cout << "��";
						else if (style == 0)
							cout << "�p";
					}
					else
					{
						if (style == 1)
							cout << "��";
						else if (style == 0)
							cout << "�m";
					}
				}
				else
				{
					if (i2 != i)
					{
						
						if (style == 1)
							cout << "��" << endl;
						else if (style == 0)
							cout << "�g" << endl;
					}
					else {
						if (style == 1)
							cout << "��" << endl;
						else if (style == 0)
							cout << "�a" << endl;
					}
				}
			}
		}
	}
}

void DrawOneBlock(int length, int height, int backcolor, int frontcolor, int style,const char content[])
{
	cct_setcolor(backcolor, frontcolor);
	
	if (style == 1)
	{
		int x, y;
		cout << "��";
		for (int i = 1; i <= (length- 4) / 2; i++)
			cout << "��";
		cout << "��";
		for (int i1 = 2; i1 <= height - 1; i1++)
		{
			cct_getxy(x, y);
			cct_gotoxy(x - length, y + 1);
			cout << "��";
			if (i1 == (height + 1) / 2)
			{
				if ((length - (strlen(content) + 1) / 2 * 2 - 4) / 2 > 0)
				{
					cout << setw((length - (strlen(content) + 1) / 2 * 2 - 4) / 2) << ' ';

				}
				cout <<setiosflags(ios::left) << setw((strlen(content) + 1) / 2 * 2) << content;
				if ((length - (strlen(content) + 1) / 2 * 2 - 4) / 2 > 0)
				{
					cout << setw((length - (strlen(content) + 1) / 2 * 2 - 4) / 2) << ' ';
				}
			}
			else
			{
				cout << setw(length - 4) << ' ';
			}
			cout << "��";
		}	
		cct_getxy(x, y);
		cct_gotoxy(x - length, y + 1);
		cout << "��";
		for (int i = 1; i <= (length - 4) / 2; i++)
			cout << "��";
		cout << "��";
	}
	else if (style == 0)
	{
		int x, y;
		cout << "�X";
		for (int i = 1; i <= (length - 4) / 2; i++)
			cout << "�T";
		cout << "�[";
		for (int i1 = 2; i1 <= height - 1; i1++)
		{
			cct_getxy(x, y);
			cct_gotoxy(x - length, y + 1);
			cout << "�U";
			if (i1 == (height + 1) / 2)
			{
				if ((length - (strlen(content) + 1) / 2 * 2 - 4) / 2 > 0)
				{
					cout << setw((length - (strlen(content) + 1) / 2 * 2 - 4) / 2) << ' ';

				}
				cout << setiosflags(ios::left) << setw((strlen(content) + 1) / 2 * 2) << content;
				if((length - (strlen(content) + 1) / 2 * 2 - 4) / 2 > 0)
				{
					cout << setw((length - (strlen(content) + 1) / 2 * 2 - 4) / 2) << ' ';
				}
			}
			else
			{
				cout << setw(length - 4) << ' ';
			}
			cout << "�U";
		}
		cct_getxy(x, y);
		cct_gotoxy(x - length, y + 1);
		cout << "�^";
		for (int i = 1; i <= (length - 4) / 2; i++)
			cout << "�T";
		cout << "�a";
	}
	else if (style == -1)
	{
		int x, y;
		for (int h1 = 1; h1 <= height; h1++)
		{
			for (int l1 = 1; l1 <= length; l1++)
			{
				cct_setcolor(backcolor, backcolor);
				cout << " ";				
			}
			cct_getxy(x, y);
			cct_gotoxy(x - length, y + 1);
		}
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��������ڵ�int����ת���ַ�����ʽ
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void IntToChar(int number, char content[])
{
	char temp[MAX] = { '\0' };
	int i = 0;
	while (number)
	{
		temp[i] = number % 10 + '0';
		i++;
		number /= 10;
	}
	int j = 0;
	for (int i1 = i - 1; i1 >= 0; i1--)
	{
		content[j] = temp[i1];
		j++;
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����һ��������
  ������������淽��ĳ����ߣ�����ʵ�ģ�1��or���ģ�0������Ϸ����mkten/erasestar��,���������������޼���ߣ����飬��������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void DrawCompleteMatrix(int length, int height, int style,const char game[], int j, int i, bool splitline,int m[][12], const char content[]="\0")
{
	DrawOutLine(length, height, j, i, style, splitline);
	for (int i1 = 1; i1 <= i; i1++)
	{
		for (int j1 = 1; j1 <= j; j1++)
		{
			if (splitline)
			{
				cct_gotoxy(4 + (j1 - 1) * (length + 2), 3 + (i1 - 1) * (height + 1));				
				if (!strcmp(game, "mkten"))
				{
					char realcontent[MAX] = { '\0' };
					IntToChar(m[i1][j1], realcontent);
					DrawOneBlock(length, height, m[i1][j1], COLOR_BLACK, style, realcontent);
				}
				else if (!strcmp(game, "erasestar"))
					DrawOneBlock(length, height, m[i1][j1], COLOR_BLACK, style, content);
			}
			else
			{
				cct_gotoxy(4 + (j1 - 1) * length, 3 + (i1 - 1) * height);								
				if (!strcmp(game, "mkten"))
				{
					char realcontent[MAX] = { '\0' };
					IntToChar(m[i1][j1], realcontent);
					DrawOneBlock(length, height, m[i1][j1], COLOR_BLACK, style, realcontent);
				}
				else if (!strcmp(game, "erasestar"))
					DrawOneBlock(length, height, m[i1][j1], COLOR_BLACK, style, content);
			}
		}
	}
}