#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
#define Battery_Ch 5
void Adc_Init(void);
u16  Get_Adc(u8 ch);
int  Get_battery_volt(void);    
void RD_TSL(void) ;
void Dly_us(void);
void  ccd_Init(void);
void  ele_Init(void);
#endif 



















