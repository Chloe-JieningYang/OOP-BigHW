/* 2050598 ����� ͨ�� */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <conio.h>
#include <cstring>
#include "..\include\cmd_console_tools.h"
#include "..\include\cmd_gmw_tools.h"
#include "90-01-b2-gmw.h"
#include "..\include\common-judge-and-seek.h"
#include "..\include\common-move-matrix.h"

using namespace std;

#define CONFIRM_CHOOSE    50
#define ARROW             51
#define MOUSE_MOVE        52
#define QUIT              53

void BlockFallDown(const CONSOLE_GRAPHICS_INFO* pCGI, MATRIX_SIGN *mat, const BLOCK_DISPLAY_INFO* bdi_normal)
{
	for (int j = 1; j <= pCGI->row_num; j++)
	{
		int count0 = 0;
		for (int i = pCGI->row_num; i >= 1; i--)
		{
			if (mat->matrix[i][j] == 0)
				count0++;
			else if (count0 != 0)
				gmw_move_block(pCGI, i - 1, j - 1, mat->matrix[i][j], 0, bdi_normal, UP_TO_DOWN, count0);
		}
	}
	
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ�ͨ�������̸ı�I,J,quit��ֵ�����Ҽ���q�˳�
  ���������
  �� �� ֵ�����������������MOUSE_LEFT_BUTTON_CLICK�����෵��0
  ˵    �����������I��J��1��ʼ��������
***************************************************************************/
int MoveOrChooseOneBlock(int m[][12], const CONSOLE_GRAPHICS_INFO* pCGI, int& I, int& J, const BLOCK_DISPLAY_INFO* bdi_normal, const BLOCK_DISPLAY_INFO* bdi_selected, bool& quit, bool once)
{
	int row = pCGI->row_num;
	int col = pCGI->col_num;
	int mac, mrow, mcol, keycode1, keycode2;
	mrow = I - 1;
	mcol = J - 1;
	while (1)
	{

		int event = gmw_read_keyboard_and_mouse(pCGI, mac, mrow, mcol, keycode1, keycode2, true);
		//���ص�mrow,mcol��0��ʼ��������
		if (event == CCT_KEYBOARD_EVENT)
		{
			if (keycode1 == 0xe0 && keycode2 == KB_ARROW_UP && m[I - 1][J] != 0)//������ϼ���Ŀǰ�������Ϸ�
			{
				gmw_draw_block(pCGI, I - 1, J - 1, m[I][J], bdi_normal);
				I--;
				gmw_draw_block(pCGI, I - 1, J - 1, m[I][J], bdi_selected);
				char temp[256] = { '\0' };
				sprintf(temp, "[��ǰ���] %c��%d��", char(I - 1 + 'A'), J - 1);
				gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
				return ARROW;
			}
			else if (keycode1 == 0xe0 && keycode2 == KB_ARROW_UP && m[I - 1][J] == 0)//���ϼ���Ŀǰ�����Ϸ����ص����·�
			{
				gmw_draw_block(pCGI, I - 1, J - 1, m[I][J], bdi_normal);
				I = row;
				gmw_draw_block(pCGI, I - 1, J - 1, m[I][J], bdi_selected);
				char temp[256] = { '\0' };
				sprintf(temp, "[��ǰ���] %c��%d��", char(I - 1 + 'A'), J - 1);
				gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
				return ARROW;
			}
			else if (keycode1 == 0xe0 && keycode2 == KB_ARROW_DOWN && m[I + 1][J] != 0)//���¼���Ŀǰ�������·�
			{
				gmw_draw_block(pCGI, I - 1, J - 1, m[I][J], bdi_normal);
				I++;
				gmw_draw_block(pCGI, I - 1, J - 1, m[I][J], bdi_selected);
				char temp[256] = { '\0' };
				sprintf(temp, "[��ǰ���] %c��%d��", char(I - 1 + 'A'), J - 1);
				gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
				return ARROW;
			}
			else if (keycode1 == 0xe0 && keycode2 == KB_ARROW_DOWN && m[I + 1][J] == 0)//���¼���Ŀǰ�����·�
			{
				gmw_draw_block(pCGI, I - 1, J - 1, m[I][J], bdi_normal);
				I = 1;
				gmw_draw_block(pCGI, I - 1, J - 1, m[I][J], bdi_selected);
				char temp[256] = { '\0' };
				sprintf(temp, "[��ǰ���] %c��%d��", char(I - 1 + 'A'), J - 1);
				gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
				return ARROW;
			}
			else if (keycode1 == 0xe0 && keycode2 == KB_ARROW_LEFT && m[I][J - 1] != 0)//�������Ŀǰ���������
			{
				gmw_draw_block(pCGI, I - 1, J - 1, m[I][J], bdi_normal);
				J--;
				gmw_draw_block(pCGI, I - 1, J - 1, m[I][J], bdi_selected);
				char temp[256] = { '\0' };
				sprintf(temp, "[��ǰ���] %c��%d��", char(I - 1 + 'A'), J - 1);
				gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
				return ARROW;
			}
			else if (keycode1 == 0xe0 && keycode2 == KB_ARROW_LEFT && m[I][J - 1] == 0)//�������Ŀǰ�������
			{
				gmw_draw_block(pCGI, I - 1, J - 1, m[I][J], bdi_normal);
				J = col;
				gmw_draw_block(pCGI, I - 1, J - 1, m[I][J], bdi_selected);
				char temp[256] = { '\0' };
				sprintf(temp, "[��ǰ���] %c��%d��", char(I - 1 + 'A'), J - 1);
				gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
				return ARROW;
			}
			else if (keycode1 == 0xe0 && keycode2 == KB_ARROW_RIGHT && m[I][J + 1] != 0)//���Ҽ���Ŀǰ�������ұ�
			{
				gmw_draw_block(pCGI, I - 1, J - 1, m[I][J], bdi_normal);
				J++;
				gmw_draw_block(pCGI, I - 1, J - 1, m[I][J], bdi_selected);
				char temp[256] = { '\0' };
				sprintf(temp, "[��ǰ���] %c��%d��", char(I - 1 + 'A'), J - 1);
				gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
				return ARROW;
			}
			else if (keycode1 == 0xe0 && keycode2 == KB_ARROW_RIGHT && m[I][J + 1] == 0)//���Ҽ���Ŀǰ�����ұ�
			{
				gmw_draw_block(pCGI, I - 1, J - 1, m[I][J], bdi_normal);
				J = 1;
				gmw_draw_block(pCGI, I - 1, J - 1, m[I][J], bdi_selected);
				char temp[256] = { '\0' };
				sprintf(temp, "[��ǰ���] %c��%d��", char(I - 1 + 'A'), J - 1);
				gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
				return ARROW;
			}
			else if (keycode1 == 'Q' || keycode1 == 'q')
			{
				quit = 1;
				return QUIT;
			}
			else if (keycode1 == 13)
				return CONFIRM_CHOOSE;
		}
		else if (event == CCT_MOUSE_EVENT && mac == MOUSE_ONLY_MOVED)
		{
			gmw_draw_block(pCGI, I - 1, J - 1, m[I][J], bdi_normal);
			I = mrow + 1;//��ΪI,J���Ǵ�1��ʼ��
			J = mcol + 1;
			gmw_draw_block(pCGI, I - 1, J - 1, m[I][J], bdi_selected);
			if (once)
				return MOUSE_MOVE;
		}
		else if (event == CCT_MOUSE_EVENT)
		{
			if (mac == MOUSE_LEFT_BUTTON_CLICK)
				return CONFIRM_CHOOSE;
			else if (mac == MOUSE_RIGHT_BUTTON_CLICK)
			{
				quit = 1;
				return QUIT;
			}
		}
	}
	return 0;
}

void OneCombine(const CONSOLE_GRAPHICS_INFO* pCGI, MATRIX_SIGN* mat,const BLOCK_DISPLAY_INFO* bdi_normal, const BLOCK_DISPLAY_INFO* bdi_related, const BLOCK_DISPLAY_INFO* bdi_selected, int& score, const int aim, bool& quit)
{
	int i0 = 1;
	int j0 = 1;
	gmw_draw_block(pCGI, 0, 0, mat->matrix[1][1], bdi_selected);
	while (1)
	{
		for (int p = 0; p <= 9; p++)
		{
			for (int q = 0; q <= 11; q++)
			{
				mat->sign[p][q] = 0;
			}
		}
		int event1 = MoveOrChooseOneBlock(mat->matrix, pCGI, i0, j0, bdi_normal, bdi_selected, quit, 0);
		if (quit == 1)
			return;
		if (event1 == CONFIRM_CHOOSE)
		{
			if (!JudgeRound(mat->matrix, i0, j0))
			{
				gmw_status_line(pCGI, LOWER_STATUS_LINE, "��ͷ��/����ƶ����س�/�������ѡ��", "��Χ����ֵͬ");
				continue;
			}
			else
			{
				FindSameNoRecursion(mat->matrix, mat->sign, i0, j0, pCGI->row_num, pCGI->col_num);
				for (int i1 = 1; i1 <= pCGI->row_num; i1++)
				{
					for (int j1 = 1; j1 <= pCGI->col_num; j1++)
					{
						if (mat->sign[i1][j1] == 1 && (i1 != i0 || j1 != j0))
							gmw_draw_block(pCGI, i1 - 1, j1 - 1, mat->matrix[i1][j1], bdi_related);
					}
				}
				gmw_status_line(pCGI, LOWER_STATUS_LINE, "��ͷ��/����ƶ�ȡ����ǰѡ�񣬻س���/��������ϳ�");
				int event2 = MoveOrChooseOneBlock(mat->matrix, pCGI, i0, j0, bdi_normal, bdi_selected, quit, 1);
				if (event2 == CONFIRM_CHOOSE)
				{
					break;
				}
				else
				{
					for (int i = 1; i <= pCGI->row_num; i++)
					{
						for (int j = 1; j <= pCGI->col_num; j++)
						{
							if (mat->sign[i][j] == 1)
								gmw_draw_block(pCGI, i - 1, j - 1, mat->matrix[i][j], bdi_normal);
						}
					}
					gmw_draw_block(pCGI, i0 - 1, j0 - 1, mat->matrix[i0][j0], bdi_selected);
				}
			}
		}
	}
	CombineSame(i0, j0, mat);
	for (int i = 1; i <= pCGI->row_num; i++)
	{
		for (int j = 1; j <= pCGI->col_num; j++)
		{
			if (mat->matrix[i][j] == 0)
				gmw_draw_block(pCGI, i - 1, j - 1, 0, bdi_normal);
		}
	}//���հ׸���
	for (int n = 1; n <= 3; n++)
	{
		gmw_draw_block(pCGI, i0 - 1, j0 - 1, mat->matrix[i0][j0], bdi_selected);
		gmw_draw_block(pCGI, i0 - 1, j0 - 1, mat->matrix[i0][j0], bdi_normal);
	}//������ҫ

	score += AddScore(mat->sign, mat->matrix[i0][j0]);

	char temp[256];
	sprintf(temp, "���ε÷�:%d �ܵ÷�:%d �ϳ�Ŀ��:%d", AddScore(mat->sign, mat->matrix[i0][j0]), score, aim);
	gmw_status_line(pCGI, TOP_STATUS_LINE, temp);//��״̬����ʾ

	BlockFallDown(pCGI, mat, bdi_normal);

	RemoveZeroGMW(pCGI, mat);

	int max = SearchForMax(mat->matrix);
	AddNewNum(pCGI->row_num, pCGI->col_num, mat->matrix, max);

	for (int i = 1; i <= pCGI->row_num; i++)
	{
		for (int j = 1; j <= pCGI->col_num; j++)
		{
			if (mat->sign[i][j] == 1)
				gmw_draw_block(pCGI, i - 1, j - 1, mat->matrix[i][j], bdi_normal);
		}
	}
	return;
}

void CompleteGame(const CONSOLE_GRAPHICS_INFO* pCGI, MATRIX_SIGN* mat, int& score, int aim, bool& quit)
{
	const BLOCK_DISPLAY_INFO bdi_normal[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0����ʾ���ÿո���伴��
		{1,  COLOR_BLUE,   COLOR_BLACK,  NULL},	//����1�ġ�������״̬
		{2,  COLOR_GREEN,  COLOR_BLACK,  NULL},
		{3,  COLOR_CYAN,   COLOR_BLACK,  NULL},
		{4,  COLOR_RED,    COLOR_BLACK,  NULL},
		{5,  COLOR_PINK,   COLOR_BLACK,  NULL},
		{6,  COLOR_HBLUE,  COLOR_BLACK,  NULL},
		{7,  COLOR_HGREEN, COLOR_BLACK,  NULL},
		{8,  COLOR_HCYAN,  COLOR_BLACK,  NULL},
		{9,  COLOR_HRED,   COLOR_BLACK,  NULL},
		{10, COLOR_HPINK,  COLOR_BLACK,  NULL},	//����10�ġ�������״̬�������Ҫ�������Լ�������11���Ժ������
		{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������Ϊ-999
	};
	const BLOCK_DISPLAY_INFO bdi_related[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0����ʾ���ÿո���伴��
		{1,  COLOR_BLUE,   COLOR_WHITE,  NULL},	//����1�ġ���ѡ��+������״̬
		{2,  COLOR_GREEN,  COLOR_WHITE,  NULL},
		{3,  COLOR_CYAN,   COLOR_WHITE,  NULL},
		{4,  COLOR_RED,    COLOR_WHITE,  NULL},
		{5,  COLOR_PINK,   COLOR_WHITE,  NULL},
		{6,  COLOR_HBLUE,  COLOR_WHITE,  NULL},
		{7,  COLOR_HGREEN, COLOR_WHITE,  NULL},
		{8,  COLOR_HCYAN,  COLOR_WHITE,  NULL},
		{9,  COLOR_HRED,   COLOR_WHITE,  NULL},
		{10, COLOR_HPINK,  COLOR_WHITE,  NULL},	//����10�ġ���ѡ��+������״̬�������Ҫ�������Լ�������11���Ժ������
		{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������Ϊ-999
	};
	const BLOCK_DISPLAY_INFO bdi_selected[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0����ʾ���ÿո���伴��
		{1, COLOR_BLUE,   COLOR_HWHITE, NULL},	//����1�ġ�ѡ�С�״̬
		{2, COLOR_GREEN,  COLOR_HWHITE, NULL},
		{3, COLOR_CYAN,   COLOR_HWHITE, NULL},
		{4, COLOR_RED,    COLOR_HWHITE, NULL},
		{5, COLOR_PINK,   COLOR_HWHITE, NULL},
		{6, COLOR_HBLUE,  COLOR_HWHITE, NULL},
		{7, COLOR_HGREEN, COLOR_HWHITE, NULL},
		{8, COLOR_HCYAN,  COLOR_HWHITE, NULL},
		{9, COLOR_HRED,   COLOR_HWHITE, NULL},
		{10,COLOR_HPINK,  COLOR_HWHITE, NULL},	//����10�ġ�ѡ�С�״̬�������Ҫ�������Լ�������11���Ժ������
		{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������Ϊ-999
	};

	int max = aim - 1;
	for (int i = 1; i <= pCGI->row_num; i++)
	{
		for (int j = 1; j <= pCGI->col_num; j++)
			gmw_draw_block(pCGI, i - 1, j - 1, mat->matrix[i][j], bdi_normal);
	}
	while (1)
	{
		OneCombine(pCGI, mat, bdi_normal, bdi_related, bdi_selected, score, aim, quit);
		if (quit == 1)
			return;
		if (SearchForMax(mat->matrix) > max)
		{
			max = SearchForMax(mat->matrix);
			char temp[256];
			sprintf(temp, "�Ѿ��ϳɵ�%d,���س���/����������������Ŀ�����", max);
			gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);

			while (1)
			{
				int mx2, my2, ma2, kc12, kc22;
				int event2 = cct_read_keyboard_and_mouse(mx2, my2, ma2, kc12, kc22);
				if (ma2 == MOUSE_LEFT_BUTTON_CLICK || kc12 == 13)
					break;
			}

		}

		if (JudgeAll(mat->matrix, pCGI->col_num, pCGI->row_num))
		{
			gmw_status_line(pCGI, LOWER_STATUS_LINE, "��Q�˳�", "�޿ɺϲ������Ϸ����");
			while (1)
			{
				int mx2, my2, ma2, kc12, kc22;
				int event2 = cct_read_keyboard_and_mouse(mx2, my2, ma2, kc12, kc22);
				if (kc12 == 'Q' || kc12 == 'q')
					break;
			}
			quit = 1;
			break;
		}
	}
}