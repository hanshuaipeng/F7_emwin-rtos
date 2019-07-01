#include "key.h"
/*****************************************
函数功能：按键扫描
入口参数：mode：0，不支持连按。1，支持连按
返回值：按键值
*****************************************/
uint8_t Key_Scan(uint8_t mode)
{
    static uint8_t key_up=1;
    if(mode == 1)
    {
        key_up=1;
    }
    if(key_up&&(KEY0==0||KEY1==0||KEY2==0||WK_UP==1))
    {
        HAL_Delay(10);
        key_up=0;
        if(KEY0 == 0) 
        {            
            return KEY0_PRES;
        }
        else if(KEY1 == 0)  
        {
            return KEY1_PRES;
        }
        else if(KEY2 == 0)  
        {
            return KEY2_PRES;
        }
        else if(WK_UP == 1) 
        {
            return WKUP_PRES;
        }
    }
    else if(KEY0==1&&KEY1==1&&KEY2==1&&WK_UP==0)
    {
        key_up=1;
    }
    return 0;
}


