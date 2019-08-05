#include "clockapp.h"
#include "GUI.h"
#include "DIALOG.h"
#include "rtc.h"
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
#include "clockapp_usepic.h"
#include "setclock.h"
#define GUI_STCOLOR_LIGHTBLUE   0x00DCA939
#define GUI_STCOLOR_DARKBLUE    0x00522000

/*
*********************************************************************************************************
*	                                  
*	模块名称 : 万年历界面设计
*	文件名称 : App_CalendarDlg.c
*	版    本 : V1.0
*	说    明 : 万年历界面
*              1. GUI主界面左上角的按钮用于弹出时间，日期和闹钟设置界面，右上角的按钮未用到。用户移植这个例子
*                 的话，此按钮用于返回系统主界面。此万年历例子都是在对话框上面实现的，移植很方便。
*              2. 闹钟时间到后，蜂鸣器名叫5声。
*              
*	修改记录 :
*		版本号   日期         作者          说明
*		V1.0    2016-12-23   Eric2013  	    首版   
*                                     
*	Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                            外部文件中定义，供本文件使用
*********************************************************************************************************
*/
extern void App_SetTimeAlarm(void);


/*
*********************************************************************************************************
*                                       宏定义
*********************************************************************************************************
*/
#define countof(Obj) (sizeof(Obj) / sizeof(Obj[0]))
#define DEG2RAD      (3.1415926f / 180)
#define MAG          3

#define ID_WINDOW_0   (GUI_ID_USER + 0x00)
#define ID_BUTTON_0   (GUI_ID_USER + 0x01)
#define ID_BUTTON_1   (GUI_ID_USER + 0x02)


/*
*********************************************************************************************************
*                                       数值
*********************************************************************************************************
*/ 
typedef struct {
  GUI_POINT        aPoints[7];   /* 多边形坐标 */
  float            Angle;        /* 旋转角度 */
  
  GUI_POINT        aPoints1[7];   
  float            Angle1;
	
  GUI_POINT        aPoints2[7];   
  float            Angle2;
} PARAM;

/* 三组指针坐标 */
static const GUI_POINT _aNeedle[] = 
{
	{ MAG * ( -1), MAG * (  0 ) },
	{ MAG * (-1), MAG * (-50 ) },
	{ MAG * (0), MAG * (-65 ) },
	{ MAG * ( 1), MAG * (-50 ) },
	{ MAG * ( 1), MAG * (0 ) },
};

static const GUI_POINT _aNeedle1[] = 
{
	{ MAG * ( -2), MAG * (  0 ) },
	{ MAG * (-2), MAG * (-50 ) },
	{ MAG * (0), MAG * (-65 ) },
	{ MAG * ( 2), MAG * (-50 ) },
	{ MAG * ( 2), MAG * (0 ) },
};

static const GUI_POINT _aNeedle2[] = 
{
	{ MAG * ( -2), MAG * (  0 ) },
	{ MAG * (-2), MAG * (-40 ) },
	{ MAG * (0), MAG * (-55 ) },
	{ MAG * ( 2), MAG * (-40 ) },
	{ MAG * ( 2), MAG * (0 ) },
};

/* 用于星期的显示 */
static const char *ucWeekDay[7] =
{
	"一",
	"二",
	"三",
	"四",
	"五",
	"六",
	"日"
};


