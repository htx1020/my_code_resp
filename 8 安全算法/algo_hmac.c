#include "config.h"
#include "core.h"

#define HMAC_IPAD        0x36
#define HMAC_OPAD        0x5C

void core_algo_xor(const u1 *buf1, const u1 *buf2, u1 *outBuf, u2 length)
{
    while (length--)
    {
        *outBuf = (*buf1) ^ (*buf2);
        buf1++;
        buf2++;
        outBuf++;
    }
}

void core_algo_hmacInitial(struct SHA256_CONTEXT *md_ctx, u1 *key, u2 key_len, u1 *k_pad0, u1 *k_pad1)
{
    if(key_len > HMAC_MD_SLEN)
    {
        core_algo_sha256Initial(md_ctx);
        core_algo_sha256Update(md_ctx, key, key_len);
        core_algo_sha256Final(md_ctx);
        key = md_ctx->digest;
        key_len = HMAC_MD_DLEN;
    }
    
    // set HMAC_IPAD and HMAC_OPAD
    core_mm_set(k_pad0, HMAC_IPAD, HMAC_MD_SLEN);
    core_mm_set(k_pad1, HMAC_OPAD, HMAC_MD_SLEN);

    // do xor
    core_algo_xor(key, k_pad0, k_pad0, key_len);
    core_algo_xor(key, k_pad1, k_pad1, key_len);

    core_algo_sha256Initial(md_ctx);
    core_algo_sha256Update(md_ctx, k_pad0, HMAC_MD_SLEN);
}

u1 core_algo_hmacUpdate(struct SHA256_CONTEXT *md_ctx, u1 *buf, u2 length)
{
    if (length != 0)
    {
        return core_algo_sha256Update(md_ctx, buf, length);
    }
    
    return true;
}

void core_algo_hmacFinal(struct SHA256_CONTEXT *md_ctx, u1 *k_pad1)
{
    u1 digest[MAX_MD_DIGEST_LEN];

    core_algo_sha256Final(md_ctx);
    core_mm_copy(digest, md_ctx->digest, HMAC_MD_DLEN);

    core_algo_sha256Initial(md_ctx);
    core_algo_sha256Update(md_ctx, k_pad1, HMAC_MD_SLEN);
    core_algo_sha256Update(md_ctx, digest, HMAC_MD_DLEN);
    core_algo_sha256Final(md_ctx);
}

extern u1 digest[SHA256_DIGEST_LENGTH];    // message digest
extern u1 state[SHA256_STATE_LENGTH];        // data buffer
extern u1 count[MAX_SHA_256_SUUPORT_BIT_COUNT];    // count of bits

int simp_hmac_sha256(const unsigned char *key, unsigned int keylen,const unsigned char *input, unsigned int ilen,unsigned char *output)
{
    struct SHA256_CONTEXT md_ctx;
    u1 k_pad0[HMAC_MD_SLEN], k_pad1[HMAC_MD_SLEN];
    
    md_ctx.digest = digest;
    md_ctx.state = state;
    md_ctx.count = count;
    core_algo_hmacInitial(&md_ctx, (unsigned char *)key, keylen, k_pad0, k_pad1);
    core_algo_hmacUpdate(&md_ctx, (unsigned char *)input, ilen);
    core_algo_hmacFinal(&md_ctx, k_pad1);
    core_mm_copy(output, md_ctx.digest, HMAC_MD_DLEN);
    return 0;
}
