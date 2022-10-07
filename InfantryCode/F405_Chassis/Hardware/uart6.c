/**********************************************************************************************************
 * @�ļ�     uart6.c
 * @˵��     uart6��ʼ��(����ϵͳͨ��)
 * @�汾  	 V1.0
 * @����     ���κ�
 * @����     2021.10
**********************************************************************************************************/
#include "uart6.h"

unsigned char JudgeReceiveBuffer[JudgeBufBiggestSize];
unsigned char JudgeSend[SEND_MAX_SIZE];
/**********************************************************************************************************
*�� �� ��: UART6_Configuration
*����˵��: uart6��ʼ��(����ϵͳͨ��)
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void UART6_Configuration(void)
{
	USART_InitTypeDef usart;
	GPIO_InitTypeDef  gpio;
	NVIC_InitTypeDef  nvic;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6); 

	gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC,&gpio);

	usart.USART_BaudRate = 115200;
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No;
	usart.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART6,&usart);

	USART_ITConfig(USART6,USART_IT_IDLE,ENABLE);
	USART_Cmd(USART6,ENABLE);
	
	USART_DMACmd(USART6,USART_DMAReq_Rx,ENABLE);	
	USART_DMACmd(USART6,USART_DMAReq_Tx,ENABLE);
	
	nvic.NVIC_IRQChannel = DMA2_Stream2_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	
	{
		DMA_InitTypeDef   dma;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
		DMA_DeInit(DMA2_Stream2);
		dma.DMA_Channel= DMA_Channel_5;
		dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART6->DR);
		dma.DMA_Memory0BaseAddr = (uint32_t)JudgeReceiveBuffer;
		dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
		dma.DMA_BufferSize = JudgeBufBiggestSize;
		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		dma.DMA_Mode = DMA_Mode_Circular;
		dma.DMA_Priority = DMA_Priority_VeryHigh;
		dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
		dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
		dma.DMA_MemoryBurst = DMA_Mode_Normal;
		dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(DMA2_Stream2,&dma);
		DMA_ITConfig(DMA2_Stream2,DMA_IT_TC,ENABLE);
		DMA_Cmd(DMA2_Stream2,ENABLE);
	}
  
    nvic.NVIC_IRQChannel = DMA2_Stream7_IRQn;
		nvic.NVIC_IRQChannelPreemptionPriority = 3;
		nvic.NVIC_IRQChannelSubPriority = 3;
		nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvic);
		USART_DMACmd(UART4,USART_DMAReq_Tx,ENABLE);
	{
		DMA_InitTypeDef   dma;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
		DMA_DeInit(DMA2_Stream7);
		dma.DMA_Channel= DMA_Channel_5;
		dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART6->DR);
		dma.DMA_Memory0BaseAddr = (uint32_t)JudgeSend;
		dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;
		dma.DMA_BufferSize = 30;
		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		dma.DMA_Mode = DMA_Mode_Circular;
		dma.DMA_Priority = DMA_Priority_VeryHigh;
		dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
		dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
		dma.DMA_MemoryBurst = DMA_Mode_Normal;
		dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(DMA2_Stream7,&dma);
		DMA_Cmd(DMA2_Stream7,DISABLE);
	}		
}

/**********************************************************************************************************
*�� �� ��: DMA2_Stream2_IRQHandler
*����˵��: usart6 DMA�����ж�
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void DMA2_Stream2_IRQHandler
(void)
{	
	if(DMA_GetITStatus(DMA2_Stream2, DMA_IT_TCIF2))
	{
    JudgeBuffReceive(JudgeReceiveBuffer,0); 
		DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);
		DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TCIF2);
	}
}
/**********************************************************************************************************
*�� �� ��: DMA2_Stream7_IRQn
*����˵��: usart6 DMA�����ж�
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void DMA2_Stream7_IRQHandler(void)
{
	//�����־
	if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)!=RESET)//�ȴ�DMA2_Steam7�������
	{
		DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7); //���DMA2_Steam7������ɱ�־
		DMA_ClearITPendingBit(DMA2_Stream7, DMA_FLAG_TCIF7);
   	DMA_Cmd(DMA2_Stream7,DISABLE);
	}
}
