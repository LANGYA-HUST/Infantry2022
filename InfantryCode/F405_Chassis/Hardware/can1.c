/**********************************************************************************************************
 * @文件     can1.c
 * @说明     can1初始化
 * @版本  	 V1.0
 * @作者     郭嘉豪
 * @日期     2021.10
**********************************************************************************************************/
/*CAN1控制底盘，ID号如下：
			        头 // 6020(3508)
			|
	0x208(0x204)	|   0x207(0x203)
			|
————————————————————————————
			|
	0x206(0x202)	|   0x205(0x201)
			|
*******************************************************************************************************			
		头（ID）
			 |
	  4	     3
			 |
————————————————————————————
			 |
	  2	     1
			 |
***************************************************************************************/
#include "main.h"
/**********************************************************************************************************
*函 数 名: CAN1_Configuration
*功能说明: can1初始化
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void CAN1_Configuration()
{
		GPIO_InitTypeDef GPIO_InitStructure;
		CAN_InitTypeDef        CAN_InitStructure;
		CAN_FilterInitTypeDef  CAN_FilterInitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		/* 打开GPIO时钟、AFIO时钟，CAN时钟 */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

		GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

		/* CAN1 Enabling interrupt */									  
		NVIC_InitStructure.NVIC_IRQChannel=CAN1_RX0_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);									
		
//		NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
		
		/* CAN  BaudRate = RCC_APB1PeriphClock/(CAN_SJW+CAN_BS1+CAN_BS2)/CAN_Prescaler */
		CAN_DeInit(CAN1);
		CAN_StructInit(&CAN_InitStructure);   

		CAN_InitStructure.CAN_TTCM=DISABLE;
		CAN_InitStructure.CAN_ABOM=ENABLE;
		CAN_InitStructure.CAN_AWUM=ENABLE;
		CAN_InitStructure.CAN_NART=ENABLE;
		CAN_InitStructure.CAN_RFLM=DISABLE;
		CAN_InitStructure.CAN_TXFP=ENABLE;
		CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;   
		//CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;
    CAN_InitStructure.CAN_SJW  = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_11tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
		CAN_InitStructure.CAN_Prescaler = 3;   //CAN BaudRate 42/(1+11+2)/3=1kbps


		CAN_Init(CAN1,&CAN_InitStructure);	// CAN1											

		CAN_FilterInitStructure.CAN_FilterNumber=0;	 
		CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;	 // 标识符屏蔽位模式
		CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_16bit;   // 16位过滤器
		CAN_FilterInitStructure.CAN_FilterIdHigh =  0x201<<5;
		CAN_FilterInitStructure.CAN_FilterIdLow = 0x202<<5;
		CAN_FilterInitStructure.CAN_FilterMaskIdHigh =  0x203<<5;
		CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x204<<5;
		CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;	 // FIFO0指向过滤器
		CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
		CAN_FilterInit(&CAN_FilterInitStructure);
		
		
	//	CAN_FilterInitStructure.CAN_FilterNumber = 1; //选择过滤器1
	//	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; //列表模式
	//	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	//	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x205<<5;
	//	CAN_FilterInitStructure.CAN_FilterIdLow =  0x206<<5;
	//	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x207<<5;  
	//	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x208<<5;
	//	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO1;//fifo
	//	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	//	CAN_FilterInit(&CAN_FilterInitStructure);
		
		CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);  // CAN1
		//CAN_ITConfig(CAN1,CAN_IT_FMP1,ENABLE);  // CAN1
		CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE); 
}

/**********************************************************************************************************
*函 数 名: CAN1_RX0_IRQHandler
*功能说明: can1接收轮毂电机反馈
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
CanRxMsg rx_message_1;
void CAN1_RX0_IRQHandler()
{
	if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET) 
	{
		CAN_Receive(CAN1, CAN_FIFO0, &rx_message_1);
		Can1Receive0(rx_message_1);
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
	}
}
/**********************************************************************************************************
*函 数 名: CAN1_RX1_IRQHandler
*功能说明: can1接收舵向中断
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
//void CAN1_RX1_IRQHandler()
//{
//	if (CAN_GetITStatus(CAN1,CAN_IT_FMP1)!= RESET) 
//	{
//		CAN_Receive(CAN1, CAN_FIFO1, &rx_message_1);
//		//Can1Receive1(rx_message_1);
//		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP1);
//	}
//}
