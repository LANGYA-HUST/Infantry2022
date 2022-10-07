/**********************************************************************************************************
 * @�ļ�     can2.c
 * @˵��     ���ݷ���
 * @�汾  	 V1.0
 * @����     ��־��
 * @����     2019.10
**********************************************************************************************************/
#include "main.h"
/**********************************************************************************************************
*�� �� ��: CAN2_Configuration
*����˵��: can2���ú���
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void CAN2_Configuration(void)
{
	CAN_InitTypeDef can;
	CAN_FilterInitTypeDef can_filter;
	GPIO_InitTypeDef gpio;
	NVIC_InitTypeDef nvic;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);
	gpio.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_12 ;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &gpio);
	nvic.NVIC_IRQChannel = CAN2_RX0_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 3;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	nvic.NVIC_IRQChannel = CAN2_RX1_IRQn;
//	nvic.NVIC_IRQChannelPreemptionPriority = 0;
//	nvic.NVIC_IRQChannelSubPriority = 2;
//	nvic.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&nvic);
//	nvic.NVIC_IRQChannel = CAN2_TX_IRQn;
//	nvic.NVIC_IRQChannelPreemptionPriority = 0;
//	nvic.NVIC_IRQChannelSubPriority = 1;
//	nvic.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&nvic); 
	CAN_DeInit(CAN2);
	CAN_StructInit(&can);
	can.CAN_TTCM = DISABLE;
	can.CAN_ABOM = ENABLE; 
	can.CAN_AWUM = ENABLE; 
	can.CAN_NART = ENABLE; //�ر�ʧ���Զ��ش� 
	can.CAN_RFLM = DISABLE; 
	can.CAN_TXFP = ENABLE; 
	can.CAN_Mode = CAN_Mode_Normal;
	can.CAN_SJW = CAN_SJW_1tq;
	can.CAN_BS1 = CAN_BS1_11tq;
	can.CAN_BS2 = CAN_BS2_2tq;
	can.CAN_Prescaler = 3; //CAN BaudRate 42/(1+9+4)/3=1Mbps
	CAN_Init(CAN2, &can);
	//FIFO0���б�ģʽ��ֻ����0x201 0x202 0x205 0x206
	can_filter.CAN_FilterNumber = 15; //ѡ�������15
	can_filter.CAN_FilterMode = CAN_FilterMode_IdList;
	can_filter.CAN_FilterScale = CAN_FilterScale_16bit;
	//can_filter.CAN_FilterIdHigh = 0x100 << 5;
	//can_filter.CAN_FilterIdLow = 0x101 << 5;
	can_filter.CAN_FilterMaskIdHigh = 0x101 << 5;
	can_filter.CAN_FilterMaskIdLow = 0x102 << 5;
	can_filter.CAN_FilterFIFOAssignment = 0;//fifo0
	can_filter.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&can_filter);
	//FIFO1��ֻ����gyro_id
//	can_filter.CAN_FilterNumber = 16; //ѡ�������16
//	can_filter.CAN_FilterMode = CAN_FilterMode_IdList; //�б�ģʽ
//	can_filter.CAN_FilterScale = CAN_FilterScale_16bit;
//	can_filter.CAN_FilterIdHigh = 0x103 << 5;
//	can_filter.CAN_FilterIdLow = 0x104 <<5;
//	can_filter.CAN_FilterMaskIdHigh = 0x100 << 5;
//	can_filter.CAN_FilterMaskIdLow = 0x101 <<5;
//	can_filter.CAN_FilterFIFOAssignment = 1;//fifo1
//	can_filter.CAN_FilterActivation = ENABLE;
//	CAN_FilterInit(&can_filter);
	//CAN�ж�����
	CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);
	//CAN_ITConfig(CAN2,CAN_IT_FMP1,ENABLE);
	CAN_ITConfig(CAN2,CAN_IT_TME,ENABLE);
}
/**********************************************************************************************************
*�� �� ��: CAN2_TX_IRQHandler
*����˵��: can2�����ж�
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
//void CAN2_TX_IRQHandler(void)
//{
//	if (CAN_GetITStatus(CAN2,CAN_IT_TME)!= RESET) 
//	{
//		CAN_ClearITPendingBit(CAN2,CAN_IT_TME);
//	}
//}
/**********************************************************************************************************
*�� �� ��: CAN2_RX0_IRQHandler
*����˵��: can2�����ж�(Pitch,Yaw����ǶȽ���)
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void CAN2_RX0_IRQHandler(void)
{
	CanRxMsg rx_message0;
	if (CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET)
	{
		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
		CAN_Receive(CAN2, CAN_FIFO0, &rx_message0);
		Can2Receive1(&rx_message0);
	}
}
/**********************************************************************************************************
*�� �� ��: CAN2_RX1_IRQHandler
*����˵��: can2�����ж�(���������ݽ���)
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
//void CAN2_RX1_IRQHandler(void)
//{
//	CanRxMsg rx_message1;
//	if (CAN_GetITStatus(CAN2,CAN_IT_FMP1)!= RESET)
//	{
//		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP1);
//		CAN_Receive(CAN2, CAN_FIFO1, &rx_message1);
//		Can2Receive1(&rx_message1); // ID�����Ǵ����ͻ
//	}
//}
