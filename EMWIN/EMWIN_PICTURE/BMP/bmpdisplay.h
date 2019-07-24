#ifndef _BMPDISPLAY_H
#define _BMPDISPLAY_H
#include "mysys.h"

#define BMPMEMORYSIZE	(8*1024*1024)	//图片大小不大于8M

//绘制无需加载到RAM中的BMP图片时，图片每行的字节数
#define BMPPERLINESIZE	2*1024		

int dispbmp(u8 *BMPFileName,u8 mode,u32 x,u32 y,int member,int denom);
int dispbmpex(u8 *BMPFileName,u8 mode,u32 x,u32 y,int member,int denom);
void emwinbmp_new_pathname(u8 *pname);
void create_bmppicture(u8 *filename,int x0,int y0,int Xsize,int Ysize);
void bmpdisplay_demo(void);
#endif
