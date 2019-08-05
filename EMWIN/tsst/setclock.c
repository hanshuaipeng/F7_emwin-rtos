#include "setclock.h"

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
#include "CALENDAR.h"
#include "EmWinHZFont.h"
#include "clockapp_usepic.h"

/*
*********************************************************************************************************
*                                        宏定义
*********************************************************************************************************
*/
#define ID_SPINBOX_0  (GUI_ID_USER + 0x01)
#define ID_SPINBOX_1  (GUI_ID_USER + 0x02)
#define ID_SPINBOX_2  (GUI_ID_USER + 0x03)
#define ID_SPINBOX_3  (GUI_ID_USER + 0x04)
#define ID_SPINBOX_4  (GUI_ID_USER + 0x05)
#define ID_SPINBOX_5  (GUI_ID_USER + 0x06)
#define ID_CALENDAR0  (GUI_ID_USER + 0x07)

/*
*********************************************************************************************************
*                                        变量
*********************************************************************************************************
*/
const char *apDays[] =
{
	"六",
	"日",
	"一",
	"二",
	"三",
	"四",
	"五",
	"六",
};

const char *apMonths[]=
{
	"1月",
	"2月",
	"3月",
	"4月",
	"5月",
	"6月",
	"7月",
	"8月",
	"9月",
	"10月",
	"11月",
	"12月",
};


