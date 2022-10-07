/**********************************************************************************************************
 * @�ļ�     DataReceiveTask.c
 * @˵��     ���ݽ���
 * @�汾  	 V1.0
 * @����     ��־��
 * @����     2021.4
**********************************************************************************************************/
#include "main.h"
unsigned char SaveBuffer[90];
JudgeReceive_t JudgeReceive;
extern roboDisconnect Robot_Disconnect;
short CAP_CrossoverFlag;
short CrossoverFlagMax = 10;
F405_typedef F405;
RM820RReceive_Typedef ChassisMotorCanReceive[4];
ChassisSpeed_t chassis;

extern char SelfProtect_Cross_Flag;
char slow_flag;
/**********************************************************************************************************
*�� �� ��: Can1Receive0
*����˵��: can1���պ��������յ�����ص��ٶȣ�����ֵ
*��    ��: rx_message0
*�� �� ֵ: ��
**********************************************************************************************************/
void Can1Receive0(CanRxMsg rx_message0)
{
	switch(rx_message0.StdId)
	{ 
		case 0x201:                                                          
				 ChassisMotorCanReceive[0].RealSpeed=rx_message0.Data[2]<<8 | rx_message0.Data[3];		//ת��ת��
		     ChassisMotorCanReceive[0].Current=rx_message0.Data[4]<<8 | rx_message0.Data[5];			//ʵ��ת�ص���
				 Robot_Disconnect.ChassisDisconnect[0]=0;
		 break;
		case 0x202:
				 ChassisMotorCanReceive[1].RealSpeed=rx_message0.Data[2]<<8 | rx_message0.Data[3];
		     ChassisMotorCanReceive[1].Current=rx_message0.Data[4]<<8 | rx_message0.Data[5];
		     Robot_Disconnect.ChassisDisconnect[1]=0;
		 break;
		case 0x203:
				 ChassisMotorCanReceive[2].RealSpeed=rx_message0.Data[2]<<8 | rx_message0.Data[3];
		     ChassisMotorCanReceive[2].Current=rx_message0.Data[4]<<8 | rx_message0.Data[5];
		     Robot_Disconnect.ChassisDisconnect[2]=0;
		 break;
		case 0x204:
				 ChassisMotorCanReceive[3].RealSpeed=rx_message0.Data[2]<<8 | rx_message0.Data[3];
		     ChassisMotorCanReceive[3].Current=rx_message0.Data[4]<<8 | rx_message0.Data[5];
		     Robot_Disconnect.ChassisDisconnect[3]=0;
		 break; 
	}
}


/**********************************************************************************************************
*�� �� ��: Can2Receive0
*����˵��: can2���պ���������F405�ṹ��
*��    ��: rx_message1
*�� �� ֵ: ��
**********************************************************************************************************/
//void Can2Receive0(CanRxMsg *rx_message)
//{
//	if(rx_message->StdId == 0x102)
//	{
//		 memcpy(&F405.SuperPowerLimit, &rx_message->Data[0], 2);
//	}
//}

/**********************************************************************************************************
*�� �� ��: Can2Receive1
*����˵��: can2���պ���, �����ϲ�崫�ص�xyw���ٶ�
*��    ��: rx_message0
*�� �� ֵ: ��
**********************************************************************************************************/
extern char Chassis_Run_Flag;
void Can2Receive1(CanRxMsg *rx_message)
{
	switch(rx_message->StdId)
	{
		case 0x101:	
			memcpy(&chassis.carSpeedx, &rx_message->Data[0], 2);
			memcpy(&chassis.carSpeedy, &rx_message->Data[2], 2);
			memcpy(&chassis.carSpeedw, &rx_message->Data[4], 2);
		
		if((ABS(chassis.carSpeedx) < 100) && ABS(chassis.carSpeedy)<100 && ABS (chassis.carSpeedw)<2000) //ǰ����ɲ�������ʱ
		{
		  Chassis_Run_Flag = 0;
		}
		else
		{
		  Chassis_Run_Flag = 1;
		}
			Robot_Disconnect.F405Disconnect=0; 
		break;
		
		case 0x102:
			memcpy(&F405.SuperPowerLimit, &rx_message->Data[0], 1);
			memcpy(&F405.Chassis_Flag, &rx_message->Data[1], 1);
			memcpy(&F405.Gimbal_100, &rx_message->Data[2],2);
			memcpy(&F405.Gimbal_Flag, &rx_message->Data[4],1);
			memcpy(&F405.Send_Pack1, &rx_message->Data[5],1);
			/*pack����*/
			F405.Mag_Flag = (F405.Send_Pack1>>0)&0x01;
			F405.Laser_Flag = (F405.Send_Pack1>>1)&0x01;
			F405.Graphic_Init_Flag = (F405.Send_Pack1>>2)&0x01;
			F405.Freq_state = (F405.Send_Pack1>>3)&0x01;
		  F405.Fric_Flag=(F405.Send_Pack1>>4)&0x01;
		  SelfProtect_Cross_Flag=(F405.Send_Pack1>>6)&0x01;
		  slow_flag = (F405.Send_Pack1>>7)&0x01;
		break;
	}
}

