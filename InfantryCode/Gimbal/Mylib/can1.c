/**********************************************************************************************************
 * @�ļ�     can1.c
 * @˵��     ���ݷ���
 * @�汾  	 V1.0
 * @����     ��־��
 * @����     2019.10
**********************************************************************************************************/
/**********************************************************************************************************
 * @�ļ�     can1.c
 * @˵��     CAN��������
 * @�汾  	 V2.0
 * @����     ����
 * @����     2022.3
**********************************************************************************************************/
#include "main.h"
/**********************************************************************************************************
*�� �� ��: CAN1_Configration
*����˵��: can1���ú���
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void CAN1_Configuration(void)
{
	  CAN_InitTypeDef        can;
    CAN_FilterInitTypeDef  can_filter;
    GPIO_InitTypeDef       gpio;
    NVIC_InitTypeDef       nvic;
		
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

		GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_CAN1);

    gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    gpio.GPIO_Mode = GPIO_Mode_AF;
	  gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
	  gpio.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &gpio);
    


    nvic.NVIC_IRQChannel = CAN1_RX0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 2;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
		
		nvic.NVIC_IRQChannel = CAN1_RX1_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    nvic.NVIC_IRQChannel = CAN1_TX_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 2;
    nvic.NVIC_IRQChannelSubPriority = 2;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    CAN_DeInit(CAN1);
    CAN_StructInit(&can);
    
    can.CAN_TTCM = DISABLE;
    can.CAN_ABOM = ENABLE;
    can.CAN_AWUM = ENABLE;
    can.CAN_NART = ENABLE;
    can.CAN_RFLM = DISABLE;
    can.CAN_TXFP = ENABLE;
    can.CAN_Mode = CAN_Mode_Normal;
                                      // can.CAN_Mode =CAN_Mode_LoopBack
    can.CAN_SJW  = CAN_SJW_1tq;   //1
    can.CAN_BS1 = CAN_BS1_9tq;    // 9
    can.CAN_BS2 = CAN_BS2_4tq;		// 4
		can.CAN_Prescaler = 3;   //CAN BaudRate 42/(1+9+4)/3=1Mbps
    CAN_Init(CAN1, &can);


//    can_filter.CAN_FilterNumber=1;
//		can_filter.CAN_FilterMode=CAN_FilterMode_IdMask;
//		can_filter.CAN_FilterScale=CAN_FilterScale_32bit;
//		can_filter.CAN_FilterIdHigh=0x0000;
//		can_filter.CAN_FilterIdLow=0x0000;
//		can_filter.CAN_FilterMaskIdHigh=0x0000;
//		can_filter.CAN_FilterMaskIdLow=0x0000;
//		can_filter.CAN_FilterFIFOAssignment=1;
//		can_filter.CAN_FilterActivation=ENABLE;
//		CAN_FilterInit(&can_filter);
		
	  can_filter.CAN_FilterNumber = 0; //ѡ�������0
		can_filter.CAN_FilterMode = CAN_FilterMode_IdList; //�б�ģʽ
		can_filter.CAN_FilterScale = CAN_FilterScale_16bit;
		can_filter.CAN_FilterIdHigh =  0x096<<5;
		can_filter.CAN_FilterIdLow = 0x094<<5;
		can_filter.CAN_FilterMaskIdHigh =  0x000<<5;
		can_filter.CAN_FilterMaskIdLow = 0x000<<5;
		can_filter.CAN_FilterFIFOAssignment = 0;//fifo
		can_filter.CAN_FilterActivation = ENABLE;
		CAN_FilterInit(&can_filter);

		
		can_filter.CAN_FilterNumber = 1; //ѡ�������1
		can_filter.CAN_FilterMode = CAN_FilterMode_IdList; //�б�ģʽ
		can_filter.CAN_FilterScale = CAN_FilterScale_32bit;
		can_filter.CAN_FilterIdHigh = 0x100<<5;
		can_filter.CAN_FilterIdLow =  0 | CAN_ID_STD;
		can_filter.CAN_FilterMaskIdHigh = 0x000<<5;  
		can_filter.CAN_FilterMaskIdLow = 0 | CAN_ID_STD;
		can_filter.CAN_FilterFIFOAssignment = 1;//fifo
		can_filter.CAN_FilterActivation = ENABLE;
		CAN_FilterInit(&can_filter);
    
		CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
    CAN_ITConfig(CAN1,CAN_IT_FMP1,ENABLE);
    CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE); 
}

/**********************************************************************************************************
*�� �� ��: CAN1_TX_IRQHandler
*����˵��: can1�����ж�
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void CAN1_TX_IRQHandler(void)
{
	if (CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET) 
	{
		CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
	}
}
/**********************************************************************************************************
*�� �� ��: CAN1_RX0_IRQHandler
*����˵��: can1�����ж�()
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void CAN1_RX0_IRQHandler(void)
{
    CanRxMsg rx_message0;
    if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
		{
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
        CAN_Receive(CAN1, CAN_FIFO0, &rx_message0);
				Can1Receive0(rx_message0);
    }
}
/**********************************************************************************************************
*�� �� ��: CAN1_RX1_IRQHandler
*����˵��: can1�����ж�(���ʰ����)
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void CAN1_RX1_IRQHandler(void)
{
	  CanRxMsg rx_message1;
    if (CAN_GetITStatus(CAN1,CAN_IT_FMP1)!= RESET)
    {
			  CAN_ClearITPendingBit(CAN1, CAN_IT_FMP1);
        CAN_Receive(CAN1, CAN_FIFO1, &rx_message1);
			  Can1Receive1(rx_message1);
				IWDG_Feed();		//ι��
    }
}
