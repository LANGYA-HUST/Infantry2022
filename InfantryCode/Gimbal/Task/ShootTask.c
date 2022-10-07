/**********************************************************************************************************
 * @�ļ�     ShootTask.c
 * @˵��     ��������
 * @�汾  	 V1.0
 * @����     ��־��
 * @����     2020.1
**********************************************************************************************************/
/**********************************************************************************************************
 * @�ļ�     ShootTask.c
 * @˵��     ��������
 * @�汾  	 V2.0
 * @����     ����
 * @����     2022.1
**********************************************************************************************************/
# include "main.h"
/*----------------------------------�ڲ�����---------------------------*/
float MirocPosition;
short FrictionWheel_speed=0,BulletSpeed,BodanMotorCurrent,ShootAct_Init_Flag;
short BodanMotorCurrent;
//int SendToTx2BullectCnt;
int Shoot_Init_flag = 0;
char ShootContinue=0;
int sum;
/*----------------------------------�ṹ��------------------------------*/
Pid_Typedef PidBodanMotorPos,PidBodanMotorSpeed,PidFrictionSpeed[2];
FuzzyPID FuzzyBodanMotorPos;
float Onegrid; 
/*----------------------------------�ⲿ����---------------------------*/
extern RC_Ctl_t RC_Ctl;
extern float Bodan_Pos;
extern F105_Typedef F105;//F105
extern F405_typedef F405;
extern Status_t Status;
extern Gimbal_Typedef Gimbal;
extern PC_Receive_t PC_Receive;
extern BodanMotorReceive_Typedef BodanReceive;
extern short armor_state;
extern RobotInit_Struct Infantry;
extern short FrictionReceive[2];
extern uint8_t CoolBuffState;

short PullerSpeed ;        //  1000 ��ȴ1����Ƶ4.5     2000 2��6.5  3000 12.5
short checkPullerSpeed;             //  800  4

/**********************************************************************************************************
*�� �� ��: PluckSpeedChoose
*����˵��: ���̲���ѡ��
*��    ��: flag
*�� �� ֵ: ��
**********************************************************************************************************/
extern char HighFreq_flag;
void Pluck_Speed_Choose()
{
	if(HighFreq_flag)
	{
		PullerSpeed = 4500;
	}
	else
	{
  	switch(F105.RobotLevel)
  {	
       /***** 1�� ******/
		case 1:
			if(!CoolBuffState)
			{
     	 PullerSpeed = 3000;    //������
			}
			else
			{
			 PullerSpeed = 4000;    //������ 
			}
		break;
		
		 /****** 2�� *****/
		case 2:
		if(!CoolBuffState)
		{
     	 PullerSpeed = 3500;    //������
		}
		else
		{
			 PullerSpeed = 4000;    //������ 
		}
		break;
		
    /******  3�� ******/		
		case 3:
			if(!CoolBuffState)
		{
     	 PullerSpeed = 4000;    //������
		}
		else
		{
			 PullerSpeed = 4500;    //������ 
		}
		break;
		
		
		default:
		PullerSpeed = 3500;
	 }	
  }
}
/**********************************************************************************************************
*�� �� ��: FrictionSpeedChoose
*����˵��: Ħ����ת��ѡ��
*��    ��: flag
*�� �� ֵ: ��
**********************************************************************************************************/
void FrictionSpeedChoose(void)
{
	switch(F105.BulletSpeedLevel)
	{
		case 0:
		{
			FrictionWheel_speed = Infantry.Low_FrictionSpeed;
			break;
		}
		case 1:
		{
				FrictionWheel_speed = Infantry.Medium_FrictionSpeed;
			break;
		}
		case 2:
		{
				FrictionWheel_speed = Infantry.High_FrictionSpeed;
			break;
		}
		default:
		{
			FrictionWheel_speed = Infantry.Low_FrictionSpeed;
			break;
		}
	}
}

///**********************************************************************************************************
//*�� �� ��: HeatUpdate
//*����˵��: ��������
//*��    ��: ��
//*�� �� ֵ: ��
//**********************************************************************************************************/
//char HeatUpdateFlag;
//uint16_t HeatMax17, HeatCool17;
//const short BulletHeat17 = 10;
//short CurHeat17, AvailableHeat17; //��ǰ������ ��һ������, ���м�������
//uint16_t Shooted17Cnt;	//һ�������Ѵ���ӵ���
//uint16_t AvailableBullet17;	//��һ�����������

