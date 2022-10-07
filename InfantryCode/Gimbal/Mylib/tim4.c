/**********************************************************************************************************
 * @文件     tim4.c
 * @说明     弹舱盖舵机初始化
 * @版本  	 V1.0
 * @作者     黄志雄
 * @日期     2020.1
**********************************************************************************************************/
#include "main.h"

#define SRC_DATA_LEN 2  //(最后面放几个0做延迟)
uint32_t accelerator=5000;//占空比

/**
  * @brief  TIM4工作模式配置
  * @param  None
  * @retval None
  */
void Tim4_Configuration(void)            
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
//	DMA_InitTypeDef DMA_InitStructure;
//	NVIC_InitTypeDef  nvic;
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);      //使能TIM4时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);     //使能GPIOB时钟
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);      //使能DMA2时钟

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;      //PB6 Tim4--Ch1     PB7 Tim4--Ch2
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;              //复用输出模式  
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;            //下拉(到GND)输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;          //开漏输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;        //输出速率
	GPIO_Init(GPIOB,&GPIO_InitStructure);                   //按照该结构体配置对应端口

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4);   //用引脚PC8；把TIM8引到AF2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4);   //用引脚PC9；把TIM8引到AF2    
	
//    DMA_DeInit(DMA1_Stream6);                                               //TIM4_UP的DMA通道
//    DMA_InitStructure.DMA_Channel =DMA_Channel_2;                           //输入通道为Channel_2,即TIM4_UP的
//	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)&(TIM4->DMAR);      //外设基地址为TIM4的DMAR寄存器
//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&accelerator;	            //存储器基地址为SRC_Buffer变量的位置
//	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;	                //用DMA的传输方向为存储器到外设
//	DMA_InitStructure.DMA_BufferSize = SRC_DATA_LEN;                        //要传输的数据项数（4个数据，每个32位，这个值写到NDTR寄存器里面去）(为什么设成32就只有15个波？设成33才行？)
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        //外设地址不递增
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 //存储器地址递增
//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; //外设数据大小为一个字长（32位）
//	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Word;     //存储器数据大小为一个字长（32位）
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                         //DMA工作在循环模式
//	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                 //DMA通道优先级配置（11b）
//	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;                   //禁止DMA的直接模式，开启队列模式（虽然不知道是干什么用的，但知道是写到DMDIS位上去）
//	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;           //队列阈值设置为全队容量（即4个字的容量）
//	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;             //存储器突发传输配置（单次传输）
//	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;     //外设突发传输配置（单次传输）
//	DMA_Init(DMA1_Stream6, &DMA_InitStructure);
				
    //时基配置：
        //每32个溢出（即32个period后）产生一个UP中断，
        //在更新中断里面，把下一次的DSHOT的16个数配置好
        //f(HSE)=168MHz,经过预分频器PSC后，得到f(CNT)=24MHz的计时单元，
        //然后以1200个计时单元为一个周期，该周期T=1/f(CNT)*1200=50.0us,
        //看到论坛上面人说是500Hz可以激活C615，这里也先用500Hz试一下先
	TIM_TimeBaseInitStruct.TIM_Prescaler=(7*3)-1;                               //时基结构体的预分频数24Mhz(168M/7=24M)
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;              //计数模式（递增计数）
	TIM_TimeBaseInitStruct.TIM_Period=8000-1;                              //f=24MHz/48000=500Hz ~ T=2.0ms（写到ARR里去）
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;                  //时钟分频数（不分频）
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;                         //重复计数器：写入0到RCR里去（即不重复的意思）
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	
    //给C615的摩擦轮变速需要在每输出一次PWM波后往CCR里重写一次脉宽的值。
    //这个功能的实现需要两个中断：
        //1、输出完一个数字信号后的TIM定时器更新中断；
            //在这个中断里面向DMA获取下一个写入CCR的脉宽值.
        //2、DMA传输完成中断；
            //在这个中断里面获取下次的脉宽值.
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;                          //输出比较模式设定为PWM1
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;              //输出比较使能（写到CCER寄存器里面去）
	TIM_OCInitStruct.TIM_Pulse = accelerator;                                     //写到CCR里去，比较寄存器的装载值（一个Period里的脉冲数）
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;                  //输出通道为高电平有效
    TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;           //互补输出不使能
    TIM_OC1Init(TIM4,&TIM_OCInitStruct);
    TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);
		
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;                          //输出比较模式设定为PWM1
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;              //输出比较使能（写到CCER寄存器里面去）
	TIM_OCInitStruct.TIM_Pulse = accelerator;                                     //写到CCR里去，比较寄存器的装载值（一个Period里的脉冲数）
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;                  //输出通道为高电平有效
    TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;           //互补输出不使能
				TIM_OC2Init(TIM4,&TIM_OCInitStruct);
		TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);

