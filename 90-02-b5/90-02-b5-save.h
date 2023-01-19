/*2050598 杨洁宁 通信*/
#pragma once
#include <iostream>
using namespace std;

/*读取position.saving里记录的位置*/
streampos GetPosSaving(string bookname);

/*记录position.saving*/
void WritePos(const string bookname, const streampos pos);