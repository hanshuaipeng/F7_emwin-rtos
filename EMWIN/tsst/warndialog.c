#include "warndialog.h"
#include "GUI.h"
#include "DIALOG.h"
#include "EmWinHZFont.h"
#include "mysys.h"
TEXT_Handle WarnText_Handle;

static const GUI_WIDGET_CREATE_INFO _awarnningDialogCreate[] = {
    { FRAMEWIN_CreateIndirect,  "Caption",           0,                       0,  0,  200,100,FRAMEWIN_CF_MOVEABLE,0},
    { BUTTON_CreateIndirect,    "BUTTON0",           GUI_ID_BUTTON0,          23, 43, 53, 21, 0,0},
    { BUTTON_CreateIndirect,    "BUTTON1",           GUI_ID_BUTTON1,          112,43, 53, 21, 0,0}
};



/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/





/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/

void warnningInitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    //
    //FRAMEWIN
    //
	FRAMEWIN_SetTitleHeight(hWin,20);
	FRAMEWIN_SetTextAlign(hWin, GUI_TA_HCENTER | GUI_TA_VCENTER);
	FRAMEWIN_SetFont(hWin,&GUI_FontHZ16);
	FRAMEWIN_SetTextColor(hWin, GUI_BLACK);
	FRAMEWIN_SetText(hWin, "提示");
    //
    //GUI_ID_TEXT0
    //
   
	
	BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON0), &GUI_FontHZ16);
    BUTTON_SetText(WM_GetDialogItem(hWin,GUI_ID_BUTTON0), "确定");
	BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON1), &GUI_FontHZ16);
    BUTTON_SetText(WM_GetDialogItem(hWin,GUI_ID_BUTTON1), "取消");
}




/*********************************************************************
*
*       Dialog callback routine
*/
static void _cbwarnningCallback(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            
            break;
        case WM_INIT_DIALOG:
            warnningInitDialog(pMsg);
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
                            GUI_EndDialog(hWin, 1);
                            break;
                    }
                    break;
                case GUI_ID_BUTTON1:
                    switch(NCode)
                    {
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


/*********************************************************************
*
*       MainTask
*
**********************************************************************
*/
uint8_t warnningTask(char *buf) 
{ 
	GUI_HWIN hwin;
	uint8_t r=0;
	hwin=GUI_CreateDialogBox(_awarnningDialogCreate, GUI_COUNTOF(_awarnningDialogCreate), &_cbwarnningCallback, WM_HBKWIN, 140, 86);
	WarnText_Handle=TEXT_CreateEx(49, 30, 101,20,hwin, WM_CF_SHOW, 0, GUI_ID_TEXT0, NULL);
	TEXT_SetTextAlign(WarnText_Handle,GUI_TA_VCENTER|GUI_TA_CENTER);
	TEXT_SetFont(WarnText_Handle,&GUI_FontHZ16);
	TEXT_SetText(WarnText_Handle, buf);
	r = GUI_ExecCreatedDialog(hwin);
	return r;
}


