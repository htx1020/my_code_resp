/******************************************************************************
** Filename:    UtilityRingQ.c
** Description: This files is designed for the ring_queue Handle
** Author:  Huangtingxuan
** Date: 2018-09-11
** Revise: 
** Copyright (c) by ShangHai Ingeek Information Technology Co.Ltd
******************************************************************************/

/*******************************************************************************
 * include
 */
#include "kit_ring_queue.h"
#include "fsl_os_abstraction.h"
/*******************************************************************************
 * function
 */

/* 队列深度250字节。*/
#define RX_BUFF_SIZE 250
#define TX_BUFF_SIZE 500

#define  E_false 	0
#define  E_true 	1
#ifndef NULL
#define  NULL 		0
#endif
#define OsInterruptDisable	OSA_DisableIRQGlobal//OSA_InterruptDisable
#define OsInterruptEnable	OSA_EnableIRQGlobal //OSA_InterruptEnable

unsigned char g_recv_buff[RX_BUFF_SIZE];	//接收缓存
S_RING_QUEUE gs_rx_queue_handle;			//队列处理句柄

unsigned char g_sent_buff[TX_BUFF_SIZE];	//接收缓存
S_RING_QUEUE gs_tx_queue_handle;			//队列处理句柄

void KitQueueInit(void)
{
	UtilRingQueueInit(&gs_rx_queue_handle,g_recv_buff,RX_BUFF_SIZE);
}

unsigned char KitQueuePush(unsigned char data)
{
	return UtilRingQueuePush(&gs_rx_queue_handle,data);
}

unsigned char KitQueuePop(unsigned char *p_data)
{
	return UtilRingQueuePop(&gs_rx_queue_handle,p_data);
}

unsigned char KitQueueIsFilled(void)
{
	return UtilRingQueueGetVolume(&gs_rx_queue_handle);
}
////////////////////////////////////////////////////////////////////////////////
void UartTxQueueInit(void)
{
	UtilRingQueueInit(&gs_tx_queue_handle,g_sent_buff,TX_BUFF_SIZE);
}

unsigned char UartTxQueuePush(unsigned char data)
{
	return UtilRingQueuePush(&gs_tx_queue_handle,data);
}

unsigned char UartTxQueuePop(unsigned char *p_data)
{
	return UtilRingQueuePop(&gs_tx_queue_handle,p_data);
}

unsigned char UartTxQueueIsFilled(void)
{
	return UtilRingQueueGetVolume(&gs_tx_queue_handle);
}
///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

/*********************************************************************
UtilRingQueueInit
*********************************************************************/
unsigned short UtilRingQueueGetVolume(S_RING_QUEUE *psRingQueue)
{
	if(psRingQueue->Tail >= psRingQueue->Head)
	{
		return (psRingQueue->Tail - psRingQueue->Head);
	}
	else
	{
		return (psRingQueue->Tail + psRingQueue->DataBufLen - psRingQueue->Head);
	}
}

/*********************************************************************
UtilRingQueueInit
*********************************************************************/
void UtilRingQueueInit(S_RING_QUEUE *psRingQueue, unsigned char *pBuf, unsigned short buff_size)
{
  psRingQueue->Head       = 0;
  psRingQueue->Tail       = 0;
  psRingQueue->pData      = pBuf;
  psRingQueue->DataBufLen = buff_size;
}

/*********************************************************************
UtilRingQueuePush
*********************************************************************/
E_bool UtilRingQueuePush(S_RING_QUEUE *psRingQueue, unsigned char StructData)
{
  if (RingQueueIsFull(psRingQueue))
  {
    return E_false;
  }
  else
  {
	OsInterruptDisable();
    psRingQueue->pData[psRingQueue->Tail] = StructData; // push
	psRingQueue->Tail = (psRingQueue->Tail + 1) % psRingQueue->DataBufLen; // update
	OsInterruptEnable();
    return E_true;
  }
}

/*********************************************************************
UtilRingQueuePop
*********************************************************************/
E_bool UtilRingQueuePop(S_RING_QUEUE *psRingQueue, unsigned char *pu16Rdata)
{
  if (RingQueueIsEmpty(psRingQueue))
  {
    return E_false;
  }
  else
  {
	OsInterruptDisable();
    *pu16Rdata = psRingQueue->pData[psRingQueue->Head]; // save
    psRingQueue->Head = (psRingQueue->Head + 1) % psRingQueue->DataBufLen; // update
    OsInterruptEnable();
  }

  return E_true;
}

