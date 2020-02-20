/******************************************************************************

                  版权所有 (C), 2018-2025, 上海银基信息安全技术股份有限公司

 ******************************************************************************
  文 件 名   : uart_protocol.c
  版 本 号   : 初稿
  作    者   : 赵四方
  生成日期   : 2018年12月5日
  最近修改   :
  功能描述   : MCU与BLE通信使用的协议
  函数列表   :
  修改历史   :
  1.日    期   : 2018年12月5日
    作    者   : 赵四方
    修改内容   : 创建文件

******************************************************************************/
#include "uart_protocol.h"
#include "convert.h"
#include "crc16.h"

#define UART_FRAME_HEAD (0x7EU)
#define UART_FRAME_TAIL (0x7FU)

#define MAX_PROTOCOL_LEN (266U)    /* (10+256) */

typedef enum
{
  PROTOCOL_INIT = 0x00U,  /* 初始状态 */
  PROTOCOL_HEAD,  /* 接收到帧头 */
  PROTOCOL_TAIL   /* 接收到帧尾 */
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
 函 数 名: ingeek_uart_protocol_decode
 功能描述: 按照帧头0x7E帧尾0x7F处理uart协议数据，接收完-
               成后按照0x7D转义规则反转义数据并进行CRC校验
 输入参数: uint8_t u8_byte       : 当前串口单字节数据
           uint8_t **pp_u8_output_buff   : 接收完成并完成反转义的输出数据buffer指针的地址
           uint16_t *p_u16_output_len   : 接收完成并完成反转义的输出数据的铲毒
 输出参数: uint8_t
 返 回 值: 1：接收完一包数据并完成反转义和CRC校验  0：未完成数据接收或反转义错误
 调用函数: 
 被调函数: 
 
 修改历史:
  1.日    期: 2018年12月6日
    作    者: 赵四方
    修改内容: 新生成函数

*****************************************************************************/
uint8_t ingeek_uart_protocol_decode(uint8_t u8_byte, uint8_t **pp_u8_output_buff, uint16_t *p_u16_output_len)
{

    uint8_t u8_ret = 0;

    /* 未收到帧头 */
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
    /* 收到帧头 */
    else if(s_uart_recv.e_status == PROTOCOL_HEAD)
    {
        /* 数据长度不超过存储区域 */
        if(s_uart_recv.s_recv_buff.u32_len < sizeof(s_uart_recv.s_recv_buff.u8_buff))
        {
            s_uart_recv.s_recv_buff.u8_buff[s_uart_recv.s_recv_buff.u32_len] = u8_byte;
            s_uart_recv.s_recv_buff.u32_len++;

            /* 再次收到帧头 */
            if ( u8_byte  == UART_FRAME_HEAD)
            {
                s_uart_recv.e_status = PROTOCOL_HEAD;
                s_uart_recv.s_recv_buff.u8_buff[0] = UART_FRAME_HEAD;
                s_uart_recv.s_recv_buff.u32_len = 1;
            }
            /* 收到帧尾 */
            else if ( u8_byte  == UART_FRAME_TAIL  )
            {
                s_uart_recv.e_status = PROTOCOL_TAIL;

                /* 反转义 */
                if(0 == Reduction_frame_data(s_uart_recv.s_recv_buff.u8_buff + 1, s_uart_recv.s_recv_buff.u32_len-2, s_uart_recv_decoded.u8_buff, &s_uart_recv_decoded.u32_len))
                {                    
                    /* crc16校验 */
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
 函 数 名: ingeek_uart_protocol_encode
 功能描述: 按照0x7D转义规则进行数据转义，增加CRC16校验-
               码，并按照帧头0x7E帧尾0x7F协议组织数据
 输入参数: uint8_t *p_u8_input_buff : 输入数据buffer指针
           uint16_t u16_input_len : 输入数据长度
           uint8_t **pp_u8_output_buff : 输出输出buffer指针的地址
           uint16 *p_u16_output_len : 输出数据长度指针
 输出参数: uint8_t
 返 回 值: 1：完成数据转义、CRC校验和数据组织  0：操作错误
 调用函数: 
 被调函数: 
 
 修改历史:
  1.日    期: 2018年12月6日
    作    者: 赵四方
    修改内容: 新生成函数

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

    /* crc16校验 */
    u16_crc = crc16_cal_ccitt_table(p_u8_input_buff, u16_input_len);

    memcpy(s_uart_send.u8_buff, p_u8_input_buff, u16_input_len);
    s_uart_send.u8_buff[u16_input_len] = u16_crc&0xFF;
    s_uart_send.u8_buff[u16_input_len + 1] = (u16_crc>>8) & 0xFF;

    s_uart_send.u32_len = u16_input_len + 2;

    /* 转义 */
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

