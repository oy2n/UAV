#include <math.h>
#include "stdio.h"
#include "delay.h"
#include "config.h"
#include "config_param.h"
#include "sensors.h"
#include "pos_estimator.h"
#include "stabilizer.h"
#include "gyro.h"
#include "accelerometer.h"
#include "compass.h"
#include "barometer.h"
#include "sensorsalignment.h"
#include "compass_qmc5883l.h"

/*FreeRTOS���ͷ�ļ�*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

sensorData_t sensors;

static bool isInit = false;

static bool isMPUPresent=false;
static bool isMagPresent=false;
static bool isBaroPresent=false;

static xQueueHandle accelerometerDataQueue;
static xQueueHandle gyroDataQueue;
static xQueueHandle magnetometerDataQueue;
static xQueueHandle barometerDataQueue;

extern I2C_HandleTypeDef hi2c1;

/*�Ӷ��ж�ȡ��������*/
bool sensorsReadGyro(Axis3f *gyro)
{
	return (pdTRUE == xQueueReceive(gyroDataQueue, gyro, 0));
}
/*�Ӷ��ж�ȡ���ټ�����*/
bool sensorsReadAcc(Axis3f *acc)
{
	return (pdTRUE == xQueueReceive(accelerometerDataQueue, acc, 0));
}
/*�Ӷ��ж�ȡ����������*/
bool sensorsReadMag(Axis3f *mag)
{
	return (pdTRUE == xQueueReceive(magnetometerDataQueue, mag, 0));
}
/*�Ӷ��ж�ȡ��ѹ����*/
bool sensorsReadBaro(baro_t *baro)
{
	return (pdTRUE == xQueueReceive(barometerDataQueue, baro, 0));
}

/* ������������ʼ�� */
void sensorsInit(void)
{
	if (isInit) return;
	
	initBoardAlignment();

	isMPUPresent = gyroInit(GYRO_UPDATE_RATE);
	isMPUPresent &= accInit(ACC_UPDATE_RATE);
	isBaroPresent = baroInit();
	//isMagPresent  = qmc5883lInit();
	
	/*�������������ݶ���*/
	accelerometerDataQueue = xQueueCreate(1, sizeof(Axis3f));
	gyroDataQueue = xQueueCreate(1, sizeof(Axis3f));
	magnetometerDataQueue = xQueueCreate(1, sizeof(Axis3f));
	barometerDataQueue = xQueueCreate(1, sizeof(baro_t));
	
	isInit = true;
}

/*����������*/
void sensorsTask(void *param)
{	
	u32 tick = 0;
	u32 lastWakeTime = getSysTickCnt();
	sensorsInit();		//��������ʼ��
	
	while (1)
	{
		vTaskDelayUntil(&lastWakeTime, F2T(RATE_1000_HZ));//1KHz����Ƶ��
		if (isMPUPresent && RATE_DO_EXECUTE(GYRO_UPDATE_RATE, tick))
		{
			gyroUpdate(&sensors.gyro);
		}

		if (isMPUPresent && RATE_DO_EXECUTE(ACC_UPDATE_RATE, tick))
		{
			accUpdate(&sensors.acc);
		}
		
		if (isMagPresent && RATE_DO_EXECUTE(MAG_UPDATE_RATE, tick))
		{
			//compassUpdate(&sensors.mag);
		}
		
		if (isBaroPresent && RATE_DO_EXECUTE(BARO_UPDATE_RATE, tick))
		{
			baroUpdate(&sensors.baro);
		}
		
		vTaskSuspendAll();	/*ȷ��ͬһʱ�̰����ݷ��������*/
		xQueueOverwrite(accelerometerDataQueue, &sensors.acc);
		xQueueOverwrite(gyroDataQueue, &sensors.gyro);
		if (isMagPresent)
		{
			xQueueOverwrite(magnetometerDataQueue, &sensors.mag);
		}
		if (isBaroPresent)
		{
			xQueueOverwrite(barometerDataQueue, &sensors.baro);
		}
		xTaskResumeAll();

		tick++;
	}	
}

/*��ȡ����������*/
void sensorsAcquire(sensorData_t *sensors, const u32 tick)
{
	sensorsReadGyro(&sensors->gyro);
	sensorsReadAcc(&sensors->acc);
	sensorsReadMag(&sensors->mag);
	sensorsReadBaro(&sensors->baro);
}

bool sensorsIsMagPresent(void)
{
	return isMagPresent;
}
