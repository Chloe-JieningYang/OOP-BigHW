/* ͨ�Ź��� 2050598 ����� */
#include <iostream>
#include <conio.h>
#include "90-01-b1-hanoi.h"
#include "..\include\cmd_console_tools.h"
#include "..\include\common-menu.h"
using namespace std;

/* ----------------------------------------------------------------------------------

     ���ļ����ܣ�
	1����main����
	2����ʼ����Ļ
	3�����ò˵�������hanoi_menu.cpp�У�������ѡ��
	4������ѡ����ò˵������Ӧ��ִ�к�����hanoi_multiple_solutions.cpp�У�

     ���ļ�Ҫ��
	1����������ȫ�ֱ��������ⲿȫ�ֺ;�̬ȫ�֣�const��#define�������Ʒ�Χ�ڣ�
	2����̬�ֲ����������������ƣ���ʹ��׼��Ҳ�ǣ����á����á��ܲ��þ�������
	3���������ϵͳͷ�ļ����Զ���ͷ�ļ��������ռ��

   ----------------------------------------------------------------------------------- */

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
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
	/* demo������ִ�д˾䣬��cmd��������Ϊ40��x120�У����������120�У�����9000�У���cmd�����Ҳ���д�ֱ�����ˣ�*/
	cct_setconsoleborder(120, 40, 120, 9000);
	int n = 0;
	char init, aim, med;
	init = '0';
	char choice;
	while (1){
		cct_cls();
		const char instruction[][100] = { "1.������","2.������(������¼)",
		"3.�ڲ�������ʾ(����)","4.�ڲ�������ʾ(����+����)","5.ͼ�ν�-Ԥ��-������Բ��",
		"6.ͼ�ν�-Ԥ��-����ʼ���ϻ�n������","7.ͼ�ν�-Ԥ��-��һ���ƶ�","8.ͼ�ν�-�Զ��ƶ��汾",
		"9.ͼ�ν�-��Ϸ��","0.�˳�" ,"\0"};
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
