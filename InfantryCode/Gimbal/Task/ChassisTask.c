/**********************************************************************************************************
 * @文件     ChassisTask.c
 * @说明     底盘控制
 * @版本  	 V1.0
 * @作者     黄志雄
 * @日期     2019.10
**********************************************************************************************************/
/**********************************************************************************************************
 * @文件     ChassisTask.c
 * @说明     底盘控制
 * @版本  	 V2.0
 * @作者     戴军
 * @日期     2022.4
     前x右y
**********************************************************************************************************/
#include "main.h"
/*----------------------------------内部变量---------------------------*/
short ChassisAct_Init_Flag=0;
float Theta,SinTheTa,CosTheTa,TanTheTa,Theta0,Speed_Theta;
char  SelfProtect_Cross_Flag;
float ResetPos;
short Be_shooted_flag;

const short FollowMaxSpeedw = 2000;			//跟随最高转速
const short RotateMaxSpeedw = 6000;			//小陀螺最高转速
/*----------------------------------结构体-----------------------------*/
ChassisSpeed_t chassis;
Pid_Typedef pidChassisPosition,pidChassisPosition_Speed;
Pid_Typedef SOLO_pidChassisPosition;
FeedForward_Typedef FF_w;
/*----------------------------------外部变量---------------------------*/
extern RC_Ctl_t RC_Ctl;
extern Status_t Status;
extern Gimbal_Typedef Gimbal;
extern RobotInit_Struct Infantry;
extern F105_Typedef F105;
extern F405_typedef F405;
extern short Turning_flag;
extern float YawMotorReceive;

