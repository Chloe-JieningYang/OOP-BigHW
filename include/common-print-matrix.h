#pragma once
/* ͨ�� 2050598 ����� */

#define UPVACROW 3
#define LEFTVACCOL 4

void OutputMatrixArray(int i, int j, int m[][12], int s[][12], bool sign);//��������ʽ��ʾ

//��һ�����ӵĺ�����style=1Ϊʵ�ߣ�styleΪ0Ϊ����
void DrawOneBlock(int length, int height, int backcolor, int frontcolor, int style, const char content[]);

//����ת��Ϊ�ַ�
void IntToChar(int number, char content[]);


//�����ܵĺ���
void DrawOutLine(int length, int height, int j, int i, int style, bool splitline);


//�����ܼ����ڲ����飬����ѡ�����޿��ߣ�����ʵ�ߣ���ӡɫ�������
void DrawCompleteMatrix(int length, int height, int style, const char game[], int j, int i, bool splitline, int m[][12], const char content[]);
