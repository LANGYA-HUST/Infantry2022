#ifndef __DATASENDTASK_H
#define __DATASENDTASK_H

#include "stdint.h"

typedef struct F105{
	short ChassisSpeedw;
	short Remain_power;
  char IsShootAble;
	char RobotRed;
  char BulletSpeedLevel;
  float Limit_Power_k;
	short HP;
	short Last_HP;
}F105_Typedef;

void ChassisCan1Send(short a,short b,short c,short d);
void Can2Send0(F105_Typedef *F105_Send);
void Can2Send1(void);
void Can2Send2(uint8_t BuffState);
void USART2SEND(void);

#endif 

