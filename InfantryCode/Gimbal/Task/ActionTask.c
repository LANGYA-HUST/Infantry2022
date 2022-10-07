/**********************************************************************************************************
 * @文件     ActionTask.c
 * @说明     动作任务
 * @版本  	 V1.0
 * @作者     黄志雄
 * @日期     2020.1
**********************************************************************************************************/
/**********************************************************************************************************
 * @文件     ActionTask.c
 * @说明     模式切换任务
 * @版本  	 V2.0
 * @作者     戴军
 * @日期     2022.4
**********************************************************************************************************/
#include "main.h"
/*--------------内部变量-------------------*/
short Mouse_Key_Flag;
short waitFlag[10]={10,0,0,0,0,0,0,0,0,0};
short SpeedMode = 1;//摩擦轮射速挡位
short ctrl_rising_flag,pre_key_ctrl,v_rising_flag,pre_key_v,c_rising_flag,pre_key_c,pre_key_e,e_rising_flag,x_rising_flag,pre_key_x,Press_Key_x_Flag,v_rising_flag,pre_key_b;
short q_rising_flag,b_rising_flag,f_rising_flag,g_rising_flag,pre_key_f,mouse_Press_l_rising_flag,pre_mouse_l,r_rising_flag,pre_key_r,z_rising_flag,pre_key_z,Press_Key_z_Flag;
short pre_key_q,pre_key_g,pre_v_rising_flag;	//上一v按键按下状态
short v_high_flag;
char q_flag,f_flag,HighFreq_flag;
char k_slow;
char smallBuff_flag;
short Turning_flag;	//小陀螺标志
char Graphic_Init_flag;	//图形初始化标志
char Budan,Buff_flag;
short PC_Sendflag;
float Buff_Yaw_Motor;
char Buff_Init;
//弹仓盖舵机 持续时间
short SteeringEngineDelay = 0;
/*---------------结构体--------------------*/
Status_t Status;
/*--------------外部变量-------------------*/
extern RC_Ctl_t RC_Ctl; 
extern unsigned char magazineState;
extern Pid_Typedef PidBodanMotorPos,PidBodanMotorSpeed; 
extern RobotInit_Struct Infantry;
extern F105_Typedef F105;
extern F405_typedef F405;
extern PC_Receive_t PC_Receive;
extern float Bodan_Pos;
extern Gimbal_Typedef Gimbal;
extern float MirocPosition;//控制拨盘旋转
extern short FrictionWheel_speed;
extern RobotInit_Struct Infantry;
extern char ShootContinue;
extern float Onegrid;
/**********************************************************************************************************
*函 数 名: Status_Act
*功能说明: 步兵车动作函数，分底盘，云台，射击三部分
					 底盘： 1)  Chassis_Act_Cal(Remote rc,Key key)                                 底盘正常模式
									2)  Chassis_SelfProtect_Cal(Remote rc,Key key)                         自我保护模式（小陀螺）
									3)	Chassis_Solo_Cal(Remote rc,Key key)																 单挑模式
									4)  Chassis_Powerdown_Cal()                                            底盘锁死模式   
									
					 云台： 1)  Gimbal_Act_Cal(Remote rc,Mouse mouse,PC_Recv_t *Pc_RecvData)       云台正常模式
									2)  Gimbal_Armor_Cal(PC_Recv_t *Pc_Recv, RC_Ctl_t *RC_clt)             辅助射击模式
									3)  Gimbal_ShenFu_Cal(PC_Recv_t *Pc_Recv, RC_Ctl_t *RC_clt)            击打神符模式
									4)  Gimbal_Powerdown_Cal()                                             云台掉电模式 
									
					 射击   1)  Shoot_Check_Cal();                                                 检录模式使用
					        2)  Shoot_Fire_Cal();                                                  正常射击模式
									3)  Shoot_Armor_Cal();                                                 辅助射击模式
									4)  Shoot_ShenFu_Cal();                                                能量机关模式
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void Status_Act(void)
{
	SetInputMode(RC_Ctl.rc);
	switch(Status.ControlMode)
	{
		case Control_RC_Mode:
			   Remote_Process(RC_Ctl.rc);
			break;
		case Control_MouseKey_Mode:
		     Mouse_Key_Process(RC_Ctl);
			break;
		case Control_Powerdown_Mode:
			   Powerdown_Process();
			break;
		default:
			break;
	}
	
	if(Mouse_Key_Flag == 3)
	{
    MouseKey_Act_Cal(RC_Ctl);
	}
	F405Can1Send(&F405);//发送405信息
}

/**********************************************************************************************************
*函 数 名: SetInputMode
*功能说明: 控制模式选择
*形    参: rc
*返 回 值: 无
**********************************************************************************************************/
void SetInputMode(Remote rc)
{
	switch(rc.s1)
	{
		case 1:
			Status.ControlMode=Control_RC_Mode;
		break;
		case 3:
			Status.ControlMode=Control_MouseKey_Mode;
		break;
		case 2:
			Status.ControlMode=Control_Powerdown_Mode;
		  SteeringEngine_Set(Infantry.MagOpen);
		  
		break;
	}
//	Tx2_Off_Test(rc);	//遥控器控制tx2关机判断
}

