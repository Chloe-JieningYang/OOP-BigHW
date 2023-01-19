/* 2050598 杨洁宁 通信 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;

#include "../include/class_aat.h"
#include "hw_check.h"

/***************************************************************************
  函数名称：
  功    能：读取课号.dat文件
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int read_stulist(const string filename, ostringstream& msg, student stu[], int& stu_num)
{
	ifstream fin;
	int line_count = 1;//行数
	int stu_count = 0;//读入学生数 也是当前stu数组下标数
	fin.open(filename, ios::in | ios::binary);
	if (!fin.is_open())
	{
		return FAIL_OPEN;
	}
	while (!fin.eof())
	{
		char class_no[10] = { '\0' };
		char stu_no[10] = { '\0' };
		char info[500] = { '\0' };
		fin.getline(info, 400, '\n');

		if (strlen(info) == 0)
			break;

		if (info[0] == '#')//如果是注释行
		{
			msg << "\t--> 学生名单[" << filename << "]中的第[" << line_count << "]行为注释行，忽略" << endl;
			line_count++;
			continue;
		}

		istringstream in(info);
		in >> class_no >> stu_no >> stu[stu_count].name >> stu[stu_count].g_ok >> stu[stu_count].c_ok;
		stu[stu_count].class_no = atoi(class_no);
		stu[stu_count].no = atoi(stu_no);
		if (stu[stu_count].g_ok != 0)
			stu[stu_count].g_ok = 1;
		if (stu[stu_count].c_ok != 0)
			stu[stu_count].c_ok = 1;

		bool re_stu = 0;//是否重复
		if (stu_count > 0)//检查重复
		{
			for (int n = 0; n <= stu_count - 1; n++)
			{
				if (stu[n].no == stu[stu_count].no)
				{
					re_stu = 1;
				}
			}
		}

		if (re_stu == 0)
		{
			line_count++;
			stu_count++;
		}
		else
		{
			msg << "\t--> 学生名单[" << filename << "]中的第[" << line_count << "]行学号[" << stu[stu_count].no << "]重复，忽略" << endl;
			line_count++;
		}
	}

	stu_num = stu_count;
	msg << "读取完成，共[" << line_count - 1 << "]行有内容，去重后学生总数[" << stu_count << "]个";

	fin.close();
	return 0;
}

/***************************************************************************
  函数名称：
  功    能：排序打印
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void print_stulist(student stu[], int stu_num, ostringstream& msg)
{
	for (int i = 0; i <= stu_num - 2; i++)//先按照学号排序
	{
		for (int j = i + 1; j <= stu_num - 1; j++)
		{
			if (stu[i].no > stu[j].no)
			{
				student temp;
				temp = stu[i];
				stu[i] = stu[j];
				stu[j] = temp;
			}
		}
	}

	for (int i = 0; i < stu_num - 1; i++)//按照课号冒泡排序
	{
		for (int j = 0; j < stu_num - i - 1; j++)
		{
			if (stu[j].class_no > stu[j + 1].class_no)
			{
				student temp;
				temp = stu[j];
				stu[j] = stu[j + 1];
				stu[j + 1] = temp;
			}
		}
	}
	msg << "序号\t课号\t学号\t姓名\t原始程序正确\t校验程序正确" << endl;
	for (int n = 0; n <= stu_num - 1; n++)
	{
		msg << n + 1 << "#\t" << stu[n].class_no << "\t" << stu[n].no << "\t" << stu[n].name << "\t" << stu[n].g_ok << "\t" << stu[n].c_ok;
		if (n != stu_num - 1)
			msg << endl;
	}

}

/***************************************************************************
  函数名称：
  功    能：判断是否是空行
  输入参数：
  返 回 值：是空行返回0 不是空行返回1
  说    明：
***************************************************************************/
bool JudgeCh(char ch[])
{
	if (strlen(ch) == 0)
		return 0;
	for (int i = 0; i <= (int)strlen(ch) - 1; i++)
	{
		if (ch[i] != '\r' && ch[i] != '\n' && ch[i] != ' ' && ch[i] != 0x09)
			return 1;
	}
	return 0;
}

