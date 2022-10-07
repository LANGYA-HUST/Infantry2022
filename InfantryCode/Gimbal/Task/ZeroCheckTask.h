#ifndef __ZEROCHECKTASK_H
#define __ZEROCHECKTASK_H

#define Position 1
#define Speed    2

typedef struct 
{
	float Circle;           //ת��Ȧ��
	float CountCycle;       //ת��һȦ���ܼ�������
	float LastValue;        //����������һ�ε�ֵ	
	float ActualValue;      //����������ǰֵ
	float PreError;         //������жϲ�ֵ
}
ZeroCheck_Typedef;

/*������ݽṹ��*/
typedef struct 
{
	float Gyro;							//Pitch/Yaw�������ǽǶ�
	float Motor;						//Pitch/Yaw�����Ƕ�
	float MotorTransAngle;	//Pitch/Yaw����ת��(����Ƕ�-��ʼ�Ƕ�)
}
GimbalData_Typedef;

/*����ṹ��*/
typedef struct 
{
	GimbalData_Typedef Pitch;
	GimbalData_Typedef Yaw;
}
Gimbal_Typedef;

float ZeroCheck(ZeroCheck_Typedef *Zero,float value,short Zerocheck_mode);
void ZeroCheck_cal(void);
void ZeroCheck_Init(void);

#endif

