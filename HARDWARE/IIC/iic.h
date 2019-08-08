#ifndef _iic_h_
#define _iic_h_

#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}  //设置为上拉/下拉输入模式
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}  //设置为推挽输出模式，速度为50MHz

#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define IIC_IN_SDA   PBin(7)  //输入SDA 


void iic_init(void);    //iic初始化函数
void iic_start(void);  //起始信号
void iic_stop(void);  //总线停止信号
unsigned char iic_write_byte(unsigned char dat);
unsigned char iic_read_nack(void);
unsigned char iic_read_ack(void);

#endif
