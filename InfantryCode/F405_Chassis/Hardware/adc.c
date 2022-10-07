/**********************************************************************************************************
 * @文件     adc.c
 * @说明     adc初始化
 * @版本  	 V2.0
 * @作者     郭嘉豪
 * @日期     2022.5
**********************************************************************************************************/
#include "main.h"
#include "stm32f4xx_adc.h"

#define ADC_NUM 3 //采集通道数目2+温度通道1
#define ADC1_DR_Address    (((u32)ADC1+0x4c))
#define ADC2_DR_Address    (((u32)ADC2+0x4c))

unsigned short ADC_ConvertedValue[ADC_NUM*30];
/**********************************************************************************************************
*函 数 名: ADC_Configuration
*功能说明: ADC初始化(PA17	电容电压)
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void ADC_Configuration(void)
{
	GPIO_InitTypeDef         gpio;
	
	DMA_InitTypeDef          dma;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOB, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	gpio.GPIO_Pin = GPIO_Pin_4;  //IN4
	gpio.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
//	gpio.GPIO_Pin = GPIO_Pin_9;  //IN9
//	GPIO_Init(GPIOB,&gpio);

	 /*首先开DMA2时钟，由405参考手册-RM0090-Reference manual
	 *308页可知，ADC与DMA2映射，而且DMA2挂载在AHB1时钟总线上*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	DMA_DeInit(DMA2_Stream0);
  DMA_StructInit(&dma);
	dma.DMA_PeripheralBaseAddr = ADC1_DR_Address;//数据传输的外设首地址
	dma.DMA_Memory0BaseAddr  = (uint32_t)&ADC_ConvertedValue;//自己定义待发送数组的首地址，要强制转换为32位
	dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
	dma.DMA_BufferSize = ADC_NUM*30;//传输数据大小为16，单位由以下确定，大小要配合定义的数组类型和外设数据类型
	dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	dma.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
	dma.DMA_Mode = DMA_Mode_Circular;
	dma.DMA_Priority = DMA_Priority_High;
	DMA_Init(DMA2_Stream0, &dma);
	DMA_Cmd(DMA2_Stream0, ENABLE);
	
	//通用ADC设置
	ADC_DeInit();
	ADC_CommonInitTypeDef  adc_comm;
	adc_comm.ADC_Mode = ADC_Mode_Independent;//只用了一个ADC
	adc_comm.ADC_Prescaler = ADC_Prescaler_Div6;//时钟6分频
	adc_comm.ADC_DMAAccessMode = ADC_DMAAccessMode_1; //DMA使能，用于12位精度
	adc_comm.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
	
	//ADC1配置
	ADC_InitTypeDef  adc;
	ADC_Init(ADC1, &adc);  
	adc.ADC_Resolution = ADC_Resolution_12b;//可选的分辨率有12位、10位、8位和6位。分辨率越高，AD转换数据精度越高，转换时间也越长；分辨率越低，AD转换数据精度越低，转换时间也越短。
	adc.ADC_ScanConvMode = ENABLE;			//各通道顺序扫描,若采集多个通道，必须开启此模式             
	adc.ADC_ContinuousConvMode = ENABLE;//配置是启动自动连续转换还是单次转换。         
	adc.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None; //禁止触发,使用软件触发 
	adc.ADC_DataAlign = ADC_DataAlign_Right;
	adc.ADC_NbrOfConversion = ADC_NUM;//2个通道
	ADC_Init(ADC1, &adc);
	ADC_TempSensorVrefintCmd(ENABLE);//开启温度通道

	// 配置 ADC 通道转换顺序和采样时间周期
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_480Cycles);	//u_in
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_480Cycles);	//u_in
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 3, ADC_SampleTime_480Cycles);	//u_in  温度通道
	ADC_DMACmd(ADC1, ENABLE);

	// 使能DMA请求 after last transfer (Single-ADC mode)

	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);// 使能DMA请求 after last transfer (Single-ADC mode)
	ADC_DMACmd(ADC1, ENABLE);// 使能ADC DMA
	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConv(ADC1); //开始adc转换，软件触发
}



