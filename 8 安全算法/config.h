#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string.h>

typedef unsigned char   u1;
typedef unsigned short  u2;
typedef unsigned int    u4;
typedef unsigned long long u8;

typedef void*           eptr_v;
typedef void*           uptr_v;
typedef u1*             eptr;
typedef u1*             uptr;
typedef u2*             eptr_u2;
typedef u2*             uptr_u2;
typedef u4*             eptr_u4;
typedef u4*             uptr_u4;
typedef u8*                            uptr_u8;

#define __data
#define __idata
#define __pdata
#define __bdata
#define __xdata
#define __code        const
#define __bit

//----------------------------------------
// user defined value
//----------------------------------------
#define true                                                        1
#define false                                                        0

/***********************************************/
/*         SHA256 definition                   */
/***********************************************/

#define SHA256_DIGEST_LENGTH                        32
#define SHA256_STATE_LENGTH                         64
#define MAX_SHA_256_SUUPORT_BIT_COUNT        		8

#define MAX_MD_DIGEST_LEN                           32
#define MAX_MD_STATE_LEN                            64
#define MAX_MD_BIT_COUNT_LEN                        8

#define HMAC_MD_SLEN                                        SHA256_STATE_LENGTH
#define HMAC_MD_DLEN                                        SHA256_DIGEST_LENGTH


#endif
