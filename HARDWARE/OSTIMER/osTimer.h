#ifndef _OSTIMER_H
#define _OSTIMER_H

#include "includes.h"



extern OS_TMR 	tmr1;		//��ʱ��1
extern OS_TMR	tmr2;		//��ʱ��2

void tmr1_callback(void *p_tmr, void *p_arg); 	//��ʱ��1�ص�����
void tmr2_callback(void *p_tmr, void *p_arg);	//��ʱ��2�ص�����



#endif

