#pragma once
/* 2050598 ����� ͨ�Ź��� */

struct matrix_sign {
	int matrix[10][12];
	int sign[10][12];
};

//�����ʼ����
void CreatMatrix(int i, int j, int m[10][12]);


//�ϲ���ͬ
void CombineSame(int i0, int j0, int m[10][12], int s[10][12]);


//�����ֵ
int SearchForMax(int m[10][12]);

//���������
void AddNewNum(int i, int j, int m[10][12], int max);

//ȷ���Ƿ�ϲ�
char Confirm(int j0, int i0);

//��end
void End(int height, int i, bool splitline, bool fakefigure);

//ѡ��ĳһ��
void InputSelection(int m[10][12], int& c, int& r, int i, int j);

//����ϳ�Ŀ��
void InputAim(int& a);

//������������
void InputRowCol(int& c, int& r);

//����÷� ���ص��� �ı��ܷ�
int AddScore(int s[10][12], int zhi);

//�������˵�
int Choice3(int aim, int i, int j, int m[10][12], int s[10][12], int& sco);


//��ʾ��ѡ����
//void ChooseOneBlock(int m[10][12], int i, int j, int& i0, int& j0, char choice, bool& quit);


//αͼ�κϲ�һ�β�����0
void OneCombine(int i, int j, int length, int height, int m[][12], int s[][12], char choice, int& score, bool& qui, int aim);

//αͼ��������Ϸ
void CompleteGame(int i, int j, int length, int height, int m[10][12], int s[10][12], char choice, int& score, bool& quit, int aim);

