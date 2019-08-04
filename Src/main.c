/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "dma.h"
#include "ltdc.h"
#include "quadspi.h"
#include "sdmmc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include "crc.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbh_core.h"
#include "usbh_msc.h"
#include "gt9147.h"
#include "limits.h"
#include "GUI.h"
#include "WM.h"
#include "led.h"
#include "rtc.h"
/* USER CODE END Includes */
#define Write_Through() (*(__IO uint32_t*)0XE000EF9C=1UL<<2) //Cache透写模式

USBH_HandleTypeDef  hUSBHost;

	FATFS fs1;
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);

static void USBH_UserProcess(USBH_HandleTypeDef * phost, uint8_t id)
{
    uint32_t total,free;
	uint8_t res=0;
    switch (id)
    {
        case HOST_USER_SELECT_CONFIGURATION:
            break;
        case HOST_USER_DISCONNECTION:
            f_mount(0,"3:",1); 	//卸载U盘
//            POINT_COLOR=RED;    //设置字体为红色	   
            printf("设备连接中...\r\n");
//            LCD_Fill(30,160,239,220,WHITE); 
            break;
        case HOST_USER_CLASS_ACTIVE:
            printf("设备连接成功!\r\n");	
            res=f_mount(&fs1,"3:",1); 	//重新挂载U盘
            res=exf_getfree("3:",&total,&free);
        	if(res==0)
            {
				printf("U盘挂载成功,total=%d,free=%d\r\n",total,free);
            }
            else
            {
                printf("U盘存储空间获取失败\r\n");
            }
            break;
        case HOST_USER_CONNECTION:
            break;
        default:
            break;
    }
}

void USB_HOST_Init(void)
{
	USBH_Init(&hUSBHost, USBH_UserProcess, USBH_SPEED_FULL);
    USBH_RegisterClass(&hUSBHost, USBH_MSC_CLASS);
    USBH_Start(&hUSBHost);
}


int main(void)
{
  /* USER CODE BEGIN 1 */
	uint8_t res=0;	  
	uint8_t key;
  /* USER CODE END 1 */
  

	/* Enable I-Cache---------------------------------------------------------*/
	Write_Through();				//强制cache透传
	MPU_Memory_Protection();		//MPU内存保护配置
	SCB_EnableICache();

	/* Enable D-Cache---------------------------------------------------------*/
	SCB_EnableDCache();
	HAL_Init();

	SystemClock_Config();
	RTC_Init();
	RTC_Set_WakeUp(RTC_WAKEUPCLOCK_CK_SPRE_16BITS,0); //配置WAKE UP中断,1秒钟中断一次  
	delay_init(216);				//延时函数初始化，时钟频率216M
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART1_UART_Init();
	MX_SDMMC1_SD_Init();
	/* USER CODE BEGIN 2 */
	SDRAM_Init();					//初始化SDRAM
	LTDC_LCD_Init();
	W25QXX_Init();					//SPI FLASH初始化
	PCF8574_Init();				    //初始化PCF8574 
	read_sdinfo();					//获取SD卡信息
	USB_HOST_Init();				//USB
					
	GT9147_Init();
	my_mem_init(SRAMIN);		    //初始化内部内存池
	my_mem_init(SRAMEX);		    //初始化外部内存池
	my_mem_init(SRAMDTCM);		    //初始化DTCM内存池
	HAL_UART_Receive_IT(&huart1,aRecBuff,1);
	exfuns_init();
	Led_Init();
	res=f_mount(fs[0],"0:",1);//挂载SD卡

	while(font_init()) 		       			//检查字库
	{
		if(res)
		{
			LTDC_ShowString(30,70,16,"SD Mount Failed!");
			HAL_Delay(200);
			LTDC_LCD_Fill(30,70,200+30,70+16,WHITE);
			HAL_Delay(200);	
		}
		else
		{
			key=update_font(20,110,16,"0:");//更新字库
			if(key)
			{
				LTDC_ShowString(30,110,16,"Font Update Failed!");
				HAL_Delay(200);
				LTDC_LCD_Fill(20,110,200+20,110+16,WHITE);
				HAL_Delay(200);		
			}
			else
			{
				break;
			}
		}
	}
  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
//  while (1)
//  {

//  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_SDMMC1|RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 432;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 6;
  PeriphClkInitStruct.PLLSAI.PLLSAIQ = 2;
  PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV2;
  PeriphClkInitStruct.PLLSAIDivQ = 1;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_8;
  PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  PeriphClkInitStruct.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_CLK48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
