/**********************************************************************************************************
 * @文件     usart2.c
 * @说明     usart2初始化,DataScope 波特率9600
 * @版本  	 V1.0
 * @作者     黄志雄
 * @日期     2020.1
**********************************************************************************************************/
#include "usart2.h"

//unsigned char DataScopeSend_Buf[DataScopeSend_BufSize];

/**********************************************************************************************************
*函 数 名: USART2_Configuration
*功能说明: USART2初始化 用于DataScope)
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
//void USART2_Configuration(void)
//{
//	

//	USART_InitTypeDef usart2;
//	GPIO_InitTypeDef  gpio;
//	NVIC_InitTypeDef  nvic;
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); 

//	gpio.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
//	gpio.GPIO_Mode = GPIO_Mode_AF;
//	gpio.GPIO_OType = GPIO_OType_PP;
//	gpio.GPIO_Speed = GPIO_Speed_100MHz;
//	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOA,&gpio);

//	usart2.USART_BaudRate = 115200;
//	usart2.USART_WordLength = USART_WordLength_8b;
//	usart2.USART_StopBits = USART_StopBits_1;
//	usart2.USART_Parity = USART_Parity_No;
//	usart2.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
//	usart2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_Init(USART2,&usart2);
//	
//	USART_Cmd(USART2,ENABLE);
//	nvic.NVIC_IRQChannel = USART2_IRQn;
//	nvic.NVIC_IRQChannelPreemptionPriority = 1;
//	nvic.NVIC_IRQChannelSubPriority = 0;
//	nvic.NVIC_IRQChannelCmd = ENABLE;		
//	NVIC_Init(&nvic);
//}



unsigned char tof_rx_buffer[RXTOF_USART2_BUFFER];
unsigned char tof_tx_buffer[TXTOF_USART2_BUFFER] = {0x50,0x06,0x00,0x00,0x00,0x01,0x45,0x8b};
int Flag_Tx_Gsm_Busy=0;
void USART2_Configuration(void)
{

	GPIO_InitTypeDef   GPIO_InitStructure;   //初始化GPIO结构体
  USART_InitTypeDef  USART_InitStructure;  //初始化Usart结构体
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);   //使能GPIOA的时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);   //使能USART2的时钟
	
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
	
	/* 配置USART2的GPIO */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
 
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* 设置USART2的模式 */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);
  
	/* 打开USART2的中断  当接收接收寄存器不空时产生中断 */
  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
 
  USART_Cmd(USART2, ENABLE);  //使能USART2

	DMA_InitTypeDef DMA_InitStructure;
 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);	//开启DMA时钟
		
		
		NVIC_InitTypeDef NVIC_InitStructure;
     /* 配置P[A|B|C|D|E]0为中断源 */
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

 
	/* 设置DMA的传输通道：DMA_Channel_4  */
    DMA_InitStructure.DMA_Channel = DMA_Channel_4; 
	
 	/* 设置DMA源：内存地址&串口数据寄存器地址 */
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART2_DR_Base;	   
 
	/* 内存地址(要传输的变量的指针) */
    DMA_InitStructure.DMA_Memory0BaseAddr = (u32)SendBuff;
	
	/* 方向：从内存到外设 */		
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;	
	
	/* 传输大小DMA_BufferSize=SENDBUFF_SIZE */	
    DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;
	
	/* 外设地址不增 */	    
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	
	/* 内存地址自增 */
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
	
	/* 外设数据单位 */	
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	
	/* 内存数据单位 8bit */
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 
	
	/* DMA模式：一次传输，循环 */
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;	 
	
	/* 优先级：中 */	
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;  
	
	/* 指定如果FIFO模式或直接模式将用于指定的流 ： 不使能FIFO模式*/
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; 
 
	/* 指定了FIFO阈值水平 */
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
		
	/* 指定的Burst转移配置内存传输 */
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
		
	/* 指定的Burst转移配置外围转移 */
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	/* 配置DMA1的4通道 */		   
    DMA_Init(DMA1_Stream6, &DMA_InitStructure); 	   
	
	/* Enable DMA */
    DMA_Cmd(DMA1_Stream6,ENABLE);	
 
	/* 配置DMA发送完成后产生中断 */ 
    DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);  
		

			
}


/*******************************************************************************
 * 函数名：USART2_IRQHandler
 * 描述  ：USART2的接收中断函数 
 * 输入  ：无
 * 输出  ：无
 * 批注  ：廖旭-2014年1月14日 21:41:04
*******************************************************************************/
void USART2_IRQHandler(void)
{
//	uint8_t data;
	if(USART_GetFlagStatus(USART2,USART_FLAG_RXNE) != RESET)
	{
		USART_ReceiveData(USART2);
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}
}


/*******************************************************************************
 * 函数名：DMA1_Channel4_IRQHandler
 * 描述  ：DMA2发送完成中断函数 
 * 输入  ：无
 * 输出  ：无
 * 批注  ：廖旭-2014年1月18日 15:43:43
*******************************************************************************/
void DMA1_Stream6_IRQHandler(void)
 
{	
	/* 判断是否为DMA发送完成中断 */
   if(DMA_GetITStatus(DMA1_Stream6,DMA_IT_TCIF6) != RESET)
   {   
	 /*清除标志位*/
		DMA_ClearITPendingBit(DMA1_Stream6,DMA_IT_TCIF6);
 
	}	
}
