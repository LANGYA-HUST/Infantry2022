/**********************************************************************************************************
 * @文件     DataSendTask.c
 * @说明     数据发送
 * @版本  	 V1.0
 * @作者     黄志雄
 * @日期     2019.10
**********************************************************************************************************/
/**********************************************************************************************************
 * @文件     DataSendTask.c
 * @说明     数据发送
 * @版本  	 V2.0
 * @作者     戴军
 * @日期     2022.3
**********************************************************************************************************/
#include "main.h"
/*--------------内部变量-------------------*/
u8 SendToPC_Buff[PC_SENDBUF_SIZE];
F405_typedef F405;
/*--------------外部变量-------------------*/
extern Gimbal_Typedef Gimbal;
extern short PC_Sendflag;
extern Status_t Status;
extern F105_Typedef F105;
extern unsigned char magazineState;
/**********************************************************************************************************
*函 数 名: ChassisCan1Send
*功能说明: 把xyw向速度发送至B板 Can1Send0
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void ChassisCan1Send(short *carSpeedx,short *carSpeedy,short *carSpeedw)
{
	CanTxMsg tx_message;
	tx_message.IDE = CAN_ID_STD;    
	tx_message.RTR = CAN_RTR_DATA; 
	tx_message.DLC = 0x08;    
	tx_message.StdId = 0x101;
  
	memcpy(&tx_message.Data[0],carSpeedx,2);
	memcpy(&tx_message.Data[2],carSpeedy,2);
	memcpy(&tx_message.Data[4],carSpeedw,2);
	tx_message.Data[6]=(short)Gimbal.Yaw.Motor&0x1ff;
	tx_message.Data[7]=((short)Gimbal.Yaw.Motor>>8)&0x1ff;
	CAN_Transmit(CAN1,&tx_message);
}
/**********************************************************************************************************
*函 数 名: BodanCan1Send
*功能说明: 发送拨弹电机电流值 Can1Send1
*形    参: 拨弹电机电流值  射速档  超级电容功率  Rc_k_100
*返 回 值: 无
**********************************************************************************************************/
void BodanCan1Send(short a)
{
    CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = 0x1ff;
	  a=LIMIT_MAX_MIN(a,8000,-8000);
    tx_message.Data[0] = (unsigned char)((a>>8)&0xff);
    tx_message.Data[1] = (unsigned char)(a&0xff);  
	
    CAN_Transmit(CAN1,&tx_message);
}

/**********************************************************************************************************
*函 数 名: F405Can1Send
*功能说明: 与B板通信	Can1Send1
*形    参: 超级电容使用允许位  小陀螺模式标志位  大陀螺模式标志位  单挑模式标志位
*返 回 值: 无
**********************************************************************************************************/
extern short armor_state;
extern char  SelfProtect_Cross_Flag;
extern char k_slow;
extern char HighFreq_flag;
void F405Can1Send(F405_typedef *F405_Send)
{
    CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = 0x102;
		if(magazineState == 0x01) F405_Send->Mag_Flag = 1;
		else F405_Send->Mag_Flag = 0;
	  F405_Send->Gimbal_100 = (short)(Gimbal.Pitch.MotorTransAngle * 100);
		F405_Send->Send_Pack1  = ((F405_Send->Mag_Flag&0x01)<<0)|
									((F405_Send->Laser_Flag&0x01)<<1)|
									((F405_Send->Graphic_Init_Flag&0x01)<<2)|
									((HighFreq_flag&0x01)<<3)|
	                ((F405_Send->Fric_Flag&0x01)<<4)|
	                ((armor_state&0x01)<<5)|
	                ((SelfProtect_Cross_Flag&0x01)<<6)|
									((k_slow&0x01)<<7);
    memcpy(&tx_message.Data[0],&F405_Send->SuperPowerLimit,1);
		memcpy(&tx_message.Data[1],&F405_Send->Chassis_Flag,1);	
		memcpy(&tx_message.Data[2],&F405_Send->Gimbal_100,2);
		memcpy(&tx_message.Data[4],&F405_Send->Gimbal_Flag,1);
		memcpy(&tx_message.Data[5],&F405_Send->Send_Pack1,1);
    CAN_Transmit(CAN1,&tx_message);
}