/*
*********************************************************************************************************
*	函 数 名: _cbButtonBack
*	功能说明: 按钮自定义回调函数
*	形    参: pMsg  消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbButtonBack(WM_MESSAGE * pMsg) 
{
	WM_HWIN  hWin;

	hWin  = pMsg->hWin;
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
			if (BUTTON_IsPressed(hWin)) 
			{
				GUI_SetColor(GUI_DARKGRAY);
				GUI_AA_FillCircle(100*MAG, 0, 72*MAG);
				GUI_SetBkColor(GUI_DARKGRAY);
				GUI_SetColor(GUI_WHITE); 
				GUI_DrawBitmap(&bmReturn, 40, 5);
			} 
			else 
			{
				GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
				GUI_AA_FillCircle(100*MAG, 0, 72*MAG);
				GUI_SetBkColor(GUI_STCOLOR_LIGHTBLUE);
				GUI_SetColor(GUI_WHITE);  
				GUI_DrawBitmap(&bmReturn, 40, 5);				
			}
			break;
			
		default:
			BUTTON_Callback(pMsg);
	}
}

/*
*********************************************************************************************************
*	函 数 名: _cbButtonList
*	功能说明: 按钮自定义回调函数
*	形    参: pMsg  消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbButtonList(WM_MESSAGE * pMsg) 
{
	WM_HWIN  hWin;

	hWin  = pMsg->hWin;
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
			if (BUTTON_IsPressed(hWin)) 
			{
				GUI_SetColor(GUI_DARKGRAY);
				GUI_AA_FillCircle(0, 0, 72*MAG);
				GUI_SetBkColor(GUI_DARKGRAY);
				GUI_SetColor(GUI_WHITE); 
				
				GUI_DrawBitmap(&bmClockIcon, 2, 5);
			} 
			else 
			{
				GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
				GUI_AA_FillCircle(0, 0, 72*MAG);
				GUI_SetBkColor(GUI_STCOLOR_LIGHTBLUE);
				GUI_SetColor(GUI_WHITE);  

				GUI_DrawBitmap(&bmClockIcon, 2, 5);
			}
			break;
			
		default:
			BUTTON_Callback(pMsg);
	}
}

/*
*********************************************************************************************************
*	函 数 名: Clock_DrawDisp
*	功能说明: 更新时钟表盘
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void Clock_DrawDisp(void) 
{
	PARAM       Param;        /* PARAM类型变量 */

    int      t0;   /* 用于三个指针的计数 */
	int      t1;
    int      t2;
	
	HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStruct,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStruct,RTC_FORMAT_BIN);
	/* 第一个指针计数，用于旋转秒针 */
	t0 = 360 - RTC_TimeStruct.Seconds * 6;

	/* 第二个指针计数，用于旋转分针 */
	t1 = 360 - RTC_TimeStruct.Minutes * 6;
	
	/* 第三个指针计数，用于旋转时针 */
	t2 = 360 - RTC_TimeStruct.Hours * 30;

	/* 旋转第一个指针 */
	Param.Angle= t0;
	Param.Angle *= 3.1415926f / 180;
	GUI_RotatePolygon(Param.aPoints, _aNeedle, countof(_aNeedle), Param.Angle);

	/* 旋转第二个指针 */
	Param.Angle1= t1;
	Param.Angle1 *= 3.1415926f / 180;
	GUI_RotatePolygon(Param.aPoints1, _aNeedle1, countof(_aNeedle1), Param.Angle1);

	/* 旋转第三个指针 */
	Param.Angle2= t2;
	Param.Angle2 *= 3.1415926f / 180;
	GUI_RotatePolygon(Param.aPoints2, _aNeedle2, countof(_aNeedle2), Param.Angle2);
						

	GUI_DrawBitmap(&abmclock, 310, 120);
		
	/* 使能抗锯齿，多任务的情况下，此函数一定要实时调用，比如截图任务切换回来就出错了 */
	GUI_AA_EnableHiRes();
	
	GUI_SetColor(GUI_RED);
	GUI_AA_FillPolygon(Param.aPoints, countof(_aNeedle), MAG * 400, MAG * 210);
	GUI_AA_FillPolygon(Param.aPoints1, countof(_aNeedle1), MAG * 400, MAG * 210);
	GUI_AA_FillPolygon(Param.aPoints2, countof(_aNeedle2), MAG * 400, MAG * 210);
}

/*
*********************************************************************************************************
*	函 数 名: Clock_Update
*	功能说明: 更新时间
*	形    参: pMsg  消息指针变量
*	返 回 值: 无
*********************************************************************************************************
*/
static void Clock_Update(WM_MESSAGE * pMsg) 
{
	char buf[30];
	WM_HWIN hWin = pMsg->hWin;

	HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStruct,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStruct,RTC_FORMAT_BIN);
	HAL_RTC_GetAlarm(&RTC_Handler,&RTC_AlarmStruct,RTC_ALARM_A,RTC_FORMAT_BIN);

	/* 更新时间 */
	sprintf(buf, 
	          "时间: %0.2d:%0.2d:%0.2d ", 
			  RTC_TimeStruct.Hours, 
			  RTC_TimeStruct.Minutes,
			  RTC_TimeStruct.Seconds);
	TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT1), buf);
 
	/* 更新日期 */
	sprintf(buf, 
	      "日期: 20%0.2d/%0.2d/%0.2d", 
		  RTC_DateStruct.Year, 
		  RTC_DateStruct.Month, 
		  RTC_DateStruct.Date);
	TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT0), buf); 
	
	/* 更新星期 */
	sprintf(buf, "星期: %s", ucWeekDay[RTC_DateStruct.WeekDay-1]);
	TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT2), buf);
	
	/* 更新闹钟 */
	sprintf(buf, 
	      "闹钟: %0.2d:%0.2d:%0.2d ", 
		  RTC_AlarmStruct.AlarmTime.Hours, 
		  RTC_AlarmStruct.AlarmTime.Minutes, 
		  RTC_AlarmStruct.AlarmTime.Seconds);
	TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT3), buf); 
}

