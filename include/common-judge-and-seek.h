#pragma once
/* 通信 2050598 杨洁宁 */

bool JudgeRound(int m[][12], int i0, int j0);
bool JudgeAll(int m[][12], int j, int i);//所有位置都没有相邻相同的返回True

void FindSameNoRecursion(int m[][12], int s[][12], int i0, int j0, int i, int j);//前提是JudgeRound返回true
void FindSameWithRecursion(int m[][12], int s[][12], int i0, int j0);//前提是JudgeRound返回true