#include "stm32f4xx.h"
#include "rc522.h"
#include "delay.h"
#include "beep.h"
#include "uart.h"
#include "spi.h"
#include "stdio.h"
#include "string.h"
#include "motor.h"

u8 RC522_Rtc_flag=1;

u8 CT[2];  //������
u8 SN[4]; //����
u8 RFID[16];			//���RFID 
u8 RFID1[16];
//unsigned char RFID1[16]={0xff,0xff,0xff,0xff,0xff,0xff,  0xFF,0x07,0x80,0x69,  0xff,0xff,0xff,0xff,0xff,0xff};
u8 data[16]={0xff,0xff,0xff,0xff,0xff,0xff,  0xFF,0x07,0x80,0x69,  0xff,0xff,0xff,0xff,0xff,0xff};
u8 data1[16]={0x07,0x80,0x69,  0xff,0xff,0xff,0xff,0xff,0xff,	0xff,0xff,0xff,0xff,0xff,0xff};
u8 KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff};
//u8 KEY[6]={0x11,0x22,0x33,0x44,0x55,0x66};
void print_info(unsigned char *p,int cnt)
{
  int i;
	for(i=0;i<cnt;i++)
	{
		printf("0x%X ",p[i]);
	}
	printf("\r\n");
}


void delay_ns(u32 ns)
{
  u32 i;
  for(i=0;i<ns;i++)
  {
    __nop();
    __nop();
    __nop();
  }
}

//u8 SPIWriteByte(u8 Byte)
//{
//	while((SPI1->SR&0X02)==0);		//�ȴ���������	  
//	SPI2->DR=Byte;	 	            //����һ��byte   
//	while((SPI1->SR&0X01)==0);      //�ȴ�������һ��byte  
//	return SPI1->DR;          	    //�����յ�������			
//}

////SPIx ��дһ���ֽ�
////TxData:Ҫд����ֽ�
////����ֵ:��ȡ�����ֽ�
//u8 SPI2_ReadWriteByte(u8 TxData)
//{		
//	u8 retry=0;				 	
//	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
//		{
//		retry++;
//		if(retry>200)return 0;
//		}			  
//	SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx����һ������
//	retry=0;

//	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
//		{
//		retry++;
//		if(retry>200)return 0;
//		}	  						    
//	return SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����					    
//}


////SPI �ٶ����ú���
////SpeedSet:
////SPI_BaudRatePrescaler_2   2��Ƶ   
////SPI_BaudRatePrescaler_8   8��Ƶ   
////SPI_BaudRatePrescaler_16  16��Ƶ  
////SPI_BaudRatePrescaler_256 256��Ƶ 
//  
//void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
//{
//  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
//	SPI2->CR1&=0XFFC7;
//	SPI2->CR1|=SPI_BaudRatePrescaler;	//����SPI2�ٶ� 
//	SPI_Cmd(SPI2,ENABLE); 
//} 




////void SPI2_Init(void)	
////{
////	GPIO_InitTypeDef GPIO_InitStructure;
////  SPI_InitTypeDef  SPI_InitStructure;
//// 	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOF, ENABLE );//PORTBʱ��ʹ�� 
////	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,ENABLE );//SPI2ʱ��ʹ��

////	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	 //IO-->PF0��PF1 �˿�����
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
////	GPIO_Init(GPIOF,&GPIO_InitStructure);					 //�����趨������ʼ��PF0��PF1
////	GPIO_ResetBits(GPIOF,GPIO_Pin_1);			             //PF1�����
////    //GPIO_SetBits(GPIOF,GPIO_Pin_0);

////	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15����������� 
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB
////	
//////	
//////	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 ;
//////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //PB13/14/15����������� 
//////	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB

////	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
////	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
////	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
////	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
////	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//����ͬ��ʱ�ӵĵ�һ�������أ��������½������ݱ�����
////	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
////	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
////	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
////	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
////	SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
//// 
////	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����
////	
////	//SPI2_ReadWriteByte(0xff);//��������	
////}



