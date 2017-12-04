

#include "task.h"
#include "includes.h"
#include "sys.h"
#include "tftlcd.h"
#include "ltdc.h"
#include "led.h"
#include "key.h"
#include "key_exti.h"
#include "gizwits_product.h" 
#include "Gizwits_user.h"

extern dataPoint_t currentDataPoint;


//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	
	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,����Ĭ�ϵ�ʱ��Ƭ����
	OSSchedRoundRobinCfg(DEF_ENABLED,5,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//�����ٽ���
	//����TASK1����
	OSTaskCreate((OS_TCB 	* )&Task1_TaskTCB,		
				 (CPU_CHAR	* )"Task1 task", 		
                 (OS_TASK_PTR )task1_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )TASK1_TASK_PRIO,     
                 (CPU_STK   * )&TASK1_TASK_STK[0],	
                 (CPU_STK_SIZE)TASK1_STK_SIZE/10,	
                 (CPU_STK_SIZE)TASK1_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  //ʱ��Ƭ����10ms					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP,
                 (OS_ERR 	* )&err);				
				 
	//����TASK2����
	OSTaskCreate((OS_TCB 	* )&Task2_TaskTCB,		
				 (CPU_CHAR	* )"task2 task", 		
                 (OS_TASK_PTR )task2_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )TASK2_TASK_PRIO,     	
                 (CPU_STK   * )&TASK2_TASK_STK[0],	
                 (CPU_STK_SIZE)TASK2_STK_SIZE/10,	
                 (CPU_STK_SIZE)TASK2_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,	//ʱ��Ƭ����10ms						
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, 
                 (OS_ERR 	* )&err);	
//	//����TASK3���� ��ʹϵͳ����   int32_t gizPutData(uint8_t *buf, uint32_t len) �г���
//	OSTaskCreate((OS_TCB 	* )&Task3_TaskTCB,		
//				 (CPU_CHAR	* )"task3 task", 		
//                 (OS_TASK_PTR )task3_task, 			
//                 (void		* )0,					
//                 (OS_PRIO	  )TASK3_TASK_PRIO,     	
//                 (CPU_STK   * )&TASK3_TASK_STK[0],	
//                 (CPU_STK_SIZE)TASK3_STK_SIZE/10,	
//                 (CPU_STK_SIZE)TASK3_STK_SIZE,		
//                 (OS_MSG_QTY  )0,					
//                 (OS_TICK	  )0,	//ʱ��Ƭ����10ms						
//                 (void   	* )0,				
//                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, 
//                 (OS_ERR 	* )&err);
	OS_CRITICAL_EXIT();	//�˳��ٽ���
	OSTaskDel((OS_TCB*)0,&err);	//ɾ��start_task��������
}


//task1������
void task1_task(void *p_arg)
{
	u8 task1_num=0;
	OS_ERR err;
	p_arg = p_arg;
	 
	POINT_COLOR = RED;
	LCD_ShowString(30,130,110,16,16,"Task1 Run:000");
	POINT_COLOR = BLUE;
	while(1)
	{
		task1_num++;	//����1ִ�д�����1 ע��task1_num1�ӵ�255��ʱ������㣡��
		LCD_ShowxNum(110,130,task1_num,3,16,0x80);	//��ʾ����ִ�д���
		//for(i=0;i<10;i++) 
		printf("Task1: %d\r\n",task1_num);
		//LED0_Toggle;
		OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ1s
	}
}

//task2������
void task2_task(void *p_arg)
{
	u8 task2_num=0;
	OS_ERR err;
	p_arg = p_arg;
	
	POINT_COLOR = RED;
	LCD_ShowString(30,150,110,16,16,"Task2 Run:000");
	POINT_COLOR = BLUE;
	while(1)
	{
		task2_num++;	//����2ִ�д�����1 ע��task1_num2�ӵ�255��ʱ������㣡��
		LCD_ShowxNum(110,150,task2_num,3,16,0x80);  //��ʾ����ִ�д���
		//for(i=0;i<20;i++) 
		printf("Task2: %d\r\n",task2_num);
		//LED1_Toggle;
		OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ1s
	}
}

//task3������ 
void task3_task(void *p_arg)
{
	u8 task3_num=0;
	u8 key;
	OS_ERR err;
	p_arg = p_arg;
	
	POINT_COLOR = RED;
	LCD_ShowString(30,170,110,16,16,"Task3 Run:000");
	POINT_COLOR = BLUE;
	//OSTaskSuspend((OS_TCB *)&Task1_TaskTCB,&err);	//����1����
	//OSTaskSuspend((OS_TCB *)&Task2_TaskTCB,&err);	//����2����
	
	 while(1)
    {
		task3_num++;	//����3ִ�д���
		LCD_ShowxNum(110,170,task3_num,3,16,0x80);  //��ʾ����ִ�д���
		printf("Task3: %d\r\n",task3_num);
		
    userHandle();//�û��ɼ�
         
    gizwitsHandle((dataPoint_t *)&currentDataPoint);//Э�鴦��
 		
	    key = KEY_Scan(0);
		if(key==KEY1_PRES)//KEY1����
		{
			//OSTaskSuspend((OS_TCB *)&Task1_TaskTCB,&err);	//����1����
			//OSTaskSuspend((OS_TCB *)&Task2_TaskTCB,&err);	//����2����
			
			printf("WIFI����AirLink����ģʽ\r\n");
			gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-linkģʽ����
			
			//OSTaskResume((OS_TCB *)&Task1_TaskTCB,&err);	//����1�ָ�
			//OSTaskResume((OS_TCB *)&Task2_TaskTCB,&err);	//����2�ָ�
		}			
		if(key==WKUP_PRES)//KEY_UP����
		{  
			printf("WIFI��λ����������������\r\n");
			gizwitsSetMode(WIFI_RESET_MODE);//WIFI��λ
		}      
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ1s
		//LED0_Toggle;
	}  
}

