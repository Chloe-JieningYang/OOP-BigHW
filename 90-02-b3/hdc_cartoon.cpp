/*2050598 杨洁宁 通信*/
#include <iostream>
#include <Windows.h>
#include <math.h>
#include "../include/cmd_hdc_tools.h"
using namespace std;

#define PI 3.14159

/*描线的颜色*/
const int Mycolor_outline = RGB(0, 0, 0);

const int DELTA_X = 0;
const int DELTA_Y = 0;
const int WIDE = 580;//整个图像宽度
const int HEIGHT = 820;//整个图像高度

/*顺时针123给出三点，用圆弧连接*/
/*看了助教gg的代码，参考了一下思路ORZ*/
static void ConnectArc(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, const int thickness, const int RGB_value, const bool filled = FALSE)
{
	double A = x1 * (y2 - y3) - y1 * (x2 - x3) + x2 * y3 - x3 * y2;
	double B = (pow(x1, 2) + pow(y1, 2)) * (y3 - y2) + (pow(x2, 2) + pow(y2, 2)) * (y1 - y3) + (pow(x3, 2) + pow(y3, 2)) * (y2 - y1);
	double C = (pow(x1, 2) + pow(y1, 2)) * (x2 - x3) + (pow(x2, 2) + pow(y2, 2)) * (x3 - x1) + (pow(x3, 2) + pow(y3, 2)) * (x1 - x2);
	double D = (pow(x1, 2) + pow(y1, 2)) * (x3 * y2 - x2 * y3) + (pow(x2, 2) + pow(y2, 2)) * (x1 * y3 - x3 * y1) + (pow(x3, 2) + pow(y3, 2)) * (x2 * y1 - x1 * y2);
	double x0 = -B / (2 * A);
	double y0 = -C / (2 * A);
	double r = sqrt(pow(x1 - x0, 2) + pow(y1 - y0, 2));

	double angle_start_cos = (-1) * (y1 - y0) / r;
	double angle_end_cos = (-1) * (y3 - y0) / r;
	double angle_start = acos(angle_start_cos) / PI * 180;
	double angle_end = acos(angle_end_cos) / PI * 180;

	/*调整角度正负*/
	angle_start = angle_start * (x1 - x0) / abs(x1 - x0);
	angle_end = angle_end * (x3 - x0) / abs(x3 - x0);

	if (filled == FALSE)
		hdc_arc(int(x0), int(y0), int(r), int(angle_start), int(angle_end), thickness, RGB_value);
	else
		hdc_sector(int(x0), int(y0), int(r), int(angle_start), int(angle_end), true, thickness, RGB_value);

}

const int Mycolor_body_grey = RGB(95, 95, 95);
const int Mycolor_body_white = RGB(245, 245, 245);