//	nvic.NVIC_IRQChannel = DMA1_Stream6_IRQn;                               //TIM4_UP引脚的DMA中断优先级配置
//	nvic.NVIC_IRQChannelPreemptionPriority = 0;                             //设置中断的主优先级
//	nvic.NVIC_IRQChannelSubPriority = 0;                                    //设置中断的抢占优先级
//	nvic.NVIC_IRQChannelCmd = ENABLE;                                       //中断处理使能
//	NVIC_Init(&nvic);

//	TIM_DMAConfig(TIM4,TIM_DMABase_CCR1,TIM_DMABurstLength_2Transfers);      //自CCR1地址开始的两个寄存器传输数据
    //TIM_DMAConfig(TIM8,TIM_DMABase_CCR1,TIM_DMABurstLength_2Transfers);      //自CCR1地址开始的两个寄存器传输数据
    //这个函数的功能很重要，正是它使得DMA能够将数据传到CCR3、CCR4连着的两个reg里去.
    //第二个参数是从DMAR读数据过来的基地址（DMAR是这列TIM接收并存放DMA数据的寄存器）;
    //第三个参数表示从第二个参数的基地址位置开始，往后面的几个寄存器传数据.
    //这个函数在这里的功能的即为：
        //从DMA接收两次数据到DMAR，第一次把数据读到CCR3里，第二次把数据读到CCR4（即CCR3后面的一个reg里），然后才产生一次更新中断.
	
//    TIM_DMACmd(TIM4, TIM_DMA_Update, ENABLE);                                //使能更新DMA请求（写到DIER的位8上去）
	TIM_ARRPreloadConfig(TIM4,ENABLE);                                       //使能TIM4的ARR预装载寄存器
    TIM_Cmd(TIM4,ENABLE);                                                    //使能TIM4外设
	TIM_CtrlPWMOutputs(TIM4,ENABLE);                                         //主输出使能（写到BDTR里去？）
//	DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);                             //使能DMA1_Stream6的传输完成中断
//	DMA_Cmd(DMA1_Stream6, ENABLE);                                           //使能DMA1_Stream6
}


/**
  * @brief  DMA中断处理函数
  * @param  
  * @retval None
  */
//void DMA1_Stream6_IRQHandler(void)
//{
//    if(DMA_GetFlagStatus(DMA1_Stream6,DMA_IT_TCIF6)!=RESET)                    //当TCIF标志被置位时（即DMA传输完成中断，该中断在NDTR置到零是触发）
//	{
//		TIM_DMACmd(TIM4, TIM_DMA_Update, DISABLE);                              //暂时关闭TIM4的更新事件请求
//		DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,DISABLE);                           //暂时关闭DMA的传输完成中断
//		DMA_Cmd(DMA1_Stream6, DISABLE);                                         //暂时关闭DMA的传输通道
//		TIM_SetCompare1(TIM4,0);                                                //手动重设写到CCR里的装载值（写零复位）
//		TIM_SetCompare2(TIM4,0);                                                //手动重设写到CCR里的装载值（写零复位）
//        
//        accelerator=LIMIT_MAX_MIN(accelerator,48000,0);
//        
//		DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);    //清除DMA传输完成的中断标志位(编号和stream的号对应)
//  
//		TIM_DMACmd(TIM4, TIM_DMA_Update, ENABLE);       //使能TIM8更新对DMA的数据传输请求
//		DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);    //使能DMA的传输完成中断
//		DMA_Cmd(DMA1_Stream6, ENABLE);
//        
//		DMA_ClearITPendingBit(DMA1_Stream6,DMA_IT_TCIF6);                       //清除DMA的TCIF标志
//	}
//}


/**
  * @brief  TIM4中断处理函数
  * @param  
  * @retval None
  */
void TIM4_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET ) 
	{
        TIM_ClearFlag(TIM4, TIM_FLAG_Update);           //清除TIM4的更新事件标志
	}	
}





