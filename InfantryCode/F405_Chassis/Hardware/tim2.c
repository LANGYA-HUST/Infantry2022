/**********************************************************************************************************
 * @文件     tim2.c
 * @说明     tim2初始化
 * @版本  	 V1.0
 * @作者     陈志鹏
 * @日期     2020.1
**********************************************************************************************************/
#include "main.h"

int Tim2_usCnt = 0;
/**********************************************************************************************************
*函 数 名: TIM2_Configuration
*功能说明: TIM2初始化 1ms 一次中断
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef tim2;
    NVIC_InitTypeDef NVIC_TIM;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    tim2.TIM_ClockDivision = TIM_CKD_DIV1;   
    tim2.TIM_Prescaler = 0;          
    tim2.TIM_CounterMode = TIM_CounterMode_Up;
    tim2.TIM_RepetitionCounter = 0;
    tim2.TIM_Period = 65535;                //1ms
	
		NVIC_TIM.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_TIM.NVIC_IRQChannelCmd = ENABLE;
		NVIC_TIM.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_TIM.NVIC_IRQChannelSubPriority = 0;
		NVIC_Init(&NVIC_TIM);
			
    TIM_TimeBaseInit(TIM2, &tim2);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE); 
    TIM_Cmd(TIM2, DISABLE);
}

void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
			Tim2_usCnt++;
    }
}


/**********************************************************************************************************
*函 数 名: delay_ms
*功能说明: ms延时
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void delay_ms(int n)
{
	TIM_SetCounter(TIM2, 0);
	TIM_Cmd(TIM2, ENABLE);
	while (TIM_GetCounter(TIM2) < (72 * n));
	TIM_Cmd(TIM2, DISABLE);
}
