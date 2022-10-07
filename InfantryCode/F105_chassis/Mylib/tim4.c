/**********************************************************************************************************
 * @�ļ�     tim4.c
 * @˵��     tim4��ʼ��(���ڿ��Ź�)
 * @�汾  	 V1.0
 * @����     ��־��
 * @����     2020.1
**********************************************************************************************************/
#include "tim4.h"
/**********************************************************************************************************
*�� �� ��: TIM4_Configuration
*����˵��: TIM4��ʼ��
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void TIM4_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 

    TIM_TimeBaseInitStructure.TIM_Period = 1000-1; 	
    TIM_TimeBaseInitStructure.TIM_Prescaler = 71;  
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; 

    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);

    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); 
    TIM_Cmd(TIM4, ENABLE); 

    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**********************************************************************************************************
*�� �� ��: TIM4_IRQHandler
*����˵��: tim4�ж�
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
volatile uint32_t CPU_RunTime = 0UL;
void  TIM4_IRQHandler (void)
{
	if ( TIM_GetITStatus( TIM4, TIM_IT_Update) != RESET ) 
	{	
    CPU_RunTime++;
		TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update);  		 
	}		 	
}

