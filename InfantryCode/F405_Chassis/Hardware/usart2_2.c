/**********************************************************************************************************
 * @�ļ�     usart2.c
 * @˵��     usart2��ʼ��,DataScope ������9600
 * @�汾  	 V1.0
 * @����     ��־��
 * @����     2020.1
**********************************************************************************************************/
#include "usart2.h"

//unsigned char DataScopeSend_Buf[DataScopeSend_BufSize];

/**********************************************************************************************************
*�� �� ��: USART2_Configuration
*����˵��: USART2��ʼ�� ����DataScope)
*��    ��: ��
*�� �� ֵ: ��
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

	GPIO_InitTypeDef   GPIO_InitStructure;   //��ʼ��GPIO�ṹ��
  USART_InitTypeDef  USART_InitStructure;  //��ʼ��Usart�ṹ��
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);   //ʹ��GPIOA��ʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);   //ʹ��USART2��ʱ��
	
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
	
	/* ����USART2��GPIO */
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
	
	/* ����USART2��ģʽ */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);
  
	/* ��USART2���ж�  �����ս��ռĴ�������ʱ�����ж� */
  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
 
  USART_Cmd(USART2, ENABLE);  //ʹ��USART2

	DMA_InitTypeDef DMA_InitStructure;
 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);	//����DMAʱ��
		
		
		NVIC_InitTypeDef NVIC_InitStructure;
     /* ����P[A|B|C|D|E]0Ϊ�ж�Դ */
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

 
	/* ����DMA�Ĵ���ͨ����DMA_Channel_4  */
    DMA_InitStructure.DMA_Channel = DMA_Channel_4; 
	
 	/* ����DMAԴ���ڴ��ַ&�������ݼĴ�����ַ */
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART2_DR_Base;	   
 
	/* �ڴ��ַ(Ҫ����ı�����ָ��) */
    DMA_InitStructure.DMA_Memory0BaseAddr = (u32)SendBuff;
	
	/* ���򣺴��ڴ浽���� */		
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;	
	
	/* �����СDMA_BufferSize=SENDBUFF_SIZE */	
    DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;
	
	/* �����ַ���� */	    
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	
	/* �ڴ��ַ���� */
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
	
	/* �������ݵ�λ */	
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	
	/* �ڴ����ݵ�λ 8bit */
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 
	
	/* DMAģʽ��һ�δ��䣬ѭ�� */
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;	 
	
	/* ���ȼ����� */	
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;  
	
	/* ָ�����FIFOģʽ��ֱ��ģʽ������ָ������ �� ��ʹ��FIFOģʽ*/
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; 
 
	/* ָ����FIFO��ֵˮƽ */
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
		
	/* ָ����Burstת�������ڴ洫�� */
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
		
	/* ָ����Burstת��������Χת�� */
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	/* ����DMA1��4ͨ�� */		   
    DMA_Init(DMA1_Stream6, &DMA_InitStructure); 	   
	
	/* Enable DMA */
    DMA_Cmd(DMA1_Stream6,ENABLE);	
 
	/* ����DMA������ɺ�����ж� */ 
    DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);  
		

			
}


/*******************************************************************************
 * ��������USART2_IRQHandler
 * ����  ��USART2�Ľ����жϺ��� 
 * ����  ����
 * ���  ����
 * ��ע  ������-2014��1��14�� 21:41:04
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
 * ��������DMA1_Channel4_IRQHandler
 * ����  ��DMA2��������жϺ��� 
 * ����  ����
 * ���  ����
 * ��ע  ������-2014��1��18�� 15:43:43
*******************************************************************************/
void DMA1_Stream6_IRQHandler(void)
 
{	
	/* �ж��Ƿ�ΪDMA��������ж� */
   if(DMA_GetITStatus(DMA1_Stream6,DMA_IT_TCIF6) != RESET)
   {   
	 /*�����־λ*/
		DMA_ClearITPendingBit(DMA1_Stream6,DMA_IT_TCIF6);
 
	}	
}
