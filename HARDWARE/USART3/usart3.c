#include "usart3.h"
#include "gizwits_product.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F7开发板
//串口3初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2016/6/17
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.0修改说明 
////////////////////////////////////////////////////////////////////////////////// 


UART_HandleTypeDef UART3_Handler;                         //UART句柄

//串口3中断服务程序
void USART3_IRQHandler(void)
{
	  u8 res;
	  if((__HAL_UART_GET_FLAG(&UART3_Handler,UART_FLAG_RXNE)!=RESET))
	  {
		  res = UART3_Handler.Instance->RDR;
		  gizPutData(&res, 1);//数据写入到缓冲区	
	  }
}


//初始化IO,串口3
//bound:波特率
void uart3_init(u32 bound)
{	
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;

	__HAL_RCC_GPIOB_CLK_ENABLE();			              //使能GPIOB时钟
	__HAL_RCC_USART3_CLK_ENABLE();			              //使能USART1时钟

	//UART 初始化设置
	UART3_Handler.Instance=USART3;					      //USART3
	UART3_Handler.Init.BaudRate=bound;				      //波特率
	UART3_Handler.Init.WordLength=UART_WORDLENGTH_8B;     //字长为8位数据格式
	UART3_Handler.Init.StopBits=UART_STOPBITS_1;	      //一个停止位
	UART3_Handler.Init.Parity=UART_PARITY_NONE;		      //无奇偶校验位
	UART3_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;     //无硬件流控
	UART3_Handler.Init.Mode=UART_MODE_TX_RX;		      //收发模式
	HAL_UART_Init(&UART3_Handler);					      //HAL_UART_Init()会使能UART3	

	GPIO_Initure.Pin=GPIO_PIN_10|GPIO_PIN_11;			  //PB10、PB11
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;		              //复用推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;			              //上拉
	GPIO_Initure.Speed=GPIO_SPEED_FAST;		              //高速
	GPIO_Initure.Alternate=GPIO_AF7_USART3;	              //复用为USART3
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);	            	  //初始化PB10,和PB11
	

	__HAL_UART_ENABLE_IT(&UART3_Handler,UART_IT_RXNE);    //开启接收中断
	HAL_NVIC_EnableIRQ(USART3_IRQn);				      //使能USART1中断通道
	HAL_NVIC_SetPriority(USART3_IRQn,2,3);			      //抢占优先级2，子优先级3


	
  
}













