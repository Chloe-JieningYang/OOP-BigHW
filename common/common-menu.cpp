/* 通信 2050598 杨洁宁 */
#include <iostream>
#include <iomanip>
#include <conio.h>
#include "..\include\cmd_console_tools.h"
using namespace std;

/*
common文件要记得：
写头文件
删除原有头文件内函数（如果有）
插入新头文件
插入新common.cpp文件
*/

/* ----------------------------------------------------------------------------------

     本文件功能：
	1、放被 hanoi_main.cpp 调用的菜单函数，要求显示各菜单项，读入正确的选项后返回

     本文件要求：
	1、不允许定义外部全局变量（const及#define不在限制范围内）
	2、不允许定义静态全局变量（全局变量的使用准则是：少用、慎用、能不用尽量不用）
	3、静态局部变量的数量不限制，但使用准则也是：少用、慎用、能不用尽量不用
	4、按需加入系统头文件、自定义头文件、命名空间等

   ----------------------------------------------------------------------------------- */


/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
char menu(const char instruction[][100])//传入的参数是菜单各项
{
	char choice;
	int lenth_of_dash = 0;
	int x = 0, y = 0;
	cct_gotoxy(0, 1);
	
	for (int i = 0; instruction[i][0] != '\0'; i++)
	{
		cout << instruction[i];
		cct_getxy(x, y);
		if (x > lenth_of_dash)
			lenth_of_dash = x;
		cout << endl;
	}
	cct_gotoxy(0, 0);
	cout << setfill('-') << setw(lenth_of_dash) << "-" << endl;
	cct_gotoxy(0, y + 1);
	cout << setfill('-') << setw(lenth_of_dash) << "-" << endl;
	cout << "[请选择:] ";

	while (1)
	{
		choice = _getch();
		if (choice >= '0' && choice <= '9'||choice>='A'&&choice<='Z'||choice>='a'&&choice<='z')
		{
			cout << choice << endl;
			break;
		}
	}
	return choice;
}