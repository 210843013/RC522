#include "stm32f4xx.h"
#include "key.h"
#include "led.h"
#include "time.h"
#include "uart.h"

//10ms��һ���ж�
void time6_key_scanf_init(void)
{
	//ʱ��ʹ��
	RCC->APB1ENR |=0X01<<4;
	//�趨���üĴ���
	TIM6->CR1 |=0X01<<7; //ARR�л���
	TIM6->CR1 &=~(0x03<<1); //ʹ�ܸ����¼�������UGλ����������¼�	
	//�趨��װ��ֵ
	TIM6->ARR =99;
	//�趨Ԥ��Ƶ
	TIM6->PSC =8399;
	//��cnt
	TIM6->CNT =0;
	//����UGλ���������¼�
	TIM6->EGR |=0x01;
	//����±�־
	TIM6->SR &=~(0x01<<0);
	
	//ʹ���ж�
	TIM6->DIER |=0X01<<0;
	
	NVIC_SetPriority(TIM6_DAC_IRQn,NVIC_EncodePriority(7-2,3,3));  //�������ȼ�
	NVIC_EnableIRQ(TIM6_DAC_IRQn); //�ⲿ�ж�ʹ�ܣ�ϵͳ�ж�û�����ʹ�ܣ�

	//����ʱ��	
	TIM6->CR1 |=0X01<<0; 
}


void time7_delay_init(void)
{
	
	//ʱ��ʹ��
	RCC->APB1ENR |=0X01<<5;
	//�趨���üĴ���
	TIM7->CR1 |=0X01<<7; //ARR�л���
	TIM7->CR1 &=~(0x03<<1); //ʹ�ܸ����¼�������UGλ����������¼�
	
}



void time10_pwm_led_init(void)
{
	//�˿�ʱ��ʹ��pf  6
	RCC->AHB1ENR |=0X01<<5;
	GPIOF->MODER &=~(0X03<<12);
	GPIOF->MODER |=0X02<<12;     //���ù���
	GPIOF->OTYPER &=~(0x01<<6);   //�������
	GPIOF->OSPEEDR &=~(0X03<<12);  //�������ٶ�2M
	GPIOF->PUPDR  &=~(0X03<<12);   //����������
	
	GPIOF->AFR[0] &=0XF0FFFFFF;
	GPIOF->AFR[0] |=0X03000000;   //pf6Ϊtime10��ch1
	
	//ʱ��ʹ��
	RCC->APB2ENR |=0X01<<17;
	
	TIM10->CR1 |=0X01<<7;
	TIM10->CR1 &=~(0X03<<1);
	
	TIM10->CCMR1 &=~(0X03);    //ͨ��1��Ϊ���
	TIM10->CCMR1 |=0x01<<3;    //CCR1��Ԥװ��
	
	TIM10->CCMR1 &=~(0x07<<4);
	TIM10->CCMR1 |=0X06<<4;    //pWMģʽ1
	
	TIM10->PSC =1679;    //CNT����Ƶ��10k
	TIM10->ARR =999;     //����10000��
	TIM10->CNT=0;        //������ֵ
	TIM10->CCR1=0;
	
	TIM10->EGR |=0X01;  //����UGλ��������ؼĴ���
	TIM10->SR &=~(0X01);  //����±�־
	
	TIM10->CCER |=0X01<<1;  //ͨ��1�͵�ƽ��Ч
	TIM10->CCER |=0X01<<0;  //��ͨ��1
	
	TIM10->CR1 |=0X01<<0;  //����ʱ��
}

void time12_pwm_LCD_init(void)
{
	//�˿�ʱ��ʹ��pb  15
	RCC->AHB1ENR |=0X01<<1;
	GPIOB->MODER &=~(0X03<<30);
	GPIOB->MODER |=0X02<<30;     //���ù���
	GPIOB->OTYPER &=~(0x01<<15);   //�������
	GPIOB->OSPEEDR &=~(0X03<<30);  //�������ٶ�2M
	GPIOB->PUPDR  &=~(0X03<<30);   //����������
	
	GPIOB->AFR[1] &=0X0FFFFFFF;
	GPIOB->AFR[1] |=0X90000000;   //pb15Ϊtime12��ch2
	
	//ʱ��ʹ�� time12
	RCC->APB1ENR |=0X01<<6;
	
	TIM12->CR1 |=0X01<<7;				//ARR�Ĵ������л���
	TIM12->CR1 &=~(0X03<<1);		//����
	
	TIM12->CCMR1 &=~(0X03<<8);    //ͨ��2��Ϊ���
	TIM12->CCMR1 |=0x01<<11;    		//CCR2��Ԥװ��
	
	TIM12->CCMR1 &=~(0x07<<12);
	TIM12->CCMR1 |=0X06<<12;    //pWMģʽ1
	
	TIM12->PSC =6719;    //CNT����Ƶ��100k
	TIM12->ARR =249;     //����1000��
	TIM12->CNT=0;        //������ֵ
	TIM12->CCR2=10;
	
	TIM12->EGR |=0X01;  //����UGλ��������ؼĴ���
	TIM12->SR &=~(0X01);  //����±�־
	
	TIM12->CCER &=~(0X01<<5);  //ͨ��2  �ߵ�ƽ��Ч
	TIM12->CCER |=0X01<<4;  //��ͨ��2
	
	TIM12->CR1 |=0X01<<0;  //����ʱ��
}


u8 interrupt_key=key_no;

void  TIM6_DAC_IRQHandler (void)
{
	u8 key;
	TIM6->SR &=~(0x01<<0);
	key=key_scanf( );
	if(key!=key_no)
	{
		interrupt_key=key;
	}
}

//��ʱ��4�жϷ������		    
void TIM4_IRQHandler(void)
{ 	
	if(TIM4->SR&0X01)//�Ǹ����ж�
	{	 			
    USART2_RX_BUF[USART2_RX_STA&0X3FFF]=0;		
		USART2_RX_STA|=1<<15;	//��ǽ������
		TIM4->SR&=~(1<<0);		//����жϱ�־λ		   
		TIM4_Set(0);			//�ر�TIM4  
	}	    
}
//����TIM4�Ŀ���
//sta:0���ر�;1,����;
void TIM4_Set(u8 sta)
{
	if(sta)
	{
			TIM4->CR1|=1<<0;     //ʹ�ܶ�ʱ��4
    	TIM4->CNT=0;         //���������
	}
	else TIM4->CR1&=~(1<<0);//�رն�ʱ��4	   
}
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
void TIM4_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<2;	//TIM4ʱ��ʹ��    
 	TIM4->ARR=arr;  	//�趨�������Զ���װֵ   
	TIM4->PSC=psc;  	//Ԥ��Ƶ��
 	TIM4->DIER|=1<<0;   //��������ж�				
 	TIM4->CR1|=0x01;  	//ʹ�ܶ�ʱ��4	
	NVIC_SetPriority(TIM4_IRQn,NVIC_EncodePriority(7-2,1,1));//ʹ�ܴ����ж�	
	NVIC_EnableIRQ(TIM4_IRQn);
								 
}



