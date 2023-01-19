/* 通信工程 2050598 杨洁宁 */
#include <iostream>
#include <conio.h>
#include "90-01-b1-hanoi.h"
#include "..\include\cmd_console_tools.h"
#include "..\include\common-menu.h"
using namespace std;

/* ----------------------------------------------------------------------------------

     本文件功能：
	1、放main函数
	2、初始化屏幕
	3、调用菜单函数（hanoi_menu.cpp中）并返回选项
	4、根据选项调用菜单各项对应的执行函数（hanoi_multiple_solutions.cpp中）

     本文件要求：
	1、不允许定义全局变量（含外部全局和静态全局，const及#define不在限制范围内）
	2、静态局部变量的数量不限制，但使用准则也是：少用、慎用、能不用尽量不用
	3、按需加入系统头文件、自定义头文件、命名空间等

   ----------------------------------------------------------------------------------- */

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/

void exe(char choice,int n,char init,char med,char aim)
{
	if (choice == '1' || choice == '2' || choice == '3') {
		hanoi(n, init, med, aim, choice);
	}
	else if (choice == '4') {
		cct_cls();
		initialize(n, init, aim, choice);
		hanoi(n, init, med, aim, choice);
	}
	else if (choice == '5') {
		cct_cls();
		draw_three_pole();
	}
	else if (choice == '6') {
		cct_cls();
		draw_six(n, init);
	}
	else if (choice == '7') {
		cct_cls();
		draw_seven(n, init, aim);
	}
	else if (choice == '8') {
		cct_cls();
		initialize(n, init, aim, choice);
		hanoi(n, init, med, aim, choice);
	}
	else if (choice == '9') {
		cct_cls();
		initialize(n, init, aim, choice);
		nine(n, aim);
	}
}

int main()
{
	/* demo中首先执行此句，将cmd窗口设置为40行x120列（缓冲区宽度120列，行数9000行，即cmd窗口右侧带有垂直滚动杆）*/
	cct_setconsoleborder(120, 40, 120, 9000);
	int n = 0;
	char init, aim, med;
	init = '0';
	char choice;
	while (1){
		cct_cls();
		const char instruction[][100] = { "1.基本解","2.基本解(步数记录)",
		"3.内部数组显示(横向)","4.内部数组显示(纵向+横向)","5.图形解-预备-画三个圆柱",
		"6.图形解-预备-在起始柱上画n个盘子","7.图形解-预备-第一次移动","8.图形解-自动移动版本",
		"9.图形解-游戏版","0.退出" ,"\0"};
		choice = menu(instruction);
		if (choice == '0')
			return 0;
		if (choice != '5')
			input(&n, &init, &aim, &med, choice);
		init_store(n, init);
		exe(choice, n, init, med, aim);
		end_(choice);
		while (1){
			int c = _getch();
			if (c == '\r')
				break;
		}
	}
	return 0;
}
