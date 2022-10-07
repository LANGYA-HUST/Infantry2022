/**********************************************************************************************************
 * @文件     GimbalTask.c
 * @说明     云台控制
 * @版本  	 V1.0
 * @作者     黄志雄
 * @日期     2020.1
**********************************************************************************************************/
/**********************************************************************************************************
 * @文件     GimbalTask.c
 * @说明     云台控制
 * @版本  	 V2.0
 * @作者     戴军
 * @日期     2022.4
**********************************************************************************************************/
#include "main.h"
/*----------------------------------内部变量---------------------------*/

int inttoshort[4];
short GimbalAct_Init_Flag=0;

short YawCurrent,PitchCurrent;
float GimbalYawPos,GimbalPitchPos;

float Recent_Angle_Buff;
float Recent_Yaw_Angle_Armor;
float Recent_Pitch_Angle_Armor;
/*-----------------------------------结构体-----------------------------*/
Pid_Typedef PidPitchSpeed,PidPitchPos,PidYawSpeed,PidYawPos;
Pid_Typedef PidPitchAidPos,PidPitchAidSpeed,PidYawAidPos,PidYawAidSpeed,PidPitchBuffSpeed,PidYawBuffSpeed;
FuzzyPID FuzzyAidPidPitchPos,FuzzyAidPidYawPos,FuzzyBuffPidPitchPos,FuzzyBuffPidYawPos;
/*----------------------------------外部变量---------------------------*/
extern RC_Ctl_t RC_Ctl; 
extern Status_t Status;
extern char PitchMotor_ReceiveFlag;
extern Gimbal_Typedef Gimbal;
extern Gyro_Typedef GyroReceive;//陀螺仪数据
extern RobotInit_Struct Infantry;
extern short FrictionWheel_speed;
extern F405_typedef F405;
extern PC_Receive_t PC_Receive;
extern char Budan;
extern float Buff_Yaw_Motor;
extern char q_flag;
extern FeedForward_Typedef FF_w;
extern char  pitch_lose_flag;
/**********************************************************************************************************
*函 数 名: Gimbal_Powerdown_Cal
*功能说明: 云台掉电模式
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void Gimbal_Powerdown_Cal()
{
	if(GimbalAct_Init_Flag!=Gimbal_Powerdown_Mode)
	{
		Laser_Off();
	  GimbalAct_Init_Flag=Gimbal_Powerdown_Mode;
	}
	
	/***********************************************************************************/
	PidPitchPos.SetPoint = Infantry.pitch_min_motor;
	
  /**************************************计算电流值**************************************/
//PITCH              
  PidPitchPos.ActualValue = Gimbal.Pitch.MotorTransAngle;	
  PidPitchSpeed.SetPoint=PID_Calc(&PidPitchPos);
  PidPitchSpeed.ActualValue = GyroReceive.GY;	
	inttoshort[0]=(PID_Calc(&PidPitchSpeed));//旧陀螺仪
	PitchCurrent=(short)(-inttoshort[0])*Infantry.motor_pn;
  PitchCurrent=0;
	YawCurrent=0;

}

///**********************************************************************************************************
//*函 数 名: Gimbal_Act_Cal
//*功能说明: 云台正常模式
//*形    参: rc  mouse  Pc_RecvData
//*返 回 值: 无
//**********************************************************************************************************/
//float test_pitch = 0;
//float test_yaw = 0;
//int aaaaa;
//void Gimbal_Act_Cal(Remote rc,Mouse mouse,PC_Receive_t *Pc_Recv)
//{
//	if( GimbalAct_Init_Flag!=Gimbal_Act_Mode)
//	{
//		Laser_On();
//		GimbalPitchPos = GyroReceive.PITCH;				//从大符模式切回，保持pitch电机角，yaw陀螺仪角，不乱动
//		GimbalYawPos = Gimbal.Yaw.Gyro;
//		GimbalAct_Init_Flag=Gimbal_Act_Mode;
//	}

//	
//	if(Status.ControlMode==Control_RC_Mode)//Rc_Control
//	{
//	  GimbalYawPos   += (1024-rc.ch2)*0.0005f;
//	  GimbalPitchPos -= (1024-rc.ch3)*0.0003f;//旧陀螺仪
//	}
//	if(Status.ControlMode==Control_MouseKey_Mode)//Mouse_Key
//	{
//    GimbalPitchPos -= mouse.y*0.005f;
//    GimbalYawPos   -= mouse.x*0.005f;
//		GimbalPitchPos += mouse.z*0.001f;
//	}

//	GimbalPitchPos=LIMIT_MAX_MIN(GimbalPitchPos,gimbal_pitch_max,gimbal_pitch_min);//限位(用电机角度)	//限住大小，因此可用MotorTransangle来pid
////YAW/***********************************************************************************/
//	PidPitchPos.SetPoint = GimbalPitchPos;
//	PidYawPos.SetPoint = GimbalYawPos;

//	
//  /**************************************计算电流值**************************************/
////PITCH                
////  PidPitchSpeed.SetPoint=PID_Calc(&PidPitchPos,Gimbal.Pitch.MotorTransAngle);   
//  PidPitchSpeed.SetPoint=PID_Calc(&PidPitchPos,GyroReceive.PITCH);   	
//	inttoshort[0]=-(PID_Calc(&PidPitchSpeed,GyroReceive.GY));//旧陀螺仪
//	PitchCurrent=(short)inttoshort[0];
////YAW
//	PidYawSpeed.SetPoint=PID_Calc(&PidYawPos,Gimbal.Yaw.Gyro);					//需要YAW轴陀螺仪角度做ActualValue
//	PidYawSpeed.SetPoint=LIMIT_MAX_MIN(PidYawSpeed.SetPoint,5.5f,-5.5f);    
//	inttoshort[1]=PID_Calc(&PidYawSpeed,GyroReceive.GZ);
//	YawCurrent=(short)inttoshort[1];
//	/* pid_test 负数*/
////	test_gyro_pitch = -Gimbal.Pitch.Gyro;
////	test_gyro_yaw = -Gimbal.Yaw.Gyro;
//}
/**********************************************************************************************************
*函 数 名: FuzzyGimbal_Act_Cal
*功能说明: 模糊云台正常模式(电机角)
*形    参: rc  mouse  Pc_RecvData
*返 回 值: 无
**********************************************************************************************************/
float k_yaw;
char ch2_return_flag;
void FuzzyMotorGimbal_Act_Cal(Remote rc,Mouse mouse)
{
	if( GimbalAct_Init_Flag!=Gimbal_Act_Mode)
	{
		Laser_On();
		GimbalPitchPos = Gimbal.Pitch.MotorTransAngle;				//从大符模式切回，保持pitch电机角，yaw陀螺仪角，不乱动
		GimbalYawPos = Gimbal.Yaw.Gyro;
		GimbalAct_Init_Flag=Gimbal_Act_Mode;
	  ch2_return_flag  =0;
	}

	
	if(Status.ControlMode==Control_RC_Mode)//Rc_Control
	{
		if(rc.ch2 != 1024)
	  {
			GimbalYawPos   =  GimbalYawPos+(1024-rc.ch2)*0.0010f;
		  ch2_return_flag = 1;
		}
		else if(ch2_return_flag)
		{
		GimbalYawPos = Gimbal.Yaw.Gyro;
		ch2_return_flag = 0;
		}

		GimbalPitchPos = GimbalPitchPos-(1024-rc.ch3)*0.0005f; 	
		FF_w.Now_DeltIn  = (1024-rc.ch2)*0.0008f;
		
	}
	if(Status.ControlMode==Control_MouseKey_Mode)//Mouse_Key
	{
    GimbalPitchPos -= mouse.y*0.003f;    //原本：0.005f
    GimbalYawPos   -= mouse.x*0.002f;   //0.0016
		GimbalPitchPos += mouse.z*0.002f;
		
		FF_w.Now_DeltIn  = -mouse.x*0.0016f;
	}

	if(Budan)
	{
	GimbalPitchPos = Infantry.pitch_min_motor;
	}
	else
	{
	GimbalPitchPos=LIMIT_MAX_MIN(GimbalPitchPos,Infantry.pitch_max_motor,Infantry.pitch_min_motor);//限位(用电机角度)	//限住大小，因此可用MotorTransangle来pid
	}
	/***********************************************************************************/

	PidPitchPos.SetPoint = GimbalPitchPos;
	PidYawPos.SetPoint = GimbalYawPos;
	
  /**************************************计算电流值**************************************/
////PITCH                 
	PidPitchPos.ActualValue = Gimbal.Pitch.MotorTransAngle;
	PidPitchSpeed.SetPoint=PID_Calc(&PidPitchPos);
  PidPitchSpeed.ActualValue = GyroReceive.GY;	
	inttoshort[0]=(PID_Calc(&PidPitchSpeed));//旧陀螺仪
	PitchCurrent=(short)(-inttoshort[0])*Infantry.motor_pn;
	
//YAW
	PidYawPos.ActualValue = Gimbal.Yaw.Gyro;
	PidYawSpeed.SetPoint=PID_Calc(&PidYawPos);					//需要YAW轴陀螺仪角度做ActualValue
	PidYawSpeed.ActualValue = GyroReceive.GZ;    
	inttoshort[1]=PID_Calc(&PidYawSpeed);
	YawCurrent=(short)inttoshort[1];
	k_yaw=YawCurrent/((1024-rc.ch2)*0.0010f);
	/* pid_test 负数*/
//	test_gyro_pitch = -Gimbal.Pitch.Gyro;
//	test_gyro_yaw = -Gimbal.Yaw.Gyro;
}
/**********************************************************************************************************
*函 数 名: FuzzyGimbal_Act_Cal
*功能说明: 模糊云台正常模式(IMU角)
*形    参: rc  mouse  Pc_RecvData
*返 回 值: 无
**********************************************************************************************************/

