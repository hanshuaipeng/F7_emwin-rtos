#include "picture_app.h"

#include "pngdisplay.h"
#include "jpegdisplay.h"
#include "gifdisplay.h"
#include "bmpdisplay.h"

#include "DIALOG.h"
#include "document.h"

#define ID_WINDOW_0              (GUI_ID_USER + 0x00)
#define ID_BUTTON_0              (GUI_ID_USER + 0x01)
#define ID_BUTTON_1              (GUI_ID_USER + 0x02)
#define ID_BUTTON_2              (GUI_ID_USER + 0x03)


  WM_HWIN Picture_Window;
  char *Picture_Path;
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "PICTURE", ID_WINDOW_0, 0, 0, 480, 272, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "UP", ID_BUTTON_0, 75, 229, 50, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "DOWN", ID_BUTTON_1, 177, 228, 50, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "CANCEL", ID_BUTTON_2, 279, 226, 50, 40, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
extern char Now_Path[300];
// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbPictureDialog(WM_MESSAGE * pMsg) {
  int NCode;
  int Id;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
	  case WM_DELETE:
		  Path_Prosses(Now_Path);
		  break;
	  case WM_PAINT:
		GUI_Clear();
		GUI_SetBkColor(GUI_BLACK);
		  if(strstr(Now_Path,".bmp")||
			 strstr(Now_Path,".BMP")||
			 strstr(Now_Path,".Bmp"))
		  {
			dispbmp((u8*)Now_Path,0,0,	0,	1,1);
		  }
		  else if(strstr(Now_Path,".jpg")||
			    strstr(Now_Path,".Jpg")||
			    strstr(Now_Path,".JPG"))
		  {
			displyjpeg((u8*)Now_Path,0,0,	0,	1,1);
		  }
		  else if(strstr(Now_Path,".png")||
			    strstr(Now_Path,".Png")||
			    strstr(Now_Path,".PNG"))
		  {
			displaypng(Now_Path,1,0,0);
		  }
		  else if(strstr(Now_Path,".gif")||
			    strstr(Now_Path,".Gif")||
			    strstr(Now_Path,".GIF"))
		  {
			displaygifex(Now_Path,0,20,20,1,1);
		  }
		break;
  case WM_INIT_DIALOG:
	 
	 break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'UP'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'DOWN'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_2: // Notifications sent by 'CANCEL'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
		WM_DeleteWindow(Picture_Window);
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
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreatePICTURE
*/

void CreatePICTURE(void) 
{
  Picture_Window = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbPictureDialog, WM_HBKWIN, 0, 0);
}


