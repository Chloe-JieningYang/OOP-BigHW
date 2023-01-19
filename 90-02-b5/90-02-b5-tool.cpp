/* 2050598 通信 杨洁宁*/
#define _CRT_SECURE_NO_WARNINGS
#include <io.h>
#include <vector>
#include <conio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "../include/common-readtool-menu.h"
#include "../include/cmd_console_tools.h"
#include "90-02-b5-save.h"
#include "90-02-b5-tool.h"
using namespace std;

/***************************************************************************
  函数名称：
  功    能：选择一本书
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int ChooseBook(const vector<string>& files)
{
	int file_num = files.size();
	int chosen_no = 0;//被选中的书号
	int mx, my, mac, k1, k2;
	int top_no = 0;//目前待在菜单最顶上的是第几个文件
	string bookname = BookName(files, chosen_no);

	/*初始状态把最上面的选中*/
	cct_gotoxy(LEFT_EXT_COL + 2, TOP_EXT_LINE + 1);
	ShowCatalogueBookName(bookname, 1);

	while (1)//不停读键
	{
		int event= cct_read_keyboard_and_mouse(mx, my, mac, k1, k2);
		if (event != CCT_KEYBOARD_EVENT)
			continue;
		if (k1 == 0xe0 && k2 == KB_ARROW_UP)
		{
			if (chosen_no != 0)
			{
				if (chosen_no > top_no)//如果上面还有，还没到翻页的时候
				{
					/*先把原先的恢复颜色*/
					cct_gotoxy(LEFT_EXT_COL + 2, TOP_EXT_LINE + 1 + chosen_no - top_no);
					bookname = BookName(files, chosen_no);
					ShowCatalogueBookName(bookname, 0);
					/*再涂新的*/
					cct_gotoxy(LEFT_EXT_COL + 2, TOP_EXT_LINE + 1 + chosen_no - 1 - top_no);
					bookname = BookName(files, chosen_no - 1);
					ShowCatalogueBookName(bookname, 1);
				}
				else//要翻页了
				{
					top_no--;
					ClearScreen();
					ShowCatalogue(files, file_num, top_no);
					cct_gotoxy(LEFT_EXT_COL + 2, TOP_EXT_LINE + 1);
					bookname = BookName(files, top_no);
					ShowCatalogueBookName(bookname, 1);
				}
				chosen_no--;
			}
		}
		else if (k1 == 0xe0 && k2 == KB_ARROW_DOWN)
		{
			if (chosen_no != file_num - 1)
			{
				if (chosen_no < top_no + MAX_LINE - 1)//无需翻页
				{
					/*先把原先的恢复颜色*/
					cct_gotoxy(LEFT_EXT_COL + 2, TOP_EXT_LINE + 1 + chosen_no - top_no);
					bookname = BookName(files, chosen_no);
					ShowCatalogueBookName(bookname, 0);
					/*再涂新的*/
					cct_gotoxy(LEFT_EXT_COL + 2, TOP_EXT_LINE + 1 + chosen_no + 1 - top_no);
					bookname = BookName(files, chosen_no + 1);
					ShowCatalogueBookName(bookname, 1);
				}
				else//得翻页
				{
					top_no++;
					ClearScreen();
					ShowCatalogue(files, file_num, top_no);
					cct_gotoxy(LEFT_EXT_COL + 2, TOP_EXT_LINE + 1 + MAX_LINE - 1);
					bookname = BookName(files, top_no + MAX_LINE - 1);
					ShowCatalogueBookName(bookname, 1);
				}
				chosen_no++;
			}
		}
		else if (k1 == '\r')
		{
			break;
		}
		else if (k1 == 'Q' || k1 == 'q')//退出
		{
			return -1;
		}
	}
	cct_gotoxy(0, TOP_EXT_LINE + 1 + MAX_LINE + 6);
	cout << "选择了" << BookName(files, chosen_no).c_str() << ".txt文件" << endl;

	return chosen_no;
}

