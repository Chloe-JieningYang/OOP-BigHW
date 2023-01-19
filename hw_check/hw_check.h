/* 2050598 杨洁宁 通信 */
#pragma once

/* 加入自己需要的定义 */

#define FAIL_OPEN        -1

#define SINGLE_NOTE      1
#define MULTIPLE_NOTE    2
#define ERROR_NOTE       3//不是//开头或不是同一行/*

#define SPECIAL_TYPE     4//特判 指/**/类型


struct partner {
	int no;
	char name[40] = { '\0' };
};

struct student {
	int class_no;//课号
	int no;//学号
	char name[33] = { '\0' };//姓名
	int g_ok;//原始程序（b2)的正确性
	int c_ok;//检查程序(b3)正确性
	int c_de_point1 = 0;//检查程序扣分1
	int c_de_point2 = 0;//检查程序扣分2
	int partner_num = 0;//互查的人数
	partner check_group[20];//用来存放互验同学的信息
};