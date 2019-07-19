/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/

#include "main.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "gt9147.h"
#include "usbh_core.h"
#include "usbh_msc.h"
#include "limits.h"
#include "GUI.h"
#include "WM.h"
#include "led_beep.h"
#include "DIALOG.h"
#include "icondemo.h"
#include "Hzfontupdata.h"

#include "pngdisplay.h"
#include "jpegdisplay.h"
#include "gifdisplay.h"
#include "bmpdisplay.h"
#include "picture_app.h"
#include "FreeRTOS.h"
#include "task.h"
//任务优先级
#define START_TASK_PRIO			1
//任务堆栈大小	
#define START_STK_SIZE 			128  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//TOUCH任务
//设置任务优先级
#define TOUCH_TASK_PRIO			2
//任务堆栈大小
#define TOUCH_STK_SIZE			512
//任务句柄
TaskHandle_t TouchTask_Handler;
//touch任务
void touch_task(void *pvParameters);

//LED0任务
//设置任务优先级
#define LED0_TASK_PRIO 			3
//任务堆栈大小
#define LED0_STK_SIZE			512
//任务句柄
TaskHandle_t Led0Task_Handler;
//led0任务
void led0_task(void *pvParameters);

//EMWINDEMO任务
//设置任务优先级
#define EMWINDEMO_TASK_PRIO		4
//任务堆栈大小
#define EMWINDEMO_STK_SIZE		2048
//任务句柄
TaskHandle_t EmwindemoTask_Handler;
//emwindemo_task任务
void emwindemo_task(void *pvParameters);

extern USBH_HandleTypeDef  hUSBHost;

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */


/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  
//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄                
    vTaskStartScheduler();  
}


//开始任务任务函数
void start_task(void *pvParameters)
{
	__HAL_RCC_CRC_CLK_ENABLE();		//使能CRC时钟
	WM_SetCreateFlags(WM_CF_MEMDEV);
	GUI_Init();  					//STemWin初始化
	WM_MULTIBUF_Enable(1);  		//开启STemWin多缓冲,RGB屏可能会用到
    taskENTER_CRITICAL();           //进入临界区
	//创建触摸任务
    xTaskCreate((TaskFunction_t )touch_task,             
                (const char*    )"touch_task",           
                (uint16_t       )TOUCH_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TOUCH_TASK_PRIO,        
                (TaskHandle_t*  )&TouchTask_Handler);   	
    //创建LED0任务
    xTaskCreate((TaskFunction_t )led0_task,             
                (const char*    )"led0_task",           
                (uint16_t       )LED0_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )LED0_TASK_PRIO,        
                (TaskHandle_t*  )&Led0Task_Handler);  
    //创建EMWIN Demo任务
    xTaskCreate((TaskFunction_t )emwindemo_task,             
                (const char*    )"emwindemo_task",           
                (uint16_t       )EMWINDEMO_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )EMWINDEMO_TASK_PRIO,        
                (TaskHandle_t*  )&EmwindemoTask_Handler);   				
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}


//EMWINDEMO任务
void emwindemo_task(void *pvParameters)
{
//	//更换皮肤
//	GUI_CURSOR_Show();
	iconviewdemo();
//	jpegdisplay_demo();
//	gifdisplay_demo();
//	pngdisplay_demo();
	while(1)
	{	
		GUI_Delay(100);
	}
}

//触摸任务的任务函数
void touch_task(void *pvParameters)
{
	while(1)
	{
		
		GUI_TOUCH_Exec();
//		USBH_Process(&hUSBHost);	
		vTaskDelay(5);		//延时5ms
	}
}

//LED0任务
void led0_task(void *p_arg)
{
	uint8_t CPU_RunInfo[400];
	GUI_ALLOC_DATATYPE k;
	GUI_ALLOC_INFO info;
	while(1)
	{
		GUI_ALLOC_GetMemInfo(&info);
		printf("TotalBytes=%ld,UsedBytes=%ld\r\n",info.TotalBytes,info.UsedBytes);
//		k=GUI_ALLOC_GetNumUsedBytes();
//		printf("UsedBytes=%ld\r\n",k);
//		memset(CPU_RunInfo,0,400); //信息缓冲区清零

//		vTaskList((char *)&CPU_RunInfo); //获取任务运行时间信息

//		printf("---------------------------------------------------------\r\n");
//		printf("任务名      任务状态      优先级      剩余栈     任务序号\r\n");
//		printf("%s", CPU_RunInfo);
//		printf("---------------------------------------------------------\r\n");

//		memset(CPU_RunInfo,0,400); //信息缓冲区清零

//		vTaskGetRunTimeStats((char *)&CPU_RunInfo);

//		printf("任务名            运行计数            使用率\r\n");
//		printf("%s", CPU_RunInfo);
//		printf("-------------------------------------------------------\r\n\n");

		HAL_GPIO_TogglePin(LED0_GPIO_Port,LED0_Pin);
		vTaskDelay(1000);		//延时1000ms
	}
}
