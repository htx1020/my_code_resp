/**
  ******************************************************************************
  * @file    queue.h
  * @author  yeaxii
  * @version V1.0.0
  * @date    2018-11-8
  * @brief   queue API接口文件
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/


#ifndef __QUEUE_H__
#define __QUEUE_H__

/* Includes ------------------------------------------------------------------*/

#include "stdin.h"

#ifdef __cplusplus
	extern "C" {
#endif


/** @defgroup queue 
  * @{
  */

/**< queue head struct */
struct queue_head 
{
    u32_t 	size;       /**< 基数大小 */
    u32_t   count;		/**< 缓冲个数 */
    u32_t   lenght;		/**< 有效数据长度 [byte = mLen * mSize] */
    u32_t   front;		/**< 首位置 */
    u32_t   rear;       /**< 尾位置 */
};

typedef struct queue_head *queue_head_t;

/**< queue struct */
struct queue 
{
    struct queue_head 	header;    /**< 头信息 */
    u8_t         		data[1];   /**< 数据缓冲区 */
};

typedef struct queue *queue_t;

/**< new queue size */
#define NEW_QUEUE_SIZE(cnt, size) \
    (sizeof(struct queue_head) + ((cnt) * (size)))


/**
 * 队列是否为空 
 * 
 * @param [queue] 队列指针 
 *
 *
 * @return 
 * 		True: 队列已空 False: 队列不为空
 *
 */
ITL_FUNC inline bool_t queue_is_empty(queue_t queue)
{
	assert_param(queue != NULL);

	return (queue->header.lenght == queue->header.count);
}


/**
 * 队列剩余空间大小 
 * 
 * @param [queue] 队列指针 
 *
 *
 * @return 
 * 		剩余大小
 *
 */
ITL_FUNC inline u32_t queue_last_size(queue_t queue)
{
	assert_param(queue != NULL);

	return (queue->header.count - queue->header.lenght);
}


/**
 * 队列数据空间大小 
 * 
 * @param [queue] 队列指针 
 *
 *
 * @return 
 * 		数据大小
 *
 */
ITL_FUNC inline u32_t queue_data_size(queue_t queue)
{
	assert_param(queue != NULL);

	return (queue->header.lenght);
}


/**
 * 清空队列数据 
 * 
 * @param [queue] 队列指针 
 *
 *
 * @return 
 * 		无
 *
 */
ITL_FUNC inline void queue_clear_data(queue_t queue)
{
	assert_param(queue != NULL);

	queue->header.front = 0;
	queue->header.rear = 0;
	queue->header.lenght = 0;
}

#ifdef __cplusplus
	extern "C" {
#endif


#endif /* __QUEUE_H__ */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) *****END OF FILE****/



