/**********************************************************************************************************
 * @文件     can2.c
 * @说明     数据发送
 * @版本  	 V1.0
 * @作者     黄志雄
 * @日期     2019.10
**********************************************************************************************************/
#include "main.h"
/**********************************************************************************************************
*函 数 名: CAN2_Configuration
*功能说明: can2配置函数
*形    参: 无
*返 回 值: 无
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
	can.CAN_NART = ENABLE; //关闭失败自动重传 
	can.CAN_RFLM = DISABLE; 
	can.CAN_TXFP = ENABLE; 
	can.CAN_Mode = CAN_Mode_Normal;
	can.CAN_SJW = CAN_SJW_1tq;
	can.CAN_BS1 = CAN_BS1_11tq;
	can.CAN_BS2 = CAN_BS2_2tq;
	can.CAN_Prescaler = 3; //CAN BaudRate 42/(1+9+4)/3=1Mbps
	CAN_Init(CAN2, &can);
	//FIFO0，列表模式，只接收0x201 0x202 0x205 0x206
	can_filter.CAN_FilterNumber = 15; //选择过滤器15
	can_filter.CAN_FilterMode = CAN_FilterMode_IdList;
	can_filter.CAN_FilterScale = CAN_FilterScale_16bit;
	//can_filter.CAN_FilterIdHigh = 0x100 << 5;
	//can_filter.CAN_FilterIdLow = 0x101 << 5;
	can_filter.CAN_FilterMaskIdHigh = 0x101 << 5;
	can_filter.CAN_FilterMaskIdLow = 0x102 << 5;
	can_filter.CAN_FilterFIFOAssignment = 0;//fifo0
	can_filter.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&can_filter);
	//FIFO1，只接收gyro_id
//	can_filter.CAN_FilterNumber = 16; //选择过滤器16
//	can_filter.CAN_FilterMode = CAN_FilterMode_IdList; //列表模式
//	can_filter.CAN_FilterScale = CAN_FilterScale_16bit;
//	can_filter.CAN_FilterIdHigh = 0x103 << 5;
//	can_filter.CAN_FilterIdLow = 0x104 <<5;
//	can_filter.CAN_FilterMaskIdHigh = 0x100 << 5;
//	can_filter.CAN_FilterMaskIdLow = 0x101 <<5;
//	can_filter.CAN_FilterFIFOAssignment = 1;//fifo1
//	can_filter.CAN_FilterActivation = ENABLE;
//	CAN_FilterInit(&can_filter);
	//CAN中断配置
	CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);
	//CAN_ITConfig(CAN2,CAN_IT_FMP1,ENABLE);
	CAN_ITConfig(CAN2,CAN_IT_TME,ENABLE);
}
/**********************************************************************************************************
*函 数 名: CAN2_TX_IRQHandler
*功能说明: can2发送中断
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
//void CAN2_TX_IRQHandler(void)
//{
//	if (CAN_GetITStatus(CAN2,CAN_IT_TME)!= RESET) 
//	{
//		CAN_ClearITPendingBit(CAN2,CAN_IT_TME);
//	}
//}
/**********************************************************************************************************
*函 数 名: CAN2_RX0_IRQHandler
*功能说明: can2接收中断(Pitch,Yaw电机角度接收)
*形    参: 无
*返 回 值: 无
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
*函 数 名: CAN2_RX1_IRQHandler
*功能说明: can2接收中断(陀螺仪数据接收)
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
//void CAN2_RX1_IRQHandler(void)
//{
//	CanRxMsg rx_message1;
//	if (CAN_GetITStatus(CAN2,CAN_IT_FMP1)!= RESET)
//	{
//		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP1);
//		CAN_Receive(CAN2, CAN_FIFO1, &rx_message1);
//		Can2Receive1(&rx_message1); // ID陀螺仪代码冲突
//	}
//}
