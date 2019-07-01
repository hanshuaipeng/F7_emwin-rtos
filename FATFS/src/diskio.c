/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "sdmmc.h"
#include "w25q256.h"
#include "usbh_diskio_dma.h" 
#include "FreeRTOS.h"
#include "task.h"
typedef __IO uint8_t  vu8;
/* Definitions of physical drive number for each drive */

#define SD_CARD 	0 //SD 卡,卷标为 0
#define EX_FLASH 	1 //外部 spi flash,卷标为 1
#define EX_NAND 	2 //外部 nand flash,卷标为 2
#define USB_HOST 	3 //usb,卷标为 3
//对于W25Q256
//前25M字节给fatfs用,25M字节后,用于存放字库,字库占用6.01M.	剩余部分,给客户自己用	 
#define FLASH_SECTOR_SIZE 	512	
#define FLASH_SECTOR_COUNT 	1024*25*2	//W25Q256,前25M字节给FATFS占用	
#define FLASH_BLOCK_SIZE   	8     		//每个BLOCK有8个扇区		
  
 
/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	stat = RES_OK;
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	int result;

	switch (pdrv) {
	case SD_CARD :
		MX_SDMMC1_SD_Init();
		result = 0;
		// translate the reslut code here

		break;

	case EX_FLASH :
		W25QXX_Init();

		// translate the reslut code here
		result = 0;
		break;

	case EX_NAND :
//		result = USB_disk_initialize();

		// translate the reslut code here

		break;
	case USB_HOST :
		result=USBH_initialize();

		// translate the reslut code here
		break;
	default :
		result=1;
		break;
	}
	if(result)
		return STA_NOINIT;
	else
		return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	uint8_t result;
	if (!count)return RES_PARERR;//count 不能等于 0，否则返回参数错误
	switch (pdrv) {
	case SD_CARD :
		// translate the arguments here
		result = SD_ReadDisk(buff,sector,count);
		// translate the reslut code here

		break;

	case EX_FLASH :
		// translate the arguments here
		for(;count>0;count--)
		{
			W25QXX_Read(buff,sector*512,512);
			sector++;
			buff+=512;
		}
		result = 0;
		// translate the reslut code here

		break;

	case EX_NAND :
		// translate the arguments here

//		result = USB_disk_read(buff, sector, count);

		// translate the reslut code here

		break;
	case USB_HOST :
		result=USBH_read(buff,sector,count);
		break;
	default :result=1;break;
	}
	if(result==1)
		return RES_PARERR;
	else
		return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	uint8_t result;
	switch (pdrv) {
	case SD_CARD :
		// translate the arguments here

		result = SD_WriteDisk((uint8_t*)buff,sector,count);
		while(result)//写出错
		{
			MX_SDMMC1_SD_Init();	//重新初始化SD卡
			result=SD_WriteDisk((uint8_t*)buff,sector,count);	
			//printf("sd wr error:%d\r\n",res);
		}
		// translate the reslut code here

		break;

	case EX_FLASH :
		// translate the arguments here
		for(;count>0;count--)
		{
			W25QXX_Write((uint8_t*)buff,sector*512,512);
			sector++;
			buff+=512;
		}
		result=0;

		// translate the reslut code here

		break;

	case EX_NAND :
		// translate the arguments here

//		res = USB_disk_write(buff, sector, count);

		// translate the reslut code here

		break;
	case USB_HOST :
		result = USBH_write(buff, sector, count);
		break;
	default : 
		result=1;
		break;
	}

	if(result==1)
		return RES_PARERR;
	else
		return RES_OK;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	switch (pdrv) {
	case SD_CARD :
		switch(cmd)
	    {
		    case CTRL_SYNC:
				res = RES_OK; 
		        break;	 
		    case GET_SECTOR_SIZE:
				*(DWORD*)buff = 512; 
		        res = RES_OK;
		        break;	 
		    case GET_BLOCK_SIZE:
				*(WORD*)buff = SDCardInfo.BlockSize;
		        res = RES_OK;
		        break;	 
		    case GET_SECTOR_COUNT:
		        *(DWORD*)buff = (SDCardInfo.BlockNbr*SDCardInfo.BlockSize)/512;
		        res = RES_OK;
		        break;
		    default:
		        res = RES_PARERR;
		        break;
	    }
		// Process of the command for the RAM drive
	case EX_FLASH :
		 switch(cmd)
	    {
		    case CTRL_SYNC:
				res = RES_OK; 
		        break;	 
		    case GET_SECTOR_SIZE:
		        *(WORD*)buff = FLASH_SECTOR_SIZE;
		        res = RES_OK;
		        break;	 
		    case GET_BLOCK_SIZE:
		        *(WORD*)buff = FLASH_BLOCK_SIZE;
		        res = RES_OK;
		        break;	 
		    case GET_SECTOR_COUNT:
		        *(DWORD*)buff = FLASH_SECTOR_COUNT;
		        res = RES_OK;
		        break;
		    default:
		        res = RES_PARERR;
		        break;
	    }
		// Process of the command for the MMC/SD card

	case EX_NAND :

		// Process of the command the USB drive

		break;
	case USB_HOST :
		res =USBH_ioctl(cmd,buff);
		break;
	}
	return res;
}
//获得时间
//User defined function to give a current time to fatfs module */
//31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */
//15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */
DWORD get_fattime (void)
{
	return 0;
}
vu8 cnt0=0;
vu8 cnt1=0;
//进入临界区
void ff_enter(FATFS *fs)
{
//	OS_ERR err; 
//	CPU_SR_ALLOC();
	if(fs->pdrv!=2)
	{
		 taskENTER_CRITICAL();           //进入临界区
		cnt0++;
	}else
	{
		vTaskSuspendAll();         // 开启调度锁 
	}
}
//退出临界区
void ff_leave(FATFS *fs)
{
	if(cnt0)
	{
		cnt0--;
		 taskEXIT_CRITICAL();            //退出临界区
	}
	if(cnt1)
	{
		cnt1--;
		xTaskResumeAll();	        //调度器解锁	
	}
}



