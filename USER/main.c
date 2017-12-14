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
 ALIENTEK ������STM32F7������ UCOSIIIʵ��
 ��6-3 UCOSIII ʱ��Ƭ��ת����
 
 UCOSIII���������ȼ��û�������ʹ�ã�ALIENTEK
 ����Щ���ȼ��������UCOSIII��5��ϵͳ�ڲ�����
 ���ȼ�0���жϷ������������� OS_IntQTask()
 ���ȼ�1��ʱ�ӽ������� OS_TickTask()
 ���ȼ�2����ʱ���� OS_TmrTask()
 ���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
 ���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()

************************************************/

//task start
#define START_TASK_PRIO		3			//�������ȼ�
#define START_STK_SIZE 		128			//�����ջ��С	
OS_TCB StartTaskTCB;					//������ƿ�
CPU_STK START_TASK_STK[START_STK_SIZE];	//�����ջ	
void start_task(void *p_arg);			//������



int main(void)
{
    OS_ERR err;
	CPU_SR_ALLOC();
    
    Write_Through();                //͸д
		MPU_Memory_Protection();				//������ش洢����
    Cache_Enable();                 //��L1-Cache
    HAL_Init();				       				//��ʼ��HAL��
    Stm32_Clock_Init(432,25,2,9);   //����ʱ��,216Mhz 
    delay_init(216);                //��ʱ��ʼ��
		uart_init(115200);		       	  //���ڳ�ʼ��
	  usmart_dev.init(108); 		    //��ʼ��USMART	
    LED_Init();                  	  //��ʼ��LED
		MY_ADC_Init();								  //��ʼ��ADC
		KEY_Init();     							  //������ʼ��
		KEY_EXTI_Init();                //�ⲿ�жϳ�ʼ��
    SDRAM_Init();                   //��ʼ��SDRAM
		TFTLCD_Init();			            //��ʼ��LCD
    //Gizwits_Init();               //Э���ʼ��
		RTC_Init();                     //��ʼ��RTC 
    RTC_Set_WakeUp(RTC_WAKEUPCLOCK_CK_SPRE_16BITS,0); //����WAKE UP�ж�,1�����ж�һ��
	
    POINT_COLOR = BRED;
//	LCD_ShowString( x, y,width,height,size,u8 *p);
	LCD_ShowString(30,10,200,32,32,"Apollo F767IG");	
	LCD_ShowString(30,46,200,32,32,"UCOSIII EMWIN");
	LCD_ShowString(30,82,200,32,32,"LWIP GIZWITS");
	//LCD_ShowString(30,70,200,16,16,"ATOM@ALIENTEK");
	//LCD_ShowString(30,90,200,16,16,"2016/7/25");
	//printf("KEY1:AirLink����ģʽ\t KEY_UP:��λ\r\n\r\n"); 
	
	OSInit(&err);		            //��ʼ��UCOSIII
	OS_CRITICAL_ENTER();//�����ٽ���
	//������ʼ����
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, //����ѡ��,Ϊ�˱���������������񶼱��渡��Ĵ�����ֵ
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	//�˳��ٽ���	 
	OSStart(&err);      //����UCOSIII
								 
    while(1);
 
}








