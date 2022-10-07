/**********************************************************************************************************
 * @文件     PowerControlTask.c
 * @说明     功率控制
 * @版本  	 V1.0
 * @作者     陈志鹏
 * @日期     2020.1
**********************************************************************************************************/
#include "main.h"
float UCAP,UDCDC;
unsigned int SUM_UCAP,SUM_UDCDC,SUM_TEMP;
float AD_actual_value;		//电容实际电压
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
*函 数 名: ChargeIO_Configuration
*功能说明: 充放电IO口初始化
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void ChargeIO_Configuration(void)
{
	GPIO_InitTypeDef gpio_init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOC,ENABLE);
	/**********************充电使能***********************************/
	gpio_init.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	gpio_init.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_init.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&gpio_init);	
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);		//初始化时不充电
	GPIO_SetBits(GPIOA,GPIO_Pin_7);	//拉高Diode Mode
	/***********************放电选择使能***********************/
	gpio_init.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5;
	gpio_init.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_init.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOC,&gpio_init);	
	Bat_on;
	CAP_off;
}


/**********************************************************************************************************
*函 数 名: ADC_Filter
*功能说明: 数据转换(得到电容实际电压)
*形    参: 无
*返 回 值: 无
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
*函 数 名: ChargeCal
*功能说明: 充放电PID控制
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
int MaxChargePower = 45000; 		//最大充电功率，最小充电功率
const short MinCharegePower = 5000;
int ActualPower = 0;
float MaxBatChargeCurrent = 5.0f;
float MaxCAPChargeCurrent = 7.0f;
float My_i;
float ChargeCal()	//由剩余功率计算充电电流
{
	float i;
	MaxChargePower = 1000 * (JudgeReceive.MaxPower-5) ;
	if(PowerState == BAT)   //0 使用电池放电 此时电池给电容充电，电池给底盘供电
	{
			ChargeCtl.SetPoint = LIMIT_MAX_MIN(MaxChargePower - INA260_2.Power,MaxChargePower,0);
		//实际：电容充电功率  SetPoint: 希望充电的功率 单位mW
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
*函 数 名: HalfCAPCal
*功能说明: 被动电容
*形    参: 无
*返 回 值: 无
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
			PowerState = BAT;  					//电容放电
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

////返回还可以再增加的充电电流

//float ChargeCal(void)
//{
//	float addition_current;
//	AvailableChargePower = JudgeReceive.MaxPower - JudgeReceive.realChassispower - ChargePowerMargin;
//	if(AvailableChargePower < -10) //超功率5W
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
//			ActualChargePower = (INA260_1.Power - INA260_2.Power) / 1000.0f;				//统一以W为单位
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
*函 数 名: Charge_Set
*功能说明: 充电电流设置
*形    参: I_Set
*返 回 值: 无
**********************************************************************************************************/
float last_i_set=0.0f;
float I_Set_Jump;
void Charge_Set(float i_Set)
{
	short DAC_Set;
	//加入斜坡函数处理
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
*函 数 名: PidInit
*功能说明: 充电PID初始化
*形    参: 无
*返 回 值: 无
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
*函 数 名: ChargeControl
*功能说明: 超级电容控制
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/

float AD_L = 10.0f, AD_H = 14.0f;
float test_current;
int t;
void ChargeControl(void)
{
	/******************充电控制*******************/
   I_Set = ChargeCal(); 
	if(PowerState == BAT)  //bat
	{
		if(CAP_CrossoverFlag == 1)			//启动/停止，大幅转向，有大电流，直接关闭电容充电
		{
			t++;
			if(t>500)
			{
			CAP_CrossoverFlag = 0;
			t=0;
			}
			I_Set = 0;
		}
		
	I_Set = LIMIT_MAX_MIN(I_Set,MaxBatChargeCurrent,0);  //剩余缓冲能量为20J，即最大不能超过200W，即9A左右，取7A
	}
	else
	{
	I_Set = LIMIT_MAX_MIN(I_Set,MaxCAPChargeCurrent,0);  //剩余缓冲能量为20J，即最大不能超过200W，即9A左右，取7A
	}
	Charge_Set(I_Set);
	/******************放电控制*******************/
	if(F405.SuperPowerLimit == 0)			//操作手关闭超级电容
	{
		if(PowerState == CAP)		//刚从电容切回
		{
			if(JudgeReceive.remainEnergy > 40)		//缓存能量够用才切回
				{
					CAP_off;   
					Bat_on;
					PowerState = BAT;
				}
		}
	}

//AD_L AD_H有差值：刚开启电容时，由于电容有内阻，瞬间会有4V压降
	else if(F405.SuperPowerLimit > 0)				//操作手开启超级电容
	{
		if(AD_actual_value<AD_L)  	//电容实际电压<10V
		{
			PowerState = BAT;						//电池放电
			CAP_off;   
			Bat_on;
		}
		else if(AD_actual_value>AD_H)  
		{
			PowerState = CAP;  					//电容放电
			Bat_off;
			CAP_on;
		}
	}
}

/**********************************************************************************************************
*函 数 名: PowerControl_task
*功能说明: 底盘任务
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
uint32_t PowerControl_high_water;
void PowerControl_task(void *pvParameters)
{
   while (1) {
			/******************数据获取*******************/
			INA_READ_Vol();			
			INA_READ_Current();
			INA_READ_Power();
			ADC_Filter();
		 if(AD_actual_value>3)
		 {
		 Robot_Disconnect.SuperPowerDisconnect=0;
		 }
			/*****************超级电容控制****************/
	//	 HalfCAPCal();
		 ChargeControl();
	 
	   IWDG_Feed();
     vTaskDelay(1); 
		 
#if INCLUDE_uxTaskGetStackHighWaterMark
        PowerControl_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}


