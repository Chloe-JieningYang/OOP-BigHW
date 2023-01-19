/* ͨ�Ź��� 2050598 ����� */
#include <iostream>
#include "..\include\cmd_console_tools.h"
#include "..\include\common-menu.h"
#include "90-01-b1-hanoi.h"
#include <iomanip>
#include <conio.h>
#include <windows.h>
using namespace std;
void stop(char choice);
/* ----------------------------------------------------------------------------------

     ���ļ����ܣ�
	1����ű� hanoi_main.cpp �и��ݲ˵�����ֵ���õĸ��˵����Ӧ��ִ�к���

     ���ļ�Ҫ��
	1�����������ⲿȫ�ֱ�����const��#define�������Ʒ�Χ�ڣ�
	2�������徲̬ȫ�ֱ�����������Ҫ��������Ҫ�����ĵ���ʾ��ȫ�ֱ�����ʹ��׼���ǣ����á����á��ܲ��þ������ã�
	3����̬�ֲ����������������ƣ���ʹ��׼��Ҳ�ǣ����á����á��ܲ��þ�������
	4���������ϵͳͷ�ļ����Զ���ͷ�ļ��������ռ��

   ----------------------------------------------------------------------------------- */

static int top_a = 0;
static int top_b = 0;
static int top_c = 0;
static int a[10];
static int b[10];
static int c[10];
static int step = 1;
static int time_ = 1;

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/