//void HeatUpdate(void)
//{
//	HeatMax17 = F105.HeatMax17 - BulletHeat17;		//ե��������ֻ����һ�ŵ��������
//	HeatCool17 = F105.HeatCool17/10;          // ����ÿ�μ�����ȴֵ
//	CurHeat17 = F105.shooterHeat17;          //���յ��Ĳ���ϵͳ����
//	
//	if(CurHeat17 != LastHeat17)
//	{
//		HeatUpdateFlag = 1;
//	}
//	
//		if(HeatUpdateFlag == 1)
//	{
//    AvailableHeat17 = LIMIT_MAX_MIN(HeatMax17 - CurHeat17 + HeatCool17,HeatMax17,0);
//		AvailableBullet17 = AvailableHeat17 / BulletHeat17;	
//  }
//	
//	HeatUpdateFlag = 0;		//�Ѵ����걾����������
//}

/**********************************************************************************************************
*�� �� ��: Shoot_Fire_Cal
*����˵��: ����ģʽ
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
extern char ReverseRotation;
char Reverse_Flag=0;
void Shoot_Fire_Cal()
{
	if(ShootAct_Init_Flag!=1)
	{
		ShootAct_Init_Flag=1;
	}

	
	if(F105.IsShootAble || HighFreq_flag)
	{

			if(ShootContinue)
			{
				PidBodanMotorSpeed.SetPoint=PullerSpeed;
			}
			else
			{
				 //���Ʋ��̷�ת��������
				if(ReverseRotation)
				{
						if(Reverse_Flag==0)
						{
							PidBodanMotorPos.SetPoint=PidBodanMotorPos.SetPoint-Onegrid;
							Reverse_Flag=1;
						}
						if(ABS(Bodan_Pos-PidBodanMotorPos.SetPoint) < PluckThreholdPos)
						{
							ReverseRotation=0;
							Reverse_Flag=0;
						}
				
				}
	
				else
				{

						if(ABS(Bodan_Pos-PidBodanMotorPos.SetPoint) < PluckThreholdPos)
						{
							PidBodanMotorPos.SetPoint=PidBodanMotorPos.SetPoint+MirocPosition; 
						}
			
				}
				PidBodanMotorPos.ActualValue = Bodan_Pos;
				PidBodanMotorSpeed.SetPoint = PID_Calc(&PidBodanMotorPos);			
			}
		
	}
	//��������
	else
	{
		Stop_Shoot:	PidBodanMotorSpeed.SetPoint=0;	
	}
		MirocPosition = 0;
}

/**********************************************************************************************************
*�� �� ��: Shoot_Check_Cal
*����˵��: ��¼ģʽ[�򿪼��⣬Ħ����/��Ƶ3~4��]
*��    ��: rc
*�� �� ֵ: ��
**********************************************************************************************************/
int delay_time =-1000;
void Shoot_Check_Cal()
{ 
	delay_time++;
	if(ShootAct_Init_Flag!=2)
	ShootAct_Init_Flag=2;
	MirocPosition = 0;
	 if(delay_time>0)
	 {
		if(F105.IsShootAble==1)
		{
      if(Shoot_Init_flag == 1)
			{				
//				PidBodanMotorPos.SetPoint=PidBodanMotorPos.SetPoint+500; 
//				PidBodanMotorSpeed.SetPoint = PID_Calc(&PidBodanMotorPos,Bodan_Pos);
				PidBodanMotorSpeed.SetPoint=checkPullerSpeed;
				delay_time=0;	
			}
		}
		else
		{
				PidBodanMotorSpeed.SetPoint=0;	
		}
	}
}

/**********************************************************************************************************
*�� �� ��: Shoot_Test_Cal
*����˵��: ����ģʽ�����ڸ�����λ������
*��    ��: 
*�� �� ֵ: ��
**********************************************************************************************************/
extern char Aim_Follow;
TickType_t EnterAimTick,nowAimTick;
void Shoot_Test_Cal()
{ 
  delay_time++;
	if(ShootAct_Init_Flag!=0xcf)
	{
	ShootAct_Init_Flag=0xcf;
	EnterAimTick = xTaskGetTickCount();
	}
	nowAimTick = xTaskGetTickCount();
	if(delay_time>0)
	{ 
//		if(((nowAimTick-EnterAimTick)>200) && F105.IsShootAble==1 && armor_state == ARMOR_AIMED && Aim_Follow==1)
		if(((nowAimTick-EnterAimTick)>200) && F105.IsShootAble==1 && armor_state == ARMOR_AIMED)		
		{
			if(ABS(Bodan_Pos-PidBodanMotorPos.SetPoint) < PluckThreholdPos)
			{
				PidBodanMotorPos.SetPoint = PidBodanMotorPos.SetPoint+Onegrid; 
				armor_state=ARMOR_NO_AIM;			
			}
		}
	}	
	 PidBodanMotorPos.ActualValue = Bodan_Pos;
	 PidBodanMotorSpeed.SetPoint = PID_Calc(&PidBodanMotorPos);		
 
}

