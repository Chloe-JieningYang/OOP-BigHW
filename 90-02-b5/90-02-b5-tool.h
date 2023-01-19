/*2050598 杨洁宁 通信*/
#pragma once


#define MEET_ENDL            1  //遇到换行符就换行
#define MEET_ENDL_AND_BLANK  2  //遇到换行符和空格才换行

/*选中一本书*/
int ChooseBook(const vector<string>& files);

/*打开一本书阅读*/
int OpenBook(const vector<string>& files);