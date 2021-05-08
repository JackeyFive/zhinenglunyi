#include<reg52.h>
#include<stdio.h>
#define uchar unsigned char
	#define uint unsigned int
		#define u8 unsigned char
#define iicr=0x91;
	#define iicw=0x90;
		
		
		#define TX_ADR_WIDTH   5  				// 5字节宽度的发送/接收地址
#define TX_PLOAD_WIDTH 32  				// 数据通道有效数据宽度

//********************************************************************************************************************//
// SPI(SI24R1) commands
#define READ_REG        0x00  // Define read command to register
#define WRITE_REG       0x20  // Define write command to register
#define RD_RX_PLOAD     0x61  // Define RX payload register address
#define WR_TX_PLOAD     0xA0  // Define TX payload register address
#define FLUSH_TX        0xE1  // Define flush TX register command
#define FLUSH_RX        0xE2  // Define flush RX register command
#define REUSE_TX_PL     0xE3  // Define reuse TX payload register command
#define NOP             0xFF  // Define No Operation, might be used to read status register

//********************************************************************************************************************//
// SPI(SI24R1) registers(addresses)
#define CONFIG          0x00  // 'Config' register address
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define SETUP_AW        0x03  // 'Setup address width' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH           0x05  // 'RF channel' register address
#define RF_SETUP        0x06  // 'RF setup' register address
#define STATUS          0x07  // 'Status' register address
#define OBSERVE_TX      0x08  // 'Observe TX' register address
#define RSSI            0x09  // 'Received Signal Strength Indecator' register address
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define TX_ADDR         0x10  // 'TX address' register address
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // 'FIFO Status Register' register address

//********************************************************************************************************************//
// STATUS Register 
#define RX_DR						0x40  /**/
#define TX_DS						0x20
#define MAX_RT					0x10


u8 code TX_ADDRESS[TX_ADR_WIDTH] = {0xA5, 0x5A, 0xA5, 0x01, 0x01};



sbit in1=P0^0;              //?????
sbit in2=P0^1;                //??????????????
sbit in3=P0^2;  //??????????????
sbit in4=P0^3;
sbit en1=P0^4;
sbit en2=P0^5;

sbit qian=P2^0;
sbit hou=P2^1;
sbit zuo=P2^2;
sbit you=P2^3;

unsigned char temp;

sbit CSN=P1^3;
	sbit CE=P1^4;
	sbit MOSI=P1^5;
	sbit SCK=P1^6;
	sbit MISO=P1^7;
	
	sbit IRQ=P1^2;


u8 fa[5]={0};
u8 shou[32]={0};


void delay(uint x)
	{uint i,j;
		for(i=0;i<x;i++)
		for(j=0;j<110;j++);
		
	}


																//					无线部分

static u8 SPI_RW(u8 byte)									//读写一字节
{
	u8 bit_ctr;
	for(bit_ctr=0; bit_ctr<8; bit_ctr++)
	{
		if(byte & 0x80)
			MOSI = 1;
		else
			MOSI = 0;																 
		byte = (byte << 1);                      
		SCK = 1;                                   
		byte |= MISO;                             
		SCK = 0;                                
	}
	return(byte);                              
}
	
	void SI24R1_Init(void)						//初始化无线
{
	SCK = 0; 													//SPI时钟线拉低
	CSN = 1;				
	CE 	= 0;				
	IRQ = 1;
}
	

u8 SI24R1_Write_Reg(u8 reg, u8 value)  			//写一条命令
{
	u8 status;
	 
	CSN = 0;                 
	status = SPI_RW(reg);				
	SPI_RW(value);
	CSN = 1;  
	 
	return(status);
}


u8 SI24R1_Write_Buf(u8 reg, const u8 *pBuf, u8 bytes)	//写入多个字节
{
	u8 status,byte_ctr;
	 
  CSN = 0;                                  			
  status = SPI_RW(reg);                          
  for(byte_ctr=0; byte_ctr<bytes; byte_ctr++)     
  { 
		SPI_RW(*pBuf);
		pBuf++;
	}
  CSN = 1;                                      	
 
  return(status);       
}	


u8 SI24R1_Read_Reg(u8 reg)				//读一个字节
{
 	u8 value;
	 
	CSN = 0;    
	SPI_RW(reg);			
	value = SPI_RW(0);
	CSN = 1;              
 
	return(value);
}



u8 SI24R1_Read_Buf(u8 reg, u8 *pBuf, u8 bytes)		//读多个字节
{
	u8 status,byte_ctr;
	 
  CSN = 0;                                        
  status = SPI_RW(reg);                           
  for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
    pBuf[byte_ctr] = SPI_RW(0);                   //读取数据，低字节在前
  CSN = 1;                                        
 
  return(status);    
}
								




//小车节点初始化