/***************************************************************************
  函数名称：
  功    能：判断一行之中是否以//或 / *开头,如果以/ *开头又是否有对应结尾 以及判
			断开头与第一个字符串、删除最后一个字符串与结尾之间的空格/制表符和结尾
  输入参数：
  返 回 值：注释类型（一行or多行） 参数返回是否有空格或\t 1为有空格 0为无空格
  说    明：
***************************************************************************/
int Analyse2nd(char(&ch)[350], bool& begin_blank)
{
	int length = strlen(ch);
	int type;
	int p;
	for (p = 0; p <= length - 1; p++)
	{
		if (ch[p] != '\t' && ch[p] != ' ')
			break;
	}
	//p指向第一个不为空格或\t的字符
	if (ch[p] != '/')
		return ERROR_NOTE;

	if (length == p + 1)//如果长度不够
		return ERROR_NOTE;

	if (ch[p + 1] == '*')
	{
		type = MULTIPLE_NOTE;
		if (length == p + 2)
			return ERROR_NOTE;
		if (ch[p + 2] != ' ' && ch[p + 2] != '\t')//如果//后面紧跟的是有效字符
			begin_blank = 0;
		else
			begin_blank = 1;
	}
	else if (ch[p + 1] == '/')
	{
		type = SINGLE_NOTE;
		if (length == p + 2)
			return ERROR_NOTE;
		if (ch[p + 2] != ' ' && ch[p + 2] != '\t')//如果//后面紧跟的是有效字符
			begin_blank = 0;
		else
			begin_blank = 1;
		/*把换行符和空格去掉*/
		int sp = sizeof(ch) - 1;
		for (int i = 0; i <= (int)sizeof(ch) - 1; i++)
		{
			if (ch[i] == '\n' || ch[i] == '\r')
			{
				ch[i] = '\0';
				sp = i;
			}
		}
		/*把换行前的空格去掉*/
		int j = sp - 1;
		while (1)
		{
			if (ch[j] != '\r' && ch[j] != '\n' && ch[j] != '\t' && ch[j] != ' ')
				break;
			if (ch[j] == ' ' || ch[j] == '\t')
				ch[j] = '\0';
			j--;
		}
		return type;
	}
	else
		return ERROR_NOTE;

	p += 2;

	bool find = 0;
	bool all_blank = 1;//判断中间是否全是空格
	for (; p <= length - 2; p++)
	{
		if (ch[p] == '*' && ch[p + 1] == '/')
		{
			find = 1;
			break;
		}
		if (ch[p] != ' ' && ch[p] != '\t')
			all_blank = 0;//如果有别的字符就不是全是空格
	}
	if (find == 0)
		return ERROR_NOTE;
	if (all_blank == 1)//如果当中全是空格
		return SPECIAL_TYPE;

	int p2 = p - 1;
	//把*/包括后面的全变成\0
	int s = sizeof(ch);
	for (; p <= s - 1; p++)
		ch[p] = '\0';

	//把*/前面的空格全去掉
	while (1)
	{
		if (ch[p2] != ' ' && ch[p2] != '\t')
			break;
		ch[p2] = '\0';
		p2--;
	}
	return MULTIPLE_NOTE;
}

