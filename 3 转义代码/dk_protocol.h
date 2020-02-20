/*
 * digital_key_api.h
 *
 *  Created on: 2018年7月12日
 *      Author: zhenchiw
 */

#ifndef DIGITAL_KEY_API_H_
#define DIGITAL_KEY_API_H_


#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus*/

typedef int32_t status_t;

#define  BUFF_MAXLEN  256

#define  GET_MSG_LEN(a)   	((a) & 0x3ff)
#define  SET_MSG_LEN(a,b) 	(((a) & (~0x3ff))|(b))
#define  SET_MSG_BIT(a,bit) (a |= (0x01<<bit))
#define  CLR_MSG_BIT(a,bit) (a &= ~(0x01<<bit))
#define  GET_MSG_BIT(a,bit) (a & (0x01<<bit))


/*消息数据来源*/
typedef enum
{
	Msg_APP,
	Msg_TERMINAL
}Msg_Dir;

/*协议消息头ID*/
typedef enum
{
	APP_ACK_ID = 0x8601,
	CAR_ACK_ID = 0x0601,
	INFO_ID	   = 0x0602,
	AUTH_ID    = 0x0603,
	CMD_ID     = 0x0604,
	CAR_MSG_ID = 0x0605,
	RTC_ID     = 0x0606,
}MSG_ID;


/*协议消息头结构*/
typedef struct
{
    uint16_t msg_id;
    uint16_t msg_attr;
    uint16_t msg_number;
}MSG_HEAD;

/*接口状态码*/
typedef enum
{
	HsmStatus_Checkcode_error,
	HsmStatus_Flag_error,
	HsmStatus_more_data,
	HsmStatus_Overflow,
	HsmStatus_Invalidlen,
	HsmStatus_Invalid_ID,
}generic_status;

extern status_t ingeek_parse_ble_msg(Msg_Dir dir, uint8_t* Msg, uint8_t* output, uint32_t* olen);

extern status_t ingeek_combine_ble_msg(uint8_t* input, uint32_t ilen, uint8_t* output ,uint32_t* olen);

#if defined(__cplusplus)
}
#endif /*_cplusplus*/


#endif /* DIGITAL_KEY_API_H_ */