/**********************************************************************************************************
*函 数 名: GimbalCan2Send
*功能说明: 发送pitch/yaw电流值	Can2Send0
*形    参: pitch/yaw电流值
*返 回 值: 无
**********************************************************************************************************/
extern RobotInit_Struct Infantry;
void GimbalCan2Send(short X,short Y)
{
    CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = 0x1FF;
		X=LIMIT_MAX_MIN(X,30000,-30000);
	  Y=LIMIT_MAX_MIN(Y,30000,-30000);
	
	 switch(Infantry.PitchMotorID)
		 { 
		 case 0x205:		 
		 tx_message.Data[0] = (unsigned char)((X>>8)&0xff);//Ptich
		 tx_message.Data[1] = (unsigned char)(X&0xff); 
		 break;
		 case 0x206:
		 tx_message.Data[2] = (unsigned char)((X>>8)&0xff);//Ptich
		 tx_message.Data[3] = (unsigned char)(X&0xff);  
		 break;
		 case 0x207:
		 tx_message.Data[4] = (unsigned char)((X>>8)&0xff);//Ptich
		 tx_message.Data[5] = (unsigned char)(X&0xff);  
		 break;
		 case 0x208:
		 tx_message.Data[6] = (unsigned char)((X>>8)&0xff);//Ptich
		 tx_message.Data[7] = (unsigned char)(X&0xff); 
		 break;
		 default:
			break;
	 }
			switch(Infantry.YawMotorID)
		 { 
		 case 0x205:		 
		 tx_message.Data[0] = (unsigned char)((Y>>8)&0xff);//Ptich
		 tx_message.Data[1] = (unsigned char)(Y&0xff); 
		 break;
		 case 0x206:
		 tx_message.Data[2] = (unsigned char)((Y>>8)&0xff);//Ptich
		 tx_message.Data[3] = (unsigned char)(Y&0xff);  
		 break;
		 case 0x207:
		 tx_message.Data[4] = (unsigned char)((Y>>8)&0xff);//Ptich
		 tx_message.Data[5] = (unsigned char)(Y&0xff);  
		 break;
		 case 0x208:
		 tx_message.Data[6] = (unsigned char)((Y>>8)&0xff);//Ptich
		 tx_message.Data[7] = (unsigned char)(Y&0xff); 
		 break;
		 default:
			break;
	  }
		CAN_Transmit(CAN2,&tx_message);
}

void FrictionBodanCan2Send(short X,short Y,short Z)
{
    CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = 0x200;
		X=LIMIT_MAX_MIN(X,9000,-9000);
	  Y=LIMIT_MAX_MIN(Y,9000,-9000);
		Z=LIMIT_MAX_MIN(Z,9500,-9500);
	 	 switch(Infantry.FricMotorID[0])
		 { 
		 case 0x201:		 
		 tx_message.Data[0] = (unsigned char)((X>>8)&0xff);//Ptich
		 tx_message.Data[1] = (unsigned char)(X&0xff); 
		 break;
		 case 0x202:
		 tx_message.Data[2] = (unsigned char)((X>>8)&0xff);//Ptich
		 tx_message.Data[3] = (unsigned char)(X&0xff);  
		 break;
		 case 0x203:
		 tx_message.Data[4] = (unsigned char)((X>>8)&0xff);//Ptich
		 tx_message.Data[5] = (unsigned char)(X&0xff);  
		 break;
		 case 0x204:
		 tx_message.Data[6] = (unsigned char)((X>>8)&0xff);//Ptich
		 tx_message.Data[7] = (unsigned char)(X&0xff); 
		 break;
		 default:
			break;
	 }
		 	 switch(Infantry.FricMotorID[1])
		 { 
		 case 0x201:		 
		 tx_message.Data[0] = (unsigned char)((Y>>8)&0xff);//Ptich
		 tx_message.Data[1] = (unsigned char)(Y&0xff); 
		 break;
		 case 0x202:
		 tx_message.Data[2] = (unsigned char)((Y>>8)&0xff);//Ptich
		 tx_message.Data[3] = (unsigned char)(Y&0xff);  
		 break;
		 case 0x203:
		 tx_message.Data[4] = (unsigned char)((Y>>8)&0xff);//Ptich
		 tx_message.Data[5] = (unsigned char)(Y&0xff);  
		 break;
		 case 0x204:
		 tx_message.Data[6] = (unsigned char)((Y>>8)&0xff);//Ptich
		 tx_message.Data[7] = (unsigned char)(Y&0xff); 
		 break;
		 default:
			break;
	 }
	
	 	 	 switch(Infantry.BodanMotorID)
		 { 
		 case 0x201:		 
		 tx_message.Data[0] = (unsigned char)((Z>>8)&0xff);//Ptich
		 tx_message.Data[1] = (unsigned char)(Z&0xff); 
		 break;
		 case 0x202:
		 tx_message.Data[2] = (unsigned char)((Z>>8)&0xff);//Ptich
		 tx_message.Data[3] = (unsigned char)(Z&0xff);  
		 break;
		 case 0x203:
		 tx_message.Data[4] = (unsigned char)((Z>>8)&0xff);//Ptich
		 tx_message.Data[5] = (unsigned char)(Z&0xff);  
		 break;
		 case 0x204:
		 tx_message.Data[6] = (unsigned char)((Z>>8)&0xff);//Ptich
		 tx_message.Data[7] = (unsigned char)(Z&0xff); 
		 break;
		 default:
			break;
	 }
	
 		CAN_Transmit(CAN2,&tx_message);
}

