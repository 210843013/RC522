#include "delay.h"
#include "stm32f4xx.h"

void delay_us(u32 n)
{
	//�趨��װ��ֵ
	if(n>1)
	TIM7->ARR =n-1;
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


void delay_ms(u32 n)
{
		//�趨��װ��ֵ
	TIM7->ARR =n*10-1;
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