void InitRc522(void)
{
  SPI1_Init();
  PcdReset();
  PcdAntennaOff();
  delay_ms(2);  
  PcdAntennaOn();
  M500PcdConfigISOType( 'A' );
}
void Reset_RC522(void)
{
  PcdReset();
  PcdAntennaOff();
  delay_ms(2);  
  PcdAntennaOn();
}                         
/////////////////////////////////////////////////////////////////////
//��    �ܣ�Ѱ��
//����˵��: req_code[IN]:Ѱ����ʽ
//                0x52 = Ѱ��Ӧ�������з���14443A��׼�Ŀ�
//                0x26 = Ѱδ��������״̬�Ŀ�
//          pTagType[OUT]����Ƭ���ʹ���
//                0x4400 = Mifare_UltraLight
//                0x0400 = Mifare_One(S50)
//                0x0200 = Mifare_One(S70)
//                0x0800 = Mifare_Pro(X)
//                0x4403 = Mifare_DESFire
//��    ��: �ɹ�����MI_OK
/////////////////////////////////////////////////////////////////////
char PcdRequest(u8 req_code,u8 *pTagType)
{
	char   status;  
	u8   unLen;
	u8   ucComMF522Buf[MAXRLEN]; 
	ClearBitMask(Status2Reg,0x08);//�������ж�λ
	WriteRawRC(BitFramingReg,0x07);
	SetBitMask(TxControlReg,0x03);//��FIFO����
	ucComMF522Buf[0] = req_code;
/*Command[IN]:RC522������
//          pIn [IN]:ͨ��RC522���͵���Ƭ������
//          InLenByte[IN]:�������ݵ��ֽڳ���
//          pOut [OUT]:���յ��Ŀ�Ƭ��������
//          *pOutLenBit[OUT]:�������ݵ�λ����*/
	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);
	if ((status == MI_OK) && (unLen == 0x10))
	{    
		*pTagType     = ucComMF522Buf[0];
		*(pTagType+1) = ucComMF522Buf[1];
	}
	else
	{   status = MI_ERR;   }
   
	return status;
}

/////////////////////////////////////////////////////////////////////
//��    �ܣ�����ײ
//����˵��: pSnr[OUT]:��Ƭ���кţ�4�ֽ�
//��    ��: �ɹ�����MI_OK
/////////////////////////////////////////////////////////////////////  
char PcdAnticoll(u8 *pSnr)
{
    char   status;
    u8   i,snr_check=0;
    u8   unLen;
    u8   ucComMF522Buf[MAXRLEN]; 
    

    ClearBitMask(Status2Reg,0x08);//�������ж�λ
    WriteRawRC(BitFramingReg,0x00);
    ClearBitMask(CollReg,0x80);
 
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
    {
    	 for (i=0; i<4; i++)
			 {   
					 *(pSnr+i)  = ucComMF522Buf[i];
					 snr_check ^= ucComMF522Buf[i];//ucComMF522Buf[i] 111 0101  ^ ���  ��ͬΪ1����ͬΪ0
			 }
			 if (snr_check != ucComMF522Buf[i])
			 {   status = MI_ERR;    }
    }
    
    SetBitMask(CollReg,0x80);
    return status;
}

/////////////////////////////////////////////////////////////////////
//��    �ܣ�ѡ����Ƭ
//����˵��: pSnr[IN]:��Ƭ���кţ�4�ֽ�
//��    ��: �ɹ�����MI_OK
/////////////////////////////////////////////////////////////////////
char PcdSelect(u8 *pSnr)
{
    char   status;
    u8   i;
    u8   unLen;
    u8   ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);//2,3,4,5
    	ucComMF522Buf[6]  ^= *(pSnr+i);//ucComMF522Buf[6]ΪpSnr[3]��ֵ
    }
		//��MF522����CRC16����  void CalulateCRC(u8 *pIn ,u8   len,u8 *pOut )
		//pOut [0] = ReadRawRC(CRCResultRegL); pOut [1] = ReadRawRC(CRCResultRegM);
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
   
    ClearBitMask(Status2Reg,0x08);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
//    //��ӡ���ؿ�����������
//		for(i=0;i<3;i++)
//		{
//			printf("%d\r\n",ucComMF522Buf[i]);
//		}
		
    if ((status == MI_OK) && (unLen == 0x18))// 0x18  24  3���ֽ�
    {   status = MI_OK;  }
    else
    {   status = MI_ERR;    }

    return status;
}

