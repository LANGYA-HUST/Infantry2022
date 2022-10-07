#ifndef __WIBI_MOTOR_H__
#define __WIBI_MOTOR_H__
#include "main.h"
#define STATE_ENCODER_VALUE									0x90				//读取编码器命令
#define	STATE_MOTOR_ERROR										0x9A				//读取电机错误标志与状态
#define	STATE_MOTOR_STATE										0x9C				//读取电机状态
#define	STATE_MOTOR_GET_MULTIANGLE					0x92				//*读取多圈角度
#define	STATE_MOTOR_GET_SINGLEANGLE					0x94				//*读取单圈角度
#define	STATE_MOTOR_GET_TORQUE							0xA1				//转矩闭环控制反馈
#define	STATE_STOP_CLEAR										0x80				//关闭电机(清除之前状态)
#define	STATE_STOP  												0x81				//关闭电机(不清除之前状态)
#define	STATE_RUN														0x88				//运行电机
	
void Can1Send_Developer(uint32_t StdId,uint8_t Oder_frame[8]);

void HUB_MOTOR_GetState(void);
void HUB_MOTOR_GetMultAngle(void);
void HUB_MOTOR_GetError(void);
static void HUB_MOTOR_ORDER(uint8_t order);
void Knee_motor_Init(uint32_t LStdId,uint32_t RStdId);
void M1502DSend_SetID(char id);
void M1502DSend_SetControlMode(void);
void M1502DSend_SetCanRestor(void);
void M1502DSend_SetfeedbackMode(void);
void M1502DSend_GetMotorState(char ID);
void MD1502HubMotor_ENABLE(void);
void MD1502HubMotor_DISABLE(void);
#endif

