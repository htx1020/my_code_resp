/******************************************************************************
** Filename:    UtilityRingQ.h
** Description: This files is designed for the layor of CAN Bus Handle
** Author:  Huangtingxuan
** Date: 2018-09-11
** Revise: 
** Copyright (c) by ShangHai Ingeek Information Technology Co.Ltd
******************************************************************************/

#ifndef __UTIL_RING_QUEUE_H__
#define __UTIL_RING_QUEUE_H__


/*******************************************************************************
include
 */
#include <stdint.h>

typedef uint8_t 	uint8;
typedef uint16_t 	uint16;
typedef uint32_t 	uint32;

typedef uint8_t 	E_bool;
typedef int32_t     status_t;

/*******************************************************************************
 * struct_
 */
typedef struct tagRingQueue{
  unsigned short 	Head;       				// Queue header
  unsigned short 	Tail;       				// Queue tail
  unsigned short 	DataBufLen; 				// Queue Depth
  unsigned char 	*pData;     				// Queue pointor
}S_RING_QUEUE;


/*******************************************************************************
 * Extern_list
 */

/* is full */
#define RingQueueIsFull(rq)    		(((rq->Tail + 1) % rq->DataBufLen) == rq->Head)
/* is empty */
#define RingQueueIsEmpty(rq)   		(rq->Head == rq->Tail)

/*user ports*/
void KitQueueInit(void);
unsigned char KitQueuePush(uint8_t Data);
unsigned char KitQueuePop(uint8_t *Data);
unsigned char KitQueueIsFilled(void);

void UartTxQueueInit(void);
unsigned char UartTxQueuePush(unsigned char data);
unsigned char UartTxQueuePop(unsigned char *p_data);
unsigned char UartTxQueueIsFilled(void);
/*basic ports*/
void UtilRingQueueInit(S_RING_QUEUE *psRingQueue, unsigned char *pBuf, unsigned short buff_size);
unsigned short UtilRingQueueGetVolume(S_RING_QUEUE *psRingQueue);
E_bool UtilRingQueuePush(S_RING_QUEUE *psRingQueue, unsigned char StructData);
E_bool UtilRingQueuePop (S_RING_QUEUE *psRingQueue, unsigned char *pu16Rdata);

#endif // __UTIL_RING_QUEUE_H__

/******************************** END OF FILE *********************************/
