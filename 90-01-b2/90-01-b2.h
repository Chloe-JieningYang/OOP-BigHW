#pragma once
/* 2050598 杨洁宁 通信工程 */

struct matrix_sign {
	int matrix[10][12];
	int sign[10][12];
};

//构造初始数组
void CreatMatrix(int i, int j, int m[10][12]);


//合并相同
void CombineSame(int i0, int j0, int m[10][12], int s[10][12]);


//找最大值
int SearchForMax(int m[10][12]);

//添加新数字
void AddNewNum(int i, int j, int m[10][12], int max);

//确认是否合并
char Confirm(int j0, int i0);

//输end
void End(int height, int i, bool splitline, bool fakefigure);

//选中某一格
void InputSelection(int m[10][12], int& c, int& r, int i, int j);

//输入合成目标
void InputAim(int& a);

//输入总行总列
void InputRowCol(int& c, int& r);

//计算得分 返回单次 改变总分
int AddScore(int s[10][12], int zhi);

//第三个菜单
int Choice3(int aim, int i, int j, int m[10][12], int s[10][12], int& sco);


//显示所选方块
//void ChooseOneBlock(int m[10][12], int i, int j, int& i0, int& j0, char choice, bool& quit);


//伪图形合并一次并下落0
void OneCombine(int i, int j, int length, int height, int m[][12], int s[][12], char choice, int& score, bool& qui, int aim);

//伪图形完整游戏
void CompleteGame(int i, int j, int length, int height, int m[10][12], int s[10][12], char choice, int& score, bool& quit, int aim);

