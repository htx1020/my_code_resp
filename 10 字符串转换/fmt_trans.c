#include <string.h>
#include <stdio.h>
#include "fmt_trans.h"

#if _MSC_VER
#pragma warning(disable:4996)
#endif

#define KEY_FILE_PATH "./source/files/push_key_data.txt"

#define IS_HEX_DATA(a) ((a>='0')&&(a<='9'))||((a>='a')&&(a<='f'))||((a>='A')&&(a<='F'))? 1:0

/************************************************************************************************************/
const char atom_unit[] = "0123456789ABCDEF";
//将数组转换为十六进制同值的字符串
unsigned int arrayToStr(unsigned char *buf, unsigned int buflen, char *out)
{
    unsigned int temp = 0,offset = 0;
    for (temp = 0; temp < buflen; temp++)
    {
        out[offset++] = atom_unit[(0xF0 & buf[temp]) >> 4];
        out[offset++] = atom_unit[(0x0F & buf[temp]) >> 0];
    }
    return offset;
}

//将十六进制的字符串转换为十六进制数组
//例如:"11223344"转化后为{0x11,0x22,0x33,0x44}
int StringToHex(const char *str, unsigned char *out, unsigned int *outlen)
{
    char *p = (char*)str;
    char high = 0, low = 0;
    int tmplen = 0;
    int cnt = 0;
    tmplen = (int)strlen(str);
    if (outlen != NULL)
    {
        *outlen = 0;
    }
    while(cnt < (tmplen / 2))
    {
        if ((IS_HEX_DATA(*p) && IS_HEX_DATA(*(p + 1))) == 0)
        {
            return -1;
        }
        high = ((*p > '9') && ((*p <= 'F') || (*p <= 'f'))) ? *p - 48 - 7 : *p - 48;
        low = (*(++ p) > '9' && ((*p <= 'F') || (*p <= 'f'))) ? *(p) - 48 - 7 : *(p) - 48;

        out[cnt] = ((high & 0x0f) << 4 | (low & 0x0f));
        p ++;
        cnt ++;
    }
    if(tmplen % 2 != 0) 
    {
        out[cnt] = ((*p > '9') && ((*p <= 'F') || (*p <= 'f'))) ? *p - 48 - 7 : *p - 48;
    }
    
    if(outlen != NULL) 
    {
        *outlen = tmplen / 2 + tmplen % 2;
    }
    
    return (tmplen / 2 + tmplen % 2);
    
}


//将十进制字符串转化为十进制数组
int StringToCom(char *str, unsigned char *out, int *outlen)
{
    char *p = str;
    char high = 0, low = 0;
    int tmplen = (int)strlen(p), cnt = 0;
    tmplen = (int)strlen(p);
    if(tmplen % 2 != 0) return -1;
    while(cnt < tmplen / 2) //1213141516171819
    {
        out[cnt] = (*p - 0x30) * 10 + (*(++p) - 0x30);
        p++;
        cnt ++;
    }
    *outlen = tmplen / 2;
    return tmplen / 2;

}

void flow_hex(const char* String, unsigned char* Data, unsigned int DataLen)
{
    unsigned int Temp;
    if (DataLen > 300)
    {
        log_str("data_len overflow %d", DataLen);
        return;
    }

    log_str("%s L=%d |", String, DataLen);
    for (Temp = 0; Temp < DataLen; Temp++)
    {
        //if (Temp % 16 == 0)
        //    log_str("\r\n ");
        log_str("%02x", Data[Temp]);
    }
    log_str("|\r\n");
}

#if 0

unsigned char ArrayCom[16] = {
    11, 12, 13, 14, 15, 16, 17, 18,
    19, 20, 21, 22, 23, 24, 25, 26};
unsigned char ArrayHex[16] = {
    0x2c, 0x57, 0x8f, 0x79, 0x27, 0xa9, 0x49, 0xd3,
    0xb5, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
 
char *strHex = "01aa0304050607083f0add0c0d0e0f00";
char *strCom = "1D1213AB6FC1718B19202122232425A6";
 
int main(int argc, const char *argv)
{
    int cnt;
    char str[33] = {0};
    unsigned int olen = 0;
    
    unsigned char out[33];
    arrayToStr(ArrayCom, 16, str, &olen);
    printf("str = %.*s\n",strlen,str);
    
    int outlen = 0;
    StringToHex(strCom, strlen(strCom),out, &outlen);
    for(cnt = 0; cnt < outlen; cnt ++)
    {
        printf("%02X ", out[cnt]);
    } 
    putchar(10);
 
    return 0;

}
#endif





