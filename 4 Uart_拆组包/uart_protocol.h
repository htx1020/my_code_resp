/**
* \brief        ���ļ����ڷ�װMCUӦ�ó�����UARTЭ�鴦��ҵ��
* \file         uart_protocol.h
* \author       ingeek telematics securtiy team
* \date         2018-3-21
* \version      A001
* \copyright    Ingeek Information Security Consulting Associates Co.
*/

/******************************************************************************

                  ��Ȩ���� (C), 2018-2025, �Ϻ�������Ϣ��ȫ�����ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : uart_protocol.h
  �� �� ��   : ����
  ��    ��   : ���ķ�
  ��������   : 2018��12��5��
  ����޸�   :
  ��������   : uart_protocol.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��12��5��
    ��    ��   : ���ķ�
    �޸�����   : �����ļ�

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
* \brief ���ݴ���Э����ɴ������ݵĽ��ܣ�����ת�������У��
* \param [in] u8_byte �ӵײ��������յĴ�������
* \param [out] pp_u8_output_buff ָ��������ݵ�ַ��ָ��
* \param [out] p_u16_output_len  ������ݵĳ���
* \retval  1��ִ�гɹ� 0��ִ��ʧ��
*/
uint8_t ingeek_uart_protocol_decode(uint8_t u8_byte, uint8_t **pp_u8_output_buff, uint16_t *p_u16_output_len);

/*!
* \brief ���ݴ���Э����ɴ������ݵļ��ܣ�������У����ͽ���ת��
* \param [in] p_u8_input_buff ָ��������ݵ�ַ��ָ��
* \param [in] u16_input_len  ������ݵĳ���
* \param [out] pp_u8_output_buff ָ��������ݵ�ַ��ָ��
* \param [out] p_u16_output_len  ������ݵĳ���
* \retval  1��ִ�гɹ� 0��ִ��ʧ��
*/
uint8_t ingeek_uart_protocol_encode( uint8_t *p_u8_input_buff, uint16_t u16_input_len, uint8_t **pp_u8_output_buff, uint16_t *p_u16_output_len );



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __UART_PROTOCOL_H__ */
/**@}*/

