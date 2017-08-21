#include "stm32f4xx.h" 
#include "motor.h"
#include "delay.h"
#include "beep.h"
#include "uart.h"
#include "stdio.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 

//���������������	   
//���ڻ���֪������Ľӿڣ�������ʱ�ͷ�д	
//�����io���������������ϵ�
////////////////////////////////////////////////////////////////////////////////// 	   


void motor_Init(void)
{
//	 GPIO_InitTypeDef  GPIO_InitStructure; 	
//   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);	 //ʹ��PF�˿�ʱ��
//	 
//	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;	 //IO-->PF1��PF2��PF3��PF4 �˿�����	                     //�������
//	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
//   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
//	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		                     //IO���ٶ�Ϊ50MHz
//	 
//	GPIO_Init(GPIOF, &GPIO_InitStructure);					                     //�����趨������ʼ��PF1��PF2��PF3��PF4
	
	
	//��PF����ʱ��
	RCC->AHB1ENR |=(1<<5);
	/**PF1��PF2��PF3��PF4����Ϊ�������**/
	//�˿�ģʽ�Ĵ���
	GPIOF->MODER &=~(0xFF<<2);  //����
	GPIOF->MODER |=(0x55<<2);     //����Ϊͨ�����ģʽ
	//�������
	GPIOF->OTYPER &=~(0xF<<1);   //�������
	//�������
	GPIOF->OSPEEDR &=~(0xFF<<2); //����
	GPIOF->OSPEEDR |=(0xaa<<2); //�������50M
	//������
	GPIOF->PUPDR &=~(0xFF<<2);  //����
	GPIOF->PUPDR|=(0x55<<2);     //����Ϊ����
}
/********************************************************************************
�����Բ������������ʽ��
�����ģ� A-B-C-D;
˫���ģ�AB-BC-CD-DA;
��˫���ģ�A-AB-B-BC-C-CD-D-DA
���ڵ�˫���ĵķ�ʽ��ÿ��ת���Ƕ�Ϊ3.75�㣬����˫����ÿ��ת���ĽǶ�Ϊ7.5��
˫���Բ������������ʽ��
�����ģ�A/-B-A-B/
˫���ģ�A/B-AB-AB/-A/B/
��˫���ģ�A/-A/B-B-AB-A-AB/-B/-A/B/
ʹ�õĵ����Բ��������������ʽΪ��˫���ģ�����оƬ��ULN2003
unsigned char  FFW[8]={0xf1,0xf3,0xf2,0xf6,0xf4,0xfc,0xf8,0xf9}; //��ת
unsigned char  FFZ[8]={0xf9,0xf8,0xfc,0xf4,0xf6,0xf2,0xf3,0xf1}; //��ת
**********************************************************************************/



/************************************
	�����ת1���ĵ� �Ƕ�ԼΪ0.7��
	�����ת90��
************************************/
void motorCW(void)
{
	int i;
	for(i=0;i<130;i++)
	{
		qmotor1=0;qmotor2=0;qmotor3=0;qmotor4=1;
		delay_ms(1);
		qmotor1=0;qmotor2=0;qmotor3=1;qmotor4=1;
		delay_ms(1);
		qmotor1=0;qmotor2=0;qmotor3=1;qmotor4=0;
		delay_ms(1);
		qmotor1=0;qmotor2=1;qmotor3=1;qmotor4=0;
		delay_ms(1);
		qmotor1=0;qmotor2=1;qmotor3=0;qmotor4=0;
		delay_ms(1);
		qmotor1=1;qmotor2=1;qmotor3=0;qmotor4=0;
		delay_ms(1);
		qmotor1=1;qmotor2=0;qmotor3=0;qmotor4=0;
		delay_ms(1);
		qmotor1=1;qmotor2=0;qmotor3=0;qmotor4=1;
		delay_ms(1);
			
	}
}

/************************************
             �����ת90��
************************************/
void motorCCW(void)
{
	int i;
	for(i=0;i<130;i++)
	{
		qmotor1=1;qmotor2=0;qmotor3=0;qmotor4=1;
		delay_ms(1);
		qmotor1=1;qmotor2=0;qmotor3=0;qmotor4=0;
		delay_ms(1);
		qmotor1=1;qmotor2=1;qmotor3=0;qmotor4=0;
		delay_ms(1);
		qmotor1=0;qmotor2=1;qmotor3=0;qmotor4=0;
		delay_ms(1);
		qmotor1=0;qmotor2=1;qmotor3=1;qmotor4=0;
		delay_ms(1);
		qmotor1=0;qmotor2=0;qmotor3=1;qmotor4=0;
		delay_ms(1);
		qmotor1=0;qmotor2=0;qmotor3=1;qmotor4=1;
		delay_ms(1);
		qmotor1=0;qmotor2=0;qmotor3=0;qmotor4=1;
		delay_ms(1);
			
	}
}





