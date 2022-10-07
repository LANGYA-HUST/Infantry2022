/**********************************************************************************************************
 * @文件     FrictionWheel.c
 * @说明     摩擦轮初始化
 * @版本  	 V1.0
 * @作者     黄志雄
 * @日期     2019.10
**********************************************************************************************************/
#include "main.h"
uint32_t SRC_Buffer[]={12,23,12,23,12,23,12,23,12,23,12,23,12,23,12,23,12,23,12,23,12,23,12,23,12,23,12,23,12,23,12,23};
/**********************************************************************************************************
*函 数 名: FrictionWheel_Configration
*功能说明: 摩擦轮配置函数(如果在魔术棒里引用DSP库，开启摩擦轮后会进硬件错误中断，直接添加lib文件后解决，原因未知)
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void FrictionWheel_Configration(void)             
{
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_OCInitTypeDef TIM_OCInitStruct;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
		DMA_InitTypeDef DMA_InitStructure;
		NVIC_InitTypeDef  nvic;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);

		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_8;//PB10  TIM2_ch3
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;//PC8 Tim8--Ch3     PC9 Tim8--Ch4
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
		GPIO_Init(GPIOC,&GPIO_InitStructure);

		GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM8);
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM8);
		DMA_DeInit(DMA2_Stream1);//tim8_up
		DMA_InitStructure.DMA_Channel =DMA_Channel_7;
		DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)&(TIM8->DMAR);
		DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)SRC_Buffer;	
		DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;	
		DMA_InitStructure.DMA_BufferSize = 32;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Word;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(DMA2_Stream1, &DMA_InitStructure);
			
		TIM_TimeBaseInitStruct.TIM_Prescaler=8-1;//21Mhz
		TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
		TIM_TimeBaseInitStruct.TIM_Period=35-1;//600k  1.67us    12   23
		TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
		TIM_TimeBaseInit(TIM8,&TIM_TimeBaseInitStruct);

		TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;//用PWM2波形会反转(未知原因)
		TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStruct.TIM_Pulse = 12;
		TIM_OC3Init(TIM8,&TIM_OCInitStruct);
		TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OC4Init(TIM8,&TIM_OCInitStruct);

		nvic.NVIC_IRQChannel = DMA2_Stream1_IRQn;
		nvic.NVIC_IRQChannelPreemptionPriority = 2;
		nvic.NVIC_IRQChannelSubPriority = 2;
		nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvic);

		TIM_DMAConfig(TIM8,TIM_DMABase_CCR3,TIM_DMABurstLength_2Transfers);
		TIM_DMACmd(TIM8, TIM_DMA_Update, ENABLE);
		TIM_Cmd(TIM8,ENABLE);
		TIM_CtrlPWMOutputs(TIM8,ENABLE);
		DMA_ITConfig(DMA2_Stream1,DMA_IT_TC,ENABLE);
		DMA_Cmd(DMA2_Stream1, ENABLE);

}

/**********************************************************************************************************
*函 数 名: DMA2_Stream1_IRQHandler
*功能说明: DMA接收中断
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void DMA2_Stream1_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA2_Stream1,DMA_IT_TCIF1)==SET)
	{
		TIM_DMACmd(TIM8, TIM_DMA_Update, DISABLE);
		DMA_ITConfig(DMA2_Stream1,DMA_IT_TC,DISABLE);
		DMA_Cmd(DMA2_Stream1, DISABLE);
		TIM_SetCompare3(TIM8,0);
		TIM_SetCompare4(TIM8,0);
		TIM_Cmd(TIM4, ENABLE);
		TIM_ClearFlag(TIM4, TIM_FLAG_Update);
		TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
		DMA_ClearITPendingBit(DMA2_Stream1,DMA_IT_TCIF1);
	}
}
/**********************************************************************************************************
*函 数 名: FrictionWheel_Set
*功能说明: 设置摩擦轮速度
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
extern uint16_t accelerator;
extern int delay_time;
extern Status_t Status;
short bias_speed = 0;
extern F105_Typedef F105;

extern Pid_Typedef PidFrictionSpeed[2];
extern short FrictionReceive[2];
short FrictionReceive_LP[2];

#define Fric_LP_K  0.05f

short FrictionCurrent[2];
int SHOOT_Speed;
void FrictionWheel_Set(short speed)
{
	if(Status.ShootMode!=Shoot_Check_Mode)
	{
		delay_time = 0;
	}
	
	
//	speed=LIMIT_MAX_MIN(speed,10000,0);//snail
	
	accelerator=speed;
	
//	switch(F105.BulletSpeedLevel)
//	{
//		case 0:
//			bias_speed = 0;
//			break;
//		case 1:
//			break;
//		case 2:
//			break;
//		default:
//			break;
//	}
	
	
	PidFrictionSpeed[0].SetPoint=accelerator;     //  accelerator+bias_speed;
	PidFrictionSpeed[1].SetPoint=-accelerator;
	
	
	PidFrictionSpeed[0].ActualValue = FrictionReceive[0];
	FrictionCurrent[0]=PID_Calc(&PidFrictionSpeed[0]);
	PidFrictionSpeed[1].ActualValue = FrictionReceive[1];
	FrictionCurrent[1]=PID_Calc(&PidFrictionSpeed[1]);
  SHOOT_Speed=FrictionReceive[0]+FrictionReceive[1];
}


