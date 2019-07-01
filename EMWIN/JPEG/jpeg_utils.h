#ifndef __JPEG_UTILS_H
#define __JPEG_UTILS_H
#include "stm32f7xx.h"
#include "core_cm7.h"
#include "stm32f7xx_hal.h"
//////////////////////////////////////////////////////////////////////////////////  
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//提供STM32F7 JPEG硬件解码  MCU(Minimum Coded Unit)块到RGB的颜色转换函数  
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/7/26
//版本：V1.0
//注意:YCbCr到RGB的转换,没有对不是MCU像素整数倍宽度的图像做处理,所以,当图片的宽度
//不是单个MCU所含像素的整数倍时,会在图片左侧出现一些条纹.这是正常的,请保证图片宽度
//为MCU所含像素的整数倍即可,即一般是16的倍数.
//********************************************************************************
//修改说明
//无
//////////////////////////////////////////////////////////////////////////////////  

//根据自己的需求,修改JPEG_BYTES_PER_PIXEL的值
#define JPEG_BYTES_PER_PIXEL 2		//输出像素大小(单位:字节)
									//2:RGB565
									//3:RGB888
									//4:ARGB8888
															
									
//////////////////////////////////////////////////////////////////////////////////  									
#if	JPEG_BYTES_PER_PIXEL>2
#define JPEG_RED_OFFSET      16		//Offset of the RED color in a pixel        
#define JPEG_GREEN_OFFSET    8		//Offset of the GREEN color in a pixel          
#define JPEG_BLUE_OFFSET     0		//Offset of the BLUE color in a pixel        
#define JPEG_ALPHA_OFFSET    24 	//Offset of the Transparency Alpha in a pixel
#endif


//函数返回值定义
#define JPEG_OK		0X00	//正常
#define JPEG_ERROR	0X01	//错误

//JPEG MCU到RGB转换需要使用的部分参数结构体
typedef struct __JPEG_MCU_RGB_ConvertorTypeDef
{
	uint32_t ColorSpace;
	uint32_t ChromaSubsampling; 
	uint32_t ImageWidth;
	uint32_t ImageHeight;
	uint32_t ImageSize_Bytes; 
	uint32_t LineOffset; 
	uint32_t H_factor;
	uint32_t V_factor; 
	uint32_t WidthExtend;
	uint32_t ScaledWidth; 
	uint32_t MCU_Total_Nb; 
}JPEG_MCU_RGB_ConvertorTypeDef;


#define YCBCR_420_BLOCK_SIZE		384				//YCbCr 4:2:0 MCU : 4 8x8 blocks of Y + 1 8x8 block of Cb + 1 8x8 block of Cr   
#define YCBCR_422_BLOCK_SIZE 		256				//YCbCr 4:2:2 MCU : 2 8x8 blocks of Y + 1 8x8 block of Cb + 1 8x8 block of Cr   
#define YCBCR_444_BLOCK_SIZE 		192				//YCbCr 4:4:4 MCU : 1 8x8 block of Y + 1 8x8 block of Cb + 1 8x8 block of Cr   
#define GRAY_444_BLOCK_SIZE 		64				//GrayScale MCU : 1 8x8 block of Y 
#define CMYK_444_BLOCK_SIZE 		256				//CMYK MCU : 1 8x8 blocks of Cyan + 1 8x8 block Magenta + 1 8x8 block of Yellow and 1 8x8 block of BlacK


typedef uint32_t (* JPEG_YCbCrToRGB_Convert_Function)(uint8_t *pInBuffer,uint8_t *pOutBuffer,uint32_t BlockIndex,uint32_t DataCount);


uint32_t JPEG_MCU_YCbCr420_ARGB_ConvertBlocks(uint8_t *pInBuffer,uint8_t *pOutBuffer,uint32_t BlockIndex,uint32_t DataCount);
uint32_t JPEG_MCU_YCbCr422_ARGB_ConvertBlocks(uint8_t *pInBuffer,uint8_t *pOutBuffer,uint32_t BlockIndex,uint32_t DataCount);
uint32_t JPEG_MCU_YCbCr444_ARGB_ConvertBlocks(uint8_t *pInBuffer,uint8_t *pOutBuffer,uint32_t BlockIndex,uint32_t DataCount);
uint32_t JPEG_MCU_Gray_ARGB_ConvertBlocks(uint8_t *pInBuffer,uint8_t *pOutBuffer,uint32_t BlockIndex,uint32_t DataCount);
uint32_t JPEG_MCU_YCCK_ARGB_ConvertBlocks(uint8_t *pInBuffer,uint8_t *pOutBuffer,uint32_t BlockIndex,uint32_t DataCount);
uint8_t JPEG_GetDecodeColorConvertFunc(JPEG_ConfTypeDef *pJpegInfo, JPEG_YCbCrToRGB_Convert_Function *pFunction, uint32_t *ImageNbMCUs);

#endif 



















