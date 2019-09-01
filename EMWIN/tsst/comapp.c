/*
******************************************************************
**                      uCGUIBuilder                            **
**                  Version:   4.0.0.0                          **
**                     2012 / 04                               **
**                   CpoyRight to: wyl                          **
**              Email:ucguibuilder@163.com                        **
**          This text was Created by uCGUIBuilder               **
******************************************************************/

#include <stddef.h>
#include "comapp.h"
#include "GUI.h"
#include "DIALOG.h"

#include "WM.h"
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "LISTBOX.h"
#include "MULTIEDIT.h"
#include "RADIO.h"
#include "SLIDER.h"
#include "TEXT.h"
#include "PROGBAR.h"
#include "SCROLLBAR.h"
#include "LISTVIEW.h"
#include "EmWinHZFont.h"
#include "keypad.h"
#include "malloc.h"
#include "Freertos.h"
#include "task.h"
#include "queue.h"
#include "usart.h"
//TOUCH任务
//设置任务优先级
#define UART_TASK_PRIO			5
//任务堆栈大小
#define UART_STK_SIZE			512
//任务句柄
TaskHandle_t UARTTask_Handler;
//uart任务
void uart_task(void *pvParameters);


QueueHandle_t UART_Queue;
MULTIEDIT_HANDLE RecShow_Handle;
GUI_HWIN Com_Dialog;
//EventsFunctionList
void OnButtonReleased(WM_MESSAGE * pMsg);
void OnDropDownSelChanged(WM_MESSAGE * pMsg);
void OnMultiEditSelChanged(WM_MESSAGE * pMsg);
//EndofEventsFunctionList


void UART_TaskCreat(void)
{
	taskENTER_CRITICAL();           //进入临界区
	//创建UART任务
    xTaskCreate((TaskFunction_t )uart_task,             
                (const char*    )"uart_task",           
                (uint16_t       )UART_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )UART_TASK_PRIO,        
                (TaskHandle_t*  )&UARTTask_Handler);  
	UART_Queue=xQueueCreate(4,REC_LEN);
	if(UART_Queue==0)
	{
		printf("creat fail\r\n");
	}
	taskEXIT_CRITICAL();            //退出临界区
}
/**************************************************
函数功能：	字符串转换位16进制
入口参数：	*src：需要转换的数组
			*dst：转换后的数组
			len：数据长度
返回值：	无
**************************************************/
void HexSring_To_Hex(char *src,char *dst,uint16_t len)
{
	int i = 0,j = 0;
	for(i=0,j=0;j<len/2;i+=2,j++)
	{
		if((src[i]<='9')&&(src[i]>='0'))				
		{
			dst[j] =  src[i]-'0';
			dst[j] = (dst[j]<<4);
		}
		else  if((src[i]>='A')&&(src[i]<='F'))
		{
			dst[j] =  src[i]-55;
		    dst[j] =(dst[j]<<4) ;
		}
		else  if((src[i]>='a')&&(src[i]<='f'))
		{
			dst[j] =  src[i]-87;
		    dst[j] =(dst[j]<<4);	
		}
		if((src[i+1]<='9')&&(src[i+1]>='0'))dst[j] |= (src[i+1] - '0');
		else if((src[i+1]>='A')&&(src[i+1]<='F'))	dst[j] |= (src[i+1] - 55);
		else  if((src[i+1]>='a')&&(src[i+1]<='f'))dst[j] |= (src[i+1] - 87);		
	}
}
/**************************************************
函数功能:	16进制转换字符串
入口参数：	*src：需要转换的数组
			*dst：转换后的数组
			len：数据长度
返回值：	无
**************************************************/
void Hex_To_HexString(char *src,char *dst,uint16_t len)
{
    int i = 0,j = 0;
    for(i=0,j=0;j<len*2;i++,j++)
    {
		if(src[i]<=9)
	    {
			dst[j] =  '0';
		    dst[++j] = src[i] + '0';	
		}
		else  if(src[i]>=0x0A&&src[i]<=0x0F)
		{
			dst[j] =  '0';
		    dst[++j] = src[i] + 55;	
		}
		else if (src[i]>=0x0F)
		{
			dst[j] =  src[i]/16 +'0';
			switch (dst[j])
			{
				case '0': dst[j] = '0';break;
				case '1': dst[j] = '1';break;
				case '2': dst[j] = '2';break;
				case '3': dst[j] = '3';break;
				case '4': dst[j] = '4';break;
				case '5': dst[j] = '5';break;
				case '6': dst[j] = '6';break;
				case '7': dst[j] = '7';break;
				case '8': dst[j] = '8';break;
				case '9': dst[j] = '9';break;
				case ':': dst[j] = 'A';break;
				case ';': dst[j] = 'B';break;
				case '<': dst[j] = 'C';break;
				case '=': dst[j] = 'D';break;
				case '>': dst[j] = 'E';break;
				case '?': dst[j] = 'F';break;
				default : break;
			}
			dst[++j] =  src[i]%16 +'0';
			switch (dst[j])
			{
				case '0': dst[j] = '0';break;
				case '1': dst[j] = '1';break;
				case '2': dst[j] = '2';break;
				case '3': dst[j] = '3';break;
				case '4': dst[j] = '4';break;
				case '5': dst[j] = '5';break;
				case '6': dst[j] = '6';break;
				case '7': dst[j] = '7';break;
				case '8': dst[j] = '8';break;
				case '9': dst[j] = '9';break;
				case ':': dst[j] = 'A';break;
				case ';': dst[j] = 'B';break;
				case '<': dst[j] = 'C';break;
				case '=': dst[j] = 'D';break;
				case '>': dst[j] = 'E';break;
				case '?': dst[j] = 'F';break;
				default : break;
			}
		}        
	}	 
}
//查询Message_Queue队列中的总队列数量和剩余队列数量,总大小=使用+剩余的。
	
