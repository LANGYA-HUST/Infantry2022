/**********************************************************************************************************
 * @文件     DataReceiveTask.c
 * @说明     数据接收
 * @版本  	 V1.0
 * @作者     陈志鹏
 * @日期     2021.4
**********************************************************************************************************/
#include "main.h"
unsigned char SaveBuffer[90];
JudgeReceive_t JudgeReceive;
extern roboDisconnect Robot_Disconnect;
//short BAT_StartFlag;
//short BAT_StopFlag;
short CAP_CrossoverFlag;
short CrossoverFlagMax = 10;
F405_typedef F405;
RM820RReceive_Typedef ChassisMotorCanReceive[4];
ChassisSpeed_t chassis;
extern char Robot_ID;
extern char SelfProtect_Cross_Flag;
char slow_flag;
/**********************************************************************************************************
*函 数 名: Can1Receive0
*功能说明: can1接收函数，接收电调传回的速度，电流值
*形    参: rx_message0
*返 回 值: 无
**********************************************************************************************************/
void Can1Receive0(CanRxMsg rx_message0)
{
	switch(rx_message0.StdId)
	{ 
		case 0x201:                                                          
				 ChassisMotorCanReceive[0].RealSpeed=rx_message0.Data[2]<<8 | rx_message0.Data[3];		//转子转速
		     ChassisMotorCanReceive[0].Current=rx_message0.Data[4]<<8 | rx_message0.Data[5];			//实际转矩电流
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
*函 数 名: Can2Receive0
*功能说明: can2接收函数，接收F405结构体
*形    参: rx_message1
*返 回 值: 无
**********************************************************************************************************/
//void Can2Receive0(CanRxMsg *rx_message)
//{
//	if(rx_message->StdId == 0x102)
//	{
//		 memcpy(&F405.SuperPowerLimit, &rx_message->Data[0], 2);
//	}
//}

/**********************************************************************************************************
*函 数 名: Can2Receive1
*功能说明: can2接收函数, 接收上层板传回的xyw向速度
*形    参: rx_message0
*返 回 值: 无
**********************************************************************************************************/
void Can2Receive1(CanRxMsg *rx_message)
{
	switch(rx_message->StdId)
	{
		case 0x101:	
			memcpy(&chassis.carSpeedx, &rx_message->Data[0], 2);
			memcpy(&chassis.carSpeedy, &rx_message->Data[2], 2);
			memcpy(&chassis.carSpeedw, &rx_message->Data[4], 2);
			
//			if(ABS(chassis.Last_carSpeedx - chassis.carSpeedx) > 1500 || ABS(chassis.Last_carSpeedy - chassis.carSpeedy) > 1500)		//发生变化
//			{
//				CAP_CrossoverFlag = 1;																										//电容停止充电
////				if(ABS(chassis.carSpeedx) < 10 || ABS(chassis.carSpeedy) < 10)						//停下
////					BAT_StopFlag = 1;
////				else if(ABS(chassis.carSpeedx) > 1500 || ABS(chassis.carSpeedy) > 1500)		//起步
////					BAT_StartFlag = 1;
//			}
//			
//			chassis.Last_carSpeedx = chassis.carSpeedx;
//			chassis.Last_carSpeedy = chassis.carSpeedy;
//			chassis.Last_carSpeedw = chassis.carSpeedw;
			
			Robot_Disconnect.F405Disconnect=0; 
		break;
		case 0x102:
			memcpy(&F405.SuperPowerLimit, &rx_message->Data[0], 1);
			memcpy(&F405.Chassis_Flag, &rx_message->Data[1], 1);
			memcpy(&F405.Gimbal_100, &rx_message->Data[2],2);
			memcpy(&F405.Gimbal_Flag, &rx_message->Data[4],1);
			memcpy(&F405.Send_Pack1, &rx_message->Data[5],1);
			/*pack解码*/
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
*函 数 名: JudgeBuffReceive
*功能说明: 裁判系统接收函数
*形    参: ReceiveBuffer[]  DataLen
*返 回 值: 无
**********************************************************************************************************/
//volatile float Last_chassisPower=0;
//volatile TickType_t SampleTick=0,SampleCount=0,Last_SampleTick=0,delt_Tick=0,Less60=0,Less100=0,From100_200=0,More200=0;
//volatile char TickCount=0;
float Last_chassisPower=0;
TickType_t SampleTick=0,SampleCount=0,Last_SampleTick=0,delt_Tick=0,Less60=0,Less100=0,From100_200=0,More200=0;
char TickCount=0;
uint16_t receivePower;
void JudgeBuffReceive(unsigned char ReceiveBuffer[],uint16_t DataLen)
{
	uint16_t cmd_id;
	short PackPoint;
	memcpy(&SaveBuffer[JudgeBufBiggestSize],&ReceiveBuffer[0],JudgeBufBiggestSize);		//把ReceiveBuffer[0]地址拷贝到SaveBuffer[24], 依次拷贝24个, 把这一次接收的存到数组后方
	for(PackPoint=0;PackPoint<JudgeBufBiggestSize;PackPoint++)		//先处理前半段数据(在上一周期已接收完成)
	{
		if(SaveBuffer[PackPoint]==0xA5) 
		{	
			if((Verify_CRC8_Check_Sum(&SaveBuffer[PackPoint],5)==1))		//frame_header 五位数据校验
			{
				cmd_id=(SaveBuffer[PackPoint+6])&0xff;
				cmd_id=(cmd_id<<8)|SaveBuffer[PackPoint+5];  
				DataLen=SaveBuffer[PackPoint+2]&0xff;
				DataLen=(DataLen<<8)|SaveBuffer[PackPoint+1];
				
				//机器人状态数据
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
				  
				}
					
				//实时功率、热量数据
				if((cmd_id==0x0202)&&(Verify_CRC16_Check_Sum(&SaveBuffer[PackPoint],DataLen+9)))
				{
					memcpy(&JudgeReceive.realChassisOutV,&SaveBuffer[PackPoint+7+0],2);
					memcpy(&JudgeReceive.realChassisOutA,&SaveBuffer[PackPoint+7+2],2);
					memcpy(&JudgeReceive.realChassispower,&SaveBuffer[PackPoint+7+4],4);
					memcpy(&JudgeReceive.remainEnergy,&SaveBuffer[PackPoint+7+8],2);
					memcpy(&JudgeReceive.shooterHeat17,&SaveBuffer[PackPoint+7+10],2);                              // 2个字节
					Can2Send1();
					
					TickCount++;
					if(TickCount==5)
					{
					TickCount=0;
					}
					if(JudgeReceive.realChassispower!=Last_chassisPower)
					{
					 SampleTick=xTaskGetTickCount();
//					 delt_Tick=SampleTick-Last_SampleTick;
//					 SampleCount=TickCount;
//					 Last_SampleTick=SampleTick;
//					 if(delt_Tick<60)
//					 {
//					 Less60++;
//					 }
//					 else if(delt_Tick<100)
//					 {
//					 Less100++;
//					 }
//					 else if(delt_Tick<200)
//					 {
//					 From100_200++;
//					 }
//					 else 
//					 {
//					 More200++;
//					 }
					}
					Last_chassisPower=JudgeReceive.realChassispower;
				}
				
				//实时射击信息
					if((cmd_id==0x0207)&&(Verify_CRC16_Check_Sum(&SaveBuffer[PackPoint],DataLen+9)))
				{
					memcpy(&JudgeReceive.bulletFreq, &SaveBuffer[PackPoint+7+2],1);
					memcpy(&JudgeReceive.bulletSpeed,&SaveBuffer[PackPoint+7+3],4);
					JudgeReceive.ShootCpltFlag = 1;
//					Can2Send2();
				}
				
			}
		}
	Robot_Disconnect.JudgeDisconnect =0;
	memcpy(&SaveBuffer[0],&SaveBuffer[JudgeBufBiggestSize],JudgeBufBiggestSize);		//把SaveBuffer[24]地址拷贝到SaveBuffer[0], 依次拷贝24个，把之前存到后面的数据提到前面，准备处理
	}
}

/**********************************************************************************************************
*函 数 名: F405_Rst
*功能说明: 主控板掉线执行函数
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void F405_Rst(void)
{
	chassis.carSpeedx = 0;
	chassis.carSpeedy = 0;
	chassis.carSpeedw = 0;
}



