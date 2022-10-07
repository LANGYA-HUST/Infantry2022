/**********************************************************************************************************
 * @�ļ�     ChassisTask.c
 * @˵��     ���̿���+��������
 * @�汾  	 V3.0
 * @����     ��ҵȨ
 * @����     2021.4.26
**********************************************************************************************************/
/**********************************************************************************************************
 * @�ļ�     ChassisTask.c
 * @˵��     ���̿���+��������
 * @�汾  	 V4.0
 * @����     ����
 * @����     2022.6
**********************************************************************************************************/
#include "main.h"

#define CAP_MAX_W      7000
#define Rand_S         0.5f   //���ڳ���
#define RandThreshold  0.5f   //ֱ��ƫ��
#define RANDA          1.3f   //���ҷ�ֵ

float k_CAP = 2.0f;
/*----------------------------------�ڲ�����---------------------------*/
short WheelCurrentSend[4];
short Set_Jump[4] = {0};
float Current_Change[4] = {0};//����������
float Current_f[4] = {0};//�������f
float Flow[4] = {0};//ʵ�ʵ���f
float speed[4] = {0};//ʵ���ٶ�f
char  WheelStopFlag[4] ; //��־���Ӽ��ٵı�־
//��������ϵ�� PowerLimit
short Actual_P_max;						//ʵ�������
short Self_Protect_Limit;			//С����ת������
float k_BAT;
char ChassisSetUp[4];

TickType_t Tickcnt=0;


/*----------------------------------�ṹ��-----------------------------*/
Pid_Typedef Pid_Current[4];
Pid_Typedef pidChassisWheelSpeed[4];
Pid_Typedef pidChassisPosition_Speed;
F105_Typedef F105;
Power_Typedef Power_method[METHOD_NUM];

