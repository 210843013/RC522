#include "stm32f4xx.h"
#include "beep.h"

void beep_init(void)
{
		GPIO_InitTypeDef GPIO_InitStruct ;
	//�˿�ʱ��ʹ��

	RCC_AHB1PeriphClockCmd  (RCC_AHB1Periph_GPIOF,ENABLE) ;

	//�˿�ģʽ����

	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed=GPIO_Low_Speed;
	GPIO_Init (GPIOF, &GPIO_InitStruct ); 
	BEEP=0;
}