/**********************************************************************************************************
*函 数 名: USART6_SendtoPC
*功能说明: 弹丸数和云台姿态交替发送,发送姿态频率是子弹数的4倍
					 PC_Sendflag 0x00待机  0x10辅瞄   0x20大符   0x30小符  0xffTx2关机
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/

typedef union{
	TickType_t xTickCount;
	unsigned char data[4];
}time_use;
time_use sendTOPC_time;
unsigned char Flag_use=0;


extern int SendToTx2BullectCnt;
short pitch; 
int yaw;
short down_sampling_rate = 4;		//1000为1帧			//4
int PC_TX_num=0;			//和Pc之间通信的计数器，用于降低通讯帧率
int Debug_yaw = 0;
int pitch_send ;
extern float GimbalYawPos,GimbalPitchPos;
extern float Buff_Yaw_Motor;
extern char smallBuff_flag;
char switchPriority;
void USART6_SendtoPC(void)
{
	char Mode_Flag;

	if(PC_TX_num % down_sampling_rate == 0)
	{
		SendToPC_Buff[0] = '!';
    Mode_Flag=(Status.GimbalMode==Gimbal_BigBuf_Mode?1:0);
		smallBuff_flag = (Status.GimbalMode == Gimbal_SmlBuf_Mode?1:0);
		SendToPC_Buff[1] = (0<<6|smallBuff_flag<<5|switchPriority<<4|Mode_Flag<<3|F105.BulletSpeedLevel<<1|F105.RobotRed)&0XFF; // 1为红色，0为蓝色
	
			
		pitch = (short)(Gimbal.Pitch.MotorTransAngle*100);
		yaw = (int)(Gimbal.Yaw.Gyro*100);
		if(Status.GimbalMode == Gimbal_Armor_Mode || Status.GimbalMode == Gimbal_Act_Mode)
		{
			pitch = (short)(Gimbal.Pitch.MotorTransAngle*100);
			yaw = (int)(Gimbal.Yaw.Gyro*100);
		}
		else if(Status.GimbalMode == Gimbal_BigBuf_Mode || Status.GimbalMode == Gimbal_SmlBuf_Mode)
		{
			pitch = (short)(Gimbal.Pitch.MotorTransAngle*100);
			yaw = (int)((Gimbal.Yaw.MotorTransAngle-Buff_Yaw_Motor)*100);
		}
		pitch_send = pitch;
		SendToPC_Buff[2] = (unsigned char)((pitch >> 8) & 0x00FF);
		SendToPC_Buff[3] = (unsigned char)((pitch) & 0x00FF);

		SendToPC_Buff[4] = (unsigned char)((yaw >> 24) & 0x000000FF);
		SendToPC_Buff[5] = (unsigned char)((yaw >> 16) & 0x000000FF);
		SendToPC_Buff[6] = (unsigned char)((yaw >> 8) & 0x000000FF);
		SendToPC_Buff[7] = (unsigned char)((yaw >> 0) & 0x000000FF);
		
		sendTOPC_time.xTickCount = xTaskGetTickCountFromISR();
		SendToPC_Buff[8] = sendTOPC_time.data[3];
		SendToPC_Buff[9] = sendTOPC_time.data[2];
		SendToPC_Buff[10] = sendTOPC_time.data[1];
		SendToPC_Buff[11] = sendTOPC_time.data[0];
		
		short sendbulletspeed=F105.bulletSpeed*100;
		SendToPC_Buff[12]= (unsigned char)((sendbulletspeed>> 8) & 0x00FF);
		SendToPC_Buff[13]= (unsigned char)((sendbulletspeed) & 0x00FF);
		SendToPC_Buff[15] = '#';
		Append_CRC8_Check_Sum(SendToPC_Buff,16);


		DMA_Cmd(DMA1_Stream6, ENABLE);
		
	}
	PC_TX_num++;
	if((PC_TX_num % (down_sampling_rate / 2) == 0)&&(PC_TX_num % down_sampling_rate != 0))
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	
}




///**********************************************************************************************************
//*函 数 名: USART6_SendtoPC
//*功能说明: 弹丸数和云台姿态交替发送,发送姿态频率是子弹数的4倍
//					 PC_Sendflag 0x00待机  0x10辅瞄   0x20大符   0x30小符  0xffTx2关机
//					 数据硬同步  down_sampling_rate控制周期 最低为5/ms
//*形    参: 无
//*返 回 值: 无
//**********************************************************************************************************/
//short down_sampling_rate = 6;		//1000为1帧			//4