/***************************************************************************
  函数名称：
  功    能：遇到换行符才换行，给位置判断内容框里第一行的第一个字符的位置
  输入参数：输入流，记录的位置，输出的方式
  返 回 值：
  说    明：
***************************************************************************/
streampos GetFirstCharPos(ifstream& infile,const streampos pos, const int choice)
{
	infile.clear();
	infile.seekg(pos, ios::beg);
	streampos bugp1 = infile.tellg();
	//infile.seekg(-2, ios::cur);//移动文件指针到要求位置的前两个位置

	bool endlmark = 0;//前面是换行符的标志

	long long chinese = 0;//数中文字符的个数（单个字 数值+2）
	bool last_chinese = 0;//判断上一个字符是不是中文

	if (choice == MEET_ENDL)//如果是碰到换行符就换行
	{
		while (1)//默认到换行符换行，所以要找到换行符计算当前第一个
		{
			char ch = infile.peek();
			streampos bugp2 = infile.tellg();
			if (ch == '\n' || infile.tellg() == 0)
			{
				if (ch == '\n')
				{
					infile.seekg(1, ios::cur);//如果找到换行符，指针需要往后一个位置
					endlmark = 1;
				}
				break;
			}
			infile.seekg(-1, ios::cur);
		}
	}
	else
	{
		while (1)//默认到换行符+空格换行，所以要找到换行符计算当前第一个
		{
			char ch = infile.peek();
			if (ch == EOF)
				infile.seekg(-1, ios::end);
			if (infile.tellg() == 0)//如果到了文件开头
				break;
			if (ch == '\n')
			{
				infile.seekg(1, ios::cur);//如果找到换行符，指针需要往后一个位置
				unsigned char next = infile.peek();
				if (next == ' ' || next == '\t' || next == 0xA1)//如果换行符后面是空格，换行
				{
					endlmark = 1;
					break;
				}
				else
					infile.seekg(-1, ios::cur);//把指针移回去
			}
			infile.seekg(-1, ios::cur);
		}
	}
	streampos screen_start = infile.tellg();
	int screenpos = 0;//记录在显示框中的位置（从1开始数）
	if (choice == MEET_ENDL)//碰到换行就换行
	{
		//if (endlmark == 1 && infile.peek() != ' ' && infile.peek() != '\t' && (unsigned char)infile.peek() != 0xA1)//如果换行符后面没有空格，添加空格
		if (endlmark == 1)
			screenpos = 4;
		

		while (infile.tellg() < pos)
		{
			unsigned char ch = infile.peek();
			if (ch >= 128 && ch <= 256)
			{
				chinese++;
				last_chinese = 1;
			}
			else if (last_chinese == 1 && chinese % 2 == 1)
			{
				chinese++;
				last_chinese = 1;
			}
			else
				last_chinese = 0;

			screenpos++;

			if (screenpos >= MAX_COL)
			{
				if (chinese % 2 == 1)//当前字符是汉字的前半个字
				{
					infile.seekg(-1, ios::cur);//文件指针往前移一个
				}
				screenpos = 0;
				chinese = 0;
				screen_start = infile.tellg() + (long long)1;
			}
			infile.seekg(1, ios::cur);
		}
	}
	else//换行符后面有空格才换行
	{
		while (infile.tellg() < pos)
		{
			unsigned char c = infile.peek();
			if (c >= 128 && c <= 256)
			{
				chinese++;
				last_chinese = 1;
			}
			else if (last_chinese == 1 && chinese % 2 == 1)
			{
				chinese++;
				last_chinese = 1;
			}
			else
				last_chinese = 0;

			if (c != '\r' && c != '\n')//换行符不占位置
				screenpos++;

			if (screenpos >= MAX_COL)
			{
				if (chinese % 2 == 1)//当前字符是汉字的前半个字
				{
					infile.seekg(-1, ios::cur);//文件指针往前移一个
				}
				screenpos = 0;
				chinese = 0;
				screen_start = infile.tellg() + (long long)1;
			}

			infile.seekg(1, ios::cur);
		}
	}
	return screen_start;
}

