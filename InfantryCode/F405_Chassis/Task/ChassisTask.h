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
	short Actual_P_max;							//�����
	short Self_Protect_Limit;	//С����ת������
	float k_BAT;						//��ͬ�����µ�ֱ���ٶ�����
	short Excess_P_max_J;			//��ʱ������˲�����޹���
	short Excess_P_max_P;     //�ȶ�ʱ����˲�������޹���
	short CurrentMax;				//����������
	short Follow_W;         //�����ٶ�����
}Power_Typedef;

#define YES 1
#define NO  0

#define METHOD_NUM 8			//�ж����ֲ�ͬ���ʲ���
#define PN_Default 0x00

void Method_Check(void);
void Chassis_Power_Control_Init(void);
void Chassis_Mode_Special_Control(void);
#endif
