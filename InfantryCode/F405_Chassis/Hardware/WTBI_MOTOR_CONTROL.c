/**********************************************************************************************************
 * @文件     WTBI_MOTOR_CONTROL.c
 * @说明     平衡步兵电机控制
 * @版本  	 V1.0
 * @作者     郭嘉豪
 * @日期     2022.1
**********************************************************************************************************/
#include "main.h"


/*********************************单电机发送消息**********************************************/


/**********************************************************************************************************
*函 数 名: HUB_MOTOR_GetState
*功能说明: 获取轮毂电机状态
*形    参: 转矩值
*返 回 值: 无
**********************************************************************************************************/
void HUB_MOTOR_GetState(void)
{
	HUB_MOTOR_ORDER(STATE_MOTOR_STATE);
}
/**********************************************************************************************************
*函 数 名: HUB_MOTOR_GetState
*功能说明: 获取轮毂电机状态
*形    参: 转矩值
*返 回 值: 无
**********************************************************************************************************/
void HUB_MOTOR_GetMultAngle(void)
{
	HUB_MOTOR_ORDER(STATE_MOTOR_GET_MULTIANGLE);
}

/**********************************************************************************************************
*函 数 名: HUB_MOTOR_GetState
*功能说明: 获取轮毂电机状态
*形    参: 转矩值
*返 回 值: 无
**********************************************************************************************************/
void HUB_MOTOR_GetError(void)
{
	HUB_MOTOR_ORDER(STATE_MOTOR_ERROR);
}

/**********************************************************************************************************
*函 数 名: HUB_MOTOR_ORDER
*功能说明: 轮毂电机控制
*形    参: order为发送控制命令，Ooder_frame为长度8的发送命令帧内容数组，如果为NULL则表示只用来接受信息
						order常用包含：
							getState:
								读取编码器命令							0x90
								写入编码器值作为电机零点		0x91
								写入当前值作为零点					0x19
								读取电机错误标志与状态			0x9A
								读取电机状态								0x9C
								*读取多圈角度								0x92
								*读取单圈角度								0x94
								关闭电机(清除之前状态/不)		0x80/0x81
								运行电机										0x88
								
							Conmmand：
								单电机转矩闭环控制								0xA1
								
*返 回 值: 无
**********************************************************************************************************/
static void HUB_MOTOR_ORDER(uint8_t order)
{
	CanTxMsg tx_message;
	tx_message.IDE = CAN_ID_STD;    
	tx_message.RTR = CAN_RTR_DATA; 
	tx_message.DLC = 0x08;    
	tx_message.Data[0] = order;

	//两个电机都读取，StdId表示获取单电机的ID号0x140+(1~32)
	tx_message.StdId = 0x140;
	CAN_Transmit(CAN1,&tx_message);
	
	tx_message.StdId = 0x141;
	CAN_Transmit(CAN1,&tx_message);
}

/**********************************************************************************************************
*函 数 名: Can1Send_Developer
*功能说明: 直接发送命令帧，调试以
*形    参: StdId表示获取单电机的ID号0x140+(1~32)，四个电机电流值
*返 回 值: 无
**********************************************************************************************************/
void Can1Send_Developer(uint32_t StdId,uint8_t Oder_frame[8])
{
    CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = StdId;
		
		for(int i=0;i<8;i++)
    tx_message.Data[i] = Oder_frame[i];

    CAN_Transmit(CAN2,&tx_message);
}

/**********************************************************************************************************
*函 数 名: Knee_motor_Init(uint32_t StdId,uint32_t StdId)
*功能说明: 直接发送命令帧，调试以
*形    参: StdId表示获取单电机的ID号0x140+(1~32)，四个电机电流值
*返 回 值: 无
**********************************************************************************************************/
void Knee_motor_Init(uint32_t LStdId,uint32_t RStdId)
{
	static int Flag = 0;
	
	if(Flag==0)
	{
		uint8_t send[8];
		send[0] = 0x19;
		Can1Send_Developer(LStdId,send);
		Can1Send_Developer(RStdId,send);	
		Flag=1;
	}

	
}


/**********************************************************************************************************
*函 数 名: HUB_TORQUE_SEND
*功能说明: 轮毂电机转矩发送
*形    参: 转矩值
*返 回 值: 无
**********************************************************************************************************/
void HUB_TORQUE_SEND(short a,short b)
{
    CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = 0x280;
	  a=LIMIT_MAX_MIN(a,2000,-2000);
	  b=LIMIT_MAX_MIN(b,2000,-2000);
    tx_message.Data[0] = (unsigned char)((a>>8)&0xff);
    tx_message.Data[1] = (unsigned char)(a&0xff);  
    tx_message.Data[2] = (unsigned char)((b>>8)&0xff);
    tx_message.Data[3] = (unsigned char)(b&0xff);
    CAN_Transmit(CAN1,&tx_message);
}



