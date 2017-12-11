
#include "includes.h"
#include "sys.h"
#include "tftlcd.h"
#include "ltdc.h"
#include "led.h"
#include "adc.h"
#include "key.h"
#include "key_exti.h"
#include "gizwits_product.h" 
#include "Gizwits_user.h"
#include "task.h"
#include "osTimer.h"
#include "os_app_hooks.h"
#include <stdio.h>
#include <string.h>
#include "rtc.h"

#define LIMITE(x,min,max) if(x<min) x=min;if(x>max) x=max;

extern dataPoint_t currentDataPoint;

OS_TMR 	tmr1;		//��ʱ��1����ʱ1S
OS_TMR	tmr2;		//��ʱ��2����ʱ2S

OS_SEM  mysem;      //�ź���
OS_MUTEX mymutex;   //�����ź���

u16 task1_num=0,task2_num=0;

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

#if OS_CFG_APP_HOOKS_EN //ʹ�ù��Ӻ���
	App_OS_SetAllHooks(); 
#endif
	
	// os_timer ---
	//������ʱ��1
	OSTmrCreate((OS_TMR		*)&tmr1,		//��ʱ��1
                (CPU_CHAR	*)"tmr1",		//��ʱ������
                (OS_TICK	 )100,			//20*10=200ms
                (OS_TICK	 )100,          //100*10=1000ms=1s
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //����ģʽ
                (OS_TMR_CALLBACK_PTR)tmr1_callback,//��ʱ��1�ص�����
                (void	    *)0,			//����Ϊ0
                (OS_ERR	    *)&err);		//���صĴ�����					
	//������ʱ��2
	OSTmrCreate((OS_TMR		*)&tmr2,		
                (CPU_CHAR	*)"tmr2",		
                (OS_TICK	 )200,			//200*10=2000ms=2s
                (OS_TICK	 )200,   					
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, 	//���ζ�ʱ��
                (OS_TMR_CALLBACK_PTR)tmr2_callback,	//��ʱ��2�ص�����
                (void	    *)0,			
                (OS_ERR	    *)&err);

	//semaphore ----
	OSSemCreate((OS_SEM *)&mysem,      //�ź���
				(CPU_CHAR *)"mysem",   //�ź�����
				(OS_SEM_CTR)1,  	   //�����ź����ĳ�ʼֵ�������ֵΪ 1��������ź���Ϊ�������ź������������1�Ļ��ʹ�����ź���Ϊ�������ź���
				(OS_ERR *)&err);	   //���صĴ�����	
				
	OSMutexCreate((OS_MUTEX *)&mymutex,
				  (CPU_CHAR *)"mymutex",
				  (OS_ERR *)&err);	   //���صĴ�����	
	
	// task ---
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
								 
	//����TASK4����
	OSTaskCreate((OS_TCB 	* )&Task4_TaskTCB,		
				 (CPU_CHAR	* )"task4 task", 		
                 (OS_TASK_PTR )task4_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )TASK4_TASK_PRIO,     	
                 (CPU_STK   * )&TASK4_TASK_STK[0],	
                 (CPU_STK_SIZE)TASK4_STK_SIZE/10,	
                 (CPU_STK_SIZE)TASK4_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,	//ʱ��Ƭ����10ms						
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, 
                 (OS_ERR 	* )&err);	
								 
	OS_CRITICAL_EXIT();	//�˳��ٽ���
								 
	OSTmrStart(&tmr1,&err);	//������ʱ��1
	OSTmrStart(&tmr2,&err);	//������ʱ��2		
	//OSTmrStop(&tmr1,OS_OPT_TMR_NONE,0,&err);	//�رն�ʱ��1
	//OSTmrStop(&tmr2,OS_OPT_TMR_NONE,0,&err);	//�رն�ʱ��2
	
	OSTaskDel((OS_TCB*)0,&err);	//ɾ��start_task��������
	
}


