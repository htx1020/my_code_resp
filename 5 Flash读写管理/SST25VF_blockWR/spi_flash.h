#ifndef __SPI_FLASH_H__
#define __SPI_FLASH_H__

#include "sys.h"
////////////////////////////////////////////////////////////////////////////////// 
//???
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg1		0x05 
#define W25X_ReadStatusReg2		0x35 
#define W25X_ReadStatusReg3		0x15 
#define W25X_WriteStatusReg1    0x01 
#define W25X_WriteStatusReg2    0x31 
#define W25X_WriteStatusReg3    0x11 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 
#define W25X_Enable4ByteAddr    0xB7
#define W25X_Exit4ByteAddr      0xE9

void W25QXX_Init(void);
u32 W25QXX_ReadID(void);
//u16  W25QXX_ReadID(void);  	    		//??FLASH ID
u8 W25QXX_ReadSR(u8 regno);             //??????? 
void W25QXX_Wait_Busy(void);           	//????
void W25QXX_Write_Enable(void);  		//??? 
void W25QXX_Write_Disable(void);		//???

//void W25QXX_4ByteAddr_Enable(void);     //??4??????
//void W25QXX_Write_SR(u8 regno,u8 sr);   //??????

static void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);

//void W25QXX_Erase_Chip(void);    	  	//????
void W25QXX_Erase_Sector(u32 Dst_Addr);	//????

//void W25QXX_PowerDown(void);        	//??????
//void W25QXX_WAKEUP(void);				//??

void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //??flash
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);

#endif
