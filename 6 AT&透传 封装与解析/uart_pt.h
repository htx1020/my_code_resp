/*
 * uart_pt.h
 *
 *  Created on: Nov 22, 2018
 *      Author: Sail
 */

#ifndef UART_PT_H_
#define UART_PT_H_
#include "util_int.h"

extern uint8_t gUartFrame[255];
extern uint8_t gUartFrameLen;

extern uint8_t KitFrameCapture(uint8_t SrcData);

#endif /* UART_PT_H_ */