/**********************************************************************************************************
*函 数 名: Chassis_Powerdown_Cal
*功能说明: 锁车模式
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void Chassis_Powerdown_Cal()
{
	if(ChassisAct_Init_Flag!=Chassis_Powerdown_Mode) 
		ChassisAct_Init_Flag=Chassis_Powerdown_Mode;
	
  chassis.carSpeedx=0;chassis.carSpeedy=0;chassis.carSpeedw=0;
}

/**********************************************************************************************************
*函 数 名: Chassis_Act_Cal
*功能说明: 正常模式
*形    参: rc  key
*返 回 值: 无
**********************************************************************************************************/
short test_w;
void Chassis_Act_Cal(Remote rc,Key key) 
{
	if(ChassisAct_Init_Flag!=Chassis_Act_Mode)
	{
		chassis.carSpeedw = 0;
    ChassisAct_Init_Flag=Chassis_Act_Mode;
	}
	Theta=ChassisPostionAngle_TranSform(Infantry.Yaw_init)/360.0f*6.28318f;  //弧度
	
	CosTheTa=cos(Theta);
	SinTheTa=sin(Theta);
	TanTheTa=tan(Theta);
	
	if(Status.ControlMode==Control_RC_Mode)
  {
//   	chassis.carSpeedx = (-2)*(-1024+rc.ch1); 
//		chassis.carSpeedy = 2*(1024-rc.ch0);
  
#if Robot_ID == 5
		if((-1024+RC_Ctl.rc.ch1)>300)  //向前
		{		
			if((-3.1416f/4)>Theta && (-3.1416f*3/4)<Theta)  //左为头
			{
				chassis.carSpeedy = -2000;
			}
			else if((3.1416f/4)<Theta && (3.1416f*3/4)>Theta)  //右为头
			{
				chassis.carSpeedy = 2000;
			}
			else if((-3.1416f*3/4)>Theta || (3.1416f*3/4)<Theta)  //后为头
			{
				chassis.carSpeedx = 2000;
			}
			else  //前为头
			{
				chassis.carSpeedx = -2000;
			}
		}
		else if((-1024+RC_Ctl.rc.ch1)<-300)  //向后
		{
			if((-3.1416f/4)>Theta && (-3.1416f*3/4)<Theta)  //左为头
			{
				chassis.carSpeedy = 2000;
			}
			else if((3.1416f/4)<Theta && (3.1416f*3/4)>Theta)  //右为头
			{
				chassis.carSpeedy = -2000;
			}
			else if((-3.1416f*3/4)>Theta || (3.1416f*3/4)<Theta)  //后为头
			{
				chassis.carSpeedx = -2000;
			}
			else  //前为头
			{
				chassis.carSpeedx = 2000;
			}
		}
		else
		{
			if(((-3.1416f/4)>Theta && (-3.1416f*3/4)<Theta) || ((3.1416f/4)<Theta && (3.1416f*3/4)>Theta)) //左或右为头
				chassis.carSpeedy = 0;
			else  //前或后为头
			  chassis.carSpeedx = 0;
		}
		
		if((-1024+RC_Ctl.rc.ch0)>300)  //向右
		{
			if((-3.1416f/4)>Theta && (-3.1416f*3/4)<Theta)  //左为头
			{
				chassis.carSpeedx = 2000;
			}
			else if((3.1416f/4)<Theta && (3.1416f*3/4)>Theta)  //右为头
			{
				chassis.carSpeedx = -2000;
			}
			else if((-3.1416f*3/4)>Theta || (3.1416f*3/4)<Theta)  //后为头
			{
				chassis.carSpeedy = 2000;
			}
			else  //前为头
			{
				chassis.carSpeedy = -2000;
			}
		}
		else if((-1024+RC_Ctl.rc.ch0)<-300)  //向左
		{
			if((-3.1416f/4)>Theta && (-3.1416f*3/4)<Theta)  //左为头
			{
				chassis.carSpeedx = -2000;
			}
			else if((3.1416f/4)<Theta && (3.1416f*3/4)>Theta)  //右为头
			{
				chassis.carSpeedx = 2000;
			}
			else if((-3.1416f*3/4)>Theta || (3.1416f*3/4)<Theta)  //后为头
			{
				chassis.carSpeedy = -2000;
			}
			else  //前为头
			{
				chassis.carSpeedy = 2000;
			}
		}
		else
		{
			if(((-3.1416f/4)>Theta && (-3.1416f*3/4)<Theta) || ((3.1416f/4)<Theta && (3.1416f*3/4)>Theta)) //左或右为头
				chassis.carSpeedx = 0;
			else  //前或后为头
			  chassis.carSpeedy = 0;
		}
		
#else
	    if((-1024+RC_Ctl.rc.ch1)>300)
			{
				chassis.carSpeedx= 2000; 
				if(Theta>3.1416f/2||Theta<-3.1416f/2)
				{
					chassis.carSpeedx= -2000; 
				}
			}
			else if((-1024+RC_Ctl.rc.ch1)<-300)
			{
				chassis.carSpeedx= -2000; 
				if(Theta>3.1416f/2||Theta<-3.1416f/2)
				{
				chassis.carSpeedx= 2000; 
				}
			}
			else
			chassis.carSpeedx= 0; 
			
			
		  if((-1024+RC_Ctl.rc.ch0)>300)
			{
				chassis.carSpeedy= 2000; 
				if(Theta>3.1416f/2||Theta<-3.1416f/2)
				{
				chassis.carSpeedy= -2000; 
				}
			}
			else if((-1024+RC_Ctl.rc.ch0)<-300)
			{
				chassis.carSpeedy= -2000; 
				if(Theta>3.1416f/2||Theta<-3.1416f/2)
				{
				chassis.carSpeedy= 2000; 
				}
			}
			else
			chassis.carSpeedy= 0; 
#endif
	}
	
	if(Status.ControlMode==Control_MouseKey_Mode)  
	{
#if Robot_ID == 5
	if(((-3.1416f/4)>Theta && (-3.1416f*3/4)<Theta) || ((3.1416f/4)<Theta && (3.1416f*3/4)>Theta)) //左或右为头
	{
		chassis.carSpeedx = + ( (key.a-key.d)*2000*SinTheTa - (key.s-key.w)*2000*CosTheTa );
		chassis.carSpeedy = - ( (key.s-key.w)*2000*SinTheTa + (key.a-key.d)*2000*CosTheTa );
	}
	else  //前或后为头
	{
		chassis.carSpeedx = - ( - ( (key.a-key.d)*2000*SinTheTa + (key.s-key.w)*2000*CosTheTa ));
  	chassis.carSpeedy = - ( + ( (key.s-key.w)*2000*SinTheTa - (key.a-key.d)*2000*CosTheTa ));
		//chassis.carSpeedx = + ( (key.a-key.d)*2000*SinTheTa - (key.s-key.w)*2000*CosTheTa );
		//chassis.carSpeedy = - ( (key.s-key.w)*2000*SinTheTa + (key.a-key.d)*2000*CosTheTa );
	}
//	  chassis.carSpeedx = -((key.a-key.d)*2000*SinTheTa+(key.s-key.w)*2000*CosTheTa); //后正
//		chassis.carSpeedy = ((key.s-key.w)*2000*SinTheTa-(key.a-key.d)*2000*CosTheTa); //左正		
#else
	  chassis.carSpeedx = -((key.a-key.d)*2000*SinTheTa+(key.s-key.w)*2000*CosTheTa); //后正
		chassis.carSpeedy = ((key.s-key.w)*2000*SinTheTa-(key.a-key.d)*2000*CosTheTa); //左正
#endif	
	}

	if(Status.GimbalMode == Gimbal_DropShot_Mode || Status.GimbalMode == Gimbal_BigBuf_Mode || Status.GimbalMode == Gimbal_SmlBuf_Mode)//吊射模式
	{	
	chassis.carSpeedw = 0;
  }
		else
	{	
	ResetPos = (Theta)/6.28318f*8192;
#if Robot_ID == 5
 				/*底盘跟随*/
	if((-3.1416f/4)>Theta && (-3.1416f*3/4)<Theta)  //右为头
	{
	pidChassisPosition.SetPoint = Gimbal.Yaw.Motor-ResetPos-2048;
	}
	else if((3.1416f/4)<Theta && (3.1416f*3/4)>Theta)  //左为头
	{
	pidChassisPosition.SetPoint = Gimbal.Yaw.Motor-ResetPos+2048;
	}
	else if((-3.1416f*3/4)>Theta)  //后
	{
	pidChassisPosition.SetPoint = Gimbal.Yaw.Motor-ResetPos-4096;
	}
	else if((3.1416f*3/4)<Theta)  //后
	{
	pidChassisPosition.SetPoint = Gimbal.Yaw.Motor-ResetPos+4096;
	}
	else 
	{
	pidChassisPosition.SetPoint = Gimbal.Yaw.Motor-ResetPos;
	}
#else
	if((-3.1416f/2)>Theta)
	{
	pidChassisPosition.SetPoint = Gimbal.Yaw.Motor-ResetPos-4096;
	}
	else if((3.1416f/2)<Theta)
	{
	pidChassisPosition.SetPoint = Gimbal.Yaw.Motor-ResetPos+4096;
	}
	else 
	{
	pidChassisPosition.SetPoint = Gimbal.Yaw.Motor-ResetPos;
	}
	
#endif	
//反馈PID控制+前馈
	pidChassisPosition.ActualValue = Gimbal.Yaw.Motor;
  chassis.carSpeedw = -PID_Calc(&pidChassisPosition) +  FeedForward_Calc(&FF_w);


	}
}



