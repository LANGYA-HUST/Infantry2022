/**********************************************************************************************************
 * @�ļ�     tim2.c
 * @˵��     tim2��ʼ��
 * @�汾  	 V1.0
 * @����     ��־��
 * @����     2020.1
**********************************************************************************************************/
#include "main.h"

int Tim2_usCnt = 0;
/**********************************************************************************************************
*�� �� ��: TIM2_Configuration
*����˵��: TIM2��ʼ�� 1ms һ���ж�
*��    ��: ��
*�� �� ֵ: ��
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
*�� �� ��: delay_ms
*����˵��: ms��ʱ
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void delay_ms(int n)
{
	TIM_SetCounter(TIM2, 0);
	TIM_Cmd(TIM2, ENABLE);
	while (TIM_GetCounter(TIM2) < (72 * n));
	TIM_Cmd(TIM2, DISABLE);
}