void input(int* num, char* i, char* a, char* m, char choice)
{
    int n;
    char init, aim;
    cout << endl;
    while (1){   
        cout << "�����뺺ŵ���Ĳ���(1-10)��" << endl;
        cin >> n;
        if (cin.good() == 1 && n >= 1 && n <= 10)
            break;
        else{        
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
    cin.clear();
    cin.ignore(10000, '\n');
    while (1){    
        cout << "��������ʼ��(A-C)��" << endl;
        cin >> init;
        if (cin.good() == 1 && (init >= 'A' && init <= 'C' || init >= 'a' && init <= 'c'))
            break;
        else{        
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
    cin.clear();
    cin.ignore(10000, '\n');
    if (init == 'a' || init == 'b' || init == 'c')
        init -= 32;
    while (1){   
        cout << "������Ŀ����(A-C)��" << endl;
        cin >> aim;
        if (cin.good() == 1 && (aim >= 'A' && aim <= 'C' || aim >= 'a' && aim <= 'c')){        
            if (aim == 'a' || aim == 'b' || aim == 'c')
                aim -= 32;
            if (init != aim)
                break;
            else{            
                cout << "Ŀ����(" << aim << ")��������ʼ��(" << init << ")��ͬ" << endl;
                cin.clear();
                cin.ignore(10000, '\n');
            }
        }
        else{        
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
    cin.clear();
    cin.ignore(10000, '\n');
    if (choice == '4' || choice == '8'){   
        while (1){        
            cout << "�������ƶ��ٶ�(0-5: 0-���س�������ʾ 1-��ʱ� 5-��ʱ���)" << endl;
            cin >> time_;
            if (cin.good() && time_ >= 0 && time_ <= 5)
                break;
        }
        cin.clear();
        cin.ignore(10000, '\n');
    }
    *num = n;
    *i = init;
    *a = aim;
    *m = 'A' + 'B' + 'C' - init - aim;
}

void init_store(int n, char init)
{
    if (init == 'A')
    {
        int i;
        for (i = 0; i <= n - 1; i++)
        {
            a[i] = n - i;
            top_a++;
        }
    }
    else if (init == 'B')
    {
        int i;
        for (i = 0; i <= n - 1; i++)
        {
            b[i] = n - i;
            top_b++;
        }
    }
    else
    {
        int i;
        for (i = 0; i <= n - 1; i++)
        {
            c[i] = n - i;
            top_c++;
        }
    }
}

void initialize_ver(int n, char init, char aim,char choice)
{
    int dif = 0;
    if (choice == '8' || choice == '9')
        dif = 15;
    cct_gotoxy(0, 12 + dif);
    cout << setfill(' ') << setw(9) << " " << setfill('=') << setw(25) << "=" << endl;
    cout << setfill(' ') << setw(12) << "A" << setw(10) << "B" << setw(10) << "C" << endl;
    if (init == 'A'){  
        int a1;
        for (a1 = 0; a1 <= 9; a1++){
            if (a[a1] != 0){
                cct_gotoxy(11, 11 + dif - a1);
                cout << a[a1];
            }
        }
    }
    else if (init == 'B'){
        int b1;
        for (b1 = 0; b1 <= 9; b1++) {       
            if (b[b1] != 0){           
                cct_gotoxy(21, 11 + dif - b1);
                cout << b[b1];
            }
        }
    }
    else{    
        int c1;
        for (c1 = 0; c1 <= 9; c1++) {
            if (c[c1] != 0){           
                cct_gotoxy(31, 11 + dif - c1);
                cout << c[c1];
            }
        }
    }
}

void initialize_ho(char choice)
{
    int dif = 0;
    if (choice == '8' || choice == '9')
        dif = 15;
    cct_gotoxy(0, 17 + dif);
    cout << "��ʼ:" << setfill(' ') << setw(15) << " " << " A:";
    int a2, b2, c2;
    for (a2 = 0; a2 <= 9; a2++)
    {
        if (a[a2] != 0)
        {
            cout << setw(2) << a[a2];
        }
    }
    if (top_a != 10)
        cout << setfill(' ') << setw(20 - 2 * top_a) << " ";
    cout << " B:";
    for (b2 = 0; b2 <= 9; b2++)
    {
        if (b[b2] != 0)
        {
            cout << setw(2) << b[b2];
        }
    }
    if (top_b != 10)
        cout << setfill(' ') << setw(20 - 2 * top_b) << " ";
    cout << " C:";
    for (c2 = 0; c2 <= 9; c2++)
    {
        if (c[c2] != 0)
        {
            cout << setw(2) << c[c2];
        }
    }
    if (top_c != 10)
        cout << setfill(' ') << setw(20 - 2 * top_c) << " ";
}


void initialize(int n, char src, char dst, char choice)//��ʼ������
{
    if (choice == '4')
    {
        cout << "�� " << src << " �ƶ��� " << dst << "���� " << n << " �㣬��ʱ����Ϊ " << time_;
        initialize_ver(n, src, dst,choice);
        initialize_ho('4');
    }
    else if (choice == '8')
    {
        cout << "�� " << src << " �ƶ��� " << dst << "���� " << n << " �㣬��ʱ����Ϊ " << time_;
        initialize_ver(n, src, dst, choice);
        initialize_ho(choice);
        draw_six(n, src);
    }
    else if (choice == '9')
    {
        cout << "�� " << src << " �ƶ��� " << dst << "���� " << n << " ��";
        initialize_ver(n, src, dst, choice);
        initialize_ho(choice);
        draw_six(n, src);
    }
}

void store(char src,char dst)//�����������еĸı�
{
	int cunfang;
    if (src == 'A')
    {
        cunfang = a[--top_a];
        a[top_a] = 0;
    }
    else if (src == 'B')
    {
        cunfang = b[--top_b];
        b[top_b] = 0;
    }
    else
    {
        cunfang = c[--top_c];
        c[top_c] = 0;
    }
    if (dst == 'A')
        a[top_a++] = cunfang;
    else if (dst == 'B')
        b[top_b++] = cunfang;
    else
        c[top_c++] = cunfang;
}

void move_one(int n, char src, char tmp, char dst)//��һ��
{
    cout << n << "# " << src << "---->" << dst << endl;
}

void move_two(int n, char src, char tmp, char dst)//�ڶ���
{
    cout << "��" << setfill(' ') << setw(4) << step << " ��(" << setw(2) << n
        << "#: " << src << "-->" << dst << ")" << endl;
}

void move_horizontal(int n, char src, char tmp, char dst, char choice)//�������
{
    int a1, b1, c1;
    if (choice == '4')
        cct_gotoxy(0, 17);
    else if (choice == '8'||choice=='9')
        cct_gotoxy(0, 32);   
    cout << "��" << setfill(' ') << setw(4) << step << " ��("
        << n << "#:" << src << "-->" << dst << ")  A:";
    for (a1 = 0; a1 <= 9; a1++)
    {
        if (a[a1] != 0)
            cout << setw(2) << a[a1];
    }
    if (top_a != 10)
        cout << setfill(' ') << setw(20 - 2 * top_a) << " ";
    cout << " B:";
    for (b1 = 0; b1 <= 9; b1++)
    {
        if (b[b1] != 0)
            cout << setw(2) << b[b1];
    }
    if (top_b != 10)
        cout << setfill(' ') << setw(20 - 2 * top_b) << " ";
    cout << " C:";
    for (c1 = 0; c1 <= 9; c1++)
    {
        if (c[c1] != 0)
            cout << setw(2) << c[c1];
    }
    if (top_c != 10)
        cout << setfill(' ') << setw(20 - 2 * top_c) << " ";
    if (choice == '3')
        cout << endl;
}

void move_vertical(int n, char src, char tmp, char dst, char choice)//�������
{
    int src_x = (int(src) - 65) * 10 + 11;
    int tmp_x = (int(tmp) - 65) * 10 + 11;
    int dst_x = (int(dst) - 65) * 10 + 11;
    int dif = 0;
    if (choice == '4')
        dif = 0;
    else if (choice == '8'||choice=='9')
        dif = 15;
    switch (src)
    {
        case 'A':
            cct_showch(src_x, 12 + dif - (top_a + 1),' ');          
            break;
        case 'B':
            cct_showch(src_x, 12 + dif - (top_b + 1),' ');
            break;
        case 'C':
            cct_showch(src_x, 12 + dif - (top_c + 1),' ');
            break;
    }
    switch (dst)
    {
        case 'A':
            cct_gotoxy(dst_x, 12 + dif - (top_a - 1 + 1));
            cout << n;
            break;
        case 'B':
            cct_gotoxy(dst_x, 12 + dif - (top_b - 1 + 1));
            cout << n;
            break;
        case 'C':
            cct_gotoxy(dst_x, 12 + dif - (top_c - 1 + 1));
            cout << n;
            break;
    }
}

void move_three(int n, char src, char tmp, char dst)//������
{
    move_horizontal(n, src, tmp, dst, '3');
}

void move_four(int n, char src, char tmp, char dst)//������
{
    move_horizontal(n, src, tmp, dst, '4');
    move_vertical(n, src, tmp, dst, '4');
}

void draw_one_pole(const int x,const int y)//xΪ��һ�����ӵ����������� ������
{
    const int bg_color = COLOR_HYELLOW;
    const int fg_color = COLOR_WHITE;
    const char ch = ' ';
    const int length = 23;
    cct_showch(x, y, ch, bg_color, fg_color, length);
}

void draw_one_pole_(const int x,const int y)//�������ϵĸ���
{
    const int bg_color = COLOR_HYELLOW;
    const int fg_color = COLOR_WHITE;
    const char ch = ' ';
    int i;
    for (i = 1; i <= 12; i++)
    {
        cct_showch(x + 11, y - i, ch, bg_color, fg_color);
        Sleep(100);
        cct_showch(x + 43, y - i, ch, bg_color, fg_color);
        Sleep(100);
        cct_showch(x + 75, y - i, ch, bg_color, fg_color);
        Sleep(100);
    }
}

void draw_three_pole()//�����������ܺ���
{
    cct_setcursor(CURSOR_INVISIBLE);
    draw_one_pole(x_1, 15);
    draw_one_pole(x_2, 15);
    draw_one_pole(x_3, 15);
    draw_one_pole_(x_1, 15);
}

void draw_six(int n, char src)//������ѡ�� ���ӳ�ʼ��
{
    draw_three_pole();
    int x = 0;
    switch (src)
    {
        case 'A':
            x = x_1;
            break;
        case 'B':
            x = x_2;
            break;
        case 'C':
            x = x_3;
            break;
    }
    int i;
    int y = 14;
    const char ch = ' ';
    for (i = n; i >=1 ; i--)
    {
        cct_showch(x + 11 - i, y, ch, i + 1, 7, 21 - (10 - i) * 2);
        y--;
        Sleep(100);
    }
}

void move_plate(int n_src,int n_dst,int bianhao,char src,char dst,char choice)//���ӵ��ƶ�
{
    int src_x = (int(src) - 65) * 32 + 12;//����ָ���ӵ�x����
    int dst_x = (int(dst) - 65) * 32 + 12;
    int x, y;
    int color = bianhao + 1;
    if (choice == '7' || choice == '8'|| choice=='9') {
        for (y = 15 - n_src; y >= 1; y--){
            cct_showch(src_x - bianhao, y, ' ', color, 7, 2 * bianhao + 1);
            if(choice!='9')
                Sleep(200 / (time_+1));
            if (y >= 3){
                cct_showch(src_x - bianhao, y, ' ', 0, 7, bianhao);
                cct_showch(src_x, y, ' ', COLOR_HYELLOW, 7);
                cct_showch(src_x + 1, y, ' ', 0, 7, bianhao);
            }
            else
                cct_showch(src_x - bianhao, y, ' ', 0, 7, 2 * bianhao + 1);
        }
        if (src_x < dst_x){
            for (x = src_x - bianhao; x <= dst_x - bianhao; x++){
                cct_showch(x, 1, ' ', color, 7, 2 * bianhao + 1);
                if (choice != '9')
                    Sleep(200 / (time_ + 1));
                cct_showch(x, 1, ' ', 0, 7, 2 * bianhao + 1);
            }
        }
        else {
            for (x = src_x - bianhao; x >= dst_x - bianhao; x--){
                cct_showch(x, 1, ' ', color, 7, 2 * bianhao + 1);
                if (choice != '9')
                    Sleep(200 / (time_ + 1));
                cct_showch(x, 1, ' ', 0, 7, 2 * bianhao + 1);
            }
        }
        for (y = 1; y <= 14 - n_dst; y++){
            cct_showch(dst_x - bianhao, y, ' ', color, 7, 2 * bianhao + 1);
            if (choice != '9')
                Sleep(200 / (time_ + 1));
            if (y < 14 - n_dst){
                if (y >= 3){
                    cct_showch(dst_x - bianhao, y, ' ', 0, 7, bianhao);
                    cct_showch(dst_x, y, ' ', COLOR_HYELLOW, 7);
                    cct_showch(dst_x + 1, y, ' ', 0, 7, bianhao);
                }
                else
                    cct_showch(dst_x - bianhao, y, ' ', 0, 7, 2 * bianhao + 1);
            }
        }
    }
}

void draw_seven(int n, char src, char dst)
{
    draw_six(n, src);
    move_plate(n, 0, 1, src, dst,'7');
}

void stop(char choice)//�����ʱ��
{
    if (choice == '4' || choice == '8')
    {  
        if (time_ >= 1 && time_ <= 4)
            Sleep(500 / time_);
    }
}

void move_(int n,char src, char tmp, char dst,char choice)//�����ƶ��ĺ���
{
    int n_src, n_dst;
    if (src == 'A')
        n_src = top_a + 1;
    else if (src == 'B')
        n_src = top_b + 1;
    else
        n_src = top_c + 1;
    if (dst == 'A')
        n_dst = top_a - 1;
    else if (dst == 'B')
        n_dst = top_b - 1;
    else
        n_dst = top_c - 1;
    switch (choice)
    {
        case '1':
            move_one(n, src, tmp, dst);
            break;
        case '2':
            move_two(n, src, tmp, dst);
            break;
        case '3':
            move_three(n, src, tmp, dst);
            break;
        case '4':
            char ch;
            if (time_ == 0)
                while(ch=_getch()!='\r');
            move_four(n, src, tmp, dst);
            break;
        case '8':
        case '9':
            char ch2;
            if (time_ == 0)
                while (ch2 = _getch() != '\r');
            move_vertical(n, src, tmp, dst, choice);
            move_horizontal(n, src, tmp, dst, choice);
            move_plate(n_src, n_dst, n, src, dst, choice);         
            break;
    }
    step++;
    stop(choice);
}

void end_(char choice)
{
    if (choice == '1' || choice == '2' || choice == '3')
    {
        cout << endl;
        cout << "���س�������";
    }
    else
        cct_showstr(0, 38, "���س�������", 0, 7, 1);
    step = 1;
    top_a = 0;
    top_b = 0;
    top_c = 0;
    int i;
    for (i = 0; i <= 9; i++)
    {
        a[i] = 0;
        b[i] = 0;
        c[i] = 0;
    }
    time_ = 1;
}

void hanoi(int n, char src, char tmp, char dst,char choice)
{
    if (n == 1){
        store(src, dst);
        move_(n, src, tmp, dst, choice);
    }
    else {
        hanoi(n - 1, src, dst, tmp,choice);
        store(src, dst);
        move_(n, src, tmp, dst, choice);
        hanoi(n - 1, tmp, src, dst,choice);
    }
}

bool judge_end(char dst, int n)
{
    int end;
    if (dst == 'A')
        end = top_a;
    else if (dst == 'B')
        end = top_b;
    else
        end = top_c;
    if (end == n)
        return true;
    else
        return false;
}



void nine(int nn,char aim)//һ����nn������
{
    cct_showstr(0, 34, "�������ƶ�������(������ʽ��AC=A���˵������ƶ���C��Q=�˳�) ");
    while (1) {  //���ڶ���ƶ�
        char se[20];
        int p = 0;
        int q = 0;
        cct_showstr(58, 34, "��");
        while (1) {   //���ڼ��һ������
            int n = 0;
            int i;
            for (i = 0; i <= 19; i++)
                se[i] = '\0';
            cct_gotoxy(60, 34);
            cct_setcursor(CURSOR_VISIBLE_NORMAL);
            while (1) {
                char ch=_getch();
                if (ch == 13)
                    break;
                se[n] = ch;
                if (se[n] >= 48 && se[n] <= 122)
                    cout << se[n];                              
                if (n > 19)
                    break;
                if (se[n] == 'a' || se[n] == 'b' || se[n] == 'c')
                    se[n] -= 32;
                n++;              
            }
            if (n==2&&se[0] >= 'A' && se[0] <= 'C' && se[1] >= 'A' && se[1] <= 'C' && se[0] != se[1]) {
                int sum_src = 0;
                switch (se[0]) {
                    case 'A':
                        p = a[top_a - 1];
                        sum_src = top_a;
                        break;
                    case 'B':
                        p = b[top_b - 1];
                        sum_src = top_b;
                        break;
                    case 'C':
                        p = c[top_c - 1];
                        sum_src = top_c;
                        break;
                }
                int sum_dst = 0;
                if (se[1] == 'A')
                    sum_dst = top_a;
                else if (se[1] == 'B')
                    sum_dst = top_b;
                else
                    sum_dst = top_c;
                if (sum_dst != 0) {
                    switch (se[1]) {
                        case 'A':
                            q = a[top_a - 1];
                            break;
                        case 'B':
                            q = b[top_b - 1];
                            break;
                        case 'C':
                            q = c[top_c - 1];
                            break;
                    }
                }
                if ((p < q || sum_dst == 0) && sum_src != 0)
                    break;
                else if (sum_src == 0)
                {
                    cct_showstr(0, 35, "Դ��Ϊ��!");
                    Sleep(300);
                    cct_showch(0, 35, ' ', 0, 7, 21);
                }
                else if (p > q) {
                    cct_showstr(0, 35, "����ѹС�̣��Ƿ��ƶ�!");
                    Sleep(300);
                    cct_showch(0, 35, ' ', 0, 7, 21);
                }
                cct_showch(60, 34, ' ', 0, 7, 20);//�������ۼ�
            }
            if (se[0] == 'q' || se[0] == 'Q') {
                cct_showstr(0, 35, "��Ϸ��ֹ!!!!!");
                return;
            }
            cct_showch(60, 34, ' ', 0, 7, 20);//�������ۼ�
        }
        cct_showch(60, 34, ' ', 0, 7, 20);//�������ۼ�
        char src = se[0];
        char dst = se[1];
        char tmp = 'A' + 'B' + 'C' - src - dst;
        store(src, dst);
        move_(p, src, tmp, dst, '9');
        if (judge_end(aim, nn)) {
            cct_showstr(0, 35, "��Ϸ����");
            break;
        }
    }
}