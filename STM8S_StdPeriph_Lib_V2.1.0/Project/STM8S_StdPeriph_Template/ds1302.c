#include "ds1302.h"
                //    秒，分，时，日，月，星期，年
uchar starts_time[7]={ 50, 59, 21, 16, 2,    3,  15};

uchar Sec;
uchar Cent;
uchar Hour;
uchar Year;
uchar Day;
uchar Week;
uchar Month;
uchar AddTime=0;
uchar UpdateMode=0;
/******************************************************************/

void ds1302_Initial(void){
    GPIO_Init(GPIOC,GPIO_PIN_2, GPIO_MODE_OUT_PP_HIGH_FAST);//RTC_CLK
    GPIO_Init(GPIOC,GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST);//RTC_CE	 
    GPIO_Init(GPIOC,GPIO_PIN_3, GPIO_MODE_IN_PU_NO_IT);			//RTC_DATA	
    
    //Set1302(starts_time);
}

//void Set(uchar sel,uchar sel_1) 
//{
//  uchar address;
//  char max,min,time;
// 
//  if(sel==11)  {address=0xc2; max=22;min=1;} 
//  if(sel==10)  {address=0xc0; max=1;min=0; }
//  if(sel==9) {address=0xc6; max=20;min=0;}
//  if(sel==8) {address=0xc4; max=1;min=0;}      
//  if(sel==7)  {address=0x80; max=59;min=0;}   
//  if(sel==6)  {address=0x82; max=59;min=0;}    
//  if(sel==5)  {address=0x84; max=23;min=0;}   
//  if(sel==4)  {address=0x8a; max=7; min=1;}   
//  if(sel==3)  {address=0x86; max=31;min=1;}  
//  if(sel==2)  {address=0x88; max=12;min=1;}  
//  if(sel==1)  {address=0x8c; max=99;min=0;}    
//  
//  
//  time=R1302(address+1)/16*10+R1302(address+1)%16;
//  if (sel_1==0) time++;  else time--;
//  if(time>max) time=min;   
//  if(time<min) time=max;
//           
//  W1302(0x8e,0x00);
//  W1302(address,time/10*16+time%10);		 
//  W1302(0x8e,0x80);  
//}

/**************************?óê±***********************/

void DelayMs(void)
 {
  uchar TempCyc;
  for(TempCyc=50;TempCyc>1;TempCyc--)
     {;}
 }


/******************************************************************** 

***********************************************************************/
void SetByte(uchar d) 
{ 
    uchar h;
    T_IO_OUT;
    for(h=0;h<8;h++)
    {
        if(d&0x01)
        {
          T_IO_1;
        }
        else
        {
          T_IO_0;
        }
        T_CLK_1;
        T_CLK_0;
        d = d >> 1; 
    } 
    T_IO_IN;
}
/******************************************************************** 

***********************************************************************/
uchar GetByte(void) 
{ 
    uchar h;
    uchar d=0;
    T_IO_IN;
    for(h=0;h<8;h++)
    {
        d = d >>1;         /*?àμ±óú??±à?Dμ? RRC */
        if(T_IO)
        {
          d|=0x80;
        }
       
        T_CLK_1;
        T_CLK_0;
    } 
    return(d); 
}
/******************************************************************** 
***********************************************************************/
void W1302(uchar ucAddr, uchar ucDa)
{
  if(ucAddr==HOUR){ucDa&=0X7F;}
  ucDa=((ucDa/10)<<4)|(ucDa%10);
    T_RST_0;
    T_CLK_0;
    T_RST_1;
    SetByte(ucAddr); 
    SetByte(ucDa);
    T_CLK_1;
    T_RST_0;
}
/******************************************************************** 

***********************************************************************/
uchar R1302(uchar ucAddr)
{
    uchar ucData;
    T_RST_0;
    T_CLK_0;
    T_RST_1;
    SetByte(ucAddr+1);    
    ucData = GetByte(); 
    T_CLK_1;
    T_RST_0;
    
    if(ucAddr==HOUR)
    {
      ucData=((ucData&0xf0)>>4)*10+(ucData&0x0f);
    }
    else
    {
      ucData=((ucData&0x70)>>4)*10+(ucData&0x0f);
    }
//    ucData=((ucData&0x70)>>4)*10+(ucData&0x0f);
    return(ucData);
}

/******************************************************************** 

***********************************************************************/
void Set1302(uchar *pClock) 
{
    uchar h;
    uchar ucAddr = 0x80; 
    W1302(0x8e,0x00);
    for(h =7; h>0; h--)
    { 
        W1302(ucAddr,*pClock); 
        pClock++;
        ucAddr +=2;
    }
//	W1302(0xc0,0x01);
//	W1302(0XC2,0X03);
    W1302(0x8e,0x80); 
}
/**********************************/
//void du1302()
//{
//      Sec=R1302(0x81);  
//	  Cent=R1302(0x83);
//	  Hour=R1302(0x85);
//	  Day=R1302(0x87);
//	  Month=R1302(0x89);
//	  Week=R1302(0x8b);
//	  Year=R1302(0x8d);				
////	  b=R1302(0xc1);
////      a=R1302(0xc3);
//	  //a=a/16*10+a%16;
//	  UpdateMode=R1302(0xc5);
//	  AddTime=R1302(0xc7);
//	 // AddTime=AddTime/16*10+AddTime%16;
//}