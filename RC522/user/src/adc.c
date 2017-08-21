#include "adc.h"
#include "stm32f4xx.h"


void adc_init(void)
{
	//�˿�Aʱ��ʹ�� PA5ģ��
	RCC->AHB1ENR |=0x01<<0;
	GPIOA->MODER |=0x03<<10;
	//ADCģ��ʹ��
	RCC->APB2ENR |=0x01<<8;
	
	ADC1->CR1 &=~(0x03<<24);		//12λ�ֱ���
	ADC1->CR1 &=~(0x01<<8);			//��ɨ��ģʽ
	
	ADC1->CR2 &=~(0x01<<11); 		//�Ҷ���
	ADC1->CR2 |=0x01<<10;				//ÿ������ת������EOC����1
	ADC1->CR2 &=~(0x01<<1);			//����ת��ģʽ
	
	ADC1->SMPR2 &=~(0x07<<15);
	ADC1->SMPR2 |=0x07<<15;			//480����������
	
	ADC1->SQR1 &=~(0x0f<<20);		//����ͨ�����г���Ϊ1��ֻ��һ��ͨ��
	ADC1->SQR3 &=~(0x1f<<0);
	ADC1->SQR3 |=0x05<<0;				//ͨ�����Ϊ5
	
	ADC->CCR &=~(0x03<<16);
	ADC->CCR |=0x01<<16;				//ADC 4��Ƶ 82M/4=21M
	
	ADC1->CR2 |=0x01<<0;				//ʹ��ADC
	
	
	
}


void adc_temp_init(void)
{
//	//�˿�Aʱ��ʹ�� PA5ģ��
//	RCC->AHB1ENR |=0x01<<0;
//	GPIOA->MODER |=0x03<<10;
	//ADCģ��ʹ��
	RCC->APB2ENR |=0x01<<8;
	
	ADC1->CR1 &=~(0x03<<24);		//12λ�ֱ���
	ADC1->CR1 &=~(0x01<<8);			//��ɨ��ģʽ
	
	ADC1->CR2 &=~(0x01<<11); 		//�Ҷ���
	ADC1->CR2 |=0x01<<10;				//ÿ������ת������EOC����1
	ADC1->CR2 &=~(0x01<<1);			//����ת��ģʽ
	
	ADC1->SMPR1 &=~(0x07<<18);	//16ͨ����
	ADC1->SMPR1 |=0x07<<18;			//480����������
	
	ADC1->SQR1 &=~(0x0f<<20);		//����ͨ��һ��ת��
	ADC1->SQR3 &=~(0x1f<<0);
	ADC1->SQR3 |=0x10<<0;				//ͨ�����16
	
	ADC->CCR &=~(0x03<<16);
	ADC->CCR |=0x01<<16;				//ADC 4��Ƶ 82M/4=21M
	
	ADC->CCR |=0x01<<23;				//ʹ���¶ȴ�������Vrefintͨ��
	
	ADC1->CR2 |=0x01<<0;				//ʹ��ADC
	
	
	
}



u16 get_adc1_ch5(void)
{
	u16 i,j,buf[100],a;

	for(i=0;i<10;i++)
	{
		//ת����ʼ
		ADC1->CR2 |=0x01<<30;
		//�ȴ�ת������
		while(!(ADC1->SR & 0x01<<1));
		buf[i]=ADC1->DR;
	}
	
	for(i=0;i<10;i++)
		for(j=i+1;j<10;j++)
		{
			if(buf[i]>buf[j])
			{
				a=buf[i];
				buf[i]=buf[j];
				buf[j]=a;
			}
		}
		
		a=0;
		for(j=1;j<9;j++)
		{
			a+=buf[j];
		}
		
		return a/8.0;
	
}



u16 get_adc1_temp_ch16(void)
{
	u16 i,j,buf[100],a;
	//ת����ʼ
	for(i=0;i<10;i++)
	{
		ADC1->CR2 |=0x01<<30;
		//�ȴ�ת������
		while(!(ADC1->SR & 0x01<<1));
		buf[i]=ADC1->DR;
	}
	
	for(i=0;i<10;i++)
		for(j=i+1;j<10;j++)
		{
			if(buf[i]>buf[j])
			{
				a=buf[i];
				buf[i]=buf[j];
				buf[j]=a;
			}
		}
		
		a=0;
		for(j=1;j<9;j++)
		{
			a+=buf[j];
		}
		
		return a/8.0;
	
}