/*身体*/
/*填充*/
static void ColorBody(const int base_x, const int base_y)
{
	const int x = base_x - WIDE / 2 + DELTA_X;
	const int y = base_y - HEIGHT / 2 + DELTA_Y;
	/*耳朵*/
	hdc_sector(x + 150, y + 85, 40, -130, 56, true, 2, Mycolor_body_grey);//左耳
	hdc_arc(x + 150, y + 85, 40, -130, 56, 4, Mycolor_outline);//左耳勾线
	hdc_sector(x + 420, y + 55, 38, -47, 120, true, 2, Mycolor_body_grey);//右耳
	hdc_arc(x + 420, y + 55, 38, -47, 120, 4, Mycolor_outline);//右耳勾线

	/*头*/
	hdc_sector(x + 302, y + 270, 247, -90, 0, 1, 1,Mycolor_body_white);
	hdc_triangle(x + 300, y + 24, x + 390, y + 29, x + 300, y + 190, 1, 1, Mycolor_body_white);
	ConnectArc(x + 300, y + 24, x + 345, y + 22, x + 390, y + 29, 1, Mycolor_body_white, true);
	hdc_sector(x + 240, y + 288, 300, 30, 90, 1, 1, Mycolor_body_white);

	/*尾巴*/
	hdc_sector(x + 90, y + 624, 43, 166, -36, true, 2, Mycolor_body_grey);
	hdc_arc(x + 90, y + 624, 43, 166, -36, 4, Mycolor_outline);//尾巴勾线
	//身子
	hdc_sector(x + 528, y + 427, 500, -95, -72, true, 2, Mycolor_body_grey);//左边黑
	hdc_sector(x + 528, y + 427, 500, -110, -95, true, 2, Mycolor_body_white);//左边白
	hdc_sector(x + 528, y + 427, 500, -125, -110, true, 2, Mycolor_body_grey);//左边黑
	hdc_triangle(x + 541, y + 290, x + 570, y + 390, x + 530, y + 400, true, 1, Mycolor_body_grey);//右边手肘
	ConnectArc(x + 568, y + 378, x + 555, y + 490, x + 459, y + 556, 1, Mycolor_body_grey, true);//右边手肘下部
	hdc_sector(x + 245, y + 555, 300, 81, 90, true, 1, Mycolor_body_grey);//右下灰
	hdc_sector(x + 245, y + 555, 300, 90, 110, true, 1, Mycolor_body_white);//右下白
	hdc_sector(x + 245, y + 555, 300, 110, 122, true, 1, Mycolor_body_grey);//右下灰
	hdc_sector(x + 302, y + 70, 670, 163, 196, true, 1, Mycolor_body_grey);//下灰

	//脚
	hdc_sector(x + 418, y + 700, 300, -107, -92, true,1,Mycolor_body_grey);//左腿左边
	hdc_sector(x + 200, y + 700, 300, 92, 108, true, 1, Mycolor_body_grey);//右腿右边
	hdc_ellipse(x + 185, y + 795, 66, 25, -9, true, 1, Mycolor_body_grey);//左腿脚面
	hdc_ellipse(x + 185, y + 795, 66, 25, -9, false, 4, Mycolor_outline);//左腿脚面先勾线
	hdc_triangle(x + 132, y + 789, x + 215, y + 774, x + 144, y + 709, true, 1, Mycolor_body_grey);//补色
	hdc_sector(x + 190, y + 718, 100, 133, 177, true, 2, Mycolor_body_grey);//左脚脚底
	hdc_sector(x + 205, y + 737, 75, 90, 131, true, 1, Mycolor_body_grey);//左脚脚跟

	hdc_ellipse(x + 430, y + 800, 66, 25, 9, true, 2, Mycolor_body_grey);//右腿脚面
	hdc_ellipse(x + 430, y + 800, 66, 25, 9, false, 4, Mycolor_outline);//脚面先勾线
	hdc_triangle(x + 484, y + 799, x + 484, y + 716, x + 375, y + 800, true, 2, Mycolor_body_grey);//补色
	hdc_sector(x + 450, y + 650, 177, -180, -140, true, 2, Mycolor_body_grey);//右脚脚底
	hdc_sector(x + 391, y + 750, 65, -122, -82, true, 2, Mycolor_body_grey);//右脚脚跟

	//身体最下填白色
	ConnectArc(x + 523, y + 660, x + 262, y + 671, x + 67, y + 602, 2, Mycolor_body_white, true);
	//身体中部灰色
	ConnectArc(x + 540, y + 556, x + 267, y + 576, x + 31, y + 474, 2, Mycolor_body_grey, true);
	hdc_rectangle(x + 460, y + 290, 80, 130, 0, true, 2, Mycolor_body_grey);
	//头部白色
	hdc_triangle(x + 54, y + 271, x + 200, y + 271, x + 160, y + 310, true, 2, Mycolor_body_white);
	ConnectArc(x + 540, y + 293, x + 315, y + 379, x + 57, y + 274, 2, Mycolor_body_white, true);
	hdc_triangle(x + 365, y + 24, x + 290, y + 107, x + 410, y + 134, true, 2, Mycolor_body_white);

	//眼睛
	hdc_circle(x + 263, y + 164, 47, true, 2, Mycolor_body_grey);//左眼
	hdc_ellipse(x + 426, y + 164, 47, 42, 80, true, 2, Mycolor_body_grey);//右眼
	hdc_circle(x + 277, y + 164, 13, true, 2, Mycolor_outline);//左眼珠
	hdc_circle(x + 398, y + 160, 13, true, 2, Mycolor_outline);//右眼珠

	//鼻子
	hdc_sector(x + 385, y + 235, 60, -5, 152, true, 2, Mycolor_body_white);
	hdc_arc(x + 385, y + 235, 60, -5, 152, 4, Mycolor_outline);
	hdc_arc(x + 375, y + 252, 77, -20, 2, 4, Mycolor_outline);
	hdc_line(x + 310, y + 191, x + 350, y + 179, 4, Mycolor_outline);

	//鼻头
	hdc_ellipse(x + 360, y + 207, 30, 13, -8, true, Mycolor_outline);

	//嘴
	hdc_arc(x + 366, y + 310, 25, -55, 35, 4, Mycolor_outline);

	//眉毛
	hdc_arc(x + 255, y + 60, 50, 126, 180, 4, Mycolor_outline);//左眉毛
	hdc_arc(x + 390, y + 65, 31, 161, -135, 4, Mycolor_outline);//右眉毛
}

