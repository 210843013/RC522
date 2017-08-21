#include "stm32f4xx.h"
#include "delay.h"
#include "led.h"
#include "uart.h"
#include "stdio.h"
#include "string.h"
#include "systick_delay.h"
#include "spi.h"
#include "time.h"
#include "math.h"
#include "beep.h"
#include "malloc.h"
#include "rc522.h"
#include "motor.h"

//////////////////////////////////////////////////////////
//M1���֞�16���ȅ^��ÿ���ȅ^��4�K���K0���K1���K2���K3���M��
//�҂�Ҳ��16���ȅ^��64���K���^����ַ��̖0~63
//��0�ȅ^�ĉK0�����^����ַ0�K��������춴�ŏS�̴��a���ѽ��̻������ɸ���
//ÿ���ȅ^�ĉK0���K1���K2�锵���K������춴�Ŕ���
//ÿ���ȅ^�ĉK3����ƉK���^����ַ�K3��7��11....�����������ܴaA����ȡ���ơ��ܴaB��

/*******************************
*����˵����
*1--CS  <----->PG7
*2--SCK <----->PB3
*3--MOSI<----->PB5
*4--MISO<----->PB4
*5--����
*6--GND <----->GND
*7--RST(CE) <----->PG6
*8--VCC <----->VCC
************************************/

int main(void)	
{
	u8 temp=0;
	time7_delay_init();	
	led_init();//LED��ʼ��
	beep_init();
	motor_Init();
	uart_init(9600);
	InitRc522(); 	//RC522
	led2=0;
  BEEP=0;			
	while(1)
	{ 
			RC522_Entrance_Guard();
			delay_ms(1000);
			printf("TEXT\r\n"); 
	}
}