/*----------------------------------�ⲿ����---------------------------*/
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
float T_ABS=1000.0f;//ɲ��ʱ��
float T_SETUP=800.0f;//����ʱ��
extern char output_filter;
extern enum CHARGESTATE_Typedef ChargeState;
/**********************************************************************************************************
*�� �� ��: ABS_Cal
*����˵��: ������ͣ
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
//void ABS_Cal(void)
//{
//static TickType_t StartTick=0,NowTick;
//static TickType_t StopTick=0;
//static char StartInitFlag=0,StopInitFlag=0;
//if(chassis.carSpeedx>50||chassis.carSpeedy>50) //������ʻ����ʼ�������
//{
//	
//	if(Goready_flag==1)//��ʼ������������̬
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
//	else if(Goready_flag==0)//����ƽ��̬
//	{

//	ABSready_flag=1; //׼���û���ɲ�� ע�⣺ֻ�������ٺ�Ż���ɲ��
//  StopInitFlag=0;
//  output_filter = 1;
//	}

//}

//else if((ABS(chassis.carSpeedx) <50)&&(ABS(chassis.carSpeedy) <50))//��ʼɲ��
//{	
//	if(ABSready_flag==1)  //ֻ����ɲ��״̬�²ſ�ʼ��ʱ
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
// if((NowTick-StopTick)>=T_ABS ||ABSready_flag==0 || ABS(ChassisMotorCanReceive[3].RealSpeed)<=200)  //��ȫɲס��
// {
//  ABSready_flag=0;   //ɲ�����
//	StopInitFlag=0;
// }
//  Goready_flag=1;// ׼����������̬
//	StartInitFlag=0;
//}

// }
/**********************************************************************************************************
*�� �� ��: ABS_Cal
*����˵��: ������ͣ
*��    ��: ��
*�� �� ֵ: ��
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
	
//	for(int t=0;t<4;t++)  //���ڱ�־�����ٶȱ仯����
//	{
//	  if(ABS(pidChassisWheelSpeed[t].SetPoint) +100 < ABS(pidChassisWheelSpeed[t].SetPointLast)) //���⾲ֹ���
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
	//  ��־�����Ѿ�����
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
*�� �� ��: Method_Check
*����˵��: ���ݲ�ͬ�Ĺ���ѡ��ͬ�Ŀ��Ʋ���
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
extern volatile TickType_t SampleTick;
TickType_t MyNowTick;
void Method_Check(void)
{
	short i;
	short choose_PN = 0;
	
	static short PN;
	static short last_PN;	
	
	for(choose_PN = 1;choose_PN < METHOD_NUM; choose_PN++)		//0���ڴ���Ĭ�ϲ���
	{
		if(Power_method[choose_PN].Actual_P_max == JudgeReceive.MaxPower)
		{
			PN = choose_PN;
			break;
		}	
	}
	if(choose_PN >= METHOD_NUM)	//û�ҵ�ƥ�������ʲ���
		PN = PN_Default;	//Ĭ�ϲ���

	if(last_PN != choose_PN)			//˵�������б仯
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
*�� �� ��: Chassis_Speed_Cal
*����˵��: ����xyw���ٶȼ���Ŀ���ٶ�ֵ
*��    ��: ��
*�� �� ֵ: ��
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
	
//xy���ٶ���w���ٶ����
	switch(F405.Chassis_Flag)
	{
		case Chassis_Powerdown_Mode:
			k_xy = 0;
			carSpeedw = 0;
		break;
		
		case Chassis_Act_Mode:
		case Chassis_Jump_Mode:
			
		if((ABS(chassis.carSpeedx)>500) && (ABS(chassis.carSpeedy) >500))
				k_xy = 2.0f;    //б����
			else
				k_xy = 3;       //ֱ����
			
		if((ABS(chassis.carSpeedx) <500) && (ABS(chassis.carSpeedy)>500))
				k_xy =2.5f;     //������
				
		
	   carSpeedw = chassis.carSpeedw;

	 
		break;
		
		case Chassis_SelfProtect_Mode:
		{
			
			//���ٴ���
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
//        ����С����		
   if(Actual_P_max < 70)
	 {		 
				carSpeedw = LIMIT_MAX_MIN(chassis.carSpeedw, rotation_lim*Self_Protect_Limit, -rotation_lim*Self_Protect_Limit);
	 }
	 else
	 {
//        ����С����
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
	
	
//���ݲ�ͬ���� ��Ӧ��ͬxy���ٶ�ϵ��
	if(PowerState == CAP)
	{
		k_xy *= k_CAP;
	}
	else 
	{
		k_xy *= k_BAT;
	}
	
	// ȡ��ʱ�����ƶ�
	if(slow_flag)
	{
		if((ABS(chassis.carSpeedx) <500) && (ABS(chassis.carSpeedy)>500))
			k_xy =1.0f;     //������
		else
			k_xy = 0.5f;    //ֱ����
	}
	
//	ABS_Cal();
	Filter_Cal();

}

/**********************************************************************************************************
*�� �� ��: PowerLimit
*����˵��: ��������
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
#define MaxOutPower  250     //��������������
#define   K_P        3.2*1E5   //����ϵ��
float test_W_Chassis_t1 = 0,test_W_Chassis_t2 = 0;	//���Թ��㹦������

short test_Jump[4];

float	W_Grad[10] = {0.98f,0.98f,0.98f,0.95f,0.95f,0.9f,0.9f,0.9f,0.85f,0.85f};
short DescendFlag;
short i;
extern char XStopFlag;
float ExcPower;
float EnergyMargin = 10.0f;		//���еĻ�����������
float My_P_max;				//����ĵ�ǰ�����
void PowerLimit(void)
{
	
	float W_Chassis_t = 0;//���̹���
  static float PowerMargin  = 150.0f;   //�ߣ����ʳ����������ڿ�����
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
	
//���������
	if(JudgeReceive.remainEnergy <= EnergyMargin)
	{
		My_P_max = JudgeReceive.MaxPower*0.8f;
	}
	else 
	{
		// ����ϵͳ0.1s������һ��
		ExcPower = PowerMargin*(JudgeReceive.remainEnergy-EnergyMargin)/(60-EnergyMargin);
		My_P_max = LIMIT_MAX_MIN(ExcPower+JudgeReceive.MaxPower, MaxOutPower, JudgeReceive.MaxPower);
	}
	
//���յ����˲�
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
                                        //���㵱ǰ����
		W_Chassis_t += ABS(WheelCurrentSend[i]*ChassisMotorCanReceive[i].RealSpeed);//���ʼ���
		
	}
	
	W_Chassis_t /= K_P;
	test_W_Chassis_t1 = W_Chassis_t;	
	
	
//��ǰ���ʳ�������� �ִ������ٶ�,�������10��
	DescendFlag = 0;
	while(W_Chassis_t > My_P_max && DescendFlag < 20)
	{
		W_Chassis_t = 0;
		
		for(i=0;i<4;i++)//ͨ�������ٶȼ�С����ֵ
		{
//			//�ٶ��������ʱӦ�ʵ��Ŵ�(�����⣬������)
//			if(WheelStopFlag[i])
//			{
//			pidChassisWheelSpeed[i].SetPoint /= W_Grad[DescendFlag];
//			}
//			else  //��������
//			{
			pidChassisWheelSpeed[i].SetPoint *= W_Grad[DescendFlag];			
//			}
			
			//�ٶȻ�+������
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
			
		W_Chassis_t += ABS(WheelCurrentSend[i]*ChassisMotorCanReceive[i].RealSpeed);//���ʼ���
		
		}	
		
		W_Chassis_t /= K_P;
		DescendFlag++;
	}
	test_W_Chassis_t2 = W_Chassis_t;

	//��ǰ���ʲ��������0.9,�ִ�����ٶ�,������10��
//	DescendFlag = 0;
//	while(W_Chassis_t < My_P_max*0.9f && DescendFlag < 10)
//	{
//		W_Chassis_t = 0;
//		
//		for(i=0;i<4;i++)//ͨ�������ٶȼ�С����ֵ
//		{
//			pidChassisWheelSpeed[i].SetPoint /= W_Grad[DescendFlag];
//			
//			//�ٶȻ�+������
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
//		W_Chassis_t += ABS(WheelCurrentSend[i]*ChassisMotorCanReceive[i].RealSpeed);//���ʼ���
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
*�� �� ��: Chassis_CurrentPid_Cal
*����˵��: ���̲���
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void Chassis_CurrentPid_Cal(void)
{
	int i=0;
	Method_Check();			//���õ�ǰ���ʲ���
	Chassis_Speed_Cal();//����xyw���ٶȼ���Ŀ���ٶ�ֵ
	
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
	//���͵���ֵ����������
}

/**********************************************************************************************************
*�� �� ��: Current_Filter_Excu
*����˵��: ���ĸ����ӵĵ�������ֵ�ֱ��˲�
*��    ��: ��
*�� �� ֵ: ��
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
*�� �� ��: Current_Set_Jump
*����˵��: �ж��Ƿ��õ�����
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
//short Current_Set_Flag;			//�ĸ����Ӷ����õ�����ʱ�����õ�����
//210513 �ֱ���Կ������� Ч������
void Current_Set_Jump(void)
{
	int i;
	for(i = 0;i < 4;i ++)
	{
		if(F405.Chassis_Flag == Chassis_Act_Mode)		//����ģʽ
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
*�� �� ��: Chassis_Power_Control_Init
*����˵��: ���̹������Ʋ�����ʼ��
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void Chassis_Power_Control_Init(void)
{
	int num = 0;
	switch(Robot_ID)
{
/****************************************  3�ų�   ************************************************************/
		case 3:
{	/****************Ĭ�ϲ���********************/       //3�ų�
	Power_method[num].Actual_P_max = 60;
	Power_method[num].Self_Protect_Limit = 4000;
	Power_method[num].k_BAT = 1.2f;
	
	/****************40W********************/
	num++;
	Power_method[num].Actual_P_max = 40;                 //3�ų�
	Power_method[num].Self_Protect_Limit = 3000;
	Power_method[num].k_BAT = 0.8f;

	/****************45W********************/
	num++;
	Power_method[num].Actual_P_max = 45;                //3�ų�
	Power_method[num].Self_Protect_Limit = 3000;
	Power_method[num].k_BAT = 0.9f;

	/****************50W********************/
	num++;
	Power_method[num].Actual_P_max = 50;               //3�ų�
	Power_method[num].Self_Protect_Limit = 3500;
	Power_method[num].k_BAT = 1.0f;   //0.6f

	/****************60W********************/
	num++;                                             //3�ų�
	Power_method[num].Actual_P_max = 60;                   
	Power_method[num].Self_Protect_Limit = 4000;  //С���ݿ���ת��
	Power_method[num].k_BAT = 1.0f;   // 0.75f              //xy���ٶ�ϵ��
//	Power_method[num].CurrentMax = 12000;
	/****************80W********************/
	num++;
	Power_method[num].Actual_P_max = 80;               //3�ų�
	Power_method[num].Self_Protect_Limit = 5500;
	Power_method[num].k_BAT = 1.5f;   //
//	Power_method[num].CurrentMax = 14000;
	/****************100W********************/
	num++;                                             //3�ų�
	Power_method[num].Actual_P_max = 100;
	Power_method[num].Self_Protect_Limit = 7000;
	Power_method[num].k_BAT = 1.8f;
//	Power_method[num].CurrentMax = 16000;
	/****************120W********************/
	num++;                                            //3�ų�
	Power_method[num].Actual_P_max = 120;
	Power_method[num].Self_Protect_Limit = 8000;
	Power_method[num].k_BAT = 2.0f;
//	Power_method[num].CurrentMax = 16000;
}break;
///****************************************  4�ų�   ************************************************************/
case 4:
{	/****************Ĭ�ϲ���********************/       //4�ų�
	Power_method[num].Actual_P_max = 60;
	Power_method[num].Self_Protect_Limit = 4000;
	Power_method[num].k_BAT = 1.2f;
	
	/****************40W********************/
	num++;
	Power_method[num].Actual_P_max = 40;                 //4�ų�
	Power_method[num].Self_Protect_Limit = 3000;
	Power_method[num].k_BAT = 0.8f;

	/****************45W********************/
	num++;
	Power_method[num].Actual_P_max = 45;                //4�ų�
	Power_method[num].Self_Protect_Limit = 3500;
	Power_method[num].k_BAT = 0.9f;

	/****************50W********************/
	num++;
	Power_method[num].Actual_P_max = 50;               //4�ų�
	Power_method[num].Self_Protect_Limit = 3700;
	Power_method[num].k_BAT = 1.0f;   //0.6f

	/****************55W********************/
	num++;
	Power_method[num].Actual_P_max = 55;               //14�ų�
	Power_method[num].Self_Protect_Limit = 3700;
	Power_method[num].k_BAT = 1.0f;   //0.6f

	/****************60W********************/
	num++;                                             //4�ų�
	Power_method[num].Actual_P_max = 60;                   
	Power_method[num].Self_Protect_Limit = 4000;  //С���ݿ���ת��
	Power_method[num].k_BAT = 1.0f;   // 0.75f              //xy���ٶ�ϵ��
//	Power_method[num].CurrentMax = 12000;
	/****************80W********************/
	num++;
	Power_method[num].Actual_P_max = 80;               //4�ų�
	Power_method[num].Self_Protect_Limit = 5500;
	Power_method[num].k_BAT = 1.5f;   //
//	Power_method[num].CurrentMax = 14000;
	/****************100W********************/
	num++;                                             //4�ų�
	Power_method[num].Actual_P_max = 100;
	Power_method[num].Self_Protect_Limit = 7000;
	Power_method[num].k_BAT = 1.8f;
//	Power_method[num].CurrentMax = 16000;
	/****************120W********************/
	num++;                                            //4��
	Power_method[num].Actual_P_max = 120;
	Power_method[num].Self_Protect_Limit = 8000;
	Power_method[num].k_BAT = 2.0f;
//	Power_method[num].CurrentMax = 16000;
}break;
///****************************************  ����Ӧ4�ų�   ************************************************************/
		case 14:
{	/****************Ĭ�ϲ���********************/       //14�ų�
	Power_method[num].Actual_P_max = 60;
	Power_method[num].Self_Protect_Limit = 4000;
	Power_method[num].k_BAT = 1.2f;
	
	/****************40W********************/
	num++;
	Power_method[num].Actual_P_max = 40;                 //14�ų�
	Power_method[num].Self_Protect_Limit = 3000;
	Power_method[num].k_BAT = 0.8f;

	/****************45W********************/
	num++;
	Power_method[num].Actual_P_max = 45;                //14�ų�
	Power_method[num].Self_Protect_Limit = 3000;
	Power_method[num].k_BAT = 0.9f;

	/****************50W********************/
	num++;
	Power_method[num].Actual_P_max = 50;               //14�ų�
	Power_method[num].Self_Protect_Limit = 3500;
	Power_method[num].k_BAT = 1.0f;   //0.6f

	/****************60W********************/
	num++;                                             //14�ų�
	Power_method[num].Actual_P_max = 60;                   
	Power_method[num].Self_Protect_Limit = 5400;  //4300   С���ݿ���ת��
	Power_method[num].k_BAT = 1.0f;   // 0.75f              //xy���ٶ�ϵ��
//	Power_method[num].CurrentMax = 12000;
	/****************80W********************/
	num++;
	Power_method[num].Actual_P_max = 80;               //14�ų�
	Power_method[num].Self_Protect_Limit = 6500;   //5500
	Power_method[num].k_BAT = 1.5f;   //
//	Power_method[num].CurrentMax = 14000;
	/****************100W********************/
	num++;                                             //14�ų�
	Power_method[num].Actual_P_max = 100;
	Power_method[num].Self_Protect_Limit = 7800;  //7000
	Power_method[num].k_BAT = 1.8f;
//	Power_method[num].CurrentMax = 16000;
	/****************120W********************/
	num++;                                            //14�ų�
	Power_method[num].Actual_P_max = 120;
	Power_method[num].Self_Protect_Limit = 8000;
	Power_method[num].k_BAT = 2.0f;
//	Power_method[num].CurrentMax = 16000;
}break;
default:
{
/****************Ĭ�ϲ���********************/ 
	Power_method[num].Actual_P_max = 60;
	Power_method[num].Self_Protect_Limit = 2500;
	Power_method[num].k_BAT = 0.7f;
	Power_method[num].CurrentMax = 10000;
}
}
}
/**********************************************************************************************************
*�� �� ��: Pid_ChassisWheelInit
*����˵��: ����XY���˶�PID������ʼ��
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void Pid_ChassisWheelInit(void)
{
	short i=0;
	
	for(i = 0;i < 4;i ++)
	{

		//������
		Pid_Current[i].P = 0.16f;
		Pid_Current[i].I = 0.0f;
		Pid_Current[i].D = 0.0f;
		Pid_Current[i].IMax = 2500;//2500
		Pid_Current[i].SetPoint = 0.0f;
		Pid_Current[i].OutMax = 8000.0f;	//8000.0f
		
		//�ٶȻ�
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
*�� �� ��: HeatControl
*����˵��: ��������
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
//��������
extern JudgeReceive_t JudgeReceive;

uint16_t HeatMax17, HeatCool17;
const short BulletHeat17 = 10;

short CurHeat17, LastHeat17, AvailableHeat17; //��ǰ������ ��һ������, ���м�������

uint16_t Shooted17Cnt;	//һ�������Ѵ���ӵ���
uint16_t AvailableBullet17;	//��һ�����������

char ShootAbleFlag;

void HeatControl(void)
{
	if(JudgeReceive.HeatUpdateFlag == 1)	//��������
	{
		Shooted17Cnt = 0;
		AvailableHeat17 = LIMIT_MAX_MIN(HeatMax17 - CurHeat17 + HeatCool17,HeatMax17,0);
//		AvailableHeat17 = HeatMax17 - CurHeat17;
		if(JudgeReceive.ShootCpltFlag == 1)	//��⵽������Ϊ�������º������ӵ�
		{
			AvailableHeat17 = LIMIT_MAX_MIN(AvailableHeat17 - BulletHeat17,HeatMax17,0);
			JudgeReceive.ShootCpltFlag = 0;	//�Ѵ����걾�η���
		}
		AvailableBullet17 = AvailableHeat17 / BulletHeat17;
		ShootAbleFlag = (AvailableBullet17 < 1)?0:1;		
	}	
	
	else if((JudgeReceive.ShootCpltFlag == 1) && (JudgeReceive.HeatUpdateFlag == 0))	//����û�и��£�����⵽����
	{
		JudgeReceive.ShootCpltFlag = 0;		//�Ѵ����걾�η���
		Shooted17Cnt++;		//������һ���ӵ�
		ShootAbleFlag = (Shooted17Cnt >= AvailableBullet17)?0:1;		
	}
}

/**********************************************************************************************************
*�� �� ��: HeatUpdate
*����˵��: ��������
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
const float HeatControlThreshold = 0.8f;   	//�����������Ƶ���ֵ

void HeatUpdate(void)
{
//	HeatMax17 = JudgeReceive.HeatMax17 + (short)(1250/JudgeReceive.maxHP) - BulletHeat17;		//ե������.jpg
	HeatMax17 = JudgeReceive.HeatMax17 - BulletHeat17;		//ե��������ֻ����һ�ŵ��������
	HeatCool17 = JudgeReceive.HeatCool17/10;          // ����ÿ�μ�����ȴֵ
	CurHeat17 = JudgeReceive.shooterHeat17;          //���յ��Ĳ���ϵͳ����
	
	if(CurHeat17 != LastHeat17)
	{
		JudgeReceive.HeatUpdateFlag = 1;
		JudgeReceive.ShootCpltFlag = 0;			//���������򽫷����־λ����(û�д�����Ĵ�)
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
	
	JudgeReceive.HeatUpdateFlag = 0;		//�Ѵ����걾����������
	LastHeat17 = CurHeat17;
	F105.IsShootAble = ShootAbleFlag;
}

/**********************************************************************************************************
*�� �� ��: BuildF105
*����˵��: ����Ҫ�����ϲ���F105�ṹ��
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void BuildF105(void)
{
	if(JudgeReceive.robot_id < 10)
		F105.RobotRed = 1;
	else
		F105.RobotRed = 0;			//0Ϊ��ɫ��1Ϊ��ɫ
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
*�� �� ��: Chassis_task
*����˵��: ��������
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
uint32_t Chassis_high_water;

void Chassis_task(void *pvParameters)
{
  portTickType xLastWakeTime;
	const portTickType xFrequency = 1;
	
  while (1) {
    xLastWakeTime = xTaskGetTickCount();
		
		
		//���ݳ�ŵ����
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
	
		//��������
    Chassis_CurrentPid_Cal();
	
		ChassisCan1Send(WheelCurrentSend[0],WheelCurrentSend[1],WheelCurrentSend[2],WheelCurrentSend[3]); 
		
		//��������
		HeatUpdate();
		BuildF105();
		Can2Send0(&F105);
		
 
   	VOFA_Send();
	
		IWDG_Feed();//ι��		
		vTaskDelayUntil(&xLastWakeTime,xFrequency); 
		 
#if INCLUDE_uxTaskGetStackHighWaterMark
        Chassis_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}
