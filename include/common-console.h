/* 2050598 ����� ͨ�� */
#pragma once

//���������ת����������
int MousePositionAndKeyboardToIJ(int m[][12], int& I, int& J, int i, int j, int length, int height, bool splitline, int last = 0);

//��ʾһ��ѡ��
void ChooseOneBlock(int m[][12], int i, int j, int length, int height, int& i0, int& j0, char choice, bool splitline, int style, const char game[], bool& quit);