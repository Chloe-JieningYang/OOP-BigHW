/* 2050598 杨洁宁 通信 */
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
		此处可以给出需要的静态全局变量（尽可能少，最好没有）、静态全局只读变量/宏定义（个数不限）等
   -------------------------------------------------- */


/* --------------------------------------------------
		此处可以给出需要的内部辅助工具函数
		1、函数名不限，建议为 gmw_inner_* 
		2、个数不限
		3、必须是static函数，确保只在本源文件中使用
   -------------------------------------------------- */

   /***************************************************************************
	 函数名称：
	 功    能：填充 CONSOLE_FRAME_TYPE 结构中的11种线型,被gmw_set_frame_linetype和gmw_set_block_linetype调用
	 输入参数：top_left, top_right...
			   const char *...						：共11/6种，具体见.h，此处略
	 返 回 值：
	 说    明：约定为一个中文制表符，可以使用其它内容，人为保证2字节
			   1、超过2字节则只取前2字节
			   2、如果给NULL，用两个空格替代
			   3、如果给1字节，则补一个空格，如果因此而导致显示乱，不算错
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
  函数名称：
  功    能：利用值算出在bdi数组中的下标
  输入参数：
  返 回 值：
  说    明：
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
  函数名称：
  功    能：显示某一个色块(内容为字符串，坐标为row/col
			(与外部函数区别：需要在调用函数前先gotoxy走到方块左上角的位置！！！)
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int row_no						：行号（从0开始，人为保证正确性，程序不检查）
		   const int col_no						：列号（从0开始，人为保证正确性，程序不检查）
		   const int bdi_value						：需要显示的值
		   const BLOCK_DISPLAY_INFO *const bdi		：存放该值对应的显示信息的结构体数组
  返 回 值：
  说    明：1、BLOCK_DISPLAY_INFO 的含义见头文件，用法参考测试样例
			2、bdi_value为 BDI_VALUE_BLANK 表示空白块，要特殊处理
***************************************************************************/
static int gmw_inner_draw_block_without_gotoxy(const CONSOLE_GRAPHICS_INFO* const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO* const bdi)
{
	if (bdi_value == BDI_VALUE_BLANK)//画空白块
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
	}//计算数字长度
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
	if (pCGI->CBI.block_border) {//如果色块有外围线
		cout << pCGI->CBI.top_left;//╔
		for (int i = 1; i <= (pCGI->CFI.block_width - 2 * 2) / 2; i++)
		{
			cout << pCGI->CBI.h_normal;//═
		}
		cout << pCGI->CBI.top_right;//╗
		for (int i1 = 2; i1 <= pCGI->CFI.block_high - 1; i1++)
		{
			cct_getxy(x, y);
			cct_gotoxy(x - pCGI->CFI.block_width, y + 1);
			cout << pCGI->CBI.v_normal;//║
			if (i1 == (pCGI->CFI.block_high + 1) / 2)
			{
				if (bdi[no].content == NULL)//如果输出的是内部值
				{
					if ((pCGI->CFI.block_width - (value_len + 1) / 2 * 2 - 2 * 2) / 2 > 0)
						cout << setw((pCGI->CFI.block_width - (value_len + 1) / 2 * 2 - 2 * 2) / 2) << " ";
					cout << setiosflags(ios::left) << setw((value_len + 1) / 2 * 2) << bdi[no].value;
					if ((pCGI->CFI.block_width - (value_len + 1) / 2 * 2 - 2 * 2) / 2 > 0)
						cout << setw((pCGI->CFI.block_width - (value_len + 1) / 2 * 2 - 2 * 2) / 2) << " ";
				}
				else//如果输出的是指定内容
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
			cout << pCGI->CBI.v_normal;//║
			Sleep(pCGI->delay_of_draw_block);
		}
		cct_getxy(x, y);
		cct_gotoxy(x - pCGI->CFI.block_width, y + 1);
		cout << pCGI->CBI.lower_left;//╚
		for (int i = 1; i <= (pCGI->CFI.block_width - 4) / 2; i++)
		{
			cout << pCGI->CBI.h_normal;//═
		}
		cout << pCGI->CBI.lower_right;//╝
	}
	else//方块无外围线
	{
		for (int i = 1; i <= pCGI->CFI.block_high; i++)
		{
			if (i == (pCGI->CFI.block_high + 1) / 2)
			{
				if (bdi[no].content == NULL)//如果输出的是内部值
				{
					if ((pCGI->CFI.block_width - (value_len + 1) / 2 * 2) / 2 > 0)
						cout << setw((pCGI->CFI.block_width - (value_len + 1) / 2 * 2) / 2) << " ";
					cout << setiosflags(ios::left) << setw((value_len + 1) / 2 * 2) << bdi[no].value;
					if ((pCGI->CFI.block_width - (value_len + 1) / 2 * 2) / 2 > 0)
						cout << setw((pCGI->CFI.block_width - (value_len + 1) / 2 * 2) / 2) << " ";
				}
				else//如果输出的是指定内容
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
	return 0; //此句可根据需要修改
}


/* ----------------------------------------------- 
		实现下面给出的函数（函数声明不准动）
   ----------------------------------------------- */
/***************************************************************************
  函数名称：
  功    能：设置游戏主框架的行列数
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int row						：行数(错误则为0，不设上限，人为保证正确性)
			const int col						：列数(错误则为0，不设上限，人为保证正确性)
  返 回 值：
  说    明：1、指消除类游戏的矩形区域的行列值
            2、行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->row_num = row;
	pCGI->col_num = col;
	pCGI->CFI.bhigh = pCGI->CFI.block_high + pCGI->CFI.block_high_ext;
	pCGI->lines = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no + 1 + pCGI->row_num * pCGI->CFI.bhigh
		+ (1 - pCGI->CFI.separator) + pCGI->lower_status_line + pCGI->extern_down_lines + 4;
	pCGI->cols = pCGI->extern_left_cols + 2 * pCGI->draw_frame_with_row_no + 2 + pCGI->col_num * pCGI->CFI.bwidth + 2 * (1 - pCGI->CFI.separator) + pCGI->extern_right_cols + 1;
	pCGI->SLI.lower_start_y = pCGI->lines - 5 - pCGI->extern_down_lines;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置整个窗口（含游戏区、附加区在内的整个cmd窗口）的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int bg_color					：前景色（缺省COLOR_BLACK）
		   const int fg_color					：背景色（缺省COLOR_WHITE）
		   const bool cascade					：是否级联（缺省为true-级联）
  返 回 值：
  说    明：1、cascade = true时
				同步修改游戏主区域的颜色
				同步修改上下状态栏的正常文本的背景色和前景色，醒目文本的背景色（前景色不变）
			2、不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15
				    前景色背景色的值一致导致无法看到内容
					前景色正好是状态栏醒目前景色，导致无法看到醒目提示
					...
***************************************************************************/
int gmw_set_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor, const bool cascade)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
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

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置窗口的字体
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const char *fontname					：字体名称（只能是"Terminal"和"新宋体"两种，错误则返回-1，不改变字体）
		   const int fs_high					：字体高度（缺省及错误为16，不设其它限制，人为保证）
		   const int fs_width					：字体高度（缺省及错误为8，不设其它限制，人为保证）
  返 回 值：
  说    明：1、与cmd_console_tools中的setfontsize相似，目前只支持“点阵字体”和“新宋体”
            2、若设置其它字体则直接返回，保持原字体设置不变
***************************************************************************/
int gmw_set_font(CONSOLE_GRAPHICS_INFO *const pCGI, const char *fontname, const int fs_high, const int fs_width)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	if (!strcmp(fontname, "Terminal") && !strcmp(fontname, "新宋体"))
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
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置延时
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int type						：延时的类型（目前为3种）
		   const int delay_ms					：以ms为单位的延时
			   画边框的延时：0 ~ 不设上限，人为保证正确（<0则置0）
			   画色块的延时：0 ~ 不设上限，人为保证正确（<0则置0）
			   色块移动的延时：BLOCK_MOVED_DELAY_MS ~ 不设上限，人为保证正确（ <BLOCK_MOVED_DELAY_MS 则置 BLOCK_MOVED_DELAY_MS）
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_delay(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int delay_ms)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	int delay_mst = delay_ms;//纠正输入错误
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
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  输入参数：设置游戏主框架结构之外需要保留的额外区域
  功    能：CONSOLE_GRAPHICS_INFO *const pCGI	：
		   const int up_lines					：上部额外的行（缺省及错误为0，不设上限，人为保证）
		   const int down_lines				：下部额外的行（缺省及错误为0，不设上限，人为保证）
		   const int left_cols					：左边额外的列（缺省及错误为0，不设上限，人为保证）
		   const int right_cols				：右边额外的列（缺省及错误为0，不设上限，人为保证）
  返 回 值：
  说    明：额外行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_ext_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int up_lines, const int down_lines, const int left_cols, const int right_cols)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
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
	pCGI->SLI.top_start_x = pCGI->start_x;//上状态栏起始位置
	pCGI->SLI.top_start_y = pCGI->start_y;
	pCGI->SLI.lower_start_x = pCGI->start_x;
	pCGI->SLI.lower_start_y = pCGI->lines - 5 - pCGI->extern_down_lines;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的11种线型（缺省4种）
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：1 - 全线 2 - 全单线 3 - 横双竖单 4 - 横单竖双
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_frame_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	if (type < 1 || type>4)
		return -1;
	if (type == 1)
	{
		strcpy(pCGI->CFI.top_left, "╔");			// "╔"
		strcpy(pCGI->CFI.lower_left, "╚");		// "╚"
		strcpy(pCGI->CFI.top_right, "╗");		// "╗"
		strcpy(pCGI->CFI.lower_right, "╝");		// "╝"
		strcpy(pCGI->CFI.h_normal, "═");			// "═"	//Horizontal
		strcpy(pCGI->CFI.v_normal, "║");			// "║"	//Vertical
		strcpy(pCGI->CFI.h_top_separator, "╦");	// "╤"
		strcpy(pCGI->CFI.h_lower_separator, "╩");	// "╧"
		strcpy(pCGI->CFI.v_left_separator, "╠");// "╟"
		strcpy(pCGI->CFI.v_right_separator, "╣");	// "╢"
		strcpy(pCGI->CFI.mid_separator, "╬");		// "┼"
	}
	else if (type == 2)
	{
		strcpy(pCGI->CFI.top_left, "┏");			// "╔"
		strcpy(pCGI->CFI.lower_left, "┗");		// "╚"
		strcpy(pCGI->CFI.top_right, "┓");		// "╗"
		strcpy(pCGI->CFI.lower_right, "┛");		// "╝"
		strcpy(pCGI->CFI.h_normal, "━");			// "═"	//Horizontal
		strcpy(pCGI->CFI.v_normal, "┃");			// "║"	//Vertical
		strcpy(pCGI->CFI.h_top_separator, "┳");	// "╤"
		strcpy(pCGI->CFI.h_lower_separator, "┻");	// "╧"
		strcpy(pCGI->CFI.v_left_separator, "┣");// "╟"
		strcpy(pCGI->CFI.v_right_separator, "┫");	// "╢"
		strcpy(pCGI->CFI.mid_separator, "╋");		// "┼"
	}
	else if (type == 3)
	{
		strcpy(pCGI->CFI.top_left, "╒");			// "╔"
		strcpy(pCGI->CFI.lower_left, "╘");		// "╚"
		strcpy(pCGI->CFI.top_right, "╕");		// "╗"
		strcpy(pCGI->CFI.lower_right, "╛");		// "╝"
		strcpy(pCGI->CFI.h_normal, "═");			// "═"	//Horizontal
		strcpy(pCGI->CFI.v_normal, "┃");			// "║"	//Vertical
		strcpy(pCGI->CFI.h_top_separator, "╤");	// "╤"
		strcpy(pCGI->CFI.h_lower_separator, "╧");	// "╧"
		strcpy(pCGI->CFI.v_left_separator, "╞");// "╟"
		strcpy(pCGI->CFI.v_right_separator, "╡");	// "╢"
		strcpy(pCGI->CFI.mid_separator, "╪");		// "┼"
	}
	else if (type == 4)
	{
		strcpy(pCGI->CFI.top_left, "╓");			// "╔"
		strcpy(pCGI->CFI.lower_left, "╙");		// "╚"
		strcpy(pCGI->CFI.top_right, "╖");		// "╗"
		strcpy(pCGI->CFI.lower_right, "╜");		// "╝"
		strcpy(pCGI->CFI.h_normal, "━");			// "═"	//Horizontal
		strcpy(pCGI->CFI.v_normal, "║");			// "║"	//Vertical
		strcpy(pCGI->CFI.h_top_separator, "╥");	// "╤"
		strcpy(pCGI->CFI.h_lower_separator, "╨");	// "╧"
		strcpy(pCGI->CFI.v_left_separator, "╟");// "╟"
		strcpy(pCGI->CFI.v_right_separator, "╢");	// "╢"
		strcpy(pCGI->CFI.mid_separator, "╫");		// "┼"
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的11种线型
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const char *...						：共11种，具体见.h，此处略
  返 回 值：
  说    明：约定为一个中文制表符，可以使用其它内容，人为保证2字节
			1、超过2字节则只取前2字节
			2、如果给NULL，用两个空格替代
			3、如果给1字节，则补一个空格，如果因此而导致显示乱，不算错
***************************************************************************/
int gmw_set_frame_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right,
	const char *lower_right, const char *h_normal, const char *v_normal, const char *h_top_separator,
	const char *h_lower_separator, const char *v_left_separator, const char *v_right_separator, const char *mid_separator)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
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

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的色块数量大小、是否需要分隔线等
  输入参数：输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int block_width						：宽度（错误及缺省2，因为约定表格线为中文制表符，如果给出奇数，要+1）
			const int block_high						：高度（错误及缺省1）
			const bool separator						：是否需要分隔线（缺省为true，需要分隔线）
  返 回 值：
  说    明：框架大小/是否需要分隔线等的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_frame_style(CONSOLE_GRAPHICS_INFO *const pCGI, const int block_width, const int block_high, const bool separator)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
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
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BORDER_TYPE 结构中的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int bg_color					：背景色（缺省 -1表示用窗口背景色）
			const int fg_color					：前景色（缺省 -1表示用窗口前景色）
  返 回 值：
  说    明：不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15，前景色背景色的值一致导致无法看到内容等
***************************************************************************/
int gmw_set_frame_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
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
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BLOCK_INFO 结构中的6种线型（缺省4种）
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：1 - 全双线 2 - 全单线 3 - 横双竖单 4 - 横单竖双
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_block_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	if (type == 1)
	{
		strcpy(pCGI->CBI.top_left, "╔");
		strcpy(pCGI->CBI.top_right, "╗");
		strcpy(pCGI->CBI.lower_left, "╚");
		strcpy(pCGI->CBI.lower_right, "╝");
		strcpy(pCGI->CBI.v_normal, "║");
		strcpy(pCGI->CBI.h_normal, "═");
	}
	else if (type == 2)
	{
		strcpy(pCGI->CBI.top_left, "┏");
		strcpy(pCGI->CBI.top_right, "┓");
		strcpy(pCGI->CBI.lower_left, "┗");
		strcpy(pCGI->CBI.lower_right, "┛");
		strcpy(pCGI->CBI.v_normal, "┃");
		strcpy(pCGI->CBI.h_normal, "━");
	}
	else if (type == 3)
	{
		strcpy(pCGI->CBI.top_left, "╒");
		strcpy(pCGI->CBI.top_right, "╕");
		strcpy(pCGI->CBI.lower_left, "╘");
		strcpy(pCGI->CBI.lower_right, "╛");
		strcpy(pCGI->CBI.v_normal, "┃");
		strcpy(pCGI->CBI.h_normal, "═");
	}
	else if (type == 4)
	{
		strcpy(pCGI->CBI.top_left, "╓");
		strcpy(pCGI->CBI.top_right, "╖");
		strcpy(pCGI->CBI.lower_left, "╙");
		strcpy(pCGI->CBI.lower_right, "╜");
		strcpy(pCGI->CBI.v_normal, "║");
		strcpy(pCGI->CBI.h_normal, "━");
	}

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BLOCK_INFO 结构中的6种线型
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const char *...					：共6种，具体见.h，此处略
  返 回 值：
  说    明：约定为一个中文制表符，可以使用其它内容，人为保证2字节
			1、超过2字节则只取前2字节
			2、如果给NULL，用两个空格替代
			3、如果给1字节，则补一个空格，如果因此而导致显示乱，不算错
***************************************************************************/
int gmw_set_block_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right, const char *lower_right, const char *h_normal, const char *v_normal)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	gmw_inner_linetype_fill(pCGI->CBI.top_left, top_left);
	gmw_inner_linetype_fill(pCGI->CBI.lower_left, lower_left);
	gmw_inner_linetype_fill(pCGI->CBI.top_right, top_right);
	gmw_inner_linetype_fill(pCGI->CBI.lower_right, lower_right);
	gmw_inner_linetype_fill(pCGI->CBI.h_normal, h_normal);
	gmw_inner_linetype_fill(pCGI->CBI.v_normal, v_normal);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置每个游戏色块(彩球)是否需要小边框
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const bool on_off					：true - 需要 flase - 不需要（缺省false）
  返 回 值：
  说    明：边框约定为中文制表符，双线