/*
*********************************************************************************************************
*	               对话框控件列表
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateRTC[] = {
	{ WINDOW_CreateIndirect,  "Window",    ID_WINDOW_0,     0,   0,  480, 272, 0, 0x0, 0 },
	{ TEXT_CreateIndirect,      "日期",    GUI_ID_TEXT0,     62,   34, 200, 20, 0,0},
    { TEXT_CreateIndirect,      "时间",    GUI_ID_TEXT1,    272,   34, 200, 20, 0,0},
	{ TEXT_CreateIndirect,      "星期",    GUI_ID_TEXT2,     62,   55, 200, 20, 0,0},
    { TEXT_CreateIndirect,      "闹钟",    GUI_ID_TEXT3,    272,   55, 200, 20, 0,0},

	{ BUTTON_CreateIndirect, "DlgBack",    ID_BUTTON_0,  380,  0, 100, 100, 0, 0, 0 },	
	{ BUTTON_CreateIndirect, "TimeSet",    ID_BUTTON_1,    0,  0, 100, 100, 0, 0, 0 },
};

/*
*********************************************************************************************************
*	函 数 名: InitDialogRTC
*	功能说明: 对话框回调函数的初始化消息
*	形    参: pMsg   消息指针变量
*	返 回 值: 无
*********************************************************************************************************
*/
void InitDialogRTC(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
	WM_HWIN hItem;
	
	WM_CreateTimer(hWin, 0, 10, 0);						
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
	WM_SetHasTrans(hItem);
	WM_SetCallback(hItem, _cbButtonBack);
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
	WM_SetHasTrans(hItem);
	WM_SetCallback(hItem, _cbButtonList);
	
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT0),&GUI_FontHZ16);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT1),&GUI_FontHZ16);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT2),&GUI_FontHZ16);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT3),&GUI_FontHZ16);
	Clock_Update(pMsg);
	
	/* 使能抗锯齿 */
	GUI_AA_EnableHiRes();
	
	/* 设置抗锯齿因数 */
	GUI_AA_SetFactor(MAG);
}

/*
*********************************************************************************************************
*	函 数 名: _cbCallbackRTC
*	功能说明: 回调函数
*	形    参: pMsg   消息指针变量
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbCallbackRTC(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
 	GUI_RECT rRTC={310, 120, 490, 300};
	
    switch (pMsg->MsgId) 
    {
		case WM_INIT_DIALOG:
            InitDialogRTC(pMsg);
            break;
				
		case WM_PRE_PAINT:
			GUI_MULTIBUF_Begin();
			break;
		
		case WM_PAINT:
			Clock_DrawDisp();
            break;
		
		case WM_POST_PAINT:
			GUI_MULTIBUF_End();
			break;	
		
		/* 定时1秒更新一次时间 */
		case WM_TIMER:
			WM_InvalidateRect(hWin, &rRTC);
			Clock_Update(pMsg);
		    WM_RestartTimer(pMsg->Data.v, 1000);
	        break;
			
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (Id) 
            {
				/* 关闭对话框 */
				case ID_BUTTON_0:
                    switch(NCode)
                    {
						case WM_NOTIFICATION_RELEASED:
							GUI_AA_DisableHiRes();
                            GUI_EndDialog(hWin, 0);
                            break;
                    }
                    break;
					
				/* 设置时间，日期和闹钟 */				
				case ID_BUTTON_1:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
//                            App_CalendarChild(hWin);
                            break;
                    }
                    break;
            }
            break;
			
        default:
            WM_DefaultProc(pMsg);
    }
}

/*
*********************************************************************************************************
*	函 数 名: App_Calendar
*	功能说明: 万年历对话框
*	形    参: hWin 所创建对话框父窗口 	
*	返 回 值: 无
*********************************************************************************************************
*/
void App_Calendar(WM_HWIN hWin) 
{
	GUI_CreateDialogBox(_aDialogCreateRTC, 
	                    GUI_COUNTOF(_aDialogCreateRTC), 
	                    &_cbCallbackRTC, 
	                    hWin, 
	                    0, 
	                    0);
}
