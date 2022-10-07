/**********************************************************************************************************
 * @文件     uart4.c
 * @说明     uart4 f4初始化(裁判系统通信)
 * @版本  	 V2.0
 * @作者     郭嘉豪
 * @日期     2022.3
**********************************************************************************************************/
#include "main.h"

unsigned char JudgeReceiveBuffer[JudgeBufBiggestSize];
unsigned char JudgeSend[SEND_MAX_SIZE];
/**********************************************************************************************************
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void UART4_Configuration(void)
{
	USART_InitTypeDef usart;
	GPIO_InitTypeDef  gpio;
	NVIC_InitTypeDef  nvic;
	DMA_InitTypeDef   dma;
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);

	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4); 

	gpio.GPIO_Pin = GPIO_Pin_10;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC,&gpio);
	
	gpio.GPIO_Pin = GPIO_Pin_11;
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
	USART_Init(UART4,&usart);
	
	USART_Cmd(UART4,ENABLE);
	
	USART_DMACmd(UART4,USART_DMAReq_Rx,ENABLE);	                      //使能USART4DMA发送

	nvic.NVIC_IRQChannel = DMA1_Stream2_IRQn;
  nvic.NVIC_IRQChannelPreemptionPriority = 2;
  nvic.NVIC_IRQChannelSubPriority = 1;
  nvic.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic);

		
		{
			DMA_InitTypeDef   dma;
			/* 配置DMA1 Stream2，UART4接受 */
 			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
			DMA_DeInit(DMA1_Stream2);
			dma.DMA_Channel= DMA_Channel_4;
			dma.DMA_PeripheralBaseAddr = (uint32_t)&(UART4->DR);
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
			DMA_Init(DMA1_Stream2,&dma);
			DMA_ITConfig(DMA1_Stream2,DMA_IT_TC,ENABLE);
			DMA_Cmd(DMA1_Stream2,ENABLE);
		}
		
		USART_DMACmd(UART4,USART_DMAReq_Tx,ENABLE);                           //使能USART4DMA发送

		nvic.NVIC_IRQChannel = DMA1_Stream4_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 3;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
		
		{
			DMA_InitTypeDef   DMA_InitStructure;
			DMA_DeInit(DMA1_Stream4);
			while (DMA_GetCmdStatus(DMA1_Stream4) != DISABLE);//等待DMA可配置 
			/* 配置DMA1 Stream4，UART4发送 */
			DMA_InitStructure.DMA_Channel            = DMA_Channel_4;               //通道选择
			DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(UART4->DR);      //DMA外设地址
			DMA_InitStructure.DMA_Memory0BaseAddr    = (u32)JudgeSend;      //DMA 存储器0地址
			DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;  //存储器到外设模式
			DMA_InitStructure.DMA_BufferSize         = SEND_MAX_SIZE;       //数据传输量 
			DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;   //外设非增量模式
			DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;        //存储器增量模式
			DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据长度:8位
			DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;     //存储器数据长度:8位
			DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;             //使用普通模式 
			DMA_InitStructure.DMA_Priority           = DMA_Priority_High;         //中等优先级
			DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;         
			DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_1QuarterFull;
			DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;      //存储器突发单次传输
			DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;  //外设突发单次传输
			DMA_Init(DMA1_Stream4, &DMA_InitStructure);                             //初始化DMA Stream4

			DMA_ITConfig(DMA1_Stream4,DMA_IT_TC,ENABLE);				//DMA2传输完成中断
			DMA_Cmd(DMA1_Stream4, DISABLE);											//不使能
		
		}
}
///**********************************************************************************************************
//*函 数 名: DMA1_Stream_IRQHandler
//*功能说明: usart2 DMA接收中断
//*形    参: 无
//*返 回 值: 无
//**********************************************************************************************************/
uint8_t JudgeReveice_Flag;
void DMA1_Stream2_IRQHandler(void)
{	
	if(DMA_GetITStatus(DMA1_Stream2, DMA_IT_TCIF2))
	{ 
		JudgeReveice_Flag = 1;
		DMA_ClearITPendingBit(DMA1_Stream2, DMA_IT_TCIF2);
	}
}
///**********************************************************************************************************
//*函 数 名: DMA1_Stream4_IRQHandler
//*功能说明: usart2 DMA发送中断
//*形    参: 无
//*返 回 值: 无
//**********************************************************************************************************/
void DMA1_Stream4_IRQHandler(void)
{	
	if(DMA_GetITStatus(DMA1_Stream4, DMA_IT_TCIF4))
	{
		DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4);
		DMA_ClearITPendingBit(DMA1_Stream4, DMA_IT_TCIF4);
		DMA_Cmd(DMA1_Stream4, DISABLE);			
	}
}



