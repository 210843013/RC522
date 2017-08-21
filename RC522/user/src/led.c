#include "led.h"
#include "stm32f4xx.h"

void led_init(void)
{
	  //GPIOF��6��9��10��GPIOC��0�˿�ʱ��ʹ��
		RCC->AHB1ENR |=(0x01<<5) | (0x01<<2);				//����GPIOF��C��ʱ��	
  
		//GPIOF��6��9��10��GPIOC��0 Ϊͨ�����ģʽ
		GPIOF->MODER &=~(0x03<<12);		//F�˿�6����
		GPIOF->MODER |=0X01<<12;			//F�˿�6Ϊͨ�����ģʽ
	
		GPIOF->MODER &=~(0x03<<18);		//F�˿�9����
		GPIOF->MODER |=0X01<<18;
	
		GPIOF->MODER &=~(0x03<<20);		//F��10����
		GPIOF->MODER |=0X01<<20;
	
		GPIOC->MODER &=~(0x03<<0);		//C�˿�6����
		GPIOC->MODER |=0X01<<0;
	
		//GPIOF��6��9��10��GPIOC��0�������
		GPIOF->OTYPER &=~(0X01<<6);		//F�˿�6Ϊ�������
	  GPIOF->OTYPER &=~(0X01<<9);
		GPIOF->OTYPER &=~(0X01<<10);
		GPIOC->OTYPER &=~(0X01<<0);
	
		//GPIOF��6��9��10��GPIOC��0����	
		GPIOF->OSPEEDR &=~(0x03<<12); //Ϊ����
		GPIOF->OSPEEDR &=~(0x03<<18);
		GPIOF->OSPEEDR &=~(0x03<<20);
		GPIOC->OSPEEDR &=~(0x03<<0);
		
		//GPIOF��6��9��10��GPIOC��0������������
		GPIOF->PUPDR &=~(0x03<<12);		//������������
		GPIOF->PUPDR &=~(0x03<<18);
		GPIOF->PUPDR &=~(0x03<<20);
		GPIOC->PUPDR &=~(0x03<<0);
		
		GPIOF->ODR |=0x01<<6;
		GPIOF->ODR |=0x01<<9;
		GPIOF->ODR |=0x01<<10;
		GPIOC->ODR |=0x01<<0;
	}		









