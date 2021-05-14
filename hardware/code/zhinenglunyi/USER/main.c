#include "stdio.h"
#include "stdlib.h"
#include "delay.h"
#include "string.h"
#include "math.h"
#include "sys.h"
#include "usart.h"	 
#include "dht11.h" 	
#include "gprs.h"

#include "para.h"

#include "max30102.h" 
#include "myiic.h"
#include "algorithm.h"
#include "gps.h"

char *gpsbuf;
char *temp;
int i;
int b;

void uart2_init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

	USART_Init(USART2, &USART_InitStructure); 
	//USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE); 


}


 int ceshiGPS(void)
 {	 		  
 	while(1)
	{
		if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE))
		{
			s=USART_ReceiveData(USART2);
			USART_SendData(USART1,s);
 		}
	}								   
	 
	
}





void helpinit()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource3);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	EXTI_ClearITPendingBit(EXTI_Line3);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


void tim_init()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
	TIM_TimeBaseStructure.TIM_Period = 15000; 
	TIM_TimeBaseStructure.TIM_Prescaler =7199; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );	
		
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3, ENABLE);
	

}


int main()
{	
	char tem[256];
	gpsbuf=tem;
	gpsbuf_i=0;
	gps_already=0;
	
	wendu=30.00;
	shidu=20.00;
	rate=100.00;
	spo2=20.00;
	jingdu=123.918884;
	weidu=47.351009;
	message="help!";
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(8);  
	uart_init(115200);
	uart2_init(9600);
	gps_init();
	helpinit();
	tim_init();
	max_once();
	
	while(1)
	{
					
	}
	
}



void USART2_IRQHandler(void)
{
		while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE))
		{
			s=USART_ReceiveData(USART2);
			USART_SendData(USART1,s);
 		}
			
}


void USART3_IRQHandler(void)
{		
		//USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);	
		if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE))
		{	
			gpsbuf[gpsbuf_i++]=USART_ReceiveData(USART3);
			
 		}
		else if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE))
		{
			USART3->SR;
			USART3->DR;
			gps_already=1;			
		
		}
		
}




void EXTI3_IRQHandler(void)
{		delay_ms(1);
		if((EXTI_GetITStatus(EXTI_Line3)!=RESET)&&(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)==0))
		{	
		//USART_SendData(USART1,'3');	
		sendmessage();
		
	
		EXTI_ClearITPendingBit(EXTI_Line3);
		}
		
}



void TIM3_IRQHandler(void)
{
	//char mybuf[256];
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update ); 
		
			gps_getdat();					//获得gps信息
			DHT11_get(&wendu , &shidu);		//获得温湿度信息
			maxim_get(&rate,&spo2);			//	获得心率血氧数据
			sendtoserver();					//发送数据
			
			
//			sprintf(mybuf,"wen=%.2f, \r\n", wendu);
//				max_senddat(mybuf);
//			
//			
//			sprintf(mybuf,"shi=%.2f, \r\n", shidu);
//				max_senddat(mybuf);
//						
//					
//					max_senddat("\r\n");
//		sprintf(mybuf,"HR=%.2f, ", rate); 
//				max_senddat(mybuf);
//				
//				sprintf(mybuf,"SpO2=%.2f, \r\n", spo2);
//				max_senddat(mybuf);

	}

}


