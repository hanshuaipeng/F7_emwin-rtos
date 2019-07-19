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



//EventsFunctionList
void OnButtonReleased(WM_MESSAGE * pMsg);
void OnDropDownSelChanged(WM_MESSAGE * pMsg);
void OnMultiEditSelChanged(WM_MESSAGE * pMsg);
//EndofEventsFunctionList


/*********************************************************************
*
*       static data
*
**********************************************************************
*/



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
    { BUTTON_CreateIndirect,    "OPEN",              GUI_ID_BUTTON1,          309,17, 54, 23, 0,0},
    { BUTTON_CreateIndirect,    "SEND",              GUI_ID_BUTTON2,          380,16, 67, 24, 0,0}
};



/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/

void PaintDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
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
	FRAMEWIN_SetTextColor(hWin, GUI_RED);
	FRAMEWIN_SetText(hWin, "串口调试");
	
	BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON1), &GUI_FontHZ12);
	BUTTON_SetText(WM_GetDialogItem(hWin,GUI_ID_BUTTON1),"打开");
	BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON2), &GUI_FontHZ12);
	BUTTON_SetText(WM_GetDialogItem(hWin,GUI_ID_BUTTON2),"发送");
    //
    //GUI_ID_MULTIEDIT0
    //
	MULTIEDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_MULTIEDIT0), &GUI_FontHZ24);
	MULTIEDIT_SetAutoScrollH(WM_GetDialogItem(hWin,GUI_ID_MULTIEDIT0),1);
    MULTIEDIT_SetAutoScrollV(WM_GetDialogItem(hWin,GUI_ID_MULTIEDIT0),1);
	MULTIEDIT_SetReadOnly(WM_GetDialogItem(hWin,GUI_ID_MULTIEDIT0),1);
    //
    //GUI_ID_MULTIEDIT1
    //
	MULTIEDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_MULTIEDIT1), &GUI_FontHZ24);
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

}




/*********************************************************************
*
*       Dialog callback routine
*/
static void _comCallback(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
		case WM_DELETE:
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
//                            OnMultiEditSelChanged(pMsg);
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
//                            OnButtonReleased(pMsg);
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
    GUI_CreateDialogBox(_comDialogCreate, GUI_COUNTOF(_comDialogCreate), &_comCallback, 0, 0, 0);
}

