#ifndef __FMT_TRANS_COMM_H_
#define __FMT_TRANS_COMM_H_


#ifdef __cplusplus
    extern "C" {
#endif

#define log_str printf
#define log_hex flow_hex

void flow_hex(const char* String, unsigned char* Data, unsigned int DataLen);

//int arrayToStr(unsigned char *buf, unsigned int buflen, char *out, unsigned int *olen);

int StringToHex(const char* str, unsigned char* out, unsigned int* outlen);

int StringToCom(char *str, unsigned char *out, int *outlen);

//int HexToString(unsigned char *input, unsigned int ilen, unsigned char *ouput, unsigned int *olen);

#ifdef __cpluscplus
}
#endif

#endif

