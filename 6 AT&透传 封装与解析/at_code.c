#include "at_code.h"
#include "stdio.h"
#include "string.h"

Packet_Typedef  gsRxPacketInf;
uint8_t gUartFrame[255];
uint8_t gUartFrameLen;

const uint8_t Byte1 = 'A';
const uint8_t Byte2 = 'T';
const uint8_t Byte3 = '-';
const uint8_t Byten = 0x0D;

/*从FIFO中取出AT指令数据包*/
uint8_t MslFrameCapture(uint8_t SrcData)
{
	uint8_t Temp = 0;
  uint8_t FlagResult = 0;
	static  uint8_t  sFlagHeader = 0;         //帧头识别
	static  uint8_t  sRxCnt = 0;              //计数
	static  uint8_t  sUartRecStartFlg = 0;  	//新开始标记
	static  uint32_t sLastReceiT = 0;   			//上次计时
	static	uint8_t  sCommaCount = 0;    			//逗号数量
	static  uint8_t  pFristComma = 0;         //第1个逗号位置
	
	//开始接收包体数据
	if (sUartRecStartFlg)
	{
		/*填充缓存区*/
		gUartFrame[sRxCnt] = SrcData & 0xff;
		sRxCnt++;
		/*  */ 
		if (',' == SrcData)                                                             
		{
			sCommaCount++;
			if(sCommaCount == 1)
			{
				pFristComma = sRxCnt; //记录第1个逗号
			}
			else if(sCommaCount == 2)
			{
				FetchValueFromString((char*)&gUartFrame[pFristComma],&Temp);
				if(Temp > 255)
				{
					sUartRecStartFlg = 0;      //重置接收标记_______________
				}
				
				gUartFrameLen = sRxCnt;
				gUartFrameLen += Temp; 
				gsRxPacketInf.Len =  gUartFrameLen; //送入结构体
			}
		}
		if(sRxCnt>16)
		{
			if(sRxCnt == gUartFrameLen)
			{
				FlagResult = 1;
				sUartRecStartFlg = 0;      //重置接收标记_______________
				
			}
				
		}
	}
  // 收到包头标记
  if(SrcData == Byte1)
  {	
		sFlagHeader = 1;
  }
	else if(SrcData == Byte2)
	{
		if(sFlagHeader == 1)
		{
			 sFlagHeader = 2;
		}
		else
		{
			 sFlagHeader = 0;
		}
	}
	else if(SrcData == Byte3)
	{
		if(sFlagHeader == 2)
		{
			sFlagHeader = 0;
			sUartRecStartFlg = 1;      //开始接收新数据帧
			gUartFrameLen = 0;
			sCommaCount = 0;
			Temp = 255;
			while(Temp > 0)
			{
				gUartFrame[255-Temp] = 0;
				Temp--;
			}
			gUartFrame[0] = 'A';
			gUartFrame[1] = 'T';
			gUartFrame[2] = '-';
			sRxCnt = 3;
			sLastReceiT = OSIF_GetMilliseconds();
		}
		else
		{
			 sFlagHeader = 0;
		}
	}
  else
  {
    
  }
	if((sRxCnt > 240) || ((OSIF_GetMilliseconds() - sLastReceiT) > 2))
	{
		sUartRecStartFlg = 0; //重置接收标记_______________
	}
	
  return FlagResult;  
}

/*从数据缓冲区中取出DK数据结构*/
void FetchInfoFromData(Packet_Typedef *PackInfo,uint8_t *Data,uint8_t Len)
{
	uint8_t Temp,TempData;
	uint8_t CntSpace = 0;
	uint8_t CntComma = 0;
	uint8_t CypStart = 0;
	
	for(Temp=0;Temp<Len;Temp++)
	{
		if(CypStart)  //开始拷贝
		{
			PackInfo->Data[Temp - CypStart] = Data[Temp];
		}
		
		if(' ' == Data[Temp] )
		{
			CntSpace++;
			if(CntSpace == 2)
			{	
				FetchValueFromString((char*)&Data[Temp+1],&TempData);
				PackInfo->Uuid = TempData;
			}
		}
		if(',' == Data[Temp] )
		{
			CntComma++;
			if(CntComma == 1)
			{
				FetchValueFromString((char*)&Data[Temp+1],&TempData);
				PackInfo->Len = TempData;
				CypStart = Temp+1;
			}
			else if(CntComma == 2)
			{
				CypStart = Temp+1;
			}
		}
	}
}

/*将DK数据结构填充到数据缓冲区中*/
void AssembleInfoToData(Packet_Typedef *PackInfo,uint8_t *Data,uint8_t *Len)
{
	uint8_t Temp,StrLen;
	char		StringData[20] ;
	
	Temp = 20;
	while(Temp--)
	{
		StringData[Temp] = 0;
	}
	sprintf(StringData,"AT+B DATIND %d,%d,",PackInfo->Uuid,PackInfo->Len);
	StrLen = strlen(StringData);
	*Len = StrLen + PackInfo->Len;
	
	for(Temp=0;Temp<*Len;Temp++)
	{
		if(Temp < StrLen)
		{
			Data[Temp] = StringData[Temp]; 
		}
		else
		{
		  Data[Temp] = PackInfo->Data[Temp-StrLen]; 
		}
		
	}
}






char StdNumTable[10]={'0','1','2','3','4','5','6','7','8','9'};
uint8_t FetchNumFromChar(char Data)
{
	uint8_t RetVal = 0xFF,Temp;
	
	for(Temp = 0; Temp < 10; Temp++)
	{
		if(Data == StdNumTable[Temp])
		{
			RetVal = Temp;
		}
	}
	
	return RetVal;
}

#define NUM_LENTH 8
uint8_t FetchValueFromString(char* Data,uint8_t* Value)
{
	uint8_t 	Temp,NumLen;
	uint8_t 	NumList[NUM_LENTH];
	uint8_t 	RetNum = 0;
	
	for(Temp = 0; Temp < NUM_LENTH; Temp++)
	{
		NumList[Temp] = FetchNumFromChar(*Data);
		if(0xFF == NumList[Temp])
		{
			NumLen = Temp;
			break;
		}
		Data++;
	}
	for(Temp = 0; Temp < NumLen; Temp++)
	{
		RetNum = RetNum*10 + NumList[Temp];
	}
	if(NumLen != 0)
	{
		*Value = RetNum;
	}
	return NumLen;
}
