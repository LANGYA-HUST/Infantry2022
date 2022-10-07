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
 * @日期     2022.4
**********************************************************************************************************/
#include "main.h"
/*----------------------------------内部变量---------------------------*/

/*----------------------------------结构体-----------------------------*/

/*----------------------------------外部变量---------------------------*/
extern INA260 INA260_1;//输入
extern INA260 INA260_2;//输出
extern float AD_actual_value;//电容实际电压
extern F405_typedef F405;
extern JudgeReceive_t JudgeReceive;
extern char Judge_Lost;
/**********************************************************************************************************
*函 数 名: ChassisCan1Send
*功能说明: 底盘电机电流值发送
*形    参: 四个电机电流值
*返 回 值: 无
**********************************************************************************************************/
//TickType_t nowtick,lasttick,interval;
short last[4],lastsum,sum;
short post_out[4];
extern short CAP_CrossoverFlag;
char output_filter;
float K_Power;
extern RM820RReceive_Typedef ChassisMotorCanReceive[4];
void ChassisCan1Send(short a,short b,short c,short d)
{
	
    CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = 0x200;
  
    sum=ABS(a)+ABS(b)+ABS(c)+ABS(d);
	
				if(sum-lastsum>2000)		//发生变化
			{
				CAP_CrossoverFlag = 1;																										//电容停止充电
			}
			lastsum=sum;

			if(F405.Chassis_Flag == Chassis_Test_Mode)
			{
	      a = -1000;
        b =  1000;
		    c = -1000;
	      d	=  1000;	
			}
//		K_Power = 0.01f/JudgeReceive.realChassispower*(ABS(ChassisMotorCanReceive[0].RealSpeed)+ABS(ChassisMotorCanReceive[1].RealSpeed)+ABS(ChassisMotorCanReceive[2].RealSpeed)+ABS(ChassisMotorCanReceive[3].RealSpeed));
		//1000   1700
		post_out[0] = a;
		post_out[1] = b;
		post_out[2] = c;
		post_out[3] = d;
    tx_message.Data[0] = (unsigned char)((a>>8)&0xff);
    tx_message.Data[1] = (unsigned char)( a &0xff);  
    tx_message.Data[2] = (unsigned char)((b>>8)&0xff);
    tx_message.Data[3] = (unsigned char)( b &0xff);
    tx_message.Data[4] = (unsigned char)((c>>8)&0xff);
    tx_message.Data[5] = (unsigned char)( c&0xff);
    tx_message.Data[6] = (unsigned char)((d>>8)&0xff);
    tx_message.Data[7] = (unsigned char)( d &0xff);
    CAN_Transmit(CAN1,&tx_message);
}
/**********************************************************************************************************
*函 数 名: Can2Send0
*功能说明: can2发送函数
*形    参: ChassisSpeedw, Remain_power, IsShootAble, RobotRed, BulletSpeedLevel
*返 回 值: 无
**********************************************************************************************************/
void Can2Send0(F105_Typedef *F105_Send)
{
	  CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = 0x100;
	  
	  memcpy(&tx_message.Data[0],&F105_Send->ChassisSpeedw,2);
		memcpy(&tx_message.Data[2],&F105_Send->Remain_power,2);
		memcpy(&tx_message.Data[4],&F105_Send->IsShootAble,1);
		memcpy(&tx_message.Data[5],&F105_Send->RobotRed,1);
		memcpy(&tx_message.Data[6],&F105_Send->BulletSpeedLevel,1);	

	  CAN_Transmit(CAN2,&tx_message);
}
/**********************************************************************************************************
*函 数 名: Can2Send1
*功能说明: can2发送函数
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void Can2Send1(void)
{
	  CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = 0x094;
	  
	  memcpy(&tx_message.Data[0],&JudgeReceive.bulletSpeed,4);	
    memcpy(&tx_message.Data[4],&Judge_Lost,1);
	  memcpy(&tx_message.Data[5],&JudgeReceive.RobotLevel,1);
	  memcpy(&tx_message.Data[6],&JudgeReceive.HeatCool17,2);		
	   
	  CAN_Transmit(CAN2,&tx_message);
}

/**********************************************************************************************************
*函 数 名: Can2Send1
*功能说明: can2发送函数
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/

void Can2Send2(uint8_t BuffState)
{
	  CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x01;    
    tx_message.StdId = 0x096;
	  
	  tx_message.Data[0] = BuffState&0x02; 
	
	  CAN_Transmit(CAN2,&tx_message);
}
