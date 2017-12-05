

#include "key_exti.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "gizwits_product.h" 
#include "includes.h"
#include "osTimer.h"


//�ⲿ�жϳ�ʼ��
void KEY_EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();               //����GPIOAʱ��
    __HAL_RCC_GPIOC_CLK_ENABLE();               //����GPIOCʱ��
    __HAL_RCC_GPIOH_CLK_ENABLE();               //����GPIOHʱ��
    
    GPIO_Initure.Pin=GPIO_PIN_0;                //PA0
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;      //�����ش���
    GPIO_Initure.Pull=GPIO_PULLDOWN;			//����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
    GPIO_Initure.Pin=GPIO_PIN_13;               //PC13
    GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;     //�½��ش���
    GPIO_Initure.Pull=GPIO_PULLUP;				//����
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
    
    GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3;     //PH2,3  �½��ش���������
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);
    
    //�ж���0
    HAL_NVIC_SetPriority(EXTI0_IRQn,2,0);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ0
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);             //ʹ���ж���0
    
    //�ж���2
    HAL_NVIC_SetPriority(EXTI2_IRQn,2,1);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ1
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);             //ʹ���ж���2
    
    //�ж���3
    HAL_NVIC_SetPriority(EXTI3_IRQn,2,2);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ2
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);             //ʹ���ж���2
    
    //�ж���13
    HAL_NVIC_SetPriority(EXTI15_10_IRQn,2,3);   //��ռ���ȼ�Ϊ3�������ȼ�Ϊ3
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);         //ʹ���ж���13  
}


//�жϷ�����
void EXTI0_IRQHandler(void)
{
	OSIntEnter();
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);   //�����жϴ����ú���
	OSIntExit();
}

void EXTI2_IRQHandler(void)
{
	OSIntEnter();
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);   //�����жϴ����ú���
	OSIntExit();
}

void EXTI3_IRQHandler(void)
{
	OSIntEnter();
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);   //�����жϴ����ú���
	OSIntExit();
}

void EXTI15_10_IRQHandler(void)
{
	OSIntEnter();
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);  //�����жϴ����ú���
	OSIntExit();
}

//�жϷ����������Ҫ��������
//��HAL�������е��ⲿ�жϷ�����������ô˺���
//GPIO_Pin:�ж����ź�
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    static u8 led0sta=1,led1sta=1;
		OS_ERR err;
	
    delay_ms(50);      //����
    switch(GPIO_Pin)
    {
        case GPIO_PIN_0:
            if(WK_UP==1)	//����LED0,LED1�������
            {
                led1sta=!led1sta;
                led0sta=!led1sta;
                LED1(led1sta);
                LED0(led0sta);
								//printf("WIFI��λ����������������\r\n");
								//gizwitsSetMode(WIFI_RESET_MODE);//WIFI��λ
            }
            break;
        case GPIO_PIN_2:
            if(KEY1==0) 	//����LED1��ת	
            {
                led1sta=!led1sta;
                LED1(led1sta);
								OSTmrStop(&tmr2,OS_OPT_TMR_NONE,0,&err);	//�رն�ʱ��1							
								//printf("WIFI����AirLink����ģʽ\r\n");
								//gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-linkģʽ����
            };
            break;
        case GPIO_PIN_3:
            if(KEY0==0)  	//ͬʱ����LED0,LED1��ת 
            {
                led1sta=!led1sta;
                led0sta=!led0sta;
                LED1(led1sta);
                LED0(led0sta); 
								OSTmrStop(&tmr1,OS_OPT_TMR_NONE,0,&err);	//�رն�ʱ��1			
            }
            break;

        case GPIO_PIN_13:
            if(KEY2==0)  	//����LED0��ת
            {
                led0sta=!led0sta;
                LED0(led0sta);
								OSTmrStart(&tmr1,&err);	//������ʱ��1
								OSTmrStart(&tmr2,&err);	//������ʱ��1
            }
            break;
    }
}
