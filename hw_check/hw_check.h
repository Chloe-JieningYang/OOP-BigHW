/* 2050598 ����� ͨ�� */
#pragma once

/* �����Լ���Ҫ�Ķ��� */

#define FAIL_OPEN        -1

#define SINGLE_NOTE      1
#define MULTIPLE_NOTE    2
#define ERROR_NOTE       3//����//��ͷ����ͬһ��/*

#define SPECIAL_TYPE     4//���� ָ/**/����


struct partner {
	int no;
	char name[40] = { '\0' };
};

struct student {
	int class_no;//�κ�
	int no;//ѧ��
	char name[33] = { '\0' };//����
	int g_ok;//ԭʼ����b2)����ȷ��
	int c_ok;//������(b3)��ȷ��
	int c_de_point1 = 0;//������۷�1
	int c_de_point2 = 0;//������۷�2
	int partner_num = 0;//���������
	partner check_group[20];//������Ż���ͬѧ����Ϣ
};