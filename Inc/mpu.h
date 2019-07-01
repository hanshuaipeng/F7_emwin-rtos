#ifndef __MPU_H
#define __MPU_H
#include "main.h"


uint8_t MPU_Set_Protection(uint32_t baseaddr,uint32_t size,uint32_t rnum,uint32_t ap);
void MPU_Memory_Protection(void);
#endif
