#include "updateapp.h"
#include "FreeRTOS.h"
#include "task.h"
#include "warndialog.h"
#include "Hzfontupdata.h"
#include "EmWinHZFont.h"
extern GUI_CONST_STORAGE GUI_BITMAP bmfontupdate;
extern GUI_CONST_STORAGE GUI_BITMAP bmsystemupdate;


static const GUI_WIDGET_CREATE_INFO updateappDialogCreate[] = {
    { FRAMEWIN_CreateIndirect,  "Caption",           0,                       0,  0,  480,272,FRAMEWIN_CF_MOVEABLE,0},
    { BUTTON_CreateIndirect,    "BUTTON0",           GUI_ID_BUTTON0,          60, 54, 100,100,0,0},
    { BUTTON_CreateIndirect,    "BUTTON1",           GUI_ID_BUTTON1,          298,54, 100,100,0,0},
    { TEXT_CreateIndirect,      "TEXT0",             GUI_ID_TEXT0,            60, 160,100,25, 0,0},
    { TEXT_CreateIndirect,      "TEXT1",             GUI_ID_TEXT1,            298,160,100,25, 0,0}
};



/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/

void PaintUpdateappDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;

}

static void _cbButtonFont(WM_MESSAGE * pMsg) 
{
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
			GUI_DrawBitmap(&bmfontupdate, 0, 0);
			break;
			
		default:
			BUTTON_Callback(pMsg);
	}
}

static void _cbButtonSys(WM_MESSAGE * pMsg) 
{
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
			GUI_DrawBitmap(&bmsystemupdate, 0, 0);
			break;
			
		default:
			BUTTON_Callback(pMsg);
	}
}

/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/

void InitUpdateappDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    //
    //FRAMEWIN
    //
    FRAMEWIN_SetTextAlign(hWin,GUI_TA_VCENTER|GUI_TA_CENTER);
    FRAMEWIN_AddCloseButton(hWin, FRAMEWIN_BUTTON_RIGHT, 0);
    FRAMEWIN_AddMaxButton(hWin, FRAMEWIN_BUTTON_RIGHT, 1);
    FRAMEWIN_AddMinButton(hWin, FRAMEWIN_BUTTON_RIGHT, 2);
    FRAMEWIN_SetTitleHeight(hWin,20);
	FRAMEWIN_SetFont(hWin,&GUI_FontHZ16);
	FRAMEWIN_SetText(hWin, "更新");
    //
    //GUI_ID_TEXT0
    //
    TEXT_SetTextAlign(WM_GetDialogItem(hWin,GUI_ID_TEXT0),GUI_TA_VCENTER|GUI_TA_CENTER);
	TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT0),&GUI_FontHZ16);
	TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT0),"字库更新");
    //
    //GUI_ID_TEXT1
    //
    TEXT_SetTextAlign(WM_GetDialogItem(hWin,GUI_ID_TEXT1),GUI_TA_VCENTER|GUI_TA_CENTER);
	TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT1),&GUI_FontHZ16);
	TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT1),"程序更新");
	
	BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON0),&GUI_FontHZ16);
	BUTTON_SetText(WM_GetDialogItem(hWin,GUI_ID_BUTTON0), "字库更新");
	WM_SetHasTrans(WM_GetDialogItem(hWin,GUI_ID_BUTTON0));
	WM_SetCallback(WM_GetDialogItem(hWin,GUI_ID_BUTTON0), _cbButtonFont);
	
	BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON1),&GUI_FontHZ16);
	BUTTON_SetText(WM_GetDialogItem(hWin,GUI_ID_BUTTON1), "程序");
	WM_SetHasTrans(WM_GetDialogItem(hWin,GUI_ID_BUTTON1));
	WM_SetCallback(WM_GetDialogItem(hWin,GUI_ID_BUTTON1), _cbButtonSys);
}




/*********************************************************************
*
*       Dialog callback routine
*/
static void updateappCallback(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            PaintUpdateappDialog(pMsg);
            break;
        case WM_INIT_DIALOG:
            InitUpdateappDialog(pMsg);
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
                case GUI_ID_BUTTON0:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
							if(warnningTask("更新字库",pMsg->hWin))
								HzFontupdataCreate(pMsg->hWin);    //创建字库升级
                            break;
                    }
                    break;
                case GUI_ID_BUTTON1:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
							if(warnningTask("更新程序",pMsg->hWin))
								 
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
void updateappTask(GUI_HWIN hwin) 
{ 
	GUI_CreateDialogBox(updateappDialogCreate, GUI_COUNTOF(updateappDialogCreate), &updateappCallback, hwin, 0, 0);
}

