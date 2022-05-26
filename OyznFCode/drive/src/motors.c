#include "sys.h"
#include "delay.h"
#include "motors.h"
#include "pm.h"
#include "main.h"

extern TIM_HandleTypeDef htim3;

static bool isInit = false;
u32 motor_ratios[] = {0, 0, 0, 0};
const u32 MOTORS[] = { MOTOR_M1, MOTOR_M2, MOTOR_M3, MOTOR_M4 };

/*val:0-1000*/
static u16 ratioToCCRx(u16 val)
{
	return (MOTOR_TIM_CNT_FOR_HIGH + val * MOTOR_TIM_CNT_FOR_HIGH/ 1000);//MOTOR_TIM_CNT_FOR_HIGHΪ�̶��ߵ�ƽʱ��ֵ
}

void motorsInit(void)	/*�����ʼ��*/
{
	htim3.Init.Period = MOTOR_TIM_PERIOD;			//�Զ���װ��ֵ
	htim3.Init.Prescaler = MOTOR_TIM_PRESCALE;		//��ʱ����Ƶ
	if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
	{
	  Error_Handler();
	}				//��ʼ��TIM3
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
	isInit = true;
}

/*���õ��PWMռ�ձ�*/
/*ithrust:0-1000*/
void motorsSetRatio(u32 id, u16 ithrust)
{
	if (isInit)
	{
		switch(id)
		{
			case 0:		/*MOTOR_M1*/
				 __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, ratioToCCRx(ithrust));
				break;
			case 1:		/*MOTOR_M2*/
				 __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, ratioToCCRx(ithrust));
				break;
			case 2:		/*MOTOR_M3*/
				 __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, ratioToCCRx(ithrust));
				break;
			case 3:		/*MOTOR_M4*/
				 __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, ratioToCCRx(ithrust));
				break;
			default: break;
		}
	}
}
