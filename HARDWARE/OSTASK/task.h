#ifndef _TASK_H
#define _TASK_H

#include "includes.h"

//task1
#define TASK1_TASK_PRIO		5			//任务优先级
#define TASK1_STK_SIZE 		128			//任务堆栈大小
OS_TCB Task1_TaskTCB;					//任务控制块
CPU_STK TASK1_TASK_STK[TASK1_STK_SIZE]; //任务堆栈	
void task1_task(void *p_arg);


//task2
#define TASK2_TASK_PRIO		5            //任务优先级
#define TASK2_STK_SIZE 		128          //任务堆栈大小	
OS_TCB Task2_TaskTCB;                    //任务控制块
CPU_STK TASK2_TASK_STK[TASK2_STK_SIZE];	 //任务堆栈	
void task2_task(void *p_arg);            //任务函数


//task3
#define TASK3_TASK_PRIO		4            //任务优先级
#define TASK3_STK_SIZE 		128          //任务堆栈大小	
OS_TCB Task3_TaskTCB;                    //任务控制块
CPU_STK TASK3_TASK_STK[TASK3_STK_SIZE];	 //任务堆栈	
void task3_task(void *p_arg);            //任务函数

//task4
#define TASK4_TASK_PRIO		6            //任务优先级
#define TASK4_STK_SIZE 		128          //任务堆栈大小	
OS_TCB Task4_TaskTCB;                    //任务控制块
CPU_STK TASK4_TASK_STK[TASK4_STK_SIZE];	 //任务堆栈	
void task4_task(void *p_arg);            //任务函数

#endif