void FuzzyGyroGimbal_Act_Cal(Remote rc,Mouse mouse)
{
	if( GimbalAct_Init_Flag!=Gimbal_Act_Mode)
	{
		Laser_On();
		GimbalPitchPos = Gimbal.Pitch.Gyro;				
		GimbalYawPos = Gimbal.Yaw.Gyro;
		GimbalAct_Init_Flag=Gimbal_Act_Mode;
	}

	
	if(Status.ControlMode==Control_RC_Mode)//Rc_Control
	{
	  GimbalYawPos   = (rc.ch2 == 1024) ? Gimbal.Yaw.Gyro: ( GimbalYawPos+(1024-rc.ch2)*0.0010f);
	  GimbalPitchPos = (rc.ch3 == 1024) ? Gimbal.Pitch.Gyro: ( GimbalPitchPos-(1024-rc.ch3)*0.0005f);//旧陀螺仪
		FF_w.Now_DeltIn = (1024-rc.ch2)*0.0005f;
	}
	if(Status.ControlMode==Control_MouseKey_Mode)//Mouse_Key
	{
    GimbalPitchPos -= mouse.y*0.005f;
    GimbalYawPos   -= mouse.x*0.005f;
		GimbalPitchPos -= mouse.z*0.001f;
		FF_w.Now_DeltIn = -mouse.x*0.005f;
	}
	
	
	GimbalPitchPos=LIMIT_MAX_MIN(GimbalPitchPos,Infantry.pitch_max_gyro,Infantry.pitch_min_gyro);//限位(用电机角度)	//限住大小，因此可用MotorTransangle来pid
	/***********************************************************************************/
	PidPitchPos.SetPoint = GimbalPitchPos;
	PidYawPos.SetPoint = GimbalYawPos;
	
  /**************************************计算电流值**************************************/
//PITCH                
////PITCH                 
	PidPitchPos.ActualValue = Gimbal.Pitch.Gyro;
	PidPitchSpeed.SetPoint=PID_Calc(&PidPitchPos); 
  PidPitchSpeed.ActualValue = GyroReceive.GY;
	inttoshort[0]=(PID_Calc(&PidPitchSpeed));//旧陀螺仪
	PitchCurrent=(short)(-inttoshort[0])*Infantry.motor_pn;

//YAW
	PidYawPos.ActualValue = Gimbal.Yaw.Gyro;
	PidYawSpeed.SetPoint=PID_Calc(&PidYawPos);					//需要YAW轴陀螺仪角度做ActualValue
	PidYawSpeed.ActualValue = GyroReceive.GZ;
	inttoshort[1]=PID_Calc(&PidYawSpeed);
	YawCurrent=(short)inttoshort[1];

	
}

/**********************************************************************************************************
*函 数 名: Gimbal_Armor_Cal
*功能说明: 云台辅瞄模式
*形    参: rc  mouse  Pc_RecvData
*返 回 值: 无
**********************************************************************************************************/
float speed_limit = 30.0f;
char Aim_Follow;
float Inte_z;
extern float aim_yaw, aim_pitch;
float last_aim_yaw;

void Gimbal_Armor_Cal(Remote rc,Mouse mouse)
{
	if(GimbalAct_Init_Flag!=Gimbal_Armor_Mode)
	{
		GimbalAct_Init_Flag=Gimbal_Armor_Mode;
		last_aim_yaw = GimbalYawPos = Gimbal.Yaw.Gyro;
	  GimbalPitchPos = Gimbal.Pitch.MotorTransAngle;
		aim_yaw = Gimbal.Yaw.Gyro;
		aim_pitch = Gimbal.Pitch.MotorTransAngle;
		Inte_z = 0;
	}

	Recent_Pitch_Angle_Armor = aim_pitch;
	Recent_Yaw_Angle_Armor = aim_yaw;
	Inte_z += mouse.z*0.002f;
	
	if(ABS(Recent_Yaw_Angle_Armor - Gimbal.Yaw.Gyro) < 70 && ABS(Recent_Pitch_Angle_Armor - (Gimbal.Pitch.MotorTransAngle))< 60)		//程序安全
	{
				GimbalPitchPos = Recent_Pitch_Angle_Armor + Inte_z;;	//更新值
				GimbalYawPos = Recent_Yaw_Angle_Armor;
		    FF_w.Now_DeltIn = ((GimbalYawPos - last_aim_yaw)>0.2)?((GimbalYawPos - last_aim_yaw)):(0);
	}
	else
	{
				GimbalPitchPos = Gimbal.Pitch.MotorTransAngle;	//更新值
				GimbalYawPos = Gimbal.Yaw.Gyro;
				FF_w.Now_DeltIn = 0;
	}
	
	last_aim_yaw = GimbalYawPos;  
	GimbalPitchPos=LIMIT_MAX_MIN(GimbalPitchPos,Infantry.pitch_max_motor,Infantry.pitch_min_motor);//限位(用电机角度)	//限住大小，因此可用MotorTransangle来pid
	/***********************************************************************************/
	FuzzyAidPidPitchPos.SetPoint = GimbalPitchPos;
	FuzzyAidPidPitchPos.ActualValue = Gimbal.Pitch.MotorTransAngle;
	
  PidPitchAidSpeed.SetPoint=FuzzyPID_Calc(&FuzzyAidPidPitchPos);   	
	PidPitchAidSpeed.ActualValue=GyroReceive.GY;
	inttoshort[0]=(PID_Calc(&PidPitchAidSpeed));//旧陀螺仪
	PitchCurrent=-(short)inttoshort[0]*Infantry.motor_pn;
		
	FuzzyAidPidYawPos.SetPoint = GimbalYawPos;
	FuzzyAidPidYawPos.ActualValue = Gimbal.Yaw.Gyro;
	
	PidYawAidSpeed.SetPoint=FuzzyPID_Calc(&FuzzyAidPidYawPos); 
	PidYawAidSpeed.ActualValue = GyroReceive.GZ;
	inttoshort[1] = PID_Calc(&PidYawAidSpeed);
	YawCurrent = inttoshort[1];
}


