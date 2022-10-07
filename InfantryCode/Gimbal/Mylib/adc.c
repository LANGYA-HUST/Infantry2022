#include "adc.h"
#include "stm32f4xx_adc.h"

#define TSL_SI    PCout(3)   //SI  
#define TSL_CLK   PCout(2)   //CLK 
/**************************************************************************
�������ܣ���ʱ
��ڲ�������
����  ֵ����
��    �ߣ�ƽ��С��֮��
**************************************************************************/
void Dly_us(void)
{
   int ii;    
   for(ii=0;ii<10;ii++);      
}
/**************************************************************************
�������ܣ�CCD���ݲɼ�
��ڲ�������
����  ֵ����
��    �ߣ�ƽ��С��֮��
**************************************************************************/
 void RD_TSL(void) 
{
 u8 i=0,tslp=0;
  TSL_CLK=1;
  TSL_SI=0; 
  Dly_us();
      
  TSL_SI=1; 
  TSL_CLK=0;
  Dly_us();
      
  TSL_CLK=1;
  TSL_SI=0;
  Dly_us(); 
  for(i=0;i<128;i++)
  { 
			TSL_CLK=0; 
			Dly_us();  //�����ع�ʱ��
			Dly_us();  //�����ع�ʱ��
			ADV[tslp]=(Get_Adc(11))>>4;
			++tslp;
			TSL_CLK=1;
      Dly_us();
  }  
}
/**************************************************************************
���ߣ�ƽ��С��֮��
�������ܣ�ACD��ʼ����ص�ѹ���
��ڲ�������
����  ֵ����
��    �ߣ�ƽ��С��֮��
**************************************************************************/
void  Adc_Init(void)
{    
 	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE );//ʹ��ADC1ͨ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��
	                      
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;			//GPIOA.5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//ģ������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);				//��ʼ��
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	//ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);//��λ����	
	
	ADC_CommonInitStructure.ADC_Mode=ADC_Mode_Independent;//����ģʽ
	ADC_CommonInitStructure.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
	ADC_CommonInitStructure.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled; //DMAʧ��
	ADC_CommonInitStructure.ADC_Prescaler=ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
}		

/**************************************************************************
�������ܣ�AD����
��ڲ�����ADC1 ��ͨ��
����  ֵ��ADת�����
**************************************************************************/
u16 Get_Adc(u8 ch)   
{
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles);//ADC1,ͨ��,480������,��߲���ʱ�������߾���
	ADC_SoftwareStartConv(ADC1);//ʹ��ָ����ADC1�����ת����������		 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}


/**************************************************************************
�������ܣ���ȡ��ص�ѹ 
��ڲ�������
����  ֵ����ص�ѹ ��λMV
**************************************************************************/
int Get_battery_volt(void)   
{  
	int Volt;//��ص�ѹ
	Volt=Get_Adc(Battery_Ch)*3.3*11*100/4096;	//�����ѹ���������ԭ��ͼ�򵥷������Եõ�	
	return Volt;
}

/**************************************************************************
�������ܣ�����CCD��ʼ��
��ڲ�������
����  ֵ����
��    �ߣ�ƽ��С��֮��
**************************************************************************/
void  ccd_Init(void)
{    
 	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE );//ʹ��ADC1ͨ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��
	                      
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;			//GPIOA.5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//ģ������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);				//��ʼ��
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;		     //�˿�����
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;          //���ģʽ
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;         //�������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;      //����50MHZ
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;           //����
	GPIO_Init(GPIOC, &GPIO_InitStructure);				 //�����趨������ʼ��GPIOA5.12
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	//ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);//��λ����	
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
}	

void  ele_Init(void)
{    
 	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE );//ʹ��ADC1ͨ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��
	                      
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;			//GPIOA.5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//ģ������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);				//��ʼ��
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	//ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);//��λ����	
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
}		
