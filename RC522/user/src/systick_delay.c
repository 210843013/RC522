#include "systick_delay.h"
#include "stm32f4xx.h"
#include "led.h"
#include "uart.h"

//u8 key=key_no;

//void systick_delay_us(u32 nus)
//{
//	u32 sta;
//	//ѡ��ʱ��Դ
//	SysTick->CTRL &=~(0x01<<2);
//	//����load��ֵ
//	SysTick->LOAD =nus*SYSTICK/8;
//	//��valд����
//	SysTick->VAL =0;
//	//����ʱ��
//	SysTick->CTRL |=0x01<<0;
//	//�ȴ���ʱʱ�䵽����ѯCTRL.16�Ƿ�Ϊ1��
//	do
//	{
//		sta =SysTick->CTRL;
//	}while((sta & 0x01) && !(sta & (0x01<<16)));
//	//��ʱ��
//	SysTick->CTRL &=~(0x01<<0);
//}


//void systick_delay_ms(u32 nms)
//{
//	u32 sta;
//	//ѡ��ʱ��Դ
//	SysTick->CTRL &=~(0x01<<2);
//	//����load��ֵ
//	SysTick->LOAD =nms*1000*SYSTICK/8;
//	//��valд����
//	SysTick->VAL =0;
//	//����ʱ��
//	SysTick->CTRL |=0x01<<0;
//	//�ȴ���ʱʱ�䵽����ѯCTRL.16�Ƿ�Ϊ1��
//	do
//	{
//		sta =SysTick->CTRL;
//	}while((sta & 0x01) && !(sta & (0x01<<16)));
//	//��ʱ��
//	SysTick->CTRL &=~(0x01<<0);
//}


//void systick_interrupt_init(u32 nms)
//{
//	//ѡ��ʱ��Դ
//	SysTick->CTRL &=~(0x01<<2);
//	//����load��ֵ
//	SysTick->LOAD =nms*1000*SYSTICK/8;
//	//��valд����
//	SysTick->VAL =0;
//	//�����ж�
//	SysTick->CTRL |=0x01<<1;
//	//�������ȼ����������ȼ�
//	NVIC_SetPriority(SysTick_IRQn,NVIC_EncodePriority(7-2,1,0));
//	//����systick
//	//SysTick->CTRL |=0x01<<0;
//}


//void SysTick_Handler(void)
//{
//	SysTick->CTRL &=~(0x01<<0);
//	rec_sta=1;
//	rec_buf[rec_num]='\0';
//	rec_num=0;
//	
//}


void systick_delay_us(u32 nus)
{
	//�趨��װ��ֵ
	if(nus>1)
	TIM7->ARR =nus-1;
	else
	TIM7->ARR =1;
	//�趨Ԥ��Ƶ
	TIM7->PSC =83;
	//��cnt
	TIM7->CNT =0;
	//����UGλ���������¼�
	TIM7->EGR |=0x01;
	//����±�־
	TIM7->SR &=~(0x01<<0);
	//����ʱ��	
	TIM7->CR1 |=0X01<<0; 
	while(!(TIM7->SR & 0x01));  //�ȴ���ʱʱ�䵽
	TIM7->SR &=~(0x01<<0);
	TIM7->CR1 &=~(0X01);
}


void systick_delay_ms(u32 nms)
{
		//�趨��װ��ֵ
	TIM7->ARR =nms*10-1;
	//�趨Ԥ��Ƶ
	TIM7->PSC =8399;
	//��cnt
	TIM7->CNT =0;
	//����UGλ���������¼�
	TIM7->EGR |=0x01;
	//����±�־
	TIM7->SR &=~(0x01<<0);
	//����ʱ��	
	TIM7->CR1 |=0X01<<0; 
	while(!(TIM7->SR & 0x01));  //�ȴ���ʱʱ�䵽
	TIM7->SR &=~(0x01<<0);
	TIM7->CR1 &=~(0X01);
}


