#include "adc.h"
#include "stm32f4xx_adc.h"

#define TSL_SI    PCout(3)   //SI  
#define TSL_CLK   PCout(2)   //CLK 
/**************************************************************************
函数功能：延时
入口参数：无
返回  值：无
作    者：平衡小车之家
**************************************************************************/
void Dly_us(void)
{
   int ii;    
   for(ii=0;ii<10;ii++);      
}
/**************************************************************************
函数功能：CCD数据采集
入口参数：无
返回  值：无
作    者：平衡小车之家
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
			Dly_us();  //调节曝光时间
			Dly_us();  //调节曝光时间
			ADV[tslp]=(Get_Adc(11))>>4;
			++tslp;
			TSL_CLK=1;
      Dly_us();
  }  
}
/**************************************************************************
作者：平衡小车之家
函数功能：ACD初始化电池电压检测
入口参数：无
返回  值：无
作    者：平衡小车之家
**************************************************************************/
void  Adc_Init(void)
{    
 	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE );//使能ADC1通道时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟
	                      
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;			//GPIOA.5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//不带上下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);				//初始化
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	//ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);//复位结束	
	
	ADC_CommonInitStructure.ADC_Mode=ADC_Mode_Independent;//独立模式
	ADC_CommonInitStructure.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
	ADC_CommonInitStructure.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled; //DMA失能
	ADC_CommonInitStructure.ADC_Prescaler=ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
}		

/**************************************************************************
函数功能：AD采样
入口参数：ADC1 的通道
返回  值：AD转换结果
**************************************************************************/
u16 Get_Adc(u8 ch)   
{
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles);//ADC1,通道,480个周期,提高采样时间可以提高精度
	ADC_SoftwareStartConv(ADC1);//使能指定的ADC1的软件转换启动功能		 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}


/**************************************************************************
函数功能：读取电池电压 
入口参数：无
返回  值：电池电压 单位MV
**************************************************************************/
int Get_battery_volt(void)   
{  
	int Volt;//电池电压
	Volt=Get_Adc(Battery_Ch)*3.3*11*100/4096;	//电阻分压，具体根据原理图简单分析可以得到	
	return Volt;
}

/**************************************************************************
函数功能：线性CCD初始化
入口参数：无
返回  值：无
作    者：平衡小车之家
**************************************************************************/
void  ccd_Init(void)
{    
 	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE );//使能ADC1通道时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟
	                      
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;			//GPIOA.5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//不带上下拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);				//初始化
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;		     //端口配置
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;          //输出模式
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;         //推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;      //高速50MHZ
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;           //上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);				 //根据设定参数初始化GPIOA5.12
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	//ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);//复位结束	
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
}	

void  ele_Init(void)
{    
 	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE );//使能ADC1通道时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟
	                      
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;			//GPIOA.5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//不带上下拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);				//初始化
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	//ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);//复位结束	
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
}		