/**********************************************************************************************************
*函 数 名: Chassis_SelfProtect_Cal
*功能说明: 保护模式
*形    参: rc  key
*返 回 值: 无
**********************************************************************************************************/
float SP_Theta,CosSP_Theta,SinSP_Theta,TanSP_Theta;
void Chassis_SelfProtect_Cal(Remote rc,Key key)
{
	if(ChassisAct_Init_Flag!=Chassis_SelfProtect_Mode)
	  ChassisAct_Init_Flag=Chassis_SelfProtect_Mode;
	
	SP_Theta=ChassisPostionAngle_TranSform(Infantry.Yaw_init)/360.0f*6.28318f;
	CosSP_Theta=cos(SP_Theta);
	SinSP_Theta=sin(SP_Theta);
	TanSP_Theta=tan(SP_Theta);
	
if(Status.ControlMode==Control_RC_Mode)
  {
			
		   if((-1024+RC_Ctl.rc.ch1)>300)
			{
				chassis.carSpeedx= 1000; 
				if(SP_Theta>3.1416f/2||SP_Theta<-3.1416f/2)
				{
					chassis.carSpeedx=-1000; 
				}
			}
			else if((-1024+RC_Ctl.rc.ch1)<-300)
			{
				chassis.carSpeedx= -1000; 
				if(SP_Theta>3.1416f/2||SP_Theta<-3.1416f/2)
				{
				chassis.carSpeedx= 1000; 
				}
			}
			else
			chassis.carSpeedx= 0; 
			
			
		  if((-1024+RC_Ctl.rc.ch0)>300)
			{
				chassis.carSpeedy= 1000; 
				if(SP_Theta>3.1416f/2||SP_Theta<-3.1416f/2)
				{
				chassis.carSpeedy= -1000; 
				}
			}
			else if((-1024+RC_Ctl.rc.ch0)<-300)
			{
				chassis.carSpeedy= -1000; 
				if(SP_Theta>3.1416f/2||SP_Theta<-3.1416f/2)
				{
				chassis.carSpeedy= 1000; 
				}
			}
			else
			chassis.carSpeedy= 0; 
	}
	if(Status.ControlMode==Control_MouseKey_Mode)  
	{
	  chassis.carSpeedx = -((key.a-key.d)*1000*SinSP_Theta+(key.s-key.w)*1000*CosSP_Theta);
		chassis.carSpeedy = ((key.s-key.w)*1000*SinSP_Theta-(key.a-key.d)*1000*CosSP_Theta);
		if((key.a==1&&key.s==1)||(key.a==1&&key.w==1)||(key.d==1&&key.s==1)||(key.d==1&&key.w==1))
		{
		  SelfProtect_Cross_Flag=1;
		}
		else
		{
		 	SelfProtect_Cross_Flag=0;
		}
	}
	
//	if(F105.HP < F105.Last_HP)				//被射了就转快一点
//	{w
//		Be_shooted_flag = 200;www
//		F105.Last_HP = F105.HP;
//	}
//	if(Be_shooted_flag > 0)
//	{
//		chassis.carSpeedw = 600;
//		Be_shooted_flag--;
//	}
//	else
	
//	ResetPos = ABS((ChassisPostionAngle_TranSform(Infantry.Yaw_init) - 100)/360*8192);			//5°余量
	
//	if(ResetPos > 0 && ResetPos <=1024)
//		chassis.carSpeedw = RotateMaxSpeedw - 4 * ResetPos;
//	else if(ResetPos > 1024 && ResetPos <= 2048)
//		chassis.carSpeedw = RotateMaxSpeedw - 4 * (2048 - ResetPos);
//	else if(ResetPos > 2048 && ResetPos <= 3072)
//		chassis.carSpeedw = RotateMaxSpeedw - 4 * (ResetPos - 2048);
//	else if(ResetPos > 3072 && ResetPos <= 4096)
//		chassis.carSpeedw = RotateMaxSpeedw - 4 * (4096 - ResetPos);
//	else	
//		chassis.carSpeedw = RotateMaxSpeedw;			//发送100W时的旋转速度，在底盘再限速

	chassis.carSpeedw = -11000;				//底盘直接设置
	
	if(Turning_flag%2 == 1)
		chassis.carSpeedw *= -1;
}

