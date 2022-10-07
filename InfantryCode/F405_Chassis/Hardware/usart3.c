/**********************************************************************************************************
 * @文件     usart3.c
 * @说明     usart3遥控器
 * @版本  	 V1.0
 * @作者     郭嘉豪
 * @日期     2021.11
**********************************************************************************************************/
#include "main.h"
extern RC_Ctl_t RC_Ctl; 
volatile unsigned char sbus_rx_buffer[18];
/**********************************************************************************************************
*函 数 名: USART3_Configuration
*功能说明: 遥控器串口初始化
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void USART3_Configuration(void)
{
USART_InitTypeDef usart;
		GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;
	
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); 
	
		gpio.GPIO_Pin = GPIO_Pin_11;
		gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOB,&gpio);

		usart.USART_BaudRate = 100000;
		usart.USART_WordLength = USART_WordLength_8b;
		usart.USART_StopBits = USART_StopBits_1;
		usart.USART_Parity = USART_Parity_Even;
		usart.USART_Mode = USART_Mode_Rx;
    usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_Init(USART3,&usart);

    USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);
		USART_Cmd(USART3,ENABLE);
    USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);	

    nvic.NVIC_IRQChannel = USART3_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 2;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
		
		{
			DMA_InitTypeDef   dma;
 			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
			DMA_DeInit(DMA1_Stream1);
			dma.DMA_Channel= DMA_Channel_4;
			dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
			dma.DMA_Memory0BaseAddr = (uint32_t)sbus_rx_buffer;
			dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
			dma.DMA_BufferSize = RX_USART3_BUFFER;
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
			DMA_Init(DMA1_Stream1,&dma);
			DMA_ITConfig(DMA1_Stream1,DMA_IT_TC,ENABLE);
			DMA_Cmd(DMA1_Stream1,ENABLE);
		}
}

/**********************************************************************************************************
*函 数 名: USART3_IRQHandler
*功能说明: USART3遥控器数据闲时中断
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void USART3_IRQHandler(void)
{
	static int DATA_LENGTH=0;
	if (USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{
    (void)USART3->SR;
		(void)USART3->DR;	
	  DMA_Cmd(DMA1_Stream1,DISABLE);
	  DATA_LENGTH=RX_USART3_BUFFER-DMA_GetCurrDataCounter(DMA1_Stream1);
		if(DATA_LENGTH==18)
		{
			RemoteReceive(sbus_rx_buffer);//解码函数
		}
		DMA_SetCurrDataCounter(DMA1_Stream1,RX_USART3_BUFFER);	
		DMA_Cmd(DMA1_Stream1,ENABLE);
  }
}

