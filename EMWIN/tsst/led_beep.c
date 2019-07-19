/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.44                          *
*        Compiled Nov 10 2017, 08:53:57                              *
*        (c) 2017 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END

#include "DIALOG.h"
#include "buttonbmp.h"
#include "main.h"
#include "FRAMEWIN.h"
#include "led_beep.h"
#include "led.h"
#include "EmWinHZFont.h"
#include "document.h"
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#define ID_FRAMEWIN_0            (GUI_ID_USER + 0x00)
#define ID_TEXT_0            (GUI_ID_USER + 0x01)
#define ID_TEXT_1            (GUI_ID_USER + 0x02)
#define ID_TEXT_2            (GUI_ID_USER + 0x03)
#define ID_BUTTON_0            (GUI_ID_USER + 0x04)
#define ID_BUTTON_1            (GUI_ID_USER + 0x05)
#define ID_BUTTON_2            (GUI_ID_USER + 0x06)

GUI_BITMAP buttonbmp_tab[2];

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "LED&BEEP", ID_FRAMEWIN_0, 0, 0, 480, 272, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "LED1", ID_TEXT_0, 85, 50, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "LED2", ID_TEXT_1, 85, 122, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "BEEP", ID_TEXT_2, 85, 189, 80, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED1", ID_BUTTON_0, 177, 33, 110, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED2", ID_BUTTON_1, 177, 106, 110, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "BEEP", ID_BUTTON_2, 177, 173, 110, 40, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};


/*********************************************************************
*
*       _cbDialog
*/
static void _LEDcbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
	static unsigned char led1flag=0;
	static unsigned char led2flag=0;
	static unsigned char beepflag=0;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Framewin'
    //
  
    hItem = pMsg->hWin;
    FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,2); //添加关闭按钮
	FRAMEWIN_AddMaxButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //添加最大化按钮
	FRAMEWIN_AddMinButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //添加最小化按钮
    FRAMEWIN_SetTitleHeight(hItem,20);
	//初始化BUTTON0
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
	BUTTON_SetBitmapEx(hItem,0,led1flag?&buttonbmp_tab[1]:&buttonbmp_tab[0],0,0);
	BUTTON_SetText(hItem, "");
	
	//初始化BUTTON1
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
	BUTTON_SetBitmapEx(hItem,0,led2flag?&buttonbmp_tab[1]:&buttonbmp_tab[0],0,0);
	BUTTON_SetText(hItem, "");
  
  //初始化BUTTON3
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
	BUTTON_SetBitmapEx(hItem,0,beepflag?&buttonbmp_tab[1]:&buttonbmp_tab[0],0,0);
	BUTTON_SetText(hItem, "");
    //
    // Initialization of 'LED1'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);
    //
    // Initialization of 'LED2'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);
    //
    // Initialization of 'BEEP'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
    TEXT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'Button'
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
		  led1flag=~led1flag;
		  led1flag?LED0_ON:LED0_OFF;
		  BUTTON_SetBitmapEx(hItem,0,led1flag?&buttonbmp_tab[1]:&buttonbmp_tab[0],0,0);
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'Button'
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
		led2flag=~led2flag;
		led2flag?LED1_ON:LED1_OFF;
		BUTTON_SetBitmapEx(hItem,0,led2flag?&buttonbmp_tab[1]:&buttonbmp_tab[0],0,0);
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_2: // Notifications sent by 'Button'
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
		beepflag=~beepflag;
	    beepflag?BEEP_ON:BEEP_OFF;

		BUTTON_SetBitmapEx(hItem,0,beepflag?&buttonbmp_tab[1]:&buttonbmp_tab[0],0,0);
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       CreateLED
*/
void CreateFramewin(void) {
	buttonbmp_tab[0]=bmBUTTONOFF;
	buttonbmp_tab[1]=bmBUTTONON;
    GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _LEDcbDialog, WM_HBKWIN, 0, 0);
	while(1)
  {
	GUI_Delay(10);
  }
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