/***************************************************************************
  函数名称：
  功    能：读第二行
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int read_2nd_line(const string filename, student stu[], int stu_num, ostringstream& msg)
{
	msg << "序号\t课号\t学号\t姓名\t文件名\t分析结果" << endl;

	for (int s = 0; s < stu_num; s++)//所有学生循环
	{
		ifstream fin;//写在里面，这样getline读入错误时不用重置状态
		char note[350] = { '\0' };//用来装一行的注释
		int fault = 0;//错误的个数（用来看错误输出是否要加\t）

		msg << s + 1 << "#\t" << stu[s].class_no << "\t" << stu[s].no << "\t" << stu[s].name << "\t" << filename << "\t";

		string fn = "./source/"; //真正的filename
		char class_no[50] = { '\0' };
		_itoa(stu[s].class_no, class_no, 10);
		char s_no[50] = { 0 };
		_itoa(stu[s].no, s_no, 10);
		fn = fn + class_no + "-" + filename + "/" + s_no + "-" + stu[s].name + "-" + filename;
		fin.open(fn, ios::in | ios::binary);
		if (!fin.is_open())
		{
			msg << "未提交" << endl;
			stu[s].c_ok = 0;//检查程序原始分为0
			continue;
		}

		bool overfl = 0;
		while (1)//把前面的空行和第一行读完
		{
			char rubbish[300] = { '\0' };
			fin.getline(rubbish, 290, '\n');
			if (strlen(rubbish) >= 255)
			{
				msg << "非空行超过了合理长度[256]字节." << endl;
				overfl = 1;
				fin.close();
				break;
			}
			if (JudgeCh(rubbish) == 1)
				break;
		}
		if (overfl == 1)
			continue;

		overfl = 0;
		while (1)
		{
			for (int n = 0; n <= (int)sizeof(note) - 1; n++)
				note[n] = '\0';
			fin.getline(note, 340, '\n');//读入第二行
			if (!fin.good())
			{
				fin.clear();
			}
			if (strlen(note) >= 255)
			{
				msg << "非空行超过了合理长度[256]字节." << endl;
				fin.close();
				overfl = 1;
				break;
			}
			if (JudgeCh(note) == 1)
				break;
		}

		if (overfl == 1)
			continue;

		bool begin_blank;
		int analyse = Analyse2nd(note, begin_blank);


		if (analyse == ERROR_NOTE)
		{
			msg << "第2行信息有错[不是//或/*开头，或者以/*开头但*/不在同一行]" << endl;
			fin.close();
			continue;
		}

		if (analyse == SPECIAL_TYPE)
		{
			msg << "ok" << endl;
			fin.close();
			continue;
		}

		istringstream in(note);
		int count = 0;//互验学生的个数

		while (in.good())//按学生数读入
		{
			char no[260] = { '\0' };
			char name[260] = { '\0' };
			int noi;

			if (count == 0)//如果读入第一个
			{
				if (begin_blank == 1)//如果//与后面之间有空格
				{
					char rub[260];
					in >> rub >> no;

				}
				else {
					in >> no;

					int len = (int)(strlen(no));
					for (int i = 2; i <= len - 1; i++)//整个往前挪2位 踢掉//
					{
						no[i - 2] = no[i];
						no[i] = '\0';
					}
				}
			}
			else
				in >> no;

			if (strlen(no) > 63)
			{
				int len = strlen(no);
				for (int n = 63; n <= len - 1; n++)
					name[n - 63] = no[n];
				no[63] = '\0';
			}
			else if (!in.good())//已经到文件末尾了
			{
				fault++;//多了一个错误
				if (fault > 1)
					msg << "\t\t\t\t\t";
				msg << "第[" << count << "]个学生后面的信息不全，忽略" << endl;
				in.clear();
				break;
			}
			else
				in >> name;//读入姓名
			/*
			if (strlen(name) == 2 && name[0] == '*' && name[1] == '/')
			{
				fault++;//多了一个错误
				if (fault > 1)
					msg << "\t\t\t\t\t";
				msg << "第[" << count << "]个学生后面的信息不全，忽略" << endl;
				break;
			}
			*/

			count++;//加学生个数
			noi = atoi(no);
			bool find = 0;
			int i;

			if (noi == stu[s].no)
			{
				fault++;//多了一个错误
				if (fault > 1)
					msg << "\t\t\t\t\t";
				msg << "第[" << count << "]项写了自己，耍无赖么？" << endl;
				break;
			}

			for (i = 0; i <= stu_num - 1; i++)//在所有学生中找这个学生
			{
				if (stu[i].no == noi)
				{
					find = 1;
					break;
				}
			}

			if (find == 1 && strcmp(name, stu[i].name) == 0)//找到并且名字一样
			{
				bool restu = 0;
				for (int j = 0; j < stu[s].partner_num; j++)//检查是否重复写同一个人
				{
					if (noi == stu[s].check_group[j].no)//如果重复
					{
						fault++;//多了一个错误
						if (fault > 1)
							msg << "\t\t\t\t\t";
						msg << "互验列表第[" << count << "]项的学号[" << noi << "]重复，忽略" << endl;
						restu = 1;
						break;
					}
				}
				if (restu == 1)//如果是重复的学生
					continue;//读下一个


				stu[s].partner_num++;//互查人数加一			

				stu[s].check_group[stu[s].partner_num - 1].no = noi;//添加新值
				strcpy(stu[s].check_group[stu[s].partner_num - 1].name, name);

			}
			else
			{
				fault++;
				if (fault > 1)
					msg << "\t\t\t\t\t";
				msg << "第[" << count << "]项的学号[" << no << "]/姓名[" << name << "]不正确，忽略" << endl;
			}
		}
		if (stu[s].partner_num == 0)
			continue;
		if (fault == 0)
			msg << "ok" << endl;
		msg << "\t读到的正确互验者" << endl;
		/*先按学号排个序*/
		for (int m = 0; m <= stu[s].partner_num - 1 - 1; m++)
		{
			for (int n = m + 1; n <= stu[s].partner_num - 1; n++)
			{
				if (stu[s].check_group[m].no > stu[s].check_group[n].no)
				{
					partner temp;
					temp = stu[s].check_group[m];
					stu[s].check_group[m] = stu[s].check_group[n];
					stu[s].check_group[n] = temp;
				}
			}
		}
		/*输出*/
		for (int n = 0; n <= stu[s].partner_num - 1; n++)
		{
			msg << "\t" << n + 1 << "\t" << stu[s].check_group[n].no << "\t" << stu[s].check_group[n].name << endl;
		}

		fin.close();
	}

	return 0;
}

