///**
//  ******************************************************************************
//  * File Name          : QUADSPI.c
//  * Description        : This file provides code for the configuration
//  *                      of the QUADSPI instances.
//  ******************************************************************************
//  * @attention
//  *
//  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
//  * All rights reserved.</center></h2>
//  *
//  * This software component is licensed by ST under Ultimate Liberty license
//  * SLA0044, the "License"; You may not use this file except in compliance with
//  * the License. You may obtain a copy of the License at:
//  *                             www.st.com/SLA0044
//  *
//  ******************************************************************************
//  */

///* Includes ------------------------------------------------------------------*/
//#include "quadspi.h"

///* USER CODE BEGIN 0 */

///* USER CODE END 0 */

//QSPI_HandleTypeDef hqspi;

///* QUADSPI init function */
//void MX_QUADSPI_Init(void)
//{

//  hqspi.Instance = QUADSPI;
//  hqspi.Init.ClockPrescaler = 1;
//  hqspi.Init.FifoThreshold = 4;
//  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
//  hqspi.Init.FlashSize = POSITION_VAL(0X2000000)-1;  //SPI FLASH大小，W25Q256大小为32M字节;
//  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_6_CYCLE;
//  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
//  hqspi.Init.FlashID = QSPI_FLASH_ID_1;
//  hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
//  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
//  {
//    Error_Handler();
//  }
//	
//}

//void HAL_QSPI_MspInit(QSPI_HandleTypeDef* qspiHandle)
//{

//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  if(qspiHandle->Instance==QUADSPI)
//  {
//  /* USER CODE BEGIN QUADSPI_MspInit 0 */

//  /* USER CODE END QUADSPI_MspInit 0 */
//    /* QUADSPI clock enable */
//    __HAL_RCC_QSPI_CLK_ENABLE();
//  
//    __HAL_RCC_GPIOF_CLK_ENABLE();
//    __HAL_RCC_GPIOB_CLK_ENABLE();
//    /**QUADSPI GPIO Configuration    
//    PF6     ------> QUADSPI_BK1_IO3
//    PF7     ------> QUADSPI_BK1_IO2
//    PF8     ------> QUADSPI_BK1_IO0
//    PF9     ------> QUADSPI_BK1_IO1
//    PB2     ------> QUADSPI_CLK
//    PB6     ------> QUADSPI_BK1_NCS 
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
//    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

//    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
//    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

//    GPIO_InitStruct.Pin = GPIO_PIN_2;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//    GPIO_InitStruct.Pin = GPIO_PIN_6;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//  /* USER CODE BEGIN QUADSPI_MspInit 1 */

//  /* USER CODE END QUADSPI_MspInit 1 */
//  }
//}

//void HAL_QSPI_MspDeInit(QSPI_HandleTypeDef* qspiHandle)
//{

//  if(qspiHandle->Instance==QUADSPI)
//  {
//  /* USER CODE BEGIN QUADSPI_MspDeInit 0 */

//  /* USER CODE END QUADSPI_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_QSPI_CLK_DISABLE();
//  
//    /**QUADSPI GPIO Configuration    
//    PF6     ------> QUADSPI_BK1_IO3
//    PF7     ------> QUADSPI_BK1_IO2
//    PF8     ------> QUADSPI_BK1_IO0
//    PF9     ------> QUADSPI_BK1_IO1
//    PB2     ------> QUADSPI_CLK
//    PB6     ------> QUADSPI_BK1_NCS 
//    */
//    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9);

//    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_2|GPIO_PIN_6);

//  /* USER CODE BEGIN QUADSPI_MspDeInit 1 */

//  /* USER CODE END QUADSPI_MspDeInit 1 */
//  }
//} 

///* USER CODE BEGIN 1 */
////QSPI发送命令
////instruction:要发送的指令
////address:发送到的目的地址
////dummyCycles:空指令周期数
////	instructionMode:指令模式;QSPI_INSTRUCTION_NONE,QSPI_INSTRUCTION_1_LINE,QSPI_INSTRUCTION_2_LINE,QSPI_INSTRUCTION_4_LINE  
////	addressMode:地址模式; QSPI_ADDRESS_NONE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_2_LINE,QSPI_ADDRESS_4_LINE
////	addressSize:地址长度;QSPI_ADDRESS_8_BITS,QSPI_ADDRESS_16_BITS,QSPI_ADDRESS_24_BITS,QSPI_ADDRESS_32_BITS
////	dataMode:数据模式; QSPI_DATA_NONE,QSPI_DATA_1_LINE,QSPI_DATA_2_LINE,QSPI_DATA_4_LINE