***************************************************************************/
int gmw_set_block_border_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->CBI.block_border = on_off;
	if (on_off)
	{
		strcpy(pCGI->CBI.top_left,"╔");
		strcpy(pCGI->CBI.top_right, "╗");
		strcpy(pCGI->CBI.lower_left, "╚");
		strcpy(pCGI->CBI.lower_right, "╝");
		strcpy(pCGI->CBI.h_normal, "═");
		strcpy(pCGI->CBI.v_normal, "║");
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示上下状态栏
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：状态栏类型（上/下）
			const bool on_off					：true - 需要 flase - 不需要（缺省true）
  返 回 值：
  说    明：1、状态栏的相关约定如下：
			   1.1、上状态栏只能一行，在主区域最上方框线/列标的上面，为主区域的最开始一行（主区域的左上角坐标就是上状态栏的坐标）
			   1.2、下状态栏只能一行，在主区域最下方框线的下面
			   1.3、状态栏的宽度为主区域宽度，如果信息过长则截断
		   2、行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_status_line_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
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
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置上下状态栏的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：状态栏类型（上/下）
			const int normal_bgcolor			：正常文本背景色（缺省 -1表示使用窗口背景色）
			const int normal_fgcolor			：正常文本前景色（缺省 -1表示使用窗口前景色）
			const int catchy_bgcolor			：醒目文本背景色（缺省 -1表示使用窗口背景色）
			const int catchy_fgcolor			：醒目文本前景色（缺省 -1表示使用亮黄色）
  输入参数：
  返 回 值：
  说    明：不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15，前景色背景色的值一致导致无法看到内容等
***************************************************************************/
int gmw_set_status_line_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int normal_bgcolor, const int normal_fgcolor, const int catchy_bgcolor, const int catchy_fgcolor)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
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
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示行号
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const bool on_off					：true - 显示 flase - 不显示（缺省false）
  返 回 值：
  说    明：1、行号约定为字母A开始连续排列（如果超过26，则从a开始，超过52的统一为*，实际应用不可能）
            2、是否显示行号的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_rowno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->draw_frame_with_row_no = on_off;

	pCGI->cols = pCGI->extern_left_cols + 2 * pCGI->draw_frame_with_row_no + 2 + pCGI->col_num * pCGI->CFI.bwidth + 2 * (1 - pCGI->CFI.separator) + pCGI->extern_right_cols + 1;
	
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示列标
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const bool on_off					：true - 显示 flase - 不显示（缺省false）
  返 回 值：
  说    明：1、列标约定为数字0开始连续排列（数字0-99，超过99统一为**，实际应用不可能）
            2、是否显示列标的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_colno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->draw_frame_with_col_no = on_off;

	pCGI->lines = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no + 1 + pCGI->row_num * pCGI->CFI.bhigh + (1 - pCGI->CFI.separator) + pCGI->lower_status_line + pCGI->extern_down_lines + 4;
	pCGI->SLI.lower_start_y = pCGI->lines - 5 - pCGI->extern_down_lines;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：打印 CONSOLE_GRAPHICS_INFO 结构体中的各成员值
  输入参数：
  返 回 值：
  说    明：1、仅供调试用，打印格式自定义
            2、本函数测试用例中未调用过，可以不实现