/**********************************************************************************************************
*函 数 名: Gimbal_Buff_Cal
*功能说明: 云台打符模式
*形    参: rc  mouse  Pc_RecvData
*返 回 值: 无
**********************************************************************************************************/
void Gimbal_Buff_Cal(Remote rc,Mouse mouse)
{
	if(GimbalAct_Init_Flag!=Gimbal_BigBuf_Mode)
	{
		GimbalAct_Init_Flag=Gimbal_BigBuf_Mode;
		GimbalYawPos = Gimbal.Yaw.MotorTransAngle;
	  GimbalPitchPos=Gimbal.Pitch.MotorTransAngle;
		aim_yaw = 0;
		aim_pitch = Gimbal.Pitch.MotorTransAngle;
	}


	Recent_Pitch_Angle_Armor = aim_pitch;
	Recent_Yaw_Angle_Armor = aim_yaw+Buff_Yaw_Motor;
	
	if(ABS(Recent_Yaw_Angle_Armor - Gimbal.Yaw.MotorTransAngle) < 70 && ABS(Recent_Pitch_Angle_Armor - Gimbal.Pitch.MotorTransAngle)< 60)		//程序安全
	{
				GimbalPitchPos = Recent_Pitch_Angle_Armor;	//更新值
				GimbalYawPos = Recent_Yaw_Angle_Armor;
	}
	else
	{
				GimbalPitchPos = Gimbal.Pitch.MotorTransAngle;	//更新值
				GimbalYawPos = Gimbal.Yaw.MotorTransAngle;
	}
	
	GimbalPitchPos = LIMIT_MAX_MIN(GimbalPitchPos,Infantry.pitch_max_motor,Infantry.pitch_min_motor);//限位
	/***************************************************************************************/
	FuzzyBuffPidPitchPos.SetPoint = GimbalPitchPos;
	FuzzyBuffPidPitchPos.ActualValue = Gimbal.Pitch.MotorTransAngle;
	
  PidPitchBuffSpeed.SetPoint=FuzzyPID_Calc(&FuzzyBuffPidPitchPos);
  PidPitchBuffSpeed.ActualValue = GyroReceive.GY;
	inttoshort[0]=(PID_Calc(&PidPitchBuffSpeed));//旧陀螺仪
	PitchCurrent=-(short)inttoshort[0]*Infantry.motor_pn;
		
	FuzzyBuffPidYawPos.SetPoint = GimbalYawPos;
	FuzzyBuffPidYawPos.ActualValue = Gimbal.Yaw.MotorTransAngle;
	
	PidYawBuffSpeed.SetPoint=FuzzyPID_Calc(&FuzzyBuffPidYawPos); 
	PidYawBuffSpeed.ActualValue = GyroReceive.GZ;
	inttoshort[1] = PID_Calc(&PidYawBuffSpeed);
	YawCurrent = inttoshort[1];
}

///**********************************************************************************************************
//*函 数 名: Gimbal_DropShot_Cal
//*功能说明: 云台吊射模式
//*形    参: rc  mouse  Pc_RecvData
//*返 回 值: 无
//**********************************************************************************************************/

//void Gimbal_DropShot_Cal(Remote rc,Mouse mouse)
//{
//	if( GimbalAct_Init_Flag!=Gimbal_DropShot_Mode)
//	{
//		Laser_On();
//		GimbalAct_Init_Flag=Gimbal_DropShot_Mode;
//		GimbalYawPos = Gimbal.Yaw.MotorTransAngle;//定轴模式
//		GimbalPitchPos = Gimbal.Pitch.MotorTransAngle;
//	}
//	
//	if(Status.ControlMode==Control_RC_Mode)//Rc_Control
//	{
//	  GimbalYawPos   += (1024-rc.ch2)*0.0005f;
//		GimbalPitchPos -= (1024-rc.ch3)*0.0005f;//陀螺仪
//	}
//	if(Status.ControlMode==Control_MouseKey_Mode)//Mouse_Key
//	{
//    GimbalPitchPos -= mouse.y*0.005f;
//    GimbalYawPos   -= mouse.x*0.005f;
//	}
//	
//	GimbalPitchPos=LIMIT_MAX_MIN(GimbalPitchPos,Infantry.pitch_max_motor,Infantry.pitch_min_motor);//限位(用电机角度)
//  /***********************************************************************************/
//	PidPitchPos.SetPoint = GimbalPitchPos;
//	PidYawPos.SetPoint = GimbalYawPos;
//  /**************************************计算电流值**************************************/
//	PidPitchSpeed.SetPoint=PID_Calc(&PidPitchPos,Gimbal.Pitch.MotorTransAngle);  
//	inttoshort[0]=-(PID_Calc(&PidPitchSpeed,GyroReceive.GY));//旧陀螺仪
//	PitchCurrent=(short)inttoshort[0];
//	
//	PidYawSpeed.SetPoint=PID_Calc(&PidYawPos,Gimbal.Yaw.MotorTransAngle);	
//	PidYawSpeed.SetPoint=LIMIT_MAX_MIN(PidYawSpeed.SetPoint,5.5f,-5.5f);    
//	inttoshort[1]=PID_Calc(&PidYawSpeed,GyroReceive.GZ);
//	YawCurrent=inttoshort[1];	
//}

/**********************************************************************************************************
*函 数 名: Gimbal_Test_Cal
*功能说明: 云台测试模式
*形    参: rc  mouse  Pc_RecvData
*返 回 值: 无
**********************************************************************************************************/
void Gimbal_Test_Cal(Remote rc,Mouse mouse)
{
	if(GimbalAct_Init_Flag!=Gimbal_Test_Mode)
	{
		GimbalAct_Init_Flag=Gimbal_Test_Mode;
		GimbalYawPos = Gimbal.Yaw.MotorTransAngle;
	  GimbalPitchPos=Gimbal.Pitch.MotorTransAngle;
	}
	if(Status.ControlMode==Control_RC_Mode)//Rc_Control
	{
	  GimbalYawPos   += (1024-rc.ch2)*0.00005f;
		GimbalPitchPos -= (1024-rc.ch3)*0.00005f;//陀螺仪
	}
	
	GimbalPitchPos = LIMIT_MAX_MIN(GimbalPitchPos,Infantry.pitch_max_motor,Infantry.pitch_min_motor);//限位
	/***************************************************************************************/
	PidPitchAidPos.SetPoint = GimbalPitchPos;
	PidPitchAidPos.ActualValue = Gimbal.Pitch.MotorTransAngle;
	PidPitchAidSpeed.SetPoint=PID_Calc(&PidPitchAidPos);  	
	PidPitchAidSpeed.ActualValue = GyroReceive.GY;
	inttoshort[0]=(PID_Calc(&PidPitchAidSpeed));//旧陀螺仪
	PitchCurrent=-(short)inttoshort[0]*Infantry.motor_pn;
		
	PidYawAidPos.SetPoint = GimbalYawPos;
	PidYawAidPos.ActualValue = Gimbal.Yaw.MotorTransAngle;
	PidYawAidSpeed.SetPoint=PID_Calc(&PidYawAidPos); 
	PidYawAidSpeed.ActualValue = GyroReceive.GZ;
	inttoshort[1] = PID_Calc(&PidYawAidSpeed);
	YawCurrent = inttoshort[1];
	
//	PidPitchPos.SetPoint = GimbalPitchPos;
//	PidPitchSpeed.SetPoint=PID_Calc(&PidPitchPos,Gimbal.Pitch.MotorTransAngle);  	
//	inttoshort[0]=(PID_Calc(&PidPitchSpeed,GyroReceive.GY));//旧陀螺仪
//	PitchCurrent=-(short)inttoshort[0];
//		
//	PidYawPos.SetPoint = GimbalYawPos;
//	PidYawSpeed.SetPoint=PID_Calc(&PidYawPos,Gimbal.Yaw.MotorTransAngle); 
//	inttoshort[1] = PID_Calc(&PidYawSpeed,GyroReceive.GZ);
//	YawCurrent = inttoshort[1];
}

/**********************************************************************************************************
*函 数 名: Gimbal_SI_Cal
*功能说明: 云台系统辨识模式
*形    参: rc  mouse  Pc_RecvData
*返 回 值: 无
**********************************************************************************************************/
short F_Change_flag = 0;		//切换频率标志
float	Gimbal_direct;

int T;//周期
int T_cnt = 0;//计数
int T_Time_cnt = 0;//周期次数计数
int F_cnt = 0,F_cnt_last;//指向F的指针
float F = 1;

