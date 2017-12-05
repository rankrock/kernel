#ifndef _OSTIMER_H
#define _OSTIMER_H

#include "includes.h"



extern OS_TMR 	tmr1;		//定时器1
extern OS_TMR	tmr2;		//定时器2

void tmr1_callback(void *p_tmr, void *p_arg); 	//定时器1回调函数
void tmr2_callback(void *p_tmr, void *p_arg);	//定时器2回调函数



#endif