/////////////////////////////////////////////////////////////////////
//��    �ܣ���֤��Ƭ����
//����˵��: auth_mode[IN]: ������֤ģʽ
//                 0x60 = ��֤A��Կ
//                 0x61 = ��֤B��Կ 
//          addr[IN]�����ַ
//          pKey[IN]������
//          pSnr[IN]����Ƭ���кţ�4�ֽ�
//��    ��: �ɹ�����MI_OK  
//PcdAuthState(0x60,0x09,KEY,SN);
/////////////////////////////////////////////////////////////////////               
char PcdAuthState(u8   auth_mode,u8   addr,u8 *pKey,u8 *pSnr)								
{
    char   status;
    u8   unLen;
    u8   i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
//    for (i=0; i<6; i++)
//    {    ucComMF522Buf[i+2] = *(pKey+i);   }
//    for (i=0; i<6; i++)
//    {    ucComMF522Buf[i+8] = *(pSnr+i);   }
	//void	*_Cdecl memcpy	(void *dest, const void *src, size_t n);
	//��Դ�洢�飨src����ǰn���ֽڸ��Ƶ�Ŀ�Ĵ洢�飨dest���С�
    memcpy(&ucComMF522Buf[2], pKey, 6); 
    memcpy(&ucComMF522Buf[8], pSnr, 4); 
// 60 07 ff ff ff ff ff ff id id id id   
    status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
    if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
    {   status = MI_ERR;   }
    
    return status;
}

/////////////////////////////////////////////////////////////////////
//��    �ܣ���ȡM1��һ������
//����˵��: addr[IN]�����ַ
//          p [OUT]�����������ݣ�16�ֽ�
//��    ��: �ɹ�����MI_OK
///////////////////////////////////////////////////////////////////// 
char PcdRead(u8   addr,u8 *p )
{
    char   status;
    u8   unLen;
    u8   i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);	

    if ((status == MI_OK) && (unLen == 0x90))
 //   {   memcpy(p , ucComMF522Buf, 16);   }
    {
        for (i=0; i<16; i++)
        {    
					*(p +i) = ucComMF522Buf[i];
				}
    }
    else
    {   status = MI_ERR;   }
    
    return status;
}

/////////////////////////////////////////////////////////////////////
//��    �ܣ�д���ݵ�M1��һ��
//����˵��: addr[IN]�����ַ
//          p [IN]��д������ݣ�16�ֽ�
//��    ��: �ɹ�����MI_OK
/////////////////////////////////////////////////////////////////////                  
char PcdWrite(u8   addr,u8 *p )
{
    char   status;
    u8   unLen;
    u8   i,ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
        
    if (status == MI_OK)
    {
        //memcpy(ucComMF522Buf, p , 16);
        for (i=0; i<16; i++)
        {    
        	ucComMF522Buf[i] = *(p +i);   
        }
        CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);

        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   }
    }
		printf("status=%d\r\n",status);
    
    return status;
}

/////////////////////////////////////////////////////////////////////
//��    �ܣ����Ƭ��������״̬
//��    ��: �ɹ�����MI_OK
/////////////////////////////////////////////////////////////////////
char PcdHalt(void)
{
    u8   status;
    u8   unLen;
    u8   ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    return MI_OK;
}

