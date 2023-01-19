/*  2050598  通信  杨洁宁  */
#include <iostream>
#include <Windows.h>
#include <math.h>
#include <conio.h>
#include "../include/cmd_hdc_tools.h"
using namespace std;

#define PI 3.14159

/*阿基米德螺旋线*/

/*中心点坐标*/
const int center_x = 400;
const int center_y = 400;
const int half_wide = 500;
const int half_height = 500;
/*终点的角度*/
const int angle = 990;

const int a = 0;
const double b = 0.3;

void hdc_draw_Mathematical_curve_2050598(const int thickness,const int RGB_value)
{
	hdc_cls();

	hdc_line(center_x - half_wide, center_y, center_x + half_wide, center_y, 4);
	hdc_line(center_x, center_y - half_height, center_x, center_y + half_height, 4);
	int ang, x, y;
	double r;
	int old_x = INT_MAX;
	int old_y = INT_MAX;
	for (ang = 0; ang <= angle; ang++)
	{
		r = a + b * ang;
		x = int(center_x + r * sin(ang * PI / 180));
		y = int(center_y - r * cos(ang * PI / 180));
		if (x != old_x || y != old_y)
		{
			if (ang == 0)
				hdc_point(x, y, thickness, RGB_value);
			else
			{
				hdc_line(old_x, old_y, x, y, thickness, RGB_value);
				old_x = x;
				old_y = y;
			}
		}
	}
	cct_setcolor();
	cct_gotoxy(0, 0);
	cout << "按Q退出";
	while (1)
	{
		char ch = _getch();
		if (ch == 'q' || ch == 'Q')
			return;
	}
}