/**********************************************************************************************************
*函 数 名: Chassis_Solo_Cal
*功能说明: 单挑模式，撞角对着别人
*形    参: rc  key
*返 回 值: 无
**********************************************************************************************************/
short SOLO_bias;
short SOLO_bias_max = 450;//550;
short H_pid = 100;
short L_pid = 20;
float H_P = 0.8f;

void Chassis_Solo_Cal(Remote rc,Key key)
{
	if(ChassisAct_Init_Flag!=Chassis_Solo_Mode)
	{
		SOLO_bias = SOLO_bias_max;
	  ChassisAct_Init_Flag=Chassis_Solo_Mode;
	}
	
	Theta=ChassisPostionAngle_TranSform(Infantry.Yaw_init)/360.0f*6.28318f;
	CosTheTa=cos(Theta);
	SinTheTa=sin(Theta);
	TanTheTa=tan(Theta);
	
if(Status.ControlMode==Control_RC_Mode)
  {//阶跃控制
   if((-1024+RC_Ctl.rc.ch1)>300)
			{
				chassis.carSpeedx= 2000; 
				if(Theta>3.1416f/2||Theta<-3.1416f/2)
				{
					chassis.carSpeedx= -2000; 
				}
			}
			else if((-1024+RC_Ctl.rc.ch1)<-300)
			{
				chassis.carSpeedx= -2000; 
				if(Theta>3.1416f/2||Theta<-3.1416f/2)
				{
				chassis.carSpeedx= 2000; 
				}
			}
			else
			chassis.carSpeedx= 0; 
			
			
		  if((-1024+RC_Ctl.rc.ch0)>300)
			{
				chassis.carSpeedy= 2000; 
				if(Theta>3.1416f/2||Theta<-3.1416f/2)
				{
				chassis.carSpeedy= -2000; 
				}
			}
			else if((-1024+RC_Ctl.rc.ch0)<-300)
			{
				chassis.carSpeedy= -2000; 
				if(Theta>3.1416f/2||Theta<-3.1416f/2)
				{
				chassis.carSpeedy= 2000; 
				}
			}
			else
			chassis.carSpeedy= 0; 
	}
	if(Status.ControlMode==Control_MouseKey_Mode)  
	{
	  chassis.carSpeedx = -((key.a-key.d)*2000*SinTheTa+(key.s-key.w)*2000*CosTheTa);
		chassis.carSpeedy = ((key.s-key.w)*2000*SinTheTa-(key.a-key.d)*2000*CosTheTa);
	}
	/*底盘跟随*/
		if(ABS(chassis.carSpeedx) <=300 && ABS(chassis.carSpeedy) < 300)		//静止时扭腰
		{
			ResetPos = (ChassisPostionAngle_TranSform(Infantry.Solo_Yaw_init))/360*8192;		//与正对角差值计算
	//		SOLO_pidChassisPosition.SetPoint = Gimbal.Yaw.Motor-ResetPos+SOLO_bias;					//单挑模式用单独的pid
		  if(ABS(ResetPos - SOLO_bias) < 50)
			{
				SOLO_bias = -SOLO_bias;
	  	}
			SOLO_pidChassisPosition.SetPoint = SOLO_bias;					//单挑模式用单独的pid
			SOLO_pidChassisPosition.ActualValue = ResetPos;
			pidChassisPosition_Speed.SetPoint = -PID_Calc(&SOLO_pidChassisPosition);
			pidChassisPosition_Speed.ActualValue = F105.ChassisSpeedw;
			chassis.carSpeedw = PID_Calc(&pidChassisPosition_Speed);

		}
		else		//开始运动，停止扭腰
		{
			ResetPos = (ChassisPostionAngle_TranSform(Infantry.Solo_Yaw_init))/360*8192;		//与正对角差值计算
			
			SOLO_pidChassisPosition.SetPoint = 0;
			SOLO_pidChassisPosition.ActualValue = ResetPos;
			pidChassisPosition_Speed.SetPoint = -PID_Calc(&SOLO_pidChassisPosition);
			
			pidChassisPosition_Speed.ActualValue = F105.ChassisSpeedw;
			chassis.carSpeedw = PID_Calc(&pidChassisPosition_Speed);

		}
	
	//速度环
//	pidChassisPosition_Speed.SetPoint = - chassis.carSpeedw;	
//	chassis.carSpeedw = PID_Calc(&pidChassisPosition_Speed, 0);
	
//	if(ABS(chassis.carSpeedw) < 200)		//抑制静态零漂
//		chassis.carSpeedw = 0;	
//	pidChassisPosition_Speed.SetPoint = -PID_Calc(&pidChassisPosition, Gimbal.Yaw.Motor);
//	chassis.carSpeedw = LIMIT_MAX_MIN(PID_Calc(&SOLO_pidChassisPosition, -0.002f*F105.ChassisSpeedw), 400, -400); 
}

