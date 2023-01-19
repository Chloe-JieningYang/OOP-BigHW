/* 2050598 通信 杨洁宁*/
#define _CRT_SECURE_NO_WARNINGS
#include <io.h>
#include <vector>
#include <conio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "../include/common-readtool-menu.h"
#include "../include/cmd_console_tools.h"
using namespace std;

/***************************************************************************
  函数名称：
  功    能：判断是不是空行
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
bool Is_Blank(char s[])
{
	int len = strlen(s);
	if (len == 0)
		return true;
	for (int k = 0; k <= len - 1; k++)
	{
		if (s[k] != ' ' && s[k] != '\t' && s[k] != 0xA1)
			return false;
	}
	return true;
}

/***************************************************************************
  函数名称：
  功    能：读取position.saving里记录的位置
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
streampos GetPosSaving(string bookname)
{
	char s[1000] = { 0 };
	bool find = 0;
	streampos pos;
	ifstream posfile(".\\book\\position.saving", ios::in | ios::binary);

	while (posfile.peek() != EOF)
	{
		for (int k = 0; k <= 999; k++)
			s[k] = '\0';

		posfile.getline(s, 999);
		string name = s;
		if (name == "[" + bookname + ".txt]")
		{
			find = 1;
			break;
		}
	}

	
	if (find == 1)
	{
		char num_c[1000] = { 0 };
		while (1)
		{
			posfile.getline(num_c, 999);
			if (Is_Blank(num_c) == false)
				break;
		}
		long long num = atoll(num_c);
		if (num < 0)
			pos = 0;
		else
			pos = num;
	}
	else
		pos = 0;

	posfile.close();
	return pos;
}

/***************************************************************************
  函数名称：
  功    能：重写position.saving里记录的位置
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void WritePos(const string bookname, const streampos pos)
{
	string str;
	ifstream inpos(".\\book\\position.saving", ios::in | ios::binary);
	ofstream outtemp(".\\book\\temp.saving", ios::out | ios::binary);//临时文件
	bool find = 0;

	while (inpos.peek() != EOF)//找名字
	{
		char ch[1000] = { 0 };
		inpos.getline(ch, 999);
		str = ch;
		outtemp << ch << endl;
		if (str == "[" + bookname + ".txt]")
		{
			find = 1;
			break;
		}	
	}

	if (find == 1)
	{
		while (inpos.peek() != EOF)
		{
			char ch[1000] = { 0 };
			inpos.getline(ch, 999);
			if (Is_Blank(ch) == 0)
			{
				outtemp << pos << endl;
				break;
			}
			else
				outtemp << ch << endl;
		}
		while (inpos.peek() != EOF)
		{
			char ch[1000] = { 0 };
			inpos.getline(ch, 999);
			outtemp << ch << endl;
		}
	}
	else//没找到，在文件最后补写
	{
		outtemp << endl;
		outtemp << "[" << bookname << ".txt]" << endl;
		outtemp << pos << endl;
	}

	outtemp.close();
	inpos.close();

	ofstream outpos(".\\book\\position.saving", ios::out | ios::binary);
	ifstream intemp(".\\book\\temp.saving", ios::in | ios::binary);

	while (intemp.peek() != EOF)
	{
		char ch[1000] = { 0 };
		intemp.getline(ch, 999);
		outpos << ch << endl;
	}
	
	outpos.close();
	intemp.close();
	remove(".\\book\\temp.saving");//删除临时文件
}