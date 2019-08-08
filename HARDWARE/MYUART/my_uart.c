#include "my_uart.h"
#include "stm32f10x.h"

void usart_init(unsigned int baudrate)
{
	GPIO_InitTypeDef GPIO_InitStru;
	USART_InitTypeDef USART_InitStru;
	NVIC_InitTypeDef NVIC_InitStruc;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);
	USART_DeInit(USART1);
	
	GPIO_InitStru.GPIO_Mode=GPIO_Mode_AF_PP;    //设为推挽复用输出
	GPIO_InitStru.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStru.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStru);
  
	GPIO_InitStru.GPIO_Mode=GPIO_Mode_IN_FLOATING;    //设P10为浮空输入GPIO_Mode_AF_PP
	GPIO_InitStru.GPIO_Pin=GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIO_InitStru);
	
	USART_InitStru.USART_BaudRate=baudrate;
	USART_InitStru.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStru.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStru.USART_Parity=USART_Parity_No;
	USART_InitStru.USART_StopBits=USART_StopBits_1;
	USART_InitStru.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStru);
	
	NVIC_InitStruc.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStruc.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruc.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruc.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStruc);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART1,ENABLE);
}

///*串口中断处理函数*/
//void USART1_IRQHandler(void)
//{
//	uint16_t rx;
//	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE))
//	{
//		rx = USART_ReceiveData(USART1);
//		USART_SendData(USART1,rx);
//	}
//}
