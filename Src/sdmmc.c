/**
  ******************************************************************************
  * File Name          : SDMMC.c
  * Description        : This file provides code for the configuration
  *                      of the SDMMC instances.
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

/* Includes ------------------------------------------------------------------*/
#include "sdmmc.h"
#include "mysys.h"
/* USER CODE BEGIN 0 */
//SD_ReadDisk/SD_WriteDisk函数专用buf,当这两个函数的数据缓存区地址不是4字节对齐的时候,
//需要用到该数组,确保数据缓存区地址是4字节对齐的.
__align(4) uint8_t SDIO_DATA_BUFFER[512];
//如果使用DMA的话下面两个变量用来标记SD卡读写是否完成
static volatile uint8_t SDCardWriteStatus=0,SDCardReadStatus=0; 
HAL_SD_CardInfoTypeDef SDCardInfo;
#if SDMMC_DMA
DMA_HandleTypeDef hdma_sdmmc1_rx;
DMA_HandleTypeDef hdma_sdmmc1_tx;
#endif
/* USER CODE END 0 */

SD_HandleTypeDef hsd1;

/* SDMMC1 init function */

void MX_SDMMC1_SD_Init(void)
{

  hsd1.Instance = SDMMC1;
  hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
  hsd1.Init.ClockBypass = SDMMC_CLOCK_BYPASS_DISABLE;
  hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd1.Init.BusWide = SDMMC_BUS_WIDE_1B;
  hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd1.Init.ClockDiv = 0;
  if (HAL_SD_Init(&hsd1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_SD_ConfigWideBusOperation(&hsd1, SDMMC_BUS_WIDE_4B) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_SD_MspInit(SD_HandleTypeDef* sdHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(sdHandle->Instance==SDMMC1)
  {
  /* USER CODE BEGIN SDMMC1_MspInit 0 */

  /* USER CODE END SDMMC1_MspInit 0 */
    /* SDMMC1 clock enable */
    __HAL_RCC_SDMMC1_CLK_ENABLE();
   __HAL_RCC_DMA2_CLK_ENABLE();    //使能DMA2时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**SDMMC1 GPIO Configuration    
    PC8     ------> SDMMC1_D0
    PC9     ------> SDMMC1_D1
    PC10     ------> SDMMC1_D2
    PC11     ------> SDMMC1_D3
    PC12     ------> SDMMC1_CK
    PD2     ------> SDMMC1_CMD 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
                          |GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDMMC1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDMMC1;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* USER CODE BEGIN SDMMC1_MspInit 1 */
#if SDMMC_DMA
	 /* SDMMC1 DMA Init */
    /* SDMMC1_RX Init */
    hdma_sdmmc1_rx.Instance = DMA2_Stream3;
    hdma_sdmmc1_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_sdmmc1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_sdmmc1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sdmmc1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sdmmc1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sdmmc1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_sdmmc1_rx.Init.Mode = DMA_PFCTRL;
    hdma_sdmmc1_rx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
    hdma_sdmmc1_rx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_sdmmc1_rx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_sdmmc1_rx.Init.MemBurst = DMA_MBURST_INC4;
    hdma_sdmmc1_rx.Init.PeriphBurst = DMA_PBURST_INC4;
    if (HAL_DMA_Init(&hdma_sdmmc1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(sdHandle,hdmarx,hdma_sdmmc1_rx);

    /* SDMMC1_TX Init */
    hdma_sdmmc1_tx.Instance = DMA2_Stream6;
    hdma_sdmmc1_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_sdmmc1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_sdmmc1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sdmmc1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sdmmc1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sdmmc1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_sdmmc1_tx.Init.Mode = DMA_PFCTRL;
    hdma_sdmmc1_tx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
    hdma_sdmmc1_tx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_sdmmc1_tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_sdmmc1_tx.Init.MemBurst = DMA_MBURST_INC4;
    hdma_sdmmc1_tx.Init.PeriphBurst = DMA_PBURST_INC4;
    if (HAL_DMA_Init(&hdma_sdmmc1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(sdHandle,hdmatx,hdma_sdmmc1_tx);

    /* SDMMC1 interrupt Init */
    HAL_NVIC_SetPriority(SDMMC1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(SDMMC1_IRQn);
	/* DMA2_Stream3_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
	/* DMA2_Stream6_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
#endif
  /* USER CODE END SDMMC1_MspInit 1 */
  }
}

void HAL_SD_MspDeInit(SD_HandleTypeDef* sdHandle)
{

  if(sdHandle->Instance==SDMMC1)
  {
  /* USER CODE BEGIN SDMMC1_MspDeInit 0 */

  /* USER CODE END SDMMC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SDMMC1_CLK_DISABLE();
  
    /**SDMMC1 GPIO Configuration    
    PC8     ------> SDMMC1_D0
    PC9     ------> SDMMC1_D1
    PC10     ------> SDMMC1_D2
    PC11     ------> SDMMC1_D3
    PC12     ------> SDMMC1_CK
    PD2     ------> SDMMC1_CMD 
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
                          |GPIO_PIN_12);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);

  /* USER CODE BEGIN SDMMC1_MspDeInit 1 */
#if SDMMC_DMA
	HAL_DMA_DeInit(sdHandle->hdmarx);
	HAL_DMA_DeInit(sdHandle->hdmatx);

	/* SDMMC1 interrupt Deinit */
	HAL_NVIC_DisableIRQ(SDMMC1_IRQn);
#endif
  /* USER CODE END SDMMC1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
//得到卡信息
//cardinfo:卡信息存储区
//返回值:错误状态
uint8_t SD_GetCardInfo(HAL_SD_CardInfoTypeDef *cardinfo)
{
    uint8_t sta;
	sta=HAL_SD_GetCardInfo(&hsd1,cardinfo);
    return sta;
}
//判断SD卡是否可以传输(读写)数据
//返回值:SD_TRANSFER_OK 传输完成，可以继续下一次传输
//		 SD_TRANSFER_BUSY SD卡正忙，不可以进行下一次传输
uint8_t SD_GetCardState(void)
{
  return((HAL_SD_GetCardState(&hsd1)==HAL_SD_CARD_TRANSFER )?SD_TRANSFER_OK:SD_TRANSFER_BUSY);
}
#if SDMMC_DMA

//读SD卡
//buf:读数据缓存区
//sector:扇区地址
//cnt:扇区个数	
//返回值:错误状态;0,正常;其他,错误代码;
uint8_t SD_ReadDisk(uint8_t* buf,uint32_t sector,uint32_t cnt)
{
    uint8_t sta=HAL_ERROR;
	SDCardReadStatus=0;
	SCB_CleanInvalidateDCache();//cache一致性
	if(HAL_SD_ReadBlocks_DMA(&hsd1,(uint8_t*)buf,(uint32_t)sector,(uint32_t)cnt)==HAL_OK)
	{
		while(SDCardReadStatus==0){};	//等待读完成
		
		SDCardReadStatus=0;
		while(SD_GetCardState()){};		//等待SD卡空闲
		sta=HAL_OK;
	}
	SCB_CleanInvalidateDCache();//cache一致性
    return sta;
}  

//写SD卡
//buf:写数据缓存区
//sector:扇区地址
//cnt:扇区个数	
//返回值:错误状态;0,正常;其他,错误代码;	
uint8_t SD_WriteDisk(uint8_t *buf,uint32_t sector,uint32_t cnt)
{   
    uint8_t sta=HAL_ERROR;
	
	SDCardWriteStatus=0;
	SCB_CleanInvalidateDCache();//cache一致性
	if(HAL_SD_WriteBlocks_DMA(&hsd1,(uint8_t*)buf,(uint32_t)sector,(uint32_t)cnt)==HAL_OK)
	{
		while(SDCardWriteStatus==0){};	//等待写完成
		
		SDCardWriteStatus=0;
		while(SD_GetCardState()){};		//等待SD卡空闲
		sta=HAL_OK;
	}
	SCB_CleanInvalidateDCache();//cache一致性	
    return sta;
} 

//SDMMC1中断服务函数
void SDMMC1_IRQHandler(void)
{
    HAL_SD_IRQHandler(&hsd1);
}
void DMA2_Stream3_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_sdmmc1_rx);
}
void DMA2_Stream6_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_sdmmc1_tx);
}
//SDMMC1写完成回调函数
void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
{
	SDCardWriteStatus=1; //标记写完成
}

//SDMMC1读完成回调函数
void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
{
	SDCardReadStatus=1;	//标记读完成
}
#else

//读SD卡
//buf:读数据缓存区
//sector:扇区地址
//cnt:扇区个数	
//返回值:错误状态;0,正常;其他,错误代码;
uint8_t SD_ReadDisk(uint8_t* buf,uint32_t sector,uint32_t cnt)
{
    uint8_t sta=HAL_OK;
	uint32_t timeout=SD_TIMEOUT;
    long long lsector=sector;
    INTX_DISABLE();//关闭总中断(POLLING模式,严禁中断打断SDIO读写操作!!!)
	sta=HAL_SD_ReadBlocks(&hsd1, (uint8_t*)buf,lsector,cnt,SD_TIMEOUT);//多个sector的读操作
	
	//等待SD卡读完
	while(SD_GetCardState()!=SD_TRANSFER_OK)
    {
		if(timeout-- == 0)
		{	
			sta=SD_TRANSFER_BUSY;
            break;
		}
    }
    INTX_ENABLE();//开启总中断
    return sta;
}  


//写SD卡
//buf:写数据缓存区
//sector:扇区地址
//cnt:扇区个数	
//返回值:错误状态;0,正常;其他,错误代码;	
uint8_t SD_WriteDisk(uint8_t *buf,uint32_t sector,uint32_t cnt)
{   
    uint8_t sta=HAL_OK;
	uint32_t timeout=SD_TIMEOUT;
    long long lsector=sector;
    INTX_DISABLE();//关闭总中断(POLLING模式,严禁中断打断SDIO读写操作!!!)
	sta=HAL_SD_WriteBlocks(&hsd1,(uint8_t*)buf,lsector,cnt,SD_TIMEOUT);//多个sector的写操作
		
	//等待SD卡写完
	while(SD_GetCardState()!=SD_TRANSFER_OK)
    {
		if(timeout-- == 0)
		{	
			sta=SD_TRANSFER_BUSY;
            break;
		}
    }    
	INTX_ENABLE();//开启总中断
    return sta;
}
#endif

void read_sdinfo()
{
	SD_GetCardInfo(&SDCardInfo);
	printf("BlockNbr:%d\r\n",SDCardInfo.BlockNbr);
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