/**********************************************************************************************************
*函 数 名: Powerdown_Process
*功能说明: 断电模式
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void Powerdown_Process()
{
	if(Mouse_Key_Flag!=1)
	{
		Mouse_Key_Flag=1;
		Budan = 1;
		q_flag = 0;
		f_flag = 0;
	 	F405.SuperPowerLimit = 0;			
		HighFreq_flag = 0;
		Buff_Init =0;
	}
	Status.ChassisMode = Chassis_Powerdown_Mode;
	Status.GimbalMode  = Gimbal_Powerdown_Mode;
	Status.ShootMode = Shoot_Powerdown_Mode;
	
}

///**********************************************************************************************************
//*函 数 名: Tx2_Off_Test
//*功能说明: Tx2关机指令判断，将右拨杆拨至最下，即掉电模式后，两摇杆向内靠，发送关机指令
//*形    参: 无
//*返 回 值: 无
//**********************************************************************************************************/
//void Tx2_Off_Test(Remote rc)
//{
//	if(rc.s1 == 2 && rc.ch0 < 500 && rc.ch2 > 1500)
//		Tx2_Off_times++;		//计数器
//	else 
//		{
//			Tx2_Off_times = 0;
//			PC_Sendflag = 0;
//		}
//	if(Tx2_Off_times > 500)
//		PC_Sendflag = Tx2_Off;
//}

/**********************************************************************************************************
*函 数 名: Remote_Process
*功能说明: 遥控器模式选择
*形    参: rc
*返 回 值: 无
**********************************************************************************************************/

