/**********************************************************************************************************
 * @文件     can2.c
 * @说明     can2初始化
 * @版本  	 V1.0
 * @作者     陈志鹏
 * @日期     2021.4
**********************************************************************************************************/
#include "main.h"
/**********************************************************************************************************
*函 数 名: CAN2_Configuration
*功能说明: can2初始化
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void CAN2_Configuration()
{
		GPIO_InitTypeDef GPIO_InitStructure;
		CAN_InitTypeDef        CAN_InitStructure;
		CAN_FilterInitTypeDef  CAN_FilterInitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		/* 打开GPIO时钟、AFIO时钟，CAN时钟 */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

		/* CAN2 RX PB12 */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		/* CAN2 TX PB13 */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);								

		/* CAN2_RX0 Enabling interrupt */								 	  
//		NVIC_InitStructure.NVIC_IRQChannel=CAN2_RX0_IRQn;	// FIFO_0
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//		NVIC_Init(&NVIC_InitStructure);			
		
		/* CAN2_RX1 Enabling interrupt */								 	  
		NVIC_InitStructure.NVIC_IRQChannel=CAN2_RX1_IRQn;	// FIFO_1
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);				

//	NVIC_InitStructure.NVIC_IRQChannel = CAN2_TX_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure); 


		/* CAN  BaudRate = RCC_APB1PeriphClock/(CAN_SJW+CAN_BS1+CAN_BS2)/CAN_Prescaler */
		CAN_DeInit(CAN2);
		CAN_StructInit(&CAN_InitStructure);   

//可用
		CAN_InitStructure.CAN_TTCM=DISABLE;
		CAN_InitStructure.CAN_ABOM=DISABLE;
		CAN_InitStructure.CAN_AWUM=DISABLE;
		CAN_InitStructure.CAN_NART=DISABLE;
		CAN_InitStructure.CAN_RFLM=DISABLE;
		CAN_InitStructure.CAN_TXFP=DISABLE;
		
//		CAN_InitStructure.CAN_TTCM=DISABLE;
//		CAN_InitStructure.CAN_ABOM=ENABLE;
//		CAN_InitStructure.CAN_AWUM=ENABLE;
//		CAN_InitStructure.CAN_NART=ENABLE;
//		CAN_InitStructure.CAN_RFLM=DISABLE;
//		CAN_InitStructure.CAN_TXFP=ENABLE;
		
		CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;   
		//CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;
		CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
		CAN_InitStructure.CAN_BS1=CAN_BS1_5tq;  
		CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;	
		CAN_InitStructure.CAN_Prescaler=4;

		CAN_Init(CAN2,&CAN_InitStructure);   // CAN2		

//		CAN_FilterInitStructure.CAN_FilterNumber=15;	 			//选择过滤器15
//		CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;	 // 标识符屏蔽位模式
//		CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;   // 32位过滤器
//		CAN_FilterInitStructure.CAN_FilterIdHigh=0x102 << 5;		// 过滤器标识符
//		CAN_FilterInitStructure.CAN_FilterIdLow=0 | CAN_ID_STD;		
//		CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0 | CAN_ID_STD;		// 过滤器屏蔽标识符  需要屏蔽吗？
//		CAN_FilterInitStructure.CAN_FilterMaskIdLow=0 | CAN_ID_STD;
//		CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;	 // FIFO0指向过滤器
//		CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
//		CAN_FilterInit(&CAN_FilterInitStructure);
//		CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);  // CAN2 FIFO0

//		CAN_FilterInitStructure.CAN_FilterNumber=14;	// 
//		CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;	 // 标识符列表模式
//		CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;   // 16位过滤器
//		CAN_FilterInitStructure.CAN_FilterIdHigh=0x101 << 5;			// 过滤器标识符
//		CAN_FilterInitStructure.CAN_FilterIdLow=0 | CAN_ID_STD;				
//		CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0 | CAN_ID_STD;		
//		CAN_FilterInitStructure.CAN_FilterMaskIdLow=0 | CAN_ID_STD;
//		CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO1;	 // FIFO1指向过滤器
//		CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
//		CAN_FilterInit(&CAN_FilterInitStructure);
//		CAN_ITConfig(CAN2,CAN_IT_FMP1,ENABLE);  // CAN2 FIFO1
		
		CAN_FilterInitStructure.CAN_FilterNumber=14;	// 
		CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;	 // 标识符列表模式
		CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;   // 16位过滤器
		CAN_FilterInitStructure.CAN_FilterIdHigh=0x101 << 5;			// 过滤器标识符
		CAN_FilterInitStructure.CAN_FilterIdLow=0 | CAN_ID_STD;				
		CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x102 << 5;		
		CAN_FilterInitStructure.CAN_FilterMaskIdLow=0 | CAN_ID_STD;
		CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO1;	 // FIFO1指向过滤器
		CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
		CAN_FilterInit(&CAN_FilterInitStructure);
		CAN_ITConfig(CAN2,CAN_IT_FMP1,ENABLE);  // CAN2 FIFO1
		
//			CAN_ITConfig(CAN2,CAN_IT_TME,ENABLE);


// 可用
//		CAN_FilterInitStructure.CAN_FilterNumber=14;	// 
//		CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	 // 标识符屏蔽位模式
//		CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;   // 32位过滤器
//		CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;			// 过滤器标识符
//		CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;				
//		CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;		// 过滤器屏蔽标识符
//		CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
//		CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO1;	 // FIFO1指向过滤器
//		CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
//		CAN_FilterInit(&CAN_FilterInitStructure);
//		CAN_ITConfig(CAN2,CAN_IT_FMP1,ENABLE);  // CAN2 FIFO1
}

/**********************************************************************************************************
*函 数 名: CAN2_RX0_IRQHandler
*功能说明: can2接收中断 超级电容信息
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
//CanRxMsg rx_message_0;
//void CAN2_RX0_IRQHandler()
//{
//	if (CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET) 
//	{
//		CAN_Receive(CAN2, CAN_FIFO0, &rx_message_0);
//  	Can2Receive0(&rx_message_0);
//		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
//	}
//}

//CanRxMsg rx_message_1;
//void CAN2_RX0_IRQHandler()
//{
//	if (CAN_GetITStatus(CAN2,CAN_IT_FMP1)!= RESET) 
//	{
//		CAN_Receive(CAN2, CAN_FIFO1, &rx_message_1);
//  	Can2Receive1(&rx_message_1);
//		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP1);
//	}
//}

/**********************************************************************************************************
*函 数 名: CAN2_RX1_IRQHandler
*功能说明: can2接收中断 xyw向速度信息
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
CanRxMsg rx_message_2;
void CAN2_RX1_IRQHandler()
{
	if (CAN_GetITStatus(CAN2,CAN_IT_FMP1)!= RESET) 
	{
		CAN_Receive(CAN2, CAN_FIFO1, &rx_message_2);
  	Can2Receive1(&rx_message_2);
		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP1);
	}
}

//void CAN2_TX_IRQHandler(void)
//{
//	if (CAN_GetITStatus(CAN2,CAN_IT_TME)!= RESET) 
//	{
//		CAN_ClearITPendingBit(CAN2,CAN_IT_TME);
//	}
//}
