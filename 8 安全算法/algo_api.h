
#ifndef _CHIPHER_AES_H_
#define _CHIPHER_AES_H_

#ifdef __cplusplus
extern "C"
{
#endif

#if 1
#include "dl_config.h"
#define IS_MODE_AES128_OR_AES256  CONFIG_SSE_ALGO_AES_MODE
#else
/* 配置AES模式为128bit，或256bit   
    AES128:{IS_MODE_AES128_OR_AES256 = 1}
    AES256:{IS_MODE_AES128_OR_AES256 = 0}
*/
#define IS_MODE_AES128_OR_AES256        1
#endif

extern int simp_aes_cbc_cipher_nopadding(const unsigned char* key, const unsigned char*iv, const unsigned char*in, unsigned int length, unsigned char*out, unsigned char mode);
extern int simp_aes_ecb_cipher_nopadding(const unsigned char* key, const unsigned char*in, unsigned int length, unsigned char*out, unsigned char mode);

extern int simp_sha256_md(const unsigned char *input, unsigned int ilen, unsigned char *output);

extern int simp_hmac_sha256(const unsigned char *key, unsigned int keylen, const unsigned char *input, unsigned int ilen, unsigned char *output);

#ifdef __cplusplus
}
#endif

#endif //_INGEEK_AES_H_

