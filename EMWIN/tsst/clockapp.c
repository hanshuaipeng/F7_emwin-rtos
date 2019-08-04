#include "clockapp.h"
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

#define ID_FRAMEWIN_0            (GUI_ID_USER + 0x00)
#define ID_SPINBOX_0            (GUI_ID_USER + 0x01)
#define ID_SPINBOX_1            (GUI_ID_USER + 0x02)
#define ID_SPINBOX_2            (GUI_ID_USER + 0x03)


static const GUI_WIDGET_CREATE_INFO _ClockAPPCreate[] = {
  { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 480, 272, 0, 0x0, 0 },
  { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_0, 27, 34, 80, 20, 0, 0x0, 0 },
  { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_1, 142, 43, 80, 20, 0, 0x0, 0 },
  { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_2, 255, 46, 80, 20, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       _cbDialog
*/
static void _cbClockAPPDialog(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
	int     NCode;
	int     Id;
	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
			hItem = pMsg->hWin;
			FRAMEWIN_SetTitleHeight(hItem, 20);
			FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT, 0);
			FRAMEWIN_AddMaxButton(hItem, FRAMEWIN_BUTTON_RIGHT, 1);
			FRAMEWIN_AddMinButton(hItem, FRAMEWIN_BUTTON_RIGHT, 2);
			FRAMEWIN_SetFont(hItem,&GUI_FontHZ16);
			FRAMEWIN_SetTextColor(hItem, GUI_RED);
			FRAMEWIN_SetText(hItem, "Ê±ÖÓÉèÖÃ");
			break;
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id) 
			{
				case ID_SPINBOX_0: // Notifications sent by 'Spinbox'
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:

							break;
						case WM_NOTIFICATION_RELEASED:
						// USER START (Optionally insert code for reacting on notification message)
						// USER END
							break;
						case WM_NOTIFICATION_MOVED_OUT:
						// USER START (Optionally insert code for reacting on notification message)
						// USER END
							break;
						case WM_NOTIFICATION_VALUE_CHANGED:
						// USER START (Optionally insert code for reacting on notification message)
						// USER END
							break;
					}
					break;
				case ID_SPINBOX_1: // Notifications sent by 'Spinbox'
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
						// USER START (Optionally insert code for reacting on notification message)
						// USER END
							break;
						case WM_NOTIFICATION_RELEASED:
						// USER START (Optionally insert code for reacting on notification message)
						// USER END
							break;
						case WM_NOTIFICATION_MOVED_OUT:
							break;
						case WM_NOTIFICATION_VALUE_CHANGED:
							break;
					}
					break;
			case ID_SPINBOX_2: // Notifications sent by 'Spinbox'
				switch(NCode) 
				{
					case WM_NOTIFICATION_CLICKED:
					// USER START (Optionally insert code for reacting on notification message)
					// USER END
					break;
					case WM_NOTIFICATION_RELEASED:
					// USER START (Optionally insert code for reacting on notification message)
					// USER END
					break;
					case WM_NOTIFICATION_MOVED_OUT:
					// USER START (Optionally insert code for reacting on notification message)
					// USER END
					break;
					case WM_NOTIFICATION_VALUE_CHANGED:
					// USER START (Optionally insert code for reacting on notification message)
					// USER END
					break;
					// USER START (Optionally insert additional code for further notification handling)
					// USER END
				}
				break;
		}
		break;
		default:
			WM_DefaultProc(pMsg);
		break;
	}
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateFramewin
*/
void CreateClockAPP(void) 
{
	GUI_CreateDialogBox(_ClockAPPCreate, GUI_COUNTOF(_ClockAPPCreate), _cbClockAPPDialog, WM_HBKWIN, 0, 0);
}


