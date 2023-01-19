/*2050598 通信 杨洁宁*/
#pragma once

struct matrix_sign {
	int matrix[12][12];
	int sign[12][12];
};

//构建初始的数组
void CreatMatrix(int i, int j, int m[12][12]);

//消除相同的星星
void EraseSame(int i0, int j0, int m[12][12], int s[12][12]);

//选中某一格
void InputSelection(int m[12][12], int& c, int& r, int i, int j);

//命令行模式确认选中
char Confirm(int j0, int i0);

//计算一步的得分
int AddScore(int s[12][12], int zhi);

//计算最终奖励分
int AwardScore(int m[12][12], int i, int j);

//命令行完成一次消除
int OrderLineOneErase(int i, int j, int m[12][12], int s[12][12], int& sc);

//完成一次伪图形的选中到结束的过程
void OneErase(int i, int j, int length, int height, int m[][12], int s[][12], char choice, bool splitline, int& score, bool& qui);

//游戏完整循环
void CompleteGame(int i, int j, int length, int height, int m[][12], int s[][12], bool splitline, char choice, int& score, bool& quit);