void Remote_Process(Remote rc)
{
	if(Mouse_Key_Flag!=2)
	{
	  Mouse_Key_Flag=2;
		Budan = 0;
	}
	
	if(rc.s2==3) //正常模式
	{
		Buff_Init=0;
		Status.GimbalMode=Gimbal_Act_Mode;
		Status.ChassisMode=Chassis_Act_Mode;
		Status.ShootMode=Shoot_Powerdown_Mode;
		SteeringEngine_Set(Infantry.MagClose);
	}
//		if(rc.s2==2) //飞坡模式
//	{
//		Status.GimbalMode=Gimbal_Jump_Mode; 
//		Status.ChassisMode=Chassis_SelfProtect_Mode;
//		Status.ShootMode=Shoot_Powerdown_Mode;
//		SteeringEngine_Set(Infantry.MagClose);
//	}
	
//	
//		if(rc.s2==1) //弹道测试模式
//	{
//		Status.GimbalMode=Gimbal_Act_Mode;
//		Status.ChassisMode=Chassis_Powerdown_Mode;
//		Status.ShootMode=Shoot_Check_Mode;
//    SteeringEngine_Set(Infantry.MagOpen);
//	}

//			if(rc.s2==2) //底盘测试模式
//	{
//		Status.GimbalMode=Gimbal_Act_Mode;
//		Status.ChassisMode=Chassis_Test_Mode;
//		Status.ShootMode=Shoot_Powerdown_Mode;
//    SteeringEngine_Set(Infantry.MagOpen);
//	}
//	
	if(rc.s2==2) //检录模式
	{
		Status.GimbalMode=Gimbal_Test_Mode; 
		Status.ChassisMode=Chassis_Powerdown_Mode;
		Status.ShootMode=Shoot_Check_Mode;
		SteeringEngine_Set(Infantry.MagClose);
	}	
	if(rc.s2==1) //辅瞄模式
	{
		Status.GimbalMode=Gimbal_Armor_Mode; 
		Status.ChassisMode=Chassis_Act_Mode;
		Status.ShootMode=Shoot_Tx2_Mode;
		SteeringEngine_Set(Infantry.MagClose);
	}
//		if(rc.s2==1) //小陀螺模式
//	{
//		Status.GimbalMode=Gimbal_Act_Mode;
//		Status.ChassisMode=Chassis_SelfProtect_Mode;
//		Status.ShootMode=Shoot_Powerdown_Mode;
//    SteeringEngine_Set(Infantry.MagClose);
//	}
////	
//	if(rc.s2==2) //小陀螺辅瞄模式
//	{
//		Status.GimbalMode=Gimbal_Armor_Mode;
//		Status.ChassisMode=Chassis_SelfProtect_Mode;
//		Status.ShootMode=Shoot_Tx2_Mode;
//    SteeringEngine_Set(Infantry.MagOpen);
////	}

//	if(rc.s2 == 2)//大符模式
//	{
//		if(Buff_Init==0)
//		{
//		Buff_Yaw_Motor =Gimbal.Yaw.MotorTransAngle;
//		Buff_Init=1;
//		}
//		Status.GimbalMode = Gimbal_BigBuf_Mode;
//    SteeringEngine_Set(Infantry.MagOpen);	
//		Status.ChassisMode = Chassis_Act_Mode;
//		Status.ShootMode = Shoot_Tx2_Mode;
//	}
// 
	
//		if(rc.s2 == 1)//小符模式
//	{
//		if(Buff_Init==0)
//		{
//		Buff_Yaw_Motor =Gimbal.Yaw.MotorTransAngle;
//		Buff_Init=1;
//		}
//		Status.GimbalMode = Gimbal_SmlBuf_Mode;
//    SteeringEngine_Set(Infantry.MagClose);	
//		Status.ChassisMode = Chassis_Act_Mode;
//		Status.ShootMode = Shoot_Tx2_Mode;
//	}
//	
	
//	
//		if(rc.s2==2)  //系统辨识模式
//	{
//		Status.GimbalMode=Gimbal_SI_Mode;
//		Status.ChassisMode=Chassis_Act_Mode;
//		Status.ShootMode=Shoot_Powerdown_Mode;
//		SteeringEngine_Set(Infantry.MagClose);
//  }

//	if(rc.s2==1)  //系统辨识模式
//	{
//		Status.GimbalMode=Gimbal_SI_Mode;
//		Status.ChassisMode=Chassis_Act_Mode;
//		Status.ShootMode=Shoot_Powerdown_Mode;
//		SteeringEngine_Set(Infantry.MagClose);		
//	}
}

