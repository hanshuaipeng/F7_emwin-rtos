#include "use_app.h"
#include "DIALOG.h"
#include "EmWinHZFont.h"
#include "Freertos.h"
#include "task.h"
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0            (GUI_ID_USER + 0x00)
#define ID_MULTIEDIT0            (GUI_ID_USER + 0x01)

//TOUCH任务
//设置任务优先级
#define CPU_TASK_PRIO			5
//任务堆栈大小
#define CPU_STK_SIZE			512
//任务句柄
TaskHandle_t CPUTask_Handler;
//touch任务
void cpu_task(void *pvParameters);

MULTIEDIT_HANDLE Show_Handle;
// USER START (Optionally insert additional defines)
// USER END

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
static const GUI_WIDGET_CREATE_INFO _useappDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 480, 272, 0, 0x0, 0 },
  { MULTIEDIT_CreateIndirect,  NULL,    ID_MULTIEDIT0,  0, 0,470, 248,0,0x0,0},
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

void Cpu_TaskCreat(void)
{
	taskENTER_CRITICAL();           //进入临界区
	//创建触摸任务
    xTaskCreate((TaskFunction_t )cpu_task,             
                (const char*    )"cpu_task",           
                (uint16_t       )CPU_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )CPU_TASK_PRIO,        
                (TaskHandle_t*  )&CPUTask_Handler);  
	taskEXIT_CRITICAL();            //退出临界区
}

void cpu_task(void *pvParameters)
{
	char CPU_RunInfo[400];
	char Header_Buff[500];
	while(1)
	{
		MULTIEDIT_SetText(Show_Handle,0);
		memset(CPU_RunInfo,0,400); //信息缓冲区清零

		vTaskList((char *)&CPU_RunInfo); //获取任务运行时间信息

//		printf("---------------------------------------------------------\r\n");
//		printf("任务名      任务状态      优先级      剩余栈     任务序号\r\n");
//		printf("%s", CPU_RunInfo);
//		printf("---------------------------------------------------------\r\n");
//		MULTIEDIT_AddText(Show_Handle,CPU_RunInfo);
//		printf("---------------------------------------------------------\r\n");
		MULTIEDIT_AddText(Show_Handle,"---------------------------------------------------------\r\n");
		MULTIEDIT_AddText(Show_Handle,"任务名      任务状态      优先级      剩余栈     任务序号\r\n");
		MULTIEDIT_AddText(Show_Handle,"---------------------------------------------------------\r\n");
		MULTIEDIT_AddText(Show_Handle,CPU_RunInfo);
		MULTIEDIT_AddText(Show_Handle,"---------------------------------------------------------\r\n");
		memset(CPU_RunInfo,0,400); //信息缓冲区清零

		vTaskGetRunTimeStats((char *)&CPU_RunInfo);

//		printf("任务名            运行计数            使用率\r\n");
//		printf("%s", CPU_RunInfo);
//		printf("-------------------------------------------------------\r\n\n");
		MULTIEDIT_AddText(Show_Handle,"任务名            运行计数            使用率\r\n");
		MULTIEDIT_AddText(Show_Handle, CPU_RunInfo);
		MULTIEDIT_AddText(Show_Handle,"-------------------------------------------------------\r\n\n");
		vTaskDelay(1000);		//延时1000ms
	}
}
/*********************************************************************
*
*       _cbDialog
*/
static void _cbuseappDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
	  case WM_DELETE:
		   vTaskDelete(CPUTask_Handler); //删除开始任务
	  break;
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Framewin'
    //
    hItem = pMsg->hWin;
	FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,2); //添加关闭按钮
	FRAMEWIN_AddMaxButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //添加最大化按钮
	FRAMEWIN_AddMinButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //添加最小化按钮
	
	  
	FRAMEWIN_SetFont(hItem,&GUI_FontHZ16);
	FRAMEWIN_SetTextColor(hItem, GUI_BLACK);
	FRAMEWIN_SetTitleHeight(hItem,20);
	FRAMEWIN_SetText(hItem, "使用率");
    //
    // Initialization of 'Listview'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT0);
	MULTIEDIT_SetFont(hItem, &GUI_FontHZ12);
	MULTIEDIT_SetAutoScrollH(hItem,1);
    MULTIEDIT_SetAutoScrollV(hItem,1);
	MULTIEDIT_SetAutoScrollH(hItem,1);
    MULTIEDIT_SetAutoScrollV(hItem,1);
	MULTIEDIT_SetFocusable(hItem,0);
	Show_Handle=hItem;
	Cpu_TaskCreat();
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
	case WM_PAINT:
		GUI_Clear();
		GUI_SetBkColor(GUI_BLACK);
		break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_MULTIEDIT0: // Notifications sent by 'Listview'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
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
*       CreateFramewin
*/

void CreateUseAppwin(void) 
{
  GUI_CreateDialogBox(_useappDialogCreate, GUI_COUNTOF(_useappDialogCreate), _cbuseappDialog, WM_HBKWIN, 0, 0);
}


