
#include "Gizwits_user.h"
#include "timer.h"
#include "usart3.h"
#include "gizwits_product.h" 

// 用户区当前设备状态结构体
dataPoint_t currentDataPoint;


//Gizwits协议初始化
void Gizwits_Init(void)
{
	TIM3_Init(10-1,10800-1); //1MS系统定时 
	uart3_init(9600);//WIFI初始化 
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//设备状态结构体初始化
	gizwitsInit();//缓冲区初始化
	
}

//数据采集
void userHandle(void)
{
   	u8 led1;
	
    led1 = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);//判断当前LED1开关量
	if(led1==0)
	{
	   currentDataPoint. valueLED_OnOff= 1;
	}
	else
	{
	   currentDataPoint. valueLED_OnOff= 0;
	}  
}



