/**********************************************************************************************************
 * @文件     tim7.c
 * @说明     tim7初始化
 * @版本  	 V1.0
 * @作者     黄志雄
 * @日期     2020.1
**********************************************************************************************************/
#include "main.h"
#include "tim7.h"

/**********************************************************************************************************
*函 数 名: Tim7_Configuration
*功能说明: tim7初始化
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void TIM7_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); 

    TIM_TimeBaseInitStructure.TIM_Period = 1000-1; 	
    TIM_TimeBaseInitStructure.TIM_Prescaler = 71;  
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; 

    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStructure);

    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE); 
    TIM_Cmd(TIM7, ENABLE); 

    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**********************************************************************************************************
*函 数 名: TIM7_IRQHandler
*功能说明: tim7中断
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
volatile uint32_t CPU_RunTime = 0UL;

void  TIM7_IRQHandler (void)
{
	if ( TIM_GetITStatus( TIM7, TIM_IT_Update) != RESET ) 
	{	
    CPU_RunTime++;
		TIM_ClearITPendingBit(TIM7 , TIM_FLAG_Update);  		 
	}		 	
}
