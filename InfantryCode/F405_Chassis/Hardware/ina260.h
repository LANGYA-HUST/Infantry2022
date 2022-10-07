#ifndef __INA260_H
#define __INA260_H
#include "stm32f4xx.h"

#define INA260_1_ID 	0x45		//1000101
#define INA260_2_ID 	0x44		//1000100
#define REG_SETTING 	0x00
#define REG_CURRENT		0x01
#define REG_VOLTAGE 	0x02
#define REG_POWER			0x03
#define LSB_CURRENT		1.25	//mA
#define LSB_VOLTAGE 	1.25	//mV
#define LSB_POWER			10		//mW
//#define  I_Max 				5
#define CAP_VOLTAGE_LOW 11
#define CAP_VOLTAGE_HIGH 14

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

typedef struct INA260
{
	unsigned char Configuration[2];		//INA260ÅäÖÃ
	float Current;
	float Voltage;
  float Power;
}INA260;


void INA_READ_Current(void);
void INA_READ_Vol(void);
void INA_READ_Power(void);
short INA260_Read(u8 address, u8 reg);

#endif


