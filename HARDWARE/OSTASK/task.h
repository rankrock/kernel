#ifndef _TASK_H
#define _TASK_H

#include "includes.h"

//task1
#define TASK1_TASK_PRIO		5			//�������ȼ�
#define TASK1_STK_SIZE 		128			//�����ջ��С
OS_TCB Task1_TaskTCB;					//������ƿ�
CPU_STK TASK1_TASK_STK[TASK1_STK_SIZE]; //�����ջ	
void task1_task(void *p_arg);


//task2
#define TASK2_TASK_PRIO		5            //�������ȼ�
#define TASK2_STK_SIZE 		128          //�����ջ��С	
OS_TCB Task2_TaskTCB;                    //������ƿ�
CPU_STK TASK2_TASK_STK[TASK2_STK_SIZE];	 //�����ջ	
void task2_task(void *p_arg);            //������


//task3
#define TASK3_TASK_PRIO		4            //�������ȼ�
#define TASK3_STK_SIZE 		128          //�����ջ��С	
OS_TCB Task3_TaskTCB;                    //������ƿ�
CPU_STK TASK3_TASK_STK[TASK3_STK_SIZE];	 //�����ջ	
void task3_task(void *p_arg);            //������

//task4
#define TASK4_TASK_PRIO		6            //�������ȼ�
#define TASK4_STK_SIZE 		128          //�����ջ��С	
OS_TCB Task4_TaskTCB;                    //������ƿ�
CPU_STK TASK4_TASK_STK[TASK4_STK_SIZE];	 //�����ջ	
void task4_task(void *p_arg);            //������

#endif
