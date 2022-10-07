/**********************************************************************************************************
 * @文件     main.c
 * @说明     主文件
 * @版本  	 V1.0
 * @作者     陈志鹏
 * @日期     2021.4
**********************************************************************************************************/
/**********************************************************************************************************
 * @文件     main.c
 * @说明     主文件
 * @版本  	 V2.0
 * @作者     戴军
 * @日期     2022.5
**********************************************************************************************************/
#include "main.h"
roboDisconnect Robot_Disconnect;
char Judge_Lost;
extern ChassisSpeed_t chassis;

extern JudgeReceive_t JudgeReceive;
extern enum CHARGESTATE_Typedef ChargeState;
extern TaskHandle_t ChassisTask_Handler; //任务句柄
extern TaskHandle_t PowerControlTask_Handler; //任务句柄
/**********************************************************************************************************
*函 数 名: main
*功能说明: 主函数
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
int main()
{
  BSP_Init();
	Robot_Init();
	
	startTast();
	vTaskStartScheduler();
	while(1)
	{
		
	}
}

/**********************************************************************************************************
*函 数 名: System_Config
*功能说明: 系统初始化
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void BSP_Init(void)
{
	while(SysTick_Config(168000));	
	ADC_Configuration();//ADC初始化要放在串口之前，不然串口不能接收数据
	DAC1_Init();
	CAN1_Configuration();
	CAN2_Configuration();
	VOFA_USART_Configuration();
	UART4_Configuration();
	TIM2_Configuration();
	TIM4_Configuration();
	IWDG_Config(IWDG_Prescaler_64 ,625);
	i2c_init();
	ChargeIO_Configuration();
	Charge_Off;
	ChargeState = ChargeOff;
	delay_ms(300);//等主控板初始化完成，防止主控板初始化过程中向底盘发送错误数据
}
/**********************************************************************************************************
*函 数 名: System_Init
*功能说明: 系统参数初始化
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void Robot_Init(void)
{
	Pid_ChargeCal_Init();
	Pid_ChassisWheelInit();
	Chassis_Power_Control_Init();		//这个和pid的顺序不能调换
}
/**********************************************************************************************************
*函 数 名: Offline_Check_task
*功能说明: 掉线检测任务(任务优先级太低，在这里放串口通信会导致数据发不出去)
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void Judge_Rst()
{
 JudgeReceive.MaxPower=60;
 JudgeReceive.remainEnergy=40;
}
/**********************************************************************************************************
*函 数 名: Offline_Check_task
*功能说明: 掉线检测任务(任务优先级太低，在这里放串口通信会导致数据发不出去)
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
uint32_t Offline_Check_high_water;
extern ext_student_interactive_header_data_t custom_grapic_draw;
extern uint8_t seq;
void Offline_Check_task(void *pvParameters)
{
	static char ChassisSuspend[4];
   while (1) {
    

		/*主控板掉线检测*/
		if(Robot_Disconnect.F405Disconnect>5)
		{
			F405_Rst();
		}
		Robot_Disconnect.F405Disconnect++;
		
		/*裁判系统掉线检测*/
		if(Robot_Disconnect.JudgeDisconnect>100)
		{
			Judge_Rst();
			Judge_Lost=1;
		}else
		{
		  Judge_Lost=0;
		}
		Robot_Disconnect.JudgeDisconnect++;
		
			/*底盘电机掉线检测*/
		for(int i=0;i<4;i++)
	{
		if(ABS(Robot_Disconnect.ChassisDisconnect[i])>100)
		{
			ChassisSuspend[i]=1;
		}else
		{
			if(ChassisSuspend[i])
			{
			ChassisSuspend[i]=0;
			break;
			}
		}
	  Robot_Disconnect.ChassisDisconnect[i]++;
	}	
	
				/*超级电容掉线检测*/

		if(Robot_Disconnect.SuperPowerDisconnect>100)
		{
		
		}
	  Robot_Disconnect.SuperPowerDisconnect++;	
		
		
		IWDG_Feed();//喂狗

		vTaskDelay(1); 
		 
#if INCLUDE_uxTaskGetStackHighWaterMark
        Offline_Check_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}


