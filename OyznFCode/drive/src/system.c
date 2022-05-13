#include "system.h"

/*�ײ�Ӳ����ʼ��*/
void systemInit(void)
{
	IICInit();
	ledInit();			/*led��ʼ��*/
	ledStripInit();		/*��ʼ��WS2812RGB��*/
	beeperInit();		/*��������ʼ��*/
	configParamInit();	/*��ʼ�����ò���*/
	pmInit();			/*��Դ��ѹ�����ʼ��*/
	rxInit();			/*ң����ppm�źŽ��ճ�ʼ��*/
	usblinkInit();		/*USBͨ�ų�ʼ��*/
	atkpInit();			/*����Э���ʼ��*/
	consoleInit();		/*��ӡ��ʼ��*/
	stabilizerInit();	/*��� PID ��̬�����ʼ��*/
}
