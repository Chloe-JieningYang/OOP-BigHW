/* ͨ�Ź��� 2050598 ����� */
#pragma once

/* ------------------------------------------------------------------------------------------------------

     ���ļ����ܣ�
	1��Ϊ�˱�֤ hanoi_main.cpp/hanoi_menu.cpp/hanoi_multiple_solutions.cpp ���໥���ʺ����ĺ�������
	2��һ�����ϵ�cpp���õ��ĺ궨�壨#define����ȫ��ֻ����const����������������
	3�����Բο� cmd_console_tools.h ��д���������Ķ�����ᣩ
   ------------------------------------------------------------------------------------------------------ */

#define x_1 1 //��һ�����ӵ�����������
#define x_2 33
#define x_3 65

//�й�����ͳ�ʼ���ĺ���
//char menu();
void input(int* num, char* i, char* a, char* m, char choice);
void init_store(int n, char init);
void initialize(int n, char src, char dst, char choice);

//�йػ�ͼ�εĺ���
void draw_three_pole();
void draw_six(int n, char src);
void draw_seven(int n, char src, char dst);
void nine(int nn,char aim);
void hanoi(int n, char src, char tmp, char dst, char choice);

//����һ����Ϸ����
void end_(char choice);

