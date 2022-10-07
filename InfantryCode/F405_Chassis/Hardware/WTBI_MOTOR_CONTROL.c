/**********************************************************************************************************
 * @�ļ�     WTBI_MOTOR_CONTROL.c
 * @˵��     ƽ�ⲽ���������
 * @�汾  	 V1.0
 * @����     ���κ�
 * @����     2022.1
**********************************************************************************************************/
#include "main.h"


/*********************************�����������Ϣ**********************************************/


/**********************************************************************************************************
*�� �� ��: HUB_MOTOR_GetState
*����˵��: ��ȡ��챵��״̬
*��    ��: ת��ֵ
*�� �� ֵ: ��
**********************************************************************************************************/
void HUB_MOTOR_GetState(void)
{
	HUB_MOTOR_ORDER(STATE_MOTOR_STATE);
}
/**********************************************************************************************************
*�� �� ��: HUB_MOTOR_GetState
*����˵��: ��ȡ��챵��״̬
*��    ��: ת��ֵ
*�� �� ֵ: ��
**********************************************************************************************************/
void HUB_MOTOR_GetMultAngle(void)
{
	HUB_MOTOR_ORDER(STATE_MOTOR_GET_MULTIANGLE);
}

/**********************************************************************************************************
*�� �� ��: HUB_MOTOR_GetState
*����˵��: ��ȡ��챵��״̬
*��    ��: ת��ֵ
*�� �� ֵ: ��
**********************************************************************************************************/
void HUB_MOTOR_GetError(void)
{
	HUB_MOTOR_ORDER(STATE_MOTOR_ERROR);
}

/**********************************************************************************************************
*�� �� ��: HUB_MOTOR_ORDER
*����˵��: ��챵������
*��    ��: orderΪ���Ϳ������Ooder_frameΪ����8�ķ�������֡�������飬���ΪNULL���ʾֻ����������Ϣ
						order���ð�����
							getState:
								��ȡ����������							0x90
								д�������ֵ��Ϊ������		0x91
								д�뵱ǰֵ��Ϊ���					0x19
								��ȡ��������־��״̬			0x9A
								��ȡ���״̬								0x9C
								*��ȡ��Ȧ�Ƕ�								0x92
								*��ȡ��Ȧ�Ƕ�								0x94
								�رյ��(���֮ǰ״̬/��)		0x80/0x81
								���е��										0x88
								
							Conmmand��
								�����ת�رջ�����								0xA1
								
*�� �� ֵ: ��
**********************************************************************************************************/
static void HUB_MOTOR_ORDER(uint8_t order)
{
	CanTxMsg tx_message;
	tx_message.IDE = CAN_ID_STD;    
	tx_message.RTR = CAN_RTR_DATA; 
	tx_message.DLC = 0x08;    
	tx_message.Data[0] = order;

	//�����������ȡ��StdId��ʾ��ȡ�������ID��0x140+(1~32)
	tx_message.StdId = 0x140;
	CAN_Transmit(CAN1,&tx_message);
	
	tx_message.StdId = 0x141;
	CAN_Transmit(CAN1,&tx_message);
}

/**********************************************************************************************************
*�� �� ��: Can1Send_Developer
*����˵��: ֱ�ӷ�������֡��������
*��    ��: StdId��ʾ��ȡ�������ID��0x140+(1~32)���ĸ��������ֵ
*�� �� ֵ: ��
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
*�� �� ��: Knee_motor_Init(uint32_t StdId,uint32_t StdId)
*����˵��: ֱ�ӷ�������֡��������
*��    ��: StdId��ʾ��ȡ�������ID��0x140+(1~32)���ĸ��������ֵ
*�� �� ֵ: ��
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
*�� �� ��: HUB_TORQUE_SEND
*����˵��: ��챵��ת�ط���
*��    ��: ת��ֵ
*�� �� ֵ: ��
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
*�� �� ��: M1502DSend_SetID
*����˵��: ����M1502D�����ID
*��    ��: 
*�� �� ֵ: ��
**********************************************************************************************************/
void M1502DSend_SetID(char id)
{
	static int Flag = 0;
	

	CanTxMsg tx_message;
	tx_message.IDE = CAN_ID_STD;    
	tx_message.RTR = CAN_RTR_DATA; 
	tx_message.DLC = 0x08;    
	tx_message.StdId = 0x108;
	

	tx_message.Data[0] = id; //���õ�ǰ���ӵ��ID 1-7
	for(int i=1;i<8;i++)
		tx_message.Data[i]=0;

	
	CAN_Transmit(CAN1,&tx_message);


}