void Gimbal_SI_Cal(float Gimbal_pitch,float Gimbal_yaw)
{
	if(GimbalAct_Init_Flag!=Gimbal_SI_Mode)
	{
		GimbalPitchPos = 0;				
		GimbalYawPos = Gimbal.Yaw.Gyro;
  	GimbalPitchPos = Gimbal.Pitch.MotorTransAngle;   //用于阶跃响应测试				
  	GimbalYawPos = Gimbal.Yaw.Gyro +40.0f ;
		F = 1;		
		GimbalAct_Init_Flag = Gimbal_SI_Mode;
	}
		
	  T_change();		
//		GimbalPitchPos = LIMIT_MAX_MIN((Gimbal_direct*Gimbal_pitch),Infantry.pitch_max_motor,Infantry.pitch_min_motor);

  /**************************************计算电流值**************************************/	
	/***************************************************************************************/
	GimbalPitchPos=LIMIT_MAX_MIN(GimbalPitchPos,Infantry.pitch_max_motor,Infantry.pitch_min_motor);//限位(用电机角度)	//限住大小，因此可用MotorTransangle来pid
	/***********************************************************************************/
	FuzzyAidPidPitchPos.SetPoint = GimbalPitchPos;
	FuzzyAidPidPitchPos.ActualValue = Gimbal.Pitch.MotorTransAngle;
	
  PidPitchAidSpeed.SetPoint=FuzzyPID_Calc(&FuzzyAidPidPitchPos);
  PidPitchAidSpeed.ActualValue=	GyroReceive.GY;
	inttoshort[0]=(PID_Calc(&PidPitchAidSpeed));//旧陀螺仪
	PitchCurrent=-(short)inttoshort[0]*Infantry.motor_pn;
		
	FuzzyAidPidYawPos.SetPoint = GimbalYawPos;
	FuzzyAidPidYawPos.ActualValue = Gimbal.Yaw.Gyro;
	
	PidYawAidSpeed.SetPoint=FuzzyPID_Calc(&FuzzyAidPidYawPos);
  PidYawAidSpeed.ActualValue=	GyroReceive.GZ;
	inttoshort[1] = PID_Calc(&PidYawAidSpeed);
	YawCurrent = inttoshort[1];
	
///************************************** 用于开环系统辨识 ******************************************/	
//  	PitchCurrent= Gimbal_direct*Gimbal_pitch;
//	  YawCurrent = Gimbal_direct*Gimbal_yaw;
}

void get_F(void)
{
	static int j = 0;
	if(F < 22)
	{
		j++;
		F += 0.5f;
	}
	else if(F == 22)
	{
		F = 24;
	}
	else if(F >= 24&&F < 40)
	{
		F = F + 2;
	}
	else if(F == 40)
	{
		F = 50;
	}
	else if(F >= 50&&F < 120)
	{
		F = F + 10;
	}
	else if(F == 120)
	{
		F = 200;
	}
	else if(F == 250)
	{
		F = 333;
	}
	else if(F == 333)
	{
		F = 500;
	}
}
/**********************************************************************************************************
*函 数 名: T_Change
*功能说明: 根据不同的频率获得对应不同周期下sin信号,F低于50重复10周期，否则20周期
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void T_change(void)
{
	T = round(1000/F);
	if(GimbalAct_Init_Flag == Gimbal_SI_Mode)
	{
		if(F_Change_flag == 0)
		{
			Gimbal_direct = sin(2*3.14*T_cnt/T);
			if(T_cnt >= T)
			{
				T_cnt = 0;
				T_Time_cnt++;
			}
			T_cnt++;
			F_cnt_last = F_cnt;

			if(T_Time_cnt >= 10 && F <22)
			{
				T_Time_cnt = 0;
				F_cnt++; 
			}
			else if(T_Time_cnt >= 10 && F >= 22 && F <50)
			{
				T_Time_cnt = 0;
				F_cnt++; 
			}
			else if(T_Time_cnt >= 20 && F >= 50)
			{
				T_Time_cnt = 0;
				F_cnt++; 
			}
			if(F_cnt != F_cnt_last)		//频率改变
			{
				F_Change_flag  = 1;
			}
		}
		else if(F_Change_flag == 1)
		{
			Gimbal_direct = 0;
			if((ABS(PidPitchPos.SetPoint - PidPitchPos.ActualValue) < 0.2f) || (ABS(PidYawPos.SetPoint - PidYawPos.ActualValue) < 0.2f))		//回到初始位置
			{
				get_F();
				T_cnt = 0;
				F_Change_flag = 0;
			}
		}
	}
	else if(GimbalAct_Init_Flag != Gimbal_SI_Mode)
	{
		F = 1;
	}
}


/**********************************************************************************************************
*函 数 名: Gimbal_CurrentPid_Cal
*功能说明: 发送电流值
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void Gimbal_CurrentPid_Cal(void)
{
	switch (Status.GimbalMode)
	{
		case Gimbal_Powerdown_Mode:
			Gimbal_Powerdown_Cal();
			break;
		
		case Gimbal_Act_Mode:
			FuzzyMotorGimbal_Act_Cal(RC_Ctl.rc,RC_Ctl.mouse);
			break;  
		
		case Gimbal_Armor_Mode:
			Gimbal_Armor_Cal(RC_Ctl.rc,RC_Ctl.mouse);
			break;
		
		case Gimbal_BigBuf_Mode:
		case Gimbal_SmlBuf_Mode:
			Gimbal_Buff_Cal(RC_Ctl.rc,RC_Ctl.mouse);
			break;
		
//		case Gimbal_DropShot_Mode:
//			Gimbal_DropShot_Cal(RC_Ctl.rc,RC_Ctl.mouse);
//			break; 
		
		case Gimbal_SI_Mode:
			Gimbal_SI_Cal(8000.0, 0);			//pitch
//			Gimbal_SI_Cal(0.0, 30.0);		//yaw
			break;
		
		case Gimbal_Jump_Mode:
			FuzzyGyroGimbal_Act_Cal(RC_Ctl.rc,RC_Ctl.mouse);
		  break;
		
		case Gimbal_Test_Mode:
			Gimbal_Test_Cal(RC_Ctl.rc,RC_Ctl.mouse);
			break;
		
		default:
				Gimbal_Powerdown_Cal();	
			break;
	}
	if(q_flag) F405.Gimbal_Flag = 7;     //云台自己打弹
	else       F405.Gimbal_Flag = Status.GimbalMode;
	
	if(!pitch_lose_flag)  //防堵转
	{
	GimbalCan2Send(PitchCurrent,YawCurrent);
	}
	else
	{
	 GimbalCan2Send(0,YawCurrent);
	}
}


/**********************************************************************************************************
*函 数 名: Pid_Yaw_MotorPos_GyroSpeed
*功能说明: Yaw轴辅瞄双环PID
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void PidGimbalMotor_Init(void)
{ 
#if Robot_ID == 3
/********************************************* 3号车 ********************************************************/	
 //手动pitch双环
  PidPitchPos.P=0.25f;       //手动pitch角度环
	PidPitchPos.I=0.000f;
	PidPitchPos.D=0.0f;       
	PidPitchPos.IMax=10.0f;
	PidPitchPos.SetPoint=0.0f;
	PidPitchPos.OutMax=5.5f;	
	PidPitchPos.I_L = 0.2f;
	PidPitchPos.I_U = 0.4f;
	PidPitchPos.RC_DF = 0.5f;
	
  PidPitchSpeed.P=10000.0f;  //手动pitch速度环
	PidPitchSpeed.I=12.0f;  
	PidPitchSpeed.D=0.0f;
	PidPitchSpeed.IMax=550.0f;
	PidPitchSpeed.SetPoint=0.0f;
	PidPitchSpeed.OutMax=30000.0f;
	PidPitchSpeed.I_L = 0.3f;
	PidPitchSpeed.I_U = 0.7f;
	PidPitchSpeed.RC_DF = 0.5f;
  
	
	//手动yaw双环                                 // 3号车
  PidYawPos.P=0.22f;        //手动yaw角度环
	PidYawPos.I=0.00f;
	PidYawPos.D=0.0f;       
	PidYawPos.IMax=10.0f;
	PidYawPos.SetPoint=0.0f;
	PidYawPos.OutMax=5.5f;
	PidYawPos.I_L = 0.2f;
	PidYawPos.I_U = 0.4f;
	PidYawPos.RC_DF = 0.5f;
	
	PidYawSpeed.P=22000.0f;   //手动yaw速度环
	PidYawSpeed.I=1.0f;
	PidYawSpeed.D=0.0f;
	PidYawSpeed.IMax=2000.0f;
	PidYawSpeed.SetPoint=0.0f;
	PidYawSpeed.OutMax=30000.0f;
	PidYawSpeed.I_L = 0.3f;
	PidYawSpeed.I_U = 0.7f;
	PidYawSpeed.RC_DF = 0.5f;
	