/**********************************************************************************************************
*函 数 名: MouseKey_Act_Cal
*功能说明: 键鼠模式
           w,s,a,d          前进左右后退
           q								辅瞄，自己打弹（按住）
					 e             		切换优先级
           r                开关弹舱   Pitch压低
           f                飞坡模式
					 g								拨盘反转一格（按一下）
					 shift+z					大符(先按紧shift,再按一下z进入/退出,然后就可松开shift，避免误触)
           shift+x          小符(先按紧shift,再按一下x进入/退出,然后就可松开shift，避免误触)          
           c                高低射频切换
           v                缓慢移动（按住）
					 b								图形初始化
           shift            使用超级电容（按住）
           ctrl             小陀螺(按住)
           mouse.press_r    （按住）辅瞄，操作手自己打弹
           mouse.press_l    单击点射(单发)/长按连发
					 鼠标滚轮         辅瞄微调

*形    参: RC_Ctl
*返 回 值: 无
**********************************************************************************************************/
extern float GimbalPitchPos;
extern int   Shoot_Init_flag;
float k_onegrid = 0.1f;
float k_single = 0.9f;
short aaaa = 500;
char 	ARMOR_FLAG=0;
char Last_shift;
char shift_press_flag,shift_flag;
char ReverseRotation=0;
extern char switchPriority;
void MouseKey_Act_Cal(RC_Ctl_t RC_Ctl)
{		
		static int waitb=0;

/******************************拨盘反转 g键*****************************************/
		g_rising_flag=RC_Ctl.key.g-pre_key_g;
    pre_key_g=RC_Ctl.key.g;
			if(g_rising_flag==1)
			{
				 ReverseRotation=1;	
				 ShootContinue=0;
         g_rising_flag=0;						
			}
			

/******************************图形界面初始化 b键*****************************************/
		b_rising_flag=RC_Ctl.key.b-pre_key_b;
    pre_key_b=RC_Ctl.key.b;
		if(b_rising_flag==1)
		{
			
			if(Graphic_Init_flag==0)
			{	
				F405.Graphic_Init_Flag = 1;
				Graphic_Init_flag=1;
			}
			else 
			{
				waitb++;
				if(waitb>5)
				{
				F405.Graphic_Init_Flag = 0;
				Graphic_Init_flag=0;
				waitb=0;
				}
			}			
		}
/******************************自我保护模式(小陀螺) ctrl键*****************************************/
		if(Status.GimbalMode != Gimbal_BigBuf_Mode && Status.GimbalMode != Gimbal_SmlBuf_Mode)
	{	
		ctrl_rising_flag = RC_Ctl.key.ctrl - pre_key_ctrl;
		pre_key_ctrl=RC_Ctl.key.ctrl;
		if(ctrl_rising_flag==1)
			Turning_flag++;
		if(RC_Ctl.key.ctrl==1)//还没有被设置模式或者一直按着的情况
		{
			if(Status.ChassisMode == Chassis_Act_Mode || Status.ChassisMode == Chassis_SelfProtect_Mode)
			Status.ChassisMode=Chassis_SelfProtect_Mode;
		}
		else if(Status.ChassisMode == Chassis_SelfProtect_Mode)
		{
			Status.ChassisMode = Chassis_Act_Mode;
		}
	}	
/******************************切换优先级 e键*****************************************/	
			e_rising_flag=RC_Ctl.key.e-pre_key_e;
			pre_key_e = RC_Ctl.key.e;
		
			
/*按下一次切换优先级*/
			if(e_rising_flag == 1)
			{
        switchPriority = !switchPriority;
			}		

			
	
	
//			e_rising_flag=RC_Ctl.key.e-pre_key_e;
//			pre_key_e = RC_Ctl.key.e;
//		
//		  /*按住才单挑*/
//			if(RC_Ctl.key.e==1)//还没有被设置模式或者一直按着的情况
//			{
//				if(Status.ChassisMode == Chassis_Act_Mode || Status.ChassisMode == Chassis_Solo_Mode)
//				Status.ChassisMode=Chassis_Solo_Mode;
//			}
//			else if(Status.ChassisMode == Chassis_Solo_Mode)
//			{
//				Status.ChassisMode = Chassis_Act_Mode;
//			}
//			
///*按下一次切换模式*/
//			if(e_rising_flag == 1)
//			{
//				if(Status.ChassisMode == Chassis_Act_Mode || Status.ChassisMode == Chassis_Solo_Mode)
//					Solo_flag++;
//			}		
////Solo_flag为偶 进正常；为奇，进单挑；错位：+1恢复
//			if(Solo_flag % 2 == 0 && Status.ChassisMode == Chassis_Solo_Mode)				//还原
//			{
//				Status.ChassisMode = Chassis_Act_Mode;
//			}
//			else if(Solo_flag % 2 == 1 && Status.ChassisMode == Chassis_Act_Mode)		//设置为单挑模式
//			{
//				Status.ChassisMode = Chassis_Solo_Mode;
//			}
//			

/*******************************回城补弹控制 r键**********************************************/			
	 r_rising_flag=RC_Ctl.key.r-pre_key_r;
   pre_key_r=RC_Ctl.key.r;
	 if(r_rising_flag==1)			//按键按下时
	 {
		 if(magazineState == 0x00) 
		 {
			 Budan = 1;
     }
			 else
			 {
			 Budan = 0;
       }
	 }

	 
/********************************超级电容控制**********************************************/	
  if(RC_Ctl.key.shift==1)
	{
	  F405.SuperPowerLimit = 2;		//使用超级电容
	}
  else
	{
	  F405.SuperPowerLimit = 0;				//不使用超级电容
	}

/****************************** 辅瞄自行打弹模式 q键*****************************************/
	 
	 if(Status.ShootMode != Shoot_Powerdown_Mode)
	 {
    	if(RC_Ctl.key.q == 1 && Status.GimbalMode == Gimbal_Act_Mode && Status.ShootMode == Shoot_Fire_Mode)
			{
				Status.GimbalMode = Gimbal_Armor_Mode;
				Status.ShootMode = Shoot_Tx2_Mode;	
	      q_flag = 1;				
			}
			else if(RC_Ctl.key.q == 0 && Status.GimbalMode == Gimbal_Armor_Mode && Status.ShootMode == Shoot_Tx2_Mode)
			{
				Status.GimbalMode = Gimbal_Act_Mode;
				Status.ShootMode = Shoot_Fire_Mode;	
        q_flag = 0;				
			}
	}
	
	
/******************************高低射频切换 c键*****************************************/
			c_rising_flag=RC_Ctl.key.c-pre_key_c;
			pre_key_c = RC_Ctl.key.c;
			if(c_rising_flag == 1)
			{	
					HighFreq_flag = !HighFreq_flag;
			}		

/******************************飞坡模式 f键*****************************************/
			f_rising_flag=RC_Ctl.key.f-pre_key_f;
			pre_key_f = RC_Ctl.key.f;
			if(f_rising_flag == 1)
			{				
				if(!f_flag)
				{
				Status.GimbalMode = Gimbal_Jump_Mode;
				Status.ChassisMode = Chassis_Jump_Mode;
				f_flag = 1;
				}
				else
				{
				Status.GimbalMode = Gimbal_Act_Mode;
				Status.ChassisMode = Chassis_Act_Mode;
				f_flag = 0;
				}
			}		

/******************************缓慢移动模式 v键*****************************************/

			if(RC_Ctl.key.v == 1)
			{				
        k_slow = 1;
			}		
			else 
			{
				k_slow = 0;
			}

///******************************大符模式 z键*****************************************/
			z_rising_flag=RC_Ctl.key.z-pre_key_z;
			pre_key_z = RC_Ctl.key.z;
			if(z_rising_flag ==1 && RC_Ctl.key.shift == 1)
		{
			if(Status.GimbalMode == Gimbal_BigBuf_Mode)			//
		  {
				Status.GimbalMode = Gimbal_Act_Mode;
			  Buff_Init=0;
			}
				else
			{
					if(Buff_Init==0)
				{
				Buff_Yaw_Motor =Gimbal.Yaw.MotorTransAngle;
				Buff_Init=1;
				}
				Status.GimbalMode = Gimbal_BigBuf_Mode;
			}
		}
	
//		/******************************Small  Buff 打小符 x键*****************************************/
		x_rising_flag=RC_Ctl.key.x-pre_key_x;
    pre_key_x=RC_Ctl.key.x;
		if(x_rising_flag ==1 && RC_Ctl.key.shift == 1)
		{
			if( Status.GimbalMode != Gimbal_SmlBuf_Mode )
			{
				if(Buff_Init==0)
				{
				Buff_Yaw_Motor =Gimbal.Yaw.MotorTransAngle;
				Buff_Init=1;
				}
			  Status.GimbalMode=Gimbal_SmlBuf_Mode;
			}
			else
			{
				Buff_Init=0;
				Status.GimbalMode=Gimbal_Act_Mode;
			}
		}

		/******************************鼠标按键射击*******************************************/
		mouse_Press_l_rising_flag=RC_Ctl.mouse.press_l-pre_mouse_l;
		pre_mouse_l=RC_Ctl.mouse.press_l;
		if(Status.ShootMode==Shoot_Fire_Mode)
	{
		if(RC_Ctl.mouse.press_l==1&&ReverseRotation==0)
		{
			 waitFlag[5]++;  
			 if((waitFlag[5]<50)&&(waitFlag[5]>0))//短按     //延时可以调一调
			 {	
				 if	(mouse_Press_l_rising_flag==1)	
				 {
						waitFlag[5]=0;
						if(F105.IsShootAble==1)//Heat_Limit
						{
							if(ABS(Bodan_Pos-PidBodanMotorPos.SetPoint)<PluckThreholdPos && Shoot_Init_flag == 1)
							{
								MirocPosition += Onegrid;		
								ShootContinue=0;
							}						
						}
				 }			    	 
			 }
			 if(waitFlag[5]>100)			//长按
			 {
 
						if(F105.IsShootAble==1)
						{
							if( ABS(Bodan_Pos-PidBodanMotorPos.SetPoint) < PluckThreholdPos  && Shoot_Init_flag == 1)
							{
                ShootContinue=1;
							}						
						}
			 }		 
		}
		else if(RC_Ctl.mouse.press_l==0&&ReverseRotation==0)			//不按
		{
			waitFlag[5]=0;
		   if( ShootContinue==1 || ABS(Bodan_Pos-PidBodanMotorPos.SetPoint) < 1000)
		  {
        PidBodanMotorPos.SetPoint=Bodan_Pos;
		  }	
			ShootContinue=0;
		}
	}	
		/******************************辅助射击控制（辅瞄） 右键*****************************************/
		if(RC_Ctl.mouse.press_r==1 && (Status.GimbalMode != Gimbal_BigBuf_Mode&&Status.GimbalMode != Gimbal_SmlBuf_Mode)) 
		{
		  Status.GimbalMode=Gimbal_Armor_Mode;
		}
		else if( q_flag==0 && Status.GimbalMode==Gimbal_Armor_Mode) 
		{
			Status.GimbalMode=Gimbal_Act_Mode;
		}
}


