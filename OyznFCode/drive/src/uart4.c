
#include <uart4.h>
#include "stdio.h"
//DMA_InitTypeDef DMA_InitStructure;
uint16_t DMA1_MEM_LEN;//����DMAÿ�����ݴ��͵ĳ���
/*
DMA1�ĸ�ͨ����������Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
DMA_CHx:DMAͨ��CHx      cpar:�����ַ
cmar:�洢����ַ         cndtr:���ݴ�����
*/
//void Quad_DMA_Config(DMA_Channel_TypeDef* DMA_CHx,uint32_t cpar,uint32_t cmar,uint16_t cndtr)
//{
////  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
////  DMA_DeInit(DMA_CHx);   											        //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
////  DMA1_MEM_LEN=cndtr;
////  DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA�������ַ
////  DMA_InitStructure.DMA_MemoryBaseAddr =cmar;				//DMA�ڴ����ַ
////  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//������Ϊ���ݴ����Ŀ�ĵ�
////  DMA_InitStructure.DMA_BufferSize = cndtr;  												//DMAͨ����DMA����Ĵ�С
////  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
////  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  					//�ڴ��ַ�Ĵ�������
////  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//���ݿ��Ϊ8λ
////  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;        //���ݿ��Ϊ8λ
////  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  									       //��������������ģʽ
////  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                  //DMAͨ�� xӵ�������ȼ�
////  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                           //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
////  DMA_Init(DMA_CHx, &DMA_InitStructure);//����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
//}


void DMA_Send_Vcan(float userdata1,float userdata2,
									 float userdata3,float userdata4,
									 float userdata5,float userdata6,
									 float userdata7,float userdata8)
{
	static uint8_t databuf[36];
	uint8_t _cnt=0;
	databuf[_cnt++]=0x03;
	databuf[_cnt++]=0xfc;
	Float2Byte(&userdata1,databuf,_cnt);//3
	_cnt+=4;
	Float2Byte(&userdata2,databuf,_cnt);//7
	_cnt+=4;
	Float2Byte(&userdata3,databuf,_cnt);//11
	_cnt+=4;
	Float2Byte(&userdata4,databuf,_cnt);//15
	_cnt+=4;
	Float2Byte(&userdata5,databuf,_cnt);//19
	_cnt+=4;
	Float2Byte(&userdata6,databuf,_cnt);//23
	_cnt+=4;
	Float2Byte(&userdata7,databuf,_cnt);//27
	_cnt+=4;
	Float2Byte(&userdata8,databuf,_cnt);//31
	_cnt+=4;
	databuf[_cnt++]=0xfc;//35
	databuf[_cnt++]=0x03;//36
	//USART1_Send_Dma(databuf,_cnt);
	Serial_Data_Send(databuf,_cnt);
}


void UART2_Send(unsigned char tx_buf)
{
  while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);//���������fputcʱ��һ����
  USART_SendData(USART2 , tx_buf);//�����ַ�������ĵ����ַ�
  while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
}

void USART2_Send(unsigned char *tx_buf, int len)
{
  USART_ClearFlag(USART2, USART_FLAG_TC);
  USART_ClearITPendingBit(USART2, USART_FLAG_TXE);
  while(len--)
  {
    USART_SendData(USART2, *tx_buf);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) != 1);
    USART_ClearFlag(USART2, USART_FLAG_TC);
    USART_ClearITPendingBit(USART2, USART_FLAG_TXE);
    tx_buf++;
  }
}


unsigned char Buffer[2]={9,8};
void USART2_Init(unsigned long bound)
{
//  GPIO_InitTypeDef GPIO_InitStructure;
//  USART_InitTypeDef USART_InitStructure;
//
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO
//                         |RCC_APB2Periph_GPIOA , ENABLE);//����2
//
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);//����2 ����
//
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  //	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//
//  USART_InitStructure.USART_BaudRate = bound;//
//  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8bits
//  USART_InitStructure.USART_StopBits = USART_StopBits_1;//stop bit is 1
//  USART_InitStructure.USART_Parity = USART_Parity_No;//no parity
//  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//no Hardware Flow Control
//  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//enable tx and rx
//  USART_Init(USART2, &USART_InitStructure);//
//
//  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//rx interrupt is enable
//  USART_Cmd(USART2, ENABLE);
  
  //USART2_Send((unsigned char *)Buffer,2);
  //UART2_Send(0xAA);
}

//u16 GPS_ISR_CNT=0;
//uint16 Ublox_Try_Cnt=0;
//uint16 Ublox_Try_Makesure=0;
//uint16 Ublox_Try_Start=0;
//uint8 Ublox_Data[95]={0};
//uint16 Ublox_Cnt=0;
//uint16 GPS_Update_finished=0;
//uint16 GPS_Update_finished_Correct_Flag=0;
//Testime GPS_Time_Delta;
//void USART2_IRQHandler(void)//����GPS�����UBLOX  PVTЭ��
//{
//  unsigned char ch;
//  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//  {
//    //Test_Period(&GPS_Time_Delta1);
//    if(GPS_ISR_CNT<=2000)
//    {
//      GPS_ISR_CNT++;
//    }
//
//    ch=USART_ReceiveData(USART2);
//
//    if(Ublox_Try_Makesure==1)
//    {
//      Ublox_Data[Ublox_Cnt++]=ch;
//      if(Ublox_Cnt==94)
//      {
//        Ublox_Cnt=0;
//        Ublox_Try_Makesure=0;
//        GPS_Update_finished=1;
//        GPS_Update_finished_Correct_Flag=1;
//        Test_Period(&GPS_Time_Delta);//GPS���ݸ��¼������
//      }
//    }
//
//    if(Ublox_Try_Makesure==0
//       &&ch==0xB5)//����֡ͷ���ֽڣ��ж�֡ͷ�Ƿ�����
//    {
//      Ublox_Try_Start=1;
//      Ublox_Try_Cnt=0;
//    }
//
//    if(Ublox_Try_Start==1)
//    {
//      Ublox_Try_Cnt++;
//      if(Ublox_Try_Cnt>=5)
//      {
//        Ublox_Try_Start=0;
//        Ublox_Try_Cnt=0;
//
//        if(ch==0x5C) Ublox_Try_Makesure=1;//ȷ��Ϊ֡ͷ����ʼ����
//        else Ublox_Try_Makesure=0;//��֡ͷ����λ�ȴ��ٴ�ȷ��
//      }
//    }
//  }
//  USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//
//}