//辅瞄yaw
	PidYawAidPos.P =0.25f;	  //yaw PID位置环（辅瞄）
	PidYawAidPos.I=0.000f; 
	PidYawAidPos.D=0.20f;
	PidYawAidPos.IMax=40.0f;
	PidYawAidPos.SetPoint=0.0f;
	PidYawAidPos.OutMax=5.5f;
	PidYawAidPos.I_L = 0.2f;
	PidYawAidPos.I_U = 0.4f;
	PidYawAidPos.RC_DF = 0.5f;
	                              
	FuzzyAidPidYawPos.Kp0=0.25f;   //模糊PID yaw角度环（辅瞄）
	FuzzyAidPidYawPos.Ki0=0.0001f;  
	FuzzyAidPidYawPos.Kd0=0.1f;
	FuzzyAidPidYawPos.IMax=40.0f;
	FuzzyAidPidYawPos.SetPoint=0.0f;	
	FuzzyAidPidYawPos.OutMax=8.5f;
  FuzzyAidPidYawPos.I_L = 0.2f;
	FuzzyAidPidYawPos.I_U = 0.4f;
	FuzzyAidPidYawPos.RC_DF = 0.5f;
	
	FuzzyAidPidYawPos.stair = 0.2f;
	FuzzyAidPidYawPos.Kp_stair = 0.015f;
	FuzzyAidPidYawPos.Ki_stair = 0.0005f;
	FuzzyAidPidYawPos.Kd_stair = 0.001f;
	
	
	PidYawAidSpeed.P=10000.0f;			//32000  yaw速度环PID(辅瞄)
	PidYawAidSpeed.I=3.0f; 
	PidYawAidSpeed.D=0.0f;
	PidYawAidSpeed.IMax=2000.0f;
	PidYawAidSpeed.SetPoint=0.0f;
	PidYawAidSpeed.OutMax=30000.0f;
	PidYawAidSpeed.I_L = 0.3f;
	PidYawAidSpeed.I_U = 0.7f;
	PidYawAidSpeed.RC_DF = 0.5f;		
		//辅瞄pitch
	PidPitchAidPos.P =0.2f;	  //普通PID位置环(辅瞄)
	PidPitchAidPos.I=0.00001f; 
	PidPitchAidPos.D=0.0f;
	PidPitchAidPos.IMax=40.0f;
	PidPitchAidPos.SetPoint=0.0f;
	PidPitchAidPos.OutMax=5.5f;
	PidPitchAidPos.I_L = 0.2f;
	PidPitchAidPos.I_U = 0.4f;
	PidPitchAidPos.RC_DF = 0.5f;
	
	FuzzyAidPidPitchPos.Kp0=0.28f;
	FuzzyAidPidPitchPos.Ki0=0.00f;  //模糊PID位置环（辅瞄）
	FuzzyAidPidPitchPos.Kd0=0.1f;
	FuzzyAidPidPitchPos.IMax=40.0f;
	FuzzyAidPidPitchPos.SetPoint=0.0f;
	FuzzyAidPidPitchPos.OutMax=8.5f;
	FuzzyAidPidPitchPos.I_L = 0.2f;
	FuzzyAidPidPitchPos.I_U = 0.4f;
	FuzzyAidPidPitchPos.RC_DF = 0.5f;
	
	FuzzyAidPidPitchPos.stair = 0.2f;
	FuzzyAidPidPitchPos.Kp_stair = 0.015f;
	FuzzyAidPidPitchPos.Ki_stair = 0.0005f;
	FuzzyAidPidPitchPos.Kd_stair = 0.001f;
	
  PidPitchAidSpeed.P=10000.0f;	  //速度环PID（辅瞄）
	PidPitchAidSpeed.I=18.0f; 
	PidPitchAidSpeed.D=0.0f;
	PidPitchAidSpeed.IMax=250.0f;
	PidPitchAidSpeed.SetPoint=0.0f;
	PidPitchAidSpeed.OutMax=30000.0f;
	PidPitchAidSpeed.I_L = 0.3f;
	PidPitchAidSpeed.I_U = 0.7f;
	PidPitchAidSpeed.RC_DF = 0.5f;

	
	//大符 Pitch
	FuzzyBuffPidPitchPos.Kp0=0.28f;
	FuzzyBuffPidPitchPos.Ki0=0.003f;  //模糊PID位置环（辅瞄）
	FuzzyBuffPidPitchPos.Kd0=0.0f;
	FuzzyBuffPidPitchPos.IMax=40.0f;
	FuzzyBuffPidPitchPos.SetPoint=0.0f;
	FuzzyBuffPidPitchPos.OutMax=8.5f;
	FuzzyBuffPidPitchPos.I_L = 0.2f;
	FuzzyBuffPidPitchPos.I_U = 0.4f;
	FuzzyBuffPidPitchPos.RC_DF = 0.5f;
	
	FuzzyBuffPidPitchPos.stair = 0.2f;
	FuzzyBuffPidPitchPos.Kp_stair = 0.015f;
	FuzzyBuffPidPitchPos.Ki_stair = 0.0005f;
	FuzzyBuffPidPitchPos.Kd_stair = 0.001f;
	
  PidPitchBuffSpeed.P=10000.0f;	  //速度环PID（辅瞄）
	PidPitchBuffSpeed.I=15.0f; 
	PidPitchBuffSpeed.D=0.0f;
	PidPitchBuffSpeed.IMax=250.0f;
	PidPitchBuffSpeed.SetPoint=0.0f;
	PidPitchBuffSpeed.OutMax=30000.0f;
	PidPitchBuffSpeed.I_L = 0.3f;
	PidPitchBuffSpeed.I_U = 0.7f;
	PidPitchBuffSpeed.RC_DF = 0.5f;
	
	//大符 Yaw
	FuzzyBuffPidYawPos.Kp0=0.25f;   //模糊PID yaw角度环（辅瞄）
	FuzzyBuffPidYawPos.Ki0=0.0001f;  
	FuzzyBuffPidYawPos.Kd0=0.0f;
	FuzzyBuffPidYawPos.IMax=40.0f;
	FuzzyBuffPidYawPos.SetPoint=0.0f;	
	FuzzyBuffPidYawPos.OutMax=8.5f;
	FuzzyBuffPidYawPos.I_L = 0.2f;
	FuzzyBuffPidYawPos.I_U = 0.4f;
	FuzzyBuffPidYawPos.RC_DF = 0.5f;
	
	FuzzyBuffPidYawPos.stair = 0.2f;
	FuzzyBuffPidYawPos.Kp_stair = 0.015f;
	FuzzyBuffPidYawPos.Ki_stair = 0.0005f;
	FuzzyBuffPidYawPos.Kd_stair = 0.001f;
	
	PidYawBuffSpeed.P=10000.0f;			//  yaw速度环PID(辅瞄)
	PidYawBuffSpeed.I=5.0f; 
	PidYawBuffSpeed.D=0.0f;
	PidYawBuffSpeed.IMax=2000.0f;
	PidYawBuffSpeed.SetPoint=0.0f;
	PidYawBuffSpeed.OutMax=30000.0f;
	PidYawBuffSpeed.I_L = 0.3f;
	PidYawBuffSpeed.I_U = 0.7f;
	PidYawBuffSpeed.RC_DF = 0.5f;


#elif Robot_ID == 4
/********************************************* 4号车 ********************************************************/	
 //手动pitch双环
  PidPitchPos.P=0.25f;       //手动pitch角度环
	PidPitchPos.I=0.01f;
	PidPitchPos.D=0.0f;       
	PidPitchPos.IMax=10.0f;
	PidPitchPos.SetPoint=0.0f;
	PidPitchPos.OutMax=5.5f;	
	PidPitchPos.I_L = 0.2f;
	PidPitchPos.I_U = 0.4f;
	PidPitchPos.RC_DF = 0.5f;
	
  PidPitchSpeed.P=12000.0f;  //手动pitch速度环
	PidPitchSpeed.I=7.0f;  
	PidPitchSpeed.D=0.0f;
	PidPitchSpeed.IMax=550.0f;
	PidPitchSpeed.SetPoint=0.0f;
	PidPitchSpeed.OutMax=30000.0f;
	PidPitchSpeed.I_L = 0.3f;
	PidPitchSpeed.I_U = 0.7f;
	PidPitchSpeed.RC_DF = 0.5f;
  
	
	//手动yaw双环                                 // 4号车
  PidYawPos.P=0.27f;        //手动yaw角度环
	PidYawPos.I=0.00f;
	PidYawPos.D=0.0f;       
	PidYawPos.IMax=10.0f;
	PidYawPos.SetPoint=0.0f;
	PidYawPos.OutMax=5.5f;
	PidYawPos.I_L = 0.2f;
	PidYawPos.I_U = 0.4f;
	PidYawPos.RC_DF = 0.5f;
	
	PidYawSpeed.P=22000.0f;   //手动yaw速度环
	PidYawSpeed.I=5.0f;
	PidYawSpeed.D=0.0f;
	PidYawSpeed.IMax=2000.0f;
	PidYawSpeed.SetPoint=0.0f;
	PidYawSpeed.OutMax=30000.0f;
	PidYawSpeed.I_L = 0.3f;
	PidYawSpeed.I_U = 0.7f;
	PidYawSpeed.RC_DF = 0.5f;
	