/***************************************************************************
  函数名称：
  功    能：根据第一行第一个字符的位置输出,碰到换行就换行，空白自己添加
  输入参数：输入流，第一行第一个字符的位置，记录第二行、最后一行首字符开始位置
  返 回 值：
  说    明：
***************************************************************************/
int OutputScreenEndl(ifstream& infile, const streampos pos, streampos& secondline,streampos& lastline)
{
	int line = 1;//记录目前所在行数
	bool endlmark = 0;//读入换行符下一行需要空四格的标记
	int end = 0;

	ClearScreen();//先清屏

	if (pos == 0)
	{
		infile.seekg(pos, ios::beg);
		endlmark = 0;
	}
	else
	{
		infile.seekg(pos, ios::beg);
		infile.seekg(-1, ios::cur);//往前移一个看看是否是换行符或者空白
		char ch1 = infile.peek();
		infile.seekg(1, ios::cur);//移回来
		unsigned char ch2 = infile.peek();
		//if (ch1 == '\n' && ch2 != ' ' && ch2 != '\t' && ch2 != 0xA1)
		if (ch1 == '\n')
			endlmark = 1;//前面得留空格
		else
			endlmark = 0;
	}
	

	while (line <= MAX_LINE && infile.peek() != EOF)//循环读入一行的操作
	{
		if (line == 2)
			secondline = infile.tellg();
		if (line == MAX_LINE)
			lastline = infile.tellg();

		/*读入一行*/
		int chinese = 0;//记录一行中的中文字符个数
		char ch[MAX_COL + 1] = { 0 };

		//判断一行该读入多少字符

		int max;

		if (endlmark == 1 && line == 1)
		{
			max = MAX_COL - 4;
			cct_gotoxy(LEFT_EXT_COL + 6, TOP_EXT_LINE + line);
		}
		else if (endlmark == 1 && line > 1)
		{
			unsigned char c = infile.peek();
			//if (c != '\t' && c != ' ' && c != 0xA1)
			
			max = MAX_COL - 4;
			cct_gotoxy(LEFT_EXT_COL + 6, TOP_EXT_LINE + line);
			
			/*
			else
			{
				max = MAX_COL;
				cct_gotoxy(LEFT_EXT_COL + 2, TOP_EXT_LINE + line);
			}
			*/
		}
		else
		{
			max = MAX_COL;
			cct_gotoxy(LEFT_EXT_COL + 2, TOP_EXT_LINE + line);
		}

		endlmark = 0;


		for (int k = 0; k < max; k++)
		{
			if (infile.peek() == EOF)
			{
				end = 1;
				lastline = infile.tellg() - (long long)1;
				break;
			}
			ch[k] = infile.get();
			if ((unsigned char)ch[k] >= 128 && (unsigned char)ch[k] <= 256)
				chinese++;
			else if (k >= 1 && (unsigned char)ch[k - 1] >= 128 && (unsigned char)ch[k - 1] <= 256 && chinese % 2 == 1)
				chinese++;
			if (ch[k] == '\n')//读到换行要结束
			{
				break;
			}
		}

		int len = strlen(ch);

		if (len == 1 && ch[0] == '\n' || len == 2 && ch[0] == '\r' && ch[1] == '\n')//读到无内容空行，不输出
		{
			endlmark = 1;
			line++;
			continue;
		}

		if (chinese % 2 == 1)//多出来汉字前一半
		{
			ch[len - 1] = '\0';
			infile.seekg(-1, ios::cur);
		}


		if (len >= 1 && ch[len - 1] == '\n')
		{
			endlmark = 1;//下一行开头空格
			ch[len - 1] = '\0';
		}
		if (len >= 2 && ch[len - 2] == '\r')
			ch[len - 2] = '\0';

		cout << ch << endl;//输出一行

		line++;
	}
	return end;
}

