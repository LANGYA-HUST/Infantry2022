/**********************************************************************************************************
 * @文件     usart.c
 * @说明     usart初始化,VOFA
 * @版本  	 V1.0
 * @作者     戴军
 * @日期     2022.3
**********************************************************************************************************/
#include "main.h"

//unsigned char DataScopeSend_Buf[DataScopeSend_BufSize];
#define	VOFA_MAX_CHANNEL 10
float 	VOFA_justfloat[VOFA_MAX_CHANNEL];
uint8_t VOFA_send_Data[VOFA_MAX_CHANNEL*4+4];
DMA_InitTypeDef   dma;
extern float INA_Power;
/**********************************************************************************************************
*函 数 名: USART2_Configuration
*功能说明: usart初始化 用于DataScope)
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
			DMA_DeInit( VOFA_DMA_TX_CHANNEL);
			dma.DMA_PeripheralBaseAddr = (uint32_t)&(VOFA_USARTx->DR);
			dma.DMA_MemoryBaseAddr = (uint32_t)VOFA_send_Data;
			dma.DMA_DIR = DMA_DIR_PeripheralDST;
			dma.DMA_BufferSize = (VOFA_MAX_CHANNEL*4+4);
			dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
			dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
			dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
			dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
			dma.DMA_Mode = DMA_Mode_Normal;
			dma.DMA_Priority = DMA_Priority_VeryHigh;
			dma.DMA_M2M=DMA_M2M_Disable;
		
			DMA_Init(VOFA_DMA_TX_CHANNEL,&dma);
		  DMA_ClearFlag(VOFA_DMA_FLAG_TCIF);  // clear all DMA flags
			DMA_ITConfig(VOFA_DMA_TX_CHANNEL,DMA_IT_TC,ENABLE);  //open DMA send inttrupt
			DMA_Cmd(VOFA_DMA_TX_CHANNEL,DISABLE);
		}	
	
	VOFA_USART_GPIO_APBxClock_FUN(VOFA_USART_GPIO_CLK|RCC_APB2Periph_AFIO,ENABLE);
	VOFA_USART_APBxClock_FUN(VOFA_USART_CLK,ENABLE);


	gpio.GPIO_Pin = VOFA_USART_TX_PIN;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(VOFA_USART_TX_PORT,&gpio);
	
	gpio.GPIO_Pin = VOFA_USART_RX_PIN;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
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

	DMA_Cmd(VOFA_DMA_TX_CHANNEL,ENABLE);
}
void VOFA_DMA_TX_INT_FUN(void)
{
	if(DMA_GetITStatus(VOFA_DMA_IT)==SET)
	{
		DMA_ClearITPendingBit(VOFA_DMA_IT);
		DMA_Cmd(VOFA_DMA_TX_CHANNEL,DISABLE); 
  }
}

//将要输出的变量声明extern
extern JudgeReceive_t JudgeReceive;
extern RM820RReceive_Typedef ChassisMotorCanReceive[4];
extern ChassisSpeed_t chassis;
extern enum POWERSTATE_Typedef PowerState;
extern short WheelCurrentSend[4];
extern short carSpeedw;
extern float I_Set;
extern volatile TickType_t SampleTick,SampleCount;
void VOFA_Send(void)
{
	//需转为float型数据存储
	VOFA_justfloat[0]=(float)(SampleTick);
	VOFA_justfloat[1]=(float)(SampleCount);
//	VOFA_justfloat[2]=(float)(JudgeReceive.remainEnergy);
//	VOFA_justfloat[3]=(float)(I_Set);

	//拷贝到传输变量
	memcpy(VOFA_send_Data, (uint8_t *)VOFA_justfloat, sizeof(VOFA_justfloat));
	
	VOFA_DMA_TX_CHANNEL->CNDTR = VOFA_MAX_CHANNEL*4+4; 
  DMA_Cmd(VOFA_DMA_TX_CHANNEL, ENABLE);   

}