/**********************************************************************************************************
*函 数 名: Chassis_Jump_Cal
*功能说明: 无跟随模式
*形    参: rc  key
*返 回 值: 无
**********************************************************************************************************/
void Chassis_Jump_Cal(Remote rc,Key key) 
{
	if(ChassisAct_Init_Flag!=Chassis_Jump_Mode)
	{
    ChassisAct_Init_Flag=Chassis_Jump_Mode;
	}
	Theta=ChassisPostionAngle_TranSform(Infantry.Yaw_init)/360.0f*6.28318f;
	
	CosTheTa=cos(Theta);
	SinTheTa=sin(Theta);
	TanTheTa=tan(Theta);
	
if(Status.ControlMode==Control_RC_Mode)
  {
#if Robot_ID == 5
		if((-1024+RC_Ctl.rc.ch1)>300)  //向前
		{		
			if((-3.1416f/4)>Theta && (-3.1416f*3/4)<Theta)  //左为头
			{
				chassis.carSpeedy = -2000;
			}
			else if((3.1416f/4)<Theta && (3.1416f*3/4)>Theta)  //右为头
			{
				chassis.carSpeedy = 2000;
			}
			else if((-3.1416f*3/4)>Theta || (3.1416f*3/4)<Theta)  //后为头
			{
				chassis.carSpeedx = 2000;
			}
			else  //前为头
			{
				chassis.carSpeedx = -2000;
			}
		}
		else if((-1024+RC_Ctl.rc.ch1)<-300)  //向后
		{
			if((-3.1416f/4)>Theta && (-3.1416f*3/4)<Theta)  //左为头
			{
				chassis.carSpeedy = 2000;
			}
			else if((3.1416f/4)<Theta && (3.1416f*3/4)>Theta)  //右为头
			{
				chassis.carSpeedy = -2000;
			}
			else if((-3.1416f*3/4)>Theta || (3.1416f*3/4)<Theta)  //后为头
			{
				chassis.carSpeedx = -2000;
			}
			else  //前为头
			{
				chassis.carSpeedx = 2000;
			}
		}
		else
		{
			if(((-3.1416f/4)>Theta && (-3.1416f*3/4)<Theta) || ((3.1416f/4)<Theta && (3.1416f*3/4)>Theta)) //左或右为头
				chassis.carSpeedy = 0;
			else  //前或后为头
			  chassis.carSpeedx = 0;
		}
		
		if((-1024+RC_Ctl.rc.ch0)>300)  //向右
		{
			if((-3.1416f/4)>Theta && (-3.1416f*3/4)<Theta)  //左为头
			{
				chassis.carSpeedx = 2000;
			}
			else if((3.1416f/4)<Theta && (3.1416f*3/4)>Theta)  //右为头
			{
				chassis.carSpeedx = -2000;
			}
			else if((-3.1416f*3/4)>Theta || (3.1416f*3/4)<Theta)  //后为头
			{
				chassis.carSpeedy = 2000;
			}
			else  //前为头
			{
				chassis.carSpeedy = -2000;
			}
		}
		else if((-1024+RC_Ctl.rc.ch0)<-300)  //向左
		{
			if((-3.1416f/4)>Theta && (-3.1416f*3/4)<Theta)  //左为头
			{
				chassis.carSpeedx = -2000;
			}
			else if((3.1416f/4)<Theta && (3.1416f*3/4)>Theta)  //右为头
			{
				chassis.carSpeedx = 2000;
			}
			else if((-3.1416f*3/4)>Theta || (3.1416f*3/4)<Theta)  //后为头
			{
				chassis.carSpeedy = -2000;
			}
			else  //前为头
			{
				chassis.carSpeedy = 2000;
			}
		}
		else
		{
			if(((-3.1416f/4)>Theta && (-3.1416f*3/4)<Theta) || ((3.1416f/4)<Theta && (3.1416f*3/4)>Theta)) //左或右为头
				chassis.carSpeedx = 0;
			else  //前或后为头
			  chassis.carSpeedy = 0;
		}
#else
     if((-1024+RC_Ctl.rc.ch1)>300)
			{
				chassis.carSpeedx= 2000; 
				if(Theta>3.1416f/2||Theta<-3.1416f/2)
				{
					chassis.carSpeedx= -2000; 
				}
			}
			else if((-1024+RC_Ctl.rc.ch1)<-300)
			{
				chassis.carSpeedx= -2000; 
				if(Theta>3.1416f/2||Theta<-3.1416f/2)
				{
				chassis.carSpeedx= 2000; 
				}
			}
			else
			chassis.carSpeedx= 0; 
			
			
		  if((-1024+RC_Ctl.rc.ch0)>300)
			{
				chassis.carSpeedy= 2000; 
				if(Theta>3.1416f/2||Theta<-3.1416f/2)
				{
				chassis.carSpeedy= -2000; 
				}
			}
			else if((-1024+RC_Ctl.rc.ch0)<-300)
			{
				chassis.carSpeedy= -2000; 
				if(Theta>3.1416f/2||Theta<-3.1416f/2)
				{
				chassis.carSpeedy= 2000; 
				}
			}
			else
			chassis.carSpeedy= 0; 
#endif
	}
	if(Status.ControlMode==Control_MouseKey_Mode)  
	{
#if Robot_ID == 5
	if(((-3.1416f/4)>Theta && (-3.1416f*3/4)<Theta) || ((3.1416f/4)<Theta && (3.1416f*3/4)>Theta)) //左或右为头
	{
		chassis.carSpeedx = + ( (key.a-key.d)*2000*SinTheTa - (key.s-key.w)*2000*CosTheTa );
		chassis.carSpeedy = - ( (key.s-key.w)*2000*SinTheTa + (key.a-key.d)*2000*CosTheTa );
	}
	else  //前或后为头
	{
		chassis.carSpeedx = - ( - ( (key.a-key.d)*2000*SinTheTa + (key.s-key.w)*2000*CosTheTa ));
  	chassis.carSpeedy = - ( + ( (key.s-key.w)*2000*SinTheTa - (key.a-key.d)*2000*CosTheTa ));
		//chassis.carSpeedx = + ( (key.a-key.d)*2000*SinTheTa - (key.s-key.w)*2000*CosTheTa );
		//chassis.carSpeedy = - ( (key.s-key.w)*2000*SinTheTa + (key.a-key.d)*2000*CosTheTa );
	}
#else
	  chassis.carSpeedx = -((key.a-key.d)*2000*SinTheTa+(key.s-key.w)*2000*CosTheTa);
		chassis.carSpeedy = ((key.s-key.w)*2000*SinTheTa-(key.a-key.d)*2000*CosTheTa);
#endif	
	}
	
	//头朝前
	ResetPos = (Theta)/6.28318f*8192;
 				/*底盘跟随*/
		if(0>Theta)
	{
	pidChassisPosition.SetPoint = Gimbal.Yaw.Motor-ResetPos-4096;
	}
	else 
	{
	pidChassisPosition.SetPoint = Gimbal.Yaw.Motor-ResetPos+4096;
	}
	
//反馈PID控制+前馈
	pidChassisPosition.ActualValue = Gimbal.Yaw.Motor;
  chassis.carSpeedw = -PID_Calc(&pidChassisPosition) +  FeedForward_Calc(&FF_w);


}

