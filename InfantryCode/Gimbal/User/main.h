#ifndef __MAIN_H
#define __MAIN_H


#define Robot_ID  4       // 不同机器人用不同的序号
//  4   14 为新自适应麦轮编号    
//  3      为独立悬挂麦轮编号
//  5      为全向轮

/*Library*/
#include <stm32f4xx.h>	 
#include <stm32f4xx_conf.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "arm_math.h"

/*Mylib*/
#include "gpio.h"
#include "can1.h"
#include "can2.h"
#include "tim4.h"
#include "tim7.h"
#include "usart1.h"
#include "uart4.h"
#include "usart1.h"
#include "usart2.h"
#include "MicroSw.h"
#include "SteeringEngine.h"
#include "frictionwheel.h"
#include "iwdg.h"
#include "laser.h"


/*Algorithm*/
#include "pid.h"
#include "algorithmOfCRC.h"
#include "KalmanFilter.h"

/*Task*/
#include "ZeroCheckTask.h"
#include "DataSendTask.h"
#include "DataReceivetask.h"
#include "ActionTask.h"
#include "GimbalTask.h"
#include "ShootTask.h"
#include "ChassisTask.h"
#include "Start_Task.h"


#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#define ABS(x) ((x)>0? (x):(-(x))) 
#define LIMIT_MAX_MIN(x, max, min)	(((x) <= (min)) ? (min):(((x) >= (max)) ? (max) : (x)))
#define POWER_OFF 0
#define CHARGE_ENABLE 1

//IO口地址映射 适合F405
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40010814 
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40010C14 
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40011014 
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40011414 
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40011814 
#define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40011A14    
#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40011E14    

#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40010810 
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40010C10 
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40011010 
#define GPIOD_IDR_Addr    (GPIOD_BASE+16) //0x40011410 
#define GPIOE_IDR_Addr    (GPIOE_BASE+16) //0x40011810 
#define GPIOF_IDR_Addr    (GPIOF_BASE+16) //0x40011A10 
#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40011E10

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


/*步兵初始参数结构体*/
typedef struct
{
	  int8_t gyro_pn;
	  int8_t motor_pn;
		unsigned short MagOpen;
		unsigned short MagClose;
	  unsigned short Pitch_init;
	  unsigned short Yaw_init;
		unsigned short Solo_Yaw_init;	//左单挑模式的Yaw_init
		unsigned short Low_FrictionSpeed;
		unsigned short Medium_FrictionSpeed;
		unsigned short High_FrictionSpeed;
	  unsigned short PitchMotorID;
	  unsigned short YawMotorID;
	  unsigned short FricMotorID[2];
	  unsigned short BodanMotorID;
	  short pitch_max_motor;
		short pitch_min_motor;
		short pitch_max_gyro;
		short pitch_min_gyro;
}RobotInit_Struct;

void BSP_Init(void);
void Robot_Init(void);

void Infantry_Init(void);
void delay_ms(unsigned long t);
void delay_us(unsigned long t);

void Offline_Check_task(void *pvParameters);
void Cmera_rising_edge(int *state,int cur_num);



#endif
