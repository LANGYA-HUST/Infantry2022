/**********************************************************************************************************
 * @文件     ChassisTask.c
 * @说明     底盘控制+功率限制
 * @版本  	 V3.0
 * @作者     赵业权
 * @日期     2021.4.26
**********************************************************************************************************/
/**********************************************************************************************************
 * @文件     ChassisTask.c
 * @说明     底盘控制+功率限制
 * @版本  	 V4.0
 * @作者     戴军
 * @日期     2022.6
**********************************************************************************************************/
#include "main.h"

#define CAP_MAX_W      7000
#define Rand_S         0.5f   //周期长短
#define RandThreshold  0.5f   //直流偏置
#define RANDA          1.3f   //正弦幅值

float k_CAP = 2.0f;
/*----------------------------------内部变量---------------------------*/
short WheelCurrentSend[4];
short Set_Jump[4] = {0};
float Current_Change[4] = {0};//电流环增量
float Current_f[4] = {0};//输出电流f
float Flow[4] = {0};//实际电流f
float speed[4] = {0};//实际速度f
char  WheelStopFlag[4] ; //标志轮子减速的标志
//功率限制系数 PowerLimit
short Actual_P_max;						//实际最大功率
short Self_Protect_Limit;			//小陀螺转速限制
float k_BAT;
char ChassisSetUp[4];

TickType_t Tickcnt=0;


/*----------------------------------结构体-----------------------------*/
Pid_Typedef Pid_Current[4];
Pid_Typedef pidChassisWheelSpeed[4];
Pid_Typedef pidChassisPosition_Speed;
F105_Typedef F105;
Power_Typedef Power_method[METHOD_NUM];

/*----------------------------------外部变量---------------------------*/
extern JudgeReceive_t JudgeReceive;
extern ChassisSpeed_t chassis;
extern RM820RReceive_Typedef ChassisMotorCanReceive[4];
extern F405_typedef F405;
extern enum POWERSTATE_Typedef PowerState;
extern char Robot_ID;
extern float output_fil;
extern float Input[4];
extern float Output[4];
extern char slow_flag;

