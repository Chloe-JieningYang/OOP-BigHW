/*2050598 ����� ͨ��*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include "../include/class_aat.h"
#include "txt_compare.h"
using namespace std;

/* ���������Ҫ������ */

/*���ַ������п�ͷ�Ŀո��\tɾ��*/
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

/*���ַ������н�β�Ŀո��\tɾ��*/
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
	/*posָ���һ�����ǿո���е�λ��*/
	/*pos2ָ����β���з�λ��*/
	if (pos == (int)strlen(ch) - 1 || pos + 1 == pos2)
		return;
	
	strcpy(ch + pos + 1, ch + pos2);
}

/*����CR_CRLF��ͬ,ȫ����ɵ���\n*/
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
	cout << "�Ķ���ʾ��" << endl;
	cout << "        2���ļ�������<EOF>���" << endl;
	cout << "        3��������ͬ��λ�õĲ����ַ�����ɫ���" << endl;
	cout << "        4��ÿ���е�CR/LF/VT/BS/BEL��X���(���㿴�������ַ�)" << endl;
	cout << "        5��ÿ��β�Ķ�����ַ�����ɫ�����CR/LF/VT/BS/BEL����ɫX���(���㿴�������ַ�)" << endl;
	cout << "        6��ÿ�������<CR>���(���㿴�������ַ�)" << endl;
	cout << "        7��������Ϊ�������⣬����λ�ÿ��ܱ��ں��������ϣ����������ֶ���ɫ���" << endl;
	if (args[DISPLAY].get_string() == "normal")
		cout << "        8����--display detailed���Եõ�����ϸ����Ϣ" << endl;
}

