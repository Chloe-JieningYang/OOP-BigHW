/* 2050598 ����� ͨ�� */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include "../include/class_aat.h"
//���б�Ҫ�����Լ�������ͷ�ļ�
using namespace std;

#if !ENABLE_LIB_AAT_TOOLS //��ʹ��lib����Ч

/* ---------------------------------------------------------------
	 �������������Ҫstatic�������ڲ������ã�
   ---------------------------------------------------------------- */

//��d��z�η�
static unsigned int my_pow(const unsigned int d, const unsigned int z)
{
	if (z == 0)
		return 1;
	unsigned int res = 1;
	for (unsigned int k = 1; k <= z; k++)
		res *= d;
	return res;
}

/***************************************************************************
	�������ƣ�
	��    �ܣ�str ip to u_int ip
	���������
	�� �� ֵ��
	˵    ����
***************************************************************************/
static unsigned int str_ip_to_uint_ip(const string sip, bool &is_right)
{
	int mark1 = 0;
	int mark2 = 0;
	int mark3 = 0;//��ǵ��λ��
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
		is_right = 0;
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
		is_right = 0;
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
		is_right = 0;
		return IP_ERROR;
	}
	if (mark1 == 0 || mark2 == 0 || mark3 == 0 || mark1 + 1 == mark2 || mark2 + 1 == mark3
		|| mark1 > 4 || mark2 - mark1 > 4 || mark3 - mark2 > 4)
	{
		is_right = 0;
		return IP_ERROR;
	}
	char num_ch[4][4] = { '\0' };
	for (i = 0; i < mark1; i++)
	{
		if (sip[i] >= '0' && sip[i] <= '9')
			num_ch[0][i] = sip[i];
		else
		{
			is_right = 0;
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
			is_right = 0;
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
			is_right = 0;
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
			is_right = 0;
			return IP_ERROR;
		}
	}

	int num[4];
	for (i = 0; i <= 3; i++)
	{
		num[i] = atoi(num_ch[i]);
		if (num[i] < 0 || num[i] > 255)
		{
			is_right = 0;
			return IP_ERROR;
		}
	}

	is_right = 1;

	unsigned int res = num[0] / 16 * my_pow(16, 7) + num[0] % 16 * my_pow(16, 6) + num[1] / 16 * my_pow(16, 5)
		+ num[1] % 16 * my_pow(16, 4) + num[2] / 16 * my_pow(16, 3) + num[2] % 16 * my_pow(16, 2) +
		num[3] / 16 * my_pow(16, 1) + num[3] % 16;

	return res;
}

/***************************************************************************
	�������ƣ�
	��    �ܣ���ӡ����е�type
	���������
	�� �� ֵ��
	˵    ����
***************************************************************************/
static string str_type(const ST_EXTARGS_TYPE type_i)
{
	string type_str[] = {"null","Bool","IntWithDefault","IntWithError","IntSETWithDefault",
	"IntSETWithError","String","StringSETWithDefault","StringSETWithError","IPAddrWithDefault",
	"IPAddrWithError"};
	string res;
	if (type_i == ST_EXTARGS_TYPE::null)
	{
		res = type_str[0];
	}
	else if (type_i == ST_EXTARGS_TYPE::boolean)
	{
		res = type_str[1];
	}
	else if (type_i == ST_EXTARGS_TYPE::int_with_default)
	{
		res = type_str[2];
	}
	else if (type_i == ST_EXTARGS_TYPE::int_with_error)
	{
		res = type_str[3];
	}
	else if (type_i == ST_EXTARGS_TYPE::int_with_set_default)
	{
		res = type_str[4];
	}
	else if (type_i == ST_EXTARGS_TYPE::int_with_set_error)
	{
		res = type_str[5];
	}
	else if (type_i == ST_EXTARGS_TYPE::str)
	{
		res = type_str[6];
	}
	else if (type_i == ST_EXTARGS_TYPE::str_with_set_default)
	{
		res = type_str[7];
	}
	else if (type_i == ST_EXTARGS_TYPE::str_with_set_error)
	{
		res = type_str[8];
	}
	else if (type_i == ST_EXTARGS_TYPE::ipaddr_with_default)
	{
		res = type_str[9];
	}
	else if (type_i == ST_EXTARGS_TYPE::ipaddr_with_error)
	{
		res = type_str[10];
	}

	return res;
}

