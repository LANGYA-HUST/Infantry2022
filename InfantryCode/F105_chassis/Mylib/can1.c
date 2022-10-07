/**********************************************************************************************************
 * @�ļ�     can1.c
 * @˵��     can1��ʼ��
 * @�汾  	 V1.0
 * @����     ��־��
 * @����     2021.4
**********************************************************************************************************/
/*CAN1���Ƶ��̣�ID�����£�
			ͷ
			|
	0x203	|   0x204
			|
��������������������������������������������������������
			|
	0x201	|   0x202
			|
***************************************************************************************/
#include "main.h"
/**********************************************************************************************************
*�� �� ��: CAN1_Configuration
*����˵��: can1��ʼ��
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void CAN1_Configuration()
{
		GPIO_InitTypeDef GPIO_InitStructure;
		CAN_InitTypeDef        CAN_InitStructure;
		CAN_FilterInitTypeDef  CAN_FilterInitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		/* ��GPIOʱ�ӡ�AFIOʱ�ӣ�CANʱ�� */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);


		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);	//CAN_RX

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	   
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);    //CAN_TX

		/* CAN1 Enabling interrupt */									  
		NVIC_InitStructure.NVIC_IRQChannel=CAN1_RX0_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
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
		CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
		CAN_InitStructure.CAN_BS1=CAN_BS1_5tq;  
		CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;	
		CAN_InitStructure.CAN_Prescaler=4;


		CAN_Init(CAN1,&CAN_InitStructure);	// CAN1											

		CAN_FilterInitStructure.CAN_FilterNumber=0;	 
		CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;	 // ��ʶ������λģʽ
		CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_16bit;   // 32λ������
		CAN_FilterInitStructure.CAN_FilterIdHigh=0x201 << 5;		// ��������ʶ��
		CAN_FilterInitStructure.CAN_FilterIdLow=0x202 << 5;		
		CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x203 << 5;		// ���������α�ʶ��
		CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x204 << 5;
		CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;	 // FIFO0ָ�������
		CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
		CAN_FilterInit(&CAN_FilterInitStructure);
		CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);  // CAN1
		
//		CAN_FilterInitStructure.CAN_FilterNumber=0;	 
//		CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	 // ��ʶ������λģʽ
//		CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;   // 32λ������
//		CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;			// ��������ʶ��
//		CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;				
//		CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;		// ���������α�ʶ��
//		CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
//		CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;	 // FIFO0ָ�������
//		CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
//		CAN_FilterInit(&CAN_FilterInitStructure);
//		CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);  // CAN1
}

/**********************************************************************************************************
*�� �� ��: CAN1_RX0_IRQHandler
*����˵��: can1�����ж�
*��    ��: ��
*�� �� ֵ: ��
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