//void QSPI_Send_CMD(uint32_t instruction,uint32_t address,uint32_t dummyCycles,uint32_t instructionMode,uint32_t addressMode,uint32_t addressSize,uint32_t dataMode)
//{
//    QSPI_CommandTypeDef Cmdhandler;
//    
//    Cmdhandler.Instruction=instruction;                 	//指令
//    Cmdhandler.Address=address;                            	//地址
//    Cmdhandler.DummyCycles=dummyCycles;                     //设置空指令周期数
//    Cmdhandler.InstructionMode=instructionMode;				//指令模式
//    Cmdhandler.AddressMode=addressMode;   					//地址模式
//    Cmdhandler.AddressSize=addressSize;   					//地址长度
//    Cmdhandler.DataMode=dataMode;             				//数据模式
//    Cmdhandler.SIOOMode=QSPI_SIOO_INST_EVERY_CMD;       	//每次都发送指令
//    Cmdhandler.AlternateByteMode=QSPI_ALTERNATE_BYTES_NONE; //无交替字节
//    Cmdhandler.DdrMode=QSPI_DDR_MODE_DISABLE;           	//关闭DDR模式
//    Cmdhandler.DdrHoldHalfCycle=QSPI_DDR_HHC_ANALOG_DELAY;

//    HAL_QSPI_Command(&hqspi,&Cmdhandler,5000);
//}
////QSPI接收指定长度的数据
////buf:接收数据缓冲区首地址
////datalen:要传输的数据长度
////返回值:0,正常
////    其他,错误代码
//uint8_t QSPI_Receive(uint8_t* buf,uint32_t datalen)
//{
//    hqspi.Instance->DLR=datalen-1;                           //配置数据长度
//    if(HAL_QSPI_Receive(&hqspi,buf,5000)==HAL_OK) 
//		return 0;  //接收数据
//    else 
//		return 1;
//}

////QSPI发送指定长度的数据
////buf:发送数据缓冲区首地址
////datalen:要传输的数据长度
////返回值:0,正常
////    其他,错误代码
//uint8_t QSPI_Transmit(uint8_t* buf,uint32_t datalen)
//{
//    hqspi.Instance->DLR=datalen-1;                            //配置数据长度
//    if(HAL_QSPI_Transmit(&hqspi,buf,5000)==HAL_OK) return 0;  //发送数据
//    else return 1;
//}
///* USER CODE END 1 */

///************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
#include "quadspi.h"
#include "mysys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板 
//QSPI 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/7/18
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

