#pragma once
/* ͨ�� 2050598 ����� */

bool JudgeRound(int m[][12], int i0, int j0);
bool JudgeAll(int m[][12], int j, int i);//����λ�ö�û��������ͬ�ķ���True

void FindSameNoRecursion(int m[][12], int s[][12], int i0, int j0, int i, int j);//ǰ����JudgeRound����true
void FindSameWithRecursion(int m[][12], int s[][12], int i0, int j0);//ǰ����JudgeRound����true