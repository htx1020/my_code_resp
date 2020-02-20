/*===========================================================================*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                                    */
/*%%   _____________________________   %%                                    */
/*%%  |                             |  %%                                    */
/*%%  | &&&&&   &&                  |  %%                                    */
/*%%  |   &     & &                 |  %%                                    */
/*%%  |   &     &  &                |  %%                                    */
/*%%  |   &     &&&&&               |  %%                                    */
/*%%  | &&&&&   &                   |  %%                                    */
/*%%  |_____________________________|  %%                                    */
/*%%                                   %%                                    */
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                                    */
/*===========================================================================*/
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*===========================================================================*/


/*=============================== FILE INCLUSION *===========================*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*
 * note: 
 */


#include "queue.h"
#include "rh850x.h"

void queue_init(queue_t queue, u32_t cnt, u32_t size)
{
	assert_param(queue != NULL);

	queue->header.count = cnt;
	queue->header.size = size;
	queue->header.front = 0;
	queue->header.rear = 0;
	queue->header.lenght = 0;
}


void queue_put(queue_t queue, void *data, u32_t cnt, u32_t size)
{
	u8_t *pdata = (u8_t *)data;
	assert_param(queue != NULL);
	assert_param(queue->header.size == size);
	if ((queue_last_size(queue) < cnt) || !pdata || !cnt) 
	{
		return ;
	}
	
	hw_interrupt_disable();
	while (cnt--)
	{
		if ((queue->header.front + size) > (queue->header.count * size))
		{
			queue->header.front = 0;
		}

		memcpy(&(queue->data[queue->header.front]), pdata, size);
		pdata += size;
		queue->header.front += size;
		queue->header.lenght++;
	}
	hw_interrupt_enable();
}


void queue_get(queue_t queue, void *data, u32_t cnt, u32_t size)
{
	u8_t *pdata = (u8_t *)data;
	assert_param(queue != NULL);
	assert_param(queue->header.size == size);
	if (queue_data_size(queue) < cnt || !pdata || !cnt)
	{
		return ;
	}

	hw_interrupt_disable();\
	while (cnt--)
	{
		if ((queue->header.rear + size) > (queue->header.count * size))
		{
			queue->header.rear = 0;
		}

		memcpy(pdata, &(queue->data[queue->header.rear]), size);
		pdata += size;
		queue->header.rear += size;
		queue->header.lenght--;
	}
	
	hw_interrupt_enable();
}

