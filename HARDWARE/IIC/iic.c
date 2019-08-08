#include "iic.h"
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"

void iic_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//使能GPIOB时钟
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  PBout(6) = 1;  //将PB6设置为高电平
  PBout(7) = 1;  //将PB7设置为高电平
}

void iic_start(void)
{
	SDA_OUT();
	IIC_SDA = 1;
	IIC_SCL = 1;
	delay_us(4);
	IIC_SDA = 0;
	delay_us(4);
	IIC_SCL = 0;
}

void iic_stop(void)
{
	SDA_OUT();
	IIC_SCL = 0;
	IIC_SDA = 0;
	delay_us(4);
	IIC_SCL = 1;
	delay_us(4);
	IIC_SDA = 1;
	delay_us(4);
}

unsigned char iic_write_byte(unsigned char dat)
{
	
	unsigned char ack;
	unsigned char mask;
	
	SDA_OUT();
	for(mask=0x80;mask!=0;mask>>=1)
	{
		if((mask&dat)==0)
			IIC_SDA = 0;
		else
			IIC_SDA = 1;
		delay_us(4);
		IIC_SCL = 1;
		delay_us(4);
		IIC_SCL = 0;
	}
	IIC_SDA = 1;
	delay_us(4);
	IIC_SCL = 1;
	ack = IIC_SDA;
	delay_us(4);
	IIC_SCL = 0;
	
	return ~ack;
}

unsigned char iic_read_nack(void)
{
	unsigned char mask,dat;
	
	SDA_IN();
	IIC_SDA = 1;
	for(mask=0x80;mask!=0;mask>>=1)
	{
		delay_us(4);
		IIC_SCL = 1;
		if(IIC_SDA==0)
			dat&=~mask;
		else
			dat|=mask;
		delay_us(4);
		IIC_SCL = 0;
	}
	IIC_SDA = 1;
	delay_us(4);
	IIC_SCL = 1;
	delay_us(4);
	IIC_SCL = 0;
	
	return dat;
}

unsigned char iic_read_ack(void)
{
	unsigned char mask,dat;
	
	SDA_IN();
	IIC_SDA = 1;
	for(mask=0x80;mask!=0;mask>>=1)
	{
		delay_us(4);
		IIC_SCL = 1;
		if(IIC_SDA==0)
			dat&=~mask;
		else
			dat|=mask;
		delay_us(4);
		IIC_SCL = 0;
	}
	IIC_SDA = 0;
	delay_us(4);
	IIC_SCL = 1;
	delay_us(4);
	IIC_SCL = 0;
	
	return dat;
}