/*身体勾线*/
static void DrawLine(const int base_x, const int base_y)
{
	const int x = base_x - WIDE / 2 + DELTA_X;
	const int y = base_y - HEIGHT / 2 + DELTA_Y;

	/*头*/
	hdc_arc(x + 302, y + 270, 247, -90, 0, 4, Mycolor_outline);//左
	ConnectArc(x + 300, y + 24, x + 345, y + 22, x + 390, y + 29, 4, Mycolor_outline, false);//中
	hdc_arc(x + 240, y + 288, 300, 30, 90, 4, Mycolor_outline);//右

	/*身子*/
	hdc_arc(x + 528, y + 427, 500, -125, -72, 4, Mycolor_outline);//左
	hdc_line(x + 541, y + 290, x + 570, y + 390, 4, Mycolor_outline);//右手肘
	ConnectArc(x + 568, y + 378, x + 555, y + 490, x + 459, y + 556, 4, Mycolor_outline, false);//右手肘下部
	hdc_arc(x + 245, y + 555, 300, 81, 122, 4, Mycolor_outline);//右下身子

	/*脚*/
	hdc_arc(x + 418, y + 700, 300, -107, -92, 4, Mycolor_outline);//左腿左边
	hdc_arc(x + 200, y + 700, 300, 92, 108, 4, Mycolor_outline);//右腿右边
	hdc_arc(x + 190, y + 718, 100, 133, 177, 4, Mycolor_outline);//左腿脚底
	hdc_arc(x + 205, y + 737, 75, 90, 131, 4, Mycolor_outline);//左脚脚跟
	hdc_arc(x + 450, y + 650, 177, -180, -140, 4, Mycolor_outline);//右脚脚底
	hdc_arc(x + 391, y + 750, 65, -122, -82, 4, Mycolor_outline);//右脚脚跟
	hdc_arc(x + 302, y + 70, 670, 173, 185, 4, Mycolor_outline);//裆部

	/*最下的分界线*/
	ConnectArc(x + 523, y + 660, x + 262, y + 671, x + 67, y + 602, 4, Mycolor_outline);
	/*中间分界线*/
	ConnectArc(x + 540, y + 556, x + 267, y + 576, x + 31, y + 474, 4, Mycolor_outline);
	/*脖子那边分界线*/
	ConnectArc(x + 540, y + 293, x + 315, y + 379, x + 57, y + 274, 4, Mycolor_outline);

	/*手*/
	/*右手*/
	ConnectArc(x + 465, y + 555, x + 425, y + 550, x + 406, y + 541, 4, Mycolor_outline);
	ConnectArc(x + 407, y + 541, x + 378, y + 518, x + 356, y + 486, 4, Mycolor_outline);
	ConnectArc(x + 356, y + 486, x + 362, y + 412, x + 476, y + 421, 4, Mycolor_outline);
	hdc_arc(x + 361, y + 455, 26, -10, 51, 4, Mycolor_outline);
	hdc_arc(x + 375, y + 442, 47, 9, 46, 4, Mycolor_outline);

	/*左手*/
	ConnectArc(x + 152, y + 526, x + 92, y + 463, x + 90, y + 369, 4, Mycolor_outline);
	ConnectArc(x + 292, y + 515, x + 240, y + 536, x + 152, y + 526, 4, Mycolor_outline);
	ConnectArc(x + 206, y + 406, x + 316, y + 414, x + 292, y + 515, 4, Mycolor_outline);
	hdc_arc(x + 321, y + 462, 30, -42, 0, 4, Mycolor_outline);
	hdc_arc(x + 295, y + 454, 55, -38, 3, 4, Mycolor_outline);

	/*左脚趾*/
	hdc_arc(x + 163, y + 815, 20, -90, -30, 4, Mycolor_outline);
	hdc_arc(x + 207, y + 819, 20, -90, -30, 4, Mycolor_outline);

	/*右脚趾*/
	hdc_arc(x + 403, y + 826, 30, 50, 90, 4, Mycolor_outline);
	hdc_arc(x + 444, y + 826, 30, 50, 90, 4, Mycolor_outline);
}

const int Mycolor_flower_orange = RGB(239, 111, 33);//花蕊颜色
const int Mycolor_flower_yellow = RGB(233, 201, 39);//花瓣颜色
/*花花*/
static void Flower(const int base_x, const int base_y)
{
	const int x = base_x - WIDE / 2 + DELTA_X;
	const int y = base_y - HEIGHT / 2 + DELTA_Y;

	ConnectArc(x + 318, y + 375, x + 322, y + 396, x + 320, y + 415, 4, Mycolor_outline);
	hdc_circle(x + 276, y + 331, 21, true, 2, Mycolor_flower_yellow);
	hdc_circle(x + 320, y + 320, 21, true, 2, Mycolor_flower_yellow);
	hdc_circle(x + 290, y + 375, 21, true, 2, Mycolor_flower_yellow);
	hdc_circle(x + 333, y + 364, 21, true, 2, Mycolor_flower_yellow);
	hdc_circle(x + 306, y + 344, 22, true, 2, Mycolor_flower_orange);
}

void hdc_draw_cartoon_2050598(const int base_x, const int base_y)
{
	hdc_cls();

	int cols, lines, buffer_cols, buffer_lines;
	cct_getconsoleborder(cols, lines, buffer_cols, buffer_lines);
	cct_setconsoleborder(cols, lines + 2);
	const int x = base_x - WIDE / 2 + DELTA_X;
	const int y = base_y - HEIGHT / 2 + DELTA_Y;
	
	ColorBody(base_x, base_y);
	DrawLine(base_x, base_y);
	Flower(base_x, base_y);
}