/******************************************************************************

                  ��Ȩ���� (C), 2018-2025, �Ϻ�������Ϣ��ȫ�����ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : uart_protocol.c
  �� �� ��   : ����
  ��    ��   : ���ķ�
  ��������   : 2018��12��5��
  ����޸�   :
  ��������   : MCU��BLEͨ��ʹ�õ�Э��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��12��5��
    ��    ��   : ���ķ�
    �޸�����   : �����ļ�

******************************************************************************/
#include "uart_protocol.h"
#include "convert.h"
#include "crc16.h"

#define UART_FRAME_HEAD (0x7EU)
#define UART_FRAME_TAIL (0x7FU)

#define MAX_PROTOCOL_LEN (266U)    /* (10+256) */

typedef enum
{
  PROTOCOL_INIT = 0x00U,  /* ��ʼ״̬ */
  PROTOCOL_HEAD,  /* ���յ�֡ͷ */
  PROTOCOL_TAIL   /* ���յ�֡β */
}e_uart_protocol_status;

typedef struct
{
  uint8_t u8_buff[MAX_PROTOCOL_LEN*2U]; 
  uint32_t u32_len;
}s_uart_buffer;


typedef struct
{
  e_uart_protocol_status e_status;
  
  s_uart_buffer s_recv_buff;
}s_uart_recv_buffer;

static s_uart_recv_buffer s_uart_recv = {PROTOCOL_INIT,{0U}};

static s_uart_buffer s_uart_recv_decoded = {0U};

static s_uart_buffer s_uart_send = {0U};
static s_uart_buffer s_uart_send_encoded = {0U};

/*****************************************************************************
 �� �� ��: ingeek_uart_protocol_decode
 ��������: ����֡ͷ0x7E֡β0x7F����uartЭ�����ݣ�������-
               �ɺ���0x7Dת�����ת�����ݲ�����CRCУ��
 �������: uint8_t u8_byte       : ��ǰ���ڵ��ֽ�����
           uint8_t **pp_u8_output_buff   : ������ɲ���ɷ�ת����������bufferָ��ĵ�ַ
           uint16_t *p_u16_output_len   : ������ɲ���ɷ�ת���������ݵĲ���
 �������: uint8_t
 �� �� ֵ: 1��������һ�����ݲ���ɷ�ת���CRCУ��  0��δ������ݽ��ջ�ת�����
 ���ú���: 
 ��������: 
 
 �޸���ʷ:
  1.��    ��: 2018��12��6��
    ��    ��: ���ķ�
    �޸�����: �����ɺ���

*****************************************************************************/
uint8_t ingeek_uart_protocol_decode(uint8_t u8_byte, uint8_t **pp_u8_output_buff, uint16_t *p_u16_output_len)
{

    uint8_t u8_ret = 0;

    /* δ�յ�֡ͷ */
    if(s_uart_recv.e_status == PROTOCOL_INIT)
    {
        if ( u8_byte  == UART_FRAME_HEAD)
        {
            s_uart_recv.e_status = PROTOCOL_HEAD;
            s_uart_recv.s_recv_buff.u8_buff[0] = UART_FRAME_HEAD;
            s_uart_recv.s_recv_buff.u32_len = 1;
        }
        else
        {
            /* do nothing */
        }

    }
    /* �յ�֡ͷ */
    else if(s_uart_recv.e_status == PROTOCOL_HEAD)
    {
        /* ���ݳ��Ȳ������洢���� */
        if(s_uart_recv.s_recv_buff.u32_len < sizeof(s_uart_recv.s_recv_buff.u8_buff))
        {
            s_uart_recv.s_recv_buff.u8_buff[s_uart_recv.s_recv_buff.u32_len] = u8_byte;
            s_uart_recv.s_recv_buff.u32_len++;

            /* �ٴ��յ�֡ͷ */
            if ( u8_byte  == UART_FRAME_HEAD)
            {
                s_uart_recv.e_status = PROTOCOL_HEAD;
                s_uart_recv.s_recv_buff.u8_buff[0] = UART_FRAME_HEAD;
                s_uart_recv.s_recv_buff.u32_len = 1;
            }
            /* �յ�֡β */
            else if ( u8_byte  == UART_FRAME_TAIL  )
            {
                s_uart_recv.e_status = PROTOCOL_TAIL;

                /* ��ת�� */
                if(0 == Reduction_frame_data(s_uart_recv.s_recv_buff.u8_buff + 1, s_uart_recv.s_recv_buff.u32_len-2, s_uart_recv_decoded.u8_buff, &s_uart_recv_decoded.u32_len))
                {                    
                    /* crc16У�� */
                    if (0 == crc16_cal_ccitt_table(s_uart_recv_decoded.u8_buff, s_uart_recv_decoded.u32_len) )
                    {
                        if((pp_u8_output_buff!=NULL) && (p_u16_output_len!=NULL))
                        {
                            *pp_u8_output_buff = s_uart_recv_decoded.u8_buff;
                            *p_u16_output_len = (s_uart_recv_decoded.u32_len) & 0xFFFF;
                            u8_ret = 1;              
                        }
                        else
                        {
                            u8_ret = 2;
                        }
                    }
                    else
                    {
                        /* do nothing */
                    }                  
                  }
                  else
                  {
                      u8_ret = 3;
                  }
                  s_uart_recv.e_status = PROTOCOL_INIT;
                  s_uart_recv.s_recv_buff.u32_len = 0;              
            }
            else
            {
              /* do nothing */
            }
        }
        else
        {
            s_uart_recv.e_status = PROTOCOL_INIT;
            s_uart_recv.s_recv_buff.u32_len = 0;
        }
    }
    

    return u8_ret;
}