/////////////////////////////////////////////////////////////////////
//��MF522����CRC16����
/////////////////////////////////////////////////////////////////////
void CalulateCRC(u8 *pIn ,u8   len,u8 *pOut )
{
    u8   i,n;
    ClearBitMask(DivIrqReg,0x04);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    for (i=0; i<len; i++)
    {   WriteRawRC(FIFODataReg, *(pIn +i));   }
    WriteRawRC(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = ReadRawRC(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOut [0] = ReadRawRC(CRCResultRegL);
    pOut [1] = ReadRawRC(CRCResultRegM);
}

/////////////////////////////////////////////////////////////////////
//��    �ܣ���λRC522
//��    ��: �ɹ�����MI_OK
/////////////////////////////////////////////////////////////////////
char PcdReset(void)
{
	//PORTD|=(1<<RC522RST);
	SET_RC522RST;
    delay_ns(10);
	//PORTD&=~(1<<RC522RST);
	CLR_RC522RST;
    delay_ns(10);
	//PORTD|=(1<<RC522RST);
	SET_RC522RST;
    delay_ns(10);
    WriteRawRC(CommandReg,PCD_RESETPHASE);
	WriteRawRC(CommandReg,PCD_RESETPHASE);
    delay_ns(10);
    
    WriteRawRC(ModeReg,0x3D);            //��Mifare��ͨѶ��CRC��ʼֵ0x6363
    WriteRawRC(TReloadRegL,30);           
    WriteRawRC(TReloadRegH,0);
    WriteRawRC(TModeReg,0x8D);
    WriteRawRC(TPrescalerReg,0x3E);
	
	WriteRawRC(TxAutoReg,0x40);//����Ҫ
   
    return MI_OK;
}
//////////////////////////////////////////////////////////////////////
//����RC632�Ĺ�����ʽ 
//////////////////////////////////////////////////////////////////////
char M500PcdConfigISOType(u8   type)
{
   if (type == 'A')                     //ISO14443_A
   { 
       ClearBitMask(Status2Reg,0x08);
       WriteRawRC(ModeReg,0x3D);//3F
       WriteRawRC(RxSelReg,0x86);//84
       WriteRawRC(RFCfgReg,0x7F);   //4F
   	   WriteRawRC(TReloadRegL,30);//tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
	   WriteRawRC(TReloadRegH,0);
       WriteRawRC(TModeReg,0x8D);
	   WriteRawRC(TPrescalerReg,0x3E);
	   delay_ns(1000);
       PcdAntennaOn();
   }
   else{ return 1; }
   
   return MI_OK;
}
/////////////////////////////////////////////////////////////////////
//��    �ܣ���RC632�Ĵ���
//����˵����Address[IN]:�Ĵ�����ַ
//��    �أ�������ֵ
/////////////////////////////////////////////////////////////////////
u8 ReadRawRC(u8   Address)
{
    u8   ucAddr;
    u8   ucResult=0;
	CLR_SPI_CS;
    ucAddr = ((Address<<1)&0x7E)|0x80;
	
	SPI1_ReadWriteByte(ucAddr);
	ucResult=SPIReadByte();
	SET_SPI_CS;
   return ucResult;
}

/////////////////////////////////////////////////////////////////////
//��    �ܣ�дRC632�Ĵ���
//����˵����Address[IN]:�Ĵ�����ַ
//          value[IN]:д���ֵ
/////////////////////////////////////////////////////////////////////
void WriteRawRC(u8   Address, u8   value)
{  
    u8   ucAddr;
//	u8 tmp;

	CLR_SPI_CS;
    ucAddr = ((Address<<1)&0x7E);

	SPI1_ReadWriteByte(ucAddr);
	SPI1_Read_WriteByte(value);
	SET_SPI_CS;


//	tmp=ReadRawRC(Address);
//
//	if(value!=tmp)
//		printf("wrong\n");
}
/////////////////////////////////////////////////////////////////////
//��    �ܣ���RC522�Ĵ���λ
//����˵����reg[IN]:�Ĵ�����ַ
//          mask[IN]:��λֵ
/////////////////////////////////////////////////////////////////////
void SetBitMask(u8   reg,u8   mask)  
{
    char   tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg,tmp | mask);  // set bit mask
}

/////////////////////////////////////////////////////////////////////
//��    �ܣ���RC522�Ĵ���λ
//����˵����reg[IN]:�Ĵ�����ַ
//          mask[IN]:��λֵ
/////////////////////////////////////////////////////////////////////
void ClearBitMask(u8   reg,u8   mask)  
{
    char   tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg, tmp & ~mask);  // clear bit mask
} 

/////////////////////////////////////////////////////////////////////
//��    �ܣ�ͨ��RC522��ISO14443��ͨѶ
//����˵����Command[IN]:RC522������
//          pIn [IN]:ͨ��RC522���͵���Ƭ������
//          InLenByte[IN]:�������ݵ��ֽڳ���
//          pOut [OUT]:���յ��Ŀ�Ƭ��������
//          *pOutLenBit[OUT]:�������ݵ�λ����
/////////////////////////////////////////////////////////////////////
char PcdComMF522(u8   Command, 
                 u8 *pIn , 
                 u8   InLenByte,
                 u8 *pOut , 
                 u8 *pOutLenBit)
{
    char   status = MI_ERR;
    u8   irqEn   = 0x00;
    u8   waitFor = 0x00;
    u8   lastBits;
    u8   n;
    u16   i;
    switch (Command)
    {
        case PCD_AUTHENT:
			irqEn   = 0x12;
			waitFor = 0x10;
			break;
		case PCD_TRANSCEIVE:
			irqEn   = 0x77;
			waitFor = 0x30;
			break;
		default:
			break;
    }
   
    WriteRawRC(ComIEnReg,irqEn|0x80);
    ClearBitMask(ComIrqReg,0x80);	//�������ж�λ
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);	 	//��FIFO����
    
    for (i=0; i<InLenByte; i++)
    {   WriteRawRC(FIFODataReg, pIn [i]);    }
    WriteRawRC(CommandReg, Command);	  
