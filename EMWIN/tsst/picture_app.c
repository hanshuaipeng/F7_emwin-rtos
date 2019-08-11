#include "picture_app.h"

#include "pngdisplay.h"
#include "jpegdisplay.h"
#include "gifdisplay.h"
#include "bmpdisplay.h"
#include "EmWinHZFont.h"
#include "DIALOG.h"
#include "document.h"
#include "diskio.h"
#include "ff.h"
//#define ID_WINDOW_0             (GUI_ID_USER + 0x01)
//#define ID_BUTTON_0             (GUI_ID_USER + 0x02)
//#define ID_BUTTON_1             (GUI_ID_USER + 0x03)
//#define ID_BUTTON_2             (GUI_ID_USER + 0x04)
//#define ID_TEXT_0            	(GUI_ID_USER + 0x05)
//uint32_t *wavoffsettbl;	//图片offset索引表
//uint16_t curindex;			//当前索引
//	
//  WM_HWIN Picture_Window;
//  char *Picture_Path;
///*********************************************************************
//*
//*       Static data
//*
//**********************************************************************
//*/

//uint16_t Find_files (char* path,uint8_t type,uint8_t mode)
//{
//    FRESULT res;
//    DIR dir;
//    UINT i;
//	static uint16_t totwavnum; 		//图片文件总数
//    static FILINFO fno;


//    res = f_opendir(&dir, path);                       /* Open the directory */
//    if (res == FR_OK) 
//	{
//        for (;;) 
//		{
//            res = f_readdir(&dir, &fno);                   /* Read a directory item */
//            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
//            if (fno.fattrib & AM_DIR) 
//			{                    /* It is a directory */
//                i = strlen(path);
//                sprintf(&path[i], "/%s", fno.fname);
//                res = Find_files(path,type,mode);                    /* Enter the directory */
//                if (res != FR_OK) break;
//                path[i] = 0;
//            } else 
//			{                                       /* It is a file. */
//                res=f_typetell((uint8_t*)fno.fname);
//				if((res&0XF0)==type)//取高四位,看看是不是图片文件	
//				{
//					if(mode==0)
//						totwavnum++;
//					else
//					{
//						wavoffsettbl[curindex++]=dir.dptr;
//					}
//				}
//				printf("%s/%s\n", path, fno.fname);
//            }
//        }
////        f_closedir(&dir);
//    }
//	if(mode==0)
//		wavoffsettbl=mymalloc(SRAMIN,totwavnum);
//    return totwavnum;
//}


///*********************************************************************
//*
//*       _aDialogCreate
//*/
//static const GUI_WIDGET_CREATE_INFO _picDialogCreate[] = {
//  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 480, 272, 0, 0x0, 0 },
//  { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 10, 105, 57, 33, 0, 0x0, 0 },
//  { BUTTON_CreateIndirect, "Button", ID_BUTTON_1, 418, 107, 50, 35, 0, 0x0, 0 },
//  { BUTTON_CreateIndirect, "Button", ID_BUTTON_2, 422, 227, 49, 35, 0, 0x0, 0 },
//  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 181, 9, 96, 20, 0, 0x64, 0 },
//  // USER START (Optionally insert additional widgets)
//  // USER END
//};

///*********************************************************************
//*
//*       Static code
//*
//**********************************************************************
//*/
//extern char Now_Path[300];
//// USER START (Optionally insert additional static code)
//// USER END

///*********************************************************************
//*
//*       _cbDialog
//*/
//static void _cbpicDialog(WM_MESSAGE * pMsg) 
//{
//	WM_HWIN hItem;
//	int     NCode;
//	int     Id;
//	DIR wavdir;	 		//目录
//	FRESULT res;
//	FILINFO wavfileinfo;//文件信息
//	switch (pMsg->MsgId) 
//	{
//		case WM_INIT_DIALOG:
//			Find_files("0:PICTURE",0x50,0);
//			Find_files("0:PICTURE",0x50,1);
//			res=f_opendir(&wavdir,(const TCHAR*)"0:PICTURE"); 	//打开目录
//			curindex=0;
//			if(res==FR_OK)
//			{
//				dir_sdi(&wavdir,wavoffsettbl[curindex]);
//				res=f_readdir(&wavdir,&wavfileinfo); 
//				printf("文件=%s\r\n",wavfileinfo.fname);				
//			}
//			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
//			BUTTON_SetFont(hItem,&GUI_FontHZ16);
//			BUTTON_SetText(hItem, "上一张");

//			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
//			BUTTON_SetFont(hItem,&GUI_FontHZ16);
//			BUTTON_SetText(hItem, "下一张");
//		
//			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
//			BUTTON_SetFont(hItem,&GUI_FontHZ16);
//			BUTTON_SetText(hItem, "返回");
//		
//			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
//			TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
//			
//			TEXT_SetText(hItem, "title:1/20");
//			break;
//		case WM_PAINT:
//			
//			break;
//		case WM_NOTIFY_PARENT:
//			Id    = WM_GetId(pMsg->hWinSrc);
//			NCode = pMsg->Data.v;
//			switch(Id) 
//			{
//				case ID_BUTTON_0: // Notifications sent by 'Button'
//					switch(NCode) 
//					{
//						case WM_NOTIFICATION_CLICKED:
//							
//							break;
//						case WM_NOTIFICATION_RELEASED:

//							break;
//					}
//					break;	
//				case ID_BUTTON_1: // Notifications sent by 'Button'
//					switch(NCode) 
//					{
//						case WM_NOTIFICATION_CLICKED:

//							break;
//						case WM_NOTIFICATION_RELEASED:

//							break;
//					}
//					break;	
//				case ID_BUTTON_2: // Notifications sent by 'Button'
//					switch(NCode) 
//					{
//						case WM_NOTIFICATION_CLICKED:
//							
//							break;
//						case WM_NOTIFICATION_RELEASED:
//							GUI_EndDialog(pMsg->hWin, 0);
//							break;
//					}
//					break;	
//			}
//		break;
//		default:
//			WM_DefaultProc(pMsg);
//			break;
//	}
//}
///*********************************************************************
//*
//*       CreatePICTURE
//*/

//void CreatePICTURE(void) 
//{
//	GUI_CreateDialogBox(_picDialogCreate, GUI_COUNTOF(_picDialogCreate), _cbpicDialog, WM_HBKWIN, 0, 0);
////  Picture_Window = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbPictureDialog, WM_HBKWIN, 0, 0);
//}


