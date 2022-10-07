/**********************************************************************************************************
 * @文件     SteeringEngine.c
 * @说明     弹舱盖舵机初始化
 * @版本  	 V1.0
 * @作者     黄志雄
 * @日期     2020.1
**********************************************************************************************************/
#include "main.h"
#include "SteeringEngine.h"
extern RobotInit_Struct Infantry;
/**********************************************************************************************************
*函 数 名: SteeringEngine_Configuration
*功能说明: 弹舱盖舵机初始化
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void SteeringEngine_Configuration(void)   //PB1 TIM3 CH4
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;												//初始化结构体
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);			//时钟配置
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM3);			//引脚复用
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;										//GPIO配置
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;		//Timer配置
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=20000-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler=72-1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = Infantry.MagClose;
	TIM_OC4Init(TIM3,&TIM_OCInitStruct);											//Channel4配置
	
	TIM_OC4PreloadConfig(TIM3,ENABLE);
	
	TIM_ARRPreloadConfig(TIM3,ENABLE);
	
	TIM_CtrlPWMOutputs(TIM3,ENABLE);
	
	TIM_Cmd(TIM3,ENABLE);

  TIM_SetCompare4(TIM3,Infantry.MagClose);
//	GPIO_InitTypeDef GPIO_InitStruct;
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
//	TIM_OCInitTypeDef TIM_OCInitStruct;												//初始化结构体
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);			//时钟配置
//	
//	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM3);			//引脚复用
//	
//	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;										//GPIO配置
//	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;
//	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
//  GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
//	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
//	GPIO_Init(GPIOB,&GPIO_InitStruct);
//	
//	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;		//Timer配置
//	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
//	TIM_TimeBaseInitStruct.TIM_Period=20000-1;
//	TIM_TimeBaseInitStruct.TIM_Prescaler=72-1;
//	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
//	
//	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
//	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
//	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
//	TIM_OC3Init(TIM3,&TIM_OCInitStruct);											//Channel4配置
//	
//	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
//	
//	TIM_ARRPreloadConfig(TIM3,ENABLE);
//	
//	TIM_CtrlPWMOutputs(TIM3,ENABLE);
//	
//	TIM_Cmd(TIM3,ENABLE);
}

/**********************************************************************************************************
*函 数 名: SteeringEngine_Set
*功能说明: 开/关弹舱盖
*形    参: position
*返 回 值: 无
**********************************************************************************************************/
unsigned char magazineState=0x00; //初次记录为关着的状态
int mag_place;
void SteeringEngine_Set(int position)       
{
//	SteeringEngine_Configuration();
	if(position==Infantry.MagOpen)
	  magazineState=0x01;
	if(position==Infantry.MagClose)
	  magazineState=0x00;
	mag_place=position;
	TIM_SetCompare4(TIM3,position);
}

