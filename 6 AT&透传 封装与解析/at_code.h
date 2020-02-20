#ifndef KIT_AT_CODE_H
#define KIT_AT_CODE_H
#include "stdint.h"

typedef struct{
	uint16_t Uuid;
	uint8_t  Len;
	uint8_t  Data[255];
} Packet_Typedef;

extern Packet_Typedef  gsRxPacketInf;
	
extern uint8_t gUartFrame[255];
extern uint8_t gUartFrameLen;

extern uint32_t OSIF_GetMilliseconds(void);

uint8_t MslFrameCapture(uint8_t SrcData);
uint8_t FetchValueFromString(char* Data,uint8_t* Value);
void FetchInfoFromData(Packet_Typedef *PackInfo,uint8_t *Data,uint8_t Len);
void AssembleInfoToData(Packet_Typedef *PackInfo,uint8_t *Data,uint8_t *Len);

#endif

