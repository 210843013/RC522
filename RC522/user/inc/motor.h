#ifndef __MOTOR_H
#define __MOTOR_H	

#include "io_bit.h"

//���ڻ���֪������Ľӿڣ�������ʱ�ͷ�д	
//�����io���������������ϵ�
#define qmotor1 PFout(1)       // PF1ǰ����������ź�
#define qmotor2 PFout(2)       // PF2ǰ��������ź�	
#define qmotor3 PFout(3)       // PF3������������ź�
#define qmotor4 PFout(4)       // PF4������������ź�

void motor_Init(void);           //��ʼ��

void motorCW(void);//��ת60��

void motorCCW(void);//��ת60��


#endif



