/* 2050598 ͨ�� �����*/
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
  �������ƣ�
  ��    �ܣ�ѡ��һ����
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int ChooseBook(const vector<string>& files)
{
	int file_num = files.size();
	int chosen_no = 0;//��ѡ�е����
	int mx, my, mac, k1, k2;
	int top_no = 0;//Ŀǰ���ڲ˵���ϵ��ǵڼ����ļ�
	string bookname = BookName(files, chosen_no);

	/*��ʼ״̬���������ѡ��*/
	cct_gotoxy(LEFT_EXT_COL + 2, TOP_EXT_LINE + 1);
	ShowCatalogueBookName(bookname, 1);

	while (1)//��ͣ����
	{
		int event= cct_read_keyboard_and_mouse(mx, my, mac, k1, k2);
		if (event != CCT_KEYBOARD_EVENT)
			continue;
		if (k1 == 0xe0 && k2 == KB_ARROW_UP)
		{
			if (chosen_no != 0)
			{
				if (chosen_no > top_no)//������滹�У���û����ҳ��ʱ��
				{
					/*�Ȱ�ԭ�ȵĻָ���ɫ*/
					cct_gotoxy(LEFT_EXT_COL + 2, TOP_EXT_LINE + 1 + chosen_no - top_no);
					bookname = BookName(files, chosen_no);
					ShowCatalogueBookName(bookname, 0);
					/*��Ϳ�µ�*/
					cct_gotoxy(LEFT_EXT_COL + 2, TOP_EXT_LINE + 1 + chosen_no - 1 - top_no);
					bookname = BookName(files, chosen_no - 1);
					ShowCatalogueBookName(bookname, 1);
				}
				else//Ҫ��ҳ��
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
				if (chosen_no < top_no + MAX_LINE - 1)//���跭ҳ
				{
					/*�Ȱ�ԭ�ȵĻָ���ɫ*/
					cct_gotoxy(LEFT_EXT_COL + 2, TOP_EXT_LINE + 1 + chosen_no - top_no);
					bookname = BookName(files, chosen_no);
					ShowCatalogueBookName(bookname, 0);
					/*��Ϳ�µ�*/
					cct_gotoxy(LEFT_EXT_COL + 2, TOP_EXT_LINE + 1 + chosen_no + 1 - top_no);
					bookname = BookName(files, chosen_no + 1);
					ShowCatalogueBookName(bookname, 1);
				}
				else//�÷�ҳ
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
		else if (k1 == 'Q' || k1 == 'q')//�˳�
		{
			return -1;
		}
	}
	cct_gotoxy(0, TOP_EXT_LINE + 1 + MAX_LINE + 6);
	cout << "ѡ����" << BookName(files, chosen_no).c_str() << ".txt�ļ�" << endl;

	return chosen_no;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��������з��Ż��У���λ���ж����ݿ����һ�еĵ�һ���ַ���λ��
  �������������������¼��λ�ã�����ķ�ʽ
  �� �� ֵ��
  ˵    ����
***************************************************************************/
streampos GetFirstCharPos(ifstream& infile,const streampos pos, const int choice)
{
	infile.clear();
	infile.seekg(pos, ios::beg);
	streampos bugp1 = infile.tellg();
	//infile.seekg(-2, ios::cur);//�ƶ��ļ�ָ�뵽Ҫ��λ�õ�ǰ����λ��

	bool endlmark = 0;//ǰ���ǻ��з��ı�־

	long long chinese = 0;//�������ַ��ĸ����������� ��ֵ+2��
	bool last_chinese = 0;//�ж���һ���ַ��ǲ�������

	if (choice == MEET_ENDL)//������������з��ͻ���
	{
		while (1)//Ĭ�ϵ����з����У�����Ҫ�ҵ����з����㵱ǰ��һ��
		{
			char ch = infile.peek();
			streampos bugp2 = infile.tellg();
			if (ch == '\n' || infile.tellg() == 0)
			{
				if (ch == '\n')
				{
					infile.seekg(1, ios::cur);//����ҵ����з���ָ����Ҫ����һ��λ��
					endlmark = 1;
				}
				break;
			}
			infile.seekg(-1, ios::cur);
		}
	}
	else
	{
		while (1)//Ĭ�ϵ����з�+�ո��У�����Ҫ�ҵ����з����㵱ǰ��һ��
		{
			char ch = infile.peek();
			if (ch == EOF)
				infile.seekg(-1, ios::end);
			if (infile.tellg() == 0)//��������ļ���ͷ
				break;
			if (ch == '\n')
			{
				infile.seekg(1, ios::cur);//����ҵ����з���ָ����Ҫ����һ��λ��
				unsigned char next = infile.peek();
				if (next == ' ' || next == '\t' || next == 0xA1)//������з������ǿո񣬻���
				{
					endlmark = 1;
					break;
				}
				else
					infile.seekg(-1, ios::cur);//��ָ���ƻ�ȥ
			}
			infile.seekg(-1, ios::cur);
		}
	}
	streampos screen_start = infile.tellg();
	int screenpos = 0;//��¼����ʾ���е�λ�ã���1��ʼ����
	if (choice == MEET_ENDL)//�������оͻ���
	{
		//if (endlmark == 1 && infile.peek() != ' ' && infile.peek() != '\t' && (unsigned char)infile.peek() != 0xA1)//������з�����û�пո���ӿո�
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
				if (chinese % 2 == 1)//��ǰ�ַ��Ǻ��ֵ�ǰ�����
				{
					infile.seekg(-1, ios::cur);//�ļ�ָ����ǰ��һ��
				}
				screenpos = 0;
				chinese = 0;
				screen_start = infile.tellg() + (long long)1;
			}
			infile.seekg(1, ios::cur);
		}
	}
	else//���з������пո�Ż���
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

			if (c != '\r' && c != '\n')//���з���ռλ��
				screenpos++;

			if (screenpos >= MAX_COL)
			{
				if (chinese % 2 == 1)//��ǰ�ַ��Ǻ��ֵ�ǰ�����
				{
					infile.seekg(-1, ios::cur);//�ļ�ָ����ǰ��һ��
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
  �������ƣ�
  ��    �ܣ����ݵ�һ�е�һ���ַ���λ�����,�������оͻ��У��հ��Լ����
  �������������������һ�е�һ���ַ���λ�ã���¼�ڶ��С����һ�����ַ���ʼλ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int OutputScreenEndl(ifstream& infile, const streampos pos, streampos& secondline,streampos& lastline)
{
	int line = 1;//��¼Ŀǰ��������
	bool endlmark = 0;//���뻻�з���һ����Ҫ���ĸ�ı��
	int end = 0;

	ClearScreen();//������

	if (pos == 0)
	{
		infile.seekg(pos, ios::beg);
		endlmark = 0;
	}
	else
	{
		infile.seekg(pos, ios::beg);
		infile.seekg(-1, ios::cur);//��ǰ��һ�������Ƿ��ǻ��з����߿հ�
		char ch1 = infile.peek();
		infile.seekg(1, ios::cur);//�ƻ���
		unsigned char ch2 = infile.peek();
		//if (ch1 == '\n' && ch2 != ' ' && ch2 != '\t' && ch2 != 0xA1)
		if (ch1 == '\n')
			endlmark = 1;//ǰ������ո�
		else
			endlmark = 0;
	}
	

	while (line <= MAX_LINE && infile.peek() != EOF)//ѭ������һ�еĲ���
	{
		if (line == 2)
			secondline = infile.tellg();
		if (line == MAX_LINE)
			lastline = infile.tellg();

		/*����һ��*/
		int chinese = 0;//��¼һ���е������ַ�����
		char ch[MAX_COL + 1] = { 0 };

		//�ж�һ�иö�������ַ�

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
			if (ch[k] == '\n')//��������Ҫ����
			{
				break;
			}
		}

		int len = strlen(ch);

		if (len == 1 && ch[0] == '\n' || len == 2 && ch[0] == '\r' && ch[1] == '\n')//���������ݿ��У������
		{
			endlmark = 1;
			line++;
			continue;
		}

		if (chinese % 2 == 1)//���������ǰһ��
		{
			ch[len - 1] = '\0';
			infile.seekg(-1, ios::cur);
		}


		if (len >= 1 && ch[len - 1] == '\n')
		{
			endlmark = 1;//��һ�п�ͷ�ո�
			ch[len - 1] = '\0';
		}
		if (len >= 2 && ch[len - 2] == '\r')
			ch[len - 2] = '\0';

		cout << ch << endl;//���һ��

		line++;
	}
	return end;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ݵ�һ�е�һ���ַ���λ�����,�������з�+�հײŻ��У��������з�������
  �������������������һ�е�һ���ַ���λ�ã���¼�ڶ��С����һ�����ַ���ʼλ��
  �� �� ֵ���Ѿ������ļ�ĩβ��1��δ������0
  ˵    ����
***************************************************************************/
int OutputScreenEndlAndBlank(ifstream& infile, const streampos pos, streampos& secondline, streampos& lastline)
{
	int line = 1;//��¼Ŀǰ��������
	int end = 0;

	ClearScreen();//������

	
	infile.seekg(pos, ios::beg);
	

	while (line <= MAX_LINE && infile.peek() != EOF)//ѭ������һ�в�����Ĳ���
	{
		if (line == 2)
			secondline = infile.tellg();
		if (line == MAX_LINE)
			lastline = infile.tellg();

		/*����һ��*/
		int chinese = 0;//��¼һ���е������ַ�����
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

			if (ch[k] == '\n' && (infile.peek() == ' ' || infile.peek() == '\t' || (unsigned char)infile.peek() == 0xA1))//��������+�ո�Ҫ����
			{
				break;
			}

			if (ch[k] != '\n' && ch[k] != '\r')
				k++;
		}//���ǻ��з����Ȼ�пհ�

		int len = strlen(ch);

		if (chinese % 2 == 1)//���������ǰһ��
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

		cout << ch << endl;//���һ��

		line++;
	}

	return end;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ����������ļ����ָ��
  �� �� ֵ�������ļ�ָ��
  ˵    ����
***************************************************************************/
streampos InputPos(const streampos endp)
{
	cct_setcursor(CURSOR_VISIBLE_NORMAL);
	double percent = 0;
	cct_gotoxy(0, TOP_EXT_LINE + HEIGHT + 4);
	cout << "����������תλ�õİٷֱ�(0-100�������λС��) : ";
	cin >> percent;
	if (cin.fail() || percent < 0 || percent>100)
		percent = -1;//����ֵ��-1
	streampos start;
	start = (long long)((endp - (long long)1) * (percent / 100));
	cct_gotoxy(0, TOP_EXT_LINE + HEIGHT + 4);
	cout << setw(80) << setfill(' ') << " ";
	cct_setcursor(CURSOR_INVISIBLE);
	return start;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ȡһ����
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int OpenBook(const vector<string>& files)
{
	cct_setcursor(CURSOR_INVISIBLE);
	bool quit = 0;//����˳�
	int chosen_no = ChooseBook(files);//ѡ��һ����
	/*�����ʾ*/
	cct_gotoxy(0, TOP_EXT_LINE + HEIGHT + 1);
	cout << setw(50) << setfill(' ') << " ";

	if (chosen_no < 0)
	{
		return -1;
	}
	int output_choice = MEET_ENDL_AND_BLANK;
	ifstream infile(files[chosen_no], ios::in | ios::binary);

	/*��ȡ�ϴζ�����λ���ļ�*/
	
	streampos pos = GetPosSaving(BookName(files, chosen_no));//��ʼ�Ķ���λ��
	
	infile.seekg(0, ios::end);
	streampos endp = infile.tellg();

	/*�����ʼλ��̫����߷Ƿ�*/
	if (pos >= endp || pos < 0)
		pos = 0;

	if (endp > 20 * 1024 * 1024)
	{
		cct_gotoxy(0, TOP_EXT_LINE + 1 + MAX_LINE + 5);
		cout << "�ļ�����20MB���޷���" << endl;
		infile.close();
		return -1;
	}

	

	streampos screen_first_pos, second_line_pos, last_line_pos;
	/*��ӡ��ʼλ�ÿ�ʼ���ı�*/
	screen_first_pos = GetFirstCharPos(infile, pos, output_choice);
	if (output_choice == MEET_ENDL)
		OutputScreenEndl(infile, screen_first_pos, second_line_pos, last_line_pos);
	else if (output_choice == MEET_ENDL_AND_BLANK)
		OutputScreenEndlAndBlank(infile, screen_first_pos, second_line_pos, last_line_pos);

	/*��ӡ��ʾ*/
	cct_gotoxy(0, TOP_EXT_LINE + HEIGHT + 3);
	cout << "��ʾ��ʽ1:�����з��ͻ���(����ӿո�)/��ʾ��ʽ2:�����з�+�ո�Ż���" << endl;
	cct_gotoxy(0, TOP_EXT_LINE + HEIGHT + 2);
	cout << "��/�¼�ͷ�ƶ�һ��,PgUp/PgDn��ҳ,S/s��ת����,F/f�ı���ʾ��ʽ,N/n����,Q/q�˳�";
	cct_gotoxy(0, TOP_EXT_LINE + HEIGHT + 1);
	cout << "��ǰ���ȣ�" << setprecision(2) << (double)screen_first_pos / endp * 100 << "%";
	if (output_choice == MEET_ENDL_AND_BLANK)
		cout << "  ��ʾ��ʽ:2" << endl;
	else
		cout << "  ��ʾ��ʽ:1" << endl;

	int end = 0;
	/*��ȡ����*/
	while (1)//�ظ�����
	{ 
		
		int mx, my, mac, k1, k2;
		int event = cct_read_keyboard_and_mouse(mx, my, mac, k1, k2);
		if (event != CCT_KEYBOARD_EVENT)
			continue;

		if (k1 == 0xe0 && k2 == KB_ARROW_DOWN)//�ڶ��б������
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
		else if (k1 == 0xe0 && k2 == KB_PGDN)//���һ�б������
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
		else if (k1 == 0xe0 && k2 == KB_PGUP)//���б�����һ��
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
		else if (k1 == 'f' || k1 == 'F')//F�л���ʾ��ʽ
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
		cout << "��ǰ���ȣ�" << setiosflags(ios::fixed) << setprecision(2) << (double)screen_first_pos / endp * 100 << "%";
		if (output_choice == MEET_ENDL_AND_BLANK)
			cout << "  ��ʾ��ʽ:2" << endl;
		else
			cout << "  ��ʾ��ʽ:1" << endl;
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