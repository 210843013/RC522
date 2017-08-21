#include "stm32f4xx.h"
#include "uart.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"
#include "time.h"
#include "dma.h"

void uart_init(u32 bond)
{
	//PA
	RCC->AHB1ENR |=0x01<<0;
	//PA 9 10
	GPIOA->MODER &=~(0x03<<18 | 0x03<<20);
	GPIOA->MODER |=(0x02<<18 | 0x02<<20);       //PA 9 10���ù���ģʽ
	GPIOA->OTYPER &=~(0x01<<9);								//PA9�������
	GPIOA->OSPEEDR &=~(0x03<<18);							//PA9����
	GPIOA->PUPDR &=~(0x03<<18 |0x03<<20);               //PA9��������
	
	GPIOA->AFR[1] &= 0xFFFFF00F;							//���ù���
	GPIOA->AFR[1] |= 0x00000770;
	//����uartʱ��ʹ��
	RCC->APB2ENR |=0x01<<4;
	//������
	USART1->BRR =84000000/bond;
	//16������
	USART1->CR1 &=~(0x01<<15);
	//�ֳ�
	USART1->CR1 &=~(0x01<<12);
	//�����գ�������
	USART1->CR1 |=(0x01<<2 |0x01<<3);
	//uartʹ��
	USART1->CR1 |=(0x01<<13);
	
}


void uart1_interrupt_init(u32 bond)
{
	//PA
	RCC->AHB1ENR |=0x01<<0;
	//PA 9 10
	GPIOA->MODER &=~(0x03<<18 | 0x03<<20);
	GPIOA->MODER |=0x02<<18 | 0x02<<20;       //PA 9 10���ù���ģʽ
	GPIOA->OTYPER &=~(0x01<<9);								//PA9�������
	GPIOA->OSPEEDR &=~(0x03<<18);							//PA9����
	GPIOA->PUPDR &=~(0x03<<18 |0x03<<20);               //PA9��������
	
	GPIOA->AFR[1] &= 0xFFFFF00F;							//���ù���
	GPIOA->AFR[1] |= 0x00000770;
	//����uartʱ��ʹ��
	RCC->APB2ENR |=0x01<<4;
	//������
	USART1->BRR =84000000/bond;
	//16������
	USART1->CR1 &=~(0x01<<15);
	//�ֳ�
	USART1->CR1 &=~(0x01<<12);
	//�����գ�������
	USART1->CR1 |=(0x01<<2 |0x01<<3);
	
	//�����ڽ����ж�ʹ��
	USART1->CR1 |=0x01<<5;
	//�������ȼ����������ȼ�
	NVIC_SetPriority(USART1_IRQn,NVIC_EncodePriority(7-2,1,0));
	//ʹ�ܴ����ж�
	NVIC_EnableIRQ(USART1_IRQn);
	//uartʹ��
	USART1->CR1 |=(0x01<<13);
}

u8 rec_buf[1000];
u16 rec_num=0;
u8 rec_sta=0;     //���ݽ�����ɱ�־
void USART1_IRQHandler(void)
{
	u8 read;
	if((rec_sta==0) && (USART1->SR & (0x01<<5)))		//�Ƿ���ܵ�����
	{
		SysTick->CTRL &=~(0x01<<0);
		SysTick->VAL =0;
		SysTick->CTRL |=0x01<<0;
		rec_buf[rec_num++]=USART1->DR;	
	}
	else
	{
		read=USART1->DR;
	}
}


void uart_send_string(s8 *p)			//s8��char��
{
	while(*p!='\0')
	{
		while(!(USART1->SR & (0x01<<7)));
		USART1->DR=*(p++);
	}
}

//���ڷ��ͻ����� 	
__align(8) u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 	//���ͻ���,���USART2_MAX_SEND_LEN�ֽ�
//���ڽ��ջ����� 	
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//���ջ���,���USART2_MAX_RECV_LEN���ֽ�.

u16 USART2_RX_STA=0;
//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
   	 
void USART2_IRQHandler(void)
{
	u8 res;	    
	if(USART2->SR&(1<<5))//���յ�����
	{	 
		res=USART2->DR; 			 
		if(USART2_RX_STA<USART2_MAX_RECV_LEN)		//�����Խ�������
		{
			TIM4->CNT=0;         									//���������
			if(USART2_RX_STA==0) TIM4_Set(1);	 		//ʹ�ܶ�ʱ��4���ж� 
			USART2_RX_BUF[USART2_RX_STA++]=res;		//��¼���յ���ֵ	 
		}else 
		{
			USART2_RX_BUF[USART2_RX_STA&0X3FFF]=0;
			USART2_RX_STA|=1<<15;					//ǿ�Ʊ�ǽ������
			
		} 
	}  					
}

//��ʼ��IO ����2
//bound:������	
void usart2_init(u32 bound)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
//	USART_InitTypeDef USART_InitStruct;
	
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA , ENABLE );	//GPIOAʹ��
	//PA9
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_3;				//PA3
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;			//����
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;			//���
	GPIO_InitStruct.GPIO_Speed = GPIO_Low_Speed;		//����
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;  //����
	GPIO_Init( GPIOA,  &GPIO_InitStruct );
	//PA10
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_2;				//PA2
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;	//����
	GPIO_Init( GPIOA,  &GPIO_InitStruct );
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2,  GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);


	
	RCC->APB1ENR |=0x01<<17;								//UART2ʹ��	
	USART2->BRR =84000000/bound;							//������	
	USART2->CR1 &=~(0x01<<15);							//16������	
	USART2->CR1 &=~(0x01<<12);							//�ֳ�	
	USART2->CR1 |=(0x01<<13);								//uart2ʹ��		
	USART2->CR1 |=(0x01<<2 |0x01<<3);				//�����գ�������
	USART2->CR3 |=0x01<<7;									//DMA����ʹ��	
	UART_DMA_Config((u32)&USART2->DR,(u32)USART2_TX_BUF);
#ifdef USART2_RX_EN		  									//���ʹ���˽���
	USART2->CR1|=1<<8;    									//PE�ж�ʹ��	
	USART2->CR1 |=0x01<<5;									//�����ڽ����ж�ʹ��
	NVIC_SetPriority(USART2_IRQn,NVIC_EncodePriority(7-2,1,0));//ʹ�ܴ����ж�	
	NVIC_EnableIRQ(USART2_IRQn);
	TIM4_Init(999,7199);	//100ms�ж�
	USART2_RX_STA=0;		//����
	TIM4_Set(1);			//�رն�ʱ��4	
#endif	
}

//����2,printf ����
//ȷ��һ�η������ݲ�����USART2_MAX_SEND_LEN�ֽ�
void u2_printf(char* fmt,...)  
{  
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	while(DMA1_Stream6->CR & 0x01<<0);	//�ȴ�����������	
	UART_DMA_Enable(strlen((const char*)USART2_TX_BUF)); 	//ͨ��dma���ͳ�ȥ
}



#pragma import(__use_no_semihosting_swi) //ȡ��������״̬

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;

int fputc(int ch, FILE *f) 
{
		while((USART1->SR &(0X01<<7))==0 );
		USART1->DR=ch;
		return (ch);
}
int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}


void _ttywrch(int ch) {
  while((USART1->SR &(0X01<<7))==0);
		USART1->DR=ch;
}


void _sys_exit(int return_code) {
label:  goto label;  /* endless loop */
}


