/**********************************************************************************************************
 * @文件     usart2.c
 * @说明     usart2初始化：PC通信
 * @版本  	 V1.0
 * @作者     戴军
 * @日期     2022.1
**********************************************************************************************************/
#include "main.h"

unsigned char PCbuffer[PC_RECVBUF_SIZE]={0,0,0};
extern unsigned char SendToPC_Buff[PC_SENDBUF_SIZE];

/**********************************************************************************************************
*函 数 名: USART2_Configuration
*功能说明: usart2配置函数(PC通信)
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void USART2_Configuration(void)
{
    USART_InitTypeDef usart2;
		GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;
	
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); 
	
    gpio.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
		gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA,&gpio);

		usart2.USART_BaudRate = 115200;
		usart2.USART_WordLength = USART_WordLength_8b;
		usart2.USART_StopBits = USART_StopBits_1;
		usart2.USART_Parity = USART_Parity_No;
		usart2.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
    usart2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_Init(USART2,&usart2);
		
		USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
		USART_Cmd(USART2,ENABLE);
		
		USART_Cmd(USART2,ENABLE);
    USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);	
		USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
		
    nvic.NVIC_IRQChannel = DMA1_Stream5_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
		
		{                              // RX
			DMA_InitTypeDef   dma;
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
			DMA_DeInit(DMA1_Stream5);
			dma.DMA_Channel= DMA_Channel_4;
			dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
			dma.DMA_Memory0BaseAddr = (uint32_t)PCbuffer;
			dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
			dma.DMA_BufferSize = PC_RECVBUF_SIZE;
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
			DMA_Init(DMA1_Stream5,&dma);
			DMA_ITConfig(DMA1_Stream5,DMA_IT_TC,ENABLE);
			DMA_Cmd(DMA1_Stream5,ENABLE);
		}
		
		{                             //  TX
			DMA_InitTypeDef   dma;
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
			DMA_DeInit(DMA1_Stream6);
			dma.DMA_Channel= DMA_Channel_4;
			dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
			dma.DMA_Memory0BaseAddr = (uint32_t)SendToPC_Buff;
			dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;
			dma.DMA_BufferSize = PC_SENDBUF_SIZE;
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
			DMA_Init(DMA1_Stream6,&dma);
			DMA_Cmd(DMA1_Stream6,DISABLE);
		}	
}
/**********************************************************************************************************
*函 数 名: DMA2_Stream2_IRQHandler
*功能说明: usart6 DMA接收中断
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/

extern TaskHandle_t DataReceiveTask_Handler; //任务句柄
uint8_t PC_ReceiveFlag;

/**********************************************普通接收****************************************************************/
void DMA1_Stream5_IRQHandler(void)
{
//	static BaseType_t pxHigherTaskToWoken = pdFALSE;
	if(DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5))
	{
		PC_ReceiveFlag = 1;
//		vTaskNotifyGiveFromISR(DataReceiveTask_Handler,&pxHigherTaskToWoken);
		DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5);
		DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);
  }
}
