#ifndef __WIBI_MOTOR_H__
#define __WIBI_MOTOR_H__
#include "main.h"
#define STATE_ENCODER_VALUE									0x90				//��ȡ����������
#define	STATE_MOTOR_ERROR										0x9A				//��ȡ��������־��״̬
#define	STATE_MOTOR_STATE										0x9C				//��ȡ���״̬
#define	STATE_MOTOR_GET_MULTIANGLE					0x92				//*��ȡ��Ȧ�Ƕ�
#define	STATE_MOTOR_GET_SINGLEANGLE					0x94				//*��ȡ��Ȧ�Ƕ�
#define	STATE_MOTOR_GET_TORQUE							0xA1				//ת�رջ����Ʒ���
#define	STATE_STOP_CLEAR										0x80				//�رյ��(���֮ǰ״̬)
#define	STATE_STOP  												0x81				//�رյ��(�����֮ǰ״̬)
#define	STATE_RUN														0x88				//���е��
	
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

