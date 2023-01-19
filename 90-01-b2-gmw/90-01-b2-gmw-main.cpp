/* 2050598 ����� ͨ�� */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <stdio.h>  
#include <stdlib.h>  
#include <conio.h>  
#include <math.h>
#include <time.h>
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
#include "90-01-b2-gmw.h"
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

int main()
{
	srand((unsigned)time(NULL));
	CONSOLE_GRAPHICS_INFO Mto10_CGI;
	MATRIX_SIGN mat;
	int col_num, row_num, aim;
	while (1)
	{
		int score = 0;
		bool quit = 0;
		for (int p = 0; p <= 9; p++)
		{
			for (int q = 0; q <= 11; q++) {
				mat.matrix[p][q] = 0;
				mat.sign[p][q] = 0;
			}
		}//��ʼ��mat���� ����

		gmw_init(&Mto10_CGI);//��ʼ��������������

		InputRowCol(col_num, row_num);//��������
		InputAim(aim);//����Ŀ��

		gmw_set_rowcol(&Mto10_CGI, row_num, col_num);//��������
		gmw_set_frame_style(&Mto10_CGI, 6, 3, false);//���ÿ��
		gmw_set_block_border_switch(&Mto10_CGI, true);//ɫ��߿�
		gmw_set_block_default_linetype(&Mto10_CGI, 2);//ɫ��߿�����
		gmw_set_colno_switch(&Mto10_CGI, true);//�кſ�
		gmw_set_rowno_switch(&Mto10_CGI, true);//�кſ�
		gmw_set_font(&Mto10_CGI, "������", 20);//��������
		gmw_set_frame_color(&Mto10_CGI, COLOR_HWHITE, COLOR_BLACK);//������Ϸ����ɫ

		CreatMatrix(Mto10_CGI.row_num, Mto10_CGI.col_num, mat.matrix);

		gmw_draw_frame(&Mto10_CGI);//����Ϸ�������

		char temp[256];
		/* ��״̬����ʾ���� */
		sprintf(temp, "��Ļ��ǰ����Ϊ��%d�� %d��", Mto10_CGI.lines, Mto10_CGI.cols);
		gmw_status_line(&Mto10_CGI, TOP_STATUS_LINE, temp);

		CompleteGame(&Mto10_CGI, &mat, score, aim, quit);
		
		if (quit == 1)
			break;
	}

	return 0;
}