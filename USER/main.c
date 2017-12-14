#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "usmart.h"
#include "key.h"
#include "key_exti.h"
#include "led.h"
#include "adc.h"
#include "tftlcd.h"
#include "ltdc.h"
#include "sdram.h"
#include "mpu.h"
#include "includes.h"
#include "timer.h"
#include "rtc.h"
//#include "gizwits_product.h" 
#include "Gizwits_user.h"

/************************************************
 ALIENTEK 阿波罗STM32F7开发板 UCOSIII实验
 例6-3 UCOSIII 时间片轮转调度
 
 UCOSIII中以下优先级用户程序不能使用，ALIENTEK
 将这些优先级分配给了UCOSIII的5个系统内部任务
 优先级0：中断服务服务管理任务 OS_IntQTask()
 优先级1：时钟节拍任务 OS_TickTask()
 优先级2：定时任务 OS_TmrTask()
 优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
 优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()

************************************************/

//task start
#define START_TASK_PRIO		3			//任务优先级
#define START_STK_SIZE 		128			//任务堆栈大小	
OS_TCB StartTaskTCB;					//任务控制块
CPU_STK START_TASK_STK[START_STK_SIZE];	//任务堆栈	
void start_task(void *p_arg);			//任务函数



int main(void)
{
    OS_ERR err;
	CPU_SR_ALLOC();
    
    Write_Through();                //透写
		MPU_Memory_Protection();				//保护相关存储区域
    Cache_Enable();                 //打开L1-Cache
    HAL_Init();				       				//初始化HAL库
    Stm32_Clock_Init(432,25,2,9);   //设置时钟,216Mhz 
    delay_init(216);                //延时初始化
		uart_init(115200);		       	  //串口初始化
	  usmart_dev.init(108); 		    //初始化USMART	
    LED_Init();                  	  //初始化LED
		MY_ADC_Init();								  //初始化ADC
		KEY_Init();     							  //按键初始化
		KEY_EXTI_Init();                //外部中断初始化
    SDRAM_Init();                   //初始化SDRAM
		TFTLCD_Init();			            //初始化LCD
    //Gizwits_Init();               //协议初始化
		RTC_Init();                     //初始化RTC 
    RTC_Set_WakeUp(RTC_WAKEUPCLOCK_CK_SPRE_16BITS,0); //配置WAKE UP中断,1秒钟中断一次
	
    POINT_COLOR = BRED;
//	LCD_ShowString( x, y,width,height,size,u8 *p);
	LCD_ShowString(30,10,200,32,32,"Apollo F767IG");	
	LCD_ShowString(30,46,200,32,32,"UCOSIII EMWIN");
	LCD_ShowString(30,82,200,32,32,"LWIP GIZWITS");
	//LCD_ShowString(30,70,200,16,16,"ATOM@ALIENTEK");
	//LCD_ShowString(30,90,200,16,16,"2016/7/25");
	//printf("KEY1:AirLink连接模式\t KEY_UP:复位\r\n\r\n"); 
	
	OSInit(&err);		            //初始化UCOSIII
	OS_CRITICAL_ENTER();//进入临界区
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
				 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, //任务选项,为了保险起见，所有任务都保存浮点寄存器的值
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区	 
	OSStart(&err);      //开启UCOSIII
								 
    while(1);
 
}