/*���ڱȽϵĺ���*/
static int compare(const args_analyse_tools args[])
{
	ifstream infile1(args[FILE1].get_string(), ios::in | ios::binary);
	if (!infile1.is_open())
	{
		cout << "��1���ļ�[" << args[FILE1].get_string() << "]�޷���." << endl;
		return -1;
	}
	ifstream infile2(args[FILE2].get_string(), ios::in | ios::binary);
	if (!infile2.is_open())
	{
		cout << "��2���ļ�[" << args[FILE2].get_string() << "]�޷���." << endl;
		return -1;
	}

	/*�����еļ���*/
	int line_f1 = 0;
	int line_f2 = 0;

	/*�����ж��ļ��Ƿ��Ѿ�����*/
	bool f1_eof = 0;
	bool f2_eof = 0;

	/*�������Ƚ������в��������*/
	int count_dif_line = 0;

	/*�������Ƚ��˶�����*/
	int count_compare_line = 0;

	/*�ǲ�����lineoffset���Ѿ�����*/
	bool one_eof = 0;
	/*��Ҫ�Ӳ�ͬ�п�ʼ�Ƚ�*/
	if (args[LINEOFFSET].existed())
	{
		if (args[LINEOFFSET].get_int() < 0)//������ʾ����file1��ǰn��
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
					cout << "�ļ�[" << args[FILE1].get_string() << "]�ĵ�(" << k << ")�в�����Ҫ�󣬳�����󳤶�[" << MAX_SIZE << "]." << endl;
					return -1;
				}
			}
		}
		else if (args[LINEOFFSET].get_int() > 0)//������ʾ����file2��ǰn��
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
					cout << "�ļ�[" << args[FILE2].get_string() << "]�ĵ�(" << k << ")�в�����Ҫ�󣬳�����󳤶�[" << MAX_SIZE << "]." << endl;
					return -1;
				}
			}
		}
	}

	/*�������һ���ļ��Ѿ�������*/
	if (f1_eof || f2_eof)
	{
		if (f1_eof && args[LINEOFFSET].existed() && args[LINEOFFSET].get_int() < 0)
			line_f1 = -args[LINEOFFSET].get_int();
		else if (f2_eof && args[LINEOFFSET].existed() && args[LINEOFFSET].get_int() > 0)
			line_f2 = args[LINEOFFSET].get_int();
		count_dif_line++;
		one_eof = 1;
	}

	/*��ʼ����Ҫ�Ƚϵ�����*/
	
	while (infile1.eof() != 1 || infile2.eof() != 1)
	{
		int count1 = 0;//����һ���ļ�һ�б�������ַ���
		int count2 = 0;//���ڶ����ļ�һ�б�������ַ���
		char f1[1026] = { 0 };
		char f2[1026] = { 0 };

		
		/*file1��ȡһ��*/
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
				cout << "�ļ�[" << args[FILE1].get_string() << "]�ĵ�(" << line_f1 << ")�в�����Ҫ�󣬳�����󳤶�[" << MAX_SIZE << "]." << endl;
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

		/*file2��ȡһ��*/
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
				cout << "�ļ�[" << args[FILE2].get_string() << "]�ĵ�(" << line_f2 << ")�в�����Ҫ�󣬳�����󳤶�[" << MAX_SIZE << "]." << endl;
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
		

		/*�Ƚ�һ��*/
		/*�ȴ���trim���ѿո��/tɾ��*/
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
		}/*������trim*/

		/*����ignore_blank*/
		if (args[IGNORE_BLANK].existed())
		{
			/*��һ���ļ��������У��ͼ�������һ��ֱ����Ϊ��Ϊֹ*/
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
							cout << "�ļ�[" << args[FILE1].get_string() << "]�ĵ�(" << line_f1 << ")�в�����Ҫ�󣬳�����󳤶�[" << MAX_SIZE << "]." << endl;
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

					if (args[TRIM].existed())//����trim
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

			/*�ڶ����ļ��������У��ͼ�������һ��ֱ����Ϊ��Ϊֹ*/
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
							cout << "�ļ�[" << args[FILE2].get_string() << "]�ĵ�(" << line_f2 << ")�в�����Ҫ�󣬳�����󳤶�[" << MAX_SIZE << "]." << endl;
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

					if (args[TRIM].existed())//����trim
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

			/*һ���ļ���������һ���ļ�ĩβֻ�л���*/
			if (f1_eof && !f2_eof && infile2.peek() == EOF && f2[(int)strlen(f2) - 1] == '\n')
			{
				int len2 = strlen(f2);
				f2_eof = 1;
				if (len2 - 2 >= 0 && f2[len2 - 2] == '\r')//�����\r\n
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

		count_compare_line++;//�Ƚϵ�����+1

		/*���LF��CRLF������һ�����Ͷ�����ɵ���\n*/
		if (!args[CR_CRLF_NOT_EQUAL].existed())
		{
			CR_CRLF(f1);
			CR_CRLF(f2);
		}
		

		int pos = 0;
		int first_dif_pos = 0;//��һ�����ڲ���ĵط�
		bool dif = 0;//�����Ƿ��в���
		int linelen1 = strlen(f1);
		int linelen2 = strlen(f2);
		int minlen = (linelen1 < linelen2) ? linelen1 : linelen2;

		/*�ȼ�������ַ�����С�Ƿ���ͬ*/
		bool len_is_same = (linelen1 == linelen2) ? 1 : 0;

		

		while (pos < minlen && f1[pos] != '\n' && f2[pos] != '\n')//�Դ�������С�����ڵ�ÿ���ַ����в���
		{
			if (f1[pos] != f2[pos])//����в�ͬ
			{
				if ((!args[DISPLAY].existed() || args[DISPLAY].get_string() == "none") && !args[DEBUG].existed())
				{
					cout << "�ļ���ͬ." << endl;
					return 0;
				}
				else
				{
					if (dif == 0)//�����һ��֮ǰû�в���
						first_dif_pos = pos;
					dif = 1;
					pos++;
				}
			}
			else
				pos++;
		}

		if (one_eof || dif == 1 || len_is_same == 0 || (f1_eof == 0 && f2_eof == 1) || (f1_eof == 1 && f2_eof == 0))//������ڲ���򳤶Ȳ�ͬ��normal/detailed��Ҫ��ӡf1 f2����һ��
		{
			count_dif_line++;//�в��������+1

			int maxlen = (linelen1 > linelen2) ? linelen1 : linelen2;

			if (args[DISPLAY].existed() && args[DISPLAY].get_string() == "normal"||args[DEBUG].existed())//normal�Ĵ�ӡ
			{
				if (count_dif_line == 1)
				{
					cout << "�ȽϽ�������" << endl;
					cout << "====================" << endl;
				}

				if (dif)//����ǱȽ��в���
					cout << "��[" << line_f1 << " / " << line_f2 << "]�� - ��[" << first_dif_pos << "]���ַ���ʼ�в��죺" << endl;
				else if(len_is_same == 0 && (maxlen-minlen != 1 || maxlen - minlen == 1 && f1_eof == f2_eof)|| one_eof)//�ǳ��Ȳ�ͬ���ж����ַ�,�Ҳ�ֻ�Ƕ�һ��\n
				{
					cout << "��[" << line_f1 << " / " << line_f2 << "]�� - �ļ�";
					if (linelen1 > minlen||f2_eof && one_eof)
						cout << "1";
					else
						cout << "2";
					cout << "��β���ж����ַ���" << endl;
				}
				else if (f1_eof == 1 && f2_eof == 0)
				{
					cout << "��[" << line_f1 << " / " << line_f2 << "]�� - �ļ�1�ѽ���/�ļ�2�������ݣ�" << endl;
				}
				else if (f1_eof == 0 && f2_eof == 1)
				{
					cout << "��[" << line_f1 << " / " << line_f2 << "]�� - �ļ�1��������/�ļ�2�ѽ�����" << endl;
				}


				cout << "�ļ�1 : ";
				int k = 0;
				while (k < linelen1)
				{
					if (f1[k] == '\n' && k == linelen1 - 1)//����β��\n
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
						if (f1[k] != f2[k] || k > linelen2 - 1)//��һ���Ļ��������ַ��ò�ͬ��ɫ���
							cct_setcolor(COLOR_HYELLOW, COLOR_HRED);
						cout << "X";
						cct_setcolor();
					}
					else
					{
						if (f1[k] != f2[k] || k > linelen2 - 1)//��һ���Ļ��������ַ��ò�ͬ��ɫ���
							cct_setcolor(COLOR_HYELLOW, COLOR_HRED);
						cout << f1[k];
						cct_setcolor();
					}
					k++;
				}
				if (f1_eof)
					cout << "<EOF>";
				cout << endl;

				cout << "�ļ�2 : ";
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
						if (f1[k] != f2[k] || k > linelen1 - 1)//��һ���Ļ��������ַ��ò�ͬ��ɫ���
							cct_setcolor(COLOR_HYELLOW, COLOR_HRED);
						cout << "X";
						cct_setcolor();
					}
					else
					{
						if (f1[k] != f2[k] || k > linelen1 - 1)//��һ���Ļ��������ַ��ò�ͬ��ɫ���
							cct_setcolor(COLOR_HYELLOW, COLOR_HRED);
						cout << f2[k];
						cct_setcolor();
					}
					k++;
				}
				if (f2_eof)
					cout << "<EOF>";
				cout << endl << endl;
			}//normal ������
			else if (args[DISPLAY].existed() && args[DISPLAY].get_string() == "detailed")//detailed ���
			{
				if (count_dif_line == 1)
				{
					cout << "�ȽϽ�������" << endl;
					cout << setfill('=') << setw(100) << "=" << endl;
				}

				if (dif)//����ǱȽ��в���
					cout << "��[" << line_f1 << " / " << line_f2 << "]�� - ��[" << first_dif_pos << "]���ַ���ʼ�в��죺" << endl;
				else if (len_is_same == 0 && (maxlen - minlen != 1 || maxlen - minlen == 1 && f1_eof == f2_eof) || one_eof)//�ǳ��Ȳ�ͬ���ж����ַ�,�Ҳ�ֻ�Ƕ�һ��\n
				{
					cout << "��[" << line_f1 << " / " << line_f2 << "]�� - �ļ�";
					if (linelen1 > minlen)
						cout << "1";
					else
						cout << "2";
					cout << "��β���ж����ַ���" << endl;
				}
				else if (f1_eof == 1 && f2_eof == 0)
				{
					cout << "��[" << line_f1 << " / " << line_f2 << "]�� - �ļ�1�ѽ���/�ļ�2�������ݣ�" << endl;
				}
				else if (f1_eof == 0 && f2_eof == 1)
				{
					cout << "��[" << line_f1 << " / " << line_f2 << "]�� - �ļ�1��������/�ļ�2�ѽ�����" << endl;
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

				cout << "�ļ�1 : ";
				int k = 0;
				while (k < linelen1)
				{
					if ( f1[k] == '\n' && k == linelen1 - 1)//����β��\n
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
						if (f1[k] != f2[k] || k > linelen2 - 1)//��һ���Ļ��������ַ��ò�ͬ��ɫ���
							cct_setcolor(COLOR_HYELLOW, COLOR_HRED);
						cout << "X";
						cct_setcolor();
					}
					else
					{
						if (f1[k] != f2[k] || k > linelen2 - 1)//��һ���Ļ��������ַ��ò�ͬ��ɫ���
							cct_setcolor(COLOR_HYELLOW, COLOR_HRED);
						cout << f1[k];
						cct_setcolor();
					}
					k++;
				}
				if (f1_eof)
					cout << "<EOF>";
				cout << endl;

				cout << "�ļ�2 : ";
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
						if (f1[k] != f2[k] || k > linelen1 - 1)//��һ���Ļ��������ַ��ò�ͬ��ɫ���
							cct_setcolor(COLOR_HYELLOW, COLOR_HRED);
						cout << "X";
						cct_setcolor();
					}
					else
					{
						if (f1[k] != f2[k] || k > linelen1 - 1)//��һ���Ļ��������ַ��ò�ͬ��ɫ���
							cct_setcolor(COLOR_HYELLOW, COLOR_HRED);
						cout << f2[k];
						cct_setcolor();
					}
					k++;
				}
				if (f2_eof)
					cout << "<EOF>";
				cout << endl;

				cout << "�ļ�1(HEX) :" << endl;
				
				int hex_line1 = 0;
				bool end1 = 0;
				while (1)
				{
					cout << setfill('0') << setw(7) << hex << hex_line1 << "0 : ";
					int j;
					for (j = 0; j <= 15; j++)
					{
						if (f1[hex_line1 * 16 + j] == '\0')//�ַ�������
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
				}//��1���ļ���HEX�������

				cout << "�ļ�2(HEX) :" << endl;

				int hex_line2 = 0;
				bool end2 = 0;
				while (1)
				{
					cout << setfill('0') << setw(7) << hex << hex_line2 << "0 : ";
					int j;
					for (j = 0; j <= 15; j++)
					{
						if (f2[hex_line2 * 16 + j] == '\0')//�ַ�������
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
				}//��2���ļ���HEX�������
				cout << endl;
			}//detailed�������
		}//if (dif == 1 || len_is_same == 0) ���������Ϣ���
		if (args[MAX_DIFF].existed() && count_dif_line >= args[MAX_DIFF].get_int())
			break;
		if (args[MAX_LINE].existed() && count_compare_line >= args[MAX_LINE].get_int())
			break;
		if (f1_eof || f2_eof || infile1.eof() || infile2.eof())
			break;
	}//����ļ��Ķ�ȡ

	cout << setw(100) << setfill('=') << "=" << endl;
	if (count_dif_line != 0)
	{
		cout << "��ָ����������¹�" << count_dif_line << "���в���";
		instruction(args);
		if (args[MAX_DIFF].existed() && count_dif_line == args[MAX_DIFF].get_int())
			cout << "[�ѵ��趨��������ֵ]";
		cout << "." << endl;
	}
	else
		cout << "��ָ�������������ȫһ��." << endl;
	cout << setw(100) << setfill('=') << "=" << endl;

	infile1.close();
	infile2.close();
	return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
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

	msg << setw(wkey) << ' ' << "��ѡ�" << endl;
	msg << setw(wopt) << ' ' << "--file1 name1              : ָ��Ҫ�Ƚϵĵ�1���ļ�������ѡ��" << endl;
	msg << setw(wopt) << ' ' << "--file2 name2              : ָ��Ҫ�Ƚϵĵ�2���ļ�������ѡ��" << endl;
	msg << endl;

	msg << setw(wkey) << ' ' << "��ѡ�" << endl;
	msg << setw(wopt) << ' ' << "--trim none/left/right/all     : ָ��ÿ�еĿո�/tab�ĺ��Է�ʽ���޴�����Ĭ��Ϊnone��" << endl;
	msg << setw(wopt) << ' ' << "\tnone   : ÿ�о��ϸ�ƥ��" << endl;
	msg << setw(wopt) << ' ' << "\tleft   : ÿ�о�����ͷ���ո�" << endl;
	msg << setw(wopt) << ' ' << "\tright  : ÿ�о�����β���ո�" << endl;
	msg << setw(wopt) << ' ' << "\tall    : ÿ�о�����ͷβ�ո�" << endl;
	msg << setw(wopt) << ' ' << "--lineoffset n                 : ָ���ļ���λ���������޴�����nΪ0��" << endl;
	msg << setw(wopt) << ' ' << "\tȡֵ[-100..100]��������ʾ����file1��ǰn�У�������ʾ����file2��ǰn��" << endl;
	msg << setw(wopt) << ' ' << "--ignore_blank                 : �����ļ���--trim������п���(�޴����򲻺���)" << endl;
	msg << setw(wopt) << ' ' << "--CR_CRLF_not_equal            : ������Windows/Linux�ļ��Ļ��з�����(�޴�������Բ���)" << endl;
	msg << setw(wopt) << ' ' << "--max_diff m                   : ָ���ļ���λ���������޴�����mΪ0��" << endl;
	msg << setw(wopt) << ' ' << "\tȡֵ[0..100]����ʾ��m����ͬ����������У����ٽ��к����ıȽ�" << endl;
	msg << setw(wopt) << ' ' << "--max_line x                   : ָ���ļ��Ƚϵ�����������޴�����xΪ0��" << endl;
	msg << setw(wopt) << ' ' << "\tȡֵ[0..10000]����ʾ�Ƚ�x�к���������У����ٽ��к����ıȽ�" << endl;
	msg << setw(wopt) << ' ' << "--display none/normal/detailed : ָ����ʾ����Ϣ���޴�����Ĭ��Ϊnone��" << endl;
	msg << setw(wopt) << ' ' << "\tnone   : ��һ��ƥ��������ʾ" << endl;
	msg << setw(wopt) << ' ' << "\tnormal : ÿ�������и���������Ϣ" << endl;
	msg << setw(wopt) << ' ' << "\tright  : ÿ�������и�������ϸ�Ĳ�����Ϣ" << endl;
	msg << endl;

	msg << "e.g.   " << procname << " --file1 my.txt --file2 std.txt" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt��ȫ��ƥ�䣬��һ�����" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --trim all" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt��ÿ�о�ȥ��ͷβ�ո񣬽�һ�����" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --lineoffset -2" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt������my.txt��ǰ2��(��my.txt�ĵ�3����std.txt�ĵ�1�н��бȽϣ�ÿ���ϸ�ƥ��)����һ�����" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --trim left --lineoffset 3" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt������std.txt��ǰ3��(��my.txt�ĵ�1����std.txt�ĵ�4�н��бȽ�)��ÿ��ȥ��ͷ���ո񣬽�һ�����" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --ignore_blank" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt�������ļ��е����п��У���һ�����" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --ignore_blank --trim right" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt�������ļ���ȥ��ͷβ�ո������п��У���һ�����" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --max_diff 3" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt��ȫ��ƥ�䣬��3�в�ƥ���������У���һ�����" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --max_line 7" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt��ȫ��ƥ�䣬���Ƚ�ǰ7�У���һ�����" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --CR_CRLF_not_queal" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt��ȫ��ƥ�䣬������Windows/Linux���ļ����죬��һ�����" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --display normal" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt��ȫ��ƥ�䣬��ƥ�������ʾ��Ϣ" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --display detailed" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt��ȫ��ƥ�䣬��ƥ�������ʾ��ϸ��Ϣ" << endl;
	msg << endl;
	cout << msg.str() << endl;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int main(int argc, char** argv)
{
	/* ��Ϊ������ lib_aat_tools.lib���˴��ȼ���С�Ƿ����Ҫ�� */
	if (sizeof(args_analyse_tools) != 200) {
		cout << "class args_analyse_tools ����200�ֽڣ����ʹ�� lib_aat_tools.lib �еĺ�������ܳ���" << endl;
		return -1;
	}

	/* ָ��ȥ���ո�ķ�ʽ */
	const string TrimType[] = { "none",	"left",	"right",	"all", "" };
	/* ָ����Ϣ����ʾ��ʽ */
	const string DisplayType[] = { "none",	"normal",	"detailed", "" };
	args_analyse_tools args[] = {
		args_analyse_tools("--help",				ST_EXTARGS_TYPE::boolean, 0, false),		//��ʾ������Ϣ
		args_analyse_tools("--debug",				ST_EXTARGS_TYPE::boolean, 0, false),		//��ʾ������Ϣ����Ҫ����demo��ȫ��ͬ��
		args_analyse_tools("--file1",				ST_EXTARGS_TYPE::str, 1, string("")),		//���ڱȽϵ��ļ���1
		args_analyse_tools("--file2",				ST_EXTARGS_TYPE::str, 1, string("")),		//���ڱȽϵ��ļ���2
		args_analyse_tools("--trim",				ST_EXTARGS_TYPE::str_with_set_error, 1, 0, TrimType),		//�ո�ıȽϷ�ʽ
		args_analyse_tools("--lineoffset",		ST_EXTARGS_TYPE::int_with_default,   1, 0, -100, 100),		//ָ�������ļ����в�
		args_analyse_tools("--ignore_blank",		ST_EXTARGS_TYPE::boolean, 0, false),							//���Կ���
		args_analyse_tools("--CR_CRLF_not_equal",	ST_EXTARGS_TYPE::boolean, 0, false),							//
		args_analyse_tools("--max_diff",			ST_EXTARGS_TYPE::int_with_default,   1, 0, 0, 100),			//�Ƚϵ�����������������ֹͣ
		args_analyse_tools("--max_line",			ST_EXTARGS_TYPE::int_with_default,   1, 0, 0, 10000),			//�Ƚϵĺ���
		args_analyse_tools("--display",			ST_EXTARGS_TYPE::str_with_set_error, 1, 0, DisplayType),		//��ʾ����ķ�ʽ
		args_analyse_tools()  //���һ�������ڽ���
	};
	int cur_argc, ret = 0;

	//���һ������0����ʾ��ѡ������⣬û����������
	if ((cur_argc = args_analyse_process(argc, argv, args, 0)) < 0) {
		//������Ϣ�ں������Ѵ�ӡ
		args_analyse_print(args);
		usage(argv[0]);
		return -1;
	}

	/* �������� */
	/*file1 file2����*/
	if (args[FILE1].existed() == 0 || args[FILE2].existed() == 0)
	{
		usage(argv[0]);
		cout << endl << endl;
		cout << "����ָ������[--file1��--file2]" << endl;
		return 0;
	}

	if (args[DEBUG].existed())
		args_analyse_print(args);

	compare(args);

	return 0;
}

/* �����ļ�׼��
	1.txt �������ļ�
	2.txt ����1.txt�Ļ����ϣ�ĳЩ��ǰ���޿ո�
	3.txt ����1.txt�Ļ����ϣ�ĳЩ�к����޿ո�
	4.txt ����1.txt�Ļ����ϣ����һ��û�лس�
	5.txt ����1.txt�Ļ����ϣ���һЩ����
	6.txt ����1.txt�Ļ����ϣ���һЩ���У�����ĳЩ���пո��tab
	7.txt ����1.txt��ȫ��ͬ
	8.txt ����1.txt��Linux��ʽ��

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

rem ����ͬ������
txt_compare --file1 1.txt --file2 2.txt
txt_compare --file1 1.txt --file2 2.txt --trim right
txt_compare --file1 1.txt --file2 5.txt
txt_compare --file1 1.txt --file2 5.txt --trim right
txt_compare --file1 1.txt --file2 6.txt
txt_compare --file1 1.txt --file2 6.txt --ignore_blank
txt_compare --file1 1.txt --file2 8.txt --CR_CRLF_not_equal

*/