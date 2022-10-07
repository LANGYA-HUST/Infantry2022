/**********************************************************************************************************
 * @�ļ�     PowerControlTask.c
 * @˵��     ���ʿ���
 * @�汾  	 V1.0
 * @����     ��־��
 * @����     2020.1
**********************************************************************************************************/
#include "main.h"
float UCAP,UDCDC;
unsigned int SUM_UCAP,SUM_UDCDC,SUM_TEMP;
float AD_actual_value;		//����ʵ�ʵ�ѹ
float temperate,temp3v3;
float I_Set = 0;
Pid_Typedef ChargeCtl;
enum POWERSTATE_Typedef PowerState = BAT;
enum CHARGESTATE_Typedef ChargeState = ChargeOff;

extern unsigned short ADC_ConvertedValue[90];
extern INA260 INA260_1,INA260_2;
extern F405_typedef F405;
extern JudgeReceive_t JudgeReceive;
extern ChassisSpeed_t chassis;
extern F105_Typedef F105;
extern int P_State;
extern short CAP_CrossoverFlag;
extern roboDisconnect Robot_Disconnect;
/**********************************************************************************************************
*�� �� ��: ChargeIO_Configuration
*����˵��: ��ŵ�IO�ڳ�ʼ��
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void ChargeIO_Configuration(void)
{
	GPIO_InitTypeDef gpio_init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOC,ENABLE);
	/**********************���ʹ��***********************************/
	gpio_init.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	gpio_init.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_init.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&gpio_init);	
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);		//��ʼ��ʱ�����
	GPIO_SetBits(GPIOA,GPIO_Pin_7);	//����Diode Mode
	/***********************�ŵ�ѡ��ʹ��***********************/
	gpio_init.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5;
	gpio_init.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_init.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOC,&gpio_init);	
	Bat_on;
	CAP_off;
}


/**********************************************************************************************************
*�� �� ��: ADC_Filter
*����˵��: ����ת��(�õ�����ʵ�ʵ�ѹ)
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void ADC_Filter(void)
{
	int i;
	SUM_UCAP = 0;
	SUM_UDCDC = 0;
	SUM_TEMP = 0;
	for(i=0;i<90;i++)
	{ 
		if(i%3 == 0)
			SUM_UCAP += ADC_ConvertedValue[i];
		else if(i%3 == 1)
			SUM_UDCDC += ADC_ConvertedValue[i];
		else
			SUM_TEMP +=  ADC_ConvertedValue[i];
	}
	UCAP = ((SUM_UCAP/30*825)>>10)/1000.0f;
	UDCDC = ((SUM_UDCDC/30*825)>>10)/1000.0f;
	float temp_temperature;
	temp3v3 = ((SUM_TEMP/30*825)>>10)/1000.0f;
	temp_temperature = temp3v3;
	temp_temperature = (1.43-temp_temperature)/0.0043+25;
	temperate = temp_temperature*=100;
	AD_actual_value = UCAP*7.8;
}



/**********************************************************************************************************
*�� �� ��: ChargeCal
*����˵��: ��ŵ�PID����
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
int MaxChargePower = 45000; 		//����繦�ʣ���С��繦��
const short MinCharegePower = 5000;
int ActualPower = 0;
float MaxBatChargeCurrent = 5.0f;
float MaxCAPChargeCurrent = 7.0f;
float My_i;
float ChargeCal()	//��ʣ�๦�ʼ��������
{
	float i;
	MaxChargePower = 1000 * (JudgeReceive.MaxPower-5) ;
	if(PowerState == BAT)   //0 ʹ�õ�طŵ� ��ʱ��ظ����ݳ�磬��ظ����̹���
	{
			ChargeCtl.SetPoint = LIMIT_MAX_MIN(MaxChargePower - INA260_2.Power,MaxChargePower,0);
		//ʵ�ʣ����ݳ�繦��  SetPoint: ϣ�����Ĺ��� ��λmW
	}
	else 
	{
				ChargeCtl.SetPoint = MaxChargePower;
	}
  	ActualPower = INA260_1.Power - INA260_2.Power;
		i = PID_Calc(&ChargeCtl,ActualPower)/1000;
	return i;
}
/**********************************************************************************************************
*�� �� ��: HalfCAPCal
*����˵��: ��������
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void HalfCAPCal()
{
	if(AD_actual_value>17.0f)  
		{
			PowerState = HalfCAP;
			Bat_off;
			CAP_on;
		}			
	else  
		{
			PowerState = BAT;  					//���ݷŵ�
			Bat_on;
			CAP_off;
		}

}

//float AvailableChargePower;
//float ChargePowerMargin = 5;
//float ActualChargePower;
//float ChargeCurrent;
//float ChargeCurrentMax = 4.0f;
//short MinCharegePower = 5;

////���ػ����������ӵĳ�����

//float ChargeCal(void)
//{
//	float addition_current;
//	AvailableChargePower = JudgeReceive.MaxPower - JudgeReceive.realChassispower - ChargePowerMargin;
//	if(AvailableChargePower < -10) //������5W
//	{
//		I_Set = 0;
//		addition_current = 0;
//	}
//	else
//	{
//		ChargeCurrentMax = AvailableChargePower / AD_actual_value;
//		if(AD_actual_value < 22)
//			ChargeCtl.SetPoint = AvailableChargePower;
//		else
//			ChargeCtl.SetPoint = MinCharegePower;
//		if(PowerState == BAT)
//		{
//			ActualChargePower = (INA260_1.Power - INA260_2.Power) / 1000.0f;				//ͳһ��WΪ��λ
//		}
//		else if(PowerState == CAP)
//		{
//			ActualChargePower = INA260_1.Power / 1000.0f;
//		}
//		
//		addition_current = LIMIT_MAX_MIN(PID_Calc(&ChargeCtl,ActualChargePower),ChargeCurrentMax,0);
//	}
//	return addition_current;
//}

/**********************************************************************************************************
*�� �� ��: Charge_Set
*����˵��: ����������
*��    ��: I_Set
*�� �� ֵ: ��
**********************************************************************************************************/
float last_i_set=0.0f;
float I_Set_Jump;
void Charge_Set(float i_Set)
{
	short DAC_Set;
	//����б�º�������
//	if(fabs(i_Set-last_i_set)>I_Set_Jump)
//	{
//	i_Set=(i_Set-last_i_set)*0.2+last_i_set;
//	}
//	
	DAC_Set = i_Set/10.0*2/3.3*4096;    
	DAC_Set = LIMIT_MAX_MIN(DAC_Set,4096,0);
	DAC_SetChannel2Data(DAC_Align_12b_R,DAC_Set);
	last_i_set=i_Set;
}