void CHE_RX_Mode(void)//SI24R1接收模式初始化
{
	CE = 0;
	
	SI24R1_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);	// 接收设备接收通道0使用和发送设备相同的发送地址
	SI24R1_Write_Reg(WRITE_REG + EN_AA, 0x01);               						// 使能接收通道0自动应答
	SI24R1_Write_Reg(WRITE_REG + EN_RXADDR, 0x01);           						// 使能接收通道0
	SI24R1_Write_Reg(WRITE_REG + RF_CH, 40);                 						// 选择射频通道0x40
	SI24R1_Write_Reg(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);  						// 接收通道0选择和发送通道相同有效数据宽度
	SI24R1_Write_Reg(WRITE_REG + RF_SETUP, 0x0f);            						// 数据传输率2Mbps，发射功率7dBm
	SI24R1_Write_Reg(WRITE_REG + CONFIG, 0x0f);              						// CRC使能，16位CRC校验，上电，接收模式
	SI24R1_Write_Reg(WRITE_REG + STATUS, 0xff);  												//清除所有的中断标志位
	CE = 1;                                            									// 拉高CE启动接收设备
	

}




//遥控器节点发送初始化
void YAOKONG_TX_Mode(void)			//SI24R1发送模式初始化
{
	CE = 0;
	
	SI24R1_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // 写入发送地址
	SI24R1_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // 为了应答接收设备，接收通道0地址和发送地址相同

	//SI24R1_Write_Reg(WRITE_REG + EN_AA, 0x01);       											// 使能接收通道0自动应答
	SI24R1_Write_Reg(WRITE_REG + EN_RXADDR, 0x01);   											// 使能接收通道0
	SI24R1_Write_Reg(WRITE_REG + SETUP_RETR, 0x0a);  											// 自动重发延时等待250us+86us，自动重发10次
	SI24R1_Write_Reg(WRITE_REG + RF_CH, 40);         											// 选择射频通道0x40
	SI24R1_Write_Reg(WRITE_REG + RF_SETUP, 0x0f);    											// 数据传输率2Mbps，发射功率7dBm
	SI24R1_Write_Reg(WRITE_REG + CONFIG, 0x0e);      											// CRC使能，16位CRC校验，上电
	//CE = 1;
}


u8 SI24R1_RxPacket(u8 *rxbuf)			//接收数据
{
	u8 state;
	CE = 0;
	state = SI24R1_Read_Reg(STATUS);  			                 //读取状态寄存器的值    	  
	SI24R1_Write_Reg(WRITE_REG+STATUS,state); //&(~0x40)              //清除RX_DS中断标志

	if(state & RX_DR)								                           //接收到数据
	{
		SI24R1_Read_Buf(RD_RX_PLOAD,rxbuf,TX_PLOAD_WIDTH);     //读取数据
		SI24R1_Write_Reg(FLUSH_RX,0xff);					              //清除RX FIFO寄存器
		CE = 1;
		return 's'; 
	}	   
	SI24R1_Write_Reg(FLUSH_RX,0xff);
	CE = 1;
	return 'f';                                                   //没收到任何数据
}



u8 SI24R1_TxPacket(u8 *txbuf)				//发送数据
{
	u8 state;
	
	CE=0;																										  //CE拉低，使能SI24R1配置
  SI24R1_Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH);	    //写数据到TX FIFO,32个字节
 	CE=1;																										  //CE置高，使能发送	   
	
	while(IRQ == 1);																				  //等待发送完成
	state = SI24R1_Read_Reg(STATUS);  											  //读取状态寄存器的值	   
	SI24R1_Write_Reg(WRITE_REG+STATUS, state); 		//&(~0x30)						//清除TX_DS或MAX_RT中断标志
	SI24R1_Write_Reg(FLUSH_TX,0xff);
	if(state&MAX_RT)																			    //达到最大重发次数
	{
												    //清除TX FIFO寄存器 
		return 'm'; 
	}
	if(state&TX_DS)																			      //发送完成
	{
		return 's';
	}
	return 'f';																						  //发送失败
}

	void fasong_pc(char x){
		SBUF=x;
		while(!TI);
		TI=0;
	
	}

	void dhDelay(uint j)
    {      uchar i;
	    for(;j>0;j--)
	  { 	
		for(i=0;i<27;i++);

	  }
    }
	
u8 fasong(u8 *a)
{
	u8 re=SI24R1_TxPacket((u8*)a);
	dhDelay(255);	
return re;	
}


u8 jieshou(u8 *a)
{
	u8 re=SI24R1_RxPacket(a);
dhDelay(255);
	return re;
	
}


