/* 2050598 杨洁宁 通信 */

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include "../include/cmd_hdc_tools.h"
using namespace std;

#define H_RIGHT        1
#define H_LEFT         2
#define V_UP           3
#define V_DOWN         4
#define CLOCKWISE      1
#define ANTICLOCKWISE  2

#if !HDC_SERIES_BY_TEACHER	//未定义的情况下才打开条件编译

/* 允许添加需要的头文件 */

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow(); //VS中此处会有波浪线，不影响正常使用

/* 已定义的静态全局变量 */
static HWND hWnd = GetConsoleWindow();
static HDC hdc = NULL;
static const double PI = 3.14159;
static int _BgColor_, _FgColor_, _Width_, _High;
static int _Delay_ms;

/* 允许适度使用静态全局变量，但仅限static，即外部不可见 */

/* 此处允许添加自定义函数，但仅限static，即外部不可见 */
/***************************************************************************
  函数名称：
  功    能：将自定义的0轴转化为系统的0轴
  输入参数：接收的增加角度的变量，接收系数的变量，0轴位置(V_UP/V_DOWN/H_RIGHT/HLEFT)，顺时针/逆时针(CLOCKWISE/ANTICLOCKWISE)
  返 回 值：
  说    明：
***************************************************************************/
static void hdc_inner_fit_angle_clockwise(int& add_angle,int& multiply_k, const int zero_position, const int direction)
{
	if (direction == CLOCKWISE)
	{
		if (zero_position == H_RIGHT)
			add_angle = -90;
		else if (zero_position == H_LEFT)
			add_angle = 90;
		else if (zero_position == V_UP)
			add_angle = 180;
		else if (zero_position == V_DOWN)
			add_angle = 0;
		multiply_k = 1;
	}
	else if (direction == ANTICLOCKWISE)
	{
		if (zero_position == H_RIGHT)
			add_angle = 90;
		else if (zero_position == H_LEFT)
			add_angle = -90;
		else if (zero_position == V_UP)
			add_angle = 180;
		else if (zero_position == V_DOWN)
			add_angle = 0;
		multiply_k = -1;
	}
}

/***************************************************************************
  函数名称：
  功    能：四舍五入
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
static int hdc_inner_round(double num)
{
	return (num > 0.0) ? int(floor(num + 0.5)) : int(ceil(num - 0.5));
}

/***************************************************************************
  函数名称：
  功    能：绕某点顺时针旋转一个点
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
static void hdc_inner_rotate(const int point_x, const int point_y, int& x, int& y, const int angle)
{
	int x0 = hdc_inner_round((x - point_x) * cos(angle * PI / 180) - (y - point_y) * sin(angle * PI / 180) + point_x);
	int y0 = hdc_inner_round((x - point_x) * sin(angle * PI / 180) + (y - point_y) * cos(angle * PI / 180) + point_y);
	x = x0;
	y = y0;
}

/* 下面给出了几个基本函数的完整实现，不要改动 */
/***************************************************************************
  函数名称：
  功    能：初始化
  输入参数：const int bgcolor：背景色
            const int fgcolor：前景色
			const int width  ：屏幕宽度（点阵）
			const int high   ：屏幕高度（点阵）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_init(const int bgcolor, const int fgcolor, const int width, const int high)
{
	/* 先释放，防止不release而再次init（hdc_release可重入） */
	hdc_release();

	/* 窗口init后，用一个静态全局量记录，后续hdc_cls()会用到 */
	_BgColor_ = bgcolor;
	_FgColor_ = fgcolor;
	_Width_ = width;
	_High = high;

	hdc = GetDC(hWnd);

	cct_setcursor(CURSOR_INVISIBLE);
	cct_setcolor(bgcolor, fgcolor);
	cct_setfontsize("点阵字体", 16);
	cct_setconsoleborder(width / 8 + !!(width % 8), high / 16 + !!(high % 16)); //将点阵的宽度及高度转换为特定字体的行列数，!!的含义：如果不是8/16的倍数，行列多+1
	cct_cls();
}