//   	 n = ReadRawRC(CommandReg);
    
    if (Command == PCD_TRANSCEIVE)
    {    SetBitMask(BitFramingReg,0x80);  }	 //��ʼ����
    										 
    //i = 600;//����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms
	i = 2000;
    do 
    {
        n = ReadRawRC(ComIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitFor));
    ClearBitMask(BitFramingReg,0x80);

    if (i!=0)
    {    
        if(!(ReadRawRC(ErrorReg)&0x1B))
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            {   status = MI_NOTAGERR;   }
            if (Command == PCD_TRANSCEIVE)
            {
               	n = ReadRawRC(FIFOLevelReg);
              	lastBits = ReadRawRC(ControlReg) & 0x07;
                if (lastBits)
                {   *pOutLenBit = (n-1)*8 + lastBits;   }
                else
                {   *pOutLenBit = n*8;   }
                if (n == 0)
                {   n = 1;    }
                if (n > MAXRLEN)
                {   n = MAXRLEN;   }
                for (i=0; i<n; i++)
                {   pOut [i] = ReadRawRC(FIFODataReg);    }
            }
        }
        else
        {   status = MI_ERR;   }
        
    }
   

    SetBitMask(ControlReg,0x80);           // stop timer now
    WriteRawRC(CommandReg,PCD_IDLE); 
    return status;
}

/////////////////////////////////////////////////////////////////////
//��������  
//ÿ��������ر����շ���֮��Ӧ������1ms�ļ��
/////////////////////////////////////////////////////////////////////
void PcdAntennaOn(void)
{
    u8   i;
    i = ReadRawRC(TxControlReg);
    if (!(i & 0x03))
    {
        SetBitMask(TxControlReg, 0x03);
    }
}


/////////////////////////////////////////////////////////////////////
//�ر�����
/////////////////////////////////////////////////////////////////////
void PcdAntennaOff(void)
{
	ClearBitMask(TxControlReg, 0x03);
}

/********************************************************************
//��    �ܣ��Ž�����ز���
//����˵��: 
0����ѯ�Ž����Ƿ�Ϲ�  
1����ӷ��Ϲ涨���Ž��� 
2��ɾ���Ϲ���Ž���  
   
����ֵ��
0��δ��ѯ����  
1��ͨѶʧ��
2: �Ϲ�
3�����Ϲ�
4: ���/ɾ����Ƭ�ɹ�
*********************************************************************/
u8 AddAccessM1(u8 mode)
{
	unsigned char status;
	u32 PassWord=0;
		
	status = PcdRequest(PICC_REQALL,CT);/*����*/	
	if(status==MI_OK)//�����ɹ�
	{
		printf("Ѱ���ɹ�\r\n");
		if (PcdAnticoll(SN) != MI_OK)    return 1;//���nײʧ��
		printf("����ײ�ɹ�\r\n");
		if (PcdSelect(SN) != MI_OK)    return 1;//ѡ��ʧ��	
		printf("ѡ���ɹ�\r\n");
		if (PcdAuthState(0x60,7,KEY,SN) != MI_OK)    return 1;//��Cʧ��
		printf("��֤�ɹ�\r\n");		
		
		if(mode == 0)
		{
			PassWord = SN[0]<<9 | SN[1]<<10 | SN[2]<<11 | SN[3]<<12;//���㷨����		
			status = PcdRead(5,RFID);
				
			if((RFID[0]<<24|RFID[1]<<16|RFID[2]<<8|RFID[3]<<0 == PassWord))
			{
//				BEEP=1;
//				delay_ms(20);
//				BEEP=0;	
				printf("���ţ���������\r\n");
				return 2;//�Ϲ�
			}			
			else 
				return 3;//���Ϲ�						
		}
		
		else if(mode == 1)//��ӷ��Ϲ涨���Ž���
		{		
			PassWord = SN[0]<<9 | SN[1]<<10 | SN[2]<<11 | SN[3]<<12;//���㷨����
			memset(RFID, 0, sizeof(RFID));
			RFID[0] = PassWord>>24;RFID[1] = PassWord>>16;RFID[2] = PassWord>>8;RFID[3] = PassWord;
//			sprintf(RFID,"%d%d%d%d",(u8)(PassWord>>24), (u8)(PassWord>>16), (u8)(PassWord>>8), (u8)(PassWord));		
			if(PcdWrite(5,RFID) == MI_OK)
			{
//				BEEP=1;
//				delay_ms(20);
//				BEEP=0;
				
				printf("��ӷ��Ϲ涨���Ž���\r\n");				
				//printf("д�룡��������\r\n");
				return 4;
			}			
			else
				return 1;		
		}

		else if(mode == 2)//ɾ���Ϲ涨���Ž���
		{		
			memset(RFID, 0, sizeof(RFID));
			if(PcdWrite(5,RFID) == MI_OK)
			{
//				BEEP=1;
//				delay_ms(20);
//				BEEP=0;	
				printf("ɾ���Ϲ涨���Ž���\r\n");				
				return 4;
			}			
			else
				return 1;		
		}		
	}	
	return 0;
}

 u8 rc522_error_i=0;
