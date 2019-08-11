#include "filewindow.h"
#include "DIALOG.h"
#include "main.h"
#include "EmWinHZFont.h"
#include "document.h"
#include "jpegdisplay.h"
#include "bmpdisplay.h"
#include "picture_app.h"
#include "malloc.h"

#define ID_FRAMEWIN_0            (GUI_ID_USER + 0x00)
#define ID_BUTTON_0            (GUI_ID_USER + 0x03)
#define ID_BUTTON_1            (GUI_ID_USER + 0x04)
#define ID_PROGBAR_0            (GUI_ID_USER + 0x05)
#define ID_LISTVIEW_0            (GUI_ID_USER + 0x06)
#define ID_TEXT_0            (GUI_ID_USER + 0x07)
char Now_Path[300]="0:";
WM_HWIN DocumenthWin;
static const GUI_WIDGET_CREATE_INFO DocumentDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, 	"Framewin", 	ID_FRAMEWIN_0, 	0, 	0, 480, 272, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, 		"CANCEL", 		ID_BUTTON_1, 351, 212, 102, 32, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, 		"OK", 			ID_BUTTON_0, 15, 212, 96, 33, 0, 0x0, 0 },
  { PROGBAR_CreateIndirect, 	"Progbar", 		ID_PROGBAR_0, 169, 218, 114, 20, 0, 0x0, 0 },
  { LISTVIEW_CreateIndirect, 	"Listview", 	ID_LISTVIEW_0, 0, 16, 470, 190, 0, 0x0, 0 },
  { TEXT_CreateIndirect, 		"Text", 		ID_TEXT_0,		 0, 0, 480,20, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDocumentAppDialog(WM_MESSAGE * pMsg) {
	WM_HWIN hItem;
	int NCode;
	int Id;
	uint32_t DiskFree, DiskTotal;
	char buff[25];
	char list_data[100]="";
	int listviewitem=0;
	switch (pMsg->MsgId) 
	{
		case WM_DELETE:
			memset(Now_Path,0,sizeof(Now_Path));
			memcpy(Now_Path,"0:",2);
		case WM_PAINT:
		break;

		case WM_INIT_DIALOG:
			hItem = pMsg->hWin;
			FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,2); //添加关闭按钮
			FRAMEWIN_AddMaxButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //添加最大化按钮
			FRAMEWIN_AddMinButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //添加最小化按钮
			
			FRAMEWIN_SetTitleHeight(hItem,20);
			FRAMEWIN_SetFont(hItem,&GUI_FontHZ16);
			FRAMEWIN_SetTextColor(hItem, GUI_BLACK);
			FRAMEWIN_SetText(hItem, "文件管理");
				
			BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_0),&GUI_FontHZ16);
			BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_1),&GUI_FontHZ16);
			BUTTON_SetText(WM_GetDialogItem(hItem,ID_BUTTON_0),"确定");
			BUTTON_SetText(WM_GetDialogItem(hItem,ID_BUTTON_1),"返回");
		
			exf_getfree("0:", &DiskTotal, &DiskFree);
			sprintf(buff, "%dMB/%dMB", (DiskTotal - DiskFree)>>10, DiskTotal>>10);
			PROGBAR_SetMinMax(WM_GetDialogItem(hItem,ID_PROGBAR_0), 0, DiskTotal>>10);
			PROGBAR_SetText(WM_GetDialogItem(hItem,ID_PROGBAR_0), buff);
			PROGBAR_SetValue(WM_GetDialogItem(hItem,ID_PROGBAR_0), (DiskTotal - DiskFree)>>10);
			
			TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_0),&GUI_FontHZ16);
			TEXT_SetText(WM_GetDialogItem(hItem,ID_TEXT_0),"0:");		
			FileListView_Init(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0));
			
		break;
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id) 
			{
				case ID_LISTVIEW_0:
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
						break;
						case WM_NOTIFICATION_RELEASED:
//                            listviewitem=LISTVIEW_GetSel(WM_GetDialogItem(pMsg->hWin,ID_LISTVIEW_0));//获取选中的项目编号
//                            //返回指定单元格的文本
//                            LISTVIEW_GetItemText(WM_GetDialogItem(pMsg->hWin,ID_LISTVIEW_0),1,listviewitem,sdpath1,20);  //获取名字  
//							strcat(sdpath,sdpath1);
//                            TEXT_SetText(WM_GetDialogItem(pMsg->hWin,ID_TEXT_0),sdpath);
						break;
						case WM_NOTIFICATION_SEL_CHANGED:
							
						break;
				  }
				  break;
				case ID_BUTTON_0: // Notifications sent by 'OK'
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:

						break;
						case WM_NOTIFICATION_RELEASED:
							listviewitem=LISTVIEW_GetSel(WM_GetDialogItem(pMsg->hWin,ID_LISTVIEW_0));//获取选中的项目编号
							//返回指定单元格的文本
							strcat(Now_Path,"/");
							printf("%s\r\n",Now_Path);
							memset(list_data,0,sizeof(list_data));
							LISTVIEW_GetItemText(WM_GetDialogItem(pMsg->hWin,ID_LISTVIEW_0),1,listviewitem,list_data,30);  //获取名字  
							strcat(Now_Path,list_data);
							printf("%s\r\n",Now_Path);
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin,ID_TEXT_0),Now_Path);
						
							memset(list_data,0,sizeof(list_data));
							LISTVIEW_GetItemText(WM_GetDialogItem(pMsg->hWin,ID_LISTVIEW_0),2,listviewitem,list_data,30);  //获取类型 	
							if(strstr(list_data,"文件夹"))
							{
								scan_files(Now_Path,WM_GetDialogItem(pMsg->hWin,ID_LISTVIEW_0));
							}
							else
							{
								CreatePICTURE(0,pMsg->hWin);
							}
						break;
					}
				break;
				case ID_BUTTON_1: // Notifications sent by 'CANCEL'
					switch(NCode)
					{
						case WM_NOTIFICATION_CLICKED:

						break;
						case WM_NOTIFICATION_RELEASED:
							if(Path_Prosses(Now_Path))
							{
								TEXT_SetText(WM_GetDialogItem(pMsg->hWin,ID_TEXT_0),Now_Path);
								scan_files(Now_Path,WM_GetDialogItem(pMsg->hWin,ID_LISTVIEW_0));
							}
							else
								WM_DeleteWindow(DocumenthWin);
						break;
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
*       CreateDocuments
*/
void CreateDocuments(void) {

  DocumenthWin = GUI_CreateDialogBox(DocumentDialogCreate, GUI_COUNTOF(DocumentDialogCreate), _cbDocumentAppDialog, 0, 0, 0);
	
}

