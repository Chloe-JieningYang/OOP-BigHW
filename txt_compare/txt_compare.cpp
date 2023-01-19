/*2050598 杨洁宁 通信*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include "../include/class_aat.h"
#include "txt_compare.h"
using namespace std;

/* 允许添加需要的内容 */

/*将字符数组中开头的空格和\t删除*/
static void trim_left(char ch[])
{
	if (ch[0] != ' ' && ch[0] != '\t')
		return;
	int pos = 0;
	while (1)
	{
		if (ch[pos] != ' ' && ch[pos] != '\t')
			break;
		else
			pos++;
	}
	if (ch[pos] == '\0')
		ch[0] = '\0';
	else
		strcpy(ch, ch + pos);
	int len = strlen(ch);
	for (int n = len; n <= MAX_SIZE - 1; n++)
		ch[n] = '\0';
}

/*将字符数组中结尾的空格和\t删除*/
static void trim_right(char ch[])
{
	int pos = strlen(ch) - 1;
	int pos2 = strlen(ch);
	if (pos2 == 0)
		return;
	while (pos >= 0)
	{
		if (ch[pos] != ' ' && ch[pos] != '\t')
		{
			if (ch[pos] == '\n')
				pos2 = pos;
			else if (ch[pos] == '\r')
			{
				if (pos < (int)strlen(ch) - 1 && ch[pos + 1] == '\n')
					pos2 = pos;
				else
					break;
			}
			else
				break;
		}
		pos--;
	}
	/*pos指向第一个不是空格或换行的位置*/
	/*pos2指向行尾换行符位置*/
	if (pos == (int)strlen(ch) - 1 || pos + 1 == pos2)
		return;
	
	strcpy(ch + pos + 1, ch + pos2);
}

/*处理CR_CRLF不同,全处理成单独\n*/
static void CR_CRLF(char ch[])
{
	int len = strlen(ch);
	if (len < 2)
		return;
	if (ch[len - 1] == '\n' && ch[len - 2] == '\r')
	{
		ch[len - 2] = '\n';
		ch[len - 1] = '\0';
	}
}

static void instruction(const args_analyse_tools args[])
{
	if (!args[DISPLAY].existed() || args[DISPLAY].get_string() == "none")
		return;
	cout << endl;
	cout << "阅读提示：" << endl;
	cout << "        2、文件结束用<EOF>标出" << endl;
	cout << "        3、两行相同列位置的差异字符用亮色标出" << endl;
	cout << "        4、每行中的CR/LF/VT/BS/BEL用X标出(方便看清隐含字符)" << endl;
	cout << "        5、每行尾的多余的字符用亮色标出，CR/LF/VT/BS/BEL用亮色X标出(方便看清隐含字符)" << endl;
	cout << "        6、每行最后用<CR>标出(方便看清隐含字符)" << endl;
	cout << "        7、中文因为编码问题，差异位置可能报在后半个汉字上，但整个汉字都亮色标出" << endl;
	if (args[DISPLAY].get_string() == "normal")
		cout << "        8、用--display detailed可以得到更详细的信息" << endl;
}

