 #include <stdio.h>
 #include "dk_protocal.h"
 
 static uint8_t index=0;
 static uint8_t tempbuf[BUFF_MAXLEN]={0};

 /*
 计算校验值
 */
 static uint8_t check_sum(uint8_t* data, uint32_t len)
 {
	 uint32_t i=0;
     uint8_t checkcode=0;

     for (i=0; i<len; i++)
     {
         checkcode^=data[i];
     }

     return checkcode;
 }

/*
转义

input: [消息头+消息体+校验值][未转义]
output:[标志位+消息头+消息体+检验值+标志位][转义后]
*/
 static status_t Escape_frame_data(uint8_t* input, uint32_t ilen, uint8_t* output, uint32_t* olen)
 {
     int i=0,index=0;
     int count=0;
	 uint8_t replacedata[2][2]={{0x7D,0x5E},{0x7D,0x5D}};

     if(input == NULL || ilen <= 0)
     {
         return HsmStatus_InvalidArgument;
     }

     output[0]=0x7E;
     output++;

     for(i=0; i<ilen; i++)
     {
         if(input[i] == 0x7E)
         {
             memcpy(output+index,replacedata[0],2);
             index+=2;
             count++;
         }
         else if(input[i] == 0x7D)
         {
             memcpy(output+index,replacedata[1],2);
             index+=2;
             count++;
         }
         else
         {
             output[index]=input[i];
             index++;
         }
     }

     output[index]=0x7E;
     *olen=index+2;

     return HsmStatus_Success;
 }

/*
反转义

input: [标志位+消息头+消息体+检验值+标志位][转义后]
output:[消息头+消息体+校验值][反转义后]
*/

 static status_t Reduction_frame_data(uint8_t* input, uint32_t ilen, uint8_t* output, uint32_t* olen)
 {
     int i=0, index=0;
     int count = 0;
     uint8_t* pdata=NULL;

     if(input == NULL || (ilen-2) <= 0)
     {
         return HsmStatus_InvalidArgument;
     }

     pdata = &input[1];
     for(i=0; i < (ilen-2); i++)
     {
         if(pdata[i] == 0x7D)
         {
        	 count++;

             if(pdata[i+1] == 0x5E)
             {
            	 output[index]=0x7E;
             }
             else if(pdata[i+1] == 0x5D)
             {
            	 output[index]=0x7D;
             }
             i++;
             index++;
         }
         else
         {
        	 output[index]=pdata[i];
             index++;
         }
     }

     *olen = index;
     return HsmStatus_Success;
}

/*
检查消息标志位，反转义和校验值。
*/
static status_t Get_frame_msg(uint8_t* input, uint32_t ilen, uint8_t* output ,uint32_t* olen)
{
	status_t Ret=HsmStatus_Success;

	if(input == NULL || output == NULL)
	{
		return HsmStatus_InvalidArgument;
	}

	if(input[0] != 0x7E || input[ilen-1] != 0x7E)
	{
		return HsmStatus_Flag_error;
	}

	Ret = Reduction_frame_data(input, ilen, output, olen);
	if(Ret != HsmStatus_Success)
	{
		return Ret;
	}

	(*olen)--;

	if(output[*olen] != check_sum(output, *olen))
	{
		return HsmStatus_Checkcode_error;
	}

	return HsmStatus_Success;
}


/*
终端蓝牙接收到APP发过来的数据，
如果没有分包，直接解析处理，获取消息头+消息体。
如果有分包，通过多次调用该接口,组完整数据包。获取消息头+消息体

input： 终端接收蓝牙数据。
ilen：  终端接收蓝牙数据大小。
output：输出反转义后的（消息头+消息体）
olen：  反转义后的(消息头+消息体)大小
*/

status_t ingeek_combine_ble_msg(uint8_t* input, uint32_t ilen, uint8_t* output ,uint32_t* olen)
{
	status_t Ret = HsmStatus_Success;
	MSG_HEAD  msg_head;

	if(input == NULL || output == NULL)
	{
		index=0;
		return HsmStatus_InvalidArgument;
	}

	if(ilen <= (sizeof(MSG_HEAD)+1))
	{
		return HsmStatus_Invalidlen;
	}

	if(input[0] == 0x7E)
	{
		memcpy((uint8_t*)&msg_head, input, sizeof(MSG_HEAD));

		if(1 == (GET_MSG_BIT(msg_head.msg_attr, 10)))
		{
			if(input[ilen-1] != 0x7E)
			{
				index=0;
				return HsmStatus_flag_error;
			}

			Ret=Get_frame_msg(input, ilen, output, olen);
			if(Ret != HsmStatus_Success)
			{
				index=0;
				return Ret;
			}
		}
		else
		{
			memcpy(tempbuf+index, input, ilen);
			index+=ilen;
			Ret = HsmStatus_More_data;
		}
	}
	else
	{
		index+=ilen;

		if(index > sizeof(tempbuf))
		{
			index=0;
			return HsmStatus_Overflow;
		}

		memcpy(tempbuf+index, input, ilen);

		if(input[ilen-1] == 0x7E)
		{
			Ret=Get_frame_msg(tempbuf, index, output, olen);
			if(Ret != HsmStatus_Success)
			{
				index=0;
				return Ret;
			}
		}
		else
		{
			Ret = HsmStatus_More_data;
		}
	}


	if(Ret == HsmStatus_Success)
	{
		index=0;
	}

	return Ret;
}

/*
传入终端蓝牙获取 Msg(消息头+消息体),处理各类型数据包数据。

Msg：	ingeek_combine_ble_msg输出数据
output：终端返回数据
olen：  终端返回数据大小
Msg_Dir:数据来源APP or 终端
*/

status_t ingeek_parse_ble_msg(Msg_Dir dir, uint8_t* Msg, uint8_t* output, uint32_t* olen)
{
	status_t Ret = HsmStatus_Success;
	MSG_HEAD  Msg_head;
	//uint8_t *Msg_body = NULL;
	
	//Msg_body=Msg+sizeof(MSG_HEAD);
	memcpy((uint8_t*)&Msg_head, Msg, sizeof(MSG_HEAD));

	switch(Msg_head.msg_id)
	{
		case INFO_ID:
			if(0 == GET_MSG_LEN(Msg_head.msg_attr)){
				//ingeek_get_info(&Msg_head);
			}
			break;
		case AUTH_ID:
			//handle_auth();
			break;
		case CMD_ID:
			//handle_cmd();
			break;
		case CAR_MSG_ID:
			//handle_msg();
			break;
		case RTC_ID:
			//handle_RTC();
			break;
		case APP_ACK_ID:
			//handle_ack();
			break;
		default:
			Ret = HsmStatus_Invalid_ID;
			break;
	}

	//Escape_frame_data(uint8_t* input, uint32_t ilen, uint8_t* output, uint32_t* olen); //数据组包

	return Ret;
}