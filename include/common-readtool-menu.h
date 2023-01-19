/* 2050598 杨洁宁 通信 */
#pragma once
#include <iostream>
using namespace std;
#define LEFT_EXT_COL  20  //整个阅读区域包括边框的x起始
#define TOP_EXT_LINE  3   //整个阅读区域包括边框的y起始
#define HEIGHT        13  //整个阅读区域包括边框的高度
#define WIDTH         16  //整个阅读区域包括边框的宽度
#define MAX_LINE      HEIGHT-2 //内容最大行数
#define MAX_COL       WIDTH-4  //内容最大列数

/*将file里的完整文件地址转化成文件名*/
string BookName(const vector<string>& files, const int no);

/*打印目录的一行书名*/
void ShowCatalogueBookName(string bookname, bool chosen);

/*打印目录*/
void ShowCatalogue(const vector<string>& files, const int file_num, const int start_no);

/*读取book文件夹+显示菜单*/
void ShowMenu(vector<string>& files);

/*清小屏*/
void ClearScreen();