//辅瞄yaw
	PidYawAidPos.P =0.25f;	  //yaw PID位置环（辅瞄）
	PidYawAidPos.I=0.000f; 
	PidYawAidPos.D=0.0f;
	PidYawAidPos.IMax=40.0f;
	PidYawAidPos.SetPoint=0.0f;
	PidYawAidPos.OutMax=5.5f;
	PidYawAidPos.I_L = 0.2f;
	PidYawAidPos.I_U = 0.4f;
	PidYawAidPos.RC_DF = 0.5f;
	                              
	FuzzyAidPidYawPos.Kp0=0.25f;   //模糊PID yaw角度环（辅瞄）
	FuzzyAidPidYawPos.Ki0=0.0000f;  
	FuzzyAidPidYawPos.Kd0=0.0f;
	FuzzyAidPidYawPos.IMax=40.0f;
	FuzzyAidPidYawPos.SetPoint=0.0f;	
	FuzzyAidPidYawPos.OutMax=8.5f;
  FuzzyAidPidYawPos.I_L = 0.2f;
	FuzzyAidPidYawPos.I_U = 0.4f;
	FuzzyAidPidYawPos.RC_DF = 0.5f;
	
	FuzzyAidPidYawPos.stair = 0.2f;
	FuzzyAidPidYawPos.Kp_stair = 0.015f;
	FuzzyAidPidYawPos.Ki_stair = 0.0005f;
	FuzzyAidPidYawPos.Kd_stair = 0.001f;
	
	
	PidYawAidSpeed.P=10000.0f;			//32000  yaw速度环PID(辅瞄)
	PidYawAidSpeed.I=5.0f; 
	PidYawAidSpeed.D=0.0f;
	PidYawAidSpeed.IMax=2000.0f;
	PidYawAidSpeed.SetPoint=0.0f;
	PidYawAidSpeed.OutMax=30000.0f;
	PidYawAidSpeed.I_L = 0.3f;
	PidYawAidSpeed.I_U = 0.7f;
	PidYawAidSpeed.RC_DF = 0.5f;		
		//辅瞄pitch
	PidPitchAidPos.P =0.25f;	  //普通PID位置环(辅瞄)
	PidPitchAidPos.I=0.003f; 
	PidPitchAidPos.D=0.0f;
	PidPitchAidPos.IMax=40.0f;
	PidPitchAidPos.SetPoint=0.0f;
	PidPitchAidPos.OutMax=5.5f;
	PidPitchAidPos.I_L = 0.2f;
	PidPitchAidPos.I_U = 0.4f;
	PidPitchAidPos.RC_DF = 0.5f;
	
	FuzzyAidPidPitchPos.Kp0=0.23f;
	FuzzyAidPidPitchPos.Ki0=0.003f;  //模糊PID位置环（辅瞄）
	FuzzyAidPidPitchPos.Kd0=0.0f;
	FuzzyAidPidPitchPos.IMax=40.0f;
	FuzzyAidPidPitchPos.SetPoint=0.0f;
	FuzzyAidPidPitchPos.OutMax=8.5f;
	FuzzyAidPidPitchPos.I_L = 0.2f;
	FuzzyAidPidPitchPos.I_U = 0.4f;
	FuzzyAidPidPitchPos.RC_DF = 0.5f;
	
	FuzzyAidPidPitchPos.stair = 0.2f;
	FuzzyAidPidPitchPos.Kp_stair = 0.015f;
	FuzzyAidPidPitchPos.Ki_stair = 0.0005f;
	FuzzyAidPidPitchPos.Kd_stair = 0.001f;
	
  PidPitchAidSpeed.P=10000.0f;	  //速度环PID（辅瞄）
	PidPitchAidSpeed.I=15.0f; 
	PidPitchAidSpeed.D=0.0f;
	PidPitchAidSpeed.IMax=250.0f;
	PidPitchAidSpeed.SetPoint=0.0f;
	PidPitchAidSpeed.OutMax=30000.0f;
	PidPitchAidSpeed.I_L = 0.3f;
	PidPitchAidSpeed.I_U = 0.7f;
	PidPitchAidSpeed.RC_DF = 0.5f;

//大符 Pitch
	FuzzyBuffPidPitchPos.Kp0=0.28f;
	FuzzyBuffPidPitchPos.Ki0=0.003f;  //模糊PID位置环（辅瞄）
	FuzzyBuffPidPitchPos.Kd0=0.0f;
	FuzzyBuffPidPitchPos.IMax=40.0f;
	FuzzyBuffPidPitchPos.SetPoint=0.0f;
	FuzzyBuffPidPitchPos.OutMax=8.5f;
	FuzzyBuffPidPitchPos.I_L = 0.2f;
	FuzzyBuffPidPitchPos.I_U = 0.4f;
	FuzzyBuffPidPitchPos.RC_DF = 0.5f;
	
	FuzzyBuffPidPitchPos.stair = 0.2f;
	FuzzyBuffPidPitchPos.Kp_stair = 0.015f;
	FuzzyBuffPidPitchPos.Ki_stair = 0.0005f;
	FuzzyBuffPidPitchPos.Kd_stair = 0.001f;
	
  PidPitchBuffSpeed.P=10000.0f;	  //速度环PID（辅瞄）
	PidPitchBuffSpeed.I=15.0f; 
	PidPitchBuffSpeed.D=0.0f;
	PidPitchBuffSpeed.IMax=250.0f;
	PidPitchBuffSpeed.SetPoint=0.0f;
	PidPitchBuffSpeed.OutMax=30000.0f;
	PidPitchBuffSpeed.I_L = 0.3f;
	PidPitchBuffSpeed.I_U = 0.7f;
	PidPitchBuffSpeed.RC_DF = 0.5f;
	
	//大符 Yaw
	FuzzyBuffPidYawPos.Kp0=0.30f;   //模糊PID yaw角度环（辅瞄）
	FuzzyBuffPidYawPos.Ki0=0.0001f;  
	FuzzyBuffPidYawPos.Kd0=0.0f;
	FuzzyBuffPidYawPos.IMax=40.0f;
	FuzzyBuffPidYawPos.SetPoint=0.0f;	
	FuzzyBuffPidYawPos.OutMax=8.5f;
	FuzzyBuffPidYawPos.I_L = 0.2f;
	FuzzyBuffPidYawPos.I_U = 0.4f;
	FuzzyBuffPidYawPos.RC_DF = 0.5f;
	
	FuzzyBuffPidYawPos.stair = 0.2f;
	FuzzyBuffPidYawPos.Kp_stair = 0.015f;
	FuzzyBuffPidYawPos.Ki_stair = 0.0005f;
	FuzzyBuffPidYawPos.Kd_stair = 0.001f;
	
	PidYawBuffSpeed.P=12000.0f;			//  yaw速度环PID(辅瞄)
	PidYawBuffSpeed.I=5.0f; 
	PidYawBuffSpeed.D=0.0f;
	PidYawBuffSpeed.IMax=2000.0f;
	PidYawBuffSpeed.SetPoint=0.0f;
	PidYawBuffSpeed.OutMax=30000.0f;
	PidYawBuffSpeed.I_L = 0.3f;
	PidYawBuffSpeed.I_U = 0.7f;
	PidYawBuffSpeed.RC_DF = 0.5f;
	
