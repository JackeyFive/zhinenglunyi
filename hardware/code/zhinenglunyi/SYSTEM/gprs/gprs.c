#include "stdio.h"
#include "stdlib.h"
#include "delay.h"
#include "string.h"
#include "gprs.h"
#include "sys.h"
#include "max30102.h"	

void sendtogprs(u8 *s)
{
	while((*s)!='\0')
	{
	USART_SendData(USART2,*s);while(!USART_GetFlagStatus(USART2, USART_FLAG_TC));
		s++;
	}
}

void tostring(float f,char *buf)
{
	char bai,shi,ge,yi,er,san;
	int i=(int)(f*1000);
	bai=(i/100000)+48;
	shi=(i%100000)/10000+48;
	ge=(i%10000)/1000+48;
	yi=(i%1000)/100+48;
	er=(i%100)/10+48;
	san=i%10+48;

	buf[0]=bai;
	buf[1]=shi;
	buf[2]=ge;
	buf[3]='.';
	buf[4]=yi;
	buf[5]=er;
	buf[6]=san;
	buf[7]='\0';

}


 void sendtoserver()
 {	 
	 char buf[256];
	 
	 strcpy(buf,"AT+HTTPPARA=\"URL\",\"http://195.133.10.150:8080/update?");
	 sprintf(buf,"%s%f&%f&%.2f&%.2f&%.2f&%.2f%s",buf,jingdu,weidu,wendu,shidu,rate,spo2,"\"\r\n"); 
	 max_senddat(buf);
	 
	 
	 sendtogprs("ATI\t\n");
	 delay_ms(200);delay_ms(200);
	 sendtogprs("AT+CSQ\r\n");
	 delay_ms(200);
	 	 sendtogprs("AT+CPIN?\r\n");
	 delay_ms(200);
	 	 sendtogprs("AT+COPS?\r\n");
	 delay_ms(200);
	 
	 //sendtogprs("AT+SAPBR=3,1,\"APN\",\"cmnet\"\r\n");
	 //delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);
	 
	 sendtogprs("AT+SAPBR=1,1\r\n");
	 delay_ms(200);
	 
	 sendtogprs("AT+SAPBR=2,1\r\n");
	 delay_ms(200);
	 
	 sendtogprs("AT+HTTPINIT\r\n");
	 delay_ms(200);
	 
	 sendtogprs("AT+HTTPPARA=\"CID\",\"1\"\r\n");
	 delay_ms(200);
	 
	 sendtogprs((u8*)buf);
	 delay_ms(200);
	 
	 sendtogprs("AT+HTTPACTION=0\r\n");
	  delay_ms(200);	 
	
}
 
void sendmessage()
{	 
	 sendtogprs("ATI\t\n");
	 delay_ms(200);delay_ms(200);
	 sendtogprs("AT+CSQ\r\n");
	 delay_ms(200);
	 	 sendtogprs("AT+CPIN?\r\n");
	 delay_ms(200);
	 	 sendtogprs("AT+COPS?\r\n");
	 delay_ms(200);
	 	 
	 sendtogprs("AT+CSCS=\"GSM\"\r\n");
	 delay_ms(200);
	 
	 sendtogprs("AT+CSCA?\r\n");
	 delay_ms(200);
	 
	 sendtogprs("AT+CMGF=1\r\n");
	 delay_ms(200);
	 
	 sendtogprs("AT+CMGS=\"13458378653\"\r\n");
	 delay_ms(200);
	 
	 sendtogprs(message);
	 delay_ms(200);
	 
	 USART_SendData(USART2,0x1A);
	 delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);
	 delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);
	 
	 sendtogprs("AT+CMGD=1,4\r\n");
	 delay_ms(200);		

}