/**********************************************************************************************************
*�� �� ��: JudgeBuffReceive
*����˵��: ����ϵͳ���պ���
*��    ��: ReceiveBuffer[]  DataLen
*�� �� ֵ: ��
**********************************************************************************************************/
float Last_chassisPower=0;
char TickCount=0;
uint16_t receivePower;
void JudgeBuffReceive(unsigned char ReceiveBuffer[],uint16_t DataLen)
{
	uint16_t cmd_id;
	short PackPoint;
	memcpy(&SaveBuffer[JudgeBufBiggestSize],&ReceiveBuffer[0],JudgeBufBiggestSize);		//��ReceiveBuffer[0]��ַ������SaveBuffer[24], ���ο���24��, ����һ�ν��յĴ浽�����
	for(PackPoint=0;PackPoint<JudgeBufBiggestSize;PackPoint++)		//�ȴ���ǰ�������(����һ�����ѽ������)
	{
		if(SaveBuffer[PackPoint]==0xA5) 
		{	
			if((Verify_CRC8_Check_Sum(&SaveBuffer[PackPoint],5)==1))		//frame_header ��λ����У��
			{
				cmd_id=(SaveBuffer[PackPoint+6])&0xff;
				cmd_id=(cmd_id<<8)|SaveBuffer[PackPoint+5];  
				DataLen=SaveBuffer[PackPoint+2]&0xff;
				DataLen=(DataLen<<8)|SaveBuffer[PackPoint+1];
				
				//������״̬����
				if((cmd_id==0x0201)&&(Verify_CRC16_Check_Sum(&SaveBuffer[PackPoint],DataLen+9))) 
				{
					memcpy(&JudgeReceive.robot_id,&SaveBuffer[PackPoint+7+0],1);
					memcpy(&JudgeReceive.RobotLevel,&SaveBuffer[PackPoint+7+1],1);
					memcpy(&JudgeReceive.remainHP,&SaveBuffer[PackPoint+7+2],2);
					memcpy(&JudgeReceive.maxHP,&SaveBuffer[PackPoint+7+4],2);
					memcpy(&JudgeReceive.HeatCool17,&SaveBuffer[PackPoint+7+6],2);
					memcpy(&JudgeReceive.HeatMax17,&SaveBuffer[PackPoint+7+8],2);
					memcpy(&JudgeReceive.BulletSpeedMax17,&SaveBuffer[PackPoint+7+10],2);
					memcpy(&JudgeReceive.MaxPower,&SaveBuffer[PackPoint+7+24],2);
					if(JudgeReceive.MaxPower == 0)
						JudgeReceive.MaxPower = 60 ;
				}
					
				//ʵʱ���ʡ���������
				if((cmd_id==0x0202)&&(Verify_CRC16_Check_Sum(&SaveBuffer[PackPoint],DataLen+9)))
				{
					memcpy(&JudgeReceive.realChassisOutV,&SaveBuffer[PackPoint+7+0],2);
					memcpy(&JudgeReceive.realChassisOutA,&SaveBuffer[PackPoint+7+2],2);
					memcpy(&JudgeReceive.realChassispower,&SaveBuffer[PackPoint+7+4],4);
					memcpy(&JudgeReceive.remainEnergy,&SaveBuffer[PackPoint+7+8],2);
					memcpy(&JudgeReceive.shooterHeat17,&SaveBuffer[PackPoint+7+10],2);                              // 2���ֽ�
					Can2Send1();
					Last_chassisPower=JudgeReceive.realChassispower;
				}
				
				//ʵʱ��������
				if((cmd_id==0x0204)&&(Verify_CRC16_Check_Sum(&SaveBuffer[PackPoint],DataLen+9)))
				{
					Can2Send2(SaveBuffer[PackPoint+7+0]);
				}
				
				//ʵʱ�����Ϣ
					if((cmd_id==0x0207)&&(Verify_CRC16_Check_Sum(&SaveBuffer[PackPoint],DataLen+9)))
				{
					memcpy(&JudgeReceive.bulletFreq, &SaveBuffer[PackPoint+7+2],1);
					memcpy(&JudgeReceive.bulletSpeed,&SaveBuffer[PackPoint+7+3],4);
					JudgeReceive.ShootCpltFlag = 1;
				}
				
			}
		}
	Robot_Disconnect.JudgeDisconnect =0;
	memcpy(&SaveBuffer[0],&SaveBuffer[JudgeBufBiggestSize],JudgeBufBiggestSize);		//��SaveBuffer[24]��ַ������SaveBuffer[0], ���ο���24������֮ǰ�浽����������ᵽǰ�棬׼������
	}
}

/**********************************************************************************************************
*�� �� ��: F405_Rst
*����˵��: ���ذ����ִ�к���
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void F405_Rst(void)
{
	chassis.carSpeedx = 0;
	chassis.carSpeedy = 0;
	chassis.carSpeedw = 0;
}

/**********************************************************************************************************
*�� �� ��: JudgeReceive_task
*����˵��: ���ݽ�������
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
extern TaskHandle_t JudgeReceiveTask_Handler; //������
extern uint8_t JudgeReveice_Flag;
extern unsigned char JudgeReceiveBuffer[JudgeBufBiggestSize];

void JudgeReceive_task()
{
			while(1)
		{
	 		ulTaskNotifyTake( pdTRUE , portMAX_DELAY );  //����֪ͨ���£��򲻻���,֪ͨʵ�ֶ�ֵ�ź���
		
/********************************* PC���ݴ��� *******************************************************/		
		 JudgeReveice_Flag = 0;
     JudgeBuffReceive(JudgeReceiveBuffer,0);
		}
 
}