/*
*********************************************************************************************************
*	                               对话框控件列表
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateSetTimeAlarm[] = {
    { WINDOW_CreateIndirect,  "Time Settings",        	0,                     0,  0,  480,272,0,0},
    { TEXT_CreateIndirect,      "时间设置",          GUI_ID_TEXT0,            68, 14, 73, 20, 0,0},
    { TEXT_CreateIndirect,      "闹钟设置",          GUI_ID_TEXT1,            188,14, 73, 20, 0,0},
    { CHECKBOX_CreateIndirect,  "保存时间设置",  	 GUI_ID_CHECK0,           48,162,210,24, 0,0},
	{ CHECKBOX_CreateIndirect,  "保存闹钟设置",   	 GUI_ID_CHECK1,           168,162,210,24, 0,0},
	{ CHECKBOX_CreateIndirect,  "保存年月日设置",    GUI_ID_CHECK2,           300,162,210,24, 0,0},
    { TEXT_CreateIndirect,      "时:",               GUI_ID_TEXT2,            24, 24, 29, 20, 0,0},
    { TEXT_CreateIndirect,      "分:",               GUI_ID_TEXT3,            24, 63+15,29, 20, 0,0},
    { TEXT_CreateIndirect,      "秒:",               GUI_ID_TEXT4,            24, 93+30,29, 20, 0,0},
	
	{ BUTTON_CreateIndirect, "Button", GUI_ID_BUTTON0, 120, 24, 100, 45, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Button", GUI_ID_BUTTON1, 300, 24, 100, 45, 0, 0x0, 0 },

	{ SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_0, 48, 33, 130, 35, 0, 0x0, 0 },
	{ SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_1, 48, 62 + 15, 130, 35, 0, 0x0, 0 },
	{ SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_2, 48, 90 + 30, 130, 35, 0, 0x0, 0 },
	{ SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_3, 168, 33, 130, 35, 0, 0x0, 0 },
	{ SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_4, 168, 63 + 15, 130, 35, 0, 0x0, 0 },
	{ SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_5, 168, 93 + 30, 130, 35, 0, 0x0, 0 },
};
/*
*********************************************************************************************************
*	函 数 名: bsp_CalcWeek
*	功能说明: 根据日期计算星期几
*	形    参: _year _mon _day  年月日  (年是2字节整数，月和日是字节整数）
*	返 回 值: 周几 （1-7） 7表示周日
*********************************************************************************************************
*/
uint8_t RTC_CalcWeek(uint16_t _year, uint8_t _mon, uint8_t _day)
{
	/*
	蔡勒（Zeller）公式
		历史上的某一天是星期几？未来的某一天是星期几？关于这个问题，有很多计算公式（两个通用计算公式和
	一些分段计算公式），其中最著名的是蔡勒（Zeller）公式。
	    即w=y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1

		公式中的符号含义如下，
	     w：星期；
	     c：年的高2位，即世纪-1
	     y：年（两位数）；
	     m：月（m大于等于3，小于等于14，即在蔡勒公式中，某年的1、2月要看作上一年的13、14月来计算，
	  	    比如2003年1月1日要看作2002年的13月1日来计算）；
	     d：日；
	     [ ]代表取整，即只要整数部分。

	    算出来的W除以7，余数是几就是星期几。如果余数是0，则为星期日。
        如果结果是负数，负数求余数则需要特殊处理：
            负数不能按习惯的余数的概念求余数，只能按数论中的余数的定义求余。为了方便
        计算，我们可以给它加上一个7的整数倍，使它变为一个正数，然后再求余数

		以2049年10月1日（100周年国庆）为例，用蔡勒（Zeller）公式进行计算，过程如下：
		蔡勒（Zeller）公式：w=y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1
		=49+[49/4]+[20/4]-2×20+[26× (10+1)/10]+1-1
		=49+[12.25]+5-40+[28.6]
		=49+12+5-40+28
		=54 (除以7余5)
		即2049年10月1日（100周年国庆）是星期5。
	*/
	uint8_t y, c, m, d;
	int16_t w;

	if (_mon >= 3)
	{
		m = _mon;
		y = _year % 100;
		c = _year / 100;
		d = _day;
	}
	else	/* 某年的1、2月要看作上一年的13、14月来计算 */
	{
		m = _mon + 12;
		y = (_year - 1) % 100;
		c = (_year - 1) / 100;
		d = _day;
	}

	w = y + y / 4 +  c / 4 - 2 * c + ((uint16_t)26*(m+1))/10 + d - 1;
	if (w == 0)
	{
		w = 7;	/* 表示周日 */
	}
	else if (w < 0)	/* 如果w是负数，则计算余数方式不同 */
	{
		w = 7 - (-w) % 7;
	}
	else
	{
		w = w % 7;
	}
	
	/* modified by eric2013 -- 2016-12-25 */
	if (w == 0)
	{
		w = 7;	/* 表示周日 */
	}
	
	return w;
}
/*
*********************************************************************************************************
*	函 数 名: InitDialogSetTimeAlarm
*	功能说明: 回调函数初始化消息 
*	形    参: pMsg  消息指针变量
*	返 回 值: 无
*********************************************************************************************************
*/
void InitDialogSetTimeAlarm(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
	
	/* 日历控件初始化 */
	CALENDAR_SetDefaultSize(CALENDAR_SI_HEADER, 35 );
	CALENDAR_SetDefaultSize(CALENDAR_SI_CELL_X, 40 );
	CALENDAR_SetDefaultSize(CALENDAR_SI_CELL_Y, 28 );

	CALENDAR_SetDefaultFont(CALENDAR_FI_CONTENT, &GUI_FontHZ16);
	CALENDAR_SetDefaultFont(CALENDAR_FI_HEADER, &GUI_FontHZ16); 
	CALENDAR_SetDefaultBkColor(CALENDAR_CI_WEEKDAY, 0xFF00FF);
	
	CALENDAR_SetDefaultDays(apDays);
	CALENDAR_SetDefaultMonths(apMonths);

	HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStruct,RTC_FORMAT_BIN);
	CALENDAR_Create(hWin, 
	                 270, 
	                 22, 
	                 RTC_DateStruct.Year+2000, 
	                 RTC_DateStruct.Month, 
					 RTC_DateStruct.Date,
					 2,
					 ID_CALENDAR0, 
					 WM_CF_SHOW);

	/* 文本控件初始化 */
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT0), &GUI_FontHZ16);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT1), &GUI_FontHZ16);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT2), &GUI_FontHZ16);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT3), &GUI_FontHZ16);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT4), &GUI_FontHZ16);

	/* 复选框初始化 */
    CHECKBOX_SetText(WM_GetDialogItem(hWin,GUI_ID_CHECK0), "保存时间设置");
    CHECKBOX_SetFont(WM_GetDialogItem(hWin,GUI_ID_CHECK0), &GUI_FontHZ16);
    CHECKBOX_SetText(WM_GetDialogItem(hWin,GUI_ID_CHECK1), "保存闹钟设置");
    CHECKBOX_SetFont(WM_GetDialogItem(hWin,GUI_ID_CHECK1), &GUI_FontHZ16);
    CHECKBOX_SetText(WM_GetDialogItem(hWin,GUI_ID_CHECK2), "保存年月日设置");
    CHECKBOX_SetFont(WM_GetDialogItem(hWin,GUI_ID_CHECK2), &GUI_FontHZ16);
	
	/* 按钮初始化 */
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON0), &GUI_FontHZ16);
    BUTTON_SetText(WM_GetDialogItem(hWin,GUI_ID_BUTTON0), "确定");
	BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON1), &GUI_FontHZ16);
    BUTTON_SetText(WM_GetDialogItem(hWin,GUI_ID_BUTTON1), "取消");
	
	/* 编辑框初始化 */
	EDIT_SetTextAlign(SPINBOX_GetEditHandle(WM_GetDialogItem(hWin,ID_SPINBOX_0)), GUI_TA_HCENTER | GUI_TA_VCENTER);
	EDIT_SetTextAlign(SPINBOX_GetEditHandle(WM_GetDialogItem(hWin,ID_SPINBOX_1)), GUI_TA_HCENTER | GUI_TA_VCENTER);
	EDIT_SetTextAlign(SPINBOX_GetEditHandle(WM_GetDialogItem(hWin,ID_SPINBOX_2)), GUI_TA_HCENTER | GUI_TA_VCENTER);
	EDIT_SetTextAlign(SPINBOX_GetEditHandle(WM_GetDialogItem(hWin,ID_SPINBOX_3)), GUI_TA_HCENTER | GUI_TA_VCENTER);
	EDIT_SetTextAlign(SPINBOX_GetEditHandle(WM_GetDialogItem(hWin,ID_SPINBOX_4)), GUI_TA_HCENTER | GUI_TA_VCENTER);
	EDIT_SetTextAlign(SPINBOX_GetEditHandle(WM_GetDialogItem(hWin,ID_SPINBOX_5)), GUI_TA_HCENTER | GUI_TA_VCENTER);

	/* SPINBOX初始化 */
	SPINBOX_SetEdge(WM_GetDialogItem(hWin,ID_SPINBOX_0), SPINBOX_EDGE_CENTER);
	SPINBOX_SetEdge(WM_GetDialogItem(hWin,ID_SPINBOX_1), SPINBOX_EDGE_CENTER);
	SPINBOX_SetEdge(WM_GetDialogItem(hWin,ID_SPINBOX_2), SPINBOX_EDGE_CENTER);
	SPINBOX_SetEdge(WM_GetDialogItem(hWin,ID_SPINBOX_3), SPINBOX_EDGE_CENTER);
	SPINBOX_SetEdge(WM_GetDialogItem(hWin,ID_SPINBOX_4), SPINBOX_EDGE_CENTER);
	SPINBOX_SetEdge(WM_GetDialogItem(hWin,ID_SPINBOX_5), SPINBOX_EDGE_CENTER);
	
	SPINBOX_SetFont(WM_GetDialogItem(hWin,ID_SPINBOX_0), &GUI_FontHZ16);
	SPINBOX_SetFont(WM_GetDialogItem(hWin,ID_SPINBOX_1), &GUI_FontHZ16);
	SPINBOX_SetFont(WM_GetDialogItem(hWin,ID_SPINBOX_2), &GUI_FontHZ16);
	SPINBOX_SetFont(WM_GetDialogItem(hWin,ID_SPINBOX_3), &GUI_FontHZ16);
	SPINBOX_SetFont(WM_GetDialogItem(hWin,ID_SPINBOX_4), &GUI_FontHZ16);
	SPINBOX_SetFont(WM_GetDialogItem(hWin,ID_SPINBOX_5), &GUI_FontHZ16);
	
	SPINBOX_SetRange(WM_GetDialogItem(hWin,ID_SPINBOX_0), 0, 23);  /* 小时 */
	SPINBOX_SetRange(WM_GetDialogItem(hWin,ID_SPINBOX_1), 0, 59);  /* 分钟 */
	SPINBOX_SetRange(WM_GetDialogItem(hWin,ID_SPINBOX_2), 0, 59);  /* 秒钟 */
	
	SPINBOX_SetRange(WM_GetDialogItem(hWin,ID_SPINBOX_3), 0, 23);  /* 闹钟小时 */
	SPINBOX_SetRange(WM_GetDialogItem(hWin,ID_SPINBOX_4), 0, 59);  /* 闹钟分钟 */
	SPINBOX_SetRange(WM_GetDialogItem(hWin,ID_SPINBOX_5), 0, 59);  /* 闹钟秒钟 */
	
	HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStruct,RTC_FORMAT_BIN);
	HAL_RTC_GetAlarm(&RTC_Handler,&RTC_AlarmStruct,RTC_ALARM_A,RTC_FORMAT_BIN);
	
	SPINBOX_SetValue(WM_GetDialogItem(hWin,ID_SPINBOX_0), RTC_TimeStruct.Hours);   /* 时 */								   
	SPINBOX_SetValue(WM_GetDialogItem(hWin,ID_SPINBOX_1), RTC_TimeStruct.Minutes); /* 分 */							   
	SPINBOX_SetValue(WM_GetDialogItem(hWin,ID_SPINBOX_2), RTC_TimeStruct.Seconds); /* 秒 */
	
	SPINBOX_SetValue(WM_GetDialogItem(hWin,ID_SPINBOX_3), RTC_AlarmStruct.AlarmTime.Hours);   /* 闹钟时 */			 
	SPINBOX_SetValue(WM_GetDialogItem(hWin,ID_SPINBOX_4), RTC_AlarmStruct.AlarmTime.Minutes); /* 闹钟分 */				 
	SPINBOX_SetValue(WM_GetDialogItem(hWin,ID_SPINBOX_5), RTC_AlarmStruct.AlarmTime.Seconds); /* 闹钟秒 */
}