/**********************************************************************************************************
*函 数 名: ChassisPostionAngle_TranSform
*功能说明: 角度转换函数
           由初始角度和Zero_CheckYawPosition()来计算-180~180的角度
           用过零检测后的值和初始化的Yaw来做位置差获取角度差
           考虑过零检测的Yaw是否大于初始化的值，大于初始化的角度值时TheTa为正，否则为负；
           并且规定TheTa值的范围为与初始值的较小角度
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
float Bias_Angle;
float ChassisPostionAngle_TranSform(short InitPos)
{
  int32_t bias;
	bias=YawMotorReceive-InitPos;
	
	if(bias>=4096)
	bias-=8192;
	else if(bias<-4096)
	bias+=8192;
	
	Bias_Angle=bias/8192.0*360.0;
	return Bias_Angle;
}

/**********************************************************************************************************
*函 数 名: Chassis_CurrentPid_Cal
*功能说明: 向底盘发送xyw向速度
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void Chassis_CurrentPid_Cal(void)
{
	switch (Status.ChassisMode)
	{
		case Chassis_Act_Mode:
			Chassis_Act_Cal(RC_Ctl.rc,RC_Ctl.key);
			break;

		case Chassis_SelfProtect_Mode:
			 Chassis_SelfProtect_Cal(RC_Ctl.rc,RC_Ctl.key);
			break;

		case Chassis_Solo_Mode:
			Chassis_Solo_Cal(RC_Ctl.rc,RC_Ctl.key);
			break;

		case Chassis_Jump_Mode:
			Chassis_Jump_Cal(RC_Ctl.rc,RC_Ctl.key);
			break;

		case Chassis_Powerdown_Mode:
			Chassis_Powerdown_Cal();
			break;
	
		default:
			Chassis_Powerdown_Cal();
			break;
	}
	F405.Chassis_Flag = Status.ChassisMode;
//	if(ABS(ResetPos) > 500 && Status.ChassisMode == Chassis_Act_Mode)		//如果还没跟随到位
//	{
//		F405.Follow_state = 1;										//发标志位给底盘
//	}else
//	{
//		F405.Follow_state = 0;
//	}
	ChassisCan1Send(&chassis.carSpeedx,&chassis.carSpeedy,&chassis.carSpeedw);
}


/**********************************************************************************************************
*函 数 名: Pid_ChassisPosition
*功能说明: 底盘随云台旋转pid参数初始化
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void Pid_ChassisPosition_Init(void)                
{
#if Robot_ID == 3
/********************************************* 3号车 ***********************************************************/	
		pidChassisPosition.P = 5.0f;				//  位置环					3号车
	  pidChassisPosition.I = 0.00f;					
	  pidChassisPosition.D = 0.0f;				
	  pidChassisPosition.IMax = 300.0f;
	  pidChassisPosition.OutMax = 16000.0f;
	  pidChassisPosition.DeadZone=0.0f;
		pidChassisPosition.RC_DF = 0.5f;

	
		FF_w.K1 = 20000.0f;
		FF_w.K2 = 0.0f;
		FF_w.OutMax = 16000.0f;
		

