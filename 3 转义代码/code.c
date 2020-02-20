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