//typedef union{
//	TickType_t xTickCount;
//	unsigned char data[4];
//}time_use;
//time_use sendTOPC_time;
//unsigned char Flag_use=0;

//typedef union{
//	uint32_t id;
//	unsigned char data[4];
//}UNION_imu_id;
//UNION_imu_id imu_id={0};

//extern int SendToTx2BullectCnt;
//short pitch; 
//int yaw;

//int PC_TX_num=0;			//和Pc之间通信的计数器，用于降低通讯帧率
//int Debug_yaw = 0;

//int pitch_send ;

//int camera_state=0;//触发信号启动

//extern float GimbalYawPos,GimbalPitchPos;
//extern float Buff_Yaw_Motor;
//extern char smallBuff_flag;
//extern Disconnect Robot_Disconnect;
//void USART6_SendtoPC(void)
//{
//	char Mode_Flag;

//	if(PC_TX_num % down_sampling_rate == 0)
//	{
//		SendToPC_Buff[0] = '!';
//    Mode_Flag=(Status.GimbalMode==Gimbal_BigBuf_Mode?1:0);
//		SendToPC_Buff[1] = (0<<6|smallBuff_flag<<5|0<<4|Mode_Flag<<3|F105.BulletSpeedLevel<<1|F105.RobotRed)&0XFF; // 1为红色，0为蓝色
//	
//			
//		pitch = (short)(Gimbal.Pitch.MotorTransAngle*100);
//		yaw = (int)(Gimbal.Yaw.Gyro*100);
//		if(Status.GimbalMode == Gimbal_Armor_Mode || Status.GimbalMode == Gimbal_Act_Mode)
//		{
//			pitch = (short)(Gimbal.Pitch.MotorTransAngle*100);
//			yaw = (int)(Gimbal.Yaw.Gyro*100);
//		}
//		else if(Status.GimbalMode == Gimbal_BigBuf_Mode || Status.GimbalMode == Gimbal_SmlBuf_Mode)
//		{
//			pitch = (short)(Gimbal.Pitch.MotorTransAngle*100);
//			yaw = (int)((Gimbal.Yaw.MotorTransAngle-Buff_Yaw_Motor)*100);
//		}
//		pitch_send = pitch;
//		SendToPC_Buff[2] = (unsigned char)((pitch >> 8) & 0x00FF);
//		SendToPC_Buff[3] = (unsigned char)((pitch) & 0x00FF);

//		SendToPC_Buff[4] = (unsigned char)((yaw >> 24) & 0x000000FF);
//		SendToPC_Buff[5] = (unsigned char)((yaw >> 16) & 0x000000FF);
//		SendToPC_Buff[6] = (unsigned char)((yaw >> 8) & 0x000000FF);
//		SendToPC_Buff[7] = (unsigned char)((yaw >> 0) & 0x000000FF);
//		
//		sendTOPC_time.xTickCount = xTaskGetTickCountFromISR();
//		SendToPC_Buff[8] = imu_id.data[3];
//		SendToPC_Buff[9] = imu_id.data[2];
//		SendToPC_Buff[10] = imu_id.data[1];
//		SendToPC_Buff[11] = imu_id.data[0];
//		
//		short sendbulletspeed=F105.bulletSpeed*100;
//		SendToPC_Buff[12]= (unsigned char)((sendbulletspeed>> 8) & 0x00FF);
//		SendToPC_Buff[13]= (unsigned char)((sendbulletspeed) & 0x00FF);
//		
//		SendToPC_Buff[15] = sendTOPC_time.data[3];
//		SendToPC_Buff[16] = sendTOPC_time.data[2];
//		SendToPC_Buff[17] = sendTOPC_time.data[1];
//		SendToPC_Buff[18] = sendTOPC_time.data[0];
//		SendToPC_Buff[19] = '#';
//		Append_CRC8_Check_Sum(SendToPC_Buff,PC_SENDBUF_SIZE);

