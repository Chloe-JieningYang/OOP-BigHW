/* 2050598 ����� ͨ�� */
#pragma once
#include <iostream>
using namespace std;
#define LEFT_EXT_COL  20  //�����Ķ���������߿��x��ʼ
#define TOP_EXT_LINE  3   //�����Ķ���������߿��y��ʼ
#define HEIGHT        13  //�����Ķ���������߿�ĸ߶�
#define WIDTH         16  //�����Ķ���������߿�Ŀ��
#define MAX_LINE      HEIGHT-2 //�����������
#define MAX_COL       WIDTH-4  //�����������

/*��file��������ļ���ַת�����ļ���*/
string BookName(const vector<string>& files, const int no);

/*��ӡĿ¼��һ������*/
void ShowCatalogueBookName(string bookname, bool chosen);

/*��ӡĿ¼*/
void ShowCatalogue(const vector<string>& files, const int file_num, const int start_no);

/*��ȡbook�ļ���+��ʾ�˵�*/
void ShowMenu(vector<string>& files);

/*��С��*/
void ClearScreen();