/**********************************************************************************************************
*�� �� ��: Shoot_Powerdown_Cal
*����˵��: ����ģʽ[΢������/Ħ����/����ת��]
*��    ��: rc
*�� �� ֵ: ��
**********************************************************************************************************/
void Shoot_Powerdown_Cal(void)
{
	if(ShootAct_Init_Flag!=4)
	  ShootAct_Init_Flag=4;
	
	PidBodanMotorPos.SetPoint = PidBodanMotorPos.ActualValue = Bodan_Pos;
	PidBodanMotorSpeed.SetPoint=PID_Calc(&PidBodanMotorPos);
	ReverseRotation = 0;
	Reverse_Flag = 0;
	ShootContinue = 0;
}

///**********************************************************************************************************
//*�� �� ��: Shoot_Tx2_Cal
//*����˵��: ����ģʽ
//*��    ��: rc
//*�� �� ֵ: ��
//**********************************************************************************************************/
//extern short armor_state;
//extern float k_onegrid;
//void Shoot_Tx2_Cal()
//{
//	if(ShootAct_Init_Flag!=3)
//	{
//		MirocPosition = 0;
//		ShootAct_Init_Flag=3;
//		SendToTx2BullectCnt=PC_Receive.ReceiveFromTx2BullectCnt=0;
//	}
///***********************************************Armor******************************************/	
//	if(Status.GimbalMode == Gimbal_Armor_Mode)
//	{
//		if(F105.IsShootAble==1)
//  	{
//			if(ABS(PC_Receive.RCPitch - (-Gimbal.Pitch.Gyro))<1.5f && ABS(PC_Receive.RCYaw - Gimbal.Yaw.Gyro)<1.5f)	//�Ѿ����鵽λ���Զ�����
//			{
//				if(ABS(PidBodanMotorPos.SetPoint-Bodan_Pos)<5000)
//				{
//					MirocPosition = k_onegrid * Onegrid;
//					SendToTx2BullectCnt++;
//				}
//			}
//    }
//	}
//	
//	if(Status.GimbalMode == Gimbal_Buff_Mode)
//	{
//		if(F105.IsShootAble==1 && armor_state == ARMOR_AIMED)
//  	{
//			if(ABS(PC_Receive.RCPitch - Gimbal.Pitch.MotorTransAngle)<1.5f && ABS(PC_Receive.RCYaw - Gimbal.Yaw.MotorTransAngle)<1.5f)	//�Ѿ����鵽λ���Զ�����
//			{
//				if(ABS(PidBodanMotorPos.SetPoint-Bodan_Pos)<8000)
//				{
//					MirocPosition = k_onegrid * Onegrid;
//					SendToTx2BullectCnt++;
//				}
//			}
//    }
//	}	
//����
//		if(armor_state == ARMOR_AIMED)
//  	{
//			if(ABS(PC_Receive.RCPitch - Gimbal.Pitch.MotorTransAngle)<1.5f && ABS(PC_Receive.RCYaw - Gimbal.Yaw.MotorTransAngle)<1.5f)	//�Ѿ����鵽λ���Զ�����
//			{
//				if(ABS(PidBodanMotorPos.SetPoint-Bodan_Pos)<8000)
//				{
//					MirocPosition = k_onegrid * Onegrid;
//					SendToTx2BullectCnt++;
//				}
//			}
//    }
//	}
	/*�����Ʒ���ʱ����*/
//	if(Status.GimbalMode == Gimbal_Buff_Mode)
//	{
//		if(F105.IsShootAble==1)
//  	{
//			if(ABS(PC_Receive.RCPitch - Gimbal.Pitch.MotorTransAngle)<0.8f && ABS(PC_Receive.RCYaw - Gimbal.Yaw.Gyro)<0.8f)	//���ҵ�Ŀ�겢����һ����Χ��
//			{
//				if(ABS(PidBodanMotorPos.SetPoint-Bodan_Pos)<8000)
//				{
//					MirocPosition = k_onegrid * Onegrid;
//					SendToTx2BullectCnt++;
//				}
//			}
//    }
//	}
	
