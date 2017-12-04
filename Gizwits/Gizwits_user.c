
#include "Gizwits_user.h"
#include "timer.h"
#include "usart3.h"
#include "gizwits_product.h" 

// �û�����ǰ�豸״̬�ṹ��
dataPoint_t currentDataPoint;


//GizwitsЭ���ʼ��
void Gizwits_Init(void)
{
	TIM3_Init(10-1,10800-1); //1MSϵͳ��ʱ 
	uart3_init(9600);//WIFI��ʼ�� 
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//�豸״̬�ṹ���ʼ��
	gizwitsInit();//��������ʼ��
	
}

//���ݲɼ�
void userHandle(void)
{
   	u8 led1;
	
    led1 = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);//�жϵ�ǰLED1������
	if(led1==0)
	{
	   currentDataPoint. valueLED_OnOff= 1;
	}
	else
	{
	   currentDataPoint. valueLED_OnOff= 0;
	}  
}



