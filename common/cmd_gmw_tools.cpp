/* 2050598 ����� ͨ�� */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <climits>
#include <conio.h>
#include <cstring>
#include <iomanip>
#include <Windows.h>
#include <ctime>
#include <cstdio>
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
using namespace std;

/* --------------------------------------------------
		�˴����Ը�����Ҫ�ľ�̬ȫ�ֱ������������٣����û�У�����̬ȫ��ֻ������/�궨�壨�������ޣ���
   -------------------------------------------------- */


/* --------------------------------------------------
		�˴����Ը�����Ҫ���ڲ��������ߺ���
		1�����������ޣ�����Ϊ gmw_inner_* 
		2����������
		3��������static������ȷ��ֻ�ڱ�Դ�ļ���ʹ��
   -------------------------------------------------- */

   /***************************************************************************
	 �������ƣ�
	 ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�11������,��gmw_set_frame_linetype��gmw_set_block_linetype����
	 ���������top_left, top_right...
			   const char *...						����11/6�֣������.h���˴���
	 �� �� ֵ��
	 ˵    ����Լ��Ϊһ�������Ʊ��������ʹ���������ݣ���Ϊ��֤2�ֽ�
			   1������2�ֽ���ֻȡǰ2�ֽ�
			   2�������NULL���������ո����
			   3�������1�ֽڣ���һ���ո������˶�������ʾ�ң������
   ***************************************************************************/
