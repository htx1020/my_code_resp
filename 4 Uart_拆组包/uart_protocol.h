/**
* \brief        本文件用于封装MCU应用程序中UART协议处理业务
* \file         uart_protocol.h
* \author       ingeek telematics securtiy team
* \date         2018-3-21
* \version      A001
* \copyright    Ingeek Information Security Consulting Associates Co.
*/

/******************************************************************************

                  版权所有 (C), 2018-2025, 上海银基信息安全技术股份有限公司

 ******************************************************************************
  文 件 名   : uart_protocol.h
  版 本 号   : 初稿
  作    者   : 赵四方
  生成日期   : 2018年12月5日
  最近修改   :
  功能描述   : uart_protocol.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2018年12月5日
    作    者   : 赵四方
    修改内容   : 创建文件

******************************************************************************/

/**
* \defgroup MCU_UART_API  
*/
/**@{*/
#ifndef __UART_PROTOCOL_H__
#define __UART_PROTOCOL_H__

#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*!
* \brief 根据串口协议完成串口数据的解密，即反转义和数据校验
* \param [in] u8_byte 从底层驱动接收的串口数据
* \param [out] pp_u8_output_buff 指向输出数据地址的指针
* \param [out] p_u16_output_len  输出数据的长度
* \retval  1：执行成功 0：执行失败
*/
uint8_t ingeek_uart_protocol_decode(uint8_t u8_byte, uint8_t **pp_u8_output_buff, uint16_t *p_u16_output_len);

/*!
* \brief 根据串口协议完成串口数据的加密，即增加校验码和进行转义
* \param [in] p_u8_input_buff 指向输出数据地址的指针
* \param [in] u16_input_len  输出数据的长庿
* \param [out] pp_u8_output_buff 指向输出数据地址的指针
* \param [out] p_u16_output_len  输出数据的长度
* \retval  1：执行成功 0：执行失败
*/
uint8_t ingeek_uart_protocol_encode( uint8_t *p_u8_input_buff, uint16_t u16_input_len, uint8_t **pp_u8_output_buff, uint16_t *p_u16_output_len );



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __UART_PROTOCOL_H__ */
/**@}*/