/**********************************************************************************************************
*函 数 名: M1502DSend_SetID
*功能说明: 设置M1502D电机的ID
*形    参: 
*返 回 值: 无
**********************************************************************************************************/
void M1502DSend_SetID(char id)
{
	static int Flag = 0;
	

	CanTxMsg tx_message;
	tx_message.IDE = CAN_ID_STD;    
	tx_message.RTR = CAN_RTR_DATA; 
	tx_message.DLC = 0x08;    
	tx_message.StdId = 0x108;
	

	tx_message.Data[0] = id; //设置当前连接电机ID 1-7
	for(int i=1;i<8;i++)
		tx_message.Data[i]=0;

	
	CAN_Transmit(CAN1,&tx_message);


}

/**********************************************************************************************************
*函 数 名: Can1Send_Developer
*功能说明: 设置M1502D电机的模式
*形    参: 
*返 回 值: 无
**********************************************************************************************************/
void M1502DSend_SetControlMode()
{

    CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = 0x105;
		

    tx_message.Data[0] = 0x01; //设置电机ID为1的控制模式为电流环
		tx_message.Data[1] = 0x01; //设置电机ID为2的控制模式为电流环
		
    CAN_Transmit(CAN1,&tx_message);

	
}
/**********************************************************************************************************
*函 数 名: Can1Send_Developer
*功能说明: 设置M1502D电机的反馈方式
*形    参: 
*返 回 值: 无
**********************************************************************************************************/
void M1502DSend_SetfeedbackMode()
{

    CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = 0x106;
		

    tx_message.Data[0] = 0x01; //设置电机ID为1的控制模式为查询上报0x80
		tx_message.Data[1] = 0x01; //设置电机ID为2的控制模式为主动1ms上报
		
    CAN_Transmit(CAN1,&tx_message);

	
}

/**********************************************************************************************************
*函 数 名: M1502DSend_SetCanRestor
*功能说明: 设置M1502D电机的can电阻
*形    参: 
*返 回 值: 无
**********************************************************************************************************/
void M1502DSend_SetCanRestor()
{

	

    CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = 0x109;
		

    tx_message.Data[0] = 0x01; //设置电机ID为1的电阻接入
		tx_message.Data[1] = 0x00; //设置电机ID为2的电阻断开
		
    CAN_Transmit(CAN1,&tx_message);


}

/**********************************************************************************************************
*函 数 名: M1502DSend_SetCanRestor
*功能说明: 查询电机模式
*形    参: 
*返 回 值: 无
**********************************************************************************************************/
void M1502DSend_GetMotorState(char ID)
{

    CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = 0x107;
		

    tx_message.Data[0] = ID; 
		tx_message.Data[1] = 0x06; //查询模式
		
    CAN_Transmit(CAN1,&tx_message);

}
/**********************************************************************************************************
*函 数 名: MD1502HubMotor_ENABLE
*功能说明: 开启MD1502
*形    参: 
*返 回 值: 无
**********************************************************************************************************/
void MD1502HubMotor_ENABLE(void){
	CanTxMsg tx_message;
	tx_message.IDE = CAN_ID_STD;    
	tx_message.RTR = CAN_RTR_DATA; 
	tx_message.DLC = 0x08;    
	tx_message.StdId = 0x105;
	

	tx_message.Data[0] = 0x0A; //设置电机ID为1的控制模式为开启
	tx_message.Data[1] = 0x0A; //设置电机ID为2的控制模式为开启
	
	CAN_Transmit(CAN1,&tx_message);
}

/**********************************************************************************************************
*函 数 名: MD1502HubMotor_DISABLE
*功能说明: 关闭MD1502
*形    参: 
*返 回 值: 无
**********************************************************************************************************/
void MD1502HubMotor_DISABLE(void){
	CanTxMsg tx_message;
	tx_message.IDE = CAN_ID_STD;    
	tx_message.RTR = CAN_RTR_DATA; 
	tx_message.DLC = 0x08;    
	tx_message.StdId = 0x105;
	

	tx_message.Data[0] = 0x09; //设置电机ID为1的控制模式为关闭
	tx_message.Data[1] = 0x09; //设置电机ID为2的控制模式为关闭
	
	CAN_Transmit(CAN1,&tx_message);
}
/*********************************接受消息**********************************************/

