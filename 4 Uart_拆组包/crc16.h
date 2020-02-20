/******************************************************************************

                  版权所有 (C), 2018-2025, 上海银基信息安全技术股份有限公司

 ******************************************************************************
  文 件 名   : crc16.h
  版 本 号   : 初稿
  作    者   : 赵四方
  生成日期   : 2018年12月5日
  最近修改   :
  功能描述   : crc16.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2018年12月5日
    作    者   : 赵四方
    修改内容   : 创建文件

******************************************************************************/
#ifndef __CRC16_H__
#define __CRC16_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include <stdint.h>


#define CRC_CAL_TABLE_MODE 1


#if CRC_CAL_TABLE_MODE

uint16_t crc16_cal_ccitt_table(uint8_t* dataIn, uint32_t length);

uint16_t crc16_cal_ibm_table(uint8_t* dataIn, uint32_t length);


#else

uint16_t crc16_cal_ccitt(uint8_t *puchMsg, uint32_t usDataLen);

uint16_t crc16_cal_ibm(uint8_t *puchMsg, uint32_t usDataLen);

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __CRC16_H__ */
