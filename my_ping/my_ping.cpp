/* 2050598 杨洁宁 通信 */
#include <iostream>
//可根据需要再加入其它需要的头文件
#include "../include/class_aat.h"
using namespace std;

/* 运行自行添加其它函数，也可以将自定义函数放在其它cpp中 */
//检查ip，返回IP_CORRECT/IP_ERROR
static int judge_ip(const string sip)
{
	int mark1 = 0;
	int mark2 = 0;
	int mark3 = 0;//标记点的位置
	int i, j, k;
	for (i = 0; sip[i] != '\0'; i++)
	{
		if (sip[i] == '.')
		{
			mark1 = i;
			break;
		}
	}
	if (sip[i] == '\0')
	{
		return IP_ERROR;
	}
	for (j = i + 1; sip[j] != '\0'; j++)
	{
		if (sip[j] == '.')
		{
			mark2 = j;
			break;
		}
	}
	if (sip[j] == '\0')
	{
		return IP_ERROR;
	}
	for (k = j + 1; sip[k] != '\0'; k++)
	{
		if (sip[k] == '.')
		{
			mark3 = k;
			break;
		}
	}
	if (sip[k] == '\0' || sip[k + 1] == '\0')
	{
		return IP_ERROR;
	}
	if (mark1 == 0 || mark2 == 0 || mark3 == 0 || mark1 + 1 == mark2 || mark2 + 1 == mark3
		|| mark1 > 4 || mark2 - mark1 > 4 || mark3 - mark2 > 4)
	{
		return IP_ERROR;
	}
	char num_ch[4][4] = { '\0' };
	for (i = 0; i < mark1; i++)
	{
		if (sip[i] >= '0' && sip[i] <= '9')
			num_ch[0][i] = sip[i];
		else
		{
			return IP_ERROR;
		}
	}

	j = 0;
	for (i = mark1 + 1; i < mark2; i++)
	{
		if (sip[i] >= '0' && sip[i] <= '9')
		{
			num_ch[1][j] = sip[i];
			j++;
		}
		else
		{
			return IP_ERROR;
		}
	}

	j = 0;
	for (i = mark2 + 1; i < mark3; i++)
	{
		if (sip[i] >= '0' && sip[i] <= '9')
		{
			num_ch[2][j] = sip[i];
			j++;
		}
		else
		{
			return IP_ERROR;
		}
	}

	j = 0;
	for (i = mark3 + 1; sip[i] != '\0'; i++)
	{
		if (sip[i] >= '0' && sip[i] <= '9')
		{
			num_ch[3][j] = sip[i];
			j++;
		}
		else
		{
			return IP_ERROR;
		}
	}

	int num[4];
	for (i = 0; i <= 3; i++)
	{
		num[i] = atoi(num_ch[i]);
		if (num[i] < 0 || num[i] > 255)
		{
			return IP_ERROR;
		}
	}
	return IP_CORRECT;
}

void static usage(const args_analyse_tools args[], const char* const procname)
{
	cout << "Usage: " << procname << " [" << args[0].get_name() << " 大小][" << args[1].get_name() << " 数量][" << args[2].get_name() << "] IP地址" << endl;
	args_analyse_print(args);
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int main(int argc, char* argv[])
{
	args_analyse_tools args[] = {
		args_analyse_tools("--l",ST_EXTARGS_TYPE::int_with_default,1,64,32,64000),
		args_analyse_tools("--n",ST_EXTARGS_TYPE::int_with_default,1,4,1,1024),
		args_analyse_tools("--t",ST_EXTARGS_TYPE::boolean,0,false),
		args_analyse_tools()  //最后一个，用于结束
	};
	if (argc == 1)
	{
		usage(args, argv[0]);
		return 0;
	}

	/*首先检查ip地址*/
	int check_ip = judge_ip(argv[argc - 1]);
	if (check_ip == IP_ERROR)
	{
		cout << "IP地址错误" << endl;
		return -1;
	}

	/*保证ip正确*/
	int need_fixed_args = 1;
	int cur_argc;
	
	if (argc >= 3)
	{
		if ((cur_argc = args_analyse_process(argc, argv, args, need_fixed_args)) < 0)
			return -1;//错误信息已在函数中打印
	}

	for (int i = 0; i <= 1; i++)
	{
		cout << args[i].get_name() << " 参数：";
		cout << args[i].get_int();
			
		cout << endl;
	}
	cout << args[2].get_name() << " 参数：" << args[2].existed() << endl;
	cout << "IP地址：" << argv[argc - 1] << endl;
	
	return 0;
}

