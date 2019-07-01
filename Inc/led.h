#ifndef led_H
#define led_H
#include "main.h"
#define LED0_ON		HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin,GPIO_PIN_RESET)
#define LED0_OFF	HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin,GPIO_PIN_SET)
#define LED1_ON		HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET)
#define LED1_OFF	HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET)
#define BEEP_ON		PCF8574_WriteBit(BEEP_IO,0)
#define BEEP_OFF	PCF8574_WriteBit(BEEP_IO,1)
	void Led_Init(void);
#endif

