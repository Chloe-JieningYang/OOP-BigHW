/*2050598 杨洁宁 通信*/
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


/*读取文件*/
void getFiles(string path, vector<string>& files)
{
    //文件句柄  
    long   hFile = 0;
    //文件信息，声明一个存储文件信息的结构体  
    struct _finddata_t fileinfo;
    string p;//字符串，存放路径
    if ((hFile = _findfirst(p.assign(path).append("\\*.txt").c_str(), &fileinfo)) != -1)//若查找成功，则进入
    {
        do
        {
            //如果是目录,迭代之（即文件夹内还有文件夹）  
            if ((fileinfo.attrib & _A_SUBDIR))
            {
                //文件名不等于"."&&文件名不等于".."
                //.表示当前目录
                //..表示当前目录的父目录
                //判断时，两者都要忽略，不然就无限递归跳不出去了！
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
            }
            //如果不是,加入列表  
            else
            {
                files.push_back(p.assign(path).append("\\").append(fileinfo.name));
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        //_findclose函数结束查找
        _findclose(hFile);
    }
}

/***************************************************************************
  函数名称：
  功    能：将file里的完整文件地址转化成文件名
  输入参数：
  返 回 值：
  说    明：
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
  函数名称：
  功    能：打印目录的一行
  输入参数：书名，是否被选中
  返 回 值：
  说    明：
***************************************************************************/
void ShowCatalogueBookName(const string bookname, bool chosen)
{
    if (chosen)
        cct_setcolor(COLOR_WHITE, COLOR_BLACK);
    else
        cct_setcolor();
    string bookname1 = bookname;
    const int len = bookname1.size();
    if (len <= WIDTH - 4)//书名小于屏幕宽度，直接输出
    {
        cout << bookname1.c_str() << endl;
    }
    else//书名超过了显示屏宽度
    {
        /*处理bookname*/
        int chinese = 0;
        for (int c = 0; c < WIDTH - 4; c++)
        {
            if (bookname[c] >= 128 && bookname[c] <= 256)
                ++chinese;
            if (c == WIDTH - 5)//最后一个字符，需要判断是不是中文的前一半，是的话删除                    
            {
                if (bookname[c] >= 128 && bookname[c] <= 256 && chinese % 2 == 1)
                    bookname1[c] = '\0';              
            }
        }
        bookname1[WIDTH - 4] = '\0'; 
        cout << bookname1.c_str() << endl;//加了endl就没有bug了，真是怪事
    }
    cct_setcolor();
    return;
}

/***************************************************************************
  函数名称：
  功    能：打印目录
  输入参数：文件数组，文件数量，第一行文件下标(从0开始数）
  返 回 值：
  说    明：
***************************************************************************/
void ShowCatalogue(const vector<string>& files, const int file_num, const int start_no)
{
    for (int k = start_no; k < file_num; k++)
    {
        if (k - start_no > HEIGHT - 2 - 1)//超过显示框高度
            break;
        cct_gotoxy(LEFT_EXT_COL + 2, TOP_EXT_LINE + 1 + k - start_no);
        string bookname = BookName(files, k);
        ShowCatalogueBookName(bookname, 0);
        bookname.assign("");
    }
}

/***************************************************************************
  函数名称：
  功    能：清小屏
  输入参数：
  返 回 值：
  说    明：
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

/*读取book文件夹+显示菜单*/
void ShowMenu(vector<string>& files)
{
    string filePath = ".\\book";//设置目录

    //获取该路径下的所有文件
    getFiles(filePath, files);

    int file_num = files.size();
    
    cct_cls();
    cct_setconsoleborder(80, 30);//cmd窗口80宽，30高

    /*画外框*/
    cct_gotoxy(LEFT_EXT_COL, TOP_EXT_LINE);
    cout << "┏";
    for (int k = 1; k <= (WIDTH - 4)/2; k++)
        cout << "━";
    cout << "┓";
    for (int line = 1; line <= HEIGHT - 2; line++)
    {
        cct_gotoxy(LEFT_EXT_COL, TOP_EXT_LINE + line);
        cout << "┃";
        cct_gotoxy(LEFT_EXT_COL + WIDTH - 2, TOP_EXT_LINE + line);
        cout << "┃";
    }
    cct_gotoxy(LEFT_EXT_COL, TOP_EXT_LINE + HEIGHT - 1);
    cout << "┗";
    for (int k = 1; k <= (WIDTH - 4)/2; k++)
        cout << "━";
    cout << "┛";
    /*外框画完*/
    ClearScreen();
    /*打印目录*/
    ShowCatalogue(files, file_num, 0);
    
    cct_gotoxy(0, TOP_EXT_LINE + HEIGHT + 1);
    cout << setw(50) << setfill(' ') << " ";
    cct_gotoxy(0, TOP_EXT_LINE + HEIGHT + 1);
    cout << "上/下键选择书目，回车键确认，Q/q键退出" << endl;
}
