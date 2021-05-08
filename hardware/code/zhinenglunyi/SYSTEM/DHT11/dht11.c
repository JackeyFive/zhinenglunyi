#include "dht11.h"
#include "delay.h"

u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)&&(retry<100))//等待变为低电平
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while((!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5))&&retry<100)//等待变高电平
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//等待40us
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5))return 1;
	else return 0;		   
}



u8 DHT11_Read_Byte(void)    
{        
    u8 i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
}



void DHT11_Read_Data(float *temp,float *humi)    
{        
 	u8 buf[5];
	u8 i;
	
	while(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5));
	
	for(i=0;i<5;i++)//读取40位数据
	{
		buf[i]=DHT11_Read_Byte();
	}
	
	if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
	{
		*humi=(float)buf[0];
		*temp=(float)buf[2];
	}
	
}


void DHT11_get(float *wendu , float *shidu)
{	u8 num=0;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
	
	delay_ms(10);
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	delay_ms(20);

	GPIO_SetBits(GPIOB,GPIO_Pin_5);
	delay_us(30);
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
			while((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5))&&(num<80))
			{
				delay_us(1);num++;
			
			}
			
			DHT11_Read_Data(wendu,shidu);
	
}


