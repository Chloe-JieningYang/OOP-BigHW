/*2050598 ����� ͨ��*/
#define _CRT_SECURE_NO_WARNINGS
#include <io.h>
#include <vector>
#include <conio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include "../include/common-readtool-menu.h"
#include "../include/cmd_console_tools.h"
using namespace std;


/*��ȡ�ļ�*/
void getFiles(string path, vector<string>& files)
{
    //�ļ����  
    long   hFile = 0;
    //�ļ���Ϣ������һ���洢�ļ���Ϣ�Ľṹ��  
    struct _finddata_t fileinfo;
    string p;//�ַ��������·��
    if ((hFile = _findfirst(p.assign(path).append("\\*.txt").c_str(), &fileinfo)) != -1)//�����ҳɹ��������
    {
        do
        {
            //�����Ŀ¼,����֮�����ļ����ڻ����ļ��У�  
            if ((fileinfo.attrib & _A_SUBDIR))
            {
                //�ļ���������"."&&�ļ���������".."
                //.��ʾ��ǰĿ¼
                //..��ʾ��ǰĿ¼�ĸ�Ŀ¼
                //�ж�ʱ�����߶�Ҫ���ԣ���Ȼ�����޵ݹ�������ȥ�ˣ�
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
            }
            //�������,�����б�  
            else
            {
                files.push_back(p.assign(path).append("\\").append(fileinfo.name));
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        //_findclose������������
        _findclose(hFile);
    }
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���file��������ļ���ַת�����ļ���
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
string BookName(const vector<string>& files, const int no)
{
    int size = files.size();
    if (no < 0 || no >= size)
        return "";
    string temp = files[no];
    int tlen = temp.length();
    int postxt = temp.find(".txt");
    temp.erase(postxt);
    string temp2;
    temp2.assign(temp, 7);
    return temp2;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ӡĿ¼��һ��
  ����������������Ƿ�ѡ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void ShowCatalogueBookName(const string bookname, bool chosen)
{
    if (chosen)
        cct_setcolor(COLOR_WHITE, COLOR_BLACK);
    else
        cct_setcolor();
    string bookname1 = bookname;
    const int len = bookname1.size();
    if (len <= WIDTH - 4)//����С����Ļ��ȣ�ֱ�����
    {
        cout << bookname1.c_str() << endl;
    }
    else//������������ʾ�����
    {
        /*����bookname*/
        int chinese = 0;
        for (int c = 0; c < WIDTH - 4; c++)
        {
            if (bookname[c] >= 128 && bookname[c] <= 256)
                ++chinese;
            if (c == WIDTH - 5)//���һ���ַ�����Ҫ�ж��ǲ������ĵ�ǰһ�룬�ǵĻ�ɾ��                    
            {
                if (bookname[c] >= 128 && bookname[c] <= 256 && chinese % 2 == 1)
                    bookname1[c] = '\0';              
            }
        }
        bookname1[WIDTH - 4] = '\0'; 
        cout << bookname1.c_str() << endl;//����endl��û��bug�ˣ����ǹ���
    }
    cct_setcolor();
    return;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ӡĿ¼
  ����������ļ����飬�ļ���������һ���ļ��±�(��0��ʼ����
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void ShowCatalogue(const vector<string>& files, const int file_num, const int start_no)
{
    for (int k = start_no; k < file_num; k++)
    {
        if (k - start_no > HEIGHT - 2 - 1)//������ʾ��߶�
            break;
        cct_gotoxy(LEFT_EXT_COL + 2, TOP_EXT_LINE + 1 + k - start_no);
        string bookname = BookName(files, k);
        ShowCatalogueBookName(bookname, 0);
        bookname.assign("");
    }
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���С��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void ClearScreen()
{
    cct_setcolor();
    for (int k = 1; k <= MAX_LINE; k++)
    {
        cct_gotoxy(LEFT_EXT_COL + 2, TOP_EXT_LINE + k);
        cout << setfill(' ') << setw(MAX_COL) << " ";
    }
}

/*��ȡbook�ļ���+��ʾ�˵�*/
void ShowMenu(vector<string>& files)
{
    string filePath = ".\\book";//����Ŀ¼

    //��ȡ��·���µ������ļ�
    getFiles(filePath, files);

    int file_num = files.size();
    
    cct_cls();
    cct_setconsoleborder(80, 30);//cmd����80��30��

    /*�����*/
    cct_gotoxy(LEFT_EXT_COL, TOP_EXT_LINE);
    cout << "��";
    for (int k = 1; k <= (WIDTH - 4)/2; k++)
        cout << "��";
    cout << "��";
    for (int line = 1; line <= HEIGHT - 2; line++)
    {
        cct_gotoxy(LEFT_EXT_COL, TOP_EXT_LINE + line);
        cout << "��";
        cct_gotoxy(LEFT_EXT_COL + WIDTH - 2, TOP_EXT_LINE + line);
        cout << "��";
    }
    cct_gotoxy(LEFT_EXT_COL, TOP_EXT_LINE + HEIGHT - 1);
    cout << "��";
    for (int k = 1; k <= (WIDTH - 4)/2; k++)
        cout << "��";
    cout << "��";
    /*�����*/
    ClearScreen();
    /*��ӡĿ¼*/
    ShowCatalogue(files, file_num, 0);
    
    cct_gotoxy(0, TOP_EXT_LINE + HEIGHT + 1);
    cout << setw(50) << setfill(' ') << " ";
    cct_gotoxy(0, TOP_EXT_LINE + HEIGHT + 1);
    cout << "��/�¼�ѡ����Ŀ���س���ȷ�ϣ�Q/q���˳�" << endl;
}
