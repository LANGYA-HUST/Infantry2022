/**********************************************************************************************************
 * @文件     usart4.c
 * @说明     uart初始化,VOFA
 * @版本  	 V1.0
**********************************************************************************************************/
#include "main.h"

//unsigned char DataScopeSend_Buf[DataScopeSend_BufSize];
#define	VOFA_MAX_CHANNEL 10
float 	VOFA_justfloat[VOFA_MAX_CHANNEL];
uint8_t VOFA_send_Data[VOFA_MAX_CHANNEL*4+4];
DMA_InitTypeDef   dma;
extern float INA_Power;
/**********************************************************************************************************
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void VOFA_USART_Configuration(void)
{
	USART_InitTypeDef usart;
	GPIO_InitTypeDef  gpio;
	NVIC_InitTypeDef  nvic;
	
			//写入结尾数据
	VOFA_send_Data[VOFA_MAX_CHANNEL*4] = 0x00;                    
	VOFA_send_Data[VOFA_MAX_CHANNEL*4+1] = 0x00;
	VOFA_send_Data[VOFA_MAX_CHANNEL*4+2] = 0x80;
	VOFA_send_Data[VOFA_MAX_CHANNEL*4+3] = 0x7f;
	
	nvic.NVIC_IRQChannel = VOFA_DMA_TX_IRQ;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	
		{
			VOFA_DMA_TX_AHBxClock_FUN(VOFA_DMA_TX_CLK,ENABLE);
			DMA_DeInit(VOFA_DMA_TX_STREAM);
			dma.DMA_Channel= VOFA_DMA_TX_CHANNEL;
			dma.DMA_PeripheralBaseAddr = (uint32_t)&(VOFA_USARTx->DR);
			dma.DMA_Memory0BaseAddr = (uint32_t)VOFA_send_Data;
			dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;
			dma.DMA_BufferSize = (VOFA_MAX_CHANNEL*4+4);
			dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
			dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
			dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
			dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
			dma.DMA_Mode = DMA_Mode_Normal;
			dma.DMA_Priority = DMA_Priority_VeryHigh;
			dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
			dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
			dma.DMA_MemoryBurst = DMA_Mode_Normal;
			dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		
			DMA_Init(VOFA_DMA_TX_STREAM,&dma);
		  DMA_ClearFlag(VOFA_DMA_TX_STREAM, VOFA_DMA_FLAG_TCIF);  // clear all DMA flags
			DMA_ITConfig(VOFA_DMA_TX_STREAM,DMA_IT_TC,ENABLE);  //open DMA send inttrupt
			DMA_Cmd(VOFA_DMA_TX_STREAM,DISABLE);
		}	
	
	VOFA_USART_GPIO_APBxClock_FUN(VOFA_USART_GPIO_CLK,ENABLE);
	VOFA_USART_APBxClock_FUN(VOFA_USART_CLK,ENABLE);

	GPIO_PinAFConfig(VOFA_USART_TX_PORT,VOFA_USART_TX_SOURCE,VOFA_USART_TX_AF);
	GPIO_PinAFConfig(VOFA_USART_RX_PORT,VOFA_USART_RX_SOURCE,VOFA_USART_RX_AF); 

	gpio.GPIO_Pin = VOFA_USART_TX_PIN;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(VOFA_USART_TX_PORT,&gpio);
	
	gpio.GPIO_Pin = VOFA_USART_RX_PIN;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(VOFA_USART_RX_PORT,&gpio);

	usart.USART_BaudRate = VOFA_USART_BAUD_RATE;
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No;
	usart.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(VOFA_USARTx,&usart);
	
	USART_Cmd(VOFA_USARTx,ENABLE);
	
	USART_DMACmd(VOFA_USARTx,USART_DMAReq_Tx,ENABLE);	

	DMA_Cmd(VOFA_DMA_TX_STREAM,ENABLE);
}
void VOFA_DMA_TX_INT_FUN(void)
{
	if(DMA_GetITStatus(VOFA_DMA_TX_STREAM,VOFA_DMA_IT_STATUS  ))
	{
		DMA_ClearITPendingBit(VOFA_DMA_TX_STREAM,VOFA_DMA_IT_STATUS);
		DMA_Cmd(VOFA_DMA_TX_STREAM, DISABLE); 
  }
}

//将要输出的变量声明extern
extern JudgeReceive_t JudgeReceive;
extern float K_Power;
void VOFA_Send(void)
{
	//需转为float型数据存储
	VOFA_justfloat[0]=(float)(JudgeReceive.realChassispower);
	VOFA_justfloat[1]=(float)(K_Power);
//	VOFA_justfloat[2]=(float)(FuzzyAidPidYawPos.ActPoint);
//	VOFA_justfloat[3]=(float)(FuzzyAidPidYawPos.SetPoint);
//	VOFA_justfloat[4]=(float)(GyroReceive.GX);
//	VOFA_justfloat[5]=(float)(GyroReceive.ROLL);

	//拷贝到传输变量
	memcpy(VOFA_send_Data, (uint8_t *)VOFA_justfloat, sizeof(VOFA_justfloat));
	
	VOFA_DMA_TX_STREAM->NDTR = VOFA_MAX_CHANNEL*4+4; 
  DMA_Cmd(VOFA_DMA_TX_STREAM, ENABLE);   

}



