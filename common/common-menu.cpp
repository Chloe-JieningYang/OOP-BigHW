/* ͨ�� 2050598 ����� */
#include <iostream>
#include <iomanip>
#include <conio.h>
#include "..\include\cmd_console_tools.h"
using namespace std;

/*
common�ļ�Ҫ�ǵã�
дͷ�ļ�
ɾ��ԭ��ͷ�ļ��ں���������У�
������ͷ�ļ�
������common.cpp�ļ�
*/

/* ----------------------------------------------------------------------------------

     ���ļ����ܣ�
	1���ű� hanoi_main.cpp ���õĲ˵�������Ҫ����ʾ���˵��������ȷ��ѡ��󷵻�

     ���ļ�Ҫ��
	1�����������ⲿȫ�ֱ�����const��#define�������Ʒ�Χ�ڣ�
	2���������徲̬ȫ�ֱ�����ȫ�ֱ�����ʹ��׼���ǣ����á����á��ܲ��þ������ã�
	3����̬�ֲ����������������ƣ���ʹ��׼��Ҳ�ǣ����á����á��ܲ��þ�������
	4���������ϵͳͷ�ļ����Զ���ͷ�ļ��������ռ��

   ----------------------------------------------------------------------------------- */


/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
char menu(const char instruction[][100])//����Ĳ����ǲ˵�����
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
	cout << "[��ѡ��:] ";

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