/***************************************************************************
  函数名称：
  功    能：输出第二行匹配结果
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void print_2nd_line(student stu[], int stu_num, ostringstream& msg)
{
	msg << "序号\t课号\t学号\t姓名" << endl;
	for (int s = 0; s <= stu_num - 1; s++)//对所有学生进行循环
	{
		msg << s + 1 << "#\t" << stu[s].class_no << "\t" << stu[s].no << "\t" << stu[s].name;
		if (stu[s].partner_num == 0)//如果互查的人是0
		{
			msg << "\t无有效匹配项" << endl;
			stu[s].c_de_point2 -= 5 * 20;
			continue;
		}
		if (stu[s].partner_num < 5)//如果互查人数小于5人
		{
			stu[s].c_de_point2 -= (5 - stu[s].partner_num) * 20;
		}

		msg << endl;
		/*检查互查小组每个人是否抛弃了你*/
		for (int p = 0; p <= stu[s].partner_num - 1; p++)//遍历小组成员
		{
			msg << "\t$" << p + 1 << "\t" << stu[s].check_group[p].no << "\t" << stu[s].check_group[p].name << "\t";
			int i;
			bool find = 0;
			for (i = 0; i <= stu_num - 1; i++)//stu数组里找这个小组成员
			{
				if (stu[i].no == stu[s].check_group[p].no)
					break;
			}
			//i指向stu数组里小组成员所在的下标
			if (stu[i].partner_num == 0)//如果对方没有互验成员
			{
				msg << "抛弃了你" << endl;
				stu[s].c_de_point1 -= 100;//相当于未经允许写别人名字
				continue;
			}

			for (int j = 0; j <= stu[i].partner_num - 1; j++)
			{
				if (stu[i].check_group[j].no == stu[s].no)//如果对方的check_group里有自己的学号
				{
					find = 1;//找到
					break;
				}
			}
			if (find == 1)//对方和你是互验关系
			{
				msg << "ok" << endl;
				if (stu[i].g_ok == 0)//如果对方的b2不正确
					stu[s].c_de_point2 -= 20;
			}
			else//对方check_group里没有你
			{
				msg << "抛弃了你" << endl;
				stu[s].c_de_point1 -= 100;//相当于未经允许写别人名字
			}
		}
	}
}

/***************************************************************************
  函数名称：
  功    能：输出最终得分
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void print_finalscore(student stu[], int stu_num, ostringstream& msg)
{
	msg << "序号\t课号\t学号\t姓名\t原始程序得分\t检查程序原始分\t检查程序扣分1\t检查程序扣分2\t检查程序总分" << endl;
	for (int s = 0; s <= stu_num - 1; s++)
	{
		msg << s + 1 << "#\t" << stu[s].class_no << "\t" << stu[s].no << "\t" << stu[s].name << "\t" << 100 * stu[s].g_ok
			<< "\t" << 100 * stu[s].c_ok << "\t" << stu[s].c_de_point1 << "\t" << stu[s].c_de_point2
			<< "\t" << 100 * stu[s].c_ok + stu[s].c_de_point1 + stu[s].c_de_point2 << endl;
	}
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：具体略，实际使用时，放入另一个cpp(例：hw_check_secondline.cpp)更合理
 ***************************************************************************/
