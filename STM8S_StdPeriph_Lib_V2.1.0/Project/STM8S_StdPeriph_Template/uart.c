#include "uart.h"

u8 uart_len=0;
u8 uart_buf[20];

void UART_Initial(void)
{
  UART2_DeInit();
  UART2_Init(115200,UART2_WORDLENGTH_8D,UART2_STOPBITS_1,UART2_PARITY_NO,UART2_SYNCMODE_CLOCK_DISABLE,UART2_MODE_TXRX_ENABLE);
  UART2_ITConfig(UART2_IT_RXNE_OR, ENABLE);
  UART2_Cmd(ENABLE);
}

void UART2_SendString(u8* Data)
{
    while(*Data!='\0')
    {	
        while((UART2->SR&0x80)==0x00);
        UART2_SendData8(*Data);  
        Data++;
    }
}

void Uart_Print_Num(unsigned int num,unsigned char m)
{
unsigned char k,n;
unsigned char i=0;
unsigned char a[8]={0,0,0,0,0,0,0,0};
i=7;
/*将输入的数据分解开来，以便输出显示*/
while(num!=0)
{
  k=num%10;//取最低位
  a[i]=k;//写入的时候是先把低位放在数组的第一位，
  num=num/10;
  i--;
}
//============================================
  n=m;
    for(k=0;k<n;k++)
    {
//       USART_SendData8(a[8-m]+'0');
      while((UART2->SR&0x80)==0x00);
       UART2_SendData8(a[8-m]+'0');
       m--;
    }
}

void UART_SendBuf(u8 *Data)
{
    u8 i=0;
    for(i=0;i<20;i++)
    {
      while((UART2->SR&0x80)==0x00);
      UART2_SendData8(*(Data+i));  
    }              
}

extern u8 gk;
extern u8 gCount;

void UART_INTERRUPT(void)
{
  u8 x=0;
  gCount=0;
  gk=0;
  
  if (UART2->SR) 
    {                                   // If reception occur 
        UART2->SR=0;                          // Clear reception flag for next reception
            x=(u8)UART2->DR;
//                UART2->DR=x;
            if(0 == uart_len){
                    if(0xaa == x){
                            uart_buf[0]=x;
                            uart_len=1;
                    }
                    else{
                            uart_len=0;
                    }
            }
            else if(sizeof(uart_buf) > uart_len){
                    uart_buf[uart_len++]=x;
            }
		
    }
}
  