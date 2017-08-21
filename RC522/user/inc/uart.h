#ifndef _UART_H_
#define _UART_H_

#include "stm32f4xx.h"

#define USART2_MAX_RECV_LEN		1024				//�����ջ����ֽ���
#define USART2_MAX_SEND_LEN		1024				//����ͻ����ֽ���

extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; 		//���ջ���,���USART2_MAX_RECV_LEN�ֽ�
extern u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 		//���ͻ���,���USART2_MAX_SEND_LEN�ֽ�
extern u16 USART2_RX_STA;   											//��������״̬
extern u8 rec_buf[1000];
extern u8 rec_sta; 
extern u16 rec_num;

void uart_init(u32 bond);
void uart_send_string(s8 *p);
void uart1_interrupt_init(u32 bond);
void usart2_init(u32 bound);
void u2_printf(char* fmt,...);

#endif
