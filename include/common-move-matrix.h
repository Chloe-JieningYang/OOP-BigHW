/* ͨ�� 2050598 ����� */
#pragma once

//��Ҫ������Ϸ��������,ֻ���ڲ������е��ƶ�
void RemoveZero(int i, int j, int m[][12], int s[][12], const char game[]);

void ZeroColMoveLeft(int i, int j, int m[][12], int s[][12]);

//αͼ�ν�����ƶ�+������ƶ�
void BlockFallDown2(int m[][12], int s[][12], int i, int j, int length, int height, bool splitline, const char game[]);

void ColBlockMoveLeft(int m[][12], int s[][12], int i, int j, int length, int height, bool splitline, int style);
