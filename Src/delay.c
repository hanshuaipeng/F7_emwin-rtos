#include "delay.h"
#include "FreeRTOS.h"					//FreeRTOS使用	 
#include "task.h"
static uint32_t fac_us=0;							//us延时倍乘数
static uint32_t fac_ms=0;		   
//初始化延迟函数
//当使用ucos的时候,此函数会初始化ucos的时钟节拍
//SYSTICK的时钟固定为AHB时钟的1/8
//SYSCLK:系统时钟频率
void delay_init(uint8_t SYSCLK)
{
    uint32_t reload;
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);//SysTick频率为HCLK
	fac_us=SYSCLK;						    //不论是否使用OS,fac_us都需要使用
	reload=SYSCLK;					        //每秒钟的计数次数 单位为K	   
	reload*=1000000/configTICK_RATE_HZ;		//根据delay_ostickspersec设定溢出时间
											//reload为24位寄存器,最大值:16777216,在216M下,约合77.7ms左右	
	fac_ms=1000/configTICK_RATE_HZ;			//代表OS可以延时的最少单位	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;//开启SYSTICK中断
	SysTick->LOAD=reload; 					//每1/OS_TICKS_PER_SEC秒中断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //开启SYSTICK
					    //不论是否使用OS,fac_us都需要使用
}								    
/*
时钟周期为1/216M,则1us需要216个周期，所以nus需要的节拍为nus*216
*/
//延时nus
//nus为要延时的us数.	
//注意:nus的值不要大于1000us
void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				//LOAD的值	
	ticks=nus*fac_us; 						//需要的节拍数 
	told=SysTick->VAL;        				//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
		}  
	};
}


			 



































