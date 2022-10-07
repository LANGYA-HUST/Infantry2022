/**********************************************************************************************************
 * @�ļ�     uart4.c
 * @˵��     uart4 f4��ʼ��(����ϵͳͨ��)
 * @�汾  	 V2.0
 * @����     ���κ�
 * @����     2022.3
**********************************************************************************************************/
#include "main.h"

unsigned char JudgeReceiveBuffer[JudgeBufBiggestSize];
unsigned char JudgeSend[SEND_MAX_SIZE];
/**********************************************************************************************************
*��    ��: ��
*�� �� ֵ: ��
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
	
	USART_DMACmd(UART4,USART_DMAReq_Rx,ENABLE);	                      //ʹ��USART4DMA����

	nvic.NVIC_IRQChannel = DMA1_Stream2_IRQn;
  nvic.NVIC_IRQChannelPreemptionPriority = 2;
  nvic.NVIC_IRQChannelSubPriority = 1;
  nvic.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic);

		
		{
			DMA_InitTypeDef   dma;
			/* ����DMA1 Stream2��UART4���� */
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
		
		USART_DMACmd(UART4,USART_DMAReq_Tx,ENABLE);                           //ʹ��USART4DMA����

		nvic.NVIC_IRQChannel = DMA1_Stream4_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 3;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
		
		{
			DMA_InitTypeDef   DMA_InitStructure;
			DMA_DeInit(DMA1_Stream4);
			while (DMA_GetCmdStatus(DMA1_Stream4) != DISABLE);//�ȴ�DMA������ 
			/* ����DMA1 Stream4��UART4���� */
			DMA_InitStructure.DMA_Channel            = DMA_Channel_4;               //ͨ��ѡ��
			DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(UART4->DR);      //DMA�����ַ
			DMA_InitStructure.DMA_Memory0BaseAddr    = (u32)JudgeSend;      //DMA �洢��0��ַ
			DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;  //�洢��������ģʽ
			DMA_InitStructure.DMA_BufferSize         = SEND_MAX_SIZE;       //���ݴ����� 
			DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;   //���������ģʽ
			DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;        //�洢������ģʽ
			DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //�������ݳ���:8λ
			DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;     //�洢�����ݳ���:8λ
			DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;             //ʹ����ͨģʽ 
			DMA_InitStructure.DMA_Priority           = DMA_Priority_High;         //�е����ȼ�
			DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;         
			DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_1QuarterFull;
			DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;      //�洢��ͻ�����δ���
			DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;  //����ͻ�����δ���
			DMA_Init(DMA1_Stream4, &DMA_InitStructure);                             //��ʼ��DMA Stream4

			DMA_ITConfig(DMA1_Stream4,DMA_IT_TC,ENABLE);				//DMA2��������ж�
			DMA_Cmd(DMA1_Stream4, DISABLE);											//��ʹ��
		
		}
}
///**********************************************************************************************************
//*�� �� ��: DMA1_Stream_IRQHandler
//*����˵��: usart2 DMA�����ж�
//*��    ��: ��
//*�� �� ֵ: ��
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
//*�� �� ��: DMA1_Stream4_IRQHandler
//*����˵��: usart2 DMA�����ж�
//*��    ��: ��
//*�� �� ֵ: ��
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