//	/*���Ʒ���ʱ����*/
//	if(Status.GimbalMode == Gimbal_Buff_Mode)
//	{
//		if(idata.IsShootAble==1)
//  	{
//			if(PC_Receive.ReceiveFromTx2BullectCnt>SendToTx2BullectCnt)
//			{
//				  if(idata.IsShootAble==1)
//					{
//						if(DaFuBodanTick %300 == 0)
//						{
//							DaFuBodanTick =0;
//							if(ABS(PidBodanMotorPos.SetPoint- ZeroCheck_BodanPosOutPut())<8000)
//							{
//					      MirocPosition = Onegrid;
//					      SendToTx2BullectCnt++;
//							}
//						}
//					}
//					DaFuBodanTick++;
//			}
//			else
//				DaFuBodanTick = 0;
//    }
//	}
	
//	if(F105.IsShootAble==1)
//		PidBodanMotorPos.SetPoint=PidBodanMotorPos.SetPoint+MirocPosition;  
//		MirocPosition = 0;
//}

/**********************************************************************************************************
*�� �� ��: BodanMotor_CurrentPid_Cal
*����˵��: �������λ�û�˫���������
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
short Last_GimbalState;
void BodanMotor_CurrentPid_Cal(void)
{
	//�����һ�θ���ģʽ�ı�־λ
	if((Last_GimbalState == Gimbal_Armor_Mode ||Last_GimbalState == Gimbal_BigBuf_Mode ||Last_GimbalState == Gimbal_SmlBuf_Mode) && Last_GimbalState != Status.GimbalMode)
	{
	  armor_state=ARMOR_NO_AIM;
	}
	Last_GimbalState = Status.GimbalMode;
	
	
	switch(Status.ShootMode)//���ģʽѡ��
	{
		case Shoot_Check_Mode:
			Shoot_Check_Cal();
			break;

		case Shoot_Fire_Mode:
			Shoot_Fire_Cal();
			break;

		case Shoot_Tx2_Mode:
			Shoot_Test_Cal();			//����
//			Shoot_Tx2_Cal();
			break;

		case Shoot_Powerdown_Mode:
			Shoot_Powerdown_Cal();
			break;

		default:
			break;
	}
	if(Shoot_Init_flag == 0)
	{
  	Shoot_Powerdown_Cal();
		delay_time=-2000;
	}
  PidBodanMotorSpeed.ActualValue = BodanReceive.RealSpeed;
	BodanMotorCurrent = (short)PID_Calc(&PidBodanMotorSpeed);
if(Status.ShootMode==Shoot_Powerdown_Mode)
{
  BodanMotorCurrent =0.0f;
}
}

/**********************************************************************************************************
*�� �� ��: Pid_BodanMotor
*����˵��: �������λ���ٶȻ�pid������ʼ��
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void Pid_BodanMotor_Init(void)
{
	
	PidBodanMotorPos.P=0.2f;  //0.4f
	PidBodanMotorPos.I=0.02f;
	PidBodanMotorPos.D=0.0f;
	PidBodanMotorPos.IMax=1500.0f;
	PidBodanMotorPos.SetPoint=0.0f;
	PidBodanMotorPos.OutMax=20000.0f;
	PidBodanMotorPos.RC_DF = 0.5F;
	PidBodanMotorPos.I_L = 8000;
	PidBodanMotorPos.I_U = 12000;
	

	PidBodanMotorSpeed.P=20.0f;  //5.0f
	PidBodanMotorSpeed.I=0.0f;//0.01f;
	PidBodanMotorSpeed.D=0.0f;
	PidBodanMotorSpeed.DeadZone=50.0f;
	PidBodanMotorSpeed.IMax=1000.0f;
	PidBodanMotorSpeed.SetPoint=0.0f;
	PidBodanMotorSpeed.OutMax = 10000.0f;
	PidBodanMotorSpeed.I_L = 100;
	PidBodanMotorSpeed.I_U = 200;
	PidBodanMotorSpeed.RC_DF = 0.5F;
	
#if(Robot_ID == 3 || Robot_ID == 4 || Robot_ID == 14)

    Onegrid=36864.0f;		  //�ϲ���
	  checkPullerSpeed = 3000;
	
#else
	
	  Onegrid=36864.0f;		  //�²���
		checkPullerSpeed = 1000;
#endif
}

/**********************************************************************************************************
*�� �� ��: Pid_Friction_Init
*����˵��: �������λ���ٶȻ�pid������ʼ��
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void Pid_Friction_Init(void)
{

#if Robot_ID == 3
/********************************************* 3�ų� *******************************************************/	
			Infantry.Low_FrictionSpeed = 4900;
			Infantry.Medium_FrictionSpeed = 5800;
			Infantry.High_FrictionSpeed =10000;
	
