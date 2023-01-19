/* 2050598 ����� ͨ�� */
#include <iostream>
using namespace std;

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ǰ������Χû����ͬ�ķ���false������ֵͬ����True
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
bool JudgeRound(int m[][12], int i0, int j0)
{
	if (m[i0][j0 + 1] != m[i0][j0] && m[i0][j0 - 1] != m[i0][j0] && m[i0 + 1][j0] != m[i0][j0] && m[i0 - 1][j0] != m[i0][j0])
		return false;
	else
		return true;
}

bool JudgeAll(int m[][12], int j, int i)//����λ�ö�û��������ͬ�ķ���True
{
	int i1, j1;
	for (i1 = 1; i1 <= i; i1++)
	{
		for (j1 = 1; j1 <= j; j1++)
		{
			if (m[i1][j1] != 0)
			{
				if (JudgeRound(m, i1, j1) == true)
					return false;
			}
		}
	}
	return true;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ѡ�к�ѡ����Χ�ķ���
  ���������
  �� �� ֵ��
  ˵    ����ǰ����JudgeRound����True
***************************************************************************/
void FindSameNoRecursion(int m[][12], int s[][12], int i0, int j0, int i,int j)//ǰ����JudgeRound����true
{
	s[i0][j0] = 1;
	while (1) {
		bool end = 1;
		int c, r;
		for (r = 1; r <= i; r++) {
			for (c = 1; c <= j; c++) {
				if (s[r][c] == 1) {
					if (m[r][c + 1] == m[r][c] && s[r][c + 1] == 0) {
						end = 0;
						s[r][c + 1] = 1;
					}
					if (m[r][c - 1] == m[r][c] && s[r][c - 1] == 0) {
						end = 0;
						s[r][c - 1] = 1;
					}
					if (m[r + 1][c] == m[r][c] && s[r + 1][c] == 0) {
						end = 0;
						s[r + 1][c] = 1;
					}
					if (m[r - 1][c] == m[r][c] && s[r - 1][c] == 0) {
						end = 0;
						s[r - 1][c] = 1;
					}
				}
			}
		}
		if (end)
			break;
	}
}

void FindSameWithRecursion(int m[][12], int s[][12], int i0, int j0)//ǰ����JudgeRound����true
{
	s[i0][j0] = 1;
	if (JudgeRound(m, i0, j0) == false)
		return;
	else
	{
		if (m[i0][j0 + 1] == m[i0][j0] && s[i0][j0 + 1] == 0)
			FindSameWithRecursion(m, s, i0, j0 + 1);
		if (m[i0][j0 - 1] == m[i0][j0] && s[i0][j0 - 1] == 0)
			FindSameWithRecursion(m, s, i0, j0 - 1);
		if (m[i0 + 1][j0] == m[i0][j0] && s[i0 + 1][j0] == 0)
			FindSameWithRecursion(m, s, i0 + 1, j0);
		if (m[i0 - 1][j0] == m[i0][j0] && s[i0 - 1][j0] == 0)
			FindSameWithRecursion(m, s, i0 - 1, j0);
	}
}