/* 通信 2050598 杨洁宁 */
#pragma once

//需要输入游戏名来区分,只是内部数组中的移动
void RemoveZero(int i, int j, int m[][12], int s[][12], const char game[]);

void ZeroColMoveLeft(int i, int j, int m[][12], int s[][12]);

//伪图形界面的移动+数组的移动
void BlockFallDown2(int m[][12], int s[][12], int i, int j, int length, int height, bool splitline, const char game[]);

void ColBlockMoveLeft(int m[][12], int s[][12], int i, int j, int length, int height, bool splitline, int style);