#elif  Robot_ID == 4
/********************************************* 4�ų� *******************************************************/	

			Infantry.Low_FrictionSpeed = 4800;    //4850:14.1  ��Ƶ��4.5
 			Infantry.Medium_FrictionSpeed = 5650;  //17.4
			Infantry.High_FrictionSpeed =10300;

#elif  Robot_ID == 14
/********************************************* 14�ų� *******************************************************/	
			Infantry.Low_FrictionSpeed = 5000;
			Infantry.Medium_FrictionSpeed = 5800;
			Infantry.High_FrictionSpeed = 8000;
	
#elif  Robot_ID == 5
/********************************************* 5�ų� *******************************************************/	

			Infantry.Low_FrictionSpeed = 4900;
			Infantry.Medium_FrictionSpeed = 5750;
			Infantry.High_FrictionSpeed = 11000;
		
/********************************************* ȱʡֵ ******************************************************/		
#else
			Infantry.Low_FrictionSpeed = 4850;
			Infantry.Medium_FrictionSpeed = 5800;
			Infantry.High_FrictionSpeed = 16000;
#endif

  PidFrictionSpeed[0].P=80.0f;
	PidFrictionSpeed[0].I=0.0f;
	PidFrictionSpeed[0].D=0.0f;
	PidFrictionSpeed[0].IMax=1500.0f;
	PidFrictionSpeed[0].SetPoint=0.0f;
	PidFrictionSpeed[0].OutMax = 9900.0f;
	PidFrictionSpeed[0].RC_DF = 0.05F;
	PidFrictionSpeed[0].I_L = 50;
	PidFrictionSpeed[0].I_U =100;
	
	
  PidFrictionSpeed[1].P=80.0f;
	PidFrictionSpeed[1].I=0.0f;
	PidFrictionSpeed[1].D=0.0f;
	PidFrictionSpeed[1].IMax=1500.0f;
	PidFrictionSpeed[1].SetPoint=0.0f;
	PidFrictionSpeed[1].OutMax = 9900.0f;
	PidFrictionSpeed[1].RC_DF = 0.05F;
	PidFrictionSpeed[1].I_L = 50;
	PidFrictionSpeed[1].I_U =100;
	
}


/**********************************************************************************************************
*�� �� ��: Shoot_task
*����˵��: ��������
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
#define POWER_ON 1
#define POWER_OFF 0
uint32_t Shoot_high_water;
int Shoot_Power = POWER_ON;		//��ȡ���������ѹ
extern short FrictionCurrent[2];
void Shoot_task(void *pvParameters)
{
  portTickType xLastWakeTime;
	const portTickType xFrequency = 1;
  while (1) {
	  xLastWakeTime = xTaskGetTickCount();
		 if(Shoot_Power == POWER_ON)
		 {

			if(Status.ShootMode == Shoot_Powerdown_Mode)
			{
				F405.Fric_Flag=0;
				FrictionWheel_speed=0;
				Shoot_Init_flag = 0;				//��ֹ����ת
			}else		//�Ѿ���ʼ���ɹ�һ���ˣ������л��˷ǵ���ģʽ
			{
			F405.Fric_Flag=1;
			FrictionSpeedChoose();
			Shoot_Init_flag = 1;
			}
    		FrictionWheel_Set(FrictionWheel_speed);
		 }
		 else if(Shoot_Power == POWER_OFF)
		 {
				Shoot_Init_flag = 0;				//��ֹ����ת����
		 }
		 
		 Pluck_Speed_Choose();   //��Ƶѡ��
		 
		 sum = PidFrictionSpeed[0].ActualValue + PidFrictionSpeed[1].ActualValue;
		 
		 BodanMotor_CurrentPid_Cal();
		 FrictionBodanCan2Send(FrictionCurrent[0],FrictionCurrent[1],BodanMotorCurrent);
		 
		 IWDG_Feed();
     vTaskDelayUntil(&xLastWakeTime,xFrequency); 
	
#if  INCLUDE_uxTaskGetStackHighWaterMark
        Shoot_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}