/***************************************************************************
  函数名称：
  功    能：释放画图资源
  输入参数：
  返 回 值：
  说    明：可重入
***************************************************************************/
void hdc_release()
{
	if (hdc) {
		ReleaseDC(hWnd, hdc);
		hdc = NULL;
		cct_setcursor(CURSOR_VISIBLE_NORMAL);
	}
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_delay(int ms)
{
	if (ms > 0)
		_Delay_ms = ms;
	else
		_Delay_ms = 0;
}

/***************************************************************************
  函数名称：
  功    能：设置画笔颜色，传入RGB值
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_pencolor(const int RGB_value)
{
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB_value);
}

/***************************************************************************
  函数名称：
  功    能：设置画笔颜色，传入RGB三色，值0-255
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_pencolor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	hdc_set_pencolor(RGB(red, green, blue));
}

/***************************************************************************
  函数名称：
  功    能：清除屏幕上现有的图形
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_cls()
{
    /* 发现一定要换一种颜色初始化才能清除像素点，找到更简便方法的同学可以通知我 */
	hdc_init(_BgColor_, (_FgColor_  + 1) % 16, _Width_, _High);

	hdc_init(_BgColor_, _FgColor_, _Width_, _High);

	/* 部分机器上运行demo时，会出现hdc_cls()后第一根线有缺线的情况，加延时即可
	   如果部分机器运行还有问题，调高此延时值 */
	Sleep(30);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画出一个像素点
  输入参数：const int x：x坐标，左上角为(0,0)
            const int y：y坐标，左上角为(0,0)
  返 回 值：
  说    明：颜色直接用当前设定，可以在调用处指定
***************************************************************************/
static inline void hdc_base_point(const int x, const int y)
{
	MoveToEx(hdc, x - 1, y - 1, NULL);
	LineTo(hdc, x, y);
	if (_Delay_ms > 0)
		Sleep(_Delay_ms);
}

/***************************************************************************
  函数名称：
  功    能：在(x1,y1)-(x2,y2)之间画出一个像素点的连线
  输入参数：const int x1：起点x坐标，左上角为(0,0)
			const int y1：起点y坐标，左上角为(0,0)
			const int x2：终点y坐标，左上角为(0,0)
			const int y2：终点y坐标，左上角为(0,0)
  返 回 值：
  说    明：颜色直接用当前设定
***************************************************************************/
static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
	if (_Delay_ms > 0)
		Sleep(_Delay_ms);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画一个指定粗细的点(用画实心圆来模拟)
  输入参数：const int x			：x坐标，左上角为(0,0)
            const int y			：y坐标，左上角为(0,0)
			const int thickness	：点的粗细，下限位1，上限不限(如过大会导致不完全填充)（有缺省值）
			const int RGB_value	：点的颜色（有缺省值）
  返 回 值：
  说    明：改进运行效率
***************************************************************************/
void hdc_point(const int x, const int y, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//处理下限
	/* 不卡点直径的上限，但是单上限超过一定大小是，画出的圆部分位置未实心 */

	const int tn_end = (tn <= 1) ? 1 : tn/2;

	int angle, level;
	int old_x1 = INT_MAX, old_y1 = INT_MAX, x1, y1;
	int count = 0;

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	/* 用画圆的方式模拟粗点 */
	for (level = 1; level <= tn_end; level++) {
		for (angle = 0; angle <= 360; angle ++) {
			/* 注意，系统的坐标轴，0°在圆的最下方 */
			x1 = x + (int)(level * sin(angle * PI / 180));
			y1 = y - (int)(level * cos(angle * PI / 180));

			/* 当半径很小时，角度变化不会引起int型的x1/y1变化，因此加判断语句，避免重复画同一像素点 */
			if (x1 != old_x1 || y1 != old_y1) {
				old_x1 = x1;
				old_y1 = y1;
				hdc_base_point(x1, y1);
				++count;
			}
		}
	}
#if 0
	/* 放开此注释，可以看到一个粗点用了几个像素点组成 */
	cct_gotoxy(0, 41);
	printf("count=%d           ", count);
	getchar();
#endif
}

/* -------------------------------------------------------------------------
	给出下列函数的实现，函数名及参数表不准改动
	1、不需要调用系统的其他有关图形操作的函数
	2、下列函数的实现都基于hdc_base_point/hdc_base_line函数的组合
	3、想一想，是不是所有函数都需要给出独立的实现过程？应该先实现哪些函数？
	4、填充是实现中最复杂的部分
	5、系统的角度与函数参数中的角度含义相差180°
   ------------------------------------------------------------------------- */

/***************************************************************************
  函数名称：
  功    能：画线段
  输入参数：const int x1		：起点的x
            const int y1		：起点的y
			const int x2		：终点的x
			const int y2		：终点的y
			const int thickness	：线段的粗细（有缺省值）
			const int RGB_value	：线段的颜色（有缺省值）
  返 回 值：
  说    明：本函数可以用基于hdc_point的组合（速度慢）
                  也可以用hdc_base_point/hdc_base_line的组合（速度快）
***************************************************************************/
void hdc_line(const int x1, const int y1, const int x2, const int y2, const int thickness, const int RGB_value)
{
	const int deltax = x2 - x1;
	const int deltay = y2 - y1;
	int tn = thickness;
	if (tn < 1)
		tn = 1;
	int tn_end = (tn <= 1) ? 1 : tn / 2;

	int angle, level;
	int old_x = INT_MAX, old_y = INT_MAX, x, y;

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	for (level = 1; level <= tn_end; level++)
	{
		for (angle = 0; angle <= 360; angle++)
		{
			x= x1 + (int)(level * sin(angle * PI / 180));
			y= y1 - (int)(level * cos(angle * PI / 180));

			if (x != old_x || y != old_y)
			{
				old_x = x;
				old_y = y;
				hdc_base_line(x, y, x + deltax, y + deltay);
			}
		}
	}
}

/***************************************************************************
  函数名称：
  功    能：给出三点的坐标，画一个三角形
  输入参数：const int x1		：第1个点的x
            const int y1		：第1个点的y
			const int x2		：第2个点的x
			const int y2		：第2个点的y
			const int x3		：第3个点的x
			const int y3		：第3个点的y
			bool filled			：是否需要填充（有缺省值）
			const int thickness	：边的粗细（有缺省值）
			const int RGB_value	：颜色（有缺省值）
  返 回 值：
  说    明：不判断三点是否共线，如果共线，划出一条直线即可
***************************************************************************/
void hdc_triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, bool filled, const int thickness, const int RGB_value)
{
	hdc_line(x1, y1, x2, y2, thickness, RGB_value);
	hdc_line(x1, y1, x3, y3, thickness, RGB_value);
	hdc_line(x2, y2, x3, y3, thickness, RGB_value);
	if (filled)
	{
		int y[3] = { y1,y2,y3 };
		int x[3] = { x1,x2,x3 };
		for (int p = 0; p <= 1; p++)//按y从小到大顺序给点排序
		{
			for (int q = p + 1; q <= 2; q++)
			{
				if (y[p] > y[q])
				{
					int tempx = x[p];
					x[p] = x[q];
					x[q] = tempx;
					int tempy = y[p];
					y[p] = y[q];
					y[q] = tempy;
				}
			}
		}
		//最高点下标为0，最低点下标为2
		if (y[0] != y[1] && y[1] != y[2])//如果任意两点不在同一水平线上
		{
			double slope0_1 = double(x[0] - x[1]) / (y[0] - y[1]);
			double slope0_2 = double(x[0] - x[2]) / (y[0] - y[2]);
			double slope1_2 = double(x[1] - x[2]) / (y[1] - y[2]);
			int start_x, end_x;
			for (int by = y[0] + 1; by <= y[2]; by++)
			{
				if (by <= y[1])
				{
					start_x = hdc_inner_round(x[0] + slope0_1 * (by - y[0]));
					end_x = hdc_inner_round(x[0] + slope0_2 * (by - y[0]));
					hdc_base_line(start_x, by, end_x, by);
				}
				else
				{
					start_x = hdc_inner_round(x[1] + slope1_2 * (by - y[1]));
					end_x = hdc_inner_round(x[0] + slope0_2 * (by - y[0]));
					hdc_base_line(start_x, by, end_x, by);
				}
			}
		}
		else if (y[0] == y[1] && y[1] != y[2])
		{
			double slope0_2 = double(x[0] - x[2]) / (y[0] - y[2]);
			double slope1_2 = double(x[1] - x[2]) / (y[1] - y[2]);
			int start_x, end_x;
			for (int by = y[0] + 1; by <= y[2]; by++)
			{
				start_x = hdc_inner_round(x[0] + slope0_2 * (by - y[0]));
				end_x = hdc_inner_round(x[1] + slope1_2 * (by - y[1]));
				hdc_base_line(start_x, by, end_x, by);
			}
		}
		else if (y[1] == y[2] && y[0] != y[1])
		{
			double slope0_1 = double(x[0] - x[1]) / (y[0] - y[1]);
			double slope0_2 = double(x[0] - x[2]) / (y[0] - y[2]);
			int start_x, end_x;
			for (int by = y[0] + 1; by <= y[2]; by++)
			{
				start_x = hdc_inner_round(x[0] + slope0_1 * (by - y[0]));
				end_x = hdc_inner_round(x[0] + slope0_2 * (by - y[0]));
				hdc_base_line(start_x, by, end_x, by);
			}
		}
	}
}

