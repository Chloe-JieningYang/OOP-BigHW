/*2050598 ͨ�� �����*/
#pragma once

struct matrix_sign {
	int matrix[12][12];
	int sign[12][12];
};

//������ʼ������
void CreatMatrix(int i, int j, int m[12][12]);

//������ͬ������
void EraseSame(int i0, int j0, int m[12][12], int s[12][12]);

//ѡ��ĳһ��
void InputSelection(int m[12][12], int& c, int& r, int i, int j);

//������ģʽȷ��ѡ��
char Confirm(int j0, int i0);

//����һ���ĵ÷�
int AddScore(int s[12][12], int zhi);

//�������ս�����
int AwardScore(int m[12][12], int i, int j);

//���������һ������
int OrderLineOneErase(int i, int j, int m[12][12], int s[12][12], int& sc);

//���һ��αͼ�ε�ѡ�е������Ĺ���
void OneErase(int i, int j, int length, int height, int m[][12], int s[][12], char choice, bool splitline, int& score, bool& qui);

//��Ϸ����ѭ��
void CompleteGame(int i, int j, int length, int height, int m[][12], int s[][12], bool splitline, char choice, int& score, bool& quit);
