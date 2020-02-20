/******************************************************************************

                  ��Ȩ���� (C), 2018-2025, �Ϻ�������Ϣ��ȫ�����ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : crc16.h
  �� �� ��   : ����
  ��    ��   : ���ķ�
  ��������   : 2018��12��5��
  ����޸�   :
  ��������   : crc16.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��12��5��
    ��    ��   : ���ķ�
    �޸�����   : �����ļ�

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