float k_xy = 3.0f,y_lim=1.0f;
short carSpeedw = 0;
float ABSready_flag=0; 
float Goready_flag=0;
float T_ABS=1000.0f;//刹车时间
float T_SETUP=800.0f;//启动时间
extern char output_filter;
extern enum CHARGESTATE_Typedef ChargeState;
/**********************************************************************************************************
*函 数 名: ABS_Cal
*功能说明: 缓速启停
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
//void ABS_Cal(void)
//{
//static TickType_t StartTick=0,NowTick;
//static TickType_t StopTick=0;
//static char StartInitFlag=0,StopInitFlag=0;
//if(chassis.carSpeedx>50||chassis.carSpeedy>50) //正常行驶，开始进入加速
//{
//	
//	if(Goready_flag==1)//开始启动，进入起步态
//	{
//	if(!StartInitFlag)
//	{
//	StartTick=xTaskGetTickCount();
//	StartInitFlag=1;
//	}
//  NowTick=xTaskGetTickCount();

//	   if((NowTick-StartTick)>=T_SETUP)
//	     {
//         Goready_flag=0;  
//         StartInitFlag=0;	 
//	     }	
//		 else
//		 {
//		 output_filter = 0;
//		 }
//	}
//	
//	else if(Goready_flag==0)//进入平稳态
//	{

//	ABSready_flag=1; //准备好缓速刹车 注意：只有在满速后才缓速刹车
//  StopInitFlag=0;
//  output_filter = 1;
//	}

//}

//else if((ABS(chassis.carSpeedx) <50)&&(ABS(chassis.carSpeedy) <50))//开始刹车
//{	
//	if(ABSready_flag==1)  //只有在刹车状态下才开始计时
//	{
//		if(!StopInitFlag)
//		{
//		StopTick=xTaskGetTickCount();
//		StopInitFlag=1;
//		}
//		NowTick=xTaskGetTickCount();
//		output_filter = 0;
//	}
//		
// if((NowTick-StopTick)>=T_ABS ||ABSready_flag==0 || ABS(ChassisMotorCanReceive[3].RealSpeed)<=200)  //完全刹住车
// {
//  ABSready_flag=0;   //刹车完毕
//	StopInitFlag=0;
// }
//  Goready_flag=1;// 准备进入启动态
//	StartInitFlag=0;
//}

// }
/**********************************************************************************************************
*函 数 名: ABS_Cal
*功能说明: 缓速启停
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
float test_watch;
char filter_en=1;
char test_cnt[4];
#define  SetUP_T  0.99f
 void Filter_Cal(void)
{

  LowPass_SetChassis(&pidChassisWheelSpeed[0].SetPoint,k_xy*(+chassis.carSpeedy+(+chassis.carSpeedx))-carSpeedw);
	LowPass_SetChassis(&pidChassisWheelSpeed[1].SetPoint,k_xy*(+chassis.carSpeedy+(-chassis.carSpeedx))-carSpeedw);
	LowPass_SetChassis(&pidChassisWheelSpeed[2].SetPoint,k_xy*(-chassis.carSpeedy+(+chassis.carSpeedx))-carSpeedw);
	LowPass_SetChassis(&pidChassisWheelSpeed[3].SetPoint,k_xy*(-chassis.carSpeedy+(-chassis.carSpeedx))-carSpeedw);
	
//	for(int t=0;t<4;t++)  //用于标志轮子速度变化方向
//	{
//	  if(ABS(pidChassisWheelSpeed[t].SetPoint) +100 < ABS(pidChassisWheelSpeed[t].SetPointLast)) //避免静止误差
//		{
//		 WheelStopFlag[t] = 1;
//		 test_cnt[t]++;
//		}
//		else
//		{
//		 WheelStopFlag[t] = 0;
//		}
//	}
//	
	//  标志车子已经起步了
		if(ABS(k_xy*(+chassis.carSpeedy+(+chassis.carSpeedx))-carSpeedw)*SetUP_T<ABS(pidChassisWheelSpeed[0].SetPoint))
		ChassisSetUp[0]=1;
		else 	ChassisSetUp[0]=0;
		
		if(ABS(k_xy*(+chassis.carSpeedy+(-chassis.carSpeedx))-carSpeedw)*SetUP_T<ABS(pidChassisWheelSpeed[1].SetPoint))
		ChassisSetUp[1]=1;
		else 	ChassisSetUp[1]=0;
		
		if(ABS(k_xy*(-chassis.carSpeedy+(+chassis.carSpeedx))-carSpeedw)*SetUP_T<ABS(pidChassisWheelSpeed[2].SetPoint))
		ChassisSetUp[2]=1;
		else 	ChassisSetUp[2]=0;
		
		if(ABS(k_xy*(-chassis.carSpeedy+(-chassis.carSpeedx))-carSpeedw)*SetUP_T<ABS(pidChassisWheelSpeed[3].SetPoint))
		ChassisSetUp[3]=1;
		else 	ChassisSetUp[3]=0;

}

/**********************************************************************************************************
*函 数 名: Method_Check
*功能说明: 根据不同的功率选择不同的控制参数
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
extern volatile TickType_t SampleTick;
TickType_t MyNowTick;
void Method_Check(void)
{
	short i;
	short choose_PN = 0;
	
	static short PN;
	static short last_PN;	
	
	for(choose_PN = 1;choose_PN < METHOD_NUM; choose_PN++)		//0用于储存默认参数
	{
		if(Power_method[choose_PN].Actual_P_max == JudgeReceive.MaxPower)
		{
			PN = choose_PN;
			break;
		}	
	}
	if(choose_PN >= METHOD_NUM)	//没找到匹配的最大功率参数
		PN = PN_Default;	//默认参数

	if(last_PN != choose_PN)			//说明参数有变化
	{
		Actual_P_max = Power_method[PN].Actual_P_max;
		Self_Protect_Limit = Power_method[PN].Self_Protect_Limit;
		k_BAT = Power_method[PN].k_BAT;
//		for(int i=0;i<4;i++)
//		{
//		 pidChassisWheelSpeed[i].OutMax=Power_method[PN].CurrentMax;
//		}
	}
		
	last_PN = PN;

}

/**********************************************************************************************************
*函 数 名: Chassis_Speed_Cal
*功能说明: 根据xyw向速度计算目标速度值
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
short test_Self_Protect_Limit = 3600;
float test_k_BAT = 1.0f;
short pre_in[2];
char SelfProtect_Cross_Flag;
int Rand_T,rand_p,rand_cnt,rand_w;
float rand_A,test_rand;
void Chassis_Speed_Cal(void)
{
	static short Angular_Velocity;
	float rotation_lim=1.0f;	
	
//xy向速度与w向速度配比
	switch(F405.Chassis_Flag)
	{
		case Chassis_Powerdown_Mode:
			k_xy = 0;
			carSpeedw = 0;
		break;
		
		case Chassis_Act_Mode:
		case Chassis_Jump_Mode:
			
		if((ABS(chassis.carSpeedx)>500) && (ABS(chassis.carSpeedy) >500))
				k_xy = 2.0f;    //斜着走
			else
				k_xy = 3;       //直着走
			
		if((ABS(chassis.carSpeedx) <500) && (ABS(chassis.carSpeedy)>500))
				k_xy =2.5f;     //横着走
				
		
	   carSpeedw = chassis.carSpeedw;

	 
		break;
		
		case Chassis_SelfProtect_Mode:
		{
			
			//变速处理
			if( rand_p == rand_cnt)
			{
				Rand_T = (rand()%4000+4000)*Rand_S;
				rand_p = Rand_T*(0.3f+rand()%7/10.0f);
				rand_cnt = 0;
			}
			else
			{
			  rand_cnt ++;
			}
			test_rand = rand_cnt*3.14f/Rand_T*2;
			rand_A = RANDA*ABS(cos(rand_cnt*3.14f/Rand_T*2));
			
			
				if((ABS(chassis.carSpeedx) >100) || (ABS(chassis.carSpeedy) >100))
				{
	
							k_xy = 1.7f;         
							rotation_lim=0.80f;
				}
				else
				{
					k_xy=0.0f;
					rotation_lim=1.0f;
				}		
				
				if(PowerState == CAP)
				{
					carSpeedw = LIMIT_MAX_MIN(chassis.carSpeedw,CAP_MAX_W,-CAP_MAX_W);
				}
				else 
				{
//        匀速小陀螺		
   if(Actual_P_max < 70)
	 {		 
				carSpeedw = LIMIT_MAX_MIN(chassis.carSpeedw, rotation_lim*Self_Protect_Limit, -rotation_lim*Self_Protect_Limit);
	 }
	 else
	 {
//        变速小陀螺
				rand_w = (RandThreshold+(1-RandThreshold)*rand_A)*Self_Protect_Limit;
				carSpeedw = LIMIT_MAX_MIN(chassis.carSpeedw, rotation_lim*(rand_w), -rotation_lim*(rand_w));
	 }
				}
			}
		break;
			
		case Chassis_Solo_Mode:
		{
			if((ABS(chassis.carSpeedx) >100) && (ABS(chassis.carSpeedy) >100))
				k_xy = 1.4f;
			else
				k_xy = 2;
			carSpeedw = chassis.carSpeedw; 
		}
		break;
		
		default: 
			k_xy = 0;
			carSpeedw = 0;
			break;
	}
	
	
//根据不同功率 对应不同xy向速度系数
	if(PowerState == CAP)
	{
		k_xy *= k_CAP;
	}
	else 
	{
		k_xy *= k_BAT;
	}
	
	// 取弹时缓慢移动
	if(slow_flag)
	{
		if((ABS(chassis.carSpeedx) <500) && (ABS(chassis.carSpeedy)>500))
			k_xy =1.0f;     //横着走
		else
			k_xy = 0.5f;    //直着走
	}
	
//	ABS_Cal();
	Filter_Cal();

}

/**********************************************************************************************************
*函 数 名: PowerLimit
*功能说明: 功率限制
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
#define MaxOutPower  250     //最大输出功率限制
#define   K_P        3.2*1E5   //功率系数
float test_W_Chassis_t1 = 0,test_W_Chassis_t2 = 0;	//测试估算功率修正

short test_Jump[4];

float	W_Grad[10] = {0.98f,0.98f,0.98f,0.95f,0.95f,0.9f,0.9f,0.9f,0.85f,0.85f};
short DescendFlag;
short i;
extern char XStopFlag;
float ExcPower;
float EnergyMargin = 10.0f;		//留有的缓存能量余量
float My_P_max;				//计算的当前最大功率
void PowerLimit(void)
{
	
	float W_Chassis_t = 0;//底盘功率
  static float PowerMargin  = 150.0f;   //瓦，功率超出量，便于快速起步
	static float k_ExcessPower;
	
	if(ABS(carSpeedw)>2000)
	{
	EnergyMargin = 10.0f;
	}
	else
	{
	EnergyMargin = 10.0f;
		for(int m=0;m<4;m++)
		{
			if(!ChassisSetUp[m])
			{
				EnergyMargin = 30.0f;
				break;
			}
		}
	}
	
//设置最大功率
	if(JudgeReceive.remainEnergy <= EnergyMargin)
	{
		My_P_max = JudgeReceive.MaxPower*0.8f;
	}
	else 
	{
		// 裁判系统0.1s检测结算一次
		ExcPower = PowerMargin*(JudgeReceive.remainEnergy-EnergyMargin)/(60-EnergyMargin);
		My_P_max = LIMIT_MAX_MIN(ExcPower+JudgeReceive.MaxPower, MaxOutPower, JudgeReceive.MaxPower);
	}
	
//接收电流滤波
	Current_Filter_Excu();
	
	for(i = 0;i < 4;i ++)
	{
		Pid_Current[i].SetPoint = PID_Calc(&pidChassisWheelSpeed[i],ChassisMotorCanReceive[i].RealSpeed);
		Current_Set_Jump();
		Current_Change[i] = PID_Calc(&Pid_Current[i],Flow[i]);
		
		
		if(Set_Jump[i] == 0)
		{
			Current_f[i] += Current_Change[i];
		}
		else if(Set_Jump[i] == 1)
		{
			Current_f[i] = Pid_Current[i].SetPoint;
		}
		WheelCurrentSend[i] = Current_f[i];	
                                        //计算当前功率
		W_Chassis_t += ABS(WheelCurrentSend[i]*ChassisMotorCanReceive[i].RealSpeed);//功率计算
		
	}
	
	W_Chassis_t /= K_P;
	test_W_Chassis_t1 = W_Chassis_t;	
	
	
//当前功率超过最大功率 分次削减速度,最多削减10次
	DescendFlag = 0;
	while(W_Chassis_t > My_P_max && DescendFlag < 20)
	{
		W_Chassis_t = 0;
		
		for(i=0;i<4;i++)//通过削减速度减小电流值
		{
//			//速度骤减，这时应适当放大(有问题，待完善)
//			if(WheelStopFlag[i])
//			{
//			pidChassisWheelSpeed[i].SetPoint /= W_Grad[DescendFlag];
//			}
//			else  //正常加速
//			{
			pidChassisWheelSpeed[i].SetPoint *= W_Grad[DescendFlag];			
//			}
			
			//速度环+电流环
			Pid_Current[i].SetPoint = PID_Calc(&pidChassisWheelSpeed[i],ChassisMotorCanReceive[i].RealSpeed);
			Current_Filter_Excu();
			Current_Set_Jump();
			Current_Change[i] = PID_Calc(&Pid_Current[i],Flow[i]);
			if(Set_Jump[i] == 0)
			{
				Current_f[i] += Current_Change[i];
			}
			else if(Set_Jump[i] == 1)
			{
				Current_f[i] = Pid_Current[i].SetPoint;
			}
			WheelCurrentSend[i] = Current_f[i];
			
		W_Chassis_t += ABS(WheelCurrentSend[i]*ChassisMotorCanReceive[i].RealSpeed);//功率计算
		
		}	
		
		W_Chassis_t /= K_P;
		DescendFlag++;
	}
	test_W_Chassis_t2 = W_Chassis_t;

	//当前功率不到最大功率0.9,分次提高速度,最多提高10次
//	DescendFlag = 0;
//	while(W_Chassis_t < My_P_max*0.9f && DescendFlag < 10)
//	{
//		W_Chassis_t = 0;
//		
//		for(i=0;i<4;i++)//通过削减速度减小电流值
//		{
//			pidChassisWheelSpeed[i].SetPoint /= W_Grad[DescendFlag];
//			
//			//速度环+电流环
//			Pid_Current[i].SetPoint = PID_Calc(&pidChassisWheelSpeed[i],ChassisMotorCanReceive[i].RealSpeed);
//			Current_Filter_Excu();
//			Current_Set_Jump();
//			Current_Change[i] = PID_Calc(&Pid_Current[i],Flow[i]);
//			if(Set_Jump[i] == 0)
//			{
//				Current_f[i] += Current_Change[i];
//			}
//			else if(Set_Jump[i] == 1)
//			{
//				Current_f[i] = Pid_Current[i].SetPoint;
//			}
//			WheelCurrentSend[i] = Current_f[i];
//			
//		W_Chassis_t += ABS(WheelCurrentSend[i]*ChassisMotorCanReceive[i].RealSpeed);//功率计算
//		
//		}	
//		
//		W_Chassis_t /= K_P;
//		DescendFlag++;
//	}
//	
//	test_W_Chassis_t2 = W_Chassis_t;
//	
	
}

/**********************************************************************************************************
*函 数 名: Chassis_CurrentPid_Cal
*功能说明: 底盘操作
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void Chassis_CurrentPid_Cal(void)
{
	int i=0;
	Method_Check();			//设置当前功率参数
	Chassis_Speed_Cal();//根据xyw向速度计算目标速度值
	
	if(F405.Chassis_Flag == Chassis_Powerdown_Mode)
	{
				
		for(i=0;i<4;i++)
		{
			WheelCurrentSend[i] = 0;
		}
	}
	else if(PowerState == CAP)
	{
		
		for(i=0;i<4;i++)
		{
			WheelCurrentSend[i] = PID_Calc(&pidChassisWheelSpeed[i],ChassisMotorCanReceive[i].RealSpeed);
		}
	}
	else
	{
		PowerLimit();
	}
	//发送电流值在任务函数中
}

/**********************************************************************************************************
*函 数 名: Current_Filter_Excu
*功能说明: 将四个轮子的电流反馈值分别滤波
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void Current_Filter_Excu(void)
{
	for(int i = 0;i < 4;i++)
	{
		Input[i] = (float)ChassisMotorCanReceive[i].Current; 
	}
	Fir(Input,Output);
}

/**********************************************************************************************************
*函 数 名: Current_Set_Jump
*功能说明: 判断是否用电流环
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
//short Current_Set_Flag;			//四个轮子都可用电流环时，才用电流环
//210513 分别各自开电流环 效果更好
void Current_Set_Jump(void)
{
	int i;
	for(i = 0;i < 4;i ++)
	{
		if(F405.Chassis_Flag == Chassis_Act_Mode)		//正常模式
		{
			if(ABS(Pid_Current[i].SetPoint - Pid_Current[i].SetPointLast) > 1500)
				Set_Jump[i] = 0;   // 0
			else
				Set_Jump[i] = 1;
		}
		else
			Set_Jump[i] = 1;
	}
}
/**********************************************************************************************************
*函 数 名: Chassis_Power_Control_Init
*功能说明: 底盘功率限制参数初始化
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void Chassis_Power_Control_Init(void)
{
	int num = 0;
	switch(Robot_ID)
{
/****************************************  3号车   ************************************************************/
		case 3:
{	/****************默认参数********************/       //3号车
	Power_method[num].Actual_P_max = 60;
	Power_method[num].Self_Protect_Limit = 4000;
	Power_method[num].k_BAT = 1.2f;
	
	/****************40W********************/
	num++;
	Power_method[num].Actual_P_max = 40;                 //3号车
	Power_method[num].Self_Protect_Limit = 3000;
	Power_method[num].k_BAT = 0.8f;

	/****************45W********************/
	num++;
	Power_method[num].Actual_P_max = 45;                //3号车
	Power_method[num].Self_Protect_Limit = 3000;
	Power_method[num].k_BAT = 0.9f;

	/****************50W********************/
	num++;
	Power_method[num].Actual_P_max = 50;               //3号车
	Power_method[num].Self_Protect_Limit = 3500;
	Power_method[num].k_BAT = 1.0f;   //0.6f

	/****************60W********************/
	num++;                                             //3号车
	Power_method[num].Actual_P_max = 60;                   
	Power_method[num].Self_Protect_Limit = 4000;  //小陀螺控制转速
	Power_method[num].k_BAT = 1.0f;   // 0.75f              //xy向速度系数
//	Power_method[num].CurrentMax = 12000;
	/****************80W********************/
	num++;
	Power_method[num].Actual_P_max = 80;               //3号车
	Power_method[num].Self_Protect_Limit = 5500;
	Power_method[num].k_BAT = 1.5f;   //
//	Power_method[num].CurrentMax = 14000;
	/****************100W********************/
	num++;                                             //3号车
	Power_method[num].Actual_P_max = 100;
	Power_method[num].Self_Protect_Limit = 7000;
	Power_method[num].k_BAT = 1.8f;
//	Power_method[num].CurrentMax = 16000;
	/****************120W********************/
	num++;                                            //3号车
	Power_method[num].Actual_P_max = 120;
	Power_method[num].Self_Protect_Limit = 8000;
	Power_method[num].k_BAT = 2.0f;
//	Power_method[num].CurrentMax = 16000;
}break;
///****************************************  4号车   ************************************************************/
case 4:
{	/****************默认参数********************/       //4号车
	Power_method[num].Actual_P_max = 60;
	Power_method[num].Self_Protect_Limit = 4000;
	Power_method[num].k_BAT = 1.2f;
	
	/****************40W********************/
	num++;
	Power_method[num].Actual_P_max = 40;                 //4号车
	Power_method[num].Self_Protect_Limit = 3000;
	Power_method[num].k_BAT = 0.8f;

	/****************45W********************/
	num++;
	Power_method[num].Actual_P_max = 45;                //4号车
	Power_method[num].Self_Protect_Limit = 3500;
	Power_method[num].k_BAT = 0.9f;

	/****************50W********************/
	num++;
	Power_method[num].Actual_P_max = 50;               //4号车
	Power_method[num].Self_Protect_Limit = 3700;
	Power_method[num].k_BAT = 1.0f;   //0.6f

	/****************55W********************/
	num++;
	Power_method[num].Actual_P_max = 55;               //14号车
	Power_method[num].Self_Protect_Limit = 3700;
	Power_method[num].k_BAT = 1.0f;   //0.6f

	/****************60W********************/
	num++;                                             //4号车
	Power_method[num].Actual_P_max = 60;                   
	Power_method[num].Self_Protect_Limit = 4000;  //小陀螺控制转速
	Power_method[num].k_BAT = 1.0f;   // 0.75f              //xy向速度系数
//	Power_method[num].CurrentMax = 12000;
	/****************80W********************/
	num++;
	Power_method[num].Actual_P_max = 80;               //4号车
	Power_method[num].Self_Protect_Limit = 5500;
	Power_method[num].k_BAT = 1.5f;   //
//	Power_method[num].CurrentMax = 14000;
	/****************100W********************/
	num++;                                             //4号车
	Power_method[num].Actual_P_max = 100;
	Power_method[num].Self_Protect_Limit = 7000;
	Power_method[num].k_BAT = 1.8f;
//	Power_method[num].CurrentMax = 16000;
	/****************120W********************/
	num++;                                            //4车
	Power_method[num].Actual_P_max = 120;
	Power_method[num].Self_Protect_Limit = 8000;
	Power_method[num].k_BAT = 2.0f;
//	Power_method[num].CurrentMax = 16000;
}break;
///****************************************  自适应4号车   ************************************************************/
		case 14:
{	/****************默认参数********************/       //14号车
	Power_method[num].Actual_P_max = 60;
	Power_method[num].Self_Protect_Limit = 4000;
	Power_method[num].k_BAT = 1.2f;
	
	/****************40W********************/
	num++;
	Power_method[num].Actual_P_max = 40;                 //14号车
	Power_method[num].Self_Protect_Limit = 3000;
	Power_method[num].k_BAT = 0.8f;

	/****************45W********************/
	num++;
	Power_method[num].Actual_P_max = 45;                //14号车
	Power_method[num].Self_Protect_Limit = 3000;
	Power_method[num].k_BAT = 0.9f;

	/****************50W********************/
	num++;
	Power_method[num].Actual_P_max = 50;               //14号车
	Power_method[num].Self_Protect_Limit = 3500;
	Power_method[num].k_BAT = 1.0f;   //0.6f

	/****************60W********************/
	num++;                                             //14号车
	Power_method[num].Actual_P_max = 60;                   
	Power_method[num].Self_Protect_Limit = 5400;  //4300   小陀螺控制转速
	Power_method[num].k_BAT = 1.0f;   // 0.75f              //xy向速度系数
//	Power_method[num].CurrentMax = 12000;
	/****************80W********************/
	num++;
	Power_method[num].Actual_P_max = 80;               //14号车
	Power_method[num].Self_Protect_Limit = 6500;   //5500
	Power_method[num].k_BAT = 1.5f;   //
//	Power_method[num].CurrentMax = 14000;
	/****************100W********************/
	num++;                                             //14号车
	Power_method[num].Actual_P_max = 100;
	Power_method[num].Self_Protect_Limit = 7800;  //7000
	Power_method[num].k_BAT = 1.8f;
//	Power_method[num].CurrentMax = 16000;
	/****************120W********************/
	num++;                                            //14号车
	Power_method[num].Actual_P_max = 120;
	Power_method[num].Self_Protect_Limit = 8000;
	Power_method[num].k_BAT = 2.0f;
//	Power_method[num].CurrentMax = 16000;
}break;
default:
{
/****************默认参数********************/ 
	Power_method[num].Actual_P_max = 60;
	Power_method[num].Self_Protect_Limit = 2500;
	Power_method[num].k_BAT = 0.7f;
	Power_method[num].CurrentMax = 10000;
}
}
}
/**********************************************************************************************************
*函 数 名: Pid_ChassisWheelInit
*功能说明: 底盘XY向运动PID参数初始化
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void Pid_ChassisWheelInit(void)
{
	short i=0;
	
	for(i = 0;i < 4;i ++)
	{

		//电流环
		Pid_Current[i].P = 0.16f;
		Pid_Current[i].I = 0.0f;
		Pid_Current[i].D = 0.0f;
		Pid_Current[i].IMax = 2500;//2500
		Pid_Current[i].SetPoint = 0.0f;
		Pid_Current[i].OutMax = 8000.0f;	//8000.0f
		
		//速度环
		pidChassisWheelSpeed[i].P = 3.0f;
		pidChassisWheelSpeed[i].I = 0.0f;
		pidChassisWheelSpeed[i].D = 0.0f;
		pidChassisWheelSpeed[i].ErrorMax = 1000.0f;
		pidChassisWheelSpeed[i].IMax = 0.0f;
		pidChassisWheelSpeed[i].SetPoint = 0.0f;	
		pidChassisWheelSpeed[i].OutMax = 16000.0f;	
	
	}
}

/**********************************************************************************************************
*函 数 名: HeatControl
*功能说明: 热量控制
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
//变量定义
extern JudgeReceive_t JudgeReceive;

uint16_t HeatMax17, HeatCool17;
const short BulletHeat17 = 10;

short CurHeat17, LastHeat17, AvailableHeat17; //当前热量， 上一次热量, 自行计算热量

uint16_t Shooted17Cnt;	//一周期内已打出子弹数
uint16_t AvailableBullet17;	//下一周期允许打弹数

char ShootAbleFlag;

void HeatControl(void)
{
	if(JudgeReceive.HeatUpdateFlag == 1)	//热量更新
	{
		Shooted17Cnt = 0;
		AvailableHeat17 = LIMIT_MAX_MIN(HeatMax17 - CurHeat17 + HeatCool17,HeatMax17,0);
//		AvailableHeat17 = HeatMax17 - CurHeat17;
		if(JudgeReceive.ShootCpltFlag == 1)	//检测到发弹。为热量更新后打出的子弹
		{
			AvailableHeat17 = LIMIT_MAX_MIN(AvailableHeat17 - BulletHeat17,HeatMax17,0);
			JudgeReceive.ShootCpltFlag = 0;	//已处理完本次发弹
		}
		AvailableBullet17 = AvailableHeat17 / BulletHeat17;
		ShootAbleFlag = (AvailableBullet17 < 1)?0:1;		
	}	
	
	else if((JudgeReceive.ShootCpltFlag == 1) && (JudgeReceive.HeatUpdateFlag == 0))	//热量没有更新，但检测到发弹
	{
		JudgeReceive.ShootCpltFlag = 0;		//已处理完本次发弹
		Shooted17Cnt++;		//发射了一发子弹
		ShootAbleFlag = (Shooted17Cnt >= AvailableBullet17)?0:1;		
	}
}

/**********************************************************************************************************
*函 数 名: HeatUpdate
*功能说明: 热量更新
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
const float HeatControlThreshold = 0.8f;   	//开启热量控制的阈值

void HeatUpdate(void)
{
//	HeatMax17 = JudgeReceive.HeatMax17 + (short)(1250/JudgeReceive.maxHP) - BulletHeat17;		//榨干热量.jpg
	HeatMax17 = JudgeReceive.HeatMax17 - BulletHeat17;		//榨干热量，只保留一颗弹丸的余量
	HeatCool17 = JudgeReceive.HeatCool17/10;          // 热量每次检测的冷却值
	CurHeat17 = JudgeReceive.shooterHeat17;          //接收到的裁判系统热量
	
	if(CurHeat17 != LastHeat17)
	{
		JudgeReceive.HeatUpdateFlag = 1;
		JudgeReceive.ShootCpltFlag = 0;			//热量更新则将发射标志位清零(没有代处理的打弹)
	}
	
	if(CurHeat17 < HeatControlThreshold*HeatMax17)
	{
		ShootAbleFlag = 1;
		JudgeReceive.ShootCpltFlag = 0;
	}
	else
	{
		if((JudgeReceive.ShootCpltFlag == 1) || (JudgeReceive.HeatUpdateFlag == 1))
		HeatControl();
	}
	
	JudgeReceive.HeatUpdateFlag = 0;		//已处理完本次热量更新
	LastHeat17 = CurHeat17;
	F105.IsShootAble = ShootAbleFlag;
}

/**********************************************************************************************************
*函 数 名: BuildF105
*功能说明: 构建要传给上层板的F105结构体
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void BuildF105(void)
{
	if(JudgeReceive.robot_id < 10)
		F105.RobotRed = 1;
	else
		F105.RobotRed = 0;			//0为蓝色，1为红色
	switch(JudgeReceive.BulletSpeedMax17)
	{
		case 15:
		{
			F105.BulletSpeedLevel = 0;
			break;
		}
		case 18:
		{
			F105.BulletSpeedLevel = 1;
			break;
		}
		case 30:
		{
			F105.BulletSpeedLevel = 2;
			break;
		}
		default:
		{
			F105.BulletSpeedLevel = 0;
			break;
		}
	}
	F105.ChassisSpeedw=0.026f*(ChassisMotorCanReceive[0].RealSpeed+ChassisMotorCanReceive[1].RealSpeed+ChassisMotorCanReceive[2].RealSpeed+ChassisMotorCanReceive[3].RealSpeed);

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
  portTickType xLastWakeTime;
	const portTickType xFrequency = 1;
	
  while (1) {
    xLastWakeTime = xTaskGetTickCount();
		
		
		//电容充放电控制
		if(JudgeReceive.remainEnergy<20)
		{
		Charge_Off;
		ChargeState = ChargeOff ;
		}
		else
		{
			Charge_On;
			ChargeState = ChargeOn;
		}	
	
		//功率限制
    Chassis_CurrentPid_Cal();
	
		ChassisCan1Send(WheelCurrentSend[0],WheelCurrentSend[1],WheelCurrentSend[2],WheelCurrentSend[3]); 
		
		//热量控制
		HeatUpdate();
		BuildF105();
		Can2Send0(&F105);
		
 
   	VOFA_Send();
	
		IWDG_Feed();//喂狗		
		vTaskDelayUntil(&xLastWakeTime,xFrequency); 
		 
#if INCLUDE_uxTaskGetStackHighWaterMark
        Chassis_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}
