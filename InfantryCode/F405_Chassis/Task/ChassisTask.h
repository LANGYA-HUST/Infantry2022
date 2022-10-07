#ifndef __CHASSISTASK_H__
#define __CHASSISTASK_H__
#include "main.h"

void Chassis_Speed_Cal(void);
void Key_Num_Check(void);
void Chassis_Mode_Special_Control(void);
void PowerLimit(void);
void Chassis_CurrentPid_Cal(void);
void Current_Filter_Excu(void);
void Current_Set_Jump(void);

void Pid_ChassisWheelInit(void);
void pid_test_cal(void);
void Chassis_task(void *pvParameters);

typedef struct Power{
	short Actual_P_max;							//最大功率
	short Self_Protect_Limit;	//小陀螺转速限制
	float k_BAT;						//不同功率下的直行速度上线
	short Excess_P_max_J;			//起步时无输出滤波最大超限功率
	short Excess_P_max_P;     //稳定时输出滤波后最大超限功率
	short CurrentMax;				//最大输出电流
	short Follow_W;         //跟随速度上限
}Power_Typedef;

#define YES 1
#define NO  0

#define METHOD_NUM 8			//有多少种不同功率参数
#define PN_Default 0x00

void Method_Check(void);
void Chassis_Power_Control_Init(void);
void Chassis_Mode_Special_Control(void);
#endif