/*
*********************************************************************************************************
*	函 数 名: _cbCallbackSetTimeAlarm
*	功能说明: 回调函数 
*	形    参: pMsg  消息指针变量
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbCallbackSetTimeAlarm(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
	CALENDAR_DATE CALENDAR_Date;
	
    switch (pMsg->MsgId) 
    {
        case WM_INIT_DIALOG:
            InitDialogSetTimeAlarm(pMsg);
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
				/* 点击确定 */
				case GUI_ID_BUTTON0:
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						
						case WM_NOTIFICATION_RELEASED:
							
							/* 是否保存时间设置 */
							if(CHECKBOX_GetState(WM_GetDialogItem(hWin,GUI_ID_CHECK0)) == 1)
							{
								RTC_TimeStruct.TimeFormat     = RTC_HOURFORMAT12_AM;
								RTC_TimeStruct.Hours   = SPINBOX_GetValue(WM_GetDialogItem(hWin,ID_SPINBOX_0));
								RTC_TimeStruct.Minutes = SPINBOX_GetValue(WM_GetDialogItem(hWin,ID_SPINBOX_1));
								RTC_TimeStruct.Seconds = SPINBOX_GetValue(WM_GetDialogItem(hWin,ID_SPINBOX_2));
								RTC_Set_Time(RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds,RTC_HOURFORMAT12_PM);
							}
							
							/* 是否保存闹钟时间设置 */
							if(CHECKBOX_GetState(WM_GetDialogItem(hWin,GUI_ID_CHECK1)) == 1)
							{
								/* 禁能 Alarm A */
//								RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
//								RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_AM;
//								RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = SPINBOX_GetValue(WM_GetDialogItem(hWin,ID_SPINBOX_3));
//								RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = SPINBOX_GetValue(WM_GetDialogItem(hWin,ID_SPINBOX_4));
//								RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = SPINBOX_GetValue(WM_GetDialogItem(hWin,ID_SPINBOX_5));
//				
//								/* 设置 Alarm A */
//								RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31;
//								RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
//								RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;

//								/* 配置 RTC Alarm A 寄存器 */
//								/* 根据需要，这里可以加上是否配置成功的状态监测 */
//								RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);

//								/* 使能 RTC Alarm A 中断 */
//								RTC_ITConfig(RTC_IT_ALRA, ENABLE);

//								/* 使能 alarm  A */
//								RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
							}
							
							/* 是否保存年月日设置 */
							if(CHECKBOX_GetState(WM_GetDialogItem(hWin,GUI_ID_CHECK2)) == 1)
							{
								CALENDAR_GetSel(WM_GetDialogItem(hWin,GUI_ID_CALENDAR0), &CALENDAR_Date);
								
								RTC_DateStruct.Year = CALENDAR_Date.Year%100;
								RTC_DateStruct.Month = CALENDAR_Date.Month;
								RTC_DateStruct.Date = CALENDAR_Date.Day;
								RTC_DateStruct.WeekDay = RTC_CalcWeek(RTC_DateStruct.Year, 
																			 RTC_DateStruct.Month,
																			 RTC_DateStruct.Date);
								RTC_Set_Date(RTC_DateStruct.Year, 
											 RTC_DateStruct.Month,
											 RTC_DateStruct.Date,
											 RTC_DateStruct.WeekDay);

								//CALENDAR_SetDate(WM_GetDialogItem(hWin,GUI_ID_CALENDAR0), &CALENDAR_Date);
							}
							
						    GUI_EndDialog(hWin, 0);
							break;
					}
					break;
				
				/* 点击取消按钮 */
				case GUI_ID_BUTTON1:
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						
						case WM_NOTIFICATION_RELEASED:
							GUI_EndDialog(hWin, 0);
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
*	函 数 名: App_CalendarChild
*	功能说明: 闹钟，时间，日期设置对话框 
*	形    参: hWin 父窗口句柄
*	返 回 值: 无
*********************************************************************************************************
*/
void App_CalendarChild(WM_HWIN hWin) 
{
	WM_HWIN hWinAlarm; 
    
	hWinAlarm = GUI_CreateDialogBox(_aDialogCreateSetTimeAlarm, 
	                                GUI_COUNTOF(_aDialogCreateSetTimeAlarm), 
	                                &_cbCallbackSetTimeAlarm, 
	                                hWin, 
	                                0, 
	                                0);
	
	/* 设置为模态窗口 */
	WM_MakeModal(hWinAlarm);
}