***************************************************************************/
int gmw_print(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	cct_gotoxy(0, 0);
	cout << pCGI->lines;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：将 CONSOLE_GRAPHICS_INFO 结构体用缺省值进行初始化
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI：整体结构指针
		   const int row					：游戏区域色块行数（缺省10）
		   const int col					：游戏区域色块列数（缺省10）
		   const int bgcolor				：整个窗口背景色（缺省 COLOR_BLACK）
		   const int fgcolor				：整个窗口背景色（缺省 COLOR_WHITE）
  返 回 值：
  说    明：窗口背景黑/前景白，点阵16*8，上下左右无额外行列，上下状态栏均有，无行号/列标，框架线型为双线，色块宽度2/高度1/无小边框，颜色略
***************************************************************************/
int gmw_init(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
{
	/* 首先置标记 */
	pCGI->inited = CGI_INITED;

	pCGI->area_bgcolor = bgcolor;
	pCGI->area_fgcolor = fgcolor;
	pCGI->CFI.bgcolor = bgcolor;
	pCGI->CFI.fgcolor = fgcolor;
	pCGI->row_num = row;
	pCGI->col_num = col;
	pCGI->CFI.block_high = 1; //色块高
	pCGI->CFI.block_width = 2;//色块宽
	pCGI->CFI.separator = 1;//框架有分割线
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
	pCGI->top_status_line = 1;//上状态栏开
	pCGI->SLI.is_top_status_line = 1;
	pCGI->lower_status_line = 1;//下状态栏开
	pCGI->SLI.is_lower_status_line = 1;
	pCGI->draw_frame_with_row_no = 0;//无行标
	pCGI->draw_frame_with_col_no = 0;//无列标
	pCGI->CFT.font_size_high = 16;//字体高
	pCGI->CFT.font_size_width = 8;//字体宽
	pCGI->CBI.block_border = 0;//色块不需要边框
	pCGI->lines = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no + 1 + pCGI->row_num * pCGI->CFI.bhigh + (1 - pCGI->CFI.separator) + pCGI->lower_status_line + pCGI->extern_down_lines + 4;
	pCGI->cols = pCGI->extern_left_cols + 2 * pCGI->draw_frame_with_row_no + 2 + pCGI->col_num * pCGI->CFI.bwidth + 2 * (1 - pCGI->CFI.separator) + pCGI->extern_right_cols + 1;
	pCGI->start_x = pCGI->extern_left_cols;
	pCGI->start_y = pCGI->extern_up_lines;
	strcpy(pCGI->CFT.font_type,"Terminal");
	strcpy(pCGI->CFI.top_left, "╔");			// "╔"
	strcpy(pCGI->CFI.lower_left, "╚");		// "╚"
	strcpy(pCGI->CFI.top_right, "╗");		// "╗"
	strcpy(pCGI->CFI.lower_right, "╝");		// "╝"
	strcpy(pCGI->CFI.h_normal, "═");			// "═"	//Horizontal
	strcpy(pCGI->CFI.v_normal, "║");			// "║"	//Vertical
	strcpy(pCGI->CFI.h_top_separator, "╦");	// "╤"
	strcpy(pCGI->CFI.h_lower_separator, "╩");	// "╧"
	strcpy(pCGI->CFI.v_left_separator, "╠");// "╟"
	strcpy(pCGI->CFI.v_right_separator, "╣");	// "╢"
	strcpy(pCGI->CFI.mid_separator, "╬");		// "┼"
	pCGI->SLI.top_start_x = pCGI->start_x;//上状态栏起始位置
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

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：画主游戏框架
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
  返 回 值：
  说    明：具体可参考demo的效果
***************************************************************************/
int gmw_draw_frame(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	cct_setfontsize(pCGI->CFT.font_type, pCGI->CFT.font_size_high, pCGI->CFT.font_size_width);//设置字体
	cct_setconsoleborder(pCGI->cols, pCGI->lines);//设置cmd窗口大小
	cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
	for (int i = 0; i <= pCGI->lines - 1; i++)	
		cct_showstr(0, i, " ", pCGI->area_bgcolor, pCGI->area_fgcolor, pCGI->cols);//先把整个窗口涂成背景色

	cct_gotoxy(pCGI->start_x, pCGI->start_y + pCGI->top_status_line);
	if (pCGI->draw_frame_with_col_no)//如果需要列标
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
	else//如果上来一开始就是画框，那么直接从start_x开始，不用打extern_left_cols的空格了
	{
		if (2 * pCGI->draw_frame_with_row_no != 0)
			cout << setw(2 * pCGI->draw_frame_with_row_no) << " ";
	}
	cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
	cout << pCGI->CFI.top_left;//╔
	for (int j1 = 1; j1 <= pCGI->col_num - 1; j1++)
	{
		for (int n1 = 1; n1 <= pCGI->CFI.block_width / 2; n1++)
			cout << pCGI->CFI.h_normal;//═
		if (pCGI->CFI.separator)
			cout << pCGI->CFI.h_top_separator;//╦
		Sleep(pCGI->delay_of_draw_frame);
	}
	for (int n1 = 1; n1 <= pCGI->CFI.block_width / 2; n1++)
		cout << pCGI->CFI.h_normal;
	cout << pCGI->CFI.top_right;//╗
	cout << endl;//上边框输出完成
	for (int i2 = 1; i2 <= pCGI->row_num; i2++)
	{
		for (int n2 = 1; n2 <= pCGI->CFI.block_high; n2++)
		{
			//画每一行的开头部分（行标加额外空出的列）
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
			//开始画格子背景部分
			cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			cout << pCGI->CFI.v_normal;//║
			for (int j3 = 1; j3 <= pCGI->col_num; j3++)//按照色块列来画
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
		if (pCGI->CFI.separator)//画分界线
		{
			cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
			if (pCGI->extern_left_cols + 2 * pCGI->draw_frame_with_row_no != 0)
				cout << setw(pCGI->extern_left_cols + 2 * pCGI->draw_frame_with_row_no) << " ";
			cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			if (i2 != pCGI->row_num)
				cout << pCGI->CFI.v_left_separator;//╠
			else
				cout << pCGI->CFI.lower_left;//╚
			for (int j4 = 1; j4 <= pCGI->col_num; j4++)
			{
				for (int n3 = 1; n3 <= pCGI->CFI.block_width / 2; n3++)
					cout << pCGI->CFI.h_normal;//═
				if (j4 != pCGI->col_num)
				{
					if (i2 != pCGI->row_num)
						cout << pCGI->CFI.mid_separator;//╬
					else
						cout << pCGI->CFI.h_lower_separator;//╩
				}
				else
				{
					if (i2 != pCGI->row_num)
						cout << pCGI->CFI.v_right_separator << endl;//╣
					else
						cout << pCGI->CFI.lower_right << endl;//╝
				}
				Sleep(pCGI->delay_of_draw_frame);
			}//有分界线的已经画完
		}
	}
	if (pCGI->CFI.separator == 0)//画无分界线的最下边一条边框
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
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：在状态栏上显示信息
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int type							：指定是上/下状态栏
		   const char *msg						：正常信息
		   const char *catchy_msg					：需要特别标注的信息（在正常信息前显示）
  返 回 值：
  说    明：1、输出宽度限定为主框架的宽度（含行号列标位置），超出则截去
            2、如果最后一个字符是某汉字的前半个，会导致后面乱码，要处理
***************************************************************************/
int gmw_status_line(const CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const char *msg, const char *catchy_msg)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
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
	cout << setw(pCGI->cols - pCGI->extern_left_cols - pCGI->extern_right_cols) << " ";//先清空这一行

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
	

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：显示某一个色块(内容为字符串，坐标为row/col)
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int row_no						：行号（从0开始，人为保证正确性，程序不检查）
		   const int col_no						：列号（从0开始，人为保证正确性，程序不检查）
		   const int bdi_value						：需要显示的值
		   const BLOCK_DISPLAY_INFO *const bdi		：存放该值对应的显示信息的结构体数组
  返 回 值：
  说    明：1、BLOCK_DISPLAY_INFO 的含义见头文件，用法参考测试样例
            2、bdi_value为 BDI_VALUE_BLANK 表示空白块，要特殊处理
***************************************************************************/
int gmw_draw_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO *const bdi)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	cct_setcursor(CURSOR_INVISIBLE);
	int x0 = pCGI->extern_left_cols + 2 * pCGI->draw_frame_with_row_no + 2 + col_no * pCGI->CFI.bwidth;
	int y0 = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no + 1 + row_no * pCGI->CFI.bhigh;
	cct_gotoxy(x0, y0);
	if (bdi_value == BDI_VALUE_BLANK)//画空白块
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
	}//计算数字长度
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
	if (pCGI->CBI.block_border) {//如果色块有外围线
		cout << pCGI->CBI.top_left;//╔
		for (int i = 1; i <= (pCGI->CFI.block_width - 2 * 2) / 2; i++)
		{
			cout << pCGI->CBI.h_normal;//═
		}
		cout << pCGI->CBI.top_right;//╗
		for (int i1 = 2; i1 <= pCGI->CFI.block_high - 1; i1++)
		{
			cct_getxy(x, y);
			cct_gotoxy(x - pCGI->CFI.block_width, y + 1);
			cout << pCGI->CBI.v_normal;//║
			if (i1 == (pCGI->CFI.block_high + 1) / 2)
			{
				if (bdi[no].content == NULL)//如果输出的是内部值
				{
					if ((pCGI->CFI.block_width - (value_len + 1) / 2 * 2 - 2 * 2) / 2 > 0)
						cout << setw((pCGI->CFI.block_width - (value_len + 1) / 2 * 2 - 2 * 2) / 2) << " ";
					cout << setiosflags(ios::left) << setw((value_len + 1) / 2 * 2) << bdi[no].value;
					if ((pCGI->CFI.block_width - (value_len + 1) / 2 * 2 - 2 * 2) / 2 > 0)
						cout << setw((pCGI->CFI.block_width - (value_len + 1) / 2 * 2 - 2 * 2) / 2) << " ";
				}
				else//如果输出的是指定内容
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
			cout << pCGI->CBI.v_normal;//║
			Sleep(pCGI->delay_of_draw_block);
		}
		cct_getxy(x, y);
		cct_gotoxy(x - pCGI->CFI.block_width, y + 1);
		cout << pCGI->CBI.lower_left;//╚
		for (int i = 1; i <= (pCGI->CFI.block_width - 4) / 2; i++)
		{
			cout << pCGI->CBI.h_normal;//═
		}
		cout << pCGI->CBI.lower_right;//╝
	}
	else//方块无外围线
	{
		for (int i = 1; i <= pCGI->CFI.block_high; i++)
		{
			if (i == (pCGI->CFI.block_high + 1) / 2) 
			{
				if (bdi[no].content == NULL)//如果输出的是内部值
				{
					if ((pCGI->CFI.block_width - (value_len + 1) / 2 * 2) / 2 > 0)
						cout << setw((pCGI->CFI.block_width - (value_len + 1) / 2 * 2) / 2) << " ";
					cout << setiosflags(ios::left) << setw((value_len + 1) / 2 * 2) << bdi[no].value;
					if ((pCGI->CFI.block_width - (value_len + 1) / 2 * 2) / 2 > 0)
						cout << setw((pCGI->CFI.block_width - (value_len + 1) / 2 * 2) / 2) << " ";
				}
				else//如果输出的是指定内容
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
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：移动某一个色块
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int row_no						：行号（从0开始，人为保证正确性，程序不检查）
		   const int col_no						：列号（从0开始，人为保证正确性，程序不检查）
		   const int bdi_value						：需要显示的值
		   const int blank_bdi_value				：移动过程中用于动画效果显示时用于表示空白的值（一般为0，此处做为参数代入，是考虑到可能出现的特殊情况）
		   const BLOCK_DISPLAY_INFO *const bdi		：存放显示值/空白值对应的显示信息的结构体数组
		   const int direction						：移动方向，一共四种，具体见cmd_gmw_tools.h
		   const int distance						：移动距离（从1开始，人为保证正确性，程序不检查）
  返 回 值：
  说    明：
***************************************************************************/
int gmw_move_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO *const bdi, const int direction, const int distance)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	int x0, y0;
	x0 = pCGI->extern_left_cols + 2 * pCGI->draw_frame_with_row_no + 2 + col_no * pCGI->CFI.bwidth;
	y0 = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no + 1 + row_no * pCGI->CFI.bhigh;
	cct_gotoxy(x0, y0);//到方块左上角
	if (direction == UP_TO_DOWN)
	{
		for (int t = 1; t <= distance; t++)//移动distance次一小格
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
			if (pCGI->CFI.separator)//如果有分隔线，补齐上面的分隔线，并走到当前方块左上角
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
			else//没有分隔线，直接走到当前方块左上角
			{
				cct_getxy(x, y);//当前位于方块右下角
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
			else//如果没有分隔线，直接去到当前方块位置的左上角
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
			if (pCGI->CFI.separator)//如果有分隔线，需要把右边封起来
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
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：读键盘或鼠标
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   int &MAction							：如果返回 CCT_MOUSE_EVENT，则此值有效，为 MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK/MOUSE_RIGHT_BUTTON_CLICK 三者之一
		                                               如果返回 CCT_KEYBOARD_EVENT，则此值无效
		   int &MRow								：如果返回 CCT_MOUSE_EVENT 且 MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK，则此值有效，表示左键选择的游戏区域的行号（从0开始）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &MCol								：如果返回 CCT_MOUSE_EVENT 且 MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK，则此值有效，表示左键选择的游戏区域的列号（从0开始）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &KeyCode1							：如果返回 CCT_KEYBOARD_EVENT，则此值有效，为读到的键码（如果双键码，则为第一个）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &KeyCode2							：如果返回 CCT_KEYBOARD_EVENT，则此值有效，为读到的键码（如果双键码，则为第二个，如果是单键码，则为0）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   const bool update_lower_status_line		：鼠标移动时，是否要在本函数中显示"[当前光标] *行*列/位置非法"的信息（true=显示，false=不显示，缺省为true）
  返 回 值：函数返回约定
		   1、如果是鼠标移动，得到的MRow/MCol与传入的相同(鼠标指针微小的移动)，则不返回，继续读
							  得到行列非法位置，则不返回，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] 位置非法"
							  得到的MRow/MCol与传入的不同(行列至少一个变化)，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] *行*列"，再返回MOUSE_ONLY_MOVED（有些游戏返回后要处理色块的不同颜色显示）
		   2、如果是按下鼠标左键，且当前鼠标指针停留在主游戏区域的*行*列上，则返回 CCT_MOUSE_EVENT ，MAction 为 MOUSE_LEFT_BUTTON_CLICK, MRow 为行号，MCol 为列标
		                          且当前鼠标指针停留在非法区域（非游戏区域，游戏区域中的分隔线），则不返回，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] 位置非法"
		   3、如果是按下鼠标右键，则不判断鼠标指针停留区域是否合法，直接返回 CCT_MOUSE_EVENT ，MAction 为 MOUSE_RIGHT_BUTTON_CLICK, MRow、MCol取当前值（因为消灭星星的右键标记需要坐标）
		   4、如果按下键盘上的某键（含双键码按键），则直接返回 CCT_KEYBOARD_EVENT，KeyCode1/KeyCode2中为对应的键码值
 说    明：通过调用 cmd_console_tools.cpp 中的 read_keyboard_and_mouse 函数实现
***************************************************************************/
int gmw_read_keyboard_and_mouse(const CONSOLE_GRAPHICS_INFO *const pCGI, int &MAction, int &MRow, int &MCol, int &KeyCode1, int &KeyCode2, const bool update_lower_status_line)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
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
			if (findcol == true)//如果找到了列数
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
					gmw_status_line(pCGI, LOWER_STATUS_LINE, "[当前光标] 位置非法");
					continue;
				}
			}
			if (mac == MOUSE_ONLY_MOVED && hefa == 1)
			{
				if (update_lower_status_line)
				{
					char temp[256] = { '\0' };
					sprintf(temp, "[当前光标] %c行%d列", char(MRow + 'A'), MCol);
					gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
				}
				if (MRow != mrow || MCol != mcol)//鼠标在合法位置且与上一次位置不同
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
