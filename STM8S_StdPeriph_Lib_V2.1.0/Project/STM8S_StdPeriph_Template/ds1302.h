#ifndef _ds1302_h_
#define _ds1302_h_

#include "main.h"

#define uchar unsigned char
#define uint unsigned int

#define T_CLK_0       GPIOC->ODR &= (uint8_t)(~GPIO_PIN_2)
#define T_CLK_1      GPIOC->ODR |= (uint8_t)GPIO_PIN_2

#define T_RST_0       GPIOC->ODR &= (uint8_t)(~GPIO_PIN_4)
#define T_RST_1      GPIOC->ODR |= (uint8_t)GPIO_PIN_4

#define T_IO_0       GPIOC->ODR &= (uint8_t)(~GPIO_PIN_3)
#define T_IO_1      GPIOC->ODR |= (uint8_t)GPIO_PIN_3

#define T_IO   ((BitStatus)(GPIOC->IDR & (uint8_t)GPIO_PIN_3))

#define T_IO_OUT   GPIO_Init(GPIOC,GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST)
#define T_IO_IN   GPIO_Init(GPIOC,GPIO_PIN_3, GPIO_MODE_IN_PU_NO_IT)

#define SECOND 0X80
#define MINUTE 0X82
#define HOUR   0X84
#define DAY    0X86
#define MONTH  0X88
#define YEAR   0X8C
#define WEEK   0X8A

void ds1302_Initial(void);
void Set(uchar sel,uchar sel_1);
void DelayMs(void);
void SetByte(uchar d);
uchar GetByte(void);
void W1302(uchar ucAddr, uchar ucDa);
uchar R1302(uchar ucAddr);
void Set1302(uchar *pClock) ;
void du1302();

#endif