void RC522_Entrance_Guard(void)
{
	u8 i=0;
	unsigned char status;
	//char card_1[4]={42,216,147,229};	
	char card_2[4]={0x30,0x60,0xef,0x79};
	u8 SNT[4];
		
	//RTC_Get();//ģ��ʱ��
	
AA:
		status=PcdRequest(PICC_REQALL,CT);/*����*/
		printf("Ѱ��\r\n");
		printf("CT[0]=%d\r\n",CT[0]);
		printf("CT[1]=%d\r\n",CT[1]);
		printf("status=%d\r\n",status);
		while( status!=MI_OK)//�ȴ�Ѱ��
		{
			goto AA;
		}
		printf("status=%d\r\n",status);
	while(status==MI_OK)//�����ɹ�
	{
		if (PcdAnticoll(SN) == MI_OK)//���nײ�ɹ�
		{		
			printf("����ײ\r\n");	
			print_info(SN,4);
			if (PcdSelect(SN) == MI_OK)//ѡ���ɹ�
			{	
				printf("ѡ��\r\n");	
				print_info(SN,4);
				if (PcdAuthState(0x60,1,KEY,SN)== MI_OK)//��C�ɹ�
					{	
						printf("��֤�ɹ�\r\n");	
						printf("KEY\r\n");	
						print_info(KEY,6);
						printf("SN1\r\n");
						print_info(SN,4);
						PcdWrite(1,SNT);
						printf("SN2\r\n");
						print_info(SNT,4);
						if(PcdRead(1,SNT)==MI_OK)//����
							{		
								printf("SN3\r\n");
								print_info(SN,4);
								printf("card_2\r\n");
								print_info((char *)card_2,4);
								if(strncmp((char *)SN,(char *)card_2,4)==0)//�жϿ����Ƿ�ƥ��
								{
										RC522_Rtc_flag=0;
										rc522_error_i=0;
										BEEP=1;
										delay_ms(20);
										BEEP=0;
										printf("����ƥ��\r\n");
									  printf("��ӭ�ؼң�\r\n");
										delay_ms(100);
										motorCW();	//�����ת90��
										delay_ms(2000);
										BEEP=1;
										delay_ms(50);
										BEEP=0;
										delay_ms(1000);
										motorCCW();	//�����ת90��
										
										return;									
								}							
								else 
								{	
										rc522_error_i++;					
										BEEP=1;
										delay_ms(20);
										BEEP=0;
										printf("���Ų�ƥ��������ˢ������\r\n");
										
									
									if(rc522_error_i>=2)
									{
				
										rc522_error_i=0;
										printf("���ˢ�������볢�������¼����\r\n");
										delay_ms(2000);
										return;
									}
									return;
								}
							}
							else
							{
								printf("����ʧ�ܣ�������ˢ����");
									return;
							}
							
					}
				}
				else
				{
					printf("ѡ��ʧ�ܣ�������ˢ����");
					return;
				
				}
			
		}
		else
		{
			printf("����ײʧ�ܣ�������ˢ����");
			return;
		
		}
	}
}





u8 CT[2];  //������
u8 SN[4]; //����
/*������������Ϊ8,182,221*/











