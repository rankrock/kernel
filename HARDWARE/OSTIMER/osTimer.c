

#include "includes.h"
#include "sys.h"

//extern u16 task1_num,task2_num;

//定时器1的回调函数
void tmr1_callback(void *p_tmr, void *p_arg)
{
	extern u16 task1_num;
	task1_num++;
}

//定时器2的回调函数
void tmr2_callback(void *p_tmr,void *p_arg)
{
	extern u16 task2_num;
	task2_num++;
}

