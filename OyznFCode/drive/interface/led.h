#ifndef __LED_H
#define __LED_H
#include "sys.h"

#define LED0_ON 		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET)
#define LED0_OFF 		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET)
#define LED0_TOGGLE 	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9)	//�л�ָ����GPIO����ʹ�õ����ŵ�ƽ


void ledInit(void);
void warningLedON(void);
void warningLedOFF(void);
void warningLedFlash(void);
void warningLedRefresh(void);
void warningLedUpdate(void);

#endif
