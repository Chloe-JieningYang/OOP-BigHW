/*2050598 ����� ͨ��*/
#pragma once
#include <iostream>
using namespace std;

/*��ȡposition.saving���¼��λ��*/
streampos GetPosSaving(string bookname);

/*��¼position.saving*/
void WritePos(const string bookname, const streampos pos);