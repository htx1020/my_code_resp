#include "config.h"
#include "core.h"

/**
 * Constants definition
 */
static __code u4 SHA256_K[64] = {
   0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
   0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
   0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
   0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
   0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
   0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
   0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
   0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

static __code u1 SHA256_IV[SHA256_DIGEST_LENGTH] = {
    0x6a, 0x09, 0xe6, 0x67, 0xbb, 0x67, 0xae, 0x85, 0x3c, 0x6e, 0xf3, 0x72, 0xa5, 0x4f, 0xf5, 0x3a,
    0x51, 0x0e, 0x52, 0x7f, 0x9b, 0x05, 0x68, 0x8c, 0x1f, 0x83, 0xd9, 0xab, 0x5b, 0xe0, 0xcd, 0x19
};


/**
* Caculate Buffer for md
*/
u1 digest[SHA256_DIGEST_LENGTH];    // message digest
u1 state[SHA256_STATE_LENGTH];        // data buffer
u1 count[MAX_SHA_256_SUUPORT_BIT_COUNT];    // count of bits

/**
 * Static Methods implementation
 */
// DBL_INT_ADD treats two unsigned ints a and b as one 64-bit integer and adds c to it
#define DBL_INT_ADD(a,b,c) if (a > 0xffffffff - (c)) ++b; a += c;
#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

void core_mm_set(u1 * dest, const u1 val, u2 length)
{
    while (length--)
        dest[length] = val;
}

void core_mm_copy(u1 * dest, const u1 * src, u2 length)
{
    if (dest < src)
    {
        while (length--)
        {
            *dest = *src;
            dest += 1L;
            src += 1L;
        }
    }
    else if (dest > src)
    {
        while (length--)
        {
            dest[length] = src[length];
        }
    }
}

void core_mm_copy_u32(u4 * dest, const u4 * src, u2 length)
{
    if (dest < src)
    {
        while (length--)
        {
            *dest = *src;
            dest += 1L;
            src += 1L;
        }
    }
    else if (dest > src)
    {
        while (length--)
        {
            dest[length] = src[length];
        }
    }
}

u2 core_algo_convert_u8tou32(u1 *inBuf, u2 inLength, u4 *outBuf)
{
    u2 i;
    u1 j;

    for (i=0, j=24; i<inLength; i++, j-=8)
    {
        if (j == 24)
        {
            outBuf[i/4] = 0;
        }
        outBuf[i/4] |= ((u4)inBuf[i]) << j;
        if (j == 0)
        {
            j = 32;
        }
    }

    return (u2)((inLength+3)/4);
}

u2 core_algo_convert_u32tou8(u4 *inBuf, u2 inLength, u1 *outBuf)
{
    u2 i;

    for (i=0; i<inLength; i++)
    {
        outBuf[i*4] = (u1)(inBuf[i] >> 24);
        outBuf[i*4+1] = (u1)(inBuf[i] >> 16);
        outBuf[i*4+2] = (u1)(inBuf[i] >> 8);
        outBuf[i*4+3] = (u1)(inBuf[i]);
    }

    return (u2)(inLength*4);
}

static void sha256Hash(u1 *state, u1 *digest)
{
    u1 i;
    u4 D[SHA256_DIGEST_LENGTH/4];
    u4 D1[SHA256_DIGEST_LENGTH/4];
    u4 t1, t2;
    u4 SHA256_M[64];

    core_algo_convert_u8tou32(digest, SHA256_DIGEST_LENGTH, D);
    core_mm_copy_u32(D1, D, SHA256_DIGEST_LENGTH/4);
    core_algo_convert_u8tou32(state, SHA256_STATE_LENGTH, SHA256_M);

    for (i=16; i<64; i++)
    {
        SHA256_M[i] = SIG1(SHA256_M[i-2]) + SHA256_M[i-7] + SIG0(SHA256_M[i-15]) + SHA256_M[i-16];
    }
    
    for (i = 0; i < 64; ++i)
    {
        t1 = D[7] + EP1(D[4]) + CH(D[4],D[5],D[6]) + SHA256_K[i] + SHA256_M[i];
        t2 = EP0(D[0]) + MAJ(D[0],D[1],D[2]);
        D[7] = D[6];
        D[6] = D[5];
        D[5] = D[4];
        D[4] = D[3] + t1;
        D[3] = D[2];
        D[2] = D[1];
        D[1] = D[0];
        D[0] = t1 + t2;
    }

    for (i=0; i<8; i++)
    {
        D1[i] += D[i];
    }

    // save resulting digest
    core_algo_convert_u32tou8(D1, SHA256_DIGEST_LENGTH/4, digest);

    // clear the state
    core_mm_set(state, 0, SHA256_STATE_LENGTH);
}

/**
 * Methods implementation
 */
void core_algo_sha256Initial(struct SHA256_CONTEXT *sha256_ctx)
{
    // set digest to initial IV
    core_mm_copy(sha256_ctx->digest, SHA256_IV, SHA256_DIGEST_LENGTH);
    // set all state byte to 0
    core_mm_set(sha256_ctx->state, 0, SHA256_STATE_LENGTH);
    // set count to 0
    core_mm_set(sha256_ctx->count, 0, 8);
}

u1 core_algo_sha256Update(struct SHA256_CONTEXT *sha256_ctx, u1 *buf, u2 length)
{
    u2 i;
    u1 data_count;
    u4 count1, count2;

    data_count = (u1)((core_dcm_readBigu2(sha256_ctx->count+6) >> 3) & 0x3F);
    count1 = core_dcm_readBigu4(sha256_ctx->count);
    count2 = core_dcm_readBigu4(sha256_ctx->count+4);

    for (i=0; i<length; i++)
    {
        sha256_ctx->state[data_count++] = buf[i];

        count2 += 8;
        if (count2 == 0)
        {
            // check overflow, max is 2^64 - 1
            if (count1 == 0xFFFFFFFF)
            {
                return false;
            }
            count1++;
        }
        
        // if state is full, update the digest
        if (data_count == SHA256_STATE_LENGTH)
        {
            sha256Hash(sha256_ctx->state, sha256_ctx->digest);
            data_count = 0;
        }
    }

    core_dcm_writeBigu4(sha256_ctx->count, count1);
    core_dcm_writeBigu4(sha256_ctx->count+4, count2);

    return true;
}

void core_algo_sha256Final(struct SHA256_CONTEXT *sha256_ctx)
{
    u1 data_count;

    data_count = (u1)((core_dcm_readBigu2(sha256_ctx->count+6) >> 3) & 0x3F);
    sha256_ctx->state[data_count] = 0x80;
    
    // if state is full, update the digest
    if (data_count > 55)
    {
        sha256Hash(sha256_ctx->state, sha256_ctx->digest);
    }

    core_mm_copy(sha256_ctx->state+56, sha256_ctx->count, 8);

    sha256Hash(sha256_ctx->state, sha256_ctx->digest);
}

int simp_sha256_md(const unsigned char *input, unsigned int ilen, unsigned char *output)
{
    int ret = 0;
    struct SHA256_CONTEXT sha256_ctx;
    
    sha256_ctx.digest = digest;
    sha256_ctx.state = state;
    sha256_ctx.count = count;

    core_algo_sha256Initial(&sha256_ctx);
    core_algo_sha256Update(&sha256_ctx, (unsigned char *)input, ilen);
    core_algo_sha256Final(&sha256_ctx);

    memcpy(output, sha256_ctx.digest, 32);
    return    ret;
}
