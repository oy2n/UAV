#ifndef __CONFIG_H
#define __CONFIG_H
#include "nvic.h"
#include "stdio.h"	/*printf ����*/

/*ң��������*/
#define RC_MIN			1000
#define RC_MID			1500
#define RC_MAX			2000

#define RC_COMMANDER_MINCHECK	1100 //ң��������С���ֵ
#define RC_COMMANDER_MAXCHECK	1900 //ң�����������ֵ

#define MINTHROTTLE		1100 //������������ֵ
#define MAXTHROTTLE		1850 //����ʱ�������ֵ


/*���Э�����ã�ֻ��ѡһ��*/
#define USE_ESC_PROTOCOL_STANDARD  //��׼PWMЭ��
//#define USE_ESC_PROTOCOL_ONESHOT125	//oneshot125Э��




#endif /* __CONFIG_H */