//task1������
void task1_task(void *p_arg)
{
	short temp; //�¶�
	unsigned char hour,minute,second;
	OS_ERR err;
	p_arg = p_arg;
	 
	POINT_COLOR = RED;
	LCD_ShowString(30,130,200,16,16,"TEMPERATE: 00.00C");//���ڹ̶�λ����ʾС����	 
	LCD_ShowString(30,150,110,16,16,"Task1 Run:00000");
	LCD_ShowString(30,170,110,16,16,"Run Time :00:00:00");
	POINT_COLOR = BLUE;
	while(1)
	{
		//OSSemPend(&mysem,0,OS_OPT_PEND_BLOCKING,0,&err); // �����ź���
		OSMutexPend(&mymutex,0,OS_OPT_PEND_BLOCKING,0,&err);//���󻥳��ź���
		OSTaskSemPost(&Task2_TaskTCB,OS_OPT_POST_NONE,&err); // ʹ��ϵͳ�ڽ��ź��������� task2  �����ź���

		temp=Get_Temprate();	//�õ��¶�ֵ 
		if(temp<0)
		{
			temp=-temp;
			LCD_ShowString(30+10*8,130,16,16,16,"-");	    //��ʾ����
		}else LCD_ShowString(30+10*8,130,16,16,16," ");		//�޷���
		LCD_ShowxNum(30+11*8,130,temp/100,2,16,0);			//��ʾ��������
		LCD_ShowxNum(30+14*8,130,temp%100,2,16,0);			//��ʾС������ 

		hour=task1_num/3600;
		minute=(task1_num-hour*3600)/60;
		second=(task1_num-hour*3600)%60;
		
		LCD_ShowxNum(30+10*8,150,task1_num,5,16,0x80);	//��ʾ����ִ�д���
	
		LCD_ShowxNum(30+10*8,170,hour,2,16,0x80);	   //��ʾ����ִ��ʱ��
		POINT_COLOR=RED;LCD_ShowString(30+12*8,170,110,16,16,":");POINT_COLOR=BLUE;
		LCD_ShowxNum(30+13*8,170,minute,2,16,0x80);
		POINT_COLOR=RED;LCD_ShowString(30+15*8,170,110,16,16,":");POINT_COLOR=BLUE;
		LCD_ShowxNum(30+16*8,170,second,2,16,0x80);
		printf("Time:  %d��%d��%d \r\n",hour,minute,second);
		printf("Task1: %d\r\n",task1_num);
		OSMutexPost(&mymutex,OS_OPT_POST_NONE,&err);
		//OSSemPost(&mysem,OS_OPT_POST_ALL,&err);//��ʱ�ж��з����ź���
		//OSSched();
		//LED0_Toggle;
		OSTimeDlyHMSM(0,0,0,1000,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ1s
		
	}
}

//task2������
void task2_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	
	POINT_COLOR = RED;
	LCD_ShowString(30,250,110,16,16,"Task2 Run:00000");
	POINT_COLOR = BLUE;
	while(1)
	{
		//OSSemPend(&mysem,0,OS_OPT_PEND_BLOCKING,0,&err); // �����ź���
		OSMutexPend(&mymutex,0,OS_OPT_PEND_BLOCKING,0,&err);//���󻥳��ź���
		OSTaskSemPost(&Task1_TaskTCB,OS_OPT_POST_NONE,&err); // ʹ��ϵͳ�ڽ��ź��������� task1  �����ź���
		
		LCD_ShowxNum(30+10*8,250,task2_num,5,16,0x80);  //��ʾ����ִ�д���
		printf("Task2: %d\r\n",task2_num);
		OSMutexPost(&mymutex,OS_OPT_POST_NONE,&err);
		//OSSemPost(&mysem,OS_OPT_POST_ALL,&err);//��ʱ�ж��з����ź���
		//OSSched();
		//LED1_Toggle;
		OSTimeDlyHMSM(0,0,0,1000,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ2s
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
//

//task4������
void task4_task(void *p_arg)
{
	u16 task4_num=0;
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	u8 tbuf[40];
	OS_ERR err;
	p_arg = p_arg;
	
	POINT_COLOR = RED;
	LCD_ShowString(30,310,110,16,16,"Task4 Run:00000");
	POINT_COLOR = BLUE;
	while(1)
	{
		task4_num++;
		LCD_ShowxNum(30+10*8,310,task4_num,5,16,0x80);  //��ʾ����ִ�д���
		printf("Task4: %d\r\n",task4_num);
		
		HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStruct,RTC_FORMAT_BIN);
		sprintf((char*)tbuf,"Time:%02d:%02d:%02d",RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds); 
		LCD_ShowString(30,330,210,16,16,tbuf);	
		HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStruct,RTC_FORMAT_BIN);
		sprintf((char*)tbuf,"Date:20%02d-%02d-%02d",RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date); 
		LCD_ShowString(30,350,210,16,16,tbuf);	
		sprintf((char*)tbuf,"Week:%d",RTC_DateStruct.WeekDay); 
		LCD_ShowString(30,370,210,16,16,tbuf);
		
		OSTimeDlyHMSM(0,0,0,1000,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ1s
	}
}





