#ifndef __DHT11_H
#define __DHT11_H 
#include "sys.h"   
#include "delay.h"
#endif

u8 DHT11_Read_Bit(void); 
u8 DHT11_Read_Byte(void);
void DHT11_Read_Data(float *temp,float *humi);
void DHT11_get(float *wendu , float *shidu);