uint8_t check_msg_queue(void)
{
//	uint8_t msgq_remain_size;	//消息队列剩余大小
    uint8_t msgq_use_size;     //消息队列总大小 
//    msgq_remain_size=uxQueueSpacesAvailable(UART_Queue);//得到队列剩余大小
    msgq_use_size=uxQueueMessagesWaiting(UART_Queue);//得到队列总大小，
	return msgq_use_size;
}
void uart_task(void *pvParameters)
{
	BaseType_t xResult;
	char *buffer;
	char *HexString;
	uint16_t queue_len=0;
	while(1)
	{
		buffer=mymalloc(SRAMIN,REC_LEN);
		memset(buffer,0,REC_LEN);	//清除缓冲区
		
		xResult=xQueueReceive(UART_Queue,buffer,portMAX_DELAY);//接收队列清空

		if(xResult==pdTRUE)
		{
			queue_len=(buffer[0]|buffer[1]<<8)-2;//减去数据长度两个字节，即为实际长度
//			printf("ucQueueMsgValue=%s\r\n",buffer);
//			MULTIEDIT_AddText(RecShow_Handle,(const char*)&ucQueueMsgValue);
			if(CHECKBOX_GetState(WM_GetDialogItem(Com_Dialog,GUI_ID_CHECK1)) == 1)//16进制显示
			{
				HexString=mymalloc(SRAMIN,REC_LEN*2);
				memset(HexString,0,REC_LEN*2);	//清除缓冲区
				Hex_To_HexString(buffer+2,HexString,queue_len);//16进制数转换成字符串形式
				MULTIEDIT_AddText(RecShow_Handle,HexString);
				if(CHECKBOX_GetState(WM_GetDialogItem(Com_Dialog,GUI_ID_CHECK2)) == 1)//自动换行
					MULTIEDIT_AddText(RecShow_Handle,"\r\n");
				myfree(SRAMIN,HexString);
			}
			else
			{
				MULTIEDIT_AddText(RecShow_Handle,buffer+2);//数据偏移两个字节
				if(CHECKBOX_GetState(WM_GetDialogItem(Com_Dialog,GUI_ID_CHECK2)) == 1)//自动换行
					MULTIEDIT_AddText(RecShow_Handle,"\r\n");
			}
		}
		myfree(SRAMIN,buffer);		//释放内存
		vTaskDelay(20);
	}
}

/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/

