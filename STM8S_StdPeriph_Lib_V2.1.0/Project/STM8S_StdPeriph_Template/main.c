/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    18-November-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/

#include "main.h"
/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
    
__no_init __eeprom unsigned char eeprom_num @0x4002;
__no_init __eeprom unsigned char eeprom_DiandongOrDingshi[16] @0x4020; //用来记录按钮是用作定时还是点动的
__no_init __eeprom unsigned char eeprom_diandongtime[16] @0x4010;  //这个是用来记录点动时间的
__no_init __eeprom unsigned char eeprom_Timing[16][30] @0x4030;

#define diandongtime 0x4010
#define DiandongOrDingshi 0x4020
#define Timing 0x4030

void WriteEEPROM(uint32_t addr,uint8_t dat);
static void TIM4_Config(void);
u8 CheckSum(u8 *a);
void OpenOrCloseRelay(u8 Relay, u8 status);
void All_OPEN(void);    
void All_CLOSE(void);

void SetTime_Respond(void);
void All_OPEN_Respond(u8 *a);
void DianDongTime_Respond(void);
void Timing_Respond(void);
void DianDongOrDingshi_Respond(void);
void UpdateTime_Respond(u8 *a);

void SendData(u8 *a);
void ClearData(u8 *a);
u8 CheckSum(u8 *a);

u8 gCount=0;
u8 gk=0;

extern u8 uart_len;
extern u8 uart_buf[20];
extern uchar starts_time[7];
               //  0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19
u8 send_buf[20]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  
void main(void)
{
  /* Infinite loop */
  u8 i=0;
  u8 i1=0;
  u16 k=0;
  u8 Sec=0; 
  u8 Min=0;
  u8 Hour=0;
  u8 Day=0;
  u8 Month=0;
  u8 Week=0;
  u8 Year=0;
  u8 TimeoutFlag=0;
    
  u8 b=0;
  u16 count=0;
  u16 time=0;
  
  u8 DiandongOrDingshiFlag=0;
                    //  0     1    2    3    4    5    6    7    8    9   10   11   12   13   14   15
  u8 diandong_buf[16]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
              //  0     1    2    3    4    5    6    7    8    9   10   11   12   13   14   15
  u8 status[16]={0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02};
               
  
  
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);//振荡器1 分频 16M
  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);//CPU时钟为主时钟1分频  16M

  GPIO_Initial();
  UART_Initial();
  TIM4_Config();
  
  ds1302_Initial();
  WriteEEPROM((uint32_t)0x4002,0x0a);
  Uart_Print_Num(eeprom_num,3);
  UART2_SendString("\r\n程序开始\r\n");
  
  enableInterrupts();
  
//  while(1)
//  {
//
//    k++;
//    if(k==100)
//    {
//      k=0;
//      
//      LED_Toggle();
//      b=eeprom_num;
//      
//      if(b==0x0a)
//      {
//        count++;
//        Uart_Print_Num(count,6);UART2_SendString("--");Uart_Print_Num(time,6);UART2_SendString("\r\n");
//        b=0;
//        
//        if(count==60000)
//        {
//          count=0;
//          time++;
//          if(time==60000)
//          {
//            time=0;
//          }
//         
//        }
//        WriteEEPROM((uint32_t)0x4002,0x0a);
//      }
//      else
//      {
//        UART2_SendString("\r\n数据错误\r\n");
//        while(1);
//      }
//    }
//    
//  }
  
//  W1302(0x8e,0x00);W1302(HOUR,10);W1302(MINUTE,18);W1302(SECOND,35);W1302(0x8e,0x80); 
  
  while (1)
  {
    k++;
    if(k==47)  //1s时间
    {
      k=0;
      LED_Toggle();
      
      Sec=R1302(SECOND);
      Min=R1302(MINUTE);
      Hour=R1302(HOUR);
      Day=R1302(DAY);
      Month=R1302(MONTH);
      Week=R1302(WEEK);
      Year=R1302(YEAR);	
      
      for(i=0;i<16;i++)
      {
        if(eeprom_DiandongOrDingshi[i]==DIANDONG)
        {
//          Uart_Print_Num(i,3);
//          UART2_SendString("--点动\r\n");
          
            if(diandong_buf[i]!=0)
            {
              diandong_buf[i]--;
              if(!diandong_buf[i])
              {
                TimeoutFlag=1;
                status[i]=CLOSE;
                OpenOrCloseRelay(i,CLOSE);
              }
            }
        }
        else if(eeprom_DiandongOrDingshi[i]==DINGSHI)
        {
          //在这比较是时间是否到
//          Uart_Print_Num(i,3);
//          UART2_SendString("--定时\r\n");
          for(i1=0;i1<5;i1++)
          {
            if((eeprom_Timing[i][2+i1*6]&(0X01<<Week))!=0)//某一天有定时开
            {
//              UART2_SendString("\r\n今天有定时开\r\n");
              if((Hour==eeprom_Timing[i][0+i1*6])&&(Min==eeprom_Timing[i][1+i1*6])&&(Sec==0)) //定时是否到
              {
                //打开
                OpenOrCloseRelay( i, OPEN );
                status[i]=OPEN;
                TimeoutFlag=1;
              }
            }
            
            if((eeprom_Timing[i][5+i1*6]&(0X01<<Week))!=0)//某一天有定时关
            {
//              UART2_SendString("\r\n今天有定时关\r\n");
              if((Hour==eeprom_Timing[i][3+i1*6])&&(Min==eeprom_Timing[i][4+i1*6])&&(Sec==0)) //定时是否到
              {
                //关闭
                OpenOrCloseRelay( i, CLOSE );
                status[i]=CLOSE;
                TimeoutFlag=1;
              }
            }
            
          }
          
          
          
        }
      }
      
     
     /*
      UART2_SendString("\r\n");Uart_Print_Num(Year,2);UART2_SendString("/");Uart_Print_Num(Month,2);UART2_SendString("/");Uart_Print_Num(Day,2);
      UART2_SendString("-");
      Uart_Print_Num(Week,2);
      UART2_SendString("-");
      Uart_Print_Num(Hour,2);UART2_SendString(":");Uart_Print_Num(Min,2);UART2_SendString(":");Uart_Print_Num(Sec,2); */
      
      ClearData(send_buf);
      send_buf[1]=0x1f;
      
      send_buf[2]=Year;
      send_buf[3]=Month;
      send_buf[4]=Day;
      send_buf[5]=Hour;
      send_buf[6]=Min;
      send_buf[7]=Sec;
      send_buf[8]=Week;
      
      SendData(send_buf);
      
    }
    
    if(TimeoutFlag)
    {
      TimeoutFlag=0;
      All_OPEN_Respond(status);
    }
    if(sizeof(uart_buf) <= uart_len){
      uart_len=0;
//      Uart_Print_Num(sizeof(uart_buf),3);
//      UART2_SendString("\r\n串口接收到正确的数据\r\n");
      if((uart_buf[0]==0xaa)&&(uart_buf[19]==0xbb))
      {
        if(uart_buf[1]==0x0a)  //如果是全开
        {
          All_OPEN();
          for(i=0;i<16;i++)
          {
            status[i]=0x01;
            diandong_buf[i]=eeprom_diandongtime[i];
          }
          All_OPEN_Respond(status);
        }
        if(uart_buf[1]==0x0b)   //如果是全关
        {
          All_CLOSE();
          for(i=0;i<16;i++)
          {
            status[i]=0x02;
          }
          All_OPEN_Respond(status);
        }
        if(uart_buf[1]==0x1e)   //如果是查询
        {
         
          
          All_OPEN_Respond(status);
        }
        if(uart_buf[1]==0x0d)//设置时间
        {
          for(i=0;i<7;i++)
          {
            starts_time[i]=uart_buf[i+2];
            Set1302(starts_time);
            
          }
          SetTime_Respond();
          
        }
        if(uart_buf[1]==0x12)//查询时间
        {
          SetTime_Respond();
        }
        if(uart_buf[1]==0x0f) //单开
        {
          OpenOrCloseRelay( uart_buf[2],uart_buf[3] );
          status[uart_buf[2]]=uart_buf[3];
          diandong_buf[uart_buf[2]]=eeprom_diandongtime[uart_buf[2]];
          All_OPEN_Respond(status);
        }
        if(uart_buf[1]==0x10) //单关
        {
          OpenOrCloseRelay( uart_buf[2],uart_buf[3] );
          status[uart_buf[2]]=uart_buf[3];
          All_OPEN_Respond(status);
        }
        if(uart_buf[1]==0x13) //设置点动时间,点动时间是一个一个设置的
        {
          
          WriteEEPROM((uint32_t)(diandongtime+uart_buf[2]),uart_buf[3]);
          diandong_buf[uart_buf[2]]=eeprom_diandongtime[uart_buf[2]];
          
//          eeprom_DiandongOrDingshi[uart_buf[2]]=DIANDONG;
          WriteEEPROM((uint32_t)(DiandongOrDingshi+uart_buf[2]),DIANDONG);  //把某个按钮设置为点动
          //设置点动的时候是一个就把点动标志和点动时间一次就设置进去了，但设置定时的时候定时标志和定时时间是分开的
          
          ClearData(send_buf); 
          send_buf[1]=0x2c;
          send_buf[2]=uart_buf[2];
          send_buf[3]=uart_buf[3];
          send_buf[4]=uart_buf[4];
          SendData(send_buf);
        }
        if(uart_buf[1]==0x15) //查询点动时间
        {
          DianDongTime_Respond();
        }
        
        if(uart_buf[1]==0x1A) //查询点动还是定时
        {
          DianDongOrDingshi_Respond();
        }
        
        if(uart_buf[1]==0x1C) //查询点动还是定时
        {
          
          ClearData(send_buf);
          send_buf[1]=0x1D;
          for(i=0;i<5;i++)
          {
            send_buf[i+3]=eeprom_Timing[uart_buf[2]][2+i*6];
          }
          for(i=0;i<5;i++)
          {
            send_buf[8+i]=eeprom_Timing[uart_buf[2]][5+i*6];
          }
          SendData(send_buf);
        }
        
        if(uart_buf[1]==0x16) //添加定时开关时间
        {
          
          //是添加定时开还是定时关
          if((uart_buf[5]!=0)&&(uart_buf[8]!=0))//有定时开且有定时关
          {
            for(i=0;i<5;i++)
            {
              if((eeprom_Timing[uart_buf[2]][2+i*6]==0)&&(eeprom_Timing[uart_buf[2]][5+i*6]==0))//寻找空位置
              {
//                eeprom_timing[uart_buf[2]][0+i*6]=uart_buf[3];
//                eeprom_timing[uart_buf[2]][1+i*6]=uart_buf[4];
//                eeprom_timing[uart_buf[2]][2+i*6]=uart_buf[5];
//                eeprom_timing[uart_buf[2]][3+i*6]=uart_buf[6];
//                eeprom_timing[uart_buf[2]][4+i*6]=uart_buf[7];
//                eeprom_timing[uart_buf[2]][5+i*6]=uart_buf[8];
                
//                WriteEEPROM((uint32_t)(Timing+uart_buf[2]*30),uart_buf[3]);
                
                WriteEEPROM((uint32_t)(Timing+uart_buf[2]*30+i*6+0),uart_buf[3]);
                WriteEEPROM((uint32_t)(Timing+uart_buf[2]*30+i*6+1),uart_buf[4]);
                WriteEEPROM((uint32_t)(Timing+uart_buf[2]*30+i*6+2),uart_buf[5]);
                
                WriteEEPROM((uint32_t)(Timing+uart_buf[2]*30+i*6+3),uart_buf[6]);
                WriteEEPROM((uint32_t)(Timing+uart_buf[2]*30+i*6+4),uart_buf[7]);
                WriteEEPROM((uint32_t)(Timing+uart_buf[2]*30+i*6+5),uart_buf[8]);
                
                //WriteEEPROM((uint32_t)(DiandongOrDingshi+uart_buf[2]),DINGSHI);//把某个按钮这种为定时
                //添加成功响应
                ClearData(send_buf); 
                send_buf[1]=0x18;
                send_buf[2]=uart_buf[2];
                send_buf[3]=eeprom_Timing[uart_buf[2]][0+i*6];
                send_buf[4]=eeprom_Timing[uart_buf[2]][1+i*6];
                send_buf[5]=eeprom_Timing[uart_buf[2]][2+i*6];
                send_buf[6]=eeprom_Timing[uart_buf[2]][3+i*6];
                send_buf[7]=eeprom_Timing[uart_buf[2]][4+i*6];
                send_buf[8]=eeprom_Timing[uart_buf[2]][5+i*6];
                SendData(send_buf);
                break;//添加成功则直接跳出
              }
                
            }
            
            
          }
          
//          Timing_Respond();
        }
        
        if(uart_buf[1]==0x17) //删除按钮定时
        {
          if((uart_buf[5]!=0)&&(uart_buf[8]!=0))//有定时开且有定时关
          {
            for(i=0;i<5;i++)
            {
              if((eeprom_Timing[uart_buf[2]][2+i*6]==uart_buf[5])&&(eeprom_Timing[uart_buf[2]][5+i*6]==uart_buf[8]))//寻找对应位置
              {
                if((eeprom_Timing[uart_buf[2]][0+i*6]==uart_buf[3])&&(eeprom_Timing[uart_buf[2]][3+i*6]==uart_buf[6]))//寻找时相同的
                {
                  if((eeprom_Timing[uart_buf[2]][1+i*6]==uart_buf[4])&&(eeprom_Timing[uart_buf[2]][4+i*6]==uart_buf[7]))//寻找分相同的
                  {
                    WriteEEPROM((uint32_t)(Timing+uart_buf[2]*30+i*6+0),0);
                    WriteEEPROM((uint32_t)(Timing+uart_buf[2]*30+i*6+1),0);
                    WriteEEPROM((uint32_t)(Timing+uart_buf[2]*30+i*6+2),0);
                    WriteEEPROM((uint32_t)(Timing+uart_buf[2]*30+i*6+3),0);
                    WriteEEPROM((uint32_t)(Timing+uart_buf[2]*30+i*6+4),0);
                    WriteEEPROM((uint32_t)(Timing+uart_buf[2]*30+i*6+5),0);
                    //添加成功响应
                    ClearData(send_buf); 
                    send_buf[1]=0x19;
                    send_buf[2]=uart_buf[2];
                    send_buf[3]=eeprom_Timing[uart_buf[2]][0+i*6];
                    send_buf[4]=eeprom_Timing[uart_buf[2]][1+i*6];
                    send_buf[5]=eeprom_Timing[uart_buf[2]][2+i*6];
                    send_buf[6]=eeprom_Timing[uart_buf[2]][3+i*6];
                    send_buf[7]=eeprom_Timing[uart_buf[2]][4+i*6];
                    send_buf[8]=eeprom_Timing[uart_buf[2]][5+i*6];
                    SendData(send_buf);
                    break;
                  }
                }
              }
            }
          }
        }
        
        if(uart_buf[1]==0x21) //设置定时开时间
        {
          
           //添加定时开
          if(uart_buf[5]!=0)//有定时开
          {
            for(i=0;i<5;i++)
            {
              if(eeprom_Timing[uart_buf[2]][2+i*6]==0)//寻找空位置
              {

                WriteEEPROM((uint32_t)(Timing+uart_buf[2]*30+i*6+0),uart_buf[3]);
                WriteEEPROM((uint32_t)(Timing+uart_buf[2]*30+i*6+1),uart_buf[4]);
                WriteEEPROM((uint32_t)(Timing+uart_buf[2]*30+i*6+2),uart_buf[5]);
                
                //WriteEEPROM((uint32_t)(Timing+uart_buf[2]*30+i*6+3),uart_buf[6]);
                //WriteEEPROM((uint32_t)(Timing+uart_buf[2]*30+i*6+4),uart_buf[7]);
                //WriteEEPROM((uint32_t)(Timing+uart_buf[2]*30+i*6+5),uart_buf[8]);

                //添加成功响应
                ClearData(send_buf); 
                send_buf[1]=0x22;
                send_buf[2]=uart_buf[2];
                send_buf[3]=eeprom_Timing[uart_buf[2]][0+i*6];
                send_buf[4]=eeprom_Timing[uart_buf[2]][1+i*6];
                send_buf[5]=eeprom_Timing[uart_buf[2]][2+i*6];
                send_buf[6]=i;
                //send_buf[6]=eeprom_Timing[uart_buf[2]][3+i*6];
                //send_buf[7]=eeprom_Timing[uart_buf[2]][4+i*6];
                //send_buf[8]=eeprom_Timing[uart_buf[2]][5+i*6];
                SendData(send_buf);
                break;//添加成功则直接跳出
              }
                
            }
            if(i==5)
            {
                ClearData(send_buf); 
                send_buf[1]=0x25;
                send_buf[2]=uart_buf[2];
                SendData(send_buf);
            }
            
          }
        }
        
        
        if(uart_buf[1]==0x23) //设置定时关时间
        {
           //添加定时开
          if(uart_buf[5]!=0)//有定时开
          {
            for(i=0;i<5;i++)
            {
              if(eeprom_Timing[uart_buf[2]][5+i*6]==0)//寻找空位置
              {


                WriteEEPROM((uint32_t)(Timing+uart_buf[2]*30+i*6+3),uart_buf[3]);
                WriteEEPROM((uint32_t)(Timing+uart_buf[2]*30+i*6+4),uart_buf[4]);
                WriteEEPROM((uint32_t)(Timing+uart_buf[2]*30+i*6+5),uart_buf[5]);

                //添加成功响应
                ClearData(send_buf); 
                send_buf[1]=0x24;
                send_buf[2]=uart_buf[2];

                send_buf[3]=eeprom_Timing[uart_buf[2]][3+i*6];
                send_buf[4]=eeprom_Timing[uart_buf[2]][4+i*6];
                send_buf[5]=eeprom_Timing[uart_buf[2]][5+i*6];
                
                send_buf[6]=i;
                
                SendData(send_buf);
                break;//添加成功则直接跳出
              }
                
            }
            if(i==5)
            {
               ClearData(send_buf); 
                send_buf[1]=0x25;
                send_buf[2]=uart_buf[2];
                SendData(send_buf);
            }
            
          }
        }
        
        
        if(uart_buf[1]==0x26) //设置为点动或定时
        {
          WriteEEPROM((uint32_t)(DiandongOrDingshi+uart_buf[2]),uart_buf[3]);
          
          ClearData(send_buf); 
          send_buf[1]=0x27;
          send_buf[2]=uart_buf[2];
          send_buf[3]=uart_buf[3];
          SendData(send_buf);
                
        }
        
        if(uart_buf[1]==0x28) //删除定时开时间
        {
          int i1=0;
          i1=uart_buf[6];
            if(eeprom_Timing[uart_buf[2]][0+i1*6]==uart_buf[3])//寻找时相同的
            {
              if(eeprom_Timing[uart_buf[2]][1+i1*6]==uart_buf[4])//寻找分相同的
              {
                  WriteEEPROM((uint32_t)(Timing+uart_buf[2]*30+i1*6+0),0);
                  WriteEEPROM((uint32_t)(Timing+uart_buf[2]*30+i1*6+1),0);
                  WriteEEPROM((uint32_t)(Timing+uart_buf[2]*30+i1*6+2),0);
                
                  ClearData(send_buf); 
                  send_buf[1]=0x29;
                  send_buf[2]=uart_buf[2];
                  send_buf[6]=uart_buf[6];
                  SendData(send_buf);
              }
            } 
        }
         if(uart_buf[1]==0x2a) //删除定时开时间
        {
          int i1=0;
          i1=uart_buf[6];
            if(eeprom_Timing[uart_buf[2]][3+i1*6]==uart_buf[3])//寻找时相同的
            {
              if(eeprom_Timing[uart_buf[2]][4+i1*6]==uart_buf[4])//寻找分相同的
              {
                  WriteEEPROM((uint32_t)(Timing+uart_buf[2]*30+i1*6+3),0);
                  WriteEEPROM((uint32_t)(Timing+uart_buf[2]*30+i1*6+4),0);
                  WriteEEPROM((uint32_t)(Timing+uart_buf[2]*30+i1*6+5),0);
                
                  ClearData(send_buf); 
                  send_buf[1]=0x2b;
                  send_buf[2]=uart_buf[2];
                  send_buf[6]=uart_buf[6];
                  SendData(send_buf);
              }
            } 
        }
        
        
        
        
        if(uart_buf[1]==0x2d) //查询定时开时间
        {
          ClearData(send_buf); 
          for(i=0;i<5;i++)
          {
            send_buf[i*3+3]=eeprom_Timing[uart_buf[2]][0+i*6];
            send_buf[i*3+4]=eeprom_Timing[uart_buf[2]][1+i*6];
            send_buf[i*3+5]=eeprom_Timing[uart_buf[2]][2+i*6];
          }
          send_buf[1]=0x2e;
          send_buf[2]=uart_buf[2];
          SendData(send_buf);

          ClearData(send_buf); ///////////////////
          for(i=0;i<5;i++)
          {
            send_buf[i*3+3]=eeprom_Timing[uart_buf[2]][3+i*6];
            send_buf[i*3+4]=eeprom_Timing[uart_buf[2]][4+i*6];
            send_buf[i*3+5]=eeprom_Timing[uart_buf[2]][5+i*6];
          }
          send_buf[1]=0x2f;
          send_buf[2]=uart_buf[2];
          SendData(send_buf);
          
          DianDongTime_Respond();
          
          DianDongOrDingshi_Respond();
        }
   
        
        
        
        
      }
    }
//    if(k==60000)
//    {
//      Sec=R1302(0x81);
//      Sec=Sec/16*10+Sec%16;
//      
////      UART2_SendString("\r\n");
////      
////      Uart_Print_Num(Sec,3);
//      k=0;
//      LED_Toggle();
//    }
    
    
    gk++;
    if(gk==5)
    { gk=0; uart_len=0; }
    
     while(1)
    {
      if(gCount>20){gCount=0;break;}
    }
  }
}
void DianDongOrDingshi_Respond(void)
{
  u8 i=0;
  ClearData(send_buf);
  send_buf[1]=0x1b;
  for(i=0;i<16;i++)
  {
    send_buf[i+2]=eeprom_DiandongOrDingshi[i];
  }
  SendData(send_buf);
}
void Timing_Respond(void)
{
  
}
void DianDongTime_Respond(void)
{
  u8 i=0;
  ClearData(send_buf);
  send_buf[1]=0x14;
  for(i=0;i<16;i++)
  {
    send_buf[i+2] = eeprom_diandongtime[i];
  }
  SendData(send_buf);
}
void All_OPEN_Respond(u8 *a)
{
  u8 i;
  ClearData(send_buf);
  send_buf[1]=0x0c;
  for(i=0;i<16;i++)
  {
    send_buf[i+2]=a[i];
  }
  SendData(send_buf);
}

void UpdateTime_Respond(u8 *a)
{
  u8 i;
  ClearData(send_buf);
  send_buf[1]=0x1f;
  for(i=0;i<16;i++)
  {
    send_buf[i+2]=a[i];
  }
  SendData(send_buf);
}
void SetTime_Respond(void)
{
  ClearData(send_buf);
  send_buf[1]=0x0e;
  send_buf[2]=R1302(SECOND);
  send_buf[3]=R1302(MINUTE);
  send_buf[4]=R1302(HOUR);
  send_buf[5]=R1302(DAY);
  send_buf[6]=R1302(MONTH);
  send_buf[7]=R1302(WEEK);
  send_buf[8]=R1302(YEAR);	
  SendData(send_buf);
}

u8 CheckSum(u8 *a)
{
  u8 sum=0;
  u8 i=0;
  for(i=1;i<17;i++)
  {
    sum+=a[i];
  }
  
  if((u8)sum==a[18])
  {
    return 1;
  }
  else
  {
    Uart_Print_Num((u8)sum,6); 
    return 0;
  }
}

void SendData(u8 *a)
{
  char i=0;
  int sum=0;
  for(i=1;i<17;i++)
  {
    sum+=a[i];
  }
  
  a[18]=(u8)sum;

  a[0]=0xCC;
  a[19]=0xDD;
  
  UART_SendBuf(send_buf);
}

void ClearData(u8 *a)
{
  char i=0;
  for(i=0;i<20;i++)
  {
    a[i]=0;
  }
}

//void OpenOrCloseRelay_Respond()
//{
//  ClearData(send_buf);
//  send_buf[1]=0x11;
//  SendData(send_buf);
//}
void OpenOrCloseRelay(u8 Relay, u8 status)
{
  if(status==OPEN)
  {
    switch(Relay)
    {
      case RL0:RL0_On();break;
      case RL1:RL1_On();break;
      case RL2:RL2_On();break;
      case RL3:RL3_On();break;
      case RL4:RL4_On();break;
      case RL5:RL5_On();break;
      case RL6:RL6_On();break;
      case RL7:RL7_On();break;
      case RL8:RL8_On();break;
      case RL9:RL9_On();break;
      case RLA:RLA_On();break;
      case RLB:RLB_On();break;
      case RLC:RLC_On();break;
      case RLD:RLD_On();break;
      case RLE:RLE_On();break;
      case RLF:RLF_On();break;
    }
  }
  else if(status==CLOSE)
  {
    switch(Relay)
    {
      case RL0:RL0_Off();break;
      case RL1:RL1_Off();break;
      case RL2:RL2_Off();break;
      case RL3:RL3_Off();break;
      case RL4:RL4_Off();break;
      case RL5:RL5_Off();break;
      case RL6:RL6_Off();break;
      case RL7:RL7_Off();break;
      case RL8:RL8_Off();break;
      case RL9:RL9_Off();break;
      case RLA:RLA_Off();break;
      case RLB:RLB_Off();break;
      case RLC:RLC_Off();break;
      case RLD:RLD_Off();break;
      case RLE:RLE_Off();break;
      case RLF:RLF_Off();break;
    }
  } 
}
void All_OPEN(void)
{
  RL1_On();
  RL2_On();
  RL3_On();
  RL4_On(); 
  RL5_On();
  RL6_On();
  RL7_On();
  RL8_On(); 
  RL9_On();
  RLA_On();
  RLB_On();
  RLC_On();
  RLD_On();
  RLE_On();
  RLF_On();
  RL0_On();
}
void All_CLOSE(void)
{
  RL1_Off();
  RL2_Off();
  RL3_Off();
  RL4_Off(); 
  RL5_Off();
  RL6_Off();
  RL7_Off();
  RL8_Off(); 
  RL9_Off();
  RLA_Off();
  RLB_Off();
  RLC_Off();
  RLD_Off();
  RLE_Off();
  RLF_Off();
  RL0_Off();
}

void WriteEEPROM(uint32_t addr,uint8_t dat)
{
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  FLASH_ProgramByte(addr, dat);//eeprom memory: address is 0x1001 =temp1
  FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);
  FLASH_Lock(FLASH_MEMTYPE_DATA);
}

#define TIM4_PERIOD       124
static void TIM4_Config(void)
{
  /* TIM4 configuration:
   - TIM4CLK is set to 16 MHz, the TIM4 Prescaler is equal to 128 so the TIM1 counter
   clock used is 16 MHz / 128 = 125 000 Hz
  - With 125 000 Hz we can generate time base:
      max time base is 2.048 ms if TIM4_PERIOD = 255 --> (255 + 1) / 125000 = 2.048 ms
      min time base is 0.016 ms if TIM4_PERIOD = 1   --> (  1 + 1) / 125000 = 0.016 ms
  - In this example we need to generate a time base equal to 1 ms
   so TIM4_PERIOD = (0.001 * 125000 - 1) = 124 */

  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, ENABLE);
  /* Time base configuration */
  TIM4_TimeBaseInit(TIM4_PRESCALER_128, TIM4_PERIOD);
  /* Clear TIM4 update flag */
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  /* Enable update interrupt */
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  /* Enable TIM4 */
  TIM4_Cmd(ENABLE);
  
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
