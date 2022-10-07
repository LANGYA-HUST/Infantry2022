#ifndef __SHOOTTASK_H
#define __SHOOTTASK_H

#include "main.h"
#include "DataReceivetask.h"

#define   PluckThreholdPos        50        


void FrictionSpeedChoose(void);



void Shoot_Check_Cal(void);
void Shoot_Fire_Cal(void);
void Shoot_Powerdown_Cal(void);
void Shoot_Tx2_Cal(void);
void Shoot_Test_Cal(void);

void BodanMotor_CurrentPid_Cal(void);
void Pid_BodanMotor_Init(void);
void Pid_Friction_Init(void);
void Shoot_task(void *pvParameters);

#endif