void init(){
	//TMOD=0x01;
		TMOD=0x21;
		TH0=0xb0;
		TL0=0x3c;
		TR0=1;
		ET0=1;
		EA=1;
		
	SCON=0x50;			
	SM0=1;SM1=1;
		SM2=0;
		REN=1;			
	PCON=0x80;			
	TH1=0xf4;          //       TH1=0xf3;			
	TL1=0xf4;					 //       TL1=0xf3;
	ES=1;										
	TR1=1;	
		
	 //IT0=0;   //打开外部中断
	// EX0=1;

		
		
	//	ES=1;
		
		//PCON=0x00;
		
	}

	
//void main()									//jie
//{	u8 re=0;
//	init();SI24R1_Init();
//	CHE_RX_Mode();
//	shou[0]='g';
//	while(1)
//	{
//	IRQ=1;
//	
//		
//	while(IRQ);
//		re++;
//	
//	jieshou(shou);
//	
//		fasong_pc(re+48);
//		fasong_pc('\n');
//		
//		
//	}
//	
//}
	
//void main()								//fa
//{ u8 re;
//	init();SI24R1_Init();
//	YAOKONG_TX_Mode();
//	fa[0]='h';
//	while(1)
//{
//	IRQ=1;
//	
//	
//	
//	re=fasong(fa);
//	
//	fasong_pc(re);
//		fasong_pc('\n');
//	delay(1000);
//	
//}

//}




void get(){  		//     接收主函数
			
		IRQ=1;
		
		init();
		SI24R1_Init();
		CHE_RX_Mode();
	 
		

	while(1)
	{
		
		
		
		IRQ=1;
		
		while(IRQ);
		
		jieshou(shou);
		
		temp=shou[0];
		
	
		
		switch(temp)
		{
			case 0x01:
				
				en1=1;en2=1;in1=1;in2=0;in3=1;in4=0;fasong_pc(temp);fasong_pc('\n');
				
				IRQ=1;
				while(IRQ);
				jieshou(shou);temp=shou[0];fasong_pc(temp);fasong_pc('\n');
				
				break;
				
			case 0x02:
				
				en1=1;en2=1;in1=0;in2=1;in3=0;in4=1;fasong_pc(temp);fasong_pc('\n');
					
				IRQ=1;
				while(IRQ);
				jieshou(shou);temp=shou[0];fasong_pc(temp);fasong_pc('\n');
				break;
			
			case 0x03:
				
				en1=1;en2=1;in1=1;in2=0;in3=0;in4=1;fasong_pc(temp);fasong_pc('\n');
			
				IRQ=1;
				while(IRQ){	en1=0;en2=0;delay(5);en1=1;en2=1;delay(5);}
				jieshou(shou);temp=shou[0];fasong_pc(temp);fasong_pc('\n');
				break;
			
			case 0x04:
				
				en1=1;en2=1;in1=0;in2=1;in3=1;in4=0;fasong_pc(temp);fasong_pc('\n');
			
				IRQ=1;
				while(IRQ){	en1=0;en2=0;delay(5);en1=1;en2=1;delay(5);}
				jieshou(shou);temp=shou[0];fasong_pc(temp);fasong_pc('\n');
				break;
			
			
			default: fasong_pc('k');fasong_pc('\n');
		
		}
		in1=0;
		in2=0;
		in3=0;
		in4=0;
	}
	
}




void send(){  		//     发送主函数
	u8 re;		
		IRQ=1;
		
		init();
		SI24R1_Init();
		YAOKONG_TX_Mode();
	 P2=0x00;
	fa[0]='x';

	while(1)
	{
		

		temp=P2;
		
		
		while(1)
		{
			if((temp=P2)!=0x00)
			{
				delay(100);
				if((temp=P2)!=0x00)
					break;
			}
		}
		switch(temp)
		{
			case 0x08:
				fa[0]=1;
				re=fasong(fa);fasong_pc(re);fasong_pc('\n');
				while((temp=P2)==0x08)
				{P2=0x00;delay(10);}
				fa[0]=0;
				fasong(fa);fasong_pc(re);fasong_pc('\n');
				break;
				
			case 0x04:
				fa[0]=2;
				re=fasong(fa);fasong_pc(re);fasong_pc('\n');
				while((temp=P2)==0x04){P2=0x00;delay(10);}
				fa[0]=0;
				fasong(fa);fasong_pc(re);fasong_pc('\n');
				break;
			
			case 0x02:
				fa[0]=3;
				re=fasong(fa);fasong_pc(re);fasong_pc('\n');
				while((temp=P2)==0x02){P2=0x00;delay(10);}
				fa[0]=0;
				fasong(fa);fasong_pc(re);fasong_pc('\n');
				break;
			
			case 0x01:
				fa[0]=4;fasong_pc(re);fasong_pc('\n');
				re=fasong(fa);
				while((temp=P2)==0x01){P2=0x00;delay(10);}
				fa[0]=0;
				fasong(fa);fasong_pc(re);fasong_pc('\n');
				break;
			
			
			default: ;
		
		}

		delay(10);P2=0x00;
	}
	
}



void main()
{
	send();
//	get();

}