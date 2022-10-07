#ifndef START_TASK_H
#define START_TASK_H
#include "main.h"
void CPU_task(void *pvParameters);
void start_task(void *pvParameters);
void startTast(void);
void Robot_Stop();
void Robot_Recover();
void Shoot_Stop();
void Shoot_Recover();
void PC_Rst();
#endif
