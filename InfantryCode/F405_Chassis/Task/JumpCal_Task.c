/**********************************************************************************************************
 * @�ļ�     JumpCalTask.c
 * @˵��     ����ʱ�õ��̹��ʼ�������������
 * @�汾  	 V1.0
 * @����     ����
 * @����     2022.5.17
**********************************************************************************************************/
#include "main.h"
extern F405_typedef F405;
extern JudgeReceive_t JudgeReceive;
extern INA260 INA260_1,INA260_2;
short MyBuffering_Energy=60;
#define TEST_Power  80 
#define TEST_HP     200
short MyHP_Cal=0;
short MyHP_Lose=0;
void JumpCal_task()
{
	TickType_t xLastWakeTime;
	TickType_t SleepTime=100;   // 10Hz���һ�Σ����� 100 ms
	char  Energy_Up_Flag=0;
	float delt_P=0;
	float Kr=0;
	char N=0;
	float HPLose_Temp;
  while(1)
	{ 
		xLastWakeTime=xTaskGetTickCount();
	 if(F405.Gimbal_Flag==Gimbal_Jump_Mode)
	 {
		if(!Energy_Up_Flag) 
		{
			MyBuffering_Energy=250;
		  Energy_Up_Flag=1;
		}
	 }else
	 {
		 Energy_Up_Flag=0;
	 }
		
	if(USING_JUDGE_EQUIPMENT)  //û�в���ϵͳ��ʱ��ʹ���Լ��ĵ����Ƽ��㻺������
	{
		delt_P=JudgeReceive.realChassispower-JudgeReceive.MaxPower;
		Kr=delt_P/JudgeReceive.MaxPower;
		MyHP_Cal=JudgeReceive.maxHP-MyHP_Lose;
		HPLose_Temp=JudgeReceive.maxHP*0.1;
	}
  else	
  {
	 delt_P=INA260_1.Power/1000-TEST_Power;
	 Kr=delt_P/TEST_Power;
	 MyHP_Cal=TEST_HP-MyHP_Lose;
	 HPLose_Temp=TEST_HP*0.1;
	} 		
	
	MyBuffering_Energy=MyBuffering_Energy-delt_P*0.1;  //���㻺������
	
	if(MyBuffering_Energy<=0)  //���㳬���ʿ�Ѫ
	{
	 MyBuffering_Energy=0;
	 if(Kr<=0.1)
	 {
	  N=10;
	 }
	 else if(Kr<=0.2)
	 {
	  N=20;
	 }
	 else 
	 {
	  N=40;
	 }
	 MyHP_Lose=MyHP_Lose+HPLose_Temp*N/100;
	 
	}
	else
	{
		if(MyBuffering_Energy>MAX_BUFFERING_ENERGY)
		{
		MyBuffering_Energy=MAX_BUFFERING_ENERGY;
		}
	}
		vTaskDelayUntil(&xLastWakeTime,SleepTime);
	}
}