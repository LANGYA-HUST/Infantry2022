/**********************************************************************************************************
 * @�ļ�     main.c
 * @˵��     ���ļ�
 * @�汾  	 V1.0
 * @����     ��־��
 * @����     2021.4
**********************************************************************************************************/
/**********************************************************************************************************
 * @�ļ�     main.c
 * @˵��     ���ļ�
 * @�汾  	 V2.0
 * @����     ����
 * @����     2022.5
**********************************************************************************************************/
#include "main.h"
roboDisconnect Robot_Disconnect;
char Judge_Lost;
extern ChassisSpeed_t chassis;

extern JudgeReceive_t JudgeReceive;
extern enum CHARGESTATE_Typedef ChargeState;
extern TaskHandle_t ChassisTask_Handler; //������
extern TaskHandle_t PowerControlTask_Handler; //������
/**********************************************************************************************************
*�� �� ��: main
*����˵��: ������
*��    ��: ��
*�� �� ֵ: ��
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
*�� �� ��: System_Config
*����˵��: ϵͳ��ʼ��
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void BSP_Init(void)
{
	while(SysTick_Config(168000));	
	ADC_Configuration();//ADC��ʼ��Ҫ���ڴ���֮ǰ����Ȼ���ڲ��ܽ�������
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
	delay_ms(300);//�����ذ��ʼ����ɣ���ֹ���ذ��ʼ������������̷��ʹ�������
}
/**********************************************************************************************************
*�� �� ��: System_Init
*����˵��: ϵͳ������ʼ��
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void Robot_Init(void)
{
	Pid_ChargeCal_Init();
	Pid_ChassisWheelInit();
	Chassis_Power_Control_Init();		//�����pid��˳���ܵ���
}
/**********************************************************************************************************
*�� �� ��: Offline_Check_task
*����˵��: ���߼������(�������ȼ�̫�ͣ�������Ŵ���ͨ�Żᵼ�����ݷ�����ȥ)
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void Judge_Rst()
{
 JudgeReceive.MaxPower=60;
 JudgeReceive.remainEnergy=40;
}
/**********************************************************************************************************
*�� �� ��: Offline_Check_task
*����˵��: ���߼������(�������ȼ�̫�ͣ�������Ŵ���ͨ�Żᵼ�����ݷ�����ȥ)
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
uint32_t Offline_Check_high_water;
extern ext_student_interactive_header_data_t custom_grapic_draw;
extern uint8_t seq;
void Offline_Check_task(void *pvParameters)
{
	static char ChassisSuspend[4];
   while (1) {
    

		/*���ذ���߼��*/
		if(Robot_Disconnect.F405Disconnect>5)
		{
			F405_Rst();
		}
		Robot_Disconnect.F405Disconnect++;
		
		/*����ϵͳ���߼��*/
		if(Robot_Disconnect.JudgeDisconnect>100)
		{
			Judge_Rst();
			Judge_Lost=1;
		}else
		{
		  Judge_Lost=0;
		}
		Robot_Disconnect.JudgeDisconnect++;
		
			/*���̵�����߼��*/
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
	
				/*�������ݵ��߼��*/

		if(Robot_Disconnect.SuperPowerDisconnect>100)
		{
		
		}
	  Robot_Disconnect.SuperPowerDisconnect++;	
		
		
		IWDG_Feed();//ι��

		vTaskDelay(1); 
		 
#if INCLUDE_uxTaskGetStackHighWaterMark
        Offline_Check_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}


