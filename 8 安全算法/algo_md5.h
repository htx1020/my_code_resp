/**
 * \file md5.h
 *
 * \brief MD5 message digest algorithm (hash function)
 *
 * \warning   MD5 is considered a weak message digest and its use constitutes a
 *            security risk. We recommend considering stronger message
 *            digests instead.
 */
#ifndef ALGO_MD5_H
#define ALGO_MD5_H

#include <stddef.h>
#include <stdint.h>


#define ALGO_ERR_MD5_HW_ACCEL_FAILED                   -0x002F  /**< MD5 hardware accelerator failed */

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \brief          MD5 context structure
 *
 * \warning        MD5 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
typedef struct algo_md5_context
{
    unsigned int total[2];          /*!< number of bytes processed  */
    unsigned int state[4];          /*!< intermediate digest state  */
    unsigned char buffer[64];   /*!< data block being processed */
}
algo_md5_context;

/**
 * \brief          Output = MD5( input buffer )
 *
 * \param input    buffer holding the data
 * \param ilen     length of the input data
 * \param output   MD5 checksum result
 *
 * \return         0 if successful
 *
 * \warning        MD5 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
int ingeek_md5_md(const unsigned char *input, unsigned int ilen, unsigned char *output);

#ifdef __cplusplus
}
#endif

#endif /* algo_md5.h */