////		while(DMA_GetCmdStatus(DMA1_Stream6)==ENABLE);
////		DMA_SetCurrDataCounter(DMA1_Stream6,PC_SENDBUF_SIZE);
//		DMA_Cmd(DMA1_Stream6, ENABLE);
//		camera_state = 1;//发送一次触发信号
//		imu_id.id++;
//		PBout(0)=1;
//	}
//	PC_TX_num++;
//	//Cmera_rising_edge(&camera_state,down_sampling_rate/2);//产生相机触发脉冲
//	if(PC_TX_num % down_sampling_rate == 1){
//		PBout(0)=0;
//	}
////	if((PC_TX_num % (down_sampling_rate / 2) == 0)&&(PC_TX_num % down_sampling_rate != 0))
////		GPIO_ResetBits(GPIOA,GPIO_Pin_1);
//	
//}




////串口传输延迟测试
//void USART6_SendtoPC(void)
//{
//	if(PC_TX_num % down_sampling_rate == 0)	//?down_sampling_rate?????,????
//	{
//	
////		PC_TX_num = 0;
//		SendToPC_Buff[0] = '!';

//		if(F105.BulletSpeedLevel == 0)
//			SendToPC_Buff[1] = 0x05;
//		else if(F105.BulletSpeedLevel == 1)
//			SendToPC_Buff[1] = 0x06;
//		else if(F105.BulletSpeedLevel == 2)
//			SendToPC_Buff[1] = 0x07;
//		else
//			SendToPC_Buff[1] = 0x05;
//			
//			pitch = (short)(Gimbal.Pitch.MotorTransAngle*100);
//			yaw = (int)(Debug_yaw);
//			Debug_yaw++;
//			if(Debug_yaw>3)
//			{
//			Debug_yaw=3;
//			}
//		
//		SendToPC_Buff[2] = (unsigned char)((pitch >> 8) & 0x00FF);
//		SendToPC_Buff[3] = (unsigned char)((pitch) & 0x00FF);

//		SendToPC_Buff[4] = (unsigned char)((yaw >> 24) & 0x000000FF);
//		SendToPC_Buff[5] = (unsigned char)((yaw >> 16) & 0x000000FF);
//		SendToPC_Buff[6] = (unsigned char)((yaw >> 8) & 0x000000FF);
//		SendToPC_Buff[7] = (unsigned char)((yaw >> 0) & 0x000000FF);
//		
//		//	Tx2_Off_CheckAndSet(SendToPC_Buff);
//		//?????
//		sendTOPC_time.xTickCount = xTaskGetTickCountFromISR();
//		SendToPC_Buff[8] = sendTOPC_time.data[3];
//		SendToPC_Buff[9] = sendTOPC_time.data[2];
//		SendToPC_Buff[10] = sendTOPC_time.data[1];
//		SendToPC_Buff[11] = sendTOPC_time.data[0];
//		
//		
//		SendToPC_Buff[13] = '#';
//		Append_CRC8_Check_Sum(SendToPC_Buff,14);

//		
//		GPIO_ResetBits(GPIOA,GPIO_Pin_1);
//		DMA_Cmd(DMA2_Stream7, ENABLE);
//		
//	}
//	PC_TX_num++;
////	if((PC_TX_num % (down_sampling_rate / 2) == 0)&&(PC_TX_num % down_sampling_rate != 0))
////		GPIO_ResetBits(GPIOA,GPIO_Pin_1);
//	
//}

/**********************************************************************************************************
*函 数 名: Tx2_Off_CheckAndSet
*功能说明: 发送指令给tx2使其关机
*形    参: 指向发送数据的指针
*返 回 值: 无
**********************************************************************************************************/
void Tx2_Off_CheckAndSet(u8* Buff)
{
	int i;
	if(PC_Sendflag == Tx2_Off)
	for(i = 0;i < 9;i++)
	{
		*Buff = '!';
		Buff++;
	}
}
/**********************************************************************************************************
*函 数 名: TX2_task
*功能说明: 通信任务
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
uint32_t TX2_high_water;
TickType_t LastWakeTime_PC;
void TX2_task(void *pvParameters)
{
   while (1) {
   LastWakeTime_PC = xTaskGetTickCount();
   USART6_SendtoPC();
	  IWDG_Feed();
		vTaskDelayUntil(&LastWakeTime_PC,1);
		 
#if INCLUDE_uxTaskGetStackHighWaterMark
        TX2_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}
