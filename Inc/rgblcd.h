#ifndef _rgblcd_H
#define _rgblcd_H
#include "main.h"
#include "ltdc.h"
//»­±ÊÑÕÉ«
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //×ØÉ«
#define BRRED 			 0XFC07 //×ØºìÉ«
#define GRAY  			 0X8430 //»ÒÉ«
extern uint32_t BACK_COLOR;
extern uint32_t POINT_COLOR;
void LTDC_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode);
void LTDC_ShowString(uint16_t x,uint16_t y,uint8_t size,uint8_t *str);
void LTDC_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);
void LTDC_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);
void LTDC_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LTDC_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LTDC_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r);
#endif