static const GUI_WIDGET_CREATE_INFO _comDialogCreate[] = {
    { FRAMEWIN_CreateIndirect,  "Caption",           0,                       0,  0,  480,272,FRAMEWIN_CF_MOVEABLE,0},
    { MULTIEDIT_CreateIndirect,  NULL,               GUI_ID_MULTIEDIT0,       0, 1,  198,143,0,0},
    { MULTIEDIT_CreateIndirect,  NULL,               GUI_ID_MULTIEDIT1,       0, 152,198,95, 0,0},
    { DROPDOWN_CreateIndirect,   NULL,               GUI_ID_DROPDOWN0,        202,8,  80, 80, 0,0},
    { DROPDOWN_CreateIndirect,   NULL,               GUI_ID_DROPDOWN1,        202,30, 80, 80, 0,0},
    { BUTTON_CreateIndirect,    "OPEN",              GUI_ID_BUTTON1,          309,8, 70, 30, 0,0},
    { BUTTON_CreateIndirect,    "SEND",              GUI_ID_BUTTON2,          380,8, 70, 30, 0,0},
	{ CHECKBOX_CreateIndirect,  "hexsned",           GUI_ID_CHECK0,           202, 52, 90, 20, 0,0},
    { CHECKBOX_CreateIndirect,  "hexrec",            GUI_ID_CHECK1,           202, 74, 90, 20, 0,0},
	{ CHECKBOX_CreateIndirect,  "auto",            GUI_ID_CHECK2,           202, 96, 90, 20, 0,0},
    { BUTTON_CreateIndirect,    "clearrec",          GUI_ID_BUTTON3,          309, 40,70, 30, 0,0},
    { BUTTON_CreateIndirect,    "clearsend",         GUI_ID_BUTTON4,          380, 40,70, 30, 0,0}
};



/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/

void PaintDialog(WM_MESSAGE * pMsg)
{
//    WM_HWIN hWin = pMsg->hWin;
	GUI_Clear();
	GUI_SetBkColor(GUI_BLACK);
}



/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/

void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    //
    //FRAMEWIN
    //
    FRAMEWIN_AddCloseButton(hWin, FRAMEWIN_BUTTON_RIGHT, 0);
    FRAMEWIN_AddMaxButton(hWin, FRAMEWIN_BUTTON_RIGHT, 1);
    FRAMEWIN_AddMinButton(hWin, FRAMEWIN_BUTTON_RIGHT, 2);
    FRAMEWIN_SetTitleHeight(hWin,20);
	FRAMEWIN_SetTextAlign(hWin, GUI_TA_HCENTER | GUI_TA_VCENTER);
	FRAMEWIN_SetFont(hWin,&GUI_FontHZ16);
	FRAMEWIN_SetTextColor(hWin, GUI_BLACK);
	FRAMEWIN_SetText(hWin, "串口调试");
	
	BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON1), &GUI_FontHZ12);
	BUTTON_SetText(WM_GetDialogItem(hWin,GUI_ID_BUTTON1),"打开");
	BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON2), &GUI_FontHZ12);
	BUTTON_SetText(WM_GetDialogItem(hWin,GUI_ID_BUTTON2),"发送");
	BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON3), &GUI_FontHZ12);
	BUTTON_SetText(WM_GetDialogItem(hWin,GUI_ID_BUTTON3),"清空接收");
	BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON4), &GUI_FontHZ12);
	BUTTON_SetText(WM_GetDialogItem(hWin,GUI_ID_BUTTON4),"清空发送");
    //
    //GUI_ID_MULTIEDIT0接收
    //
	RecShow_Handle=WM_GetDialogItem(hWin,GUI_ID_MULTIEDIT0);
	MULTIEDIT_SetFont(RecShow_Handle, &GUI_FontHZ16);
	MULTIEDIT_SetAutoScrollH(RecShow_Handle,1);//自动水平
    MULTIEDIT_SetAutoScrollV(RecShow_Handle,1);//垂直
	MULTIEDIT_SetFocusable(RecShow_Handle,0);
