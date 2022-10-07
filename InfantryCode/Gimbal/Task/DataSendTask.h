#ifndef __DATASENDTASK_H__
#define __DATASENDTASK_H__

typedef struct{
	char SuperPowerLimit;	//0Ϊ�������ݹرգ���Ϊ0����ʹ�ó�������
	char Chassis_Flag;			//0��������ģʽ  1����С����  2������ģʽ  3��ʾ����
	char Mag_Flag;					//0��ʾ���ָǹرգ�1Ϊ��
	char Laser_Flag;				//0��ʾ����رգ�1Ϊ��
	short Gimbal_100;				//pitch�Ƕ�,����10֮��
	char Gimbal_Flag;				//0Ϊ����ģʽ  1Ϊ����
	char Graphic_Init_Flag;	//0Ϊ�����ʼ��ģʽ��1Ϊ��ʼ������
	char Freq_state;			  //0��ʾ������Ƶ 1��ʾ����Ƶ
	/*�������*/
	char Send_Pack1;	
  char Fric_Flag;	
}F405_typedef;

void BodanCan1Send(short a);
void F405Can1Send(F405_typedef *F405_Send);
void GimbalCan2Send(short X,short Y);
void ChassisCan1Send(short *carSpeedx,short *carSpeedy,short *carSpeedw);

void USART6_SendtoPC(void);
void Tx2_Off_CheckAndSet(u8* Buff);
void TX2_task(void *pvParameters);
void FrictionBodanCan2Send(short X,short Y,short Z);
#define Tx2_Off 0xff				//�ػ�ģʽ
#define Tx2_Small_Buff 0x10	//С��ģʽ
#define Tx2_Big_Buff 0x20		//���ģʽ
#define Tx2_Armor 0x30			//����ģʽ

#endif
