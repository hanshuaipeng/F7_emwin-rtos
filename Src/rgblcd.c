#include "rgblcd.h"
#include "font.h"

//LCD的画笔颜色和背景色	   
uint32_t POINT_COLOR=BLACK;		//画笔颜色
uint32_t BACK_COLOR =WHITE;  	//背景色 
/************************************************************************************************
函数功能：指定位置显示一个字符
入口参数：（x,y）坐标
			num：显示的字符
			size：显示的大小12/16/24/32
			mode:叠加方式(1)还是非叠加方式(0)
返回值：无
************************************************************************************************/
void LTDC_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
{
	 uint8_t temp,t1,t;
	uint16_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
 	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[num][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[num][t];	//调用2412字体
		else if(size==32)temp=asc2_3216[num][t];	//调用3216字体
		else return;								//没有的字库
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LTDC_Draw_Point(x,y,POINT_COLOR);
			else if(mode==0)LTDC_Draw_Point(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=lcdltdc.height)
			{
				return;		//超区域了
			}
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcdltdc.width)
				{
					return;	//超区域了
				}
				break;
			}
		}  	 
	} 
}

/************************************************************************************************
函数功能：指定位置显示字符串
入口参数：（x,y）坐标
			str：显示的字符串
			size：显示的大小12/16/24/32
返回值：无
************************************************************************************************/
void LTDC_ShowString(uint16_t x,uint16_t y,uint8_t size,uint8_t *str)
{
	 while((*str<='~')&&(*str>=' '))//判断是不是非法字符!
    {       
        LTDC_ShowChar(x,y,*str,size,0);
        x+=size/2;
        str++;
    }  
}
/************************************************************************************************
函数功能：m^n函数
入口参数：
返回值：m^n次方.
************************************************************************************************/
uint32_t LCD_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}
/************************************************************************************************
函数功能：显示数字,高位为0,则不显示
入口参数：	x,y :起点坐标	 
			len :数字的位数
			size:字体大小
			color:颜色 
			num:数值(0~4294967295);
返回值：
************************************************************************************************/ 
void LTDC_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LTDC_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LTDC_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
/************************************************************************************************
函数功能：显示数字,高位为0,则不显示
入口参数：	x,y :起点坐标	 
			num:数值(0~999999999);	
			len:长度(即要显示的位数)
			size:字体大小
			num:数值(0~4294967295);
			mode:
				[7]:0,不填充;1,填充0.
				[6:1]:保留
				[0]:0,非叠加显示;1,叠加显示.
返回值：
************************************************************************************************/ 
void LTDC_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
{  
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LTDC_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LTDC_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LTDC_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
/************************************************************************************************
函数功能：画线
入口参数：	x1,y1:起点坐标
			x2,y2:终点坐标  
返回值：
************************************************************************************************/ 
void LTDC_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LTDC_Draw_Point(uRow,uCol,POINT_COLOR);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 
/************************************************************************************************
函数功能：画矩形	  
入口参数：(x1,y1),(x2,y2):矩形的对角坐标
返回值：
************************************************************************************************/ 
void LTDC_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	LTDC_DrawLine(x1,y1,x2,y1);
	LTDC_DrawLine(x1,y1,x1,y2);
	LTDC_DrawLine(x1,y2,x2,y2);
	LTDC_DrawLine(x2,y1,x2,y2);
}
/************************************************************************************************
函数功能：在指定位置画一个指定大小的圆
入口参数：(x,y):中心点
			r :半径
返回值：
************************************************************************************************/
void LTDC_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LTDC_Draw_Point(x0+a,y0-b,POINT_COLOR);             //5
 		LTDC_Draw_Point(x0+b,y0-a,POINT_COLOR);             //0           
		LTDC_Draw_Point(x0+b,y0+a,POINT_COLOR);             //4               
		LTDC_Draw_Point(x0+a,y0+b,POINT_COLOR);             //6 
		LTDC_Draw_Point(x0-a,y0+b,POINT_COLOR);             //1       
 		LTDC_Draw_Point(x0-b,y0+a,POINT_COLOR);             
		LTDC_Draw_Point(x0-a,y0-b,POINT_COLOR);             //2             
  		LTDC_Draw_Point(x0-b,y0-a,POINT_COLOR);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 	