//等待状态标志
//flag:需要等待的标志位
//sta:需要等待的状态
//wtime:等待时间
//返回值:0,等待成功.
//	     1,等待失败.
u8 QSPI_Wait_Flag(u32 flag,u8 sta,u32 wtime)
{
	u8 flagsta=0;
	while(wtime)
	{
		flagsta=(QUADSPI->SR&flag)?1:0; 
		if(flagsta==sta)break;
		wtime--;
	}
	if(wtime)return 0;
	else return 1;
}
//GPIO复用设置
//GPIOx:GPIOA~GPIOI.
//BITx:0~15,代表IO引脚编号.
//AFx:0~15,代表AF0~AF15.
//AF0~15设置情况(这里仅是列出常用的,详细的请见429/746数据手册,Table 12):
//AF0:MCO/SWD/SWCLK/RTC   		AF1:TIM1/TIM2;            		AF2:TIM3~5;               		AF3:TIM8~11
//AF4:I2C1~I2C4;          		AF5:SPI1~SPI6;            		AF6:SPI3/SAI1;            	 	AF7:SPI2/3/USART1~3/UART5/SPDIFRX;
//AF8:USART4~8/SPDIFRX/SAI2; 	AF9;CAN1~2/TIM12~14/LCD/QSPI; 	AF10:USB_OTG/USB_HS/SAI2/QSPI  	AF11:ETH
//AF12:FMC/SDMMC/OTG/HS   		AF13:DCIM                 		AF14:LCD;                  		AF15:EVENTOUT
void GPIO_AF_Set(GPIO_TypeDef* GPIOx,u8 BITx,u8 AFx)
{  
	GPIOx->AFR[BITx>>3]&=~(0X0F<<((BITx&0X07)*4));
	GPIOx->AFR[BITx>>3]|=(u32)AFx<<((BITx&0X07)*4);
}   
//GPIO通用设置 
//GPIOx:GPIOA~GPIOI.
//BITx:0X0000~0XFFFF,位设置,每个位代表一个IO,第0位代表Px0,第1位代表Px1,依次类推.比如0X0101,代表同时设置Px0和Px8.
//MODE:0~3;模式选择,0,输入(系统复位默认状态);1,普通输出;2,复用功能;3,模拟输入.
//OTYPE:0/1;输出类型选择,0,推挽输出;1,开漏输出.
//OSPEED:0~3;输出速度设置,0,低速;1,中速;2,快速;3,高速. 
//PUPD:0~3:上下拉设置,0,不带上下拉;1,上拉;2,下拉;3,保留.
//注意:在输入模式(普通输入/模拟输入)下,OTYPE和OSPEED参数无效!!
void GPIO_Set(GPIO_TypeDef* GPIOx,u32 BITx,u32 MODE,u32 OTYPE,u32 OSPEED,u32 PUPD)
{  
	u32 pinpos=0,pos=0,curpin=0;
	for(pinpos=0;pinpos<16;pinpos++)
	{
		pos=1<<pinpos;	//一个个位检查 
		curpin=BITx&pos;//检查引脚是否要设置
		if(curpin==pos)	//需要设置
		{
			GPIOx->MODER&=~(3<<(pinpos*2));	//先清除原来的设置
			GPIOx->MODER|=MODE<<(pinpos*2);	//设置新的模式 
			if((MODE==0X01)||(MODE==0X02))	//如果是输出模式/复用功能模式
			{  
				GPIOx->OSPEEDR&=~(3<<(pinpos*2));	//清除原来的设置
				GPIOx->OSPEEDR|=(OSPEED<<(pinpos*2));//设置新的速度值  
				GPIOx->OTYPER&=~(1<<pinpos) ;		//清除原来的设置
				GPIOx->OTYPER|=OTYPE<<pinpos;		//设置新的输出模式
			}  
			GPIOx->PUPDR&=~(3<<(pinpos*2));	//先清除原来的设置
			GPIOx->PUPDR|=PUPD<<(pinpos*2);	//设置新的上下拉
		}
	}
} 
//初始化QSPI接口
//返回值:0,成功;
//       1,失败;
u8 MX_QUADSPI_Init(void)
{
	u32 tempreg=0;
	RCC->AHB1ENR|=1<<1;    		//使能PORTB时钟	   
	RCC->AHB1ENR|=1<<5;    		//使能PORTF时钟	   
	RCC->AHB3ENR|=1<<1;   		//QSPI时钟使能
	GPIO_Set(GPIOB,1<<2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);	//PB2复用功能输出	
	GPIO_Set(GPIOB,1<<6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);	//PB6复用功能输出	
	GPIO_Set(GPIOF,0XF<<6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);	//PF6~9复用功能输出	
   	GPIO_AF_Set(GPIOB,2,9);		//PB2,AF9
 	GPIO_AF_Set(GPIOB,6,10);	//PB6,AF10
 	GPIO_AF_Set(GPIOF,6,9);		//PF6,AF9 
 	GPIO_AF_Set(GPIOF,7,9);		//PF7,AF9
 	GPIO_AF_Set(GPIOF,8,10);	//PF8,AF10
 	GPIO_AF_Set(GPIOF,9,10);	//PF9,AF10
	
	RCC->AHB3RSTR|=1<<1;		//复位QSPI
	RCC->AHB3RSTR&=~(1<<1);		//停止复位QSPI
	if(QSPI_Wait_Flag(1<<5,0,0XFFFF)==0)//等待BUSY空闲
	{
		tempreg=(3-1)<<24;		//设置QSPI时钟为AHB时钟的1/3,即216M/3=72Mhz,13.8ns
		tempreg|=(4-1)<<8;		//设置FIFO阈值为4个字节(最大为31,表示32个字节)
		tempreg|=0<<7;			//选择FLASH1
		tempreg|=0<<6;			//禁止双闪存模式
		tempreg|=1<<4;			//采样移位半个周期(DDR模式下,必须设置为0)
		QUADSPI->CR=tempreg;	//设置CR寄存器
		tempreg=(25-1)<<16;		//设置FLASH大小为2^25=32MB
		tempreg|=(4-1)<<8;		//片选高电平时间为4个时钟(13.8*4=55.2ns),即手册里面的tSHSL参数
		tempreg|=1<<0;			//Mode3,空闲时CLK为高电平
		QUADSPI->DCR=tempreg;	//设置DCR寄存器
		QUADSPI->CR|=1<<0;		//使能QSPI
	}else return 1;
	return 0;
}

