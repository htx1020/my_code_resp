#ifndef _CORE_H_
#define _CORE_H_

#include "config.h"


struct SHA256_CONTEXT
{
    u1 *digest;    // message digest
    u1 *state;     // data buffer
    u1 *count;     // count of bits
};

#define core_dcm_u1_lo(b)                   ((u1)((u1)(b)&0x0f))
#define core_dcm_u1_hi(b)                   ((u1)((u1)(b)>>4))

#define core_dcm_u2_lo(w)                   ((u1)((u2)(w)&0x00ff))
#define core_dcm_u2_hi(w)                   ((u1)((u2)(w)>>8))

#define core_dcm_u4_lo(dw)                  ((u2)((u4)(dw)&0xffff))
#define core_dcm_u4_hi(dw)                  ((u2)((u4)(dw)>>16))

#define core_dcm_mku2(h, l)                ((u2)(((u2)(h)<<8) | (l)))
#define core_dcm_mku4(hh, hl, lh, ll)      ((u4)((((u4)hh)<<24) | ((u4)(hl)<<16) | ((u2)(lh)<<8) | (ll)))
#define core_dcm_readu1(src)                ((u1)(*((u1 *)(src))))
#define core_dcm_readBigu2(src)             core_dcm_mku2((((u1 *)(src))[0]), (((u1 *)(src))[1]))
#define core_dcm_readBigu4(src)             core_dcm_mku4((((u1 *)(src))[0]), (((u1 *)(src))[1]), (((u1 *)(src))[2]), (((u1 *)(src))[3]))
#define core_dcm_readBigu3(src)             core_dcm_mku4(                 0, (((u1 *)(src))[0]), (((u1 *)(src))[1]), (((u1 *)(src))[2]))

#define core_dcm_writeu1(dest, value)       ((((u1 *)(dest))[0]) = (value))

#define core_dcm_writeBigu2(dest, value)    do {(((u1 *)(dest))[0]) = (u1)((value)>>8); (((u1 *)(dest))[1]) = (u1)(value);} while(0);

#define core_dcm_writeBigu4(dest, value)    do { \
                                                (((u1 *)(dest))[0]) = (u1)((value)>>24); \
                                                (((u1 *)(dest))[1]) = (u1)((value)>>16); \
                                                (((u1 *)(dest))[2]) = (u1)((value)>>8);  \
                                                (((u1 *)(dest))[3]) = (u1)(value);       \
                                            } while(0);

#define core_dcm_writeBigu3(dest, /* u32 */value)   do { \
                                                (((u1 *)(dest))[0]) = (u1)((value)>>16); \
                                                (((u1 *)(dest))[1]) = (u1)((value)>>8);  \
                                                (((u1 *)(dest))[2]) = (u1)(value);       \
                                            } while(0);


void core_mm_set(u1 * dest, const u1 val, u2 length);    
void core_mm_copy(u1 * dest, const u1 * src, u2 length);
void core_mm_copy_u32(u4 * dest, const u4 * src, u2 length);
u2 core_algo_convert_u8tou32(u1 *inBuf, u2 inLength, u4 *outBuf);
u2 core_algo_convert_u32tou8(u4 *inBuf, u2 inLength, u1 *outBuf);

#ifdef __cplusplus
extern "C" {
#endif

void core_algo_sha256Initial(struct SHA256_CONTEXT *sha256_ctx);
u1 core_algo_sha256Update(struct SHA256_CONTEXT *sha256_ctx, u1 *buf, u2 length);
void core_algo_sha256Final(struct SHA256_CONTEXT *sha256_ctx);

void core_algo_xor(const u1 *buf1, const u1 *buf2, u1 *outBuf, u2 length);
void core_algo_hmacInitial(struct SHA256_CONTEXT *md_ctx, u1 *key, u2 key_len, u1 *k_pad0, u1 *k_pad1);
u1 core_algo_hmacUpdate(struct SHA256_CONTEXT *md_ctx, u1 *buf, u2 length);
void core_algo_hmacFinal(struct SHA256_CONTEXT *md_ctx, u1 *k_pad1);    

extern int simp_sha256_md(const unsigned char *input, unsigned int ilen, unsigned char *output);
extern int simp_hmac_sha256(const unsigned char *key, unsigned int keylen, const unsigned char *input, unsigned int ilen, unsigned char *output);


#ifdef __cplusplus
}
#endif

#endif
