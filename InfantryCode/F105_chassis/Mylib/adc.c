/**********************************************************************************************************
 * @文件     adc.c
 * @说明     adc初始化
 * @版本  	 V1.0
 * @作者     黄志雄
 * @日期     2020.1
**********************************************************************************************************/
#include "main.h"

unsigned short ADC_ConvertedValue[90];

#define ADC1_DR_Address    ((u32)0x4001244C)
#define ADC2_DR_Address    ((u32)0x4001244C)

/**********************************************************************************************************
*函 数 名: ADC_Configuration
*功能说明: ADC初始化(PA17	电容电压)
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void ADC_Configuration(void)
{
	GPIO_InitTypeDef         gpio;
	ADC_InitTypeDef          adc;
	DMA_InitTypeDef          dma;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE); 

	gpio.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_4;  //I_IN			//PA3
	gpio.GPIO_Mode = GPIO_Mode_AIN;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
	
	DMA_DeInit(DMA1_Channel1);
	dma.DMA_PeripheralBaseAddr = ADC1_DR_Address;
	dma.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;
	dma.DMA_DIR = DMA_DIR_PeripheralSRC;
	dma.DMA_BufferSize = 90;//每个通道30
	dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //16位
	dma.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	 //16位
	dma.DMA_Mode = DMA_Mode_Circular;
	dma.DMA_Priority = DMA_Priority_High;
	dma.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &dma);
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	ADC_Init(ADC1, &adc);
	adc.ADC_Mode = ADC_Mode_Independent;	             
	adc.ADC_ScanConvMode = ENABLE;			//各通道顺序扫描,若采集多个通道，必须开启此模式             
	adc.ADC_ContinuousConvMode = ENABLE;	               
	adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  
	adc.ADC_DataAlign = ADC_DataAlign_Right;
	adc.ADC_NbrOfChannel = 3;						//扫描一个通道
	ADC_Init(ADC1, &adc);
	ADC_TempSensorVrefintCmd(ENABLE);//???????
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_239Cycles5);	//u_in
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_239Cycles5);	//u_in
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 3, ADC_SampleTime_239Cycles5);	//u_in
	ADC_DMACmd(ADC1, ENABLE);

	ADC_Cmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}



