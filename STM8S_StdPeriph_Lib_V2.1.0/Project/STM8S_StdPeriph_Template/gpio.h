#ifndef _stm8s_h_
#define _stm8s_h_

#include "main.h"


#define RL1_On( )       GPIOG->ODR &= (u8)(~GPIO_PIN_0);
#define RL1_Off( )      GPIOG->ODR |= GPIO_PIN_0;

#define RL2_On( )       GPIOB->ODR &= (u8)(~GPIO_PIN_0);
#define RL2_Off( )      GPIOB->ODR |= GPIO_PIN_0;

#define RL3_On( )       GPIOG->ODR &= (u8)(~GPIO_PIN_1);
#define RL3_Off( )      GPIOG->ODR |= GPIO_PIN_1;

#define RL4_On( )       GPIOB->ODR &= (u8)(~GPIO_PIN_1);
#define RL4_Off( )      GPIOB->ODR |= GPIO_PIN_1;

#define RL5_On( )       GPIOE->ODR &= (u8)(~GPIO_PIN_0);
#define RL5_Off( )      GPIOE->ODR |= GPIO_PIN_0;

#define RL6_On( )       GPIOB->ODR &= (u8)(~GPIO_PIN_2);
#define RL6_Off( )      GPIOB->ODR |= GPIO_PIN_2;

#define RL7_On( )       GPIOD->ODR &= (u8)(~GPIO_PIN_0);
#define RL7_Off( )      GPIOD->ODR |= GPIO_PIN_0;

#define RL8_On( )       GPIOB->ODR &= (u8)(~GPIO_PIN_3);
#define RL8_Off( )      GPIOB->ODR |= GPIO_PIN_3;

#define RL9_On( )       GPIOD->ODR &= (u8)(~GPIO_PIN_2);
#define RL9_Off( )      GPIOD->ODR |= GPIO_PIN_2;

#define RLA_On( )       GPIOB->ODR &= (u8)(~GPIO_PIN_6);
#define RLA_Off( )      GPIOB->ODR |= GPIO_PIN_6;

#define RLB_On( )       GPIOD->ODR &= (u8)(~GPIO_PIN_3);
#define RLB_Off( )      GPIOD->ODR |= GPIO_PIN_3;

#define RLC_On( )       GPIOB->ODR &= (u8)(~GPIO_PIN_7);
#define RLC_Off( )      GPIOB->ODR |= GPIO_PIN_7;

#define RLD_On( )       GPIOD->ODR &= (u8)(~GPIO_PIN_4);
#define RLD_Off( )      GPIOD->ODR |= GPIO_PIN_4;

#define RLE_On( )       GPIOA->ODR &= (u8)(~GPIO_PIN_4);
#define RLE_Off( )      GPIOA->ODR |= GPIO_PIN_4;

#define RLF_On( )       GPIOD->ODR &= (u8)(~GPIO_PIN_7);
#define RLF_Off( )      GPIOD->ODR |= GPIO_PIN_7;

#define RL0_On( )       GPIOA->ODR &= (u8)(~GPIO_PIN_5);
#define RL0_Off( )      GPIOA->ODR |= GPIO_PIN_5;

/*turn on the LED*/
#define LED_Off( )       GPIOE->ODR &= (u8)(~GPIO_PIN_7);

/*turn off the LED*/
#define LED_On( )      GPIOE->ODR |= GPIO_PIN_7;

/*toggle the LED*/
#define LED_Toggle( )   GPIOE->ODR ^= GPIO_PIN_7;

void GPIO_Initial( void );
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint8_t GPIO_Pin);
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint8_t GPIO_Pin);
void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint8_t GPIO_Pin);

#endif
