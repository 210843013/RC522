#include "stm32f4xx.h"
#include "dma.h"
#include "adc.h"
#include "delay.h"


//ADC��DMA
void dma2_init(u32 par,u16 ndtr,u32 mar)
{
	//DMA2ģ��ʱ��ʹ��
	RCC->AHB1ENR |=0x01<<22;
	
	DMA2_Stream4->CR &=~(0x01<<0);		//��ֹ������
	
	DMA2_Stream4->CR &=~(0x07<<25);			
	DMA2_Stream4->CR |=0X00<<25;			//ͨ��0
	
	DMA2_Stream4->CR &=~(0x03<<23);		//�洢�����δ���
	DMA2_Stream4->CR &=~(0x03<<21);		//���赥�δ���
	
	DMA2_Stream4->CR &=~(0x03<<16);		//���ȼ�Ϊ��
	
	//DMA2_Stream4->CR &=~(0x03<<13);		
	//DMA2_Stream2->CR |=0x01<<13;			//�洢�����ݴ�СΪ16λ
	DMA2_Stream4->CR &=~(0x03<<11);
	DMA2_Stream4->CR |=0x01<<11;				//�������ݾݴ�СΪ16λ
	
	DMA2_Stream4->CR |=0x01<<10;			//�洢����ַ����ģʽ
	DMA2_Stream4->CR &=~(0x01<<9);		//�����ַ�̶�
	DMA2_Stream4->CR &=~(0x01<<8);		//��ѭ��ģʽ
	
	DMA2_Stream4->CR &=~(0x03<<6);		//���赽�洢��
	
	DMA2_Stream4->PAR=par;						//�����ַ
	DMA2_Stream4->NDTR =ndtr;
	DMA2_Stream4->M0AR =mar;
	
	DMA2_Stream4->FCR &=~(0x01<<2);		//ʹ��ֱ��ģʽ��������洢�����ݿ����Ҫһ��	
				
	DMA2_Stream4->CR |=0x01<<4;				//������ж�
	
	
	
	
	NVIC_SetPriority(DMA2_Stream4_IRQn,NVIC_EncodePriority(7-2,1,0));
	NVIC_EnableIRQ(DMA2_Stream4_IRQn);
	DMA2_Stream4->CR |=0x01<<0;				//����������	
	
	ADC1->CR2 &=~(0x03<<8);
	ADC1->CR2 |=0x03<<8;				//ʹ��DMA
	ADC1->CR2 |=0X01<<30;    //���������ʼת��	





}


void dma2_M_to_M_init(u32 par,u16 ndtr,u32 mar)
{
	//DMA2ģ��ʱ��ʹ��
	RCC->AHB1ENR |=0x01<<22;
	
	DMA2_Stream0->CR &=~(0x01<<0);		//��ֹ������
	
	DMA2_Stream0->CR &=~(0x07<<25);
	DMA2_Stream0->CR |=0x00<<25;			//ͨ��0
	
	DMA2_Stream0->CR &=~(0x03<<23);		//
	DMA2_Stream0->CR &=~(0x03<<21);
	
	DMA2_Stream0->CR &=~(0x03<<16);		//���ȼ�Ϊ��
	
	//DMA2_Stream0->CR |=0x01<<13;			//�洢�����ݴ�СΪ16λ
	DMA2_Stream0->CR &=~(0x03<<11);
	DMA2_Stream0->CR |=0x01<<11;				//�������ݾݴ�СΪ16λ
	
	DMA2_Stream0->CR |=0x01<<10;			//�洢����ַ����ģʽ
	DMA2_Stream0->CR |=0x01<<9;				//�����ַ����ģʽ
	DMA2_Stream0->CR &=~(0x01<<8);		//��ѭ��ģʽ
	
	//DMA2_Stream0->FCR |=0x01<<2;			//��ֱֹ��ģʽ
	
	
	DMA2_Stream0->CR &=~(0x03<<6);	
	DMA2_Stream0->CR |=0x02<<6;				//�洢�����洢��
	
				
	
	DMA2_Stream0->PAR=par;						//�����ַ
	DMA2_Stream0->NDTR =ndtr;
	DMA2_Stream0->M0AR =mar;
	
//	DMA2_Stream0->FCR &=~(0X01<<2);  //ֱ�Ӵ���ģʽ,������洢�����ݿ����Ҫһ��	

	

	//DMA2_Stream0->CR |=0x01;   //����������	
	

}



//mar:�洢���ĵ�ַ
//ndtr����������
void dma2_transf(u16 ndtr,u32 mar)
{
	DMA2->LIFCR |=0x01<<21;		//����ɴ����־
	DMA2_Stream2->CR &=~(0x01<<0); 	//�ر�������
	while(DMA2_Stream2->CR & 0x01<<0);	//�ȴ�����������
	DMA2_Stream2->NDTR =ndtr;
	DMA2_Stream2->M0AR =mar;
	DMA2_Stream2->CR |=0x01<<0;				//����������
	
}

void DMA2_Stream4_IRQHandler(void)
{
	if(DMA2->HISR & (0x01<<5))
	{
		ADC1->CR2 &=~(0x01<<0);				//buʹ��ADC
		DMA2->HIFCR |=0x01<<5;
		while(DMA2->HISR & (0x01<<5));
		DMA2_Stream0->CR |=0x01<<0;		//����������0
	}
}

///////////////////////////////////////USART2 DMA�������ò���//////////////////////////////////	   		    
//DMA1�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_CHx:DMAͨ��CHx
//cpar:�����ַ
//cmar:�洢����ַ    
void UART_DMA_Config(u32 cpar,u32 cmar)
{
 	RCC->AHB1ENR|=1<<21;			//����DMA1ʱ��
	delay_us(5);	
	DMA1_Stream6->PAR=cpar; 					//DMA1 �����ַ 
	DMA1_Stream6->M0AR=cmar; 					//DMA1,�洢����ַ	
	DMA1_Stream6->CR &=~(0x01<<0);		//��ֹ������
	DMA1_Stream6->CR &=~(0x07<<25);			
	DMA1_Stream6->CR |=0X04<<25;			//ͨ��4
	DMA1_Stream6->CR &=~(0x03<<23);		//�洢�����δ���
	DMA1_Stream6->CR &=~(0x03<<21);		//���赥�δ���
	DMA1_Stream6->CR &=~(0x03<<16);
	DMA1_Stream6->CR |=(0x01<<16);		//���ȼ�Ϊ��
	DMA1_Stream6->CR &=~(0x03<<13);		//�洢������λ��С8λ
	DMA1_Stream6->CR &=~(0x03<<11);		//�������ݾݴ�СΪ8λ				
	DMA1_Stream6->CR |=0x01<<10;			//�洢����ַ����ģʽ
	DMA1_Stream6->CR &=~(0x01<<9);		//�����ַ������ģʽ(�̶���
	DMA1_Stream6->CR &=~(0x01<<8);		//��ѭ��ģʽ	
	DMA1_Stream6->CR &=~(0x03<<6);		
	DMA1_Stream6->CR |=(0x01<<6);		//�洢�������� 	
} 
//����һ��DMA����
//ndtr����������
void UART_DMA_Enable(u16 ndtr)
{
	DMA1->HIFCR |=0x01<<21;		//����ɴ����־
	DMA1_Stream6->CR &=~(0x01<<0); 	//�ر�������
	while(DMA1_Stream6->CR & 0x01<<0);	//�ȴ�����������
	DMA1_Stream6->NDTR =ndtr;
	DMA1_Stream6->CR |=0x01<<0;				//����������
	
	
}	   