/**********************************************************************************************************
*�� �� ��: PidInit
*����˵��: ���PID��ʼ��
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void Pid_ChargeCal_Init()
{
	ChargeCtl.SetPoint = 40000;
	ChargeCtl.P = 0.5;
	ChargeCtl.I = 0;
	ChargeCtl.IMax = 0;
	ChargeCtl.OutMax = MaxCAPChargeCurrent*1000;
}

/**********************************************************************************************************
*�� �� ��: ChargeControl
*����˵��: �������ݿ���
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/

float AD_L = 10.0f, AD_H = 14.0f;
float test_current;
int t;
void ChargeControl(void)
{
	/******************������*******************/
   I_Set = ChargeCal(); 
	if(PowerState == BAT)  //bat
	{
		if(CAP_CrossoverFlag == 1)			//����/ֹͣ�����ת���д������ֱ�ӹرյ��ݳ��
		{
			t++;
			if(t>500)
			{
			CAP_CrossoverFlag = 0;
			t=0;
			}
			I_Set = 0;
		}
		
	I_Set = LIMIT_MAX_MIN(I_Set,MaxBatChargeCurrent,0);  //ʣ�໺������Ϊ20J��������ܳ���200W����9A���ң�ȡ7A
	}
	else
	{
	I_Set = LIMIT_MAX_MIN(I_Set,MaxCAPChargeCurrent,0);  //ʣ�໺������Ϊ20J��������ܳ���200W����9A���ң�ȡ7A
	}
	Charge_Set(I_Set);
	/******************�ŵ����*******************/
	if(F405.SuperPowerLimit == 0)			//�����ֹرճ�������
	{
		if(PowerState == CAP)		//�մӵ����л�
		{
			if(JudgeReceive.remainEnergy > 40)		//�����������ò��л�
				{
					CAP_off;   
					Bat_on;
					PowerState = BAT;
				}
		}
	}

//AD_L AD_H�в�ֵ���տ�������ʱ�����ڵ��������裬˲�����4Vѹ��
	else if(F405.SuperPowerLimit > 0)				//�����ֿ�����������
	{
		if(AD_actual_value<AD_L)  	//����ʵ�ʵ�ѹ<10V
		{
			PowerState = BAT;						//��طŵ�
			CAP_off;   
			Bat_on;
		}
		else if(AD_actual_value>AD_H)  
		{
			PowerState = CAP;  					//���ݷŵ�
			Bat_off;
			CAP_on;
		}
	}
}

/**********************************************************************************************************
*�� �� ��: PowerControl_task
*����˵��: ��������
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
uint32_t PowerControl_high_water;
void PowerControl_task(void *pvParameters)
{
   while (1) {
			/******************���ݻ�ȡ*******************/
			INA_READ_Vol();			
			INA_READ_Current();
			INA_READ_Power();
			ADC_Filter();
		 if(AD_actual_value>3)
		 {
		 Robot_Disconnect.SuperPowerDisconnect=0;
		 }
			/*****************�������ݿ���****************/
	//	 HalfCAPCal();
		 ChargeControl();
	 
	   IWDG_Feed();
     vTaskDelay(1); 
		 
#if INCLUDE_uxTaskGetStackHighWaterMark
        PowerControl_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}


