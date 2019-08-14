#ifndef __HZFONTUPDATA_H
#define __HZFONTUPDATA_H
#include "GUI.h"
#include "main.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//STemwin 字库升级界面    
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/3/30
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

uint32_t emwin_fupd_prog(uint32_t fsize,uint32_t pos); //EMWIN使用的显示更新进度
uint8_t emwin_updata_fontx(uint8_t *fxpath,uint8_t fx);//EMWIN使用的更新指定字库函数
uint8_t emwin_update_font(uint8_t* src);    //EMWIN使用的字库更新函数
void HzFontupdataCreate(GUI_HWIN hwin);
#endif
