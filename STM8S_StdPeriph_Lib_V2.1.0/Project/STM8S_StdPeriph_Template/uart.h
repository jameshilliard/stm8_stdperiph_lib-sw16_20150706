#ifndef _uart_h_
#define _uart_h_

#include "main.h"

void UART_Initial(void);

void UART2_SendString(u8* Data);
void Uart_Print_Num(unsigned int num,unsigned char m);
void UART_INTERRUPT(void);
void UART_SendBuf(u8 *Data);

#endif