//QSPI发送命令
//cmd:要发送的指令
//addr:发送到的目的地址
//mode:模式,详细位定义如下:
//	mode[1:0]:指令模式;00,无指令;01,单线传输指令;10,双线传输指令;11,四线传输指令.
//	mode[3:2]:地址模式;00,无地址;01,单线传输地址;10,双线传输地址;11,四线传输地址.
//	mode[5:4]:地址长度;00,8位地址;01,16位地址;10,24位地址;11,32位地址.
//	mode[7:6]:数据模式;00,无数据;01,单线传输数据;10,双线传输数据;11,四线传输数据.
//dmcycle:空指令周期数
void QSPI_Send_CMD(u8 cmd,u32 addr,u8 mode,u8 dmcycle)
{
	u32 tempreg=0;	
	u8 status;
	if(QSPI_Wait_Flag(1<<5,0,0XFFFF)==0)	//等待BUSY空闲
	{
		tempreg=0<<31;						//禁止DDR模式
		tempreg|=0<<28;						//每次都发送指令
		tempreg|=0<<26;						//间接写模式
		tempreg|=((u32)mode>>6)<<24;		//设置数据模式
		tempreg|=(u32)dmcycle<<18;			//设置空指令周期数
		tempreg|=((u32)(mode>>4)&0X03)<<12;	//设置地址长度
		tempreg|=((u32)(mode>>2)&0X03)<<10;	//设置地址模式
		tempreg|=((u32)(mode>>0)&0X03)<<8;	//设置指令模式
		tempreg|=cmd;						//设置指令
		QUADSPI->CCR=tempreg;				//设置CCR寄存器
		if(mode&0X0C)						//有指令+地址要发送
		{
			QUADSPI->AR=addr;				//设置地址寄存器
		} 
		if((mode&0XC0)==0)					//无数据传输,等待指令发送完成
		{
			status=QSPI_Wait_Flag(1<<1,1,0XFFFF);//等待TCF,即传输完成
			if(status==0)
			{
				QUADSPI->FCR|=1<<1;			//清除TCF标志位 
			}
		}
	}	
}

//QSPI接收指定长度的数据
//buf:接收数据缓冲区首地址
//datalen:要传输的数据长度
//返回值:0,正常
//    其他,错误代码
u8 QSPI_Receive(u8* buf,u32 datalen)
{
	u32 tempreg=QUADSPI->CCR;
	u32 addrreg=QUADSPI->AR; 	
	u8 status;
	vu32 *data_reg=&QUADSPI->DR;
	QUADSPI->DLR=datalen-1;					//设置数据传输长度
	tempreg&=~(3<<26);						//清除FMODE原来的设置
	tempreg|=1<<26;							//设置FMODE为间接读取模式
	QUADSPI->FCR|=1<<1;						//清除TCF标志位
	QUADSPI->CCR=tempreg;					//回写CCR寄存器
	QUADSPI->AR=addrreg;					//回写AR寄存器,触发传输
	while(datalen)
	{
		status=QSPI_Wait_Flag(3<<1,1,0XFFFF);//等到FTF和TCF,即接收到了数据
		if(status==0)						//等待成功
		{
			*buf++=*(vu8 *)data_reg;
			datalen--;
		}else break;
	}
	if(status==0)
	{
		QUADSPI->CR|=1<<2;							//终止传输 
		status=QSPI_Wait_Flag(1<<1,1,0XFFFF);		//等待TCF,即数据传输完成
		if(status==0)
		{
			QUADSPI->FCR|=1<<1;						//清除TCF标志位 
			status=QSPI_Wait_Flag(1<<5,0,0XFFFF);	//等待BUSY位清零
		}
	}
	return status;
} 

//QSPI发送指定长度的数据
//buf:发送数据缓冲区首地址
//datalen:要传输的数据长度
//返回值:0,正常
//    其他,错误代码
u8 QSPI_Transmit(u8* buf,u32 datalen)
{
	u32 tempreg=QUADSPI->CCR;
	u32 addrreg=QUADSPI->AR; 
	u8 status;
	vu32 *data_reg=&QUADSPI->DR;
	QUADSPI->DLR=datalen-1;					//设置数据传输长度
	tempreg&=~(3<<26);						//清除FMODE原来的设置
	tempreg|=0<<26;							//设置FMODE为间接写入模式
	QUADSPI->FCR|=1<<1;						//清除TCF标志位
	QUADSPI->CCR=tempreg;					//回写CCR寄存器 
	while(datalen)
	{
		status=QSPI_Wait_Flag(1<<2,1,0XFFFF);//等到FTF
		if(status!=0)						//等待成功
		{
			break;
		}
		*(vu8 *)data_reg=*buf++;
		datalen--;
	}
	if(status==0)
	{
		QUADSPI->CR|=1<<2;							//终止传输 
		status=QSPI_Wait_Flag(1<<1,1,0XFFFF);		//等待TCF,即数据传输完成
		if(status==0)
		{
			QUADSPI->FCR|=1<<1;						//清除TCF标志位 
			status=QSPI_Wait_Flag(1<<5,0,0XFFFF);	//等待BUSY位清零
		}
	}
	return status;
}




















