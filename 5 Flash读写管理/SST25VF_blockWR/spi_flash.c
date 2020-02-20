#include "spi_flash.h"
#include <string.h>
extern int SPI_Comm(unsigned char *p_sent, unsigned char *p_recv, unsigned int length);
extern unsigned char SPI1_ReadWriteByte(unsigned char data);

#define	W25QXX_CS 		PCout(2)  		//W25QXX的片选信号

static unsigned char g_spi_inout_buff[256+5]; 
static unsigned char g_sector_buff[4096];	

u32 W25QXX_ReadID(void)
{
  u32 temp;

  W25QXX_CS=0;	
  SPI1_ReadWriteByte(0x9F);		 		         /* 发送读JEDEC ID命令(9Fh)	*/
        
  temp = (temp | SPI1_ReadWriteByte(0x00)) << 8;  /* 接收数据 */
  temp = (temp | SPI1_ReadWriteByte(0x00)) << 8;	
  temp = (temp | SPI1_ReadWriteByte(0x00)); 	     /* 在本例中,temp的值应为0xBF2541 */  
  W25QXX_CS=1;
	return temp;
}

u8 W25QXX_ReadSR(u8 regno)   
{  
	u8 command=0,offset = 0; 
  switch(regno)
  {
      case 1:
          command=W25X_ReadStatusReg1;    //??????1??
          break;
//      case 2:
//          command=W25X_ReadStatusReg2;    //??????2??
//          break;
//      case 3:
//          command=W25X_ReadStatusReg3;    //??????3??
//          break;
      default:
          command=W25X_ReadStatusReg1;    
          break;
  }    
  
  g_spi_inout_buff[offset++] = command;
  g_spi_inout_buff[offset++] = 0xFF;
  SPI_Comm(g_spi_inout_buff,g_spi_inout_buff,offset);
	return g_spi_inout_buff[offset-1];  
}

void W25QXX_Write_Enable(void)   
{                         
  unsigned int offset = 0,reg_value;
  unsigned char cmd = 0;
  offset = 0;
  g_spi_inout_buff[offset++] = 0x05;
  g_spi_inout_buff[offset++] = 0x00;
  SPI_Comm(g_spi_inout_buff,g_spi_inout_buff,offset);
  reg_value = g_spi_inout_buff[offset-1];
  
  cmd = 0x50;
  SPI_Comm(&cmd,&cmd,1);  

  offset = 0;
  g_spi_inout_buff[offset++] = 0x01;
  g_spi_inout_buff[offset++] = 0x00;
  SPI_Comm(g_spi_inout_buff,g_spi_inout_buff,offset);
  
  cmd = 0x06;
  SPI_Comm(&cmd,&cmd,1); 
} 

void W25QXX_Write_Disable(void)   
{ 
  unsigned char cmd = W25X_WriteDisable;
  SPI_Comm(&cmd,&cmd,1);  
}

void W25QXX_Erase_Sector(u32 Dst_Addr)   
{  
    unsigned int offset = 0;
   	Dst_Addr*=0x1000; //4K
    W25QXX_Write_Enable();                  //SET WEL 	 
    W25QXX_Wait_Busy();   
  
    memset(g_spi_inout_buff,0xff,sizeof(g_spi_inout_buff));
    g_spi_inout_buff[offset++] =  W25X_SectorErase; 
    g_spi_inout_buff[offset++] = (u8)((Dst_Addr)>>16);
    g_spi_inout_buff[offset++] = (u8)((Dst_Addr)>>8);
    g_spi_inout_buff[offset++] = (u8)Dst_Addr;
    SPI_Comm(g_spi_inout_buff,g_spi_inout_buff,offset); 
    W25QXX_Wait_Busy(); 
}

void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR(1)&0x01)==0x01);   // 
} 

void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{  
    unsigned int offset = 0;
    memset(g_spi_inout_buff,0xff,sizeof(g_spi_inout_buff));
    g_spi_inout_buff[offset++] = W25X_ReadData; 
    g_spi_inout_buff[offset++] = (u8)((ReadAddr)>>16);
    g_spi_inout_buff[offset++] = (u8)((ReadAddr)>>8);
    g_spi_inout_buff[offset++] = (u8)ReadAddr;
    SPI_Comm(g_spi_inout_buff,g_spi_inout_buff,offset+NumByteToRead); 
    memcpy(pBuffer,&g_spi_inout_buff[offset],NumByteToRead);
} 

#if 0
static void W25QXX_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{   
    unsigned int offset = 0;
    memset(g_spi_inout_buff,0xff,sizeof(g_spi_inout_buff));
    g_spi_inout_buff[offset++] = 0xAD;//W25X_PageProgram; 
    g_spi_inout_buff[offset++] = (u8)((WriteAddr)>>16);
    g_spi_inout_buff[offset++] = (u8)((WriteAddr)>>8);
    g_spi_inout_buff[offset++] = (u8)WriteAddr;
    memcpy(&g_spi_inout_buff[offset],pBuffer,NumByteToWrite);
    SPI_Comm(g_spi_inout_buff,g_spi_inout_buff,offset+NumByteToWrite); 
    W25QXX_Wait_Busy();				
} 
#else
static void W25QXX_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{   
    unsigned int temp = 0;
    unsigned int offset = 0,RealAddr = WriteAddr;
    for(temp=0;temp<NumByteToWrite;temp++)
    {
      offset = 0;
      memset(g_spi_inout_buff,0xff,sizeof(g_spi_inout_buff));
      g_spi_inout_buff[offset++] = 0x02; 
      g_spi_inout_buff[offset++] = (u8)((RealAddr)>>16);
      g_spi_inout_buff[offset++] = (u8)((RealAddr)>>8);
      g_spi_inout_buff[offset++] = (u8)RealAddr;
      g_spi_inout_buff[offset++] = pBuffer[temp];
      SPI_Comm(g_spi_inout_buff,g_spi_inout_buff,offset); 
      W25QXX_Wait_Busy();	
      RealAddr++;     
    }      
} 
#endif

static void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //????????		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//???256???
  W25QXX_Write_Enable();
  W25QXX_Wait_Busy();  
	while(1)
	{	   
		W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//?????
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //???????????
			if(NumByteToWrite>256)pageremain=256; //??????256???
			else pageremain=NumByteToWrite; 	  //??256????
		}
	};
  W25QXX_Write_Disable();  
}

	 
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * p_sector_buff;	  
  p_sector_buff = g_sector_buff;	     
 	secpos=WriteAddr/4096;//
	secoff=WriteAddr%4096;//
	secremain=4096-secoff;//
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//
	while(1) 
	{	
		W25QXX_Read(p_sector_buff,secpos*4096,4096);//
		for(i=0;i<secremain;i++)//
		{
			if(p_sector_buff[secoff+i]!=0XFF)break;// 	  
		}
		if(i<secremain)//
		{
			W25QXX_Erase_Sector(secpos);//
			for(i=0;i<secremain;i++)	   //
			{
				p_sector_buff[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(p_sector_buff,secpos*4096,4096);// 

		}else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);// 				   
		if(NumByteToWrite==secremain)break;//
		else//
		{
			secpos++;//
			secoff=0;//	 

		   	pBuffer+=secremain;  //
			WriteAddr+=secremain;//   
		   	NumByteToWrite-=secremain;				//
			if(NumByteToWrite>4096)secremain=4096;	//
			else secremain=NumByteToWrite;			//
		}	 
	};	 
}