/***************************************************************************
  函数名称：
  功    能：给出左上角坐标及宽度、高度，画出一个长方形
  输入参数：const int left_up_x			：左上角x
            const int left_up_y			：左上角y
			const int width				：宽度
			const int high				：高度
			const int rotation_angles	：以左上角为支点，与x轴的旋转倾角（向下转为正）
			bool filled					：是否需要填充（有缺省值）
			const int thickness			：边的粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_rectangle(const int left_up_x, const int left_up_y, const int width, const int high, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	int add_angle, multi_k;
	hdc_inner_fit_angle_clockwise(add_angle, multi_k, H_RIGHT, CLOCKWISE);
	int real_angle = rotation_angles + add_angle;
	double right_up_x, right_up_y, left_down_x, left_down_y, right_down_x, right_down_y, left_up_x2, left_up_y2;

	right_up_x = left_up_x - multi_k * width * sin(real_angle * PI / 180);
	right_up_y = left_up_y + width * cos(real_angle * PI / 180);
	hdc_line(hdc_inner_round(left_up_x), hdc_inner_round(left_up_y), hdc_inner_round(right_up_x), hdc_inner_round(right_up_y), thickness, RGB_value);

	right_down_x = right_up_x - multi_k * high * sin((real_angle + 90) * PI / 180);
	right_down_y = right_up_y + high * cos((real_angle + 90) * PI / 180);
	hdc_line(hdc_inner_round(right_up_x), hdc_inner_round(right_up_y), hdc_inner_round(right_down_x), hdc_inner_round(right_down_y), thickness, RGB_value);

	left_down_x = right_down_x - multi_k * width * sin((real_angle + 180) * PI / 180);
	left_down_y = right_down_y + width * cos((real_angle + 180) * PI / 180);
	hdc_line(hdc_inner_round(right_down_x), hdc_inner_round(right_down_y), hdc_inner_round(left_down_x), hdc_inner_round(left_down_y), thickness, RGB_value);

	left_up_x2 = left_down_x - multi_k * high * sin((real_angle + 270) * PI / 180);
	left_up_y2 = left_down_y + high * cos((real_angle + 270) * PI / 180);
	hdc_line(hdc_inner_round(left_down_x), hdc_inner_round(left_down_y), hdc_inner_round(left_up_x2), hdc_inner_round(left_up_y2), thickness, RGB_value);

	if (filled)
	{
		int y[4] = { left_up_y,hdc_inner_round(right_up_y),hdc_inner_round(left_down_y),hdc_inner_round(right_down_y) };
		int x[4] = { left_up_x,hdc_inner_round(right_up_x),hdc_inner_round(left_down_x),hdc_inner_round(right_down_x) };
		for (int p = 0; p <= 2; p++)//按y从小到大顺序给点排序
		{
			for (int q = p + 1; q <= 3; q++)
			{
				if (y[p] > y[q])
				{
					int tempx = x[p];
					x[p] = x[q];
					x[q] = tempx;
					int tempy = y[p];
					y[p] = y[q];
					y[q] = tempy;
				}
			}
		}
		//最高点下标为0，最低点下标为3
		if (y[0] != y[1])//如果不是正着摆的长方形
		{
			double slope0_1 = double(x[0] - x[1]) / (y[0] - y[1]);
			double slope0_2 = double(x[0] - x[2]) / (y[0] - y[2]);
			double slope1_3 = double(x[1] - x[3]) / (y[1] - y[3]);
			double slope2_3 = double(x[2] - x[3]) / (y[2] - y[3]);
			int start_x, end_x;
			for (int by = y[0] + 1; by <= y[3] - 1; by++)//分三段来画
			{
				if (by <= y[1])
				{
					start_x = int(x[0] + slope0_1 * (by - y[0]));
					end_x = int(x[0] + slope0_2 * (by - y[0]));
					hdc_base_line(start_x, by, end_x, by);
				}
				else if (by <= y[2])
				{
					start_x = int(x[2] + slope0_2 * (by - y[2]));
					end_x = int(x[1] + slope1_3 * (by - y[1]));
					hdc_base_line(start_x, by, end_x, by);
				}
				else
				{
					start_x = int(x[2] + slope2_3 * (by - y[2]));
					end_x = int(x[3] + slope1_3 * (by - y[3]));
					hdc_base_line(start_x, by, end_x, by);
				}
			}
		}
		else//如果是正着摆的长方形
		{
			for (int by = y[0] + 1; by <= y[3] - 1; by++)
				hdc_base_line(x[0], by, x[1], by);
		}
	}
}

/***************************************************************************
  函数名称：
  功    能：给出左上角坐标及边长，画出一个正方形
  输入参数：const int left_up_x			：左上角x
			const int left_up_y			：左上角y
			const int length			：边长
			const int rotation_angles	：以左上角为支点，与x轴的旋转倾角（向下转为正）（有缺省值）
			const bool filled			：是否需要填充（有缺省值）
			const int thickness			：边的粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_square(const int left_up_x, const int left_up_y, const int length, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	hdc_rectangle(left_up_x, left_up_y, length, length, rotation_angles, filled, thickness, RGB_value);
}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一段圆弧
  输入参数：const int point_x		：圆心的x
            const int point_y		：圆心的y
			const int radius		：半径
			const int angle_begin	：起始角度（以圆的最上方为0°，顺时针为正，注意：与系统坐标差了180°!!!）（有缺省值）
			const int angle_end		：结束角度（同上）（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_arc(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const int thickness, const int RGB_value)
{
	int add_angle, multi_k;
	hdc_inner_fit_angle_clockwise(add_angle, multi_k, V_UP, CLOCKWISE);
	int real_angle_begin = angle_begin + add_angle;
	int real_angle_end = angle_end + add_angle;
	if (real_angle_end - real_angle_begin > 360)
	{
		while (real_angle_end - real_angle_begin > 360)
			real_angle_end -= 360;
	}
	else if (real_angle_end - real_angle_begin <= 0)
	{
		while (real_angle_end <= real_angle_begin)
			real_angle_end += 360;
	}

	double angle;
	int old_x1 = INT_MAX, old_y1 = INT_MAX, x1, y1;
	int count = 0;
	for (angle = real_angle_begin; angle <= real_angle_end; angle += 1)
	{
		x1 = hdc_inner_round(point_x - multi_k * radius * sin(angle * PI / 180));
		y1 = hdc_inner_round(point_y + radius * cos(angle * PI / 180)) ;

		if (x1 != old_x1 || y1 != old_y1)
		{
			if (count == 0)
				hdc_point(x1, y1, thickness, RGB_value);
			else
				hdc_line(old_x1, old_y1, x1, y1, thickness, RGB_value);
			old_x1 = x1;
			old_y1 = y1;
			count++;
		}
	}
}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一个扇形
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const int angle_begin	：起始角度（以圆的最上方为0°，顺时针为正，注意：与系统坐标差了180°!!!）（有缺省值）
			const int angle_end		：结束角度（同上）（有缺省值）
			const bool filled		：是否需要填充（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  输入参数：
  返 回 值：
  说    明：当起始/结束角度差值为360的倍数时，不画两条边
***************************************************************************/
void hdc_sector(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const bool filled, const int thickness, const int RGB_value)
{
	int add_angle, multi_k;
	hdc_inner_fit_angle_clockwise(add_angle, multi_k, V_UP, CLOCKWISE);
	int real_angle_begin = angle_begin + add_angle;
	int real_angle_end = angle_end + add_angle;
	if (real_angle_end - real_angle_begin > 360)
	{
		while (real_angle_end - real_angle_begin > 360)
			real_angle_end -= 360;
	}
	else if (real_angle_end - real_angle_begin <= 0)
	{
		while (real_angle_end <= real_angle_begin)
			real_angle_end += 360;
	}
	int deltaangle = real_angle_end - real_angle_begin;
	int x_begin, y_begin, x_end, y_end;
	x_begin = hdc_inner_round(point_x - multi_k * radius * sin(real_angle_begin * PI / 180));
	y_begin = hdc_inner_round(point_y + radius * cos(real_angle_begin * PI / 180));
	x_end = hdc_inner_round(point_x - multi_k * radius * sin(real_angle_end * PI / 180));
	y_end = hdc_inner_round(point_y + radius * cos(real_angle_end *PI / 180));

	if (deltaangle != 360)
	{
		hdc_line(point_x, point_y, x_begin, y_begin, thickness, RGB_value);
		hdc_line(point_x, point_y, x_end, y_end, thickness, RGB_value);
	}

	hdc_arc(point_x, point_y, radius, angle_begin, angle_end, thickness, RGB_value);

	if (filled)
	{
		double slopeO_begin, slopeO_end;
		if (point_y != y_begin)
			slopeO_begin = double(point_x - x_begin) / (point_y - y_begin);
		if (point_y != y_end)
			slopeO_end = double(point_x - x_end) / (point_y - y_end);
		int x1, x2;
		if (real_angle_begin % 360 >= 0 && real_angle_begin % 360 < 90)//起点在左下区
		{
			if (real_angle_end % 360 > 0 && real_angle_end % 360 <= 90 && deltaangle <= 90)//起点在左下区，终点在左下区，且角度90以内
			{
				
				for (int by = point_y; by <= y_begin; by++)
				{
					if (by < y_end)
					{
						x1 = int(point_x + slopeO_end * (by - point_y));
						x2 = int(point_x + slopeO_begin * (by - point_y));
						hdc_base_line(x1, by, x2, by);
					}
					else
					{
						x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						x2 = int(point_x + slopeO_begin * (by - point_y));
						hdc_base_line(x1, by, x2, by);
					}
				}
			}
			else if (real_angle_end % 360 > 90 && real_angle_end % 360 <= 180)//起点在左下区，终点在左上区
			{
				for (int by = y_end; by <= y_begin; by++)
				{
					if (by < point_y)
					{
						x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						x2 = int(point_x + slopeO_end * (by - point_y));
						hdc_base_line(x1, by, x2, by);
					}
					else
					{
						x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						x2 = int(point_x + slopeO_begin * (by - point_y));
						hdc_base_line(x1, by, x2, by);
					}
				}
			}
			else if (real_angle_end % 360 > 180 && real_angle_end % 360 <= 270)//起点在左下区，终点在右上区
			{
				for (int by = point_y - radius; by <= y_begin; by++)
				{
					if (by <= y_end)
					{
						x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						x2 = int(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
					}
					else if (by <= point_y)
					{
						x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						x2 = int(point_x + slopeO_end * (by - point_y));
					}
					else
					{
						x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						x2 = int(point_x + slopeO_begin * (by - point_y));
					}
					hdc_base_line(x1, by, x2, by);
				}
			}
			else if (real_angle_end % 360 > 270 && real_angle_end % 360 < 360)//起点在左下区，终点在右下区
			{
				if (y_begin >= y_end)//起点比终点低
				{
					for (int by = point_y - radius; by <= y_begin; by++)
					{
						if (by <= point_y)
						{
							x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							x2 = int(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							hdc_base_line(x1, by, x2, by);
						}
						else if (by <= y_end)
						{
							x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							x2 = int(point_x + slopeO_begin * (by - point_y));
							int x3= int(point_x + slopeO_end * (by - point_y));
							int x4= int(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							hdc_base_line(x1, by, x2, by);
							hdc_base_line(x3, by, x4, by);
						}
						else
						{
							x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							x2 = int(point_x + slopeO_begin * (by - point_y));
							hdc_base_line(x1, by, x2, by);
						}
					}
				}
				else//起点比终点高
				{
					for (int by = point_y - radius; by <= y_end; by++)
					{
						if (by <= point_y)
						{
							x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							hdc_base_line(x1, by, x2, by);
						}
						else if (by <= y_begin)
						{
							x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							x2 = int(point_x + slopeO_begin * (by - point_y));
							int x3 = int(point_x + slopeO_end * (by - point_y));
							int x4 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							hdc_base_line(x1, by, x2, by);
							hdc_base_line(x3, by, x4, by);
						}
						else
						{
							x1= int(point_x + slopeO_end * (by - point_y));
							x2= hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							hdc_base_line(x1, by, x2, by);
						}
					}
				}
			}
			else if (real_angle_end % 360 >=0  && real_angle_end % 360 < 90 && deltaangle >= 270)//起点终点都在左下且差了270度以上
			{
				for (int by = point_y - radius; by <= point_y + radius; by++)
				{
					if (by <= point_y || by > y_end)
					{
						x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						hdc_base_line(x1, by, x2, by);
					}
					else if (by <= y_begin)
					{
						x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						x2 = int(point_x + slopeO_begin * (by - point_y));
						int x3 = int(point_x + slopeO_end * (by - point_y));
						int x4 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						hdc_base_line(x1, by, x2, by);
						hdc_base_line(x3, by, x4, by);
					}
					else if (by <= y_end)
					{
						x1 = int(point_x + slopeO_end * (by - point_y));
						x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						hdc_base_line(x1, by, x2, by);
					}
				}
			}
		}
		else if (real_angle_begin % 360 >= 90 && real_angle_begin % 360 < 180)//起始点在左上区
		{
			if (real_angle_end % 360 > 90 && real_angle_end % 360 <= 180 && deltaangle <= 90)//起点终点都在左上
			{
				for (int by = y_end; by <= point_y; by++)
				{
					if (by <= y_begin)
					{
						x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						x2 = int(point_x + slopeO_end * (by - point_y));
						hdc_base_line(x1, by, x2, by);
					}
					else
					{
						x1 = int(point_x + slopeO_begin * (by - point_y));
						x2 = int(point_x + slopeO_end * (by - point_y));
						hdc_base_line(x1, by, x2, by);
					}
				}
			}
			else if (real_angle_end % 360 > 180 && real_angle_end % 360 <= 270)//起点左上，终点右上
			{
				if (y_end <= y_begin)//end比较高
				{
					for (int by = point_y - radius; by <= point_y; by++)
					{
						if (by <= y_end)
						{
							x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							hdc_base_line(x1, by, x2, by);
						}
						else if (by <= y_begin)
						{
							x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							x2 = int(point_x + slopeO_end * (by - point_y));
							hdc_base_line(x1, by, x2, by);
						}
						else
						{
							x1 = int(point_x + slopeO_begin * (by - point_y));
							x2 = int(point_x + slopeO_end * (by - point_y));
							hdc_base_line(x1, by, x2, by);
						}
					}
				}
				else//begin比较高
				{
					for (int by = point_y - radius; by <= point_y; by++)
					{
						if (by <= y_begin)
						{
							x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							hdc_base_line(x1, by, x2, by);
						}
						else if (by <= y_end)
						{
							x1 = int(point_x + slopeO_begin * (by - point_y));
							x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							hdc_base_line(x1, by, x2, by);
						}
						else
						{
							x1 = int(point_x + slopeO_begin * (by - point_y));
							x2 = int(point_x + slopeO_end * (by - point_y));
							hdc_base_line(x1, by, x2, by);
						}
					}
				}
			}
			else if (real_angle_end % 360 > 270 && real_angle_end % 360 < 360)//起点左上，终点右下
			{
				for (int by = point_y - radius; by <= y_end; by++)
				{
					if (by <= y_begin)
					{
						x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						hdc_base_line(x1, by, x2, by);
					}
					else if (by <= point_y)
					{
						x1 = int(point_x + slopeO_begin * (by - point_y));
						x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						hdc_base_line(x1, by, x2, by);
					}
					else
					{
						x1 = int(point_x + slopeO_end * (by - point_y));
						x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						hdc_base_line(x1, by, x2, by);
					}
				}
			}
			else if (real_angle_end % 360 >= 0 && real_angle_end % 360 < 90)//起点左上，终点左下
			{
				for (int by = point_y - radius; by <= point_y + radius; by++)
				{
					if (by <= y_begin || by > y_end)
					{
						x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						hdc_base_line(x1, by, x2, by);
					}
					else if (by <= point_y)
					{
						x1 = int(point_x + slopeO_begin * (by - point_y));
						x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						hdc_base_line(x1, by, x2, by);
					}
					else if (by <= y_end)
					{
						x1 = int(point_x + slopeO_end * (by - point_y));
						x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						hdc_base_line(x1, by, x2, by);
					}
				}
			}
			else if (real_angle_end % 360 >= 90 && real_angle_end % 360 < 180 && deltaangle >= 270)//起点左上，终点左上相差超过270
			{
				for (int by = point_y - radius; by <= point_y + radius; by++)
				{
					if (by <= y_begin || by > point_y)
					{
						x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						hdc_base_line(x1, by, x2, by);
					}
					else if (by <= y_end)
					{
						x1 = int(point_x + slopeO_begin * (by - point_y));
						x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						hdc_base_line(x1, by, x2, by);
					}
					else if (by <= point_y)
					{
						x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						x2 = int(point_x + slopeO_end * (by - point_y));
						int x3= int(point_x + slopeO_begin * (by - point_y));
						int x4= hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						hdc_base_line(x1, by, x2, by);
						hdc_base_line(x3, by, x4, by);
					}
				}
			}
		}
		else if (real_angle_begin % 360 >= 180 && real_angle_begin % 360 < 270)//起点在右上区
		{
			if (real_angle_end % 360 > 180 && real_angle_end % 360 <= 270 && deltaangle <= 90)//起点终点都在右上且相差不到90度
			{
				for (int by = y_begin; by <= point_y; by++)
				{
					if (by <= y_end)
					{
						x1 = int(point_x + slopeO_begin * (by - point_y));
						x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						hdc_base_line(x1, by, x2, by);
					}
					else
					{
						x1 = int(point_x + slopeO_begin * (by - point_y));
						x2 = int(point_x + slopeO_end * (by - point_y));
						hdc_base_line(x1, by, x2, by);
					}
				}
			}
			else if (real_angle_end % 360 > 270 && real_angle_end % 360 < 360)//起点在右上区，终点在右下
			{
				for (int by = y_begin; by <= y_end; by++)
				{
					if (by <= point_y)
					{
						x1 = int(point_x + slopeO_begin * (by - point_y));
						x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						hdc_base_line(x1, by, x2, by);
					}
					else
					{
						x1 = int(point_x + slopeO_end * (by - point_y));
						x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						hdc_base_line(x1, by, x2, by);
					}
				}
			}
			else if (real_angle_end % 360 >= 0 && real_angle_end % 360 <= 90)//起点在右上，终点在左下
			{
				for (int by = y_begin; by <= point_y + radius; by++)
				{
					if (by <= point_y)
					{
						x1 = int(point_x + slopeO_begin * (by - point_y));
						x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						hdc_base_line(x1, by, x2, by);
					}
					else if (by <= y_end)
					{
						x1 = int(point_x + slopeO_end * (by - point_y));
						x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						hdc_base_line(x1, by, x2, by);
					}
					else
					{
						x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						hdc_base_line(x1, by, x2, by);
					}
				}
			}
			else if (real_angle_end % 360 > 90 && real_angle_end % 360 <= 180)//起点右上，终点左上
			{
				if (y_end >= y_begin)//end比较低
				{
					for (int by = y_begin; by <= point_y + radius; by++)
					{
						if (by <= y_end)
						{
							x1 = int(point_x + slopeO_begin * (by - point_y));
							x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							hdc_base_line(x1, by, x2, by);
						}
						else if (by <= point_y)
						{
							x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							x2 = int(point_x + slopeO_end * (by - point_y));
							int x3 = int(point_x + slopeO_begin * (by - point_y));
							int x4 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							hdc_base_line(x1, by, x2, by);
							hdc_base_line(x3, by, x4, by);
						}
						else
						{
							x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							hdc_base_line(x1, by, x2, by);
						}
					}
				}
				else//begin比较低
				{
					for (int by = y_end; by <= point_y + radius; by++)
					{
						if (by <= y_begin)
						{
							x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							x2 = int(point_x + slopeO_end * (by - point_y));
							hdc_base_line(x1, by, x2, by);
						}
						else if (by <= point_y)
						{
							x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							x2 = int(point_x + slopeO_end * (by - point_y));
							int x3 = int(point_x + slopeO_begin * (by - point_y));
							int x4 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							hdc_base_line(x1, by, x2, by);
							hdc_base_line(x3, by, x4, by);
						}
						else
						{
							x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							hdc_base_line(x1, by, x2, by);
						}
					}
				}
			}
			else if (real_angle_end % 360 >= 180 && real_angle_end % 360 <= 270 && deltaangle >= 270)//起点右上，终点右上且相差超过270
			{
				for (int by = point_y - radius; by <= point_y + radius; by++)
				{
					if (by <= y_end || by > point_y)
					{
						x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						hdc_base_line(x1, by, x2, by);
					}
					else if (by <= y_begin)
					{
						x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						x2 = int(point_x + slopeO_end * (by - point_y));
						hdc_base_line(x1, by, x2, by);
					}
					else if (by <= point_y)
					{
						x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						x2 = int(point_x + slopeO_end * (by - point_y));
						int x3 = int(point_x + slopeO_begin * (by - point_y));
						int x4 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						hdc_base_line(x1, by, x2, by);
						hdc_base_line(x3, by, x4, by);
					}
				}
			}
		}
		else if (real_angle_begin % 360 >= 270 && real_angle_begin % 360 < 360)//起点在右下区
		{
			if (real_angle_end % 360 > 270 && real_angle_end % 360 < 360 && deltaangle <= 90)//起点在右下，终点在右下且相差小于90
			{
				for (int by = point_y; by <= y_end; by++)
				{
					if (by < y_begin)
					{
						x1 = int(point_x + slopeO_end * (by - point_y));
						x2 = int(point_x + slopeO_begin * (by - point_y));
						hdc_base_line(x1, by, x2, by);
					}
					else
					{
						x1 = int(point_x + slopeO_end * (by - point_y));
						x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						hdc_base_line(x1, by, x2, by);
					}
				}
			}
			else if (real_angle_end % 360 >= 0 && real_angle_end % 360 <= 90)//起点在右下，终点在左下
			{
				if (y_end >= y_begin)//如果end在下面
				{
					for (int by = point_y; by <= point_y + radius; by++)
					{
						if (by < y_begin)
						{
							x1 = int(point_x + slopeO_end * (by - point_y));
							x2 = int(point_x + slopeO_begin * (by - point_y));
							hdc_base_line(x1, by, x2, by);
						}
						else if (by < y_end)
						{
							x1 = int(point_x + slopeO_end * (by - point_y));
							x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							hdc_base_line(x1, by, x2, by);
						}
						else
						{
							x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							hdc_base_line(x1, by, x2, by);
						}
					}
				}
				else//如果begin在下面
				{
					for (int by = point_y; by <= point_y + radius; by++)
					{
						if (by <= y_end)
						{
							x1 = int(point_x + slopeO_end * (by - point_y));
							x2 = int(point_x + slopeO_begin * (by - point_y));
							hdc_base_line(x1, by, x2, by);
						}
						else if (by <= y_begin)
						{
							x1 = hdc_inner_round(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							x2 = int(point_x + slopeO_begin * (by - point_y));
							hdc_base_line(x1, by, x2, by);
						}
						else
						{
							x1 = hdc_inner_round(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
							hdc_base_line(x1, by, x2, by);
						}
					}
				}
			}
			else if (real_angle_end % 360 > 90 && real_angle_end % 360 <= 180)//起点在右下，终点在左上
			{
				for (int by = y_end; by <= point_y + radius; by++)
				{
					if (by < point_y)
					{
						x1= hdc_inner_round(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						x2= int(point_x + slopeO_end * (by - point_y));
						hdc_base_line(x1, by, x2, by);
					}
					else if (by < y_begin)
					{
						x1 = hdc_inner_round(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						x2 = int(point_x + slopeO_begin * (by - point_y));
						hdc_base_line(x1, by, x2, by);
					}
					else
					{
						x1 = hdc_inner_round(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						hdc_base_line(x1, by, x2, by);
					}
				}
			}
			else if (real_angle_end % 360 > 180 && real_angle_end % 360 <= 270)//起点在右下，终点在右上
			{
				for (int by = point_y - radius; by <= point_y + radius; by++)
				{
					if (by <= y_end || by >= y_begin)
					{
						x1 = hdc_inner_round(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						x2 = hdc_inner_round(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						hdc_base_line(x1, by, x2, by);
					}
					else if (by < point_y)
					{
						x1 = hdc_inner_round(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						x2 = int(point_x + slopeO_end * (by - point_y));
						hdc_base_line(x1, by, x2, by);
					}
					else if (by < y_begin)
					{
						x1 = hdc_inner_round(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						x2 = int(point_x + slopeO_begin * (by - point_y));
						hdc_base_line(x1, by, x2, by);
					}
				}
			}
			else if (real_angle_end % 360 >= 270 && real_angle_end % 360 < 360 && deltaangle >= 270)//起点在右下，终点在右下且相差超过270
			{
				for (int by = point_y - radius; by <= point_y + radius; by++)
				{
					if (by <= point_y || by >= y_begin)
					{
						x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						x2 = int(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						hdc_base_line(x1, by, x2, by);
					}
					else if (by < y_end)
					{
						x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						x2 = int(point_x + slopeO_begin * (by - point_y));
						int x3 = int(point_x + slopeO_end * (by - point_y));
						int x4 = int(point_x + sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						hdc_base_line(x1, by, x2, by);
						hdc_base_line(x3, by, x4, by);
					}
					else if (by < y_begin)
					{
						x1 = int(point_x - sqrt(pow(radius, 2) - pow(by - point_y, 2)));
						x2 = int(point_x + slopeO_begin * (by - point_y));
						hdc_base_line(x1, by, x2, by);
					}
				}
			}
		}
	}
}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一个圆
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const bool filled		：是否需要填充（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_circle(const int point_x, const int point_y, const int radius, const bool filled, const int thickness, const int RGB_value)
{
	hdc_sector(point_x, point_y, radius, 0, 360, filled, thickness, RGB_value);
}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一个椭圆
  输入参数：const int point_x			：圆心的x
			const int point_y			：圆心的y
			const int radius_a			：平行于X轴的半径
			const int radius_b			：平行于Y轴的半径
			const int rotation_angles	：以圆心为支点，与x轴的旋转倾角（向下转为正）（有缺省值）
			const bool filled			：是否需要填充（有缺省值）
			const int thickness			：粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_ellipse(const int point_x, const int point_y, const int radius_a, const int radius_b, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	int x1, x2, y1, y2;
	int old_x1 = INT_MAX;
	int old_x2 = INT_MAX;
	int old_y1 = INT_MAX;
	int old_y2 = INT_MAX;
	int count = 0;
	for (double by = point_y - radius_b; by <= point_y + radius_b; by += 1)
	{
		x1 = hdc_inner_round(point_x - sqrt(pow(radius_a, 2) * (1 - pow(by - point_y, 2) / pow(radius_b, 2))));
		x2 = hdc_inner_round(point_x + sqrt(pow(radius_a, 2) * (1 - pow(by - point_y, 2) / pow(radius_b, 2))));
		y1 = hdc_inner_round(by);
		y2 = hdc_inner_round(by);
		hdc_inner_rotate(point_x, point_y, x1, y1, rotation_angles);
		hdc_inner_rotate(point_x, point_y, x2, y2, rotation_angles);
		if (x1 != old_x1 || y1 != old_y1)
		{
			if (count == 0)
				hdc_point(x1, y1, thickness, RGB_value);
			else
				hdc_line(old_x1, old_y1, x1, y1, thickness, RGB_value);
			old_x1 = x1;
			old_y1 = y1;
		}
		if (x2 != old_x2 || y2 != old_y2)
		{
			if (count == 0)
				hdc_point(x2, y2, thickness, RGB_value);
			else
				hdc_line(old_x2, old_y2, x2, y2, thickness, RGB_value);
			old_x2 = x2;
			old_y2 = y2;
		}
		if (filled)
			hdc_line(x1, y1, x2, y2, 4, RGB_value);
		count++;
	}
}

#endif !HDC_SERIES_BY_TEACHER