int check_secondline(const string& cno, const string &stulist_fname, const string& filename, const string &debug)
{
	//cout << "开始进行源程序文件次行检查..." << endl;//本次作业要求实现，本函数要改动

	ostringstream msg;
	int stu_num;
	student stu[1000];

	
	/* 读课号.dat */
	msg << "1.读取课号配置文件" << endl;
	string stulist_true_name = "./source/" + stulist_fname;
	int read_result = read_stulist(stulist_true_name, msg, stu, stu_num);
	if (read_result == FAIL_OPEN)
	{
		msg << "\t--> 无法打开" << stulist_true_name << endl;
	}
	if (debug[0] == '1')
	{
		if (debug[1] == '1' || debug[2] == '1' || debug[3] == '1' || debug[4] == '1')
			msg << endl;
		cout << msg.str() << endl;
	}

	msg.str(""); //清空msg中的信息
	msg << "2.按课号+学号排序的学生名单" << endl;
	print_stulist(stu, stu_num, msg);

	if (debug[1] == '1')
	{
		if (debug[2] == '1' || debug[3] == '1' || debug[4] == '1')
			msg << endl;
		cout << msg.str() << endl;
	}

	msg.str(""); //清空msg中的信息
	msg << "3.所有学生读取第2行的解析信息" << endl;
	read_2nd_line(filename, stu, stu_num, msg);

	if (debug[2] == '1')
	{
		if (debug[3] == '1' || debug[4] == '1')
			msg << endl;
		cout << msg.str();
	}

	msg.str(""); //清空msg中的信息
	msg << "4.所有学生的第2行互验匹配结果" << endl;
	print_2nd_line(stu, stu_num, msg);

	if (debug[3] == '1')
	{
		if (debug[4] == '1')
			msg << endl;
		cout << msg.str();
	}

	/* 再根据通过与否进行检查 */
	msg.str(""); //清空msg中的信息
	msg << "5.所有学生的第2行互验最终得分" << endl;
	print_finalscore(stu, stu_num, msg);

	if (debug[4] == '1')
	{
		cout << msg.str();
	}
	
	return 0;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：本次作业不要求实现，本函数不要改动
 ***************************************************************************/
int check_firstline(const string& cno, const string &stilist_fname, const string& filename)
{
	cout << "开始进行源程序文件首行检查..." << endl;
	return 0;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
static void usage(const char* const procname, const int args_num)
{
	const int wkey = 7 + strlen(procname) + 1;
	const int wopt = 7 + strlen(procname) + 4;

	cout << endl;
	cout << "Usage: " << procname << " { --firstline | --secondline  }" << endl;
	cout << setw(wkey) << ' ' << "{ --cno xxx }" << endl;
	cout << setw(wkey) << ' ' << "{ --filename xxx/all }" << endl; //约定不允许实际文件名为all
	cout << setw(wkey) << ' ' << "{ --stulist_fname xxx }" << endl;
	cout << setw(wkey) << ' ' << "{ --debug 11111 }" << endl;
	cout << endl;

	cout << setw(wkey) << ' ' << "必选项：指定检查类型(多选一)" << endl;
	cout << setw(wopt) << ' ' << "--firstline     : 首行检查" << endl;
	cout << setw(wopt) << ' ' << "--secondline    : 次行检查" << endl;
	cout << setw(wkey) << ' ' << "必选项：" << endl;
	cout << setw(wopt) << ' ' << "--cno           : 课号" << endl;
	cout << setw(wopt) << ' ' << "--filename      : 文件名(all表示全部)" << endl;
	cout << setw(wopt) << ' ' << "--stulist_fname : 对应的学生列表文件名" << endl;
	cout << setw(wkey) << ' ' << "可选项：" << endl;
	cout << setw(wopt) << ' ' << "--debug         : Debug选项(仅适用于--secondline，默认11111)" << endl;
	cout << endl;

	cout << "e.g.   " << procname << " --firstline --cno 100692 --filename 3-b1.cpp --stulist_fname 100692.txt               : 检查100692课程的3-b1.cpp的首行" << endl;
	cout << "       " << procname << " --firstline --cno 100692 --filename all --stulist_fname 100692-firstline.txt          : 检查100692课程的全部文件的首行" << endl;
	cout << "       " << procname << " --secondline --cno 100742 --filename 15-b5.c --stulist_fname 10108001.dat               : 检查100742课程的15-b5.c的次行，输出全部信息" << endl;
	cout << "       " << procname << " --secondline --cno 100742 --filename 15-b5.c --stulist_fname 10108001.dat --debug 01001 : 检查100742课程的15-b5.c的次行，仅输出2、5项信息" << endl;
	cout << endl;

	if (args_num == 1) {
		cout << endl << endl << "请在cmd下加参数运行." << endl << endl;
		system("pause");
	}
}

/* enum的顺序要和args的排列顺序保持一致
   不用enum class 只是不想每次都做强制转换 */
enum OPT_ARGS { OPT_ARGS_HELP = 0, OPT_ARGS_FIRSTLINE, OPT_ARGS_SECONDLINE, OPT_ARGS_COURSEID, OPT_ARGS_FILENAME, OPT_ARGS_STULIST_FNAME, OPT_ARGS_DEBUG };

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int main(int argc, char** argv)
{
	args_analyse_tools args[] = {//表示可出现的参数的类型和其他说明
		args_analyse_tools("--help", ST_EXTARGS_TYPE::boolean, 0, false),
		args_analyse_tools("--firstline", ST_EXTARGS_TYPE::boolean, 0, false),
		args_analyse_tools("--secondline", ST_EXTARGS_TYPE::boolean, 0, false),
		args_analyse_tools("--courseid", ST_EXTARGS_TYPE::str, 1, string("")),
		args_analyse_tools("--filename", ST_EXTARGS_TYPE::str, 1, string("")),
		args_analyse_tools("--stulist_fname", ST_EXTARGS_TYPE::str, 1, string("")),
		args_analyse_tools("--debug", ST_EXTARGS_TYPE::str, 1, string("11111")),
		args_analyse_tools()  //最后一个，用于结束
	};

	int cur_argc, ret = 0;

	//最后一个参数1，表示除选项参数外，还有其它参数
	if ((cur_argc = args_analyse_process(argc, argv, args, 0)) < 0) {
		//错误信息在函数中已打印
		return -1;
	}

	/* 需要自己实现 */
	//args_analyse_print(args);

	/* 对help做特殊处理 */
	if (args[OPT_ARGS_HELP].existed()) {
		//只要有 --help，其它参数都忽略，显示帮助即可
		usage(argv[0], argc);
		return -1; //执行完成直接退出
	}

	/* 检查参数互斥性 */
	if (args[OPT_ARGS_FIRSTLINE].existed() + args[OPT_ARGS_SECONDLINE].existed() != 1) {
		usage(argv[0], argc);
		cout << "参数[" << args[OPT_ARGS_FIRSTLINE].get_name() << "|" << args[OPT_ARGS_SECONDLINE].get_name() << "]必须指定一个且只能指定一个" << endl;
		return -1;
	}

	/* 判断必选项是否存在 */
	if (args[OPT_ARGS_COURSEID].existed() == 0) {
		cout << "必须指定参数[" << args[OPT_ARGS_COURSEID].get_name() << "]" << endl;
		usage(argv[0], argc);
		return -1;
	}

	/* 判断必选项是否存在 */
	if (args[OPT_ARGS_FILENAME].existed() == 0) {
		cout << "必须指定参数[" << args[OPT_ARGS_FILENAME].get_name() << "]" << endl;
		usage(argv[0], argc);
		return -1;
	}

	/* 判断必选项是否存在 */
	if (args[OPT_ARGS_STULIST_FNAME].existed() == 0) {
		cout << "必须指定参数[" << args[OPT_ARGS_STULIST_FNAME].get_name() << "]" << endl;
		usage(argv[0], argc);
		return -1;
	}

	/* 判断 --firstline 和 --debug 是否冲突 */
	if (args[OPT_ARGS_FIRSTLINE].existed() + args[OPT_ARGS_DEBUG].existed() == 2) {
		cout << "参数[" << args[OPT_ARGS_FIRSTLINE].get_name() << "]不需要[" << args[OPT_ARGS_DEBUG].get_name() << "]" << endl;
		usage(argv[0], argc);
		return -1;
	}

	/* 判断 --secondline 和 --filename all 是否冲突 */
	if (args[OPT_ARGS_SECONDLINE].existed()) {
		if (args[OPT_ARGS_FILENAME].get_string() == "all") {
			cout << "参数[" << args[OPT_ARGS_SECONDLINE].get_name() << "]的[" << args[OPT_ARGS_SECONDLINE].get_name() << "]不能为all" << endl;
			usage(argv[0], argc);
			return -1;
		}
	}

	if (args[OPT_ARGS_FIRSTLINE].existed()) {
		/* 进行首行检查 */
		check_firstline(args[OPT_ARGS_COURSEID].get_string(), args[OPT_ARGS_STULIST_FNAME].get_string(), args[OPT_ARGS_FILENAME].get_string());
	}
	else if (args[OPT_ARGS_SECONDLINE].existed()) {
		/* 进行次行检查 */
		check_secondline(args[OPT_ARGS_COURSEID].get_string(), args[OPT_ARGS_STULIST_FNAME].get_string(), args[OPT_ARGS_FILENAME].get_string(), args[OPT_ARGS_DEBUG].get_string());
	}

	cout << endl;
	return 0;
}
