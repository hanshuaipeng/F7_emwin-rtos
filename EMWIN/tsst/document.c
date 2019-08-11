#include "document.h"
#include "ff.h"
#include "diskio.h"
#include "string.h"
#include "GUI.h"
#include "DIALOG.h"
#include "WM.h"
#include "LISTVIEW.h"
#include "MESSAGEBOX.h"
#include "Hzfontupdata.h"
#include "EmWinHZFont.h"
#include "Freertos.h"
#include "task.h"
#include "malloc.h"
File_Dat *File_Data;						//文件浏览

FRESULT scan_files (char* path,LISTVIEW_Handle handle)
{
    FRESULT res;
    DIR dir;
    static FILINFO fno;
	char numbuf[5];
	char file_size[10];
	uint16_t size_h,size_l;
	uint8_t month,day,hour,min;
	uint16_t year;
	char file_time[20];
	
	LISTVIEW_DeleteAllRows(handle);

	File_Data->Document_Count=0;
	File_Data->File_Count=0;
	File_Data->ALL_File_Count=0;
	vTaskSuspendAll();         // 开启调度锁 
    res = f_opendir(&dir, path);                       /* Open the directory */
	if (res == FR_OK) 
	{
        while(1)
		{
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) 
			{   
				File_Data[File_Data->ALL_File_Count].File_Type=mymalloc(SRAMIN,100);
				File_Data[File_Data->ALL_File_Count].File_Name=mymalloc(SRAMIN,100);
				memset(File_Data[File_Data->ALL_File_Count].File_Type,0,100);
				memset(File_Data[File_Data->ALL_File_Count].File_Name,0,100);
				memcpy(File_Data[File_Data->ALL_File_Count].File_Type,"文件夹",strlen("文件夹"));
				memcpy(File_Data[File_Data->ALL_File_Count].File_Name,fno.fname,strlen(fno.fname));
//				File_Data[File_Data->ALL_File_Count].File_Name=fno.fname;		
//				File_Data[File_Data->ALL_File_Count].File_Type="文件夹";
				
				LISTVIEW_AddRow(handle, NULL);
				memset(numbuf,0,sizeof(numbuf));
				
				sprintf(numbuf,"%d.",File_Data->ALL_File_Count);
				
				LISTVIEW_SetItemText(handle,0,File_Data->ALL_File_Count,numbuf);
				LISTVIEW_SetItemText(handle,1,File_Data->ALL_File_Count,File_Data[File_Data->ALL_File_Count].File_Name);
				LISTVIEW_SetItemText(handle,2,File_Data->ALL_File_Count,File_Data[File_Data->ALL_File_Count].File_Type);
				File_Data->Document_Count++;
				
				myfree(SRAMIN,File_Data[File_Data->ALL_File_Count].File_Type);
				myfree(SRAMIN,File_Data[File_Data->ALL_File_Count].File_Name);
            }
			else
			{
				if(strstr(fno.fname,"."))
				{
					File_Data[File_Data->ALL_File_Count].File_Type=mymalloc(SRAMIN,100);
					File_Data[File_Data->ALL_File_Count].File_Name=mymalloc(SRAMIN,100);
					memset(File_Data[File_Data->ALL_File_Count].File_Type,0,100);
					memset(File_Data[File_Data->ALL_File_Count].File_Name,0,100);
					
					sprintf(File_Data[File_Data->ALL_File_Count].File_Type,"%s",strstr(fno.fname,".")+1);
					memcpy(File_Data[File_Data->ALL_File_Count].File_Name,fno.fname,strlen(fno.fname));
					
//					File_Data[File_Data->ALL_File_Count].File_Name=fno.fname;		
					File_Data[File_Data->ALL_File_Count].fsize=fno.fsize;	
					size_h=(File_Data[File_Data->ALL_File_Count].fsize)/1024;
					size_l=(File_Data[File_Data->ALL_File_Count].fsize)%1024;
					
					
					LISTVIEW_AddRow(handle, NULL);
					memset(numbuf,0,sizeof(numbuf));
					memset(file_size,0,sizeof(file_size));
					sprintf(numbuf,"%d.",File_Data->ALL_File_Count);
					sprintf(file_size,"%d.%dKb",size_h,size_l);
					LISTVIEW_SetItemText(handle,0,File_Data->ALL_File_Count,numbuf);
					LISTVIEW_SetItemText(handle,1,File_Data->ALL_File_Count,File_Data[File_Data->ALL_File_Count].File_Name);
					LISTVIEW_SetItemText(handle,2,File_Data->ALL_File_Count,File_Data[File_Data->ALL_File_Count].File_Type);
					LISTVIEW_SetItemText(handle,3,File_Data->ALL_File_Count,file_size);
					File_Data->File_Count++;
					myfree(SRAMIN,File_Data[File_Data->ALL_File_Count].File_Type);
					myfree(SRAMIN,File_Data[File_Data->ALL_File_Count].File_Name);
				}
			}
			File_Data[File_Data->ALL_File_Count].fdate=fno.fdate;
			File_Data[File_Data->ALL_File_Count].ftime=fno.ftime;
			
			year=(File_Data[File_Data->ALL_File_Count].fdate>>9)+1980;
			month=(File_Data[File_Data->ALL_File_Count].fdate>>5)&0x000f;
			day=(File_Data[File_Data->ALL_File_Count].fdate)&0x001f;
			hour=File_Data[File_Data->ALL_File_Count].ftime>>11;
			min=(File_Data[File_Data->ALL_File_Count].ftime>>5)&0x001f;
			sprintf(file_time,"%d/%02d/%02d %02d:%02d",year,month,day,hour,min);
			LISTVIEW_SetItemText(handle,4,File_Data->ALL_File_Count,file_time);	
			File_Data->ALL_File_Count++;
        }
    }
	f_closedir(&dir);
	xTaskResumeAll();	        //调度器解锁
    return res;
}
void FileListView_Init(LISTVIEW_Handle hobj)
{
	HEADER_Handle Header;
	Header=LISTVIEW_GetHeader(hobj);
	HEADER_SetFont(Header,&GUI_FontHZ16);
	LISTVIEW_SetFont(hobj,&GUI_FontHZ16);
	LISTVIEW_SetAutoScrollH(hobj,1);           //启用自动水平滚动条
	LISTVIEW_SetAutoScrollV(hobj,1);           //启用自动垂直滚动条
	LISTVIEW_SetHeaderHeight(hobj,20);         //设置标题的高度
	LISTVIEW_SetRowHeight(hobj,20);            //设置LISTVIEW的行高
	LISTVIEW_AddColumn(hobj,30, "序", GUI_TA_LEFT | GUI_TA_VCENTER); 
	LISTVIEW_AddColumn(hobj,150, "名称", GUI_TA_LEFT | GUI_TA_VCENTER);
	LISTVIEW_AddColumn(hobj,70, "类型", GUI_TA_LEFT | GUI_TA_VCENTER);
	LISTVIEW_AddColumn(hobj,70, "大小", GUI_TA_LEFT | GUI_TA_VCENTER);
	LISTVIEW_AddColumn(hobj,150, "日期", GUI_TA_LEFT | GUI_TA_VCENTER);
	LISTVIEW_SetGridVis(hobj, 1);
	scan_files("0:",hobj);
}
uint8_t Path_Prosses(char* source)
{
	uint8_t len=0;
	char * res;
	len=strlen(source)-1;
	res=mymalloc(SRAMIN,len+1);
	memcpy(res,source,len+1);
	if(res[len]==':')
	{
		printf("根目录\r\n");
		return 0 ;
	}
	else if(res[len]=='/'&&res[len-1]==':')
	{
		printf("根目录\r\n");
		return 0 ;
	}
	memset(source,0,len+1);
	while(res[len]!='/')
	{
		len--;
	}
	memcpy(source,res,len);
	myfree(SRAMIN,res);
	printf("%s\r\n",source);
	return 1;
}