static void gmw_inner_linetype_fill(char loc[], const char ch[])
{
	if (ch == NULL)
		strcpy(loc, "  ");
	else if (strlen(ch) > 2)
		strncpy(loc, ch, 2);
	else if (strlen(ch) == 1)
	{
		strcpy(loc, ch);
		strcat(loc, " ");
	}
	else if (strlen(ch) == 2)
		strcpy(loc, ch);

	return;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����ֵ�����bdi�����е��±�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
static int gmw_inner_value_to_no(const int value, const BLOCK_DISPLAY_INFO* bdi)
{
	for (int i = 1; bdi[i].value != BDI_VALUE_END; i++)
	{
		if (value == bdi[i].value)
			return i;
	}
	return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʾĳһ��ɫ��(����Ϊ�ַ���������Ϊrow/col
			(���ⲿ����������Ҫ�ڵ��ú���ǰ��gotoxy�ߵ��������Ͻǵ�λ�ã�����)
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int row_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int col_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int bdi_value						����Ҫ��ʾ��ֵ
		   const BLOCK_DISPLAY_INFO *const bdi		����Ÿ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
  �� �� ֵ��
  ˵    ����1��BLOCK_DISPLAY_INFO �ĺ����ͷ�ļ����÷��ο���������
			2��bdi_valueΪ BDI_VALUE_BLANK ��ʾ�հ׿飬Ҫ���⴦��
***************************************************************************/
static int gmw_inner_draw_block_without_gotoxy(const CONSOLE_GRAPHICS_INFO* const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO* const bdi)
{
	if (bdi_value == BDI_VALUE_BLANK)//���հ׿�
	{
		int x, y;
		cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
		for (int n = 1; n <= pCGI->CFI.block_high; n++)
		{
			cout << setfill(' ') << setw(pCGI->CFI.block_width) << " ";
			cct_getxy(x, y);
			cct_gotoxy(x - pCGI->CFI.block_width, y + 1);
			Sleep(pCGI->delay_of_draw_block);
		}
		return 0;
	}
	int no = gmw_inner_value_to_no(bdi_value, bdi);
	int value_len = 1;
	if (bdi[no].content == NULL)
	{
		int k = bdi_value;
		while (1)
		{
			k = k / 10;
			if (k != 0)
				value_len++;
			else
				break;
		}
	}//�������ֳ���
	int x, y;
	int block_bgcolor, block_fgcolor;
	if (bdi[no].bgcolor == -1)
		block_bgcolor = pCGI->CFI.bgcolor;
	else
		block_bgcolor = bdi[no].bgcolor;
	if (bdi[no].fgcolor == -1)
		block_fgcolor = pCGI->CFI.fgcolor;
	else
		block_fgcolor = bdi[no].fgcolor;
	cct_setcolor(block_bgcolor, block_fgcolor);
	if (pCGI->CBI.block_border) {//���ɫ������Χ��
		cout << pCGI->CBI.top_left;//�X
		for (int i = 1; i <= (pCGI->CFI.block_width - 2 * 2) / 2; i++)
		{
			cout << pCGI->CBI.h_normal;//�T
		}
		cout << pCGI->CBI.top_right;//�[
		for (int i1 = 2; i1 <= pCGI->CFI.block_high - 1; i1++)
		{
			cct_getxy(x, y);
			cct_gotoxy(x - pCGI->CFI.block_width, y + 1);
			cout << pCGI->CBI.v_normal;//�U
			if (i1 == (pCGI->CFI.block_high + 1) / 2)
			{
				if (bdi[no].content == NULL)//�����������ڲ�ֵ
				{
					if ((pCGI->CFI.block_width - (value_len + 1) / 2 * 2 - 2 * 2) / 2 > 0)
						cout << setw((pCGI->CFI.block_width - (value_len + 1) / 2 * 2 - 2 * 2) / 2) << " ";
					cout << setiosflags(ios::left) << setw((value_len + 1) / 2 * 2) << bdi[no].value;
					if ((pCGI->CFI.block_width - (value_len + 1) / 2 * 2 - 2 * 2) / 2 > 0)
						cout << setw((pCGI->CFI.block_width - (value_len + 1) / 2 * 2 - 2 * 2) / 2) << " ";
				}
				else//����������ָ������
				{
					if ((pCGI->CFI.block_width - (strlen(bdi[no].content) + 1) / 2 * 2 - 2 * 2) / 2 > 0)
						cout << setw((pCGI->CFI.block_width - (strlen(bdi[no].content) + 1) / 2 * 2 - 2 * 2) / 2) << " ";
					cout << setiosflags(ios::left) << setw((strlen(bdi[no].content) + 1) / 2 * 2) << bdi[no].content;
					if ((pCGI->CFI.block_width - (strlen(bdi[no].content) + 1) / 2 * 2 - 2 * 2) / 2 > 0)
						cout << setw((pCGI->CFI.block_width - (strlen(bdi[no].content) + 1) / 2 * 2 - 2 * 2) / 2) << " ";
				}
			}
			else
			{
				cout << setw(pCGI->CFI.block_width - 2 * 2) << " ";
			}
			cout << pCGI->CBI.v_normal;//�U
			Sleep(pCGI->delay_of_draw_block);
		}
		cct_getxy(x, y);
		cct_gotoxy(x - pCGI->CFI.block_width, y + 1);
		cout << pCGI->CBI.lower_left;//�^
		for (int i = 1; i <= (pCGI->CFI.block_width - 4) / 2; i++)
		{
			cout << pCGI->CBI.h_normal;//�T
		}
		cout << pCGI->CBI.lower_right;//�a
	}
	else//��������Χ��
	{
		for (int i = 1; i <= pCGI->CFI.block_high; i++)
		{
			if (i == (pCGI->CFI.block_high + 1) / 2)
			{
				if (bdi[no].content == NULL)//�����������ڲ�ֵ
				{
					if ((pCGI->CFI.block_width - (value_len + 1) / 2 * 2) / 2 > 0)
						cout << setw((pCGI->CFI.block_width - (value_len + 1) / 2 * 2) / 2) << " ";
					cout << setiosflags(ios::left) << setw((value_len + 1) / 2 * 2) << bdi[no].value;
					if ((pCGI->CFI.block_width - (value_len + 1) / 2 * 2) / 2 > 0)
						cout << setw((pCGI->CFI.block_width - (value_len + 1) / 2 * 2) / 2) << " ";
				}
				else//����������ָ������
				{
					if ((pCGI->CFI.block_width - (strlen(bdi[no].content) + 1) / 2 * 2) / 2 > 0)
						cout << setw((pCGI->CFI.block_width - (strlen(bdi[no].content) + 1) / 2 * 2) / 2) << " ";
					cout << setiosflags(ios::left) << setw((strlen(bdi[no].content) + 1) / 2 * 2) << bdi[no].content;
					if ((pCGI->CFI.block_width - (strlen(bdi[no].content) + 1) / 2 * 2) / 2 > 0)
						cout << setw((pCGI->CFI.block_width - (strlen(bdi[no].content) + 1) / 2 * 2) / 2) << " ";
				}
			}
			else
			{
				cout << setw(pCGI->CFI.block_width) << " ";
			}
			if (i < pCGI->CFI.block_high)
			{
				cct_getxy(x, y);
				cct_gotoxy(x - pCGI->CFI.block_width, y + 1);
			}
		}
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}


/* ----------------------------------------------- 
		ʵ����������ĺ���������������׼����
   ----------------------------------------------- */
/***************************************************************************
  �������ƣ�
  ��    �ܣ�������Ϸ����ܵ�������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int row						������(������Ϊ0���������ޣ���Ϊ��֤��ȷ��)
			const int col						������(������Ϊ0���������ޣ���Ϊ��֤��ȷ��)
  �� �� ֵ��
  ˵    ����1��ָ��������Ϸ�ľ������������ֵ
            2�����еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->row_num = row;
	pCGI->col_num = col;
	pCGI->CFI.bhigh = pCGI->CFI.block_high + pCGI->CFI.block_high_ext;
	pCGI->lines = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no + 1 + pCGI->row_num * pCGI->CFI.bhigh
		+ (1 - pCGI->CFI.separator) + pCGI->lower_status_line + pCGI->extern_down_lines + 4;
	pCGI->cols = pCGI->extern_left_cols + 2 * pCGI->draw_frame_with_row_no + 2 + pCGI->col_num * pCGI->CFI.bwidth + 2 * (1 - pCGI->CFI.separator) + pCGI->extern_right_cols + 1;
	pCGI->SLI.lower_start_y = pCGI->lines - 5 - pCGI->extern_down_lines;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������������ڣ�����Ϸ�������������ڵ�����cmd���ڣ�����ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int bg_color					��ǰ��ɫ��ȱʡCOLOR_BLACK��
		   const int fg_color					������ɫ��ȱʡCOLOR_WHITE��
		   const bool cascade					���Ƿ�����ȱʡΪtrue-������
  �� �� ֵ��
  ˵    ����1��cascade = trueʱ
				ͬ���޸���Ϸ���������ɫ
				ͬ���޸�����״̬���������ı��ı���ɫ��ǰ��ɫ����Ŀ�ı��ı���ɫ��ǰ��ɫ���䣩
			2���������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15
				    ǰ��ɫ����ɫ��ֵһ�µ����޷���������
					ǰ��ɫ������״̬����Ŀǰ��ɫ�������޷�������Ŀ��ʾ
					...
***************************************************************************/
int gmw_set_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor, const bool cascade)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->area_bgcolor = bgcolor;
	pCGI->area_fgcolor = fgcolor;
	if (cascade) 
	{
		pCGI->CFI.bgcolor = bgcolor;
		pCGI->CFI.fgcolor = fgcolor;
		pCGI->SLI.top_normal_bgcolor = bgcolor;
		pCGI->SLI.top_catchy_bgcolor = bgcolor;
		pCGI->SLI.lower_normal_bgcolor = bgcolor;
		pCGI->SLI.lower_catchy_bgcolor = bgcolor;
		pCGI->SLI.top_normal_fgcolor = fgcolor;
		pCGI->SLI.lower_normal_fgcolor = fgcolor;
	}

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ô��ڵ�����
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const char *fontname					���������ƣ�ֻ����"Terminal"��"������"���֣������򷵻�-1�����ı����壩
		   const int fs_high					������߶ȣ�ȱʡ������Ϊ16�������������ƣ���Ϊ��֤��
		   const int fs_width					������߶ȣ�ȱʡ������Ϊ8�������������ƣ���Ϊ��֤��
  �� �� ֵ��
  ˵    ����1����cmd_console_tools�е�setfontsize���ƣ�Ŀǰֻ֧�֡��������塱�͡������塱
            2������������������ֱ�ӷ��أ�����ԭ�������ò���
***************************************************************************/
int gmw_set_font(CONSOLE_GRAPHICS_INFO *const pCGI, const char *fontname, const int fs_high, const int fs_width)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	if (!strcmp(fontname, "Terminal") && !strcmp(fontname, "������"))
		return -1;
	strcpy(pCGI->CFT.font_type, fontname);
	if (fs_high < 0)
		pCGI->CFT.font_size_high = 16;
	else
		pCGI->CFT.font_size_high = fs_high;
	if (fs_width < 0)
		pCGI->CFT.font_size_width = 8;
	else
		pCGI->CFT.font_size_width = fs_width;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������ʱ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int type						����ʱ�����ͣ�ĿǰΪ3�֣�
		   const int delay_ms					����msΪ��λ����ʱ
			   ���߿����ʱ��0 ~ �������ޣ���Ϊ��֤��ȷ��<0����0��
			   ��ɫ�����ʱ��0 ~ �������ޣ���Ϊ��֤��ȷ��<0����0��
			   ɫ���ƶ�����ʱ��BLOCK_MOVED_DELAY_MS ~ �������ޣ���Ϊ��֤��ȷ�� <BLOCK_MOVED_DELAY_MS ���� BLOCK_MOVED_DELAY_MS��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_delay(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int delay_ms)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	int delay_mst = delay_ms;//�����������
	if (delay_ms < 0)
		delay_mst = 0;
	if (type == DELAY_OF_DRAW_FRAME)
		pCGI->delay_of_draw_frame = delay_mst;
	else if (type == DELAY_OF_DRAW_BLOCK)
		pCGI->delay_of_draw_block = delay_mst;
	else if (type == DELAY_OF_BLOCK_MOVED)
		pCGI->delay_of_block_moved = delay_mst;	
	else
		return -1;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ���������������Ϸ����ܽṹ֮����Ҫ�����Ķ�������
  ��    �ܣ�CONSOLE_GRAPHICS_INFO *const pCGI	��
		   const int up_lines					���ϲ�������У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int down_lines				���²�������У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int left_cols					����߶�����У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int right_cols				���ұ߶�����У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
  �� �� ֵ��
  ˵    �����������еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_ext_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int up_lines, const int down_lines, const int left_cols, const int right_cols)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->extern_up_lines = up_lines;
	pCGI->extern_down_lines = down_lines;
	pCGI->extern_left_cols = left_cols;
	pCGI->extern_right_cols = right_cols;

	pCGI->lines = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no + 1 + pCGI->row_num * pCGI->CFI.bhigh + (1 - pCGI->CFI.separator) + pCGI->lower_status_line + pCGI->extern_down_lines + 4;
	pCGI->start_y = pCGI->extern_up_lines;
	pCGI->cols = pCGI->extern_left_cols + 2 * pCGI->draw_frame_with_row_no + 2 + pCGI->col_num * pCGI->CFI.bwidth + 2 * (1 - pCGI->CFI.separator) + pCGI->extern_right_cols + 1;
	pCGI->start_x = pCGI->extern_left_cols;
	pCGI->SLI.top_start_x = pCGI->start_x;//��״̬����ʼλ��
	pCGI->SLI.top_start_y = pCGI->start_y;
	pCGI->SLI.lower_start_x = pCGI->start_x;
	pCGI->SLI.lower_start_y = pCGI->lines - 5 - pCGI->extern_down_lines;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�11�����ͣ�ȱʡ4�֣�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��1 - ȫ�� 2 - ȫ���� 3 - ��˫���� 4 - �ᵥ��˫
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_frame_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	if (type < 1 || type>4)
		return -1;
	if (type == 1)
	{
		strcpy(pCGI->CFI.top_left, "�X");			// "�X"
		strcpy(pCGI->CFI.lower_left, "�^");		// "�^"
		strcpy(pCGI->CFI.top_right, "�[");		// "�["
		strcpy(pCGI->CFI.lower_right, "�a");		// "�a"
		strcpy(pCGI->CFI.h_normal, "�T");			// "�T"	//Horizontal
		strcpy(pCGI->CFI.v_normal, "�U");			// "�U"	//Vertical
		strcpy(pCGI->CFI.h_top_separator, "�j");	// "�h"
		strcpy(pCGI->CFI.h_lower_separator, "�m");	// "�k"
		strcpy(pCGI->CFI.v_left_separator, "�d");// "�c"
		strcpy(pCGI->CFI.v_right_separator, "�g");	// "�f"
		strcpy(pCGI->CFI.mid_separator, "�p");		// "��"
	}
	else if (type == 2)
	{
		strcpy(pCGI->CFI.top_left, "��");			// "�X"
		strcpy(pCGI->CFI.lower_left, "��");		// "�^"
		strcpy(pCGI->CFI.top_right, "��");		// "�["
		strcpy(pCGI->CFI.lower_right, "��");		// "�a"
		strcpy(pCGI->CFI.h_normal, "��");			// "�T"	//Horizontal
		strcpy(pCGI->CFI.v_normal, "��");			// "�U"	//Vertical
		strcpy(pCGI->CFI.h_top_separator, "��");	// "�h"
		strcpy(pCGI->CFI.h_lower_separator, "��");	// "�k"
		strcpy(pCGI->CFI.v_left_separator, "��");// "�c"
		strcpy(pCGI->CFI.v_right_separator, "��");	// "�f"
		strcpy(pCGI->CFI.mid_separator, "��");		// "��"
	}
	else if (type == 3)
	{
		strcpy(pCGI->CFI.top_left, "�V");			// "�X"
		strcpy(pCGI->CFI.lower_left, "�\");		// "�^"
		strcpy(pCGI->CFI.top_right, "�Y");		// "�["
		strcpy(pCGI->CFI.lower_right, "�_");		// "�a"
		strcpy(pCGI->CFI.h_normal, "�T");			// "�T"	//Horizontal
		strcpy(pCGI->CFI.v_normal, "��");			// "�U"	//Vertical
		strcpy(pCGI->CFI.h_top_separator, "�h");	// "�h"
		strcpy(pCGI->CFI.h_lower_separator, "�k");	// "�k"
		strcpy(pCGI->CFI.v_left_separator, "�b");// "�c"
		strcpy(pCGI->CFI.v_right_separator, "�e");	// "�f"
		strcpy(pCGI->CFI.mid_separator, "�n");		// "��"
	}
	else if (type == 4)
	{
		strcpy(pCGI->CFI.top_left, "�W");			// "�X"
		strcpy(pCGI->CFI.lower_left, "�]");		// "�^"
		strcpy(pCGI->CFI.top_right, "�Z");		// "�["
		strcpy(pCGI->CFI.lower_right, "�`");		// "�a"
		strcpy(pCGI->CFI.h_normal, "��");			// "�T"	//Horizontal
		strcpy(pCGI->CFI.v_normal, "�U");			// "�U"	//Vertical
		strcpy(pCGI->CFI.h_top_separator, "�i");	// "�h"
		strcpy(pCGI->CFI.h_lower_separator, "�l");	// "�k"
		strcpy(pCGI->CFI.v_left_separator, "�c");// "�c"
		strcpy(pCGI->CFI.v_right_separator, "�f");	// "�f"
		strcpy(pCGI->CFI.mid_separator, "�o");		// "��"
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�11������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const char *...						����11�֣������.h���˴���
  �� �� ֵ��
  ˵    ����Լ��Ϊһ�������Ʊ��������ʹ���������ݣ���Ϊ��֤2�ֽ�
			1������2�ֽ���ֻȡǰ2�ֽ�
			2�������NULL���������ո����
			3�������1�ֽڣ���һ���ո������˶�������ʾ�ң������
***************************************************************************/
int gmw_set_frame_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right,
	const char *lower_right, const char *h_normal, const char *v_normal, const char *h_top_separator,
	const char *h_lower_separator, const char *v_left_separator, const char *v_right_separator, const char *mid_separator)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	gmw_inner_linetype_fill(pCGI->CFI.top_left, top_left);
	gmw_inner_linetype_fill(pCGI->CFI.lower_left, lower_left);
	gmw_inner_linetype_fill(pCGI->CFI.top_right, top_right);
	gmw_inner_linetype_fill(pCGI->CFI.lower_right, lower_right);
	gmw_inner_linetype_fill(pCGI->CFI.h_normal, h_normal);
	gmw_inner_linetype_fill(pCGI->CFI.v_normal, v_normal);
	gmw_inner_linetype_fill(pCGI->CFI.h_top_separator, h_top_separator);
	gmw_inner_linetype_fill(pCGI->CFI.h_lower_separator, h_lower_separator);
	gmw_inner_linetype_fill(pCGI->CFI.v_left_separator, v_left_separator);
	gmw_inner_linetype_fill(pCGI->CFI.v_right_separator, v_right_separator);
	gmw_inner_linetype_fill(pCGI->CFI.mid_separator, mid_separator);

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�ɫ��������С���Ƿ���Ҫ�ָ��ߵ�
  ������������������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int block_width						����ȣ�����ȱʡ2����ΪԼ�������Ϊ�����Ʊ�����������������Ҫ+1��
			const int block_high						���߶ȣ�����ȱʡ1��
			const bool separator						���Ƿ���Ҫ�ָ��ߣ�ȱʡΪtrue����Ҫ�ָ��ߣ�
  �� �� ֵ��
  ˵    ������ܴ�С/�Ƿ���Ҫ�ָ��ߵȵı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_frame_style(CONSOLE_GRAPHICS_INFO *const pCGI, const int block_width, const int block_high, const bool separator)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	if (block_width % 2 != 0)
		pCGI->CFI.block_width = block_width + 1;
	else
		pCGI->CFI.block_width = block_width;
	pCGI->CFI.block_high = block_high;
	pCGI->CFI.separator = separator;

	if (pCGI->CFI.separator)
	{
		pCGI->CFI.block_high_ext = 1;
		pCGI->CFI.block_width_ext = 2;

	}
	else
	{
		pCGI->CFI.block_high_ext = 0;
		pCGI->CFI.block_width_ext = 0;
	}
	pCGI->CFI.bhigh = pCGI->CFI.block_high + pCGI->CFI.block_high_ext;
	pCGI->CFI.bwidth = pCGI->CFI.block_width + pCGI->CFI.block_width_ext;
	pCGI->lines = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no + 1 + pCGI->row_num * pCGI->CFI.bhigh + (1 - pCGI->CFI.separator) + pCGI->lower_status_line + pCGI->extern_down_lines + 4;
	pCGI->cols = pCGI->extern_left_cols + 2 * pCGI->draw_frame_with_row_no + 2 + pCGI->col_num * pCGI->CFI.bwidth + 2 * (1 - pCGI->CFI.separator) + pCGI->extern_right_cols + 1;
	pCGI->SLI.lower_start_y = pCGI->lines - 5 - pCGI->extern_down_lines;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BORDER_TYPE �ṹ�е���ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int bg_color					������ɫ��ȱʡ -1��ʾ�ô��ڱ���ɫ��
			const int fg_color					��ǰ��ɫ��ȱʡ -1��ʾ�ô���ǰ��ɫ��
  �� �� ֵ��
  ˵    �����������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15��ǰ��ɫ����ɫ��ֵһ�µ����޷��������ݵ�
***************************************************************************/
int gmw_set_frame_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	if (bgcolor == -1)
		pCGI->CFI.bgcolor = pCGI->area_bgcolor;
	else
		pCGI->CFI.bgcolor = bgcolor;
	if (fgcolor == -1)
		pCGI->CFI.fgcolor = pCGI->area_fgcolor;
	else
		pCGI->CFI.fgcolor = fgcolor;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BLOCK_INFO �ṹ�е�6�����ͣ�ȱʡ4�֣�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��1 - ȫ˫�� 2 - ȫ���� 3 - ��˫���� 4 - �ᵥ��˫
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_block_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	if (type == 1)
	{
		strcpy(pCGI->CBI.top_left, "�X");
		strcpy(pCGI->CBI.top_right, "�[");
		strcpy(pCGI->CBI.lower_left, "�^");
		strcpy(pCGI->CBI.lower_right, "�a");
		strcpy(pCGI->CBI.v_normal, "�U");
		strcpy(pCGI->CBI.h_normal, "�T");
	}
	else if (type == 2)
	{
		strcpy(pCGI->CBI.top_left, "��");
		strcpy(pCGI->CBI.top_right, "��");
		strcpy(pCGI->CBI.lower_left, "��");
		strcpy(pCGI->CBI.lower_right, "��");
		strcpy(pCGI->CBI.v_normal, "��");
		strcpy(pCGI->CBI.h_normal, "��");
	}
	else if (type == 3)
	{
		strcpy(pCGI->CBI.top_left, "�V");
		strcpy(pCGI->CBI.top_right, "�Y");
		strcpy(pCGI->CBI.lower_left, "�\");
		strcpy(pCGI->CBI.lower_right, "�_");
		strcpy(pCGI->CBI.v_normal, "��");
		strcpy(pCGI->CBI.h_normal, "�T");
	}
	else if (type == 4)
	{
		strcpy(pCGI->CBI.top_left, "�W");
		strcpy(pCGI->CBI.top_right, "�Z");
		strcpy(pCGI->CBI.lower_left, "�]");
		strcpy(pCGI->CBI.lower_right, "�`");
		strcpy(pCGI->CBI.v_normal, "�U");
		strcpy(pCGI->CBI.h_normal, "��");
	}

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BLOCK_INFO �ṹ�е�6������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const char *...					����6�֣������.h���˴���
  �� �� ֵ��
  ˵    ����Լ��Ϊһ�������Ʊ��������ʹ���������ݣ���Ϊ��֤2�ֽ�
			1������2�ֽ���ֻȡǰ2�ֽ�
			2�������NULL���������ո����
			3�������1�ֽڣ���һ���ո������˶�������ʾ�ң������
***************************************************************************/
int gmw_set_block_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right, const char *lower_right, const char *h_normal, const char *v_normal)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	gmw_inner_linetype_fill(pCGI->CBI.top_left, top_left);
	gmw_inner_linetype_fill(pCGI->CBI.lower_left, lower_left);
	gmw_inner_linetype_fill(pCGI->CBI.top_right, top_right);
	gmw_inner_linetype_fill(pCGI->CBI.lower_right, lower_right);
	gmw_inner_linetype_fill(pCGI->CBI.h_normal, h_normal);
	gmw_inner_linetype_fill(pCGI->CBI.v_normal, v_normal);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����ÿ����Ϸɫ��(����)�Ƿ���ҪС�߿�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const bool on_off					��true - ��Ҫ flase - ����Ҫ��ȱʡfalse��
  �� �� ֵ��
  ˵    �����߿�Լ��Ϊ�����Ʊ����˫��
***************************************************************************/
int gmw_set_block_border_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->CBI.block_border = on_off;
	if (on_off)
	{
		strcpy(pCGI->CBI.top_left,"�X");
		strcpy(pCGI->CBI.top_right, "�[");
		strcpy(pCGI->CBI.lower_left, "�^");
		strcpy(pCGI->CBI.lower_right, "�a");
		strcpy(pCGI->CBI.h_normal, "�T");
		strcpy(pCGI->CBI.v_normal, "�U");
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ����״̬��
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��״̬�����ͣ���/�£�
			const bool on_off					��true - ��Ҫ flase - ����Ҫ��ȱʡtrue��
  �� �� ֵ��
  ˵    ����1��״̬�������Լ�����£�
			   1.1����״̬��ֻ��һ�У������������Ϸ�����/�б�����棬Ϊ��������ʼһ�У�����������Ͻ����������״̬�������꣩
			   1.2����״̬��ֻ��һ�У������������·����ߵ�����
			   1.3��״̬���Ŀ��Ϊ�������ȣ������Ϣ������ض�
		   2�����еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_status_line_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const bool on_off)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	if (type == TOP_STATUS_LINE)
	{
		pCGI->top_status_line = on_off;
		pCGI->SLI.is_top_status_line = on_off;
	}
	else if (type == LOWER_STATUS_LINE)
	{
		pCGI->lower_status_line = on_off;
		pCGI->SLI.is_lower_status_line = on_off;
	}

	pCGI->lines = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no + 1 + pCGI->row_num * pCGI->CFI.bhigh 
		+ (1 - pCGI->CFI.separator) + pCGI->lower_status_line + pCGI->extern_down_lines + 4;
	pCGI->SLI.lower_start_y = pCGI->lines - 5 - pCGI->extern_down_lines;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���������״̬������ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��״̬�����ͣ���/�£�
			const int normal_bgcolor			�������ı�����ɫ��ȱʡ -1��ʾʹ�ô��ڱ���ɫ��
			const int normal_fgcolor			�������ı�ǰ��ɫ��ȱʡ -1��ʾʹ�ô���ǰ��ɫ��
			const int catchy_bgcolor			����Ŀ�ı�����ɫ��ȱʡ -1��ʾʹ�ô��ڱ���ɫ��
			const int catchy_fgcolor			����Ŀ�ı�ǰ��ɫ��ȱʡ -1��ʾʹ������ɫ��
  ���������
  �� �� ֵ��
  ˵    �����������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15��ǰ��ɫ����ɫ��ֵһ�µ����޷��������ݵ�
***************************************************************************/
int gmw_set_status_line_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int normal_bgcolor, const int normal_fgcolor, const int catchy_bgcolor, const int catchy_fgcolor)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	int normal_bgcolor1, normal_fgcolor1,catchy_bgcolor1, catchy_fgcolor1;
	if (normal_bgcolor == -1)
		normal_bgcolor1 = pCGI->area_bgcolor;
	else
		normal_bgcolor1 = normal_bgcolor;
	if (normal_fgcolor == -1)
		normal_fgcolor1 = pCGI->area_fgcolor;
	else
		normal_fgcolor1 = normal_fgcolor;
	if (catchy_bgcolor == -1)
		catchy_bgcolor1 = pCGI->area_bgcolor;
	else
		catchy_bgcolor1 = catchy_bgcolor;
	if (catchy_fgcolor == -1)
		catchy_fgcolor1 = COLOR_HYELLOW;
	else
		catchy_fgcolor1 = catchy_fgcolor;

	if (type == TOP_STATUS_LINE)
	{
		pCGI->SLI.top_normal_bgcolor = normal_bgcolor1;
		pCGI->SLI.top_normal_fgcolor = normal_fgcolor1;
		pCGI->SLI.top_catchy_bgcolor = catchy_bgcolor1;
		pCGI->SLI.top_catchy_fgcolor = catchy_fgcolor1;
	}
	else if (type == LOWER_STATUS_LINE)
	{
		pCGI->SLI.lower_normal_bgcolor = normal_bgcolor1;
		pCGI->SLI.lower_normal_fgcolor = normal_fgcolor1;
		pCGI->SLI.lower_catchy_bgcolor = catchy_bgcolor1;
		pCGI->SLI.lower_catchy_fgcolor = catchy_fgcolor1;
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ�к�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const bool on_off					��true - ��ʾ flase - ����ʾ��ȱʡfalse��
  �� �� ֵ��
  ˵    ����1���к�Լ��Ϊ��ĸA��ʼ�������У��������26�����a��ʼ������52��ͳһΪ*��ʵ��Ӧ�ò����ܣ�
            2���Ƿ���ʾ�кŵı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_rowno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->draw_frame_with_row_no = on_off;

	pCGI->cols = pCGI->extern_left_cols + 2 * pCGI->draw_frame_with_row_no + 2 + pCGI->col_num * pCGI->CFI.bwidth + 2 * (1 - pCGI->CFI.separator) + pCGI->extern_right_cols + 1;
	
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ�б�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const bool on_off					��true - ��ʾ flase - ����ʾ��ȱʡfalse��
  �� �� ֵ��
  ˵    ����1���б�Լ��Ϊ����0��ʼ�������У�����0-99������99ͳһΪ**��ʵ��Ӧ�ò����ܣ�
            2���Ƿ���ʾ�б�ı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_colno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->draw_frame_with_col_no = on_off;

	pCGI->lines = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no + 1 + pCGI->row_num * pCGI->CFI.bhigh + (1 - pCGI->CFI.separator) + pCGI->lower_status_line + pCGI->extern_down_lines + 4;
	pCGI->SLI.lower_start_y = pCGI->lines - 5 - pCGI->extern_down_lines;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ӡ CONSOLE_GRAPHICS_INFO �ṹ���еĸ���Աֵ
  ���������
  �� �� ֵ��
  ˵    ����1�����������ã���ӡ��ʽ�Զ���
            2������������������δ���ù������Բ�ʵ��
***************************************************************************/
int gmw_print(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	cct_gotoxy(0, 0);
	cout << pCGI->lines;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��� CONSOLE_GRAPHICS_INFO �ṹ����ȱʡֵ���г�ʼ��
  ���������CONSOLE_GRAPHICS_INFO *const pCGI������ṹָ��
		   const int row					����Ϸ����ɫ��������ȱʡ10��
		   const int col					����Ϸ����ɫ��������ȱʡ10��
		   const int bgcolor				���������ڱ���ɫ��ȱʡ COLOR_BLACK��
		   const int fgcolor				���������ڱ���ɫ��ȱʡ COLOR_WHITE��
  �� �� ֵ��
  ˵    �������ڱ�����/ǰ���ף�����16*8�����������޶������У�����״̬�����У����к�/�б꣬�������Ϊ˫�ߣ�ɫ����2/�߶�1/��С�߿���ɫ��
***************************************************************************/
int gmw_init(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
{
	/* �����ñ�� */
	pCGI->inited = CGI_INITED;

	pCGI->area_bgcolor = bgcolor;
	pCGI->area_fgcolor = fgcolor;
	pCGI->CFI.bgcolor = bgcolor;
	pCGI->CFI.fgcolor = fgcolor;
	pCGI->row_num = row;
	pCGI->col_num = col;
	pCGI->CFI.block_high = 1; //ɫ���
	pCGI->CFI.block_width = 2;//ɫ���
	pCGI->CFI.separator = 1;//����зָ���
	if (pCGI->CFI.separator == 1)
	{
		pCGI->CFI.block_high_ext = 1;
		pCGI->CFI.block_width_ext = 2;

	}
	else
	{
		pCGI->CFI.block_high_ext = 0;
		pCGI->CFI.block_width_ext = 0;
	}
	pCGI->CFI.bwidth = pCGI->CFI.block_width + pCGI->CFI.block_width_ext;
	pCGI->CFI.bhigh = pCGI->CFI.block_high + pCGI->CFI.block_high_ext;
	pCGI->extern_up_lines = 0;
	pCGI->extern_down_lines = 0;
	pCGI->extern_left_cols = 0;
	pCGI->extern_right_cols = 0;
	pCGI->top_status_line = 1;//��״̬����
	pCGI->SLI.is_top_status_line = 1;
	pCGI->lower_status_line = 1;//��״̬����
	pCGI->SLI.is_lower_status_line = 1;
	pCGI->draw_frame_with_row_no = 0;//���б�
	pCGI->draw_frame_with_col_no = 0;//���б�
	pCGI->CFT.font_size_high = 16;//�����
	pCGI->CFT.font_size_width = 8;//�����
	pCGI->CBI.block_border = 0;//ɫ�鲻��Ҫ�߿�
	pCGI->lines = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no + 1 + pCGI->row_num * pCGI->CFI.bhigh + (1 - pCGI->CFI.separator) + pCGI->lower_status_line + pCGI->extern_down_lines + 4;
	pCGI->cols = pCGI->extern_left_cols + 2 * pCGI->draw_frame_with_row_no + 2 + pCGI->col_num * pCGI->CFI.bwidth + 2 * (1 - pCGI->CFI.separator) + pCGI->extern_right_cols + 1;
	pCGI->start_x = pCGI->extern_left_cols;
	pCGI->start_y = pCGI->extern_up_lines;
	strcpy(pCGI->CFT.font_type,"Terminal");
	strcpy(pCGI->CFI.top_left, "�X");			// "�X"
	strcpy(pCGI->CFI.lower_left, "�^");		// "�^"
	strcpy(pCGI->CFI.top_right, "�[");		// "�["
	strcpy(pCGI->CFI.lower_right, "�a");		// "�a"
	strcpy(pCGI->CFI.h_normal, "�T");			// "�T"	//Horizontal
	strcpy(pCGI->CFI.v_normal, "�U");			// "�U"	//Vertical
	strcpy(pCGI->CFI.h_top_separator, "�j");	// "�h"
	strcpy(pCGI->CFI.h_lower_separator, "�m");	// "�k"
	strcpy(pCGI->CFI.v_left_separator, "�d");// "�c"
	strcpy(pCGI->CFI.v_right_separator, "�g");	// "�f"
	strcpy(pCGI->CFI.mid_separator, "�p");		// "��"
	pCGI->SLI.top_start_x = pCGI->start_x;//��״̬����ʼλ��
	pCGI->SLI.top_start_y = pCGI->start_y;
	pCGI->SLI.lower_start_x = pCGI->start_x;
	pCGI->SLI.lower_start_y = pCGI->lines - 5 - pCGI->extern_down_lines;
	pCGI->SLI.top_normal_bgcolor = pCGI->area_bgcolor;
	pCGI->SLI.top_normal_fgcolor = pCGI->area_fgcolor;
	pCGI->SLI.top_catchy_bgcolor = COLOR_BLACK;
	pCGI->SLI.top_catchy_fgcolor = COLOR_HYELLOW;
	pCGI->delay_of_draw_frame = 0;
	pCGI->delay_of_draw_block = 0;
	pCGI->delay_of_block_moved = 1;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������Ϸ���
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
  �� �� ֵ��
  ˵    ��������ɲο�demo��Ч��
***************************************************************************/
int gmw_draw_frame(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	cct_setfontsize(pCGI->CFT.font_type, pCGI->CFT.font_size_high, pCGI->CFT.font_size_width);//��������
	cct_setconsoleborder(pCGI->cols, pCGI->lines);//����cmd���ڴ�С
	cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
	for (int i = 0; i <= pCGI->lines - 1; i++)	
		cct_showstr(0, i, " ", pCGI->area_bgcolor, pCGI->area_fgcolor, pCGI->cols);//�Ȱ���������Ϳ�ɱ���ɫ

	cct_gotoxy(pCGI->start_x, pCGI->start_y + pCGI->top_status_line);
	if (pCGI->draw_frame_with_col_no)//�����Ҫ�б�
	{
		cout << setw(2 * pCGI->draw_frame_with_row_no + 2 + pCGI->CFI.block_width / 2 - 1) << " ";
		cout << "0";
		for (int j1 = 1; j1 <= pCGI->col_num - 1; j1++)
		{
			if (j1 <= 99)
				cout << setiosflags(ios::right) << setfill(' ') << setw(pCGI->CFI.bwidth) << j1;
			else
				cout << setiosflags(ios::right) << setfill(' ') << setw(pCGI->CFI.bwidth) << "**";
		}
		cout << endl;
		if (pCGI->extern_left_cols + 2 * pCGI->draw_frame_with_row_no != 0)
			cout << setw(pCGI->extern_left_cols + 2 * pCGI->draw_frame_with_row_no) << " ";
	}
	else//�������һ��ʼ���ǻ�����ôֱ�Ӵ�start_x��ʼ�����ô�extern_left_cols�Ŀո���
	{
		if (2 * pCGI->draw_frame_with_row_no != 0)
			cout << setw(2 * pCGI->draw_frame_with_row_no) << " ";
	}
	cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
	cout << pCGI->CFI.top_left;//�X
	for (int j1 = 1; j1 <= pCGI->col_num - 1; j1++)
	{
		for (int n1 = 1; n1 <= pCGI->CFI.block_width / 2; n1++)
			cout << pCGI->CFI.h_normal;//�T
		if (pCGI->CFI.separator)
			cout << pCGI->CFI.h_top_separator;//�j
		Sleep(pCGI->delay_of_draw_frame);
	}
	for (int n1 = 1; n1 <= pCGI->CFI.block_width / 2; n1++)
		cout << pCGI->CFI.h_normal;
	cout << pCGI->CFI.top_right;//�[
	cout << endl;//�ϱ߿�������
	for (int i2 = 1; i2 <= pCGI->row_num; i2++)
	{
		for (int n2 = 1; n2 <= pCGI->CFI.block_high; n2++)
		{
			//��ÿһ�еĿ�ͷ���֣��б�Ӷ���ճ����У�
			if (pCGI->draw_frame_with_row_no)
			{
				cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
				if (n2 == (pCGI->CFI.block_high + 1) / 2)
				{
					if (pCGI->extern_left_cols != 0)
						cout << setw(pCGI->extern_left_cols) << " ";
					cout << resetiosflags(ios::right);
					if (i2 <= 26)
						cout << setw(2) << setiosflags(ios::left) << char('A' + i2 - 1);
					else if (i2 <= 52)
						cout << setw(2) << setiosflags(ios::left) << char('a' + i2 - 26 - 1);
					else
						cout << setw(2) << setiosflags(ios::left) << "*";
				}
				else
				{					
					cout << setw(pCGI->extern_left_cols + 2 * pCGI->draw_frame_with_row_no) << " ";
				}
			}
			else
			{
				cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
				if (pCGI->extern_left_cols != 0)
					cout << setw(pCGI->extern_left_cols) << " ";
			}
			//��ʼ�����ӱ�������
			cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			cout << pCGI->CFI.v_normal;//�U
			for (int j3 = 1; j3 <= pCGI->col_num; j3++)//����ɫ��������
			{
				cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
				cout << setw(pCGI->CFI.block_width) << " ";
				if (pCGI->CFI.separator)
				{
					cout << pCGI->CFI.v_normal;
				}
				else
				{
					if (j3 == pCGI->col_num)
						cout << pCGI->CFI.v_normal;
				}
				Sleep(pCGI->delay_of_draw_frame);
			}
			cout << endl;
		}
		if (pCGI->CFI.separator)//���ֽ���
		{
			cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
			if (pCGI->extern_left_cols + 2 * pCGI->draw_frame_with_row_no != 0)
				cout << setw(pCGI->extern_left_cols + 2 * pCGI->draw_frame_with_row_no) << " ";
			cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			if (i2 != pCGI->row_num)
				cout << pCGI->CFI.v_left_separator;//�d
			else
				cout << pCGI->CFI.lower_left;//�^
			for (int j4 = 1; j4 <= pCGI->col_num; j4++)
			{
				for (int n3 = 1; n3 <= pCGI->CFI.block_width / 2; n3++)
					cout << pCGI->CFI.h_normal;//�T
				if (j4 != pCGI->col_num)
				{
					if (i2 != pCGI->row_num)
						cout << pCGI->CFI.mid_separator;//�p
					else
						cout << pCGI->CFI.h_lower_separator;//�m
				}
				else
				{
					if (i2 != pCGI->row_num)
						cout << pCGI->CFI.v_right_separator << endl;//�g
					else
						cout << pCGI->CFI.lower_right << endl;//�a
				}
				Sleep(pCGI->delay_of_draw_frame);
			}//�зֽ��ߵ��Ѿ�����
		}
	}
	if (pCGI->CFI.separator == 0)//���޷ֽ��ߵ����±�һ���߿�
	{
		cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
		if (pCGI->extern_left_cols + 2 * pCGI->draw_frame_with_row_no != 0)
			cout << setw(pCGI->extern_left_cols + 2 * pCGI->draw_frame_with_row_no) << " ";
		cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
		cout << pCGI->CFI.lower_left;
		for (int j3 = 1; j3 <= pCGI->col_num; j3++)
		{
			for (int n4 = 1; n4 <= pCGI->CFI.block_width / 2; n4++)
				cout << pCGI->CFI.h_normal;
			Sleep(pCGI->delay_of_draw_frame);
		}
		cout << pCGI->CFI.lower_right << endl;
	}
	cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
	cct_gotoxy(pCGI->start_x, pCGI->start_y);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���״̬������ʾ��Ϣ
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int type							��ָ������/��״̬��
		   const char *msg						��������Ϣ
		   const char *catchy_msg					����Ҫ�ر��ע����Ϣ����������Ϣǰ��ʾ��
  �� �� ֵ��
  ˵    ����1���������޶�Ϊ����ܵĿ�ȣ����к��б�λ�ã����������ȥ
            2��������һ���ַ���ĳ���ֵ�ǰ������ᵼ�º������룬Ҫ����
***************************************************************************/
int gmw_status_line(const CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const char *msg, const char *catchy_msg)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	if (type == TOP_STATUS_LINE && pCGI->top_status_line == false)
		return 0;
	if (type == LOWER_STATUS_LINE && pCGI->lower_status_line == false)
		return 0;

	if (type == TOP_STATUS_LINE)
	{
		cct_gotoxy(pCGI->SLI.top_start_x, pCGI->SLI.top_start_y);
	}
	else if (type == LOWER_STATUS_LINE)
		cct_gotoxy(pCGI->SLI.lower_start_x, pCGI->SLI.lower_start_y);
	int lenofcatchy = 0;
	cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
	cout << setw(pCGI->cols - pCGI->extern_left_cols - pCGI->extern_right_cols) << " ";//�������һ��

	if (type == TOP_STATUS_LINE)
	{
		cct_gotoxy(pCGI->SLI.top_start_x, pCGI->SLI.top_start_y);
	}
	else if (type == LOWER_STATUS_LINE)
		cct_gotoxy(pCGI->SLI.lower_start_x, pCGI->SLI.lower_start_y);
	if (catchy_msg != NULL)
	{
		lenofcatchy = strlen(catchy_msg);
		cct_setcolor(pCGI->SLI.top_catchy_bgcolor, pCGI->SLI.top_catchy_fgcolor);
		cout << catchy_msg;
	}
	cct_setcolor(pCGI->SLI.top_normal_bgcolor, pCGI->SLI.top_normal_fgcolor);
	if (lenofcatchy + int(strlen(msg)) > pCGI->cols - pCGI->extern_left_cols - pCGI->extern_right_cols)
	{
		for (int n = 0; n <= pCGI->cols - pCGI->extern_left_cols - pCGI->extern_right_cols - lenofcatchy - 1; n++)
		{
			if (n == pCGI->cols - pCGI->extern_left_cols - pCGI->extern_right_cols - lenofcatchy - 1 && msg[n] > 0x80)
				break;
			if (msg[n] != '\0')
				cout << msg[n];
			else
				break;
		}
		cout << endl;
	}
	else
		cout << msg << endl;
	

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʾĳһ��ɫ��(����Ϊ�ַ���������Ϊrow/col)
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int row_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int col_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int bdi_value						����Ҫ��ʾ��ֵ
		   const BLOCK_DISPLAY_INFO *const bdi		����Ÿ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
  �� �� ֵ��
  ˵    ����1��BLOCK_DISPLAY_INFO �ĺ����ͷ�ļ����÷��ο���������
            2��bdi_valueΪ BDI_VALUE_BLANK ��ʾ�հ׿飬Ҫ���⴦��
***************************************************************************/
int gmw_draw_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO *const bdi)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	cct_setcursor(CURSOR_INVISIBLE);
	int x0 = pCGI->extern_left_cols + 2 * pCGI->draw_frame_with_row_no + 2 + col_no * pCGI->CFI.bwidth;
	int y0 = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no + 1 + row_no * pCGI->CFI.bhigh;
	cct_gotoxy(x0, y0);
	if (bdi_value == BDI_VALUE_BLANK)//���հ׿�
	{
		int x, y;
		cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
		for (int n = 1; n <= pCGI->CFI.block_high; n++)
		{
			cout << setfill(' ') << setw(pCGI->CFI.block_width) << " ";
			cct_getxy(x, y);
			cct_gotoxy(x - pCGI->CFI.block_width, y + 1);
			Sleep(pCGI->delay_of_draw_block);
		}
		return 0;
	}
	int no = gmw_inner_value_to_no(bdi_value, bdi);
	int value_len = 1;
	if (bdi[no].content == NULL)
	{
		int k = bdi_value;
		while (1)
		{
			k = k / 10;
			if (k != 0)
				value_len++;
			else
				break;
		}
	}//�������ֳ���
	int x, y;
	int block_bgcolor, block_fgcolor;
	if (bdi[no].bgcolor == -1)
		block_bgcolor = pCGI->CFI.bgcolor;
	else
		block_bgcolor = bdi[no].bgcolor;
	if (bdi[no].fgcolor == -1)
		block_fgcolor = pCGI->CFI.fgcolor;
	else
		block_fgcolor = bdi[no].fgcolor;
	cct_setcolor(block_bgcolor, block_fgcolor);
	if (pCGI->CBI.block_border) {//���ɫ������Χ��
		cout << pCGI->CBI.top_left;//�X
		for (int i = 1; i <= (pCGI->CFI.block_width - 2 * 2) / 2; i++)
		{
			cout << pCGI->CBI.h_normal;//�T
		}
		cout << pCGI->CBI.top_right;//�[
		for (int i1 = 2; i1 <= pCGI->CFI.block_high - 1; i1++)
		{
			cct_getxy(x, y);
			cct_gotoxy(x - pCGI->CFI.block_width, y + 1);
			cout << pCGI->CBI.v_normal;//�U
			if (i1 == (pCGI->CFI.block_high + 1) / 2)
			{
				if (bdi[no].content == NULL)//�����������ڲ�ֵ
				{
					if ((pCGI->CFI.block_width - (value_len + 1) / 2 * 2 - 2 * 2) / 2 > 0)
						cout << setw((pCGI->CFI.block_width - (value_len + 1) / 2 * 2 - 2 * 2) / 2) << " ";
					cout << setiosflags(ios::left) << setw((value_len + 1) / 2 * 2) << bdi[no].value;
					if ((pCGI->CFI.block_width - (value_len + 1) / 2 * 2 - 2 * 2) / 2 > 0)
						cout << setw((pCGI->CFI.block_width - (value_len + 1) / 2 * 2 - 2 * 2) / 2) << " ";
				}
				else//����������ָ������
				{
					if ((pCGI->CFI.block_width - (strlen(bdi[no].content) + 1) / 2 * 2 - 2 * 2) / 2 > 0)
						cout << setw((pCGI->CFI.block_width - (strlen(bdi[no].content) + 1) / 2 * 2 - 2 * 2) / 2) << " ";
					cout << setiosflags(ios::left) << setw((strlen(bdi[no].content) + 1) / 2 * 2) << bdi[no].content;
					if ((pCGI->CFI.block_width - (strlen(bdi[no].content) + 1) / 2 * 2 - 2 * 2) / 2 > 0)
						cout << setw((pCGI->CFI.block_width - (strlen(bdi[no].content) + 1) / 2 * 2 - 2 * 2) / 2) << " ";
				}
			}
			else
			{
				cout << setw(pCGI->CFI.block_width - 2 * 2) << " ";
			}
			cout << pCGI->CBI.v_normal;//�U
			Sleep(pCGI->delay_of_draw_block);
		}
		cct_getxy(x, y);
		cct_gotoxy(x - pCGI->CFI.block_width, y + 1);
		cout << pCGI->CBI.lower_left;//�^
		for (int i = 1; i <= (pCGI->CFI.block_width - 4) / 2; i++)
		{
			cout << pCGI->CBI.h_normal;//�T
		}
		cout << pCGI->CBI.lower_right;//�a
	}
	else//��������Χ��
	{
		for (int i = 1; i <= pCGI->CFI.block_high; i++)
		{
			if (i == (pCGI->CFI.block_high + 1) / 2) 
			{
				if (bdi[no].content == NULL)//�����������ڲ�ֵ
				{
					if ((pCGI->CFI.block_width - (value_len + 1) / 2 * 2) / 2 > 0)
						cout << setw((pCGI->CFI.block_width - (value_len + 1) / 2 * 2) / 2) << " ";
					cout << setiosflags(ios::left) << setw((value_len + 1) / 2 * 2) << bdi[no].value;
					if ((pCGI->CFI.block_width - (value_len + 1) / 2 * 2) / 2 > 0)
						cout << setw((pCGI->CFI.block_width - (value_len + 1) / 2 * 2) / 2) << " ";
				}
				else//����������ָ������
				{
					if ((pCGI->CFI.block_width - (strlen(bdi[no].content) + 1) / 2 * 2) / 2 > 0)
						cout << setw((pCGI->CFI.block_width - (strlen(bdi[no].content) + 1) / 2 * 2) / 2) << " ";
					cout << setiosflags(ios::left) << setw((strlen(bdi[no].content) + 1) / 2 * 2) << bdi[no].content;
					if ((pCGI->CFI.block_width - (strlen(bdi[no].content) + 1) / 2 * 2 ) / 2 > 0)
						cout << setw((pCGI->CFI.block_width - (strlen(bdi[no].content) + 1) / 2 * 2) / 2) << " ";
				}
			}
			else
			{				
				cout << setw(pCGI->CFI.block_width) << " ";
			}
			if (i < pCGI->CFI.block_high)
			{
				cct_getxy(x, y);
				cct_gotoxy(x - pCGI->CFI.block_width, y + 1);
			}
		}
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ƶ�ĳһ��ɫ��
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int row_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int col_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int bdi_value						����Ҫ��ʾ��ֵ
		   const int blank_bdi_value				���ƶ����������ڶ���Ч����ʾʱ���ڱ�ʾ�հ׵�ֵ��һ��Ϊ0���˴���Ϊ�������룬�ǿ��ǵ����ܳ��ֵ����������
		   const BLOCK_DISPLAY_INFO *const bdi		�������ʾֵ/�հ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
		   const int direction						���ƶ�����һ�����֣������cmd_gmw_tools.h
		   const int distance						���ƶ����루��1��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_move_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO *const bdi, const int direction, const int distance)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	int x0, y0;
	x0 = pCGI->extern_left_cols + 2 * pCGI->draw_frame_with_row_no + 2 + col_no * pCGI->CFI.bwidth;
	y0 = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no + 1 + row_no * pCGI->CFI.bhigh;
	cct_gotoxy(x0, y0);//���������Ͻ�
	if (direction == UP_TO_DOWN)
	{
		for (int t = 1; t <= distance; t++)//�ƶ�distance��һС��
		{
			int x, y;
			cct_getxy(x, y);						
			int y1;
			for (y1 = y; y1 <= y + pCGI->CFI.bhigh; y1++)
			{
				int x2, y2;
				cct_gotoxy(x0, y1);
				cct_getxy(x2, y2);
				gmw_inner_draw_block_without_gotoxy(pCGI, row_no, col_no, bdi_value, bdi);
				Sleep(pCGI->delay_of_block_moved);
				if (y1 < y + pCGI->CFI.bhigh)
				{
					cct_gotoxy(x2, y2);
					gmw_inner_draw_block_without_gotoxy(pCGI, row_no, col_no, blank_bdi_value, bdi);
				}
			}			
			if (pCGI->CFI.separator)//����зָ��ߣ���������ķָ��ߣ����ߵ���ǰ�������Ͻ�
			{
				cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
				cct_getxy(x, y);
				cct_gotoxy(x - pCGI->CFI.block_width, y - pCGI->CFI.block_high);
				for (int n = 1; n <= pCGI->CFI.block_width / 2; n++)
				{
					cout << pCGI->CFI.h_normal;
				}
				cct_getxy(x, y);
				cct_gotoxy(x - pCGI->CFI.block_width, y + 1);
			}
			else//û�зָ��ߣ�ֱ���ߵ���ǰ�������Ͻ�
			{
				cct_getxy(x, y);//��ǰλ�ڷ������½�
				cct_gotoxy(x - pCGI->CFI.block_width, y - (pCGI->CFI.block_high - 1));
			}
		}
	}
	else if (direction == DOWN_TO_UP)
	{
		for (int t = 1; t <= distance; t++)
		{
			int x, y;
			cct_getxy(x, y);
			int y1;
			for (y1 = y; y1 >= y - pCGI->CFI.bhigh; y1--)
			{
				int x2, y2;
				cct_gotoxy(x0, y1);
				cct_getxy(x2, y2);
				gmw_inner_draw_block_without_gotoxy(pCGI, row_no, col_no, bdi_value, bdi);
				Sleep(pCGI->delay_of_block_moved);
				if (y1 > y - pCGI->CFI.bhigh)
				{
					cct_gotoxy(x2, y2);
					gmw_inner_draw_block_without_gotoxy(pCGI, row_no, col_no, blank_bdi_value, bdi);
				}
			}
			if (pCGI->CFI.separator)
			{
				cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
				cct_getxy(x, y);
				cct_gotoxy(x - pCGI->CFI.block_width, y + 1);
				for (int n = 1; n <= pCGI->CFI.block_width / 2; n++)
				{
					cout << pCGI->CFI.h_normal;
				}
				cct_getxy(x, y);
				cct_gotoxy(x - pCGI->CFI.block_width, y - pCGI->CFI.block_high);
			}
			else//���û�зָ��ߣ�ֱ��ȥ����ǰ����λ�õ����Ͻ�
			{
				cct_getxy(x, y);
				cct_gotoxy(x - pCGI->CFI.block_width, y - (pCGI->CFI.block_high - 1));
			}
		}
	}
	else if (direction == RIGHT_TO_LEFT)
	{
		for (int t = 1; t <= distance; t++)
		{
			int x, y;
			cct_getxy(x, y);
			int x1;
			for (x1 = x; x1 >= x - pCGI->CFI.bwidth; x1--)
			{
				cct_gotoxy(x1, y);
				int x2, y2;
				cct_getxy(x2, y2);
				gmw_inner_draw_block_without_gotoxy(pCGI, row_no, col_no, bdi_value, bdi);
				Sleep(pCGI->delay_of_block_moved);
				if (x1 > x - pCGI->CFI.bwidth)
				{
					cct_gotoxy(x2, y2);
					gmw_inner_draw_block_without_gotoxy(pCGI, row_no, col_no, blank_bdi_value, bdi);
				}
			}
			if (pCGI->CFI.separator)//����зָ��ߣ���Ҫ���ұ߷�����
			{
				cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
				int x3, y3;
				cct_getxy(x3, y3);
				for (int y4 = y3 - (pCGI->CFI.block_high - 1); y4 <= y3; y4++)
				{
					cct_gotoxy(x3, y4);
					cout << pCGI->CFI.v_normal;
				}
			}
			cct_gotoxy(x - pCGI->CFI.bwidth, y);
		}
	}
	else if (direction == LEFT_TO_RIGHT)
	{
		for (int t = 1; t <= distance; t++)
		{
			int x, y;
			cct_getxy(x, y);
			int x1;
			for (x1 = x; x1 <= x + pCGI->CFI.bwidth; x1++)
			{
				cct_gotoxy(x1, y);
				int x2, y2;
				cct_getxy(x2, y2);
				gmw_inner_draw_block_without_gotoxy(pCGI, row_no, col_no, bdi_value, bdi);
				Sleep(pCGI->delay_of_block_moved);
				if (x1 < x + pCGI->CFI.bwidth)
				{
					cct_gotoxy(x2, y2);
					gmw_inner_draw_block_without_gotoxy(pCGI, row_no, col_no, blank_bdi_value, bdi);
				}
			}
			if (pCGI->CFI.separator)
			{
				cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
				int x3, y3;
				cct_getxy(x3, y3);
				for (int y4 = y3 - (pCGI->CFI.block_high - 1); y4 <= y3; y4++)
				{
					cct_gotoxy(x3 - pCGI->CFI.bwidth, y4);
					cout << pCGI->CFI.v_normal;
				}
			}
			cct_gotoxy(x + pCGI->CFI.bwidth, y);
		}
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������̻����
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   int &MAction							��������� CCT_MOUSE_EVENT�����ֵ��Ч��Ϊ MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK/MOUSE_RIGHT_BUTTON_CLICK ����֮һ
		                                               ������� CCT_KEYBOARD_EVENT�����ֵ��Ч
		   int &MRow								��������� CCT_MOUSE_EVENT �� MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK�����ֵ��Ч����ʾ���ѡ�����Ϸ������кţ���0��ʼ��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &MCol								��������� CCT_MOUSE_EVENT �� MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK�����ֵ��Ч����ʾ���ѡ�����Ϸ������кţ���0��ʼ��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &KeyCode1							��������� CCT_KEYBOARD_EVENT�����ֵ��Ч��Ϊ�����ļ��루���˫���룬��Ϊ��һ����
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &KeyCode2							��������� CCT_KEYBOARD_EVENT�����ֵ��Ч��Ϊ�����ļ��루���˫���룬��Ϊ�ڶ���������ǵ����룬��Ϊ0��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   const bool update_lower_status_line		������ƶ�ʱ���Ƿ�Ҫ�ڱ���������ʾ"[��ǰ���] *��*��/λ�÷Ƿ�"����Ϣ��true=��ʾ��false=����ʾ��ȱʡΪtrue��
  �� �� ֵ����������Լ��
		   1�����������ƶ����õ���MRow/MCol�봫�����ͬ(���ָ��΢С���ƶ�)���򲻷��أ�������
							  �õ����зǷ�λ�ã��򲻷��أ����� update_lower_status_line ����������״̬����ʾ"[��ǰ���] λ�÷Ƿ�"
							  �õ���MRow/MCol�봫��Ĳ�ͬ(��������һ���仯)������ update_lower_status_line ����������״̬����ʾ"[��ǰ���] *��*��"���ٷ���MOUSE_ONLY_MOVED����Щ��Ϸ���غ�Ҫ����ɫ��Ĳ�ͬ��ɫ��ʾ��
		   2������ǰ������������ҵ�ǰ���ָ��ͣ��������Ϸ�����*��*���ϣ��򷵻� CCT_MOUSE_EVENT ��MAction Ϊ MOUSE_LEFT_BUTTON_CLICK, MRow Ϊ�кţ�MCol Ϊ�б�
		                          �ҵ�ǰ���ָ��ͣ���ڷǷ����򣨷���Ϸ������Ϸ�����еķָ��ߣ����򲻷��أ����� update_lower_status_line ����������״̬����ʾ"[��ǰ���] λ�÷Ƿ�"
		   3������ǰ�������Ҽ������ж����ָ��ͣ�������Ƿ�Ϸ���ֱ�ӷ��� CCT_MOUSE_EVENT ��MAction Ϊ MOUSE_RIGHT_BUTTON_CLICK, MRow��MColȡ��ǰֵ����Ϊ�������ǵ��Ҽ������Ҫ���꣩
		   4��������¼����ϵ�ĳ������˫���밴��������ֱ�ӷ��� CCT_KEYBOARD_EVENT��KeyCode1/KeyCode2��Ϊ��Ӧ�ļ���ֵ
 ˵    ����ͨ������ cmd_console_tools.cpp �е� read_keyboard_and_mouse ����ʵ��
***************************************************************************/
int gmw_read_keyboard_and_mouse(const CONSOLE_GRAPHICS_INFO *const pCGI, int &MAction, int &MRow, int &MCol, int &KeyCode1, int &KeyCode2, const bool update_lower_status_line)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	cct_enable_mouse();
	cct_setcursor(CURSOR_INVISIBLE);

	bool hefa;
	int mx, my, mac, k1, k2;
	while (1)
	{
		hefa = 0;
		int event = cct_read_keyboard_and_mouse(mx, my, mac, k1, k2);
		if (event == CCT_KEYBOARD_EVENT)
		{
			KeyCode1 = k1;
			KeyCode2 = k2;
			return CCT_KEYBOARD_EVENT;
		}
		else if (event == CCT_MOUSE_EVENT)
		{
			
			MAction = mac;
			int mrow = MRow;
			int mcol = MCol;
			bool findcol = 0;
			if (mx >= pCGI->extern_left_cols + 2 * pCGI->draw_frame_with_row_no+2 &&
				mx <= pCGI->extern_left_cols + 2 * pCGI->draw_frame_with_row_no+2 - 1 + pCGI->CFI.bwidth * pCGI->col_num
				&& (mx - pCGI->extern_left_cols - 2 * pCGI->draw_frame_with_row_no-2) % pCGI->CFI.bwidth <= pCGI->CFI.block_width - 1)
			{
				mcol = (mx - pCGI->extern_left_cols - 2 * pCGI->draw_frame_with_row_no-2) / pCGI->CFI.bwidth;
				findcol = true;
			}
			if (findcol == true)//����ҵ�������
			{
				if (my >= pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no+1 &&
					my <= pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no+1 + pCGI->CFI.bhigh * pCGI->row_num - 1 &&
					(my - pCGI->start_y - pCGI->top_status_line - pCGI->draw_frame_with_col_no-1) % pCGI->CFI.bhigh <= pCGI->CFI.block_high - 1)
				{
					mrow = (my - pCGI->start_y - pCGI->top_status_line - pCGI->draw_frame_with_col_no-1) / pCGI->CFI.bhigh;
					hefa = 1;
				}
			}
			if (hefa == 0)
			{
				if (update_lower_status_line)
				{
					gmw_status_line(pCGI, LOWER_STATUS_LINE, "[��ǰ���] λ�÷Ƿ�");
					continue;
				}
			}
			if (mac == MOUSE_ONLY_MOVED && hefa == 1)
			{
				if (update_lower_status_line)
				{
					char temp[256] = { '\0' };
					sprintf(temp, "[��ǰ���] %c��%d��", char(MRow + 'A'), MCol);
					gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
				}
				if (MRow != mrow || MCol != mcol)//����ںϷ�λ��������һ��λ�ò�ͬ
				{
					MRow = mrow;
					MCol = mcol;				
					return CCT_MOUSE_EVENT;
				}
			}
			else if (mac == MOUSE_LEFT_BUTTON_CLICK && hefa)
			{

				MAction = MOUSE_LEFT_BUTTON_CLICK;
				MRow = mrow;
				MCol = mcol;
				return CCT_MOUSE_EVENT;

			}
			else if (mac == MOUSE_RIGHT_BUTTON_CLICK)
			{
				MAction = MOUSE_RIGHT_BUTTON_CLICK;
				return CCT_MOUSE_EVENT;
			}
		}
	}
	return -1;
}
