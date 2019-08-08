#ifndef _iic_h_
#define _iic_h_

#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}  //����Ϊ����/��������ģʽ
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}  //����Ϊ�������ģʽ���ٶ�Ϊ50MHz

#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define IIC_IN_SDA   PBin(7)  //����SDA 


void iic_init(void);    //iic��ʼ������
void iic_start(void);  //��ʼ�ź�
void iic_stop(void);  //����ֹͣ�ź�
unsigned char iic_write_byte(unsigned char dat);
unsigned char iic_read_nack(void);
unsigned char iic_read_ack(void);

#endif