#elif Robot_ID == 14
/********************************************* 14号车 ********************************************************/	
  //手动pitch双环
  PidPitchPos.P=0.35f;       //手动pitch角度环
	PidPitchPos.I=0.0001f;
	PidPitchPos.D=0.0f;       
	PidPitchPos.IMax=10.0f;
	PidPitchPos.SetPoint=0.0f;
	PidPitchPos.OutMax=5.5f;	
	PidPitchPos.I_L = 0.2f;
	PidPitchPos.I_U = 0.4f;
	PidPitchPos.RC_DF = 0.5f;
	
  PidPitchSpeed.P=12000.0f;  //手动pitch速度环
	PidPitchSpeed.I=12.0f;  
	PidPitchSpeed.D=0.0f;
	PidPitchSpeed.IMax=550.0f;
	PidPitchSpeed.SetPoint=0.0f;
	PidPitchSpeed.OutMax=30000.0f;
	PidPitchSpeed.I_L = 0.3f;
	PidPitchSpeed.I_U = 0.7f;
	PidPitchSpeed.RC_DF = 0.5f;
  
	
	//手动yaw双环                                 // 5号车
  PidYawPos.P=0.22f;        //手动yaw角度环
	PidYawPos.I=0.00f;
	PidYawPos.D=0.0f;       
	PidYawPos.IMax=10.0f;
	PidYawPos.SetPoint=0.0f;
	PidYawPos.OutMax=5.5f;
	PidYawPos.I_L = 0.2f;
	PidYawPos.I_U = 0.4f;
	PidYawPos.RC_DF = 0.5f;
	
	PidYawSpeed.P=22000.0f;   //手动yaw速度环
	PidYawSpeed.I=1.0f;
	PidYawSpeed.D=0.0f;
	PidYawSpeed.IMax=2000.0f;
	PidYawSpeed.SetPoint=0.0f;
	PidYawSpeed.OutMax=30000.0f;
	PidYawSpeed.I_L = 0.3f;
	PidYawSpeed.I_U = 0.7f;
	PidYawSpeed.RC_DF = 0.5f;
	

//辅瞄yaw
	PidYawAidPos.P =0.20f;	  //yaw PID位置环（辅瞄）
	PidYawAidPos.I=0.000f; 
	PidYawAidPos.D=0.20f;
	PidYawAidPos.IMax=40.0f;
	PidYawAidPos.SetPoint=0.0f;
	PidYawAidPos.OutMax=5.5f;
	PidYawAidPos.I_L = 0.2f;
	PidYawAidPos.I_U = 0.4f;
	PidYawAidPos.RC_DF = 0.5f;
	                              
	FuzzyAidPidYawPos.Kp0=0.35f;   //模糊PID yaw角度环（辅瞄）
	FuzzyAidPidYawPos.Ki0=0.0007f;  
	FuzzyAidPidYawPos.Kd0=0.0f;
	FuzzyAidPidYawPos.IMax=40.0f;
	FuzzyAidPidYawPos.SetPoint=0.0f;	
	FuzzyAidPidYawPos.OutMax=8.5f;
  FuzzyAidPidYawPos.I_L = 0.2f;
	FuzzyAidPidYawPos.I_U = 0.4f;
	FuzzyAidPidYawPos.RC_DF = 0.5f;
	
	FuzzyAidPidYawPos.stair = 0.2f;
	FuzzyAidPidYawPos.Kp_stair = 0.015f;
	FuzzyAidPidYawPos.Ki_stair = 0.0005f;
	FuzzyAidPidYawPos.Kd_stair = 0.001f;
	
	
	PidYawAidSpeed.P=10000.0f;			//32000  yaw速度环PID(辅瞄)
	PidYawAidSpeed.I=5.0f; 
	PidYawAidSpeed.D=0.0f;
	PidYawAidSpeed.IMax=2000.0f;
	PidYawAidSpeed.SetPoint=0.0f;
	PidYawAidSpeed.OutMax=30000.0f;
	PidYawAidSpeed.I_L = 0.3f;
	PidYawAidSpeed.I_U = 0.7f;
	PidYawAidSpeed.RC_DF = 0.5f;		
		//辅瞄pitch
	PidPitchAidPos.P =0.45f;	  //普通PID位置环(辅瞄)
	PidPitchAidPos.I=0.01f; 
	PidPitchAidPos.D=0.0f;
	PidPitchAidPos.IMax=40.0f;
	PidPitchAidPos.SetPoint=0.0f;
	PidPitchAidPos.OutMax=5.5f;
	PidPitchAidPos.I_L = 0.2f;
	PidPitchAidPos.I_U = 0.4f;
	PidPitchAidPos.RC_DF = 0.5f;
	
	FuzzyAidPidPitchPos.Kp0=0.45f;
	FuzzyAidPidPitchPos.Ki0=0.01f;  //模糊PID位置环（辅瞄）
	FuzzyAidPidPitchPos.Kd0=0.0f;
	FuzzyAidPidPitchPos.IMax=40.0f;
	FuzzyAidPidPitchPos.SetPoint=0.0f;
	FuzzyAidPidPitchPos.OutMax=8.5f;
	FuzzyAidPidPitchPos.I_L = 0.2f;
	FuzzyAidPidPitchPos.I_U = 0.4f;
	FuzzyAidPidPitchPos.RC_DF = 0.5f;
	
	FuzzyAidPidPitchPos.stair = 0.2f;
	FuzzyAidPidPitchPos.Kp_stair = 0.015f;
	FuzzyAidPidPitchPos.Ki_stair = 0.0005f;
	FuzzyAidPidPitchPos.Kd_stair = 0.001f;
	
  PidPitchAidSpeed.P=10000.0f;	  //速度环PID（辅瞄）
	PidPitchAidSpeed.I=15.0f; 
	PidPitchAidSpeed.D=0.0f;
	PidPitchAidSpeed.IMax=250.0f;
	PidPitchAidSpeed.SetPoint=0.0f;
	PidPitchAidSpeed.OutMax=30000.0f;
	PidPitchAidSpeed.I_L = 0.3f;
	PidPitchAidSpeed.I_U = 0.7f;
	PidPitchAidSpeed.RC_DF = 0.5f;

#elif Robot_ID == 5
/********************************************* 5号车 ********************************************************/	
  //手动pitch双环
  PidPitchPos.P=0.20f;       //手动pitch角度环
	PidPitchPos.I=0.0f;
	PidPitchPos.D=0.05f;       
	PidPitchPos.IMax=10.0f;
	PidPitchPos.SetPoint=0.0f;
	PidPitchPos.OutMax=5.5f;	
	PidPitchPos.I_L = 0.2f;
	PidPitchPos.I_U = 0.4f;
	PidPitchPos.RC_DF = 0.8f;
	
  PidPitchSpeed.P=14000.0f;  //手动pitch速度环
	PidPitchSpeed.I=15.0f;  
	PidPitchSpeed.D=0.0f;
	PidPitchSpeed.IMax=550.0f;
	PidPitchSpeed.SetPoint=0.0f;
	PidPitchSpeed.OutMax=30000.0f;
	PidPitchSpeed.I_L = 0.3f;
	PidPitchSpeed.I_U = 0.7f;
	PidPitchSpeed.RC_DF = 0.5f;
  
	
	//手动yaw双环                                 // 5号车
  PidYawPos.P=0.25f;        //手动yaw角度环
	PidYawPos.I=0.00f;
	PidYawPos.D=0.0f;       
	PidYawPos.IMax=10.0f;
	PidYawPos.SetPoint=0.0f;
	PidYawPos.OutMax=5.5f;
	PidYawPos.I_L = 0.2f;
	PidYawPos.I_U = 0.4f;
	PidYawPos.RC_DF = 0.5f;
	
	PidYawSpeed.P=15000.0f;   //手动yaw速度环
	PidYawSpeed.I=1.0f;
	PidYawSpeed.D=0.0f;
	PidYawSpeed.IMax=2000.0f;
	PidYawSpeed.SetPoint=0.0f;
	PidYawSpeed.OutMax=30000.0f;
	PidYawSpeed.I_L = 0.3f;
	PidYawSpeed.I_U = 0.7f;
	PidYawSpeed.RC_DF = 0.5f;
	

