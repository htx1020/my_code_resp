/*
 * debug.h
 *
 *  Created on: 2019年9月11日
 *      Author: Huang
 */

#ifndef INGEEK_HSM_DEBUG_H_
#define INGEEK_HSM_DEBUG_H_

#include <stdint.h>

#define ZEROPAD		            1	   /* pad with zero */
#define SIGN		              2	   /* unsigned/signed long */
#define PLUS   		            4	   /* show plus */
#define SPACE  		            8	   /* space if plus */
#define LEFT   		            16	 /* left justified */
#define SPECIAL    	          32	 /* 0x */
#define LARGE  		            64	 /* use 'ABCDEF' instead of 'abcdef' */
#define DEBUG_BUFF_CNT        256
#define DEBUG_UART  0

typedef int(*cb_xx_buff_sent_t)(unsigned char *pdata,unsigned int length);

int log_printf_int(cb_xx_buff_sent_t buff_sent);
void log_printf(const char *fmt_s, ...);
void hex_dump(void* name, uint8_t* str, uint32_t len);

#endif /* INGEEK_HSM_DEBUG_H_ */