//	MULTIEDIT_SetReadOnly(WM_GetDialogItem(hWin,GUI_ID_MULTIEDIT0),1);
    //
    //GUI_ID_MULTIEDIT1发送
    //
	MULTIEDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_MULTIEDIT1), &GUI_FontHZ16);
	MULTIEDIT_SetAutoScrollH(WM_GetDialogItem(hWin,GUI_ID_MULTIEDIT1),1);
    MULTIEDIT_SetAutoScrollV(WM_GetDialogItem(hWin,GUI_ID_MULTIEDIT1),1);
	MULTIEDIT_EnableBlink(WM_GetDialogItem(hWin,GUI_ID_MULTIEDIT1),500,1);			//开启光标,周期500ms
    MULTIEDIT_SetInsertMode(WM_GetDialogItem(hWin,GUI_ID_MULTIEDIT1),1);  //开启插入模式
    //
    //GUI_ID_DROPDOWN0
    //
    DROPDOWN_SetTextHeight(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN0),12);
    DROPDOWN_SetAutoScroll(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN0),1);
    DROPDOWN_AddString(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN0),"USART1");
    DROPDOWN_AddString(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN0),"USART2");
    DROPDOWN_AddString(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN0),"USART3");
    DROPDOWN_AddString(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN0),"USART4");
    //
    //GUI_ID_DROPDOWN1
    //
    DROPDOWN_SetTextHeight(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN1),14);
    DROPDOWN_SetAutoScroll(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN1),1);
    DROPDOWN_AddString(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN1),"2400");
    DROPDOWN_AddString(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN1),"4800");
    DROPDOWN_AddString(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN1),"9600");
    DROPDOWN_AddString(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN1),"115200");
    DROPDOWN_SetSel(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN1),0);
	 //
    //GUI_ID_CHECK0
    //
	CHECKBOX_SetFont(WM_GetDialogItem(hWin,GUI_ID_CHECK0),&GUI_FontHZ16);
    CHECKBOX_SetText(WM_GetDialogItem(hWin,GUI_ID_CHECK0),"HEX发送");
    //
    //GUI_ID_CHECK1
    //
	CHECKBOX_SetFont(WM_GetDialogItem(hWin,GUI_ID_CHECK1),&GUI_FontHZ16);
    CHECKBOX_SetText(WM_GetDialogItem(hWin,GUI_ID_CHECK1),"HEX显示");
	 //
    //GUI_ID_CHECK2
    //
	CHECKBOX_SetFont(WM_GetDialogItem(hWin,GUI_ID_CHECK2),&GUI_FontHZ16);
    CHECKBOX_SetText(WM_GetDialogItem(hWin,GUI_ID_CHECK2),"自动换行");
}




/*********************************************************************
*
*       Dialog callback routine
*/
static void _comCallback(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
	char *multiedit_data;
	int data_len;
    switch (pMsg->MsgId) 
    {
		case WM_DELETE:
			WM_DeleteWindow(keypad_dev.hKeypad);//删除键盘
			vTaskDelete(UARTTask_Handler);//删除任务
			vQueueDelete(UART_Queue);//删除队列
			break;
		case WM_PID_STATE_CHANGED://点击空白删除键盘
			WM_DeleteWindow(keypad_dev.hKeypad);//删除键盘
			break;
        case WM_PAINT:
            PaintDialog(pMsg);
            break;
        case WM_INIT_DIALOG:
            InitDialog(pMsg);
            break;
        case WM_KEY:
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
            {
                case GUI_KEY_ESCAPE:
                    GUI_EndDialog(hWin, 1);
                    break;
                case GUI_KEY_ENTER:
                    GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (Id) 
            {
                case GUI_ID_OK:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
                case GUI_ID_CANCEL:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
                case GUI_ID_MULTIEDIT1:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
							WM_SetFocus(WM_GetDialogItem(hWin,GUI_ID_MULTIEDIT1));
						if(keypad_dev.hKeypad==NULL)//如果没有创建键盘，创建键盘
							keypad_demo();
                            break;
                    }
                    break;
                case GUI_ID_DROPDOWN0:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_SEL_CHANGED:
//                            OnDropDownSelChanged(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_DROPDOWN1:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_SEL_CHANGED:
//                            OnDropDownSelChanged(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTON1:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
//                            OnButtonReleased(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTON2:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
								data_len=MULTIEDIT_GetTextSize(WM_GetDialogItem(hWin,GUI_ID_MULTIEDIT1));									
								mymalloc(SRAMIN,data_len);
								MULTIEDIT_GetText(WM_GetDialogItem(hWin,GUI_ID_MULTIEDIT1),multiedit_data,data_len);
								if(CHECKBOX_GetState(WM_GetDialogItem(hWin,GUI_ID_CHECK0)) == 1)//16进制发送
								{
//									printf("%02x\r\n",(uint8_t)multiedit_data);
								}
								else
								{
									printf("%s\r\n",multiedit_data);
								}
								myfree(SRAMIN,multiedit_data);
                            break;
                    }
                    break;
				 case GUI_ID_BUTTON3://清空接收
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
							MULTIEDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_MULTIEDIT0),0);
                            break;
                    }
                    break;
						case GUI_ID_BUTTON4://清空发送
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
							MULTIEDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_MULTIEDIT1),0);
                            break;
                    }
                    break;
            }
            break;
        default:
            WM_DefaultProc(pMsg);
    }
}


/*********************************************************************
*
*       MainTask
*
**********************************************************************
*/
void comAppwin(void) 
{ 
	UART_TaskCreat();
    Com_Dialog=GUI_CreateDialogBox(_comDialogCreate, GUI_COUNTOF(_comDialogCreate), &_comCallback, 0, 0, 0);
}

