#include "sys.h"
#include "usart.h"	
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"      //FreeRTOS ʹ��
#endif

#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x) 
{ 
	x = x; 
} 
extern int consolePutchar(int ch);
//�ض���fputc����
int fputc(int ch, FILE *f)
{ 	
//	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������
//	USART1->DR = (u8) ch;   
	consolePutchar(ch);	/*��ӡ����λ��*/
	return ch;
}
#endif
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���

//��ʼ��IO ����1
//bound:������

#endif	

 



