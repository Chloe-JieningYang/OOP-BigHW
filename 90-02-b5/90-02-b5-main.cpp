/* 2050598 Í¨ÐÅ Ñî½àÄþ*/
#define _CRT_SECURE_NO_WARNINGS
#include <io.h>
#include <vector>
#include <conio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include "../include/common-readtool-menu.h"
#include "../include/cmd_console_tools.h"
#include "90-02-b5-tool.h"
using namespace std;

int main()
{
	int open;
	
	while (1)
	{
		vector<string> files;
		ShowMenu(files);
		open = OpenBook(files);
		if (open < 0)
			break;
	}
	
	return 0;
}