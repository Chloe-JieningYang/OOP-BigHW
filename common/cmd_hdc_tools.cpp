/* 2050598 ����� ͨ�� */

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

#if !HDC_SERIES_BY_TEACHER	//δ���������²Ŵ���������

/* ���������Ҫ��ͷ�ļ� */

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow(); //VS�д˴����в����ߣ���Ӱ������ʹ��

/* �Ѷ���ľ�̬ȫ�ֱ��� */
static HWND hWnd = GetConsoleWindow();
static HDC hdc = NULL;
static const double PI = 3.14159;
static int _BgColor_, _FgColor_, _Width_, _High;
static int _Delay_ms;

/* �����ʶ�ʹ�þ�̬ȫ�ֱ�����������static�����ⲿ���ɼ� */

/* �˴���������Զ��庯����������static�����ⲿ���ɼ� */
/***************************************************************************
  �������ƣ�
  ��    �ܣ����Զ����0��ת��Ϊϵͳ��0��
  ������������յ����ӽǶȵı���������ϵ���ı�����0��λ��(V_UP/V_DOWN/H_RIGHT/HLEFT)��˳ʱ��/��ʱ��(CLOCKWISE/ANTICLOCKWISE)
  �� �� ֵ��
  ˵    ����
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
  �������ƣ�
  ��    �ܣ���������
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
static int hdc_inner_round(double num)
{
	return (num > 0.0) ? int(floor(num + 0.5)) : int(ceil(num - 0.5));
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ĳ��˳ʱ����תһ����
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
static void hdc_inner_rotate(const int point_x, const int point_y, int& x, int& y, const int angle)
{
	int x0 = hdc_inner_round((x - point_x) * cos(angle * PI / 180) - (y - point_y) * sin(angle * PI / 180) + point_x);
	int y0 = hdc_inner_round((x - point_x) * sin(angle * PI / 180) + (y - point_y) * cos(angle * PI / 180) + point_y);
	x = x0;
	y = y0;
}

/* ��������˼�����������������ʵ�֣���Ҫ�Ķ� */
/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʼ��
  ���������const int bgcolor������ɫ
            const int fgcolor��ǰ��ɫ
			const int width  ����Ļ��ȣ�����
			const int high   ����Ļ�߶ȣ�����
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_init(const int bgcolor, const int fgcolor, const int width, const int high)
{
	/* ���ͷţ���ֹ��release���ٴ�init��hdc_release�����룩 */
	hdc_release();

	/* ����init����һ����̬ȫ������¼������hdc_cls()���õ� */
	_BgColor_ = bgcolor;
	_FgColor_ = fgcolor;
	_Width_ = width;
	_High = high;

	hdc = GetDC(hWnd);

	cct_setcursor(CURSOR_INVISIBLE);
	cct_setcolor(bgcolor, fgcolor);
	cct_setfontsize("��������", 16);
	cct_setconsoleborder(width / 8 + !!(width % 8), high / 16 + !!(high % 16)); //������Ŀ�ȼ��߶�ת��Ϊ�ض��������������!!�ĺ��壺�������8/16�ı��������ж�+1
	cct_cls();
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ͷŻ�ͼ��Դ
  ���������
  �� �� ֵ��
  ˵    ����������
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_delay(int ms)
{
	if (ms > 0)
		_Delay_ms = ms;
	else
		_Delay_ms = 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����û�����ɫ������RGBֵ
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_pencolor(const int RGB_value)
{
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB_value);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����û�����ɫ������RGB��ɫ��ֵ0-255
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_pencolor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	hdc_set_pencolor(RGB(red, green, blue));
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ļ�����е�ͼ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_cls()
{
    /* ����һ��Ҫ��һ����ɫ��ʼ������������ص㣬�ҵ�����㷽����ͬѧ����֪ͨ�� */
	hdc_init(_BgColor_, (_FgColor_  + 1) % 16, _Width_, _High);

	hdc_init(_BgColor_, _FgColor_, _Width_, _High);

	/* ���ֻ���������demoʱ�������hdc_cls()���һ������ȱ�ߵ����������ʱ����
	   ������ֻ������л������⣬���ߴ���ʱֵ */
	Sleep(30);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x,y)λ�ô���ָ����ɫ����һ�����ص�
  ���������const int x��x���꣬���Ͻ�Ϊ(0,0)
            const int y��y���꣬���Ͻ�Ϊ(0,0)
  �� �� ֵ��
  ˵    ������ɫֱ���õ�ǰ�趨�������ڵ��ô�ָ��
***************************************************************************/
static inline void hdc_base_point(const int x, const int y)
{
	MoveToEx(hdc, x - 1, y - 1, NULL);
	LineTo(hdc, x, y);
	if (_Delay_ms > 0)
		Sleep(_Delay_ms);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x1,y1)-(x2,y2)֮�仭��һ�����ص������
  ���������const int x1�����x���꣬���Ͻ�Ϊ(0,0)
			const int y1�����y���꣬���Ͻ�Ϊ(0,0)
			const int x2���յ�y���꣬���Ͻ�Ϊ(0,0)
			const int y2���յ�y���꣬���Ͻ�Ϊ(0,0)
  �� �� ֵ��
  ˵    ������ɫֱ���õ�ǰ�趨
***************************************************************************/
static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
	if (_Delay_ms > 0)
		Sleep(_Delay_ms);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x,y)λ�ô���ָ����ɫ��һ��ָ����ϸ�ĵ�(�û�ʵ��Բ��ģ��)
  ���������const int x			��x���꣬���Ͻ�Ϊ(0,0)
            const int y			��y���꣬���Ͻ�Ϊ(0,0)
			const int thickness	����Ĵ�ϸ������λ1�����޲���(�����ᵼ�²���ȫ���)����ȱʡֵ��
			const int RGB_value	�������ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    �����Ľ�����Ч��
***************************************************************************/
void hdc_point(const int x, const int y, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//��������
	/* ������ֱ�������ޣ����ǵ����޳���һ����С�ǣ�������Բ����λ��δʵ�� */

	const int tn_end = (tn <= 1) ? 1 : tn/2;

	int angle, level;
	int old_x1 = INT_MAX, old_y1 = INT_MAX, x1, y1;
	int count = 0;

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	/* �û�Բ�ķ�ʽģ��ֵ� */
	for (level = 1; level <= tn_end; level++) {
		for (angle = 0; angle <= 360; angle ++) {
			/* ע�⣬ϵͳ�������ᣬ0����Բ�����·� */
			x1 = x + (int)(level * sin(angle * PI / 180));
			y1 = y - (int)(level * cos(angle * PI / 180));

			/* ���뾶��Сʱ���Ƕȱ仯��������int�͵�x1/y1�仯����˼��ж���䣬�����ظ���ͬһ���ص� */
			if (x1 != old_x1 || y1 != old_y1) {
				old_x1 = x1;
				old_y1 = y1;
				hdc_base_point(x1, y1);
				++count;
			}
		}
	}
#if 0
	/* �ſ���ע�ͣ����Կ���һ���ֵ����˼������ص���� */
	cct_gotoxy(0, 41);
	printf("count=%d           ", count);
	getchar();
#endif
}

/* -------------------------------------------------------------------------
	�������к�����ʵ�֣���������������׼�Ķ�
	1������Ҫ����ϵͳ�������й�ͼ�β����ĺ���
	2�����к�����ʵ�ֶ�����hdc_base_point/hdc_base_line���������
	3����һ�룬�ǲ������к�������Ҫ����������ʵ�ֹ��̣�Ӧ����ʵ����Щ������
	4�������ʵ������ӵĲ���
	5��ϵͳ�ĽǶ��뺯�������еĽǶȺ������180��
   ------------------------------------------------------------------------- */

/***************************************************************************
  �������ƣ�
  ��    �ܣ����߶�
  ���������const int x1		������x
            const int y1		������y
			const int x2		���յ��x
			const int y2		���յ��y
			const int thickness	���߶εĴ�ϸ����ȱʡֵ��
			const int RGB_value	���߶ε���ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ���������������û���hdc_point����ϣ��ٶ�����
                  Ҳ������hdc_base_point/hdc_base_line����ϣ��ٶȿ죩
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
  �������ƣ�
  ��    �ܣ�������������꣬��һ��������
  ���������const int x1		����1�����x
            const int y1		����1�����y
			const int x2		����2�����x
			const int y2		����2�����y
			const int x3		����3�����x
			const int y3		����3�����y
			bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness	���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value	����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    �������ж������Ƿ��ߣ�������ߣ�����һ��ֱ�߼���
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
		for (int p = 0; p <= 1; p++)//��y��С����˳���������
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
		//��ߵ��±�Ϊ0����͵��±�Ϊ2
		if (y[0] != y[1] && y[1] != y[2])//����������㲻��ͬһˮƽ����
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
  �������ƣ�
  ��    �ܣ��������Ͻ����꼰��ȡ��߶ȣ�����һ��������
  ���������const int left_up_x			�����Ͻ�x
            const int left_up_y			�����Ͻ�y
			const int width				�����
			const int high				���߶�
			const int rotation_angles	�������Ͻ�Ϊ֧�㣬��x�����ת��ǣ�����תΪ����
			bool filled					���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
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
		for (int p = 0; p <= 2; p++)//��y��С����˳���������
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
		//��ߵ��±�Ϊ0����͵��±�Ϊ3
		if (y[0] != y[1])//����������Űڵĳ�����
		{
			double slope0_1 = double(x[0] - x[1]) / (y[0] - y[1]);
			double slope0_2 = double(x[0] - x[2]) / (y[0] - y[2]);
			double slope1_3 = double(x[1] - x[3]) / (y[1] - y[3]);
			double slope2_3 = double(x[2] - x[3]) / (y[2] - y[3]);
			int start_x, end_x;
			for (int by = y[0] + 1; by <= y[3] - 1; by++)//����������
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
		else//��������Űڵĳ�����
		{
			for (int by = y[0] + 1; by <= y[3] - 1; by++)
				hdc_base_line(x[0], by, x[1], by);
		}
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��������Ͻ����꼰�߳�������һ��������
  ���������const int left_up_x			�����Ͻ�x
			const int left_up_y			�����Ͻ�y
			const int length			���߳�
			const int rotation_angles	�������Ͻ�Ϊ֧�㣬��x�����ת��ǣ�����תΪ��������ȱʡֵ��
			const bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_square(const int left_up_x, const int left_up_y, const int length, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	hdc_rectangle(left_up_x, left_up_y, length, length, rotation_angles, filled, thickness, RGB_value);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ��Բ��
  ���������const int point_x		��Բ�ĵ�x
            const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const int angle_begin	����ʼ�Ƕȣ���Բ�����Ϸ�Ϊ0�㣬˳ʱ��Ϊ����ע�⣺��ϵͳ�������180��!!!������ȱʡֵ��
			const int angle_end		�������Ƕȣ�ͬ�ϣ�����ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
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
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ������
  ���������const int point_x		��Բ�ĵ�x
			const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const int angle_begin	����ʼ�Ƕȣ���Բ�����Ϸ�Ϊ0�㣬˳ʱ��Ϊ����ע�⣺��ϵͳ�������180��!!!������ȱʡֵ��
			const int angle_end		�������Ƕȣ�ͬ�ϣ�����ȱʡֵ��
			const bool filled		���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  ���������
  �� �� ֵ��
  ˵    ��������ʼ/�����ǶȲ�ֵΪ360�ı���ʱ������������
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
		if (real_angle_begin % 360 >= 0 && real_angle_begin % 360 < 90)//�����������
		{
			if (real_angle_end % 360 > 0 && real_angle_end % 360 <= 90 && deltaangle <= 90)//��������������յ������������ҽǶ�90����
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
			else if (real_angle_end % 360 > 90 && real_angle_end % 360 <= 180)//��������������յ���������
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
			else if (real_angle_end % 360 > 180 && real_angle_end % 360 <= 270)//��������������յ���������
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
			else if (real_angle_end % 360 > 270 && real_angle_end % 360 < 360)//��������������յ���������
			{
				if (y_begin >= y_end)//�����յ��
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
				else//�����յ��
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
			else if (real_angle_end % 360 >=0  && real_angle_end % 360 < 90 && deltaangle >= 270)//����յ㶼�������Ҳ���270������
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
		else if (real_angle_begin % 360 >= 90 && real_angle_begin % 360 < 180)//��ʼ����������
		{
			if (real_angle_end % 360 > 90 && real_angle_end % 360 <= 180 && deltaangle <= 90)//����յ㶼������
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
			else if (real_angle_end % 360 > 180 && real_angle_end % 360 <= 270)//������ϣ��յ�����
			{
				if (y_end <= y_begin)//end�Ƚϸ�
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
				else//begin�Ƚϸ�
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
			else if (real_angle_end % 360 > 270 && real_angle_end % 360 < 360)//������ϣ��յ�����
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
			else if (real_angle_end % 360 >= 0 && real_angle_end % 360 < 90)//������ϣ��յ�����
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
			else if (real_angle_end % 360 >= 90 && real_angle_end % 360 < 180 && deltaangle >= 270)//������ϣ��յ���������270
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
		else if (real_angle_begin % 360 >= 180 && real_angle_begin % 360 < 270)//�����������
		{
			if (real_angle_end % 360 > 180 && real_angle_end % 360 <= 270 && deltaangle <= 90)//����յ㶼������������90��
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
			else if (real_angle_end % 360 > 270 && real_angle_end % 360 < 360)//��������������յ�������
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
			else if (real_angle_end % 360 >= 0 && real_angle_end % 360 <= 90)//��������ϣ��յ�������
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
			else if (real_angle_end % 360 > 90 && real_angle_end % 360 <= 180)//������ϣ��յ�����
			{
				if (y_end >= y_begin)//end�Ƚϵ�
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
				else//begin�Ƚϵ�
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
			else if (real_angle_end % 360 >= 180 && real_angle_end % 360 <= 270 && deltaangle >= 270)//������ϣ��յ�����������270
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
		else if (real_angle_begin % 360 >= 270 && real_angle_begin % 360 < 360)//�����������
		{
			if (real_angle_end % 360 > 270 && real_angle_end % 360 < 360 && deltaangle <= 90)//��������£��յ������������С��90
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
			else if (real_angle_end % 360 >= 0 && real_angle_end % 360 <= 90)//��������£��յ�������
			{
				if (y_end >= y_begin)//���end������
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
				else//���begin������
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
			else if (real_angle_end % 360 > 90 && real_angle_end % 360 <= 180)//��������£��յ�������
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
			else if (real_angle_end % 360 > 180 && real_angle_end % 360 <= 270)//��������£��յ�������
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
			else if (real_angle_end % 360 >= 270 && real_angle_end % 360 < 360 && deltaangle >= 270)//��������£��յ�������������270
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
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ��Բ
  ���������const int point_x		��Բ�ĵ�x
			const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const bool filled		���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_circle(const int point_x, const int point_y, const int radius, const bool filled, const int thickness, const int RGB_value)
{
	hdc_sector(point_x, point_y, radius, 0, 360, filled, thickness, RGB_value);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ����Բ
  ���������const int point_x			��Բ�ĵ�x
			const int point_y			��Բ�ĵ�y
			const int radius_a			��ƽ����X��İ뾶
			const int radius_b			��ƽ����Y��İ뾶
			const int rotation_angles	����Բ��Ϊ֧�㣬��x�����ת��ǣ�����תΪ��������ȱʡֵ��
			const bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			����ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
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
