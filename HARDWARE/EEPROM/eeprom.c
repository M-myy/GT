#include "eeprom.h"
#include "stm32f10x.h"
#include "iic.h"

void eeprom_read_dat(u8 *buf,u8 addr,u8 len)
{
	do
	{
		iic_start();
		if(iic_write_byte(0x50<<1))
		{
			break;
		}
		iic_stop();
	}	while(1);
	iic_write_byte(addr);
	iic_start();
	iic_write_byte(0xA0|0X01);
	while(len>1)
	{
		*buf++ = iic_read_ack();
		len--;
	}
	*buf = iic_read_ack();
	iic_stop();
}

void eeprom_write_dat(u8 *buf,u8 addr,u8 len)
{
	while(len>0)
	{
		do
		{
			iic_start();
			if(iic_write_byte(0xa0))
			{
				break;
			}
			iic_stop();
		}while(1);
		iic_write_byte(addr);
		while(len>0)
		{
			iic_write_byte(*buf++);
			len--;
			addr++;
			if((addr%0xff)==0)
			{break;}
		}
		iic_stop();
	}
}