/***************************************************************************
	�������ƣ�
	��    �ܣ�����ַ������Ƿ�ȫ��0-9���ַ�
	���������
	�� �� ֵ���Ƿ���1 ���Ƿ���0
	˵    ����
***************************************************************************/
static bool judge_str_num(const char ch[])
{
	bool is_num = 1;
	for (int k = 0; ch[k] != '\0'; k++)
	{
		if (ch[k] < '0' || ch[k]>'9')
		{
			if (k == 0 && ch[k] == '-')
				continue;
			is_num = 0;
			break;
		}
	}
	return is_num;
}

/***************************************************************************
	�������ƣ�
	��    �ܣ�����������Ƿ���ĳ����
	������������飬����Ԫ������Ҫ�ҵ�����
	�� �� ֵ��
	˵    �����з���1��û�з���0
***************************************************************************/
static bool judge_in_set_int(const int array[], const int array_sum, const int num)
{
	bool is_in = 0;
	for (int k = 0; k <= array_sum - 1; k++)
	{
		if (array[k] == num)
		{
			is_in = 1;
			break;
		}
	}
	return is_in;
}

/***************************************************************************
	�������ƣ�
	��    �ܣ�����ַ��������Ƿ���ĳ�ַ���
	����������ַ����飬�ַ�����Ԫ������Ҫ�ҵ��ַ���
	�� �� ֵ��
	˵    �����з���1��û�з���0
***************************************************************************/
static bool judge_in_set_str(const string array[], const int array_sum, const string str)
{
	bool is_in = 0;
	for (int k = 0; k < array_sum; k++)
	{
		if (array[k] == str)
		{
			is_in = 1;
			break;
		}
	}
	return is_in;
}

/***************************************************************************
	�������ƣ�
	��    �ܣ�
	���������unsigned int��ip ת��Ϊ string
	�� �� ֵ��
	˵    ����
***************************************************************************/
static const string ip_uint_to_str(const unsigned int temp)
{
	string res;
	char ch1[10] = { '\0' };
	char ch2[10] = { '\0' };
	char ch3[10] = { '\0' };
	char ch4[10] = { '\0' };
	unsigned int value = temp;
	_itoa(value % (16 * 16), ch4, 10);
	value /= 16 * 16;
	_itoa(value % (16 * 16), ch3, 10);
	value /= 16 * 16;
	_itoa(value % (16 * 16), ch2, 10);
	value /= 16 * 16;
	_itoa(value % (16 * 16), ch1, 10);

	res = ch1 + string(".") + ch2 + string(".") + ch3 + string(".") + ch4;
	return res; //�˾������Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����null
 ***************************************************************************/
args_analyse_tools::args_analyse_tools()
{
	args_name = "";
	extargs_type = ST_EXTARGS_TYPE::null;//��Ϊ�ж�����
	extargs_num = 0;
	extargs_bool_default = 0;
	extargs_int_default = 0;
	extargs_int_min = 0;
	extargs_int_max = 0;
	extargs_int_set = NULL;
	extargs_string_default = "";
	extargs_string_set = NULL;
	extargs_ipaddr_default = 0;
	args_existed = 0;
	extargs_int_value = 0;
	extargs_string_value = "";
	extargs_ipaddr_value = 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����boolean
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const bool def)
{
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	extargs_bool_default = def;

	extargs_int_set = NULL;
	extargs_string_set = NULL;

	args_existed = 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����int_with_default��int_with_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const int def, const int _min, const int _max)
{
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	if (type == ST_EXTARGS_TYPE::int_with_default)
		extargs_int_default = def;
	extargs_int_max = _max;
	extargs_int_min = _min;

	extargs_int_set = NULL;
	extargs_string_set = NULL;

	args_existed = 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����int_with_set_default��int_with_set_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const int def_of_set_pos, const int* const set)
{
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	int count = 0;//������int������ЧԪ�ظ���
	int k = 0;
	while (1)
	{
		if (set[count] != INVALID_INT_VALUE_OF_SET)
		{
			count++;
		}
		else
			break;
	}
	
	extargs_int_set = new(nothrow) int[count];//����ռ䣬��Ҫ�ͷ�
	if (extargs_int_set == NULL)
		return;
	for (k = 0; k < count; k++)
		extargs_int_set[k] = set[k];
	if (type == ST_EXTARGS_TYPE::int_with_set_default)//�����Ĭ��ֵ
	{
		if (def_of_set_pos <= count - 1)
			extargs_int_default = set[def_of_set_pos];
		else
			extargs_int_default = set[0];
	}
	extargs_int_set_num = count;

	extargs_string_set = NULL;

	args_existed = 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����str��ipaddr_with_default��ipaddr_with_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const string def)
{
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	if (type == ST_EXTARGS_TYPE::str)//�����str
	{
		extargs_string_default = def;
	}
	else if (type == ST_EXTARGS_TYPE::ipaddr_with_default)//�����������Ĭ�ϵ�ip��ַ
	{
		bool is_right;
		extargs_ipaddr_default = str_ip_to_uint_ip(def, is_right);
	}

	extargs_int_set = NULL;
	extargs_string_set = NULL;

	args_existed = 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����str_with_set_default��str_with_set_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const int def_of_set_pos, const string* const set)
{
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	int count = 0;
	int k = 0;
	while (1)
	{
		if (set[count] != "")//set������""��β
			count++;
		else
			break;
	}
	extargs_string_set = new(nothrow) string[count];//���룬�ǵ��ͷ�
	if (extargs_string_set == NULL)
		return;
	for (k = 0; k < count; k++)
		extargs_string_set[k] = set[k];
	if (type == ST_EXTARGS_TYPE::str_with_set_default)//�����Ĭ��ֵ
	{
		if (def_of_set_pos <= count - 1)
			extargs_string_default = set[def_of_set_pos];
		else
			extargs_string_default = set[0];
	}
	extargs_string_set_num = count;

	extargs_int_set = NULL;

	args_existed = 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
args_analyse_tools::~args_analyse_tools()
{
	if (extargs_int_set != NULL)
		delete[]extargs_int_set;
	if (extargs_string_set != NULL)
		delete[]extargs_string_set;
}

/* ---------------------------------------------------------------
	 ����AAT���Զ����Ա������ʵ�֣�private��
   ---------------------------------------------------------------- */

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
const string args_analyse_tools::get_name() const
{
	return args_name;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������!!��ֻ����0/1
			��ʵ�֣���Ҫ��
 ***************************************************************************/
const int args_analyse_tools::existed() const
{
	return !!args_existed;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
const int args_analyse_tools::get_int() const
{
	return extargs_int_value;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
const string args_analyse_tools::get_string() const
{
	return extargs_string_value;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
const unsigned int args_analyse_tools::get_ipaddr() const
{
	return extargs_ipaddr_value;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ extargs_ipaddr_value ��ֵ�� 0x7f 00 00 01 תΪ "127.0.0.1"
 ***************************************************************************/
const string args_analyse_tools::get_str_ipaddr() const
{
	string res;
	char ch1[10] = { '\0' };
	char ch2[10] = { '\0' };
	char ch3[10] = { '\0' };
	char ch4[10] = { '\0' };
	unsigned int value = extargs_ipaddr_value;
	_itoa(value % (16 * 16), ch4, 10);
	value /= 16 * 16;
	_itoa(value % (16 * 16), ch3, 10);
	value /= 16 * 16;
	_itoa(value % (16 * 16), ch2, 10);
	value /= 16 * 16;
	_itoa(value % (16 * 16), ch1, 10);

	res = ch1 + string(".") + ch2 + string(".") + ch3 + string(".") + ch4;
	return res; //�˾������Ҫ�޸�
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������follow_up_args���Ƿ��к�������
			./log_login --limit 10 --user root                    ���޺�����������0
			./ulimits_check --memory 256 --cpu 1 [./log_login ***]���к�����������1
  �� �� ֵ��
  ˵    ������Ԫ����
***************************************************************************/
int args_analyse_process(const int argc, const char* const* const argv, args_analyse_tools* const args, const int follow_up_args)
{
	int i = 1;
	int j;
	while (i < argc)
	{
		if (argv[i][0] == '-' && argv[i][1] == '-')
		{
			bool find = 0;
			for (j = 0; args[j].extargs_type != ST_EXTARGS_TYPE::null; j++)
			{
				if (args[j].args_name == argv[i])
				{
					find = 1;
					break;
				}
			}
			if (find == 0)
			{
				cout << "����[" << argv[i] << "�Ƿ�" << endl;
				return -1;
			}
			if (args[j].existed() == 0)//����ǵ�һ�γ���
			{
				args[j].args_existed = 1;//��exist��1
				if (args[j].extargs_num == 0)//������Ӳ���������0
				{
					i++;
					continue;
				}
				else//�����Ӳ���
				{
					if (i == argc - 1 || (argv[i + 1][0] == '-' && argv[i + 1][1] == '-'))//����û�ҵ����Ӳ���
					{
						cout << "����[" << argv[i] << "]�ĸ��Ӳ������� (����:";
						if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_default)
						{
							cout << "int, ��Χ[" << args[j].extargs_int_min << ".." << args[j].extargs_int_max << "] ȱʡ:" <<
								args[j].extargs_int_default << ")" << endl;
							return -1;
						}
						else if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_error)
						{
							cout << "int, ��Χ[" << args[j].extargs_int_min << ".." << args[j].extargs_int_max << "])" << endl;
							return -1;
						}
						else if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_default)
						{
							cout << "int, ��ȡֵ[";
							for (int k = 0; k < args[j].extargs_int_set_num; k++)
							{
								cout << args[j].extargs_int_set[k];
								if (k != args[j].extargs_int_set_num - 1)
									cout << "/";
							}
							cout << "] ȱʡ:" << args[j].extargs_int_default << ")" << endl;
						}
						else if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
						{
							cout << "int, ��ȡֵ[";
							for (int k = 0; k < args[j].extargs_int_set_num; k++)
							{
								cout << args[j].extargs_int_set[k];
								if (k != args[j].extargs_int_set_num - 1)
									cout << "/";
							}
							cout << "])" << endl;
						}
						else if (args[j].extargs_type == ST_EXTARGS_TYPE::str)
						{
							cout << "string";
							if (args[j].extargs_string_default == "")//�൱��û��Ĭ�ϵ�string
								cout << ")" << endl;
							else
								cout << " ȱʡ:" << args[j].extargs_string_default << ")" << endl;
						}
						else if (args[j].extargs_type == ST_EXTARGS_TYPE::str_with_set_default)
						{
							cout << "string, ��ȡֵ[";
							for (int k = 0; k < args[j].extargs_string_set_num; k++)
							{
								cout << args[j].extargs_string_set[k];
								if (k != args[j].extargs_string_set_num - 1)
									cout << "/";
							}
							cout << "] ȱʡ:" << args[j].extargs_string_default << ")" << endl;
						}
						else if (args[j].extargs_type == ST_EXTARGS_TYPE::str_with_set_error)
						{
							cout << "string, ��ȡֵ[";
							for (int k = 0; k < args[j].extargs_string_set_num; k++)
							{
								cout << args[j].extargs_string_set[k];
								if (k != args[j].extargs_string_set_num - 1)
									cout << "/";
							}
							cout << "])" << endl;
						}
						else if (args[j].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_default)
						{
							cout << "IP��ַ ȱʡ:" << args[j].get_str_ipaddr() << ")" << endl;
						}
						else if (args[j].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_error)
						{
							cout << "IP��ַ)" << endl;
						}

						return -1;
					}
					else//�����и��Ӳ����������ܸ�ʽ�Ƿ�
					{
						if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_default||
							args[j].extargs_type == ST_EXTARGS_TYPE::int_with_error||
							args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_default||
							args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
						{
							if (judge_str_num(argv[i + 1]) == 0)//������Ǵ�����
							{
								cout << "����[" << argv[i] << "]�ĸ��Ӳ�����������. (����:int, ";
								
								if(args[j].extargs_type == ST_EXTARGS_TYPE::int_with_default)
									cout<< "��Χ[" << args[j].extargs_int_min << ".." << args[j].extargs_int_max << "] ȱʡ:" <<
									args[j].extargs_int_default << ")" << endl;
								else if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_error)
								{
									cout<< "��Χ[" << args[j].extargs_int_min << ".." << args[j].extargs_int_max << "])" << endl;
								}
								else if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_default)
								{
									cout<<"��ȡֵ[";
									for (int k = 0; k < args[j].extargs_int_set_num; k++)
									{
										cout << args[j].extargs_int_set[k];
										if (k != args[j].extargs_int_set_num - 1)
											cout << "/";
									}
									cout << "] ȱʡ:" << args[j].extargs_int_default << ")" << endl;
								}
								else if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
								{
									cout<<"��ȡֵ[";
									for (int k = 0; k < args[j].extargs_int_set_num; k++)
									{
									cout << args[j].extargs_int_set[k];
									if (k != args[j].extargs_int_set_num - 1)
										cout << "/";
									}
									cout << "])" << endl;
								}

								return -1;
							}
							/*��֤�Ǵ����ֺ�*/
							int n = atoi(argv[i + 1]);

							if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_default ||
								args[j].extargs_type == ST_EXTARGS_TYPE::int_with_error)
							{
								if (n >= args[j].extargs_int_min && n <= args[j].extargs_int_max)//�ڷ�Χ��
									args[j].extargs_int_value = n;
								else//���ڷ�Χ��
								{
									if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_default)//��Ĭ��ֵ
										args[j].extargs_int_value = args[j].extargs_int_default;
									else if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_error)//û��Ĭ��ֵ
									{
										cout << "����[" << argv[i] << "]�ĸ��Ӳ���ֵ(" << argv[i + 1] << ")�Ƿ�. (����:int, ��Χ[" <<
											args[j].extargs_int_min << ".." << args[j].extargs_int_max << "])" << endl;
										return -1;
									}
								}
							}
							else if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_default ||
								args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
							{
								if (judge_in_set_int(args[j].extargs_int_set, args[j].extargs_int_set_num, n) == 0)//û���ҵ�
								{
									if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_default)//��Ĭ��ֵ
										args[j].extargs_int_value = args[j].extargs_int_default;
									else if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_error)//��Ĭ��ֵ
									{
										cout << "����[" << argv[i] << "]�ĸ��Ӳ���ֵ(" << argv[i + 1] << ")�Ƿ�. (����:int, ��ȡֵ[";
										for (int k = 0; k < args[j].extargs_int_set_num; k++)
										{
											cout << args[j].extargs_int_set[k];
											if (k != args[j].extargs_int_set_num - 1)
												cout << "/";
										}
										cout << "])" << endl;

										return -1;
									}
								}
								else//�ҵ���
								{
									args[j].extargs_int_value = n;
								}
							}

						}//end of int type
						else if (args[j].extargs_type == ST_EXTARGS_TYPE::str)//str�����û��ʲô�Ƿ��ĵط�
						{
						args[j].extargs_string_value = argv[i + 1];
						}
						else if (args[j].extargs_type == ST_EXTARGS_TYPE::str_with_set_default ||
						args[j].extargs_type == ST_EXTARGS_TYPE::str_with_set_error)
						{
						if (judge_in_set_str(args[j].extargs_string_set, args[j].extargs_string_set_num, argv[i + 1]) == 0)//���set��û��
						{
							if (args[j].extargs_type == ST_EXTARGS_TYPE::str_with_set_default)//��Ĭ��ֵ
								args[j].extargs_string_value = args[j].extargs_string_default;
							else if (args[j].extargs_type == ST_EXTARGS_TYPE::str_with_set_error)//ֱ�ӷ��ش���
							{
								cout << "����[" << argv[i] << "]�ĸ��Ӳ���ֵ(" << argv[i + 1] << ")�Ƿ�. (����:string, ��ȡֵ[";
								for (int k = 0; k < args[j].extargs_string_set_num; k++)
								{
									cout << args[j].extargs_string_set[k];
									if (k != args[j].extargs_string_set_num - 1)
										cout << "/";
								}
								cout << "])" << endl;

								return -1;
							}
						}
						else//���set����
							args[j].extargs_string_value = argv[i + 1];
						}
						else if (args[j].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_default ||
						args[j].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_error)
						{
						bool is_right;
						unsigned int ip_temp = str_ip_to_uint_ip(argv[i + 1], is_right);
						if (is_right == 1)//ip��ȷ
						{
							args[j].extargs_ipaddr_value = ip_temp;
						}
						else//ip����
						{
							if (args[j].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_default)
								args[j].extargs_ipaddr_value = args[j].extargs_ipaddr_default;
							else if (args[j].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_error)
								{
									cout << "����[" << argv[i] << "]�ĸ��Ӳ���ֵ(" << argv[i + 1] << ")�Ƿ�. (����:IP��ַ)" << endl;
									
									return -1;
								}
							}
						}
					}

					i += 1 + args[j].extargs_num;
				}
			}
			else//����Ѿ����ֹ�
			{
				cout << "����[" << argv[i] << "]�ظ�." << endl;
				return -1;
			}
		}
		else//ǰ���--��ͷ
		{
			if (follow_up_args == 0)//�������Ҫ����������
			{
				cout << "����[" << argv[i] << "]��ʽ�Ƿ�" << endl;
				return -1;
			}
			else
				i++;
		}
	}
	/*δ���ֲ�����Ĭ��ֵ*/
	for (int k = 0; args[k].extargs_type != ST_EXTARGS_TYPE::null; k++)
	{
		if (args[k].existed()==0)//δ����
		{
			if (args[k].extargs_type == ST_EXTARGS_TYPE::int_with_default
				|| args[k].extargs_type == ST_EXTARGS_TYPE::int_with_set_default)
				args[k].extargs_int_value = args[k].extargs_int_default;
			else if (args[k].extargs_type == ST_EXTARGS_TYPE::str ||
				args[k].extargs_type == ST_EXTARGS_TYPE::str_with_set_default)
				args[k].extargs_string_value = args[k].extargs_string_default;
			else if (args[k].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_default)
				args[k].extargs_ipaddr_value = args[k].extargs_ipaddr_default;
		}
	}

	return 0; //�˾������Ҫ�޸�
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������Ԫ����
***************************************************************************/
int args_analyse_print(const args_analyse_tools* const args)
{
	int name_maxlen = 0;
	for (int n = 0; args[n].extargs_type != ST_EXTARGS_TYPE::null; n++)//��������󳤶�
	{
		if ((int)args[n].get_name().length() > name_maxlen)
			name_maxlen = (int)args[n].get_name().length();
	}

	if (name_maxlen < 4)
		name_maxlen = 4;

	cout << setw(101+name_maxlen) << setfill('=') << "=" << endl;
	cout << setfill(' ') << setiosflags(ios::left) << setw(name_maxlen+1) << "name";
	cout << setw(22) << "type" << setw(17) << "default" << setw(7) << "exists" << setw(17) << "value" << "range/set" << endl;
	cout << setw(101 + name_maxlen) << setfill('=') << "=" << endl;

	for (int k = 0; args[k].extargs_type != ST_EXTARGS_TYPE::null; k++)
	{
		cout << setfill(' ') << setw(name_maxlen + 1) << args[k].get_name();
		cout << setw(22) << str_type(args[k].extargs_type);
		
		/*��ӡdefault*/
		if (args[k].extargs_type == ST_EXTARGS_TYPE::boolean)
		{
			if (args[k].extargs_bool_default == 0)
				cout << setw(17) << "false";
			else
				cout << setw(17) << "true";
		}
		else if (args[k].extargs_type == ST_EXTARGS_TYPE::int_with_default ||
			args[k].extargs_type == ST_EXTARGS_TYPE::int_with_set_default)
		{
			cout << setw(17) << args[k].extargs_int_default;
		}
		else if (args[k].extargs_type == ST_EXTARGS_TYPE::str_with_set_default)
		{
			cout << setw(17) << args[k].extargs_string_default;
		}
		else if (args[k].extargs_type == ST_EXTARGS_TYPE::str)
		{
			if (args[k].extargs_string_default != "")
				cout << setw(17) << args[k].extargs_string_default;
			else
				cout << setw(17) << "/";
		}
		else if (args[k].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_default)
		{
			cout << setw(17) << ip_uint_to_str(args[k].extargs_ipaddr_default);
		}
		else
			cout << setw(17) << "/";
		/*default�������*/

		/*��ʼ���exist*/
		cout << setw(7) << args[k].existed();
		/*���value*/
		if (args[k].existed())//�������
		{
			if (args[k].extargs_type == ST_EXTARGS_TYPE::boolean)
			{
				cout << setw(17) << "true";//���ھ���true	
			}
			else if (args[k].extargs_type == ST_EXTARGS_TYPE::int_with_default ||
				args[k].extargs_type == ST_EXTARGS_TYPE::int_with_error ||
				args[k].extargs_type == ST_EXTARGS_TYPE::int_with_set_default ||
				args[k].extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
			{
				cout << setw(17) << args[k].extargs_int_value;
			}
			else if (args[k].extargs_type == ST_EXTARGS_TYPE::str ||
				args[k].extargs_type == ST_EXTARGS_TYPE::str_with_set_default ||
				args[k].extargs_type == ST_EXTARGS_TYPE::str_with_set_error)
				cout << setw(17) << args[k].extargs_string_value;
			else if (args[k].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_default ||
				args[k].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_error)
				cout << setw(17) << args[k].get_str_ipaddr();
		}
		else//û��exist��
			cout << setw(17) << "/";
		/*value�������*/

		/*���range/set*/
		if (args[k].extargs_type == ST_EXTARGS_TYPE::boolean ||
			args[k].extargs_type == ST_EXTARGS_TYPE::str ||
			args[k].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_default ||
			args[k].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_error)
			cout << "/";
		else if (args[k].extargs_type == ST_EXTARGS_TYPE::int_with_default ||
				 args[k].extargs_type == ST_EXTARGS_TYPE::int_with_error)
			cout << "[" << args[k].extargs_int_min << ".." << args[k].extargs_int_max << "]";
		else if (args[k].extargs_type == ST_EXTARGS_TYPE::int_with_set_default ||
			     args[k].extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
		{
			for (int m = 0; m < args[k].extargs_int_set_num; m++)
			{
				cout << args[k].extargs_int_set[m];
				if (m != args[k].extargs_int_set_num - 1)
					cout << "/";
			}
		}
		else if (args[k].extargs_type == ST_EXTARGS_TYPE::str_with_set_default ||
				 args[k].extargs_type == ST_EXTARGS_TYPE::str_with_set_error)
		{
			for (int m = 0; m < args[k].extargs_string_set_num; m++)
			{
				cout << args[k].extargs_string_set[m];
				if (m != args[k].extargs_string_set_num - 1)
					cout << "/";
			}
		}
		cout << endl;
	}
	cout << setw(101 + name_maxlen) << setfill('=') << "=" << endl;
	cout << setfill(' ') << endl;
	

	return 0; //�˾������Ҫ�޸�
}

#endif // !ENABLE_LIB_AAT_TOOLS