/*用于比较的函数*/
static int compare(const args_analyse_tools args[])
{
	ifstream infile1(args[FILE1].get_string(), ios::in | ios::binary);
	if (!infile1.is_open())
	{
		cout << "第1个文件[" << args[FILE1].get_string() << "]无法打开." << endl;
		return -1;
	}
	ifstream infile2(args[FILE2].get_string(), ios::in | ios::binary);
	if (!infile2.is_open())
	{
		cout << "第2个文件[" << args[FILE2].get_string() << "]无法打开." << endl;
		return -1;
	}

	/*用于行的计数*/
	int line_f1 = 0;
	int line_f2 = 0;

	/*用于判断文件是否已经结束*/
	bool f1_eof = 0;
	bool f2_eof = 0;

	/*用于数比较下来有差异的行数*/
	int count_dif_line = 0;

	/*用于数比较了多少行*/
	int count_compare_line = 0;

	/*是不是在lineoffset里已经读完*/
	bool one_eof = 0;
	/*需要从不同行开始比较*/
	if (args[LINEOFFSET].existed())
	{
		if (args[LINEOFFSET].get_int() < 0)//负数表示忽略file1的前n行
		{
			for (int k = 1; k <= -args[LINEOFFSET].get_int(); k++)
			{
				if (infile1.peek() == EOF)
				{
					f1_eof = 1;
					break;
				}
				char rubbish1[MAX_SIZE + 2] = { 0 };
				infile1.getline(rubbish1, MAX_SIZE + 1);
				line_f1++;
				if (rubbish1[MAX_SIZE] != '\0')
				{
					cout << "文件[" << args[FILE1].get_string() << "]的第(" << k << ")行不符合要求，超过最大长度[" << MAX_SIZE << "]." << endl;
					return -1;
				}
			}
		}
		else if (args[LINEOFFSET].get_int() > 0)//正数表示忽略file2的前n行
		{
			for (int k = 1; k <= args[LINEOFFSET].get_int(); k++)
			{
				if (infile2.peek() == EOF)
				{
					f2_eof = 1;
					break;
				}
				char rubbish2[MAX_SIZE + 2] = { 0 };
				infile2.getline(rubbish2, MAX_SIZE + 1);
				line_f2++;
				if (rubbish2[MAX_SIZE] != '\0')
				{
					cout << "文件[" << args[FILE2].get_string() << "]的第(" << k << ")行不符合要求，超过最大长度[" << MAX_SIZE << "]." << endl;
					return -1;
				}
			}
		}
	}

	/*如果其中一个文件已经被读完*/
	if (f1_eof || f2_eof)
	{
		if (f1_eof && args[LINEOFFSET].existed() && args[LINEOFFSET].get_int() < 0)
			line_f1 = -args[LINEOFFSET].get_int();
		else if (f2_eof && args[LINEOFFSET].existed() && args[LINEOFFSET].get_int() > 0)
			line_f2 = args[LINEOFFSET].get_int();
		count_dif_line++;
		one_eof = 1;
	}

	/*开始读入要比较的内容*/
	
	while (infile1.eof() != 1 || infile2.eof() != 1)
	{
		int count1 = 0;//数第一个文件一行被读入的字符数
		int count2 = 0;//数第二个文件一行被读入的字符数
		char f1[1026] = { 0 };
		char f2[1026] = { 0 };

		
		/*file1读取一行*/
		while (1)
		{
			if (infile1.peek() == EOF)
			{			
				line_f1++;
				f1_eof = 1;
				break;
			}
			if (count1 >= MAX_SIZE)
			{
				cout << "文件[" << args[FILE1].get_string() << "]的第(" << line_f1 << ")行不符合要求，超过最大长度[" << MAX_SIZE << "]." << endl;
				return -1;
			}
			f1[count1] = infile1.get();
			if (f1[count1] != '\n')
				count1++;
			else
			{
				count1++;
				line_f1++;
				break;
			}
		}

		/*file2读取一行*/
		while (1)
		{
			if (infile2.peek() == EOF)
			{
				line_f2++;
				f2_eof = 1;
				break;
			}
			if (count2 >= MAX_SIZE)
			{
				cout << "文件[" << args[FILE2].get_string() << "]的第(" << line_f2 << ")行不符合要求，超过最大长度[" << MAX_SIZE << "]." << endl;
				return -1;
			}
			f2[count2] = infile2.get();
			if (f2[count2] != '\n')
				count2++;
			else
			{
				count2++;
				line_f2++;
				break;
			}
		}
		

		/*比较一行*/
		/*先处理trim，把空格或/t删除*/
		if (args[TRIM].existed())
		{
			if (args[TRIM].get_string() == "left")
			{
				trim_left(f1);
				trim_left(f2);
			}
			else if (args[TRIM].get_string() == "right")
			{
				trim_right(f1);
				trim_right(f2);
			}
			else if (args[TRIM].get_string() == "all")
			{
				trim_left(f1);
				trim_left(f2);
				trim_right(f1);
				trim_right(f2);
			}
		}/*处理完trim*/

		/*处理ignore_blank*/
		if (args[IGNORE_BLANK].existed())
		{
			/*第一个文件读到空行，就继续读下一行直到不为空为止*/
			if (f1[0] == '\n' || (f1[0] == '\r' && f1[1] == '\n'))
			{
				while (1)
				{
					count1 = 0;
								
					for (int n = 0; n < MAX_SIZE; n++)
						f1[n] = '\0';

					while (1)
					{
						if (infile1.peek() == EOF)
						{
							line_f1++;
							f1_eof = 1;
							break;
						}	

						if (count1 >= MAX_SIZE)
						{
							cout << "文件[" << args[FILE1].get_string() << "]的第(" << line_f1 << ")行不符合要求，超过最大长度[" << MAX_SIZE << "]." << endl;
							return -1;
						}
						f1[count1] = infile1.get();
						if (f1[count1] != '\n')
							count1++;
						else
						{
							count1++;
							line_f1++;
							break;
						}
					}

					if (args[TRIM].existed())//处理trim
					{
						if (args[TRIM].get_string() == "left")
						{
							trim_left(f1);
						}
						else if (args[TRIM].get_string() == "right")
						{
							trim_right(f1);
						}
						else if (args[TRIM].get_string() == "all")
						{
							trim_left(f1);;
							trim_right(f1);
						}
					}

					if (f1[0] == '\n' || (f1[0] == '\r' && f1[1] == '\n'))
						;
					else
						break;
				}
			}

			/*第二个文件读到空行，就继续读下一行直到不为空为止*/
			if (f2[0] == '\n' || (f2[0] == '\r' && f2[1] == '\n'))
			{
				while (1)
				{					
					count2 = 0;
					for (int n = 0; n < MAX_SIZE; n++)
						f2[n] = '\0';
					while (1)
					{
						if (infile2.peek() == EOF)
						{						
							line_f2++;
							f2_eof = 1;
							break;
						}

						if (count2 >= MAX_SIZE)
						{
							cout << "文件[" << args[FILE2].get_string() << "]的第(" << line_f2 << ")行不符合要求，超过最大长度[" << MAX_SIZE << "]." << endl;
							return -1;
						}
						f2[count2] = infile2.get();
						if (f2[count2] != '\n')
							count2++;
						else
						{
							count2++;
							line_f2++;
							break;
						}
					}

					if (args[TRIM].existed())//处理trim
					{
						if (args[TRIM].get_string() == "left")
						{
							trim_left(f2);
						}
						else if (args[TRIM].get_string() == "right")
						{
							trim_right(f2);
						}
						else if (args[TRIM].get_string() == "all")
						{
							trim_left(f2);;
							trim_right(f2);
						}
					}

					if (f2[0] == '\n' || (f2[0] == '\r' && f2[1] == '\n'))
						;
					else
						break;
				}
			}

			/*一个文件结束，另一个文件末尾只有换行*/
			if (f1_eof && !f2_eof && infile2.peek() == EOF && f2[(int)strlen(f2) - 1] == '\n')
			{
				int len2 = strlen(f2);
				f2_eof = 1;
				if (len2 - 2 >= 0 && f2[len2 - 2] == '\r')//处理掉\r\n
					f2[len2 - 2] = '\0';
				f2[len2 - 1] = '\0';
			}
			else if (f2_eof && !f1_eof && infile1.peek() == EOF && f1[(int)strlen(f1) - 1] == '\n')
			{
				int len1 = strlen(f1);
				f1_eof = 1;
				if (len1 - 2 >= 0 && f1[len1 - 2] == '\r')
					f1[len1 - 2] = '\0';
				f1[len1 - 1] = '\0';
			}
		}

		count_compare_line++;//比较的行数+1

		/*如果LF和CRLF被视作一样，就都处理成单独\n*/
		if (!args[CR_CRLF_NOT_EQUAL].existed())
		{
			CR_CRLF(f1);
			CR_CRLF(f2);
		}
		

		int pos = 0;
		int first_dif_pos = 0;//第一个存在差异的地方
		bool dif = 0;//此行是否有差异
		int linelen1 = strlen(f1);
		int linelen2 = strlen(f2);
		int minlen = (linelen1 < linelen2) ? linelen1 : linelen2;

		/*先检查两个字符串大小是否相同*/
		bool len_is_same = (linelen1 == linelen2) ? 1 : 0;

		

		while (pos < minlen && f1[pos] != '\n' && f2[pos] != '\n')//对此行在最小长度内的每个字符进行查验
		{
			if (f1[pos] != f2[pos])//如果有不同
			{
				if ((!args[DISPLAY].existed() || args[DISPLAY].get_string() == "none") && !args[DEBUG].existed())
				{
					cout << "文件不同." << endl;
					return 0;
				}
				else
				{
					if (dif == 0)//如果这一行之前没有差异
						first_dif_pos = pos;
					dif = 1;
					pos++;
				}
			}
			else
				pos++;
		}

		if (one_eof || dif == 1 || len_is_same == 0 || (f1_eof == 0 && f2_eof == 1) || (f1_eof == 1 && f2_eof == 0))//如果存在差异或长度不同，normal/detailed需要打印f1 f2的这一行
		{
			count_dif_line++;//有差异的行数+1

			int maxlen = (linelen1 > linelen2) ? linelen1 : linelen2;

			if (args[DISPLAY].existed() && args[DISPLAY].get_string() == "normal"||args[DEBUG].existed())//normal的打印
			{
				if (count_dif_line == 1)
				{
					cout << "比较结果输出：" << endl;
					cout << "====================" << endl;
				}

				if (dif)//如果是比较有差异
					cout << "第[" << line_f1 << " / " << line_f2 << "]行 - 第[" << first_dif_pos << "]个字符开始有差异：" << endl;
				else if(len_is_same == 0 && (maxlen-minlen != 1 || maxlen - minlen == 1 && f1_eof == f2_eof)|| one_eof)//是长度不同，有多余字符,且不只是多一个\n
				{
					cout << "第[" << line_f1 << " / " << line_f2 << "]行 - 文件";
					if (linelen1 > minlen||f2_eof && one_eof)
						cout << "1";
					else
						cout << "2";
					cout << "的尾部有多余字符：" << endl;
				}
				else if (f1_eof == 1 && f2_eof == 0)
				{
					cout << "第[" << line_f1 << " / " << line_f2 << "]行 - 文件1已结束/文件2仍有内容：" << endl;
				}
				else if (f1_eof == 0 && f2_eof == 1)
				{
					cout << "第[" << line_f1 << " / " << line_f2 << "]行 - 文件1仍有内容/文件2已结束：" << endl;
				}


				cout << "文件1 : ";
				int k = 0;
				while (k < linelen1)
				{
					if (f1[k] == '\n' && k == linelen1 - 1)//是行尾的\n
					{
						if (k == 0)
						{
							if(f1[k]!=f2[k])
								cct_setcolor(COLOR_HYELLOW, COLOR_HRED);
							cout << "<EMPTY>";
							cct_setcolor();
						}
						cout << "<CR>";
						break;
						
					}
					else if (f1[k] == '\v' || f1[k] == '\b' || f1[k] == '\r' || f1[k] == '\a')
					{
						if (f1[k] != f2[k] || k > linelen2 - 1)//不一样的或多出来的字符用不同颜色标记
							cct_setcolor(COLOR_HYELLOW, COLOR_HRED);
						cout << "X";
						cct_setcolor();
					}
					else
					{
						if (f1[k] != f2[k] || k > linelen2 - 1)//不一样的或多出来的字符用不同颜色标记
							cct_setcolor(COLOR_HYELLOW, COLOR_HRED);
						cout << f1[k];
						cct_setcolor();
					}
					k++;
				}
				if (f1_eof)
					cout << "<EOF>";
				cout << endl;

				cout << "文件2 : ";
				k = 0;
				while (k < linelen2)
				{
					if (f2[k] == '\n' && k == linelen2 - 1)
					{
						if (k == 0)
						{
							if (f1[k] != f2[k])
								cct_setcolor(COLOR_HYELLOW, COLOR_HRED);
							cout << "<EMPTY>";
							cct_setcolor();
						}
						cout << "<CR>";
						break;
					}
					else if (f2[k] == '\v' || f2[k] == '\b' || f2[k] == '\r' || f2[k] == '\a')
					{
						if (f1[k] != f2[k] || k > linelen1 - 1)//不一样的或多出来的字符用不同颜色标记
							cct_setcolor(COLOR_HYELLOW, COLOR_HRED);
						cout << "X";
						cct_setcolor();
					}
					else
					{
						if (f1[k] != f2[k] || k > linelen1 - 1)//不一样的或多出来的字符用不同颜色标记
							cct_setcolor(COLOR_HYELLOW, COLOR_HRED);
						cout << f2[k];
						cct_setcolor();
					}
					k++;
				}
				if (f2_eof)
					cout << "<EOF>";
				cout << endl << endl;
			}//normal 输出完成
			else if (args[DISPLAY].existed() && args[DISPLAY].get_string() == "detailed")//detailed 输出
			{
				if (count_dif_line == 1)
				{
					cout << "比较结果输出：" << endl;
					cout << setfill('=') << setw(100) << "=" << endl;
				}

				if (dif)//如果是比较有差异
					cout << "第[" << line_f1 << " / " << line_f2 << "]行 - 第[" << first_dif_pos << "]个字符开始有差异：" << endl;
				else if (len_is_same == 0 && (maxlen - minlen != 1 || maxlen - minlen == 1 && f1_eof == f2_eof) || one_eof)//是长度不同，有多余字符,且不只是多一个\n
				{
					cout << "第[" << line_f1 << " / " << line_f2 << "]行 - 文件";
					if (linelen1 > minlen)
						cout << "1";
					else
						cout << "2";
					cout << "的尾部有多余字符：" << endl;
				}
				else if (f1_eof == 1 && f2_eof == 0)
				{
					cout << "第[" << line_f1 << " / " << line_f2 << "]行 - 文件1已结束/文件2仍有内容：" << endl;
				}
				else if (f1_eof == 0 && f2_eof == 1)
				{
					cout << "第[" << line_f1 << " / " << line_f2 << "]行 - 文件1仍有内容/文件2已结束：" << endl;
				}

				cout << setw(8) << setfill(' ') << " ";
				cout << setw((maxlen / 10 + 1) * 10) << setfill('-') << "-" << endl;
				cout << setw(8) << setfill(' ') << " ";
				for (int n = 0; n <= maxlen / 10 ; n++)
				{
					cout << resetiosflags(ios::right);
					cout << setiosflags(ios::left) << setw(10) << n;
				}
				cout << endl;
				cout << setw(8) << setfill(' ') << " ";
				for (int n = 1; n <= maxlen / 10 + 1; n++)
				{
					cout << "0123456789";
				}
				cout << endl;

				cout << resetiosflags(ios::left);
				cout << setiosflags(ios::right) << setw(8) << setfill(' ') << " ";
				cout << setw((maxlen / 10 + 1) * 10) << setfill('-') << "-" << endl;

				cout << "文件1 : ";
				int k = 0;
				while (k < linelen1)
				{
					if ( f1[k] == '\n' && k == linelen1 - 1)//是行尾的\n
					{
						if (k == 0)
						{
							if (f1[k] != f2[k])
								cct_setcolor(COLOR_HYELLOW, COLOR_HRED);
							cout << "<EMPTY>";
							cct_setcolor();
						}
						cout << "<CR>";
						break;
					}
					else if (f1[k] == '\v' || f1[k] == '\b' || f1[k] == '\r' || f1[k] == '\a')
					{
						if (f1[k] != f2[k] || k > linelen2 - 1)//不一样的或多出来的字符用不同颜色标记
							cct_setcolor(COLOR_HYELLOW, COLOR_HRED);
						cout << "X";
						cct_setcolor();
					}
					else
					{
						if (f1[k] != f2[k] || k > linelen2 - 1)//不一样的或多出来的字符用不同颜色标记
							cct_setcolor(COLOR_HYELLOW, COLOR_HRED);
						cout << f1[k];
						cct_setcolor();
					}
					k++;
				}
				if (f1_eof)
					cout << "<EOF>";
				cout << endl;

				cout << "文件2 : ";
				k = 0;
				while (k < linelen2)
				{
					if (f2[k] == '\n' && k == linelen2 - 1)
					{
						if (k == 0)
						{
							if (f1[k] != f2[k])
								cct_setcolor(COLOR_HYELLOW, COLOR_HRED);
							cout << "<EMPTY>";
							cct_setcolor();
						}
						cout << "<CR>";
						break;
					}
					else if (f2[k] == '\v' || f2[k] == '\b' || f2[k] == '\r' || f2[k] == '\a')
					{
						if (f1[k] != f2[k] || k > linelen1 - 1)//不一样的或多出来的字符用不同颜色标记
							cct_setcolor(COLOR_HYELLOW, COLOR_HRED);
						cout << "X";
						cct_setcolor();
					}
					else
					{
						if (f1[k] != f2[k] || k > linelen1 - 1)//不一样的或多出来的字符用不同颜色标记
							cct_setcolor(COLOR_HYELLOW, COLOR_HRED);
						cout << f2[k];
						cct_setcolor();
					}
					k++;
				}
				if (f2_eof)
					cout << "<EOF>";
				cout << endl;

				cout << "文件1(HEX) :" << endl;
				
				int hex_line1 = 0;
				bool end1 = 0;
				while (1)
				{
					cout << setfill('0') << setw(7) << hex << hex_line1 << "0 : ";
					int j;
					for (j = 0; j <= 15; j++)
					{
						if (f1[hex_line1 * 16 + j] == '\0')//字符串结束
						{
							end1 = 1;
							break;
						}
						else
						{
							if (j == 8)
								cout << "- ";
							cout << setfill('0') << setw(2) << hex << (unsigned int)((unsigned char)f1[hex_line1 * 16 + j]);
							cout << " ";
							
						}
					}
					if (j == 16)
						cout << "  ";
					else if (j >= 8)
						cout << setfill(' ') << setw(2 + (16 - j) * 3) << " ";
					else
						cout << setfill(' ') << setw(4 + (16 - j) * 3) << " ";

					for (j = 0; j <= 15; j++)
					{
						if (f1[hex_line1 * 16 + j] == '\0')
							break;
						if (f1[hex_line1 * 16 + j] >= 33 && f1[hex_line1 * 16 + j] <= 126)
							cout << f1[hex_line1 * 16 + j];
						else
							cout << ".";
					}
					cout << endl;

					if (end1 == 1)
						break;

					hex_line1++;
				}//第1个文件的HEX输出结束

				cout << "文件2(HEX) :" << endl;

				int hex_line2 = 0;
				bool end2 = 0;
				while (1)
				{
					cout << setfill('0') << setw(7) << hex << hex_line2 << "0 : ";
					int j;
					for (j = 0; j <= 15; j++)
					{
						if (f2[hex_line2 * 16 + j] == '\0')//字符串结束
						{
							end2 = 1;
							break;
						}
						else
						{
							if (j == 8)
								cout << "- ";
							cout << setfill('0') << setw(2) << hex << (unsigned int)((unsigned char)f2[hex_line2 * 16 + j]);
							cout << " ";						
						}
					}
					if (j == 16)
						cout << "  ";
					else if (j >= 8)
						cout << setfill(' ') << setw(2 + (16 - j) * 3) << " ";
					else
						cout << setfill(' ') << setw(4 + (16 - j) * 3) << " ";

					for (j = 0; j <= 15; j++)
					{
						if (f2[hex_line2 * 16 + j] == '\0')
							break;
						if (f2[hex_line2 * 16 + j] >= 33 && f2[hex_line2 * 16 + j] <= 126)
							cout << f2[hex_line2 * 16 + j];
						else
							cout << ".";
					}
					cout << endl;

					if (end2 == 1)
						break;

					hex_line2++;
				}//第2个文件的HEX输出结束
				cout << endl;
			}//detailed输出结束
		}//if (dif == 1 || len_is_same == 0) 输出具体信息完成
		if (args[MAX_DIFF].existed() && count_dif_line >= args[MAX_DIFF].get_int())
			break;
		if (args[MAX_LINE].existed() && count_compare_line >= args[MAX_LINE].get_int())
			break;
		if (f1_eof || f2_eof || infile1.eof() || infile2.eof())
			break;
	}//完成文件的读取

	cout << setw(100) << setfill('=') << "=" << endl;
	if (count_dif_line != 0)
	{
		cout << "在指定检查条件下共" << count_dif_line << "行有差异";
		instruction(args);
		if (args[MAX_DIFF].existed() && count_dif_line == args[MAX_DIFF].get_int())
			cout << "[已到设定的最大差异值]";
		cout << "." << endl;
	}
	else
		cout << "在指定检查条件下完全一致." << endl;
	cout << setw(100) << setfill('=') << "=" << endl;

	infile1.close();
	infile2.close();
	return 0;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
static void usage(const char* const procname)
{
	ostringstream msg;
	const int wkey = 7 + strlen(procname) + 1;
	const int wopt = 7 + strlen(procname) + 4;
	cout << endl;

	msg << procname << "-Ver1.0.0" << endl;
	msg << endl;

	msg << "Usage: " << procname << " --file1 xxx --file2 xxx [ --trim none/left/rigth/all | --lineoffset -100..100 | --ignore_blank | --max_diff 0..100 | --display none/normal/detailed ]" << endl;

	msg << setw(wkey) << ' ' << "必选项：" << endl;
	msg << setw(wopt) << ' ' << "--file1 name1              : 指定要比较的第1个文件名（必选）" << endl;
	msg << setw(wopt) << ' ' << "--file2 name2              : 指定要比较的第2个文件名（必选）" << endl;
	msg << endl;

	msg << setw(wkey) << ' ' << "可选项：" << endl;
	msg << setw(wopt) << ' ' << "--trim none/left/right/all     : 指定每行的空格/tab的忽略方式（无此项则默认为none）" << endl;
	msg << setw(wopt) << ' ' << "\tnone   : 每行均严格匹配" << endl;
	msg << setw(wopt) << ' ' << "\tleft   : 每行均忽略头部空格" << endl;
	msg << setw(wopt) << ' ' << "\tright  : 每行均忽略尾部空格" << endl;
	msg << setw(wopt) << ' ' << "\tall    : 每行均忽略头尾空格" << endl;
	msg << setw(wopt) << ' ' << "--lineoffset n                 : 指定文件错位的行数（无此项则n为0）" << endl;
	msg << setw(wopt) << ' ' << "\t取值[-100..100]，负数表示忽略file1的前n行，正数表示忽略file2的前n行" << endl;
	msg << setw(wopt) << ' ' << "--ignore_blank                 : 忽略文件中--trim后的所有空行(无此项则不忽略)" << endl;
	msg << setw(wopt) << ' ' << "--CR_CRLF_not_equal            : 不忽略Windows/Linux文件的换行符差异(无此项则忽略差异)" << endl;
	msg << setw(wopt) << ' ' << "--max_diff m                   : 指定文件错位的行数（无此项则m为0）" << endl;
	msg << setw(wopt) << ' ' << "\t取值[0..100]，表示满m个不同行则结束运行，不再进行后续的比较" << endl;
	msg << setw(wopt) << ' ' << "--max_line x                   : 指定文件比较的最大行数（无此项则x为0）" << endl;
	msg << setw(wopt) << ' ' << "\t取值[0..10000]，表示比较x行后则结束运行，不再进行后续的比较" << endl;
	msg << setw(wopt) << ' ' << "--display none/normal/detailed : 指定显示的信息（无此项则默认为none）" << endl;
	msg << setw(wopt) << ' ' << "\tnone   : 仅一行匹配与否的提示" << endl;
	msg << setw(wopt) << ' ' << "\tnormal : 每个差异行给出差异信息" << endl;
	msg << setw(wopt) << ' ' << "\tright  : 每个差异行给出更详细的差异信息" << endl;
	msg << endl;

	msg << "e.g.   " << procname << " --file1 my.txt --file2 std.txt" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，全部匹配，仅一行输出" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --trim all" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，每行均去除头尾空格，仅一行输出" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --lineoffset -2" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，忽略my.txt的前2行(即my.txt的第3行与std.txt的第1行进行比较，每行严格匹配)，仅一行输出" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --trim left --lineoffset 3" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，忽略std.txt的前3行(即my.txt的第1行与std.txt的第4行进行比较)，每行去除头部空格，仅一行输出" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --ignore_blank" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，忽略文件中的所有空行，仅一行输出" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --ignore_blank --trim right" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，忽略文件中去除头尾空格后的所有空行，仅一行输出" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --max_diff 3" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，全部匹配，有3行不匹配后结束运行，仅一行输出" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --max_line 7" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，全部匹配，仅比较前7行，仅一行输出" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --CR_CRLF_not_queal" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，全部匹配，不忽略Windows/Linux的文件差异，仅一行输出" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --display normal" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，全部匹配，不匹配的行显示信息" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --display detailed" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，全部匹配，不匹配的行显示详细信息" << endl;
	msg << endl;
	cout << msg.str() << endl;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int main(int argc, char** argv)
{
	/* 因为引入了 lib_aat_tools.lib，此处先检查大小是否符合要求 */
	if (sizeof(args_analyse_tools) != 200) {
		cout << "class args_analyse_tools 不是200字节，如果使用 lib_aat_tools.lib 中的函数则可能出错" << endl;
		return -1;
	}

	/* 指定去除空格的方式 */
	const string TrimType[] = { "none",	"left",	"right",	"all", "" };
	/* 指定信息的显示方式 */
	const string DisplayType[] = { "none",	"normal",	"detailed", "" };
	args_analyse_tools args[] = {
		args_analyse_tools("--help",				ST_EXTARGS_TYPE::boolean, 0, false),		//显示帮助信息
		args_analyse_tools("--debug",				ST_EXTARGS_TYPE::boolean, 0, false),		//显示调试信息（不要求与demo完全相同）
		args_analyse_tools("--file1",				ST_EXTARGS_TYPE::str, 1, string("")),		//用于比较的文件名1
		args_analyse_tools("--file2",				ST_EXTARGS_TYPE::str, 1, string("")),		//用于比较的文件名2
		args_analyse_tools("--trim",				ST_EXTARGS_TYPE::str_with_set_error, 1, 0, TrimType),		//空格的比较方式
		args_analyse_tools("--lineoffset",		ST_EXTARGS_TYPE::int_with_default,   1, 0, -100, 100),		//指定左右文件的行差
		args_analyse_tools("--ignore_blank",		ST_EXTARGS_TYPE::boolean, 0, false),							//忽略空行
		args_analyse_tools("--CR_CRLF_not_equal",	ST_EXTARGS_TYPE::boolean, 0, false),							//
		args_analyse_tools("--max_diff",			ST_EXTARGS_TYPE::int_with_default,   1, 0, 0, 100),			//比较的最大错误数，超出则停止
		args_analyse_tools("--max_line",			ST_EXTARGS_TYPE::int_with_default,   1, 0, 0, 10000),			//比较的函数
		args_analyse_tools("--display",			ST_EXTARGS_TYPE::str_with_set_error, 1, 0, DisplayType),		//显示差异的方式
		args_analyse_tools()  //最后一个，用于结束
	};
	int cur_argc, ret = 0;

	//最后一个参数0，表示除选项参数外，没有其它参数
	if ((cur_argc = args_analyse_process(argc, argv, args, 0)) < 0) {
		//错误信息在函数中已打印
		args_analyse_print(args);
		usage(argv[0]);
		return -1;
	}

	/* 后续代码 */
	/*file1 file2必需*/
	if (args[FILE1].existed() == 0 || args[FILE2].existed() == 0)
	{
		usage(argv[0]);
		cout << endl << endl;
		cout << "必须指定参数[--file1和--file2]" << endl;
		return 0;
	}

	if (args[DEBUG].existed())
		args_analyse_print(args);

	compare(args);

	return 0;
}

/* 测试文件准备
	1.txt ：正常文件
	2.txt ：在1.txt的基础上，某些行前面无空格
	3.txt ：在1.txt的基础上，某些行后面无空格
	4.txt ：在1.txt的基础上，最后一行没有回车
	5.txt ：在1.txt的基础上，多一些空行
	6.txt ：在1.txt的基础上，多一些空行，其中某些行有空格和tab
	7.txt ：和1.txt完全相同
	8.txt ：是1.txt的Linux格式版

txt_compare --file1 1.txt --file2 2.txt --trim left
txt_compare --file1 1.txt --file2 2.txt --trim all
txt_compare --file1 2.txt --file2 3.txt --trim all
txt_compare --file1 1.txt --file2 4.txt --trim right --ignore_blank
txt_compare --file1 1.txt --file2 4.txt --trim all --ignore_blank
txt_compare --file1 2.txt --file2 4.txt --trim all --ignore_blank
txt_compare --file1 3.txt --file2 4.txt --trim right --ignore_blank
txt_compare --file1 3.txt --file2 4.txt --trim all --ignore_blank
txt_compare --file1 1.txt --file2 5.txt --trim right --ignore_blank
txt_compare --file1 1.txt --file2 6.txt --ignore_blank --trim right
txt_compare --file1 5.txt --file2 6.txt --ignore_blank --trim all
txt_compare --file1 1.txt --file2 7.txt
txt_compare --file1 1.txt --file2 8.txt

rem 不相同的例子
txt_compare --file1 1.txt --file2 2.txt
txt_compare --file1 1.txt --file2 2.txt --trim right
txt_compare --file1 1.txt --file2 5.txt
txt_compare --file1 1.txt --file2 5.txt --trim right
txt_compare --file1 1.txt --file2 6.txt
txt_compare --file1 1.txt --file2 6.txt --ignore_blank
txt_compare --file1 1.txt --file2 8.txt --CR_CRLF_not_equal

*/