/*****************************************************************************
 �� �� ��: ingeek_uart_protocol_encode
 ��������: ����0x7Dת������������ת�壬����CRC16У��-
               �룬������֡ͷ0x7E֡β0x7FЭ����֯����
 �������: uint8_t *p_u8_input_buff : ��������bufferָ��
           uint16_t u16_input_len : �������ݳ���
           uint8_t **pp_u8_output_buff : ������bufferָ��ĵ�ַ
           uint16 *p_u16_output_len : ������ݳ���ָ��
 �������: uint8_t
 �� �� ֵ: 1���������ת�塢CRCУ���������֯  0����������
 ���ú���: 
 ��������: 
 
 �޸���ʷ:
  1.��    ��: 2018��12��6��
    ��    ��: ���ķ�
    �޸�����: �����ɺ���

*****************************************************************************/
uint8_t ingeek_uart_protocol_encode( uint8_t *p_u8_input_buff, uint16_t u16_input_len, uint8_t **pp_u8_output_buff, uint16_t *p_u16_output_len )
{
    uint16_t u16_crc = 0;

    if ( (p_u8_input_buff == NULL) || (u16_input_len==0U) || (u16_input_len > MAX_PROTOCOL_LEN) || (pp_u8_output_buff == NULL)  || (p_u16_output_len == NULL))
    {
      return 0;
    }
    else
    {
      /* do nothing */
    }

    /* crc16У�� */
    u16_crc = crc16_cal_ccitt_table(p_u8_input_buff, u16_input_len);

    memcpy(s_uart_send.u8_buff, p_u8_input_buff, u16_input_len);
    s_uart_send.u8_buff[u16_input_len] = u16_crc&0xFF;
    s_uart_send.u8_buff[u16_input_len + 1] = (u16_crc>>8) & 0xFF;

    s_uart_send.u32_len = u16_input_len + 2;

    /* ת�� */
    if(0 == Escape_frame_data(s_uart_send.u8_buff, s_uart_send.u32_len, s_uart_send_encoded.u8_buff + 1, &s_uart_send_encoded.u32_len))
    {
        s_uart_send_encoded.u8_buff[0] = UART_FRAME_HEAD;
        s_uart_send_encoded.u8_buff[s_uart_send_encoded.u32_len + 1] = UART_FRAME_TAIL;
        *pp_u8_output_buff = s_uart_send_encoded.u8_buff;
        *p_u16_output_len = s_uart_send_encoded.u32_len + 2;

        return 1;
    }
    else
    {
      return 0;
    }

}

