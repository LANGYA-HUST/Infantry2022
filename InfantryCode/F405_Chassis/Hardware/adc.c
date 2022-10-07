/**********************************************************************************************************
 * @�ļ�     adc.c
 * @˵��     adc��ʼ��
 * @�汾  	 V2.0
 * @����     ���κ�
 * @����     2022.5
**********************************************************************************************************/
#include "main.h"
#include "stm32f4xx_adc.h"

#define ADC_NUM 3 //�ɼ�ͨ����Ŀ2+�¶�ͨ��1
#define ADC1_DR_Address    (((u32)ADC1+0x4c))
#define ADC2_DR_Address    (((u32)ADC2+0x4c))

unsigned short ADC_ConvertedValue[ADC_NUM*30];
/**********************************************************************************************************
*�� �� ��: ADC_Configuration
*����˵��: ADC��ʼ��(PA17	���ݵ�ѹ)
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void ADC_Configuration(void)
{
	GPIO_InitTypeDef         gpio;
	
	DMA_InitTypeDef          dma;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOB, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	gpio.GPIO_Pin = GPIO_Pin_4;  //IN4
	gpio.GPIO_Mode = GPIO_Mode_AIN;//ģ������
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
//	gpio.GPIO_Pin = GPIO_Pin_9;  //IN9
//	GPIO_Init(GPIOB,&gpio);

	 /*���ȿ�DMA2ʱ�ӣ���405�ο��ֲ�-RM0090-Reference manual
	 *308ҳ��֪��ADC��DMA2ӳ�䣬����DMA2������AHB1ʱ��������*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	DMA_DeInit(DMA2_Stream0);
  DMA_StructInit(&dma);
	dma.DMA_PeripheralBaseAddr = ADC1_DR_Address;//���ݴ���������׵�ַ
	dma.DMA_Memory0BaseAddr  = (uint32_t)&ADC_ConvertedValue;//�Լ����������������׵�ַ��Ҫǿ��ת��Ϊ32λ
	dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
	dma.DMA_BufferSize = ADC_NUM*30;//�������ݴ�СΪ16����λ������ȷ������СҪ��϶�����������ͺ�������������
	dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	dma.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
	dma.DMA_Mode = DMA_Mode_Circular;
	dma.DMA_Priority = DMA_Priority_High;
	DMA_Init(DMA2_Stream0, &dma);
	DMA_Cmd(DMA2_Stream0, ENABLE);
	
	//ͨ��ADC����
	ADC_DeInit();
	ADC_CommonInitTypeDef  adc_comm;
	adc_comm.ADC_Mode = ADC_Mode_Independent;//ֻ����һ��ADC
	adc_comm.ADC_Prescaler = ADC_Prescaler_Div6;//ʱ��6��Ƶ
	adc_comm.ADC_DMAAccessMode = ADC_DMAAccessMode_1; //DMAʹ�ܣ�����12λ����
	adc_comm.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
	
	//ADC1����
	ADC_InitTypeDef  adc;
	ADC_Init(ADC1, &adc);  
	adc.ADC_Resolution = ADC_Resolution_12b;//��ѡ�ķֱ�����12λ��10λ��8λ��6λ���ֱ���Խ�ߣ�ADת�����ݾ���Խ�ߣ�ת��ʱ��ҲԽ�����ֱ���Խ�ͣ�ADת�����ݾ���Խ�ͣ�ת��ʱ��ҲԽ�̡�
	adc.ADC_ScanConvMode = ENABLE;			//��ͨ��˳��ɨ��,���ɼ����ͨ�������뿪����ģʽ             
	adc.ADC_ContinuousConvMode = ENABLE;//�����������Զ�����ת�����ǵ���ת����         
	adc.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None; //��ֹ����,ʹ��������� 
	adc.ADC_DataAlign = ADC_DataAlign_Right;
	adc.ADC_NbrOfConversion = ADC_NUM;//2��ͨ��
	ADC_Init(ADC1, &adc);
	ADC_TempSensorVrefintCmd(ENABLE);//�����¶�ͨ��

	// ���� ADC ͨ��ת��˳��Ͳ���ʱ������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_480Cycles);	//u_in
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_480Cycles);	//u_in
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 3, ADC_SampleTime_480Cycles);	//u_in  �¶�ͨ��
	ADC_DMACmd(ADC1, ENABLE);

	// ʹ��DMA���� after last transfer (Single-ADC mode)

	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);// ʹ��DMA���� after last transfer (Single-ADC mode)
	ADC_DMACmd(ADC1, ENABLE);// ʹ��ADC DMA
	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConv(ADC1); //��ʼadcת�����������
}



