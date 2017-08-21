#include "stm32f4xx.h" 
#include "spi.h"

//ʹ��NRF2401������ˢ��ģ��RC522����ͨ��
//SPI��������ʼ��
void SPI1_Init(void)
{
	u16 tempreg=0;
	//SPI1 MISO   MOSI  SCK
	RCC->AHB1ENR |=0X01<<1;
	GPIOB->MODER &=~(0X03<<6 |0X03<<8 |0X03<<10 );
	GPIOB->MODER |=0X02<<6 |0X02<<8 |0X02<<10 ;
	
	GPIOB->OTYPER &=~(0X01<<3 | 0X01<<5 );  //�������
	GPIOB->OSPEEDR |=(0X02<<6 |0X02<<10 );  //����ٶ�50M
	GPIOB->PUPDR &=~(0X03<<6 |0X03<<8 | 0X03<<10 ); //��������
	
	GPIOB->AFR[0] &=0xFF000FFF;
	GPIOB->AFR[0] |=0x00555000;
	
	//NRF RC522_RST(CE)--->PG6  CS---->PG7
	RCC->AHB1ENR |=0X01<<6;	
	
	GPIOG->MODER &=~(0x03<<12 | 0x03<<14);			//ģʽ----ͨ�����ģʽ��01��
	GPIOG->MODER |=(0X01<<12 | 0X01<<14 );

	GPIOG->OTYPER &= ~(0X01<<6 | 0X01<<7);		//�������----����

	GPIOG->OSPEEDR |=(0x02<<12 | 0x02<<14);		//����ٶ�----50M

//	GPIOG->PUPDR &=~(0x03<<12 | 0x03<<14); //����	
	GPIOG->PUPDR |=(0x01<<12 | 0x01<<14);			//����
	
	
//	//SPIģ��ʹ��
//	RCC->APB2ENR |=0x01<<12;
//	RCC->APB2RSTR |=0x01<<12;			//��λģ��
//	RCC->APB2RSTR &=~(0x01<<12);	//ֹͣ��λģ��
//	
//	SPI1->CR1 &=~(0x01<<15);   	//˫�ߵ���ͨ��
//	SPI1->CR1 &=~(0x01<<11);		//8λ����֡��ʽ
//	SPI1->CR1|=1<<1; 						//CPOL=0ʱ����ģʽ��SCKΪ1  CPOL=1
//	SPI1->CR1|=1<<0; 						//���ݲ����ӵڶ���ʱ����ؿ�ʼ,CPHA=1  
//	SPI1->CR1|=0<<7; 						//MSBfirst
//	SPI1->CR1 &=~(0x01<<10);   //ȫ˫��
//	SPI1->CR1 |=0x03<<8;			 //�������
//	SPI1->CR1 &=~(0x07<<3);		 
//	SPI1->CR1|=7<<3;						//������f pclk/256
//	SPI1->CR1 |=0x01<<2;				//��ģʽ
//	SPI1->CR1 &=~(0x03<<0);			//ģʽ0  ����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
//															//����ͬ��ʱ�ӵĵ�һ�������أ��������½������ݱ�����
//	SPI1->CR1 |=0x01<<6;	

	

		RCC->AHB1ENR|=1<<1;    	//ʹ��PORTBʱ��	   
		RCC->APB2ENR|=1<<12;   	//SPI1ʱ��ʹ�� 

//		GPIOB->MODER &=~(0X03<<6 |0X03<<8 |0X03<<10 );
//		GPIOB->MODER |=0X02<<6 |0X02<<8 |0X02<<10 ;
//		
//		GPIOB->OTYPER &=~(0X01<<3 | 0X01<<5 );  //�������
//		GPIOB->OSPEEDR |=(0X02<<6 |0X02<<10 );  //����ٶ�50M
//		GPIOB->PUPDR &=~(0X03<<6 |0X03<<8 | 0X03<<10 ); //��������
//		
//		GPIOB->AFR[0] &=0xFF000FFF;//����
//		GPIOB->AFR[0] |=0x00555000;//����ΪAF5

	//����ֻ���SPI�ڳ�ʼ��
	RCC->APB2RSTR|=1<<12;	//��λSPI1
	RCC->APB2RSTR&=~(1<<12);//ֹͣ��λSPI1
	tempreg|=0<<10;			//ȫ˫��ģʽ	
	tempreg|=1<<9;			//���nss����
	tempreg|=1<<8;			 
	tempreg|=1<<2;			//SPI����  
	tempreg|=0<<11;			//8λ���ݸ�ʽ	
	tempreg|=1<<1;			//����ģʽ��SCKΪ1 CPOL=1 
	tempreg|=1<<0;			//���ݲ����ӵ�2��ʱ����ؿ�ʼ,CPHA=1  
 	//��SPI1����APB2������.ʱ��Ƶ�����Ϊ84MhzƵ��.
	tempreg|=7<<3;			//Fsck=Fpclk1/256
	tempreg|=0<<7;			//MSB First  
	tempreg|=1<<6;			//SPI���� 
	SPI1->CR1=tempreg; 		//����CR1  
	SPI1->I2SCFGR&=~(1<<11);//ѡ��SPIģʽ
	SPI1_ReadWriteByte(0xff);//��������		 
	
}

void spi_delay(void)
{
	u32 i=5;
	while(i--);
}

//������_data---�����͵�����
//����ֵ����ȡ����һ���ֽ�����
u8 SPI1_ReadWriteByte(u8 _data)
{
	while(!(SPI1->SR & 0x01<<1));		//�ȴ����ͻ�����Ϊ��
	SPI1->DR=_data;		
	while(!(SPI1->SR & 0x01<<0));		//�ȴ����ջ�����Ϊ��
	return SPI1->DR;
}

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI1_Read_WriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ������
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����					    
}