#elif Robot_ID == 4	
	/********************************************* 4号车 ***********************************************************/	
		pidChassisPosition.P = 5.0f;				//2.0  位置环					3号车
	  pidChassisPosition.I = 0.00f;					
	  pidChassisPosition.D = 0.0f;				
	  pidChassisPosition.IMax = 300.0f;
	  pidChassisPosition.OutMax = 16000.0f;
	  pidChassisPosition.DeadZone=0.0f;
		pidChassisPosition.RC_DF = 0.5f;
	
		FF_w.K1 = 20000.0f;
		FF_w.K2 = 0.0f;
		FF_w.OutMax = 16000.0f;
		
#elif Robot_ID == 14
	/********************************************* 14 号车 ***********************************************************/	
		pidChassisPosition.P = 5.0f;				//  位置环					3号车
	  pidChassisPosition.I = 0.00f;					
	  pidChassisPosition.D = 1.0f;				
	  pidChassisPosition.IMax = 300.0f;
	  pidChassisPosition.OutMax = 16000.0f;
	  pidChassisPosition.DeadZone=0.0f;
		pidChassisPosition.RC_DF = 0.5f;
	
		FF_w.K1 = 20000.0f;
		FF_w.K2 = 0.0f;
		FF_w.OutMax = 16000.0f;
		
#elif Robot_ID == 5
	/********************************************* 5 号车 ***********************************************************/	
		pidChassisPosition.P = 4.5f;				//  位置环					3号车
	  pidChassisPosition.I = 0.00f;					
	  pidChassisPosition.D = 1.0f;				
	  pidChassisPosition.IMax = 300.0f;
	  pidChassisPosition.OutMax = 16000.0f;
	  pidChassisPosition.DeadZone=0.0f;
		pidChassisPosition.RC_DF = 0.5f;
	
		FF_w.K1 = 15000.0f;
		FF_w.K2 = 0.0f;
		FF_w.OutMax = 16000.0f;
 
#endif 
}

/**********************************************************************************************************
*函 数 名: Chassis_task
*功能说明: 底盘任务
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
uint32_t Chassis_high_water;
void Chassis_task(void *pvParameters)
{
   while (1) {
    Chassis_CurrentPid_Cal(); 
		IWDG_Feed(); 
    vTaskDelay(1); 
#if INCLUDE_uxTaskGetStackHighWaterMark
        Chassis_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}
