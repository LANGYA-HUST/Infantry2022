/**********************************************************************************************************
 * @�ļ�     ZeroCheckTask.c
 * @˵��     ������
 * @�汾  	 V1.0
 * @����     ��־��
 * @����     2020.1
**********************************************************************************************************/
#include "main.h"
/*--------------------------�ڲ�����----------------------------*/
float Bodan_Pos;
float pitch_motor_temp;
char  pitch_lose_flag;
/*--------------------------�ṹ��----------------------------*/
Gimbal_Typedef Gimbal;
ZeroCheck_Typedef ZeroCheck_Pitch,ZeroCheck_Yaw,ZeroCheck_Pitch_speed,ZeroCheck_Yaw_speed; 
ZeroCheck_Typedef ZeroCheck_BodanPos,ZeroCheck_BodanSpeed;
ZeroCheck_Typedef ZeroCheck_GyroYaw,ZeroCheck_GyroPitch; 
/*--------------------------�ⲿ���ñ���----------------------------*/
extern RobotInit_Struct Infantry;
extern float PitchMotorReceive,YawMotorReceive;//Pitch,Yaw����Ƕ�
extern Gyro_Typedef GyroReceive;//����������
extern BodanMotorReceive_Typedef BodanReceive;



/**********************************************************************************************************
*�� �� ��: ZeroCheck
*����˵��: λ��ʽ���ٶ�ʽ������
					 Zero->ActualValue ��ʾ�������ǰֵ
					 Zero->LastValue ��ʾ�������һ��ֵ
					 Zero->CountCycle ��ʾ���������ʱԽ��ֵ������������
					 Zero->PreError ��ʾ�������ֵ
					 ʹ�ô˺���ǰҪ������Ӧ������ṹ��� Zero->CountCycle��Zero->LastValue
*��    ��: ZeroCheck_Typedef *Zero  ������ṹ��
  *        float value  �������
*�� �� ֵ: ȡ����Zerocheck_mode���ֱ�����������λ��ֵ���ٶ�ֵ
**********************************************************************************************************/
float ZeroCheck(ZeroCheck_Typedef *Zero,float value,short Zerocheck_mode)
{
	Zero->ActualValue=value;
	
	Zero->PreError=Zero->ActualValue-Zero->LastValue;
	Zero->LastValue=Zero->ActualValue;
	
	if(Zero->PreError>0.7f*Zero->CountCycle)
	{
		Zero->PreError=Zero->PreError-Zero->CountCycle;
		Zero->Circle++;
	}
	if(Zero->PreError<-0.7f*Zero->CountCycle)
	{
		Zero->PreError=Zero->PreError+Zero->CountCycle;
		Zero->Circle--;
	}
	
	if(Zerocheck_mode==Position)
		return Zero->ActualValue - Zero->Circle*Zero->CountCycle;
	else if(Zerocheck_mode==Speed)
	  return Zero->PreError;
	else 
		return 0;
}



/**********************************************************************************************************
*�� �� ��: ZeroCheck_cal
*����˵��: ������ִ�к���
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void ZeroCheck_cal(void)
{
	Gimbal.Pitch.Gyro = GyroReceive.PITCH;
	Gimbal.Pitch.Motor = ZeroCheck(&ZeroCheck_Pitch,PitchMotorReceive,Position);
	pitch_motor_temp = Infantry.motor_pn*(Infantry.Pitch_init-Gimbal.Pitch.Motor)/8192.0f*360.0f;
	
	if( ABS(pitch_motor_temp) > 50.0f)
	{
	  pitch_lose_flag = 1;
	}
	else
	{
	  pitch_lose_flag = 0;	
		Gimbal.Pitch.MotorTransAngle = pitch_motor_temp;
	}
	
	Gimbal.Yaw.Gyro = ZeroCheck(&ZeroCheck_GyroYaw,GyroReceive.YAW,Position);
	Gimbal.Yaw.Motor = ZeroCheck(&ZeroCheck_Yaw,YawMotorReceive,Position);
	Gimbal.Yaw.MotorTransAngle = (Gimbal.Yaw.Motor-Infantry.Yaw_init)/8192.0f*360.0f;
	
	Bodan_Pos=ZeroCheck(&ZeroCheck_BodanPos,BodanReceive.Angle,Position);
}


/**********************************************************************************************************
*�� �� ��: ZeroCheck_Init
*����˵��: ������ṹ�������ʼ��
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void ZeroCheck_Init(void)
{
	ZeroCheck_Pitch.CountCycle=8192;
	ZeroCheck_Pitch.LastValue=PitchMotorReceive;
	ZeroCheck_Pitch.Circle=0;

	ZeroCheck_Yaw.CountCycle=8192;
	ZeroCheck_Yaw.LastValue=YawMotorReceive;
	
	ZeroCheck_BodanPos.Circle=0;
	ZeroCheck_BodanPos.CountCycle=8192;
	ZeroCheck_BodanPos.LastValue=Bodan_Pos;
	
	ZeroCheck_GyroYaw.Circle=0;
	ZeroCheck_GyroYaw.CountCycle=360;
	ZeroCheck_GyroYaw.LastValue=GyroReceive.YAW;
	
	ZeroCheck_GyroPitch.Circle=0;
	ZeroCheck_GyroPitch.CountCycle=360;
	ZeroCheck_GyroPitch.LastValue=GyroReceive.PITCH;
	
	ZeroCheck_Pitch_speed.Circle=0;
	ZeroCheck_Pitch_speed.CountCycle=8192;
	ZeroCheck_Pitch_speed.LastValue=PitchMotorReceive;
	
	ZeroCheck_Yaw_speed.Circle=0;
	ZeroCheck_Yaw_speed.CountCycle=8192;
	ZeroCheck_Yaw_speed.LastValue=YawMotorReceive;
}


