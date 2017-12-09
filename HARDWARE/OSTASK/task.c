
#include "includes.h"
#include "sys.h"
#include "tftlcd.h"
#include "ltdc.h"
#include "led.h"
#include "key.h"
#include "key_exti.h"
#include "gizwits_product.h" 
#include "Gizwits_user.h"
#include "task.h"
#include "osTimer.h"

extern dataPoint_t currentDataPoint;

OS_TMR 	tmr1;		//定时器1
OS_TMR	tmr2;		//定时器2
u16 task1_num=0,task2_num=0;

//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	
	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,设置默认的时间片长度
	OSSchedRoundRobinCfg(DEF_ENABLED,5,&err);  
#endif		
	
	// os_timer ---
	//创建定时器1
	OSTmrCreate((OS_TMR		*)&tmr1,		//定时器1
                (CPU_CHAR	*)"tmr1",		//定时器名字
                (OS_TICK	 )100,			//20*10=200ms
                (OS_TICK	 )100,          //100*10=1000ms=1s
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //周期模式
                (OS_TMR_CALLBACK_PTR)tmr1_callback,//定时器1回调函数
                (void	    *)0,			//参数为0
                (OS_ERR	    *)&err);		//返回的错误码					
	//创建定时器2
	OSTmrCreate((OS_TMR		*)&tmr2,		
                (CPU_CHAR	*)"tmr2",		
                (OS_TICK	 )200,			//200*10=2000ms=2s
                (OS_TICK	 )200,   					
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, 	//单次定时器
                (OS_TMR_CALLBACK_PTR)tmr2_callback,	//定时器2回调函数
                (void	    *)0,			
                (OS_ERR	    *)&err);
	
	// task ---
	OS_CRITICAL_ENTER();	//进入临界区
	//创建TASK1任务
	OSTaskCreate((OS_TCB 	* )&Task1_TaskTCB,		
				 (CPU_CHAR	* )"Task1 task", 		
                 (OS_TASK_PTR )task1_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )TASK1_TASK_PRIO,     
                 (CPU_STK   * )&TASK1_TASK_STK[0],	
                 (CPU_STK_SIZE)TASK1_STK_SIZE/10,	
                 (CPU_STK_SIZE)TASK1_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  //时间片长度10ms					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP,
                 (OS_ERR 	* )&err);				
				 
	//创建TASK2任务
	OSTaskCreate((OS_TCB 	* )&Task2_TaskTCB,		
				 (CPU_CHAR	* )"task2 task", 		
                 (OS_TASK_PTR )task2_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )TASK2_TASK_PRIO,     	
                 (CPU_STK   * )&TASK2_TASK_STK[0],	
                 (CPU_STK_SIZE)TASK2_STK_SIZE/10,	
                 (CPU_STK_SIZE)TASK2_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,	//时间片长度10ms						
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, 
                 (OS_ERR 	* )&err);	
//	//创建TASK3任务 会使系统死机   int32_t gizPutData(uint8_t *buf, uint32_t len) 中出错
//	OSTaskCreate((OS_TCB 	* )&Task3_TaskTCB,		
//				 (CPU_CHAR	* )"task3 task", 		
//                 (OS_TASK_PTR )task3_task, 			
//                 (void		* )0,					
//                 (OS_PRIO	  )TASK3_TASK_PRIO,     	
//                 (CPU_STK   * )&TASK3_TASK_STK[0],	
//                 (CPU_STK_SIZE)TASK3_STK_SIZE/10,	
//                 (CPU_STK_SIZE)TASK3_STK_SIZE,		
//                 (OS_MSG_QTY  )0,					
//                 (OS_TICK	  )0,	//时间片长度10ms						
//                 (void   	* )0,				
//                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, 
//                 (OS_ERR 	* )&err);
								 					 
	OS_CRITICAL_EXIT();	//退出临界区
								 
	OSTmrStart(&tmr1,&err);	//开启定时器1
	OSTmrStart(&tmr2,&err);	//开启定时器2		
	//OSTmrStop(&tmr1,OS_OPT_TMR_NONE,0,&err);	//关闭定时器1
	//OSTmrStop(&tmr2,OS_OPT_TMR_NONE,0,&err);	//关闭定时器2
	
	OSTaskDel((OS_TCB*)0,&err);	//删除start_task任务自身
	
}


//task1任务函数
void task1_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	 
	POINT_COLOR = RED;
	LCD_ShowString(30,130,110,16,16,"Task1 Run:000");
	POINT_COLOR = BLUE;
	while(1)
	{
		LCD_ShowxNum(110,130,task1_num,5,16,0x80);	//显示任务执行次数
		printf("Task1: %d\r\n",task1_num);
		//LED0_Toggle;
		OSTimeDlyHMSM(0,0,0,1000,OS_OPT_TIME_HMSM_STRICT,&err); //延时1s
	}
}

//task2任务函数
void task2_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	
	POINT_COLOR = RED;
	LCD_ShowString(30,150,110,16,16,"Task2 Run:000");
	POINT_COLOR = BLUE;
	while(1)
	{
		LCD_ShowxNum(110,150,task2_num,5,16,0x80);  //显示任务执行次数
		printf("Task2: %d\r\n",task2_num);
		//LED1_Toggle;
		OSTimeDlyHMSM(0,0,0,2000,OS_OPT_TIME_HMSM_STRICT,&err); //延时1s
	}
}

//task3任务函数 
void task3_task(void *p_arg)
{
	u8 task3_num=0;
	u8 key;
	OS_ERR err;
	p_arg = p_arg;
	
	POINT_COLOR = RED;
	LCD_ShowString(30,170,110,16,16,"Task3 Run:000");
	POINT_COLOR = BLUE;
	//OSTaskSuspend((OS_TCB *)&Task1_TaskTCB,&err);	//任务1挂起
	//OSTaskSuspend((OS_TCB *)&Task2_TaskTCB,&err);	//任务2挂起
	
	 while(1)
    {
		task3_num++;	//任务3执行次数
		LCD_ShowxNum(110,170,task3_num,3,16,0x80);  //显示任务执行次数
		printf("Task3: %d\r\n",task3_num);
		
    userHandle();//用户采集
         
    gizwitsHandle((dataPoint_t *)&currentDataPoint);//协议处理
 		
	    key = KEY_Scan(0);
		if(key==KEY1_PRES)//KEY1按键
		{
			//OSTaskSuspend((OS_TCB *)&Task1_TaskTCB,&err);	//任务1挂起
			//OSTaskSuspend((OS_TCB *)&Task2_TaskTCB,&err);	//任务2挂起
			
			printf("WIFI进入AirLink连接模式\r\n");
			gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-link模式接入
			
			//OSTaskResume((OS_TCB *)&Task1_TaskTCB,&err);	//任务1恢复
			//OSTaskResume((OS_TCB *)&Task2_TaskTCB,&err);	//任务2恢复
		}			
		if(key==WKUP_PRES)//KEY_UP按键
		{  
			printf("WIFI复位，请重新配置连接\r\n");
			gizwitsSetMode(WIFI_RESET_MODE);//WIFI复位
		}      
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时1s
		//LED0_Toggle;
	}  
}
//







