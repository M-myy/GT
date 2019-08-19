#ifndef _key_h_
#define _key_h_

#define LIE1 GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_2)  //设置列编号
#define LIE2 GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_3)
#define LIE3 GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_4)

#define HANG1 PGout(5)    //设置行编号
#define HANG2 PGout(6)
#define HANG3 PGout(7)
#define HANG4 PGout(8)

#define KEY1_PRESS  1 //'1'    //"1"
#define KEY2_PRESS  2 //'2'    //"2"
#define KEY3_PRESS  3 //'3'    //"3"
#define KEY4_PRESS  4 //'4'    //"4"
#define KEY5_PRESS  5 //'5'    //"5"
#define KEY6_PRESS  6 //'6'    //"6"
#define KEY7_PRESS  7 //'7'    //"7"
#define KEY8_PRESS  8 //'8'    //"8"
#define KEY9_PRESS  9 //'9'    //"9"
#define KEY10_PRESS 10 //'\0'    //"*"
#define KEY11_PRESS 11 //'0'    //"0"
#define KEY12_PRESS 12 //'.'    //"."

void key_init(void);    //按键初始化函数
unsigned char key_scan(void);    //按键扫描函数

#endif