/**********************************************************************************************************
*函 数 名: Mouse_Key_Process
*功能说明: 键鼠模式初始化
*形    参: RC_Ctl
*返 回 值: 无
**********************************************************************************************************/
void Mouse_Key_Process(RC_Ctl_t RC_Ctl)
{
	/******************************初次参数初始*****************************************/
	if(Mouse_Key_Flag!=3)
	{
		SpeedMode=1;
		Mouse_Key_Flag=3;
		magazineState=0x00; //Close
		SteeringEngine_Set(Infantry.MagClose);
		Status.GimbalMode=Gimbal_Act_Mode;
    Status.ChassisMode=Chassis_Act_Mode;
	  Status.ShootMode=Shoot_Fire_Mode;
		Budan = 0;
	}
	
	//控制补弹开关弹舱盖
	if(Budan)
	{
	 SteeringEngine_Set(Infantry.MagOpen);
	}else
	{
	 SteeringEngine_Set(Infantry.MagClose);
	}
	
	//  控制摩擦轮开关
	if(RC_Ctl.rc.s2==2)
	{
	Status.ShootMode=Shoot_Powerdown_Mode;
	}
	else
	{
	if(Status.ShootMode!=Shoot_Tx2_Mode)
	Status.ShootMode=Shoot_Fire_Mode;
	}
}

/**********************************************************************************************************
*函 数 名: ModeChoose_task
*功能说明: 模式选择任务
*形    参: pvParameters
*返 回 值: 无
**********************************************************************************************************/
uint32_t ModeChoose_high_water;
extern char Chassis_ID;
void ModeChoose_task(void *pvParameters)
{
	
   while (1) {
		 
		Status_Act(); 
		IWDG_Feed();
    vTaskDelay(3); 
		 
#if INCLUDE_uxTaskGetStackHighWaterMark
        ModeChoose_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}