/***************************************************************************
  函数名称：
  功    能：根据第一行第一个字符的位置输出,碰到换行符+空白才换行，单单换行符不换行
  输入参数：输入流，第一行第一个字符的位置，记录第二行、最后一行首字符开始位置
  返 回 值：已经读到文件末尾置1，未读到置0
  说    明：
***************************************************************************/
int OutputScreenEndlAndBlank(ifstream& infile, const streampos pos, streampos& secondline, streampos& lastline)
{
	int line = 1;//记录目前所在行数
	int end = 0;

	ClearScreen();//先清屏

	
	infile.seekg(pos, ios::beg);
	

	while (line <= MAX_LINE && infile.peek() != EOF)//循环读入一行并输出的操作
	{
		if (line == 2)
			secondline = infile.tellg();
		if (line == MAX_LINE)
			lastline = infile.tellg();

		/*读入一行*/
		int chinese = 0;//记录一行中的中文字符个数
		char ch[MAX_COL + 1] = { 0 };

		cct_gotoxy(LEFT_EXT_COL + 2, TOP_EXT_LINE + line);

		for (int k = 0; k < MAX_COL;)
		{
			if (infile.peek() == EOF)
			{
				lastline = infile.tellg() - (long long)1;
				end = 1;
				break;
			}

			ch[k] = infile.get();
			if ((unsigned char)ch[k] >= 128 && (unsigned char)ch[k] <= 256)
				chinese++;
			else if (k >= 1 && (unsigned char)ch[k - 1] >= 128 && (unsigned char)ch[k - 1] <= 256 && chinese % 2 == 1)
				chinese++;

			if (ch[k] == '\n' && (infile.peek() == ' ' || infile.peek() == '\t' || (unsigned char)infile.peek() == 0xA1))//读到换行+空格要结束
			{
				break;
			}

			if (ch[k] != '\n' && ch[k] != '\r')
				k++;
		}//凡是换行符后必然有空白

		int len = strlen(ch);

		if (chinese % 2 == 1)//多出来汉字前一半
		{
			ch[len - 1] = '\0';
			infile.seekg(-1, ios::cur);
		}

		if (len >= 1 && ch[len - 1] == '\n')
		{
			ch[len - 1] = '\0';
		}
		if (len >= 2 && ch[len - 2] == '\r')
			ch[len - 2] = '\0';

		cout << ch << endl;//输出一行

		line++;
	}

	return end;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：文件最大指针
  返 回 值：返回文件指针
  说    明：
***************************************************************************/
streampos InputPos(const streampos endp)
{
	cct_setcursor(CURSOR_VISIBLE_NORMAL);
	double percent = 0;
	cct_gotoxy(0, TOP_EXT_LINE + HEIGHT + 4);
	cout << "请输入想跳转位置的百分比(0-100，最多两位小数) : ";
	cin >> percent;
	if (cin.fail() || percent < 0 || percent>100)
		percent = -1;//错误值置-1
	streampos start;
	start = (long long)((endp - (long long)1) * (percent / 100));
	cct_gotoxy(0, TOP_EXT_LINE + HEIGHT + 4);
	cout << setw(80) << setfill(' ') << " ";
	cct_setcursor(CURSOR_INVISIBLE);
	return start;
}

/***************************************************************************
  函数名称：
  功    能：读取一本书
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int OpenBook(const vector<string>& files)
{
	cct_setcursor(CURSOR_INVISIBLE);
	bool quit = 0;//标记退出
	int chosen_no = ChooseBook(files);//选择一本书
	/*清除提示*/
	cct_gotoxy(0, TOP_EXT_LINE + HEIGHT + 1);
	cout << setw(50) << setfill(' ') << " ";

	if (chosen_no < 0)
	{
		return -1;
	}
	int output_choice = MEET_ENDL_AND_BLANK;
	ifstream infile(files[chosen_no], ios::in | ios::binary);

	/*读取上次读到的位置文件*/
	
	streampos pos = GetPosSaving(BookName(files, chosen_no));//开始阅读的位置
	
	infile.seekg(0, ios::end);
	streampos endp = infile.tellg();

	/*如果开始位置太大或者非法*/
	if (pos >= endp || pos < 0)
		pos = 0;

	if (endp > 20 * 1024 * 1024)
	{
		cct_gotoxy(0, TOP_EXT_LINE + 1 + MAX_LINE + 5);
		cout << "文件超过20MB，无法打开" << endl;
		infile.close();
		return -1;
	}

	

	streampos screen_first_pos, second_line_pos, last_line_pos;
	/*打印初始位置开始的文本*/
	screen_first_pos = GetFirstCharPos(infile, pos, output_choice);
	if (output_choice == MEET_ENDL)
		OutputScreenEndl(infile, screen_first_pos, second_line_pos, last_line_pos);
	else if (output_choice == MEET_ENDL_AND_BLANK)
		OutputScreenEndlAndBlank(infile, screen_first_pos, second_line_pos, last_line_pos);

	/*打印提示*/
	cct_gotoxy(0, TOP_EXT_LINE + HEIGHT + 3);
	cout << "显示方式1:遇换行符就换行(都添加空格)/显示方式2:遇换行符+空格才换行" << endl;
	cct_gotoxy(0, TOP_EXT_LINE + HEIGHT + 2);
	cout << "上/下箭头移动一行,PgUp/PgDn翻页,S/s跳转进度,F/f改变显示格式,N/n返回,Q/q退出";
	cct_gotoxy(0, TOP_EXT_LINE + HEIGHT + 1);
	cout << "当前进度：" << setprecision(2) << (double)screen_first_pos / endp * 100 << "%";
	if (output_choice == MEET_ENDL_AND_BLANK)
		cout << "  显示方式:2" << endl;
	else
		cout << "  显示方式:1" << endl;

	int end = 0;
	/*读取操作*/
	while (1)//重复读键
	{ 
		
		int mx, my, mac, k1, k2;
		int event = cct_read_keyboard_and_mouse(mx, my, mac, k1, k2);
		if (event != CCT_KEYBOARD_EVENT)
			continue;

		if (k1 == 0xe0 && k2 == KB_ARROW_DOWN)//第二行变成首行
		{
			streampos old = GetFirstCharPos(infile, pos, output_choice);
			while (1)
			{
				if (pos + (long long)2 >= endp)
					break;
				pos += 1;
				if (GetFirstCharPos(infile, pos, output_choice) != old)
					break;
			}
		}
		else if (k1 == 0xe0 && k2 == KB_ARROW_UP)
		{
			streampos old = GetFirstCharPos(infile, pos, output_choice);
			while (1)
			{
				if (pos - (long long)2 <=0 )
					break;
				pos -= 1;
				if (GetFirstCharPos(infile, pos, output_choice) != old)
					break;
			}
		}
		else if (k1 == 0xe0 && k2 == KB_PGDN)//最后一行变成首行
		{
			int count_line = 0;
			streampos old = GetFirstCharPos(infile, pos, output_choice);
			while (1)
			{
				if (pos + (long long)2 >= endp)
					break;
				pos += 1;
				if (GetFirstCharPos(infile, pos, output_choice)!=old)
				{
					old = GetFirstCharPos(infile, pos, output_choice);
					count_line++;
				}
				if (count_line == MAX_LINE - 1)
					break;
			}
		}
		else if (k1 == 0xe0 && k2 == KB_PGUP)//首行变成最后一行
		{
			int count_line = 0;
			streampos old = GetFirstCharPos(infile, pos, output_choice);
			while (1)
			{
				if (pos - (long long)2 <= 0)
					break;
				pos -= 1;
				if (GetFirstCharPos(infile, pos, output_choice) != old)
				{
					count_line++;
					old = GetFirstCharPos(infile, pos, output_choice);
				}
				if (count_line == MAX_LINE - 1)
					break;
			}
		}
		else if (k1 == 'f' || k1 == 'F')//F切换显示方式
		{
			if (output_choice == MEET_ENDL)
				output_choice = MEET_ENDL_AND_BLANK;
			else if (output_choice == MEET_ENDL_AND_BLANK)
				output_choice = MEET_ENDL;
		}
		else if (k1 == 's' || k1 == 'S')
		{
			streampos temp = InputPos(endp);
			if (temp >= 0)
				pos = temp;
		}
		else if (k1 == 'N' || k1 == 'n')
		{
			WritePos(BookName(files, chosen_no), screen_first_pos);
			break;
		}
		else if (k1 == 'Q' || k1 == 'q')
		{
			WritePos(BookName(files, chosen_no), screen_first_pos);
			quit = 1;
			break;
		}

		screen_first_pos = GetFirstCharPos(infile, pos, output_choice);
	
		if (output_choice == MEET_ENDL)
			end = OutputScreenEndl(infile, screen_first_pos, second_line_pos, last_line_pos);
		else if (output_choice == MEET_ENDL_AND_BLANK)
			end = OutputScreenEndlAndBlank(infile, screen_first_pos, second_line_pos, last_line_pos);
		
		cct_gotoxy(0, TOP_EXT_LINE + HEIGHT + 1);
		cout << setw(80) << setfill(' ') << " ";
		cct_gotoxy(0, TOP_EXT_LINE + HEIGHT + 1);
		cout << "当前进度：" << setiosflags(ios::fixed) << setprecision(2) << (double)screen_first_pos / endp * 100 << "%";
		if (output_choice == MEET_ENDL_AND_BLANK)
			cout << "  显示方式:2" << endl;
		else
			cout << "  显示方式:1" << endl;
	}
	infile.close();

	cct_gotoxy(0, TOP_EXT_LINE + HEIGHT + 1);
	cout << setw(80) << setfill(' ') << " ";
	cct_gotoxy(0, TOP_EXT_LINE + HEIGHT + 2);
	cout << setw(80) << setfill(' ') << " ";
	cct_gotoxy(0, TOP_EXT_LINE + HEIGHT + 3);
	cout << setw(80) << setfill(' ') << " ";

	if (quit == 1)
		return -1;

	return 1;
}