#ifndef __MAIN_H
#define __MAIN_H

/*Library*/
#include "stm32f10x.h"
#include "misc.h"
#include <string.h>
#include "stm32f10x_dma.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_i2c.h"

/*Mylib*/
#include "can1.h"
#include "can2.h"
#include "usart2.h"
#include "uart4.h"
#include "tim2.h"
#include "tim4.h"
#include "iwdg.h"
#include "adc.h"
#include "dac.h"
#include "ina260.h"
#include "i2c.h"

/*Algorithm*/
#include "pid.h"
#include "FIR.h"
#include "DataScope_DP.h"
#include "algorithmOfCRC.h"

/*Task*/
#include "DataReceiveTask.h"
#include "DataSendTask.h"
#include "ChassisTask.h"
#include "PowerControlTask.h"
#include "StartTask.h"
#include "CharSendTask.h"
#include "GraphicsSendTask.h"
#include "JumpCal_Task.h"

/*FreeRTOS*/
#include "FreeRTOS.h"
#include "task.h"



#define ABS(x) ((x)>0? (x):(-(x))) 
#define LIMIT_MAX_MIN(x, max, min)	(((x) <= (min)) ? (min):(((x) >= (max)) ? (max) : (x)))
#define POWER_OFF 0
#define CHARGE_ENABLE 1

//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

typedef union
{
	float fdata;			//4?
	unsigned long idata;
}
FloatLongType;

typedef union
{
	float fdata;			//4?
	unsigned char idata[4];
}
FloatCharType;

typedef  struct{
short ChassisDisconnect[4];
short JudgeDisconnect;
short F405Disconnect;
short SuperPowerDisconnect;
} roboDisconnect;

void BSP_Init(void);
void Robot_Init(void);

void Offline_Check_task(void *pvParameters);


#endif