//辅瞄yaw
	PidYawAidPos.P =0.3f;	  //yaw PID位置环（辅瞄）
	PidYawAidPos.I=0.000f; 
	PidYawAidPos.D=0.20f;
	PidYawAidPos.IMax=40.0f;
	PidYawAidPos.SetPoint=0.0f;
	PidYawAidPos.OutMax=5.5f;
	PidYawAidPos.I_L = 0.2f;
	PidYawAidPos.I_U = 0.4f;
	PidYawAidPos.RC_DF = 0.5f;
	                              
	FuzzyAidPidYawPos.Kp0=0.25f;   //模糊PID yaw角度环（辅瞄）
	FuzzyAidPidYawPos.Ki0=0.0001f;  
	FuzzyAidPidYawPos.Kd0=0.1f;
	FuzzyAidPidYawPos.IMax=40.0f;
	FuzzyAidPidYawPos.SetPoint=0.0f;	
	FuzzyAidPidYawPos.OutMax=8.5f;
  FuzzyAidPidYawPos.I_L = 0.2f;
	FuzzyAidPidYawPos.I_U = 0.4f;
	FuzzyAidPidYawPos.RC_DF = 0.5f;
	
	FuzzyAidPidYawPos.stair = 0.2f;
	FuzzyAidPidYawPos.Kp_stair = 0.015f;
	FuzzyAidPidYawPos.Ki_stair = 0.0005f;
	FuzzyAidPidYawPos.Kd_stair = 0.001f;
	
	
	PidYawAidSpeed.P=10000.0f;			//32000  yaw速度环PID(辅瞄)
	PidYawAidSpeed.I=3.0f; 
	PidYawAidSpeed.D=0.0f;
	PidYawAidSpeed.IMax=2000.0f;
	PidYawAidSpeed.SetPoint=0.0f;
	PidYawAidSpeed.OutMax=30000.0f;
	PidYawAidSpeed.I_L = 0.3f;
	PidYawAidSpeed.I_U = 0.7f;
	PidYawAidSpeed.RC_DF = 0.5f;		
		//辅瞄pitch
	PidPitchAidPos.P =0.25f;	  //普通PID位置环(辅瞄)
	PidPitchAidPos.I=0.00001f; 
	PidPitchAidPos.D=0.0f;
	PidPitchAidPos.IMax=40.0f;
	PidPitchAidPos.SetPoint=0.0f;
	PidPitchAidPos.OutMax=5.5f;
	PidPitchAidPos.I_L = 0.2f;
	PidPitchAidPos.I_U = 0.4f;
	PidPitchAidPos.RC_DF = 0.5f;
	
	FuzzyAidPidPitchPos.Kp0=0.28f;
	FuzzyAidPidPitchPos.Ki0=0.00f;  //模糊PID位置环（辅瞄）
	FuzzyAidPidPitchPos.Kd0=0.0f;
	FuzzyAidPidPitchPos.IMax=40.0f;
	FuzzyAidPidPitchPos.SetPoint=0.0f;
	FuzzyAidPidPitchPos.OutMax=8.5f;
	FuzzyAidPidPitchPos.I_L = 0.2f;
	FuzzyAidPidPitchPos.I_U = 0.4f;
	FuzzyAidPidPitchPos.RC_DF = 0.5f;
	
	FuzzyAidPidPitchPos.stair = 0.2f;
	FuzzyAidPidPitchPos.Kp_stair = 0.015f;
	FuzzyAidPidPitchPos.Ki_stair = 0.0005f;
	FuzzyAidPidPitchPos.Kd_stair = 0.001f;
	
  PidPitchAidSpeed.P=10000.0f;	  //速度环PID（辅瞄）
	PidPitchAidSpeed.I=18.0f; 
	PidPitchAidSpeed.D=0.0f;
	PidPitchAidSpeed.IMax=250.0f;
	PidPitchAidSpeed.SetPoint=0.0f;
	PidPitchAidSpeed.OutMax=30000.0f;
	PidPitchAidSpeed.I_L = 0.3f;
	PidPitchAidSpeed.I_U = 0.7f;
	PidPitchAidSpeed.RC_DF = 0.5f;

	
	//大符 Pitch
	FuzzyBuffPidPitchPos.Kp0=0.28f;
	FuzzyBuffPidPitchPos.Ki0=0.003f;  //模糊PID位置环（辅瞄）
	FuzzyBuffPidPitchPos.Kd0=0.0f;
	FuzzyBuffPidPitchPos.IMax=40.0f;
	FuzzyBuffPidPitchPos.SetPoint=0.0f;
	FuzzyBuffPidPitchPos.OutMax=8.5f;
	FuzzyBuffPidPitchPos.I_L = 0.2f;
	FuzzyBuffPidPitchPos.I_U = 0.4f;
	FuzzyBuffPidPitchPos.RC_DF = 0.5f;
	
	FuzzyBuffPidPitchPos.stair = 0.2f;
	FuzzyBuffPidPitchPos.Kp_stair = 0.015f;
	FuzzyBuffPidPitchPos.Ki_stair = 0.0005f;
	FuzzyBuffPidPitchPos.Kd_stair = 0.001f;
	
  PidPitchBuffSpeed.P=10000.0f;	  //速度环PID（辅瞄）
	PidPitchBuffSpeed.I=15.0f; 
	PidPitchBuffSpeed.D=0.0f;
	PidPitchBuffSpeed.IMax=250.0f;
	PidPitchBuffSpeed.SetPoint=0.0f;
	PidPitchBuffSpeed.OutMax=30000.0f;
	PidPitchBuffSpeed.I_L = 0.3f;
	PidPitchBuffSpeed.I_U = 0.7f;
	PidPitchBuffSpeed.RC_DF = 0.5f;
	
	//大符 Yaw
	FuzzyBuffPidYawPos.Kp0=0.25f;   //模糊PID yaw角度环（辅瞄）
	FuzzyBuffPidYawPos.Ki0=0.0001f;  
	FuzzyBuffPidYawPos.Kd0=0.0f;
	FuzzyBuffPidYawPos.IMax=40.0f;
	FuzzyBuffPidYawPos.SetPoint=0.0f;	
	FuzzyBuffPidYawPos.OutMax=8.5f;
	FuzzyBuffPidYawPos.I_L = 0.2f;
	FuzzyBuffPidYawPos.I_U = 0.4f;
	FuzzyBuffPidYawPos.RC_DF = 0.5f;
	
	FuzzyBuffPidYawPos.stair = 0.2f;
	FuzzyBuffPidYawPos.Kp_stair = 0.015f;
	FuzzyBuffPidYawPos.Ki_stair = 0.0005f;
	FuzzyBuffPidYawPos.Kd_stair = 0.001f;
	
	PidYawBuffSpeed.P=10000.0f;			//  yaw速度环PID(辅瞄)
	PidYawBuffSpeed.I=5.0f; 
	PidYawBuffSpeed.D=0.0f;
	PidYawBuffSpeed.IMax=2000.0f;
	PidYawBuffSpeed.SetPoint=0.0f;
	PidYawBuffSpeed.OutMax=30000.0f;
	PidYawBuffSpeed.I_L = 0.3f;
	PidYawBuffSpeed.I_U = 0.7f;
	PidYawBuffSpeed.RC_DF = 0.5f;


#endif

	
}


/**********************************************************************************************************
*函 数 名: Gimbal_task
*功能说明: 云台任务函数
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
uint32_t Gimbal_high_water;
UBaseType_t GGGG;
extern TaskHandle_t RCReceiveTask_Handler; //任务句柄
extern TaskHandle_t PCReceiveTask_Handler; //任务句柄
extern uint8_t Remote_Receive_Flag;
extern uint8_t PC_ReceiveFlag;

void Gimbal_task(void *pvParameters)
{
	portTickType xLastWakeTime;
	const portTickType xFrequency = 1;
	vTaskDelay(1000);
   while (1) {
		xLastWakeTime = xTaskGetTickCount();
  
		if(Remote_Receive_Flag)  //数据解码
		{	
		xTaskNotifyGive(RCReceiveTask_Handler);
		}
		
		if(PC_ReceiveFlag)       //数据解码
		{
		xTaskNotifyGive(PCReceiveTask_Handler);		
		}
		 
		 if(PitchMotor_ReceiveFlag)  //确保获得电机数据后再进行控制
	{
		ZeroCheck_cal(); 
		Gimbal_CurrentPid_Cal();
	}	 
	
		
		IWDG_Feed();//喂狗
		
    vTaskDelayUntil(&xLastWakeTime,xFrequency); 

#if INCLUDE_uxTaskGetStackHighWaterMark
        Gimbal_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}

