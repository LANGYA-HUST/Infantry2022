/**********************************************************************************************************
 * @文件     uart4.c
 * @说明     uart4初始化(裁判系统通信)
 * @版本  	 V1.0
 * @作者     陈志鹏
 * @日期     2020.3
**********************************************************************************************************/
#include "uart4.h"

unsigned char JudgeReceiveBuffer[JudgeBufBiggestSize];
unsigned char JudgeSend[SEND_MAX_SIZE];
/**********************************************************************************************************
*函 数 名: UART4_Configuration
*功能说明: uart4初始化(裁判系统通信)
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void UART4_Configuration(void)
{
	USART_InitTypeDef usart;
	GPIO_InitTypeDef  gpio;
	NVIC_InitTypeDef  nvic;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC| RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	
	gpio.GPIO_Pin = GPIO_Pin_11;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC,&gpio);

	gpio.GPIO_Pin = GPIO_Pin_10;  
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&gpio);

	USART_DeInit(UART4);
	usart.USART_BaudRate = 115200;
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No ;
	usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
	USART_Init(UART4,&usart);

	USART_Cmd(UART4,ENABLE);
	USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE); 
	
	nvic.NVIC_IRQChannel = DMA2_Channel3_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	
	{
		DMA_InitTypeDef  dma;
		
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
		                               // RX
		DMA_DeInit(DMA2_Channel3);

		dma.DMA_PeripheralBaseAddr = (uint32_t)&(UART4->DR);
		dma.DMA_MemoryBaseAddr = (uint32_t)JudgeReceiveBuffer;
		dma.DMA_DIR = DMA_DIR_PeripheralSRC;
		dma.DMA_BufferSize = JudgeBufBiggestSize;
		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		dma.DMA_Mode = DMA_Mode_Circular;
		dma.DMA_Priority = DMA_Priority_VeryHigh;
		dma.DMA_M2M = DMA_M2M_Disable;
		
		DMA_Init(DMA2_Channel3, &dma);
		DMA_ITConfig(DMA2_Channel3,DMA_IT_TC,ENABLE);
		DMA_Cmd(DMA2_Channel3, ENABLE);
	}
  
    nvic.NVIC_IRQChannel = DMA2_Channel5_IRQn;
		nvic.NVIC_IRQChannelPreemptionPriority = 1;
		nvic.NVIC_IRQChannelSubPriority = 1;
		nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvic);
		USART_DMACmd(UART4,USART_DMAReq_Tx,ENABLE);
		{                                      //TX
		  DMA_InitTypeDef 	dma;
			DMA_DeInit(DMA2_Channel5);
			dma.DMA_PeripheralBaseAddr = (uint32_t)&(UART4->DR);
			dma.DMA_MemoryBaseAddr = (uint32_t)JudgeSend;
			dma.DMA_DIR = DMA_DIR_PeripheralDST;
			dma.DMA_BufferSize = SEND_MAX_SIZE;
			dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
			dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
			dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
			dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
			dma.DMA_Mode = DMA_Mode_Normal;
			dma.DMA_Priority = DMA_Priority_VeryHigh;
			dma.DMA_M2M = DMA_M2M_Disable;

			DMA_Init(DMA2_Channel5,&dma);
			DMA_ITConfig(DMA2_Channel5,DMA_IT_TC,ENABLE);
			DMA_Cmd(DMA2_Channel5,DISABLE);
	 }		
}

void DMA2_Channel3_IRQHandler(void)
{	
	if(DMA_GetFlagStatus(DMA2_FLAG_TC3) == SET)
	{
		DMA_ClearFlag(DMA2_FLAG_TC3);
    JudgeBuffReceive(JudgeReceiveBuffer,0); 
	}
}

volatile char dma_tx_busy=0;
void DMA2_Channel5_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA2_IT_TC5)!=RESET)
	{
		DMA_ClearFlag(DMA2_FLAG_TC5);
		DMA_ClearITPendingBit(DMA2_IT_TC5);
		DMA_Cmd(DMA2_Channel5,DISABLE);
		dma_tx_busy=0;
	}	
}
