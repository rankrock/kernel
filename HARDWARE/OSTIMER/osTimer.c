

#include "includes.h"
#include "sys.h"

//extern u16 task1_num,task2_num;

//��ʱ��1�Ļص�����
void tmr1_callback(void *p_tmr, void *p_arg)
{
	extern u16 task1_num;
	task1_num++;
}

//��ʱ��2�Ļص�����
void tmr2_callback(void *p_tmr,void *p_arg)
{
	extern u16 task2_num;
	task2_num++;
}

