#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gps.h>
#include <gprs.h>
#include "max30102.h"
#include <math.h>

void gps_init()  //初始化串口三为 gps 数据传输端口。
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

	USART_Init(USART3, &USART_InitStructure); 
//	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
//	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
//	//USART_Cmd(USART3, ENABLE); 

}


void gps_getdat()
{
	char ss[256];
	char *p;
	double x;
	double y;
	double temp1,temp2;	

		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
		USART_Cmd(USART3, ENABLE);
		delay_ms(1);
		gpsbuf_i=0;
		gps_already=0;
		while(!gps_already);
		gpsbuf_i=0;
		gps_already=0;
		while(!gps_already);
	
		USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
		USART_ITConfig(USART3, USART_IT_IDLE, DISABLE);
		USART_Cmd(USART3, DISABLE);
		delay_ms(1);
	
		gpsbuf[gpsbuf_i]='\0';

		strncpy(ss,gpsbuf,gpsbuf_i);

    p=strtok(ss,",");

    p=strtok(NULL,",");

    p=strtok(NULL,",");

	p=strtok(NULL,",");
//	max_senddat(p);
	y=atof(p);

    p=strtok(NULL,",");

    p=strtok(NULL,",");

	x=atof(p);

	x/=100;
	y/=100;


    jingdu=(float)((modf(x,&temp1)*100/60)+temp1);

    weidu=(float)((modf(y,&temp2)*100/60)+temp2);
	if(jingdu<0||jingdu>180)
	{
		jingdu=123.918884;
		weidu=47.351009;
	}

}

