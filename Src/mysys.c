#include "mysys.h"


#ifdef  USE_FULL_ASSERT
//当编译提示出错的时候此函数用来报告错误的文件和所在行
//file：指向源文件
//line：指向在文件中的行数
void assert_failed(uint8_t* file, uint32_t line)
{ 
	while (1)
	{
	}
}
#endif

//判断I_Cache是否打开
//返回值:0 关闭，1 打开
u8 Get_ICahceSta(void)
{
    u8 sta;
    sta=((SCB->CCR)>>17)&0X01;
    return sta;
}

//判断I_Dache是否打开
//返回值:0 关闭，1 打开
u8 Get_DCahceSta(void)
{
    u8 sta;
    sta=((SCB->CCR)>>16)&0X01;
    return sta;
}

//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
__asm void WFI_SET(void)
{
	WFI;		  
}
//关闭所有中断(但是不包括fault和NMI中断)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//开启所有中断
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}
