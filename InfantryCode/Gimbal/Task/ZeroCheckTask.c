/**********************************************************************************************************
 * @文件     ZeroCheckTask.c
 * @说明     过零检测
 * @版本  	 V1.0
 * @作者     黄志雄
 * @日期     2020.1
**********************************************************************************************************/
#include "main.h"
/*--------------------------内部变量----------------------------*/
float Bodan_Pos;
float pitch_motor_temp;
char  pitch_lose_flag;
/*--------------------------结构体----------------------------*/
Gimbal_Typedef Gimbal;
ZeroCheck_Typedef ZeroCheck_Pitch,ZeroCheck_Yaw,ZeroCheck_Pitch_speed,ZeroCheck_Yaw_speed; 
ZeroCheck_Typedef ZeroCheck_BodanPos,ZeroCheck_BodanSpeed;
ZeroCheck_Typedef ZeroCheck_GyroYaw,ZeroCheck_GyroPitch; 
/*--------------------------外部引用变量----------------------------*/
extern RobotInit_Struct Infantry;
extern float PitchMotorReceive,YawMotorReceive;//Pitch,Yaw电机角度
extern Gyro_Typedef GyroReceive;//陀螺仪数据
extern BodanMotorReceive_Typedef BodanReceive;



/**********************************************************************************************************
*函 数 名: ZeroCheck
*功能说明: 位置式和速度式过零检测
					 Zero->ActualValue 表示检测量当前值
					 Zero->LastValue 表示检测量上一次值
					 Zero->CountCycle 表示检测量过零时越变值，即计数周期
					 Zero->PreError 表示检测量差值
					 使用此函数前要申明对应检测量结构体的 Zero->CountCycle与Zero->LastValue
*形    参: ZeroCheck_Typedef *Zero  过零检测结构体
  *        float value  待检测量
*返 回 值: 取决于Zerocheck_mode，分别输出过零检测后位置值或速度值
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
*函 数 名: ZeroCheck_cal
*功能说明: 过零检测执行函数
*形    参: 无
*返 回 值: 无
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
*函 数 名: ZeroCheck_Init
*功能说明: 过零检测结构体参数初始化
*形    参: 无
*返 回 值: 无
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