/**********************************************************************************************************
*�� �� ��: Can1Send_Developer
*����˵��: ����M1502D�����ģʽ
*��    ��: 
*�� �� ֵ: ��
**********************************************************************************************************/
void M1502DSend_SetControlMode()
{

    CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = 0x105;
		

    tx_message.Data[0] = 0x01; //���õ��IDΪ1�Ŀ���ģʽΪ������
		tx_message.Data[1] = 0x01; //���õ��IDΪ2�Ŀ���ģʽΪ������
		
    CAN_Transmit(CAN1,&tx_message);

	
}
/**********************************************************************************************************
*�� �� ��: Can1Send_Developer
*����˵��: ����M1502D����ķ�����ʽ
*��    ��: 
*�� �� ֵ: ��
**********************************************************************************************************/
void M1502DSend_SetfeedbackMode()
{

    CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = 0x106;
		

    tx_message.Data[0] = 0x01; //���õ��IDΪ1�Ŀ���ģʽΪ��ѯ�ϱ�0x80
		tx_message.Data[1] = 0x01; //���õ��IDΪ2�Ŀ���ģʽΪ����1ms�ϱ�
		
    CAN_Transmit(CAN1,&tx_message);

	
}

/**********************************************************************************************************
*�� �� ��: M1502DSend_SetCanRestor
*����˵��: ����M1502D�����can����
*��    ��: 
*�� �� ֵ: ��
**********************************************************************************************************/
void M1502DSend_SetCanRestor()
{

	

    CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = 0x109;
		

    tx_message.Data[0] = 0x01; //���õ��IDΪ1�ĵ������
		tx_message.Data[1] = 0x00; //���õ��IDΪ2�ĵ���Ͽ�
		
    CAN_Transmit(CAN1,&tx_message);


}

/**********************************************************************************************************
*�� �� ��: M1502DSend_SetCanRestor
*����˵��: ��ѯ���ģʽ
*��    ��: 
*�� �� ֵ: ��
**********************************************************************************************************/
void M1502DSend_GetMotorState(char ID)
{

    CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = 0x107;
		

    tx_message.Data[0] = ID; 
		tx_message.Data[1] = 0x06; //��ѯģʽ
		
    CAN_Transmit(CAN1,&tx_message);

}
/**********************************************************************************************************
*�� �� ��: MD1502HubMotor_ENABLE
*����˵��: ����MD1502
*��    ��: 
*�� �� ֵ: ��
**********************************************************************************************************/
void MD1502HubMotor_ENABLE(void){
	CanTxMsg tx_message;
	tx_message.IDE = CAN_ID_STD;    
	tx_message.RTR = CAN_RTR_DATA; 
	tx_message.DLC = 0x08;    
	tx_message.StdId = 0x105;
	

	tx_message.Data[0] = 0x0A; //���õ��IDΪ1�Ŀ���ģʽΪ����
	tx_message.Data[1] = 0x0A; //���õ��IDΪ2�Ŀ���ģʽΪ����
	
	CAN_Transmit(CAN1,&tx_message);
}

/**********************************************************************************************************
*�� �� ��: MD1502HubMotor_DISABLE
*����˵��: �ر�MD1502
*��    ��: 
*�� �� ֵ: ��
**********************************************************************************************************/
void MD1502HubMotor_DISABLE(void){
	CanTxMsg tx_message;
	tx_message.IDE = CAN_ID_STD;    
	tx_message.RTR = CAN_RTR_DATA; 
	tx_message.DLC = 0x08;    
	tx_message.StdId = 0x105;
	

	tx_message.Data[0] = 0x09; //���õ��IDΪ1�Ŀ���ģʽΪ�ر�
	tx_message.Data[1] = 0x09; //���õ��IDΪ2�Ŀ���ģʽΪ�ر�
	
	CAN_Transmit(CAN1,&tx_message);
}
/*********************************������Ϣ**********************************************/

