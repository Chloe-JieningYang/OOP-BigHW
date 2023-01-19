/* 2050598 杨洁宁 通信 */
#pragma once

//将鼠标坐标转化成行列数
int MousePositionAndKeyboardToIJ(int m[][12], int& I, int& J, int i, int j, int length, int height, bool splitline, int last = 0);

//显示一次选中
void ChooseOneBlock(int m[][12], int i, int j, int length, int height, int& i0, int& j0, char choice, bool splitline, int style, const char game[], bool& quit);