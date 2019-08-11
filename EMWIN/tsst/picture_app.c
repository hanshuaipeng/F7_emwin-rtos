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
#define ID_WINDOW_0             (GUI_ID_USER + 0x01)
#define ID_BUTTON_0             (GUI_ID_USER + 0x02)
#define ID_BUTTON_1             (GUI_ID_USER + 0x03)
#define ID_BUTTON_2             (GUI_ID_USER + 0x04)
#define ID_TEXT_0            	(GUI_ID_USER + 0x05)

#define MAG          3
#define GUI_STCOLOR_LIGHTBLUE   0x00DCA939
#define GUI_STCOLOR_DARKBLUE    0x00522000

WM_HWIN Picture_Window;
char *Picture_Path;
uint32_t *picoffsettbl;	//音乐offset索引表  

u8 *pname;			//带路径的文件名
u16 totwavnum; 		//音乐文件总数
u16 curindex;		//当前索引	
extern GUI_CONST_STORAGE GUI_BITMAP bmright;
extern GUI_CONST_STORAGE GUI_BITMAP bmreturn;
extern GUI_CONST_STORAGE GUI_BITMAP bmleft;
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
//得到path路径下,目标文件的总个数
//path:路径		    
//返回值:总有效文件数
u16 picture_get_tnum(u8 *path)
{	  
	u8 res;
	u16 rval=0;
 	DIR tdir;	 		//临时目录
	FILINFO* tfileinfo;	//临时文件信息	 	
	tfileinfo=(FILINFO*)mymalloc(SRAMIN,sizeof(FILINFO));//申请内存
    res=f_opendir(&tdir,(const TCHAR*)path); //打开目录 
	if(res==FR_OK&&tfileinfo)
	{
		while(1)//查询总的有效文件数
		{
	        res=f_readdir(&tdir,tfileinfo);       			//读取目录下的一个文件
	        if(res!=FR_OK||tfileinfo->fname[0]==0)break;	//错误了/到末尾了,退出	 		 
			res=f_typetell((u8*)tfileinfo->fname);	
			if((res&0XF0)==0X50)//取高四位,看看是不是图片文件	
			{
				rval++;//有效文件数增加1
			}	    
		}  
	}  
	myfree(SRAMIN,tfileinfo);//释放内存
	return rval;
}
void get_picture(void)
{
	u8 res;
 	DIR wavdir;	 		//目录
	FILINFO *picfileinfo;//文件信息 
	u32 temp;  
	while(f_opendir(&wavdir,"0:PICTURE"))//打开音乐文件夹
 	{	    
			  
	} 									  
	totwavnum=picture_get_tnum("0:PICTURE"); //得到总有效文件数
  	while(totwavnum==NULL)//音乐文件总数为0		
 	{	    
			  
	}										   
	picfileinfo=(FILINFO*)mymalloc(SRAMIN,sizeof(FILINFO));	//申请内存
 	picoffsettbl=mymalloc(SRAMIN,4*totwavnum);				//申请4*totwavnum个字节的内存,用于存放音乐文件off block索引
 	while(!picfileinfo||!picoffsettbl)//内存分配出错
 	{	    
					
	} 	 
 	//记录索引
    res=f_opendir(&wavdir,"0:PICTURE"); //打开目录
	if(res==FR_OK)
	{
		curindex=0;//当前索引为0
		while(1)//全部查询一遍
		{						
			temp=wavdir.dptr;								//记录当前index 
	        res=f_readdir(&wavdir,picfileinfo);       		//读取目录下的一个文件
	        if(res!=FR_OK||picfileinfo->fname[0]==0)break;	//错误了/到末尾了,退出 		 
			res=f_typetell((u8*)picfileinfo->fname);	
			if((res&0XF0)==0X50)//取高四位,看看是不是音乐文件	
			{
				picoffsettbl[curindex]=temp;//记录索引
				curindex++;
			}	    
		} 
	}
	curindex=0;
	myfree(SRAMIN,picfileinfo);			//释放内存
}

//播放音乐
void show_picture(void)
{
	u8 res;
 	DIR wavdir;	 		//目录
	FILINFO *picfileinfo;//文件信息 	
	pname=mymalloc(SRAMIN,FF_MAX_LFN*2+1);					//为带路径的文件名分配内存	
   	picfileinfo=(FILINFO*)mymalloc(SRAMIN,sizeof(FILINFO));	//申请内存
	res=f_opendir(&wavdir,(const TCHAR*)"0:PICTURE"); 	//打开目录
	if(res==FR_OK)//打开成功
	{	
		dir_sdi(&wavdir,picoffsettbl[curindex]);				//改变当前目录索引	   
        res=f_readdir(&wavdir,picfileinfo);       				//读取目录下的一个文件
//        if(res!=FR_OK||picfileinfo->fname[0]==0)break;			//错误了/到末尾了,退出	
		if (picfileinfo->fattrib & AM_ARC)
		{
			res=f_typetell((u8*)picfileinfo->fname);
			strcpy((char*)pname,"0:PICTURE/");						//复制路径(目录)
			strcat((char*)pname,(const char*)picfileinfo->fname);	//将文件名接在后面
			switch(res)//"BMP","JPG","JPEG","GIF"
			{
				case 0x50:
					dispbmpex(pname,1,0,0,1,1);
					break;
				case 0x51:
				case 0x52:
					displayjpegex(pname,1,0,0,1,1);
					break;
				case 0x53:
					
					break;
				case 0x54:
					displaypngex((char*)pname,1,0,0);
					break;
			}
 		}
	} 											  
	myfree(SRAMIN,picfileinfo);			//释放内存			    
	myfree(SRAMIN,pname);				//释放内存			    	 
} 

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _picDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 480, 272, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 10, 105, 50, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_1, 418, 107, 50, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_2, 422, 227, 50, 50, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 181, 9, 96, 20, 0, 0x64, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
static void _cbButtonleft(WM_MESSAGE * pMsg) 
{
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:				
			GUI_DrawBitmap(&bmleft, 9, 9);
			break;
			
		default:
			BUTTON_Callback(pMsg);
	}
}
static void _cbButtonright(WM_MESSAGE * pMsg) 
{
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
			GUI_DrawBitmap(&bmright, 9, 9);
			break;
			
		default:
			BUTTON_Callback(pMsg);
	}
}
static void _cbButtonreturn(WM_MESSAGE * pMsg) 
{
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
			GUI_DrawBitmap(&bmreturn, 0, 9);
			break;
			
		default:
			BUTTON_Callback(pMsg);
	}
}
/*********************************************************************
*
*       _cbDialog
*/long hide_time=0;
static void _cbpicDialog(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
	int     NCode;
	int     Id;
	char text_buf[10];
	
	switch (pMsg->MsgId) 
	{
		case WM_DELETE:
			myfree(SRAMIN,picoffsettbl);		//释放内存
			
			break;
		case WM_INIT_DIALOG:
			get_picture();
			hide_time=WM_CreateTimer(pMsg->hWin,0,2000,0);
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
			BUTTON_SetFont(hItem,&GUI_FontHZ16);
			BUTTON_SetText(hItem, "上一张");
			WM_SetHasTrans(hItem);
			WM_SetCallback(hItem, _cbButtonleft);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
			BUTTON_SetFont(hItem,&GUI_FontHZ16);
			BUTTON_SetText(hItem, "下一张");
			WM_SetHasTrans(hItem);
			WM_SetCallback(hItem, _cbButtonright);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
			BUTTON_SetFont(hItem,&GUI_FontHZ16);
			BUTTON_SetText(hItem, "返回");
			WM_SetHasTrans(hItem);
			WM_SetCallback(hItem, _cbButtonreturn);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
			TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			TEXT_SetBkColor(hItem,GUI_RED);
			sprintf(text_buf,"%d/%d",curindex+1,totwavnum);
			TEXT_SetText(hItem, text_buf);
			break;
		case WM_TIMER:
			WM_HideWindow(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0));
			WM_HideWindow(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1));
			WM_HideWindow(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2));
			break;
		case WM_TOUCH:
			WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0));
			WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1));
			WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2));
			WM_RestartTimer(hide_time,2000);
			break;
		case WM_PAINT:
			GUI_SetBkColor(GUI_BLACK);
            GUI_Clear(); 
			show_picture();
			break;
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id) 
			{
				case ID_BUTTON_1: // Notifications sent by 'Button'
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							
							break;
						case WM_NOTIFICATION_RELEASED:
							curindex++;
							if(curindex>totwavnum-1)
								curindex=0;
							WM_RestartTimer(hide_time,2000);
							WM_InvalidateWindow(pMsg -> hWin);
							break;
					}
					break;	
				case ID_BUTTON_0: // Notifications sent by 'Button'
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:

							break;
						case WM_NOTIFICATION_RELEASED:
							if(curindex==0)
							{
								curindex=totwavnum-1;
							}
							else
								curindex--;
							WM_RestartTimer(hide_time,2000);
							WM_InvalidateWindow(pMsg -> hWin);
							break;
					}
					break;	
				case ID_BUTTON_2: // Notifications sent by 'Button'
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							
							break;
						case WM_NOTIFICATION_RELEASED:
							GUI_EndDialog(pMsg->hWin, 0);
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
*       CreatePICTURE
*/

void CreatePICTURE(void) 
{
	GUI_CreateDialogBox(_picDialogCreate, GUI_COUNTOF(_picDialogCreate), _cbpicDialog, WM_HBKWIN, 0, 0);
//  Picture_Window = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbPictureDialog, WM_HBKWIN, 0, 0);
}


