#include <stdint.h>
#include <string.h>
#include "aes_base.h"
#include "algo_api.h"

unsigned char temp_buff[288];
int simp_aes_cbc_cipher_nopadding(const unsigned char* key, const unsigned char*iv, const unsigned char*in, unsigned int length,
                                    unsigned char*out, unsigned char mode)
{
    int ret = 0;

    if (key == NULL || iv == NULL || in == NULL || length == 0)
    {
        return -1;
    }
    if (length % 16 != 0)
    {
        return -2;
    }
    memcpy(temp_buff, in, length);
    if (mode)
    {
        AES_CBC_encrypt_buffer(out, temp_buff, length, key, iv);
        ret = 0;
    }
    else
    {
        AES_CBC_decrypt_buffer(out, temp_buff, length, key, iv);
        ret = 0;
    }
    return ret;
}
int simp_aes_ecb_cipher_nopadding(const unsigned char* key, const unsigned char*in, unsigned int length,
                                    unsigned char*out, unsigned char mode)

{
    unsigned int temp = 0;

    if (key == NULL || in == NULL || length == 0)
    {
        return -1;
    }
    if (length % 16 != 0)
    {
        return -2;
    }
    memcpy(temp_buff, in, length);
    if (mode)
    {
        for (temp = 0; temp < length / 16; temp++)
            AES_ECB_encrypt(temp_buff + temp * 16, key, out + temp * 16, 16);
    }
    else
    {
        for (temp = 0; temp < length / 16; temp++)
            AES_ECB_decrypt(temp_buff + temp * 16, key, out + temp * 16, 16);
    }
    return 0;
}
