/**********************************************************************************************************
 * @�ļ�     tim4.c
 * @˵��     ���ոǶ����ʼ��
 * @�汾  	 V1.0
 * @����     ��־��
 * @����     2020.1
**********************************************************************************************************/
#include "main.h"

#define SRC_DATA_LEN 2  //(�����ż���0���ӳ�)
uint32_t accelerator=5000;//ռ�ձ�

/**
  * @brief  TIM4����ģʽ����
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
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);      //ʹ��TIM4ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);     //ʹ��GPIOBʱ��
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);      //ʹ��DMA2ʱ��

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;      //PB6 Tim4--Ch1     PB7 Tim4--Ch2
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;              //�������ģʽ  
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;            //����(��GND)���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;          //��©���
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;        //�������
	GPIO_Init(GPIOB,&GPIO_InitStructure);                   //���ոýṹ�����ö�Ӧ�˿�

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4);   //������PC8����TIM8����AF2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4);   //������PC9����TIM8����AF2    
	
//    DMA_DeInit(DMA1_Stream6);                                               //TIM4_UP��DMAͨ��
//    DMA_InitStructure.DMA_Channel =DMA_Channel_2;                           //����ͨ��ΪChannel_2,��TIM4_UP��
//	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)&(TIM4->DMAR);      //�������ַΪTIM4��DMAR�Ĵ���
//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&accelerator;	            //�洢������ַΪSRC_Buffer������λ��
//	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;	                //��DMA�Ĵ��䷽��Ϊ�洢��������
//	DMA_InitStructure.DMA_BufferSize = SRC_DATA_LEN;                        //Ҫ���������������4�����ݣ�ÿ��32λ�����ֵд��NDTR�Ĵ�������ȥ��(Ϊʲô���32��ֻ��15���������33���У�)
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        //�����ַ������
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 //�洢����ַ����
//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; //�������ݴ�СΪһ���ֳ���32λ��
//	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Word;     //�洢�����ݴ�СΪһ���ֳ���32λ��
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                         //DMA������ѭ��ģʽ
//	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                 //DMAͨ�����ȼ����ã�11b��
//	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;                   //��ֹDMA��ֱ��ģʽ����������ģʽ����Ȼ��֪���Ǹ�ʲô�õģ���֪����д��DMDISλ��ȥ��
//	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;           //������ֵ����Ϊȫ����������4���ֵ�������
//	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;             //�洢��ͻ���������ã����δ��䣩
//	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;     //����ͻ���������ã����δ��䣩
//	DMA_Init(DMA1_Stream6, &DMA_InitStructure);
				
    //ʱ�����ã�
        //ÿ32���������32��period�󣩲���һ��UP�жϣ�
        //�ڸ����ж����棬����һ�ε�DSHOT��16�������ú�
        //f(HSE)=168MHz,����Ԥ��Ƶ��PSC�󣬵õ�f(CNT)=24MHz�ļ�ʱ��Ԫ��
        //Ȼ����1200����ʱ��ԪΪһ�����ڣ�������T=1/f(CNT)*1200=50.0us,
        //������̳������˵��500Hz���Լ���C615������Ҳ����500Hz��һ����
	TIM_TimeBaseInitStruct.TIM_Prescaler=(7*3)-1;                               //ʱ���ṹ���Ԥ��Ƶ��24Mhz(168M/7=24M)
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;              //����ģʽ������������
	TIM_TimeBaseInitStruct.TIM_Period=8000-1;                              //f=24MHz/48000=500Hz ~ T=2.0ms��д��ARR��ȥ��
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;                  //ʱ�ӷ�Ƶ��������Ƶ��
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;                         //�ظ���������д��0��RCR��ȥ�������ظ�����˼��
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	
    //��C615��Ħ���ֱ�����Ҫ��ÿ���һ��PWM������CCR����дһ�������ֵ��
    //������ܵ�ʵ����Ҫ�����жϣ�
        //1�������һ�������źź��TIM��ʱ�������жϣ�
            //������ж�������DMA��ȡ��һ��д��CCR������ֵ.
        //2��DMA��������жϣ�
            //������ж������ȡ�´ε�����ֵ.
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;                          //����Ƚ�ģʽ�趨ΪPWM1
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;              //����Ƚ�ʹ�ܣ�д��CCER�Ĵ�������ȥ��
	TIM_OCInitStruct.TIM_Pulse = accelerator;                                     //д��CCR��ȥ���ȽϼĴ�����װ��ֵ��һ��Period�����������
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;                  //���ͨ��Ϊ�ߵ�ƽ��Ч
    TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;           //���������ʹ��
    TIM_OC1Init(TIM4,&TIM_OCInitStruct);
    TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);
		
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;                          //����Ƚ�ģʽ�趨ΪPWM1
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;              //����Ƚ�ʹ�ܣ�д��CCER�Ĵ�������ȥ��
	TIM_OCInitStruct.TIM_Pulse = accelerator;                                     //д��CCR��ȥ���ȽϼĴ�����װ��ֵ��һ��Period�����������
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;                  //���ͨ��Ϊ�ߵ�ƽ��Ч
    TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;           //���������ʹ��
				TIM_OC2Init(TIM4,&TIM_OCInitStruct);
		TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);

//	nvic.NVIC_IRQChannel = DMA1_Stream6_IRQn;                               //TIM4_UP���ŵ�DMA�ж����ȼ�����
//	nvic.NVIC_IRQChannelPreemptionPriority = 0;                             //�����жϵ������ȼ�
//	nvic.NVIC_IRQChannelSubPriority = 0;                                    //�����жϵ���ռ���ȼ�
//	nvic.NVIC_IRQChannelCmd = ENABLE;                                       //�жϴ���ʹ��
//	NVIC_Init(&nvic);

//	TIM_DMAConfig(TIM4,TIM_DMABase_CCR1,TIM_DMABurstLength_2Transfers);      //��CCR1��ַ��ʼ�������Ĵ�����������
    //TIM_DMAConfig(TIM8,TIM_DMABase_CCR1,TIM_DMABurstLength_2Transfers);      //��CCR1��ַ��ʼ�������Ĵ�����������
    //��������Ĺ��ܺ���Ҫ��������ʹ��DMA�ܹ������ݴ���CCR3��CCR4���ŵ�����reg��ȥ.
    //�ڶ��������Ǵ�DMAR�����ݹ����Ļ���ַ��DMAR������TIM���ղ����DMA���ݵļĴ�����;
    //������������ʾ�ӵڶ��������Ļ���ַλ�ÿ�ʼ��������ļ����Ĵ���������.
    //�������������Ĺ��ܵļ�Ϊ��
        //��DMA�����������ݵ�DMAR����һ�ΰ����ݶ���CCR3��ڶ��ΰ����ݶ���CCR4����CCR3�����һ��reg���Ȼ��Ų���һ�θ����ж�.
	
//    TIM_DMACmd(TIM4, TIM_DMA_Update, ENABLE);                                //ʹ�ܸ���DMA����д��DIER��λ8��ȥ��
	TIM_ARRPreloadConfig(TIM4,ENABLE);                                       //ʹ��TIM4��ARRԤװ�ؼĴ���
    TIM_Cmd(TIM4,ENABLE);                                                    //ʹ��TIM4����
	TIM_CtrlPWMOutputs(TIM4,ENABLE);                                         //�����ʹ�ܣ�д��BDTR��ȥ����
//	DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);                             //ʹ��DMA1_Stream6�Ĵ�������ж�
//	DMA_Cmd(DMA1_Stream6, ENABLE);                                           //ʹ��DMA1_Stream6
}


/**
  * @brief  DMA�жϴ�����
  * @param  
  * @retval None
  */
//void DMA1_Stream6_IRQHandler(void)
//{
//    if(DMA_GetFlagStatus(DMA1_Stream6,DMA_IT_TCIF6)!=RESET)                    //��TCIF��־����λʱ����DMA��������жϣ����ж���NDTR�õ����Ǵ�����
//	{
//		TIM_DMACmd(TIM4, TIM_DMA_Update, DISABLE);                              //��ʱ�ر�TIM4�ĸ����¼�����
//		DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,DISABLE);                           //��ʱ�ر�DMA�Ĵ�������ж�
//		DMA_Cmd(DMA1_Stream6, DISABLE);                                         //��ʱ�ر�DMA�Ĵ���ͨ��
//		TIM_SetCompare1(TIM4,0);                                                //�ֶ�����д��CCR���װ��ֵ��д�㸴λ��
//		TIM_SetCompare2(TIM4,0);                                                //�ֶ�����д��CCR���װ��ֵ��д�㸴λ��
//        
//        accelerator=LIMIT_MAX_MIN(accelerator,48000,0);
//        
//		DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);    //���DMA������ɵ��жϱ�־λ(��ź�stream�ĺŶ�Ӧ)
//  
//		TIM_DMACmd(TIM4, TIM_DMA_Update, ENABLE);       //ʹ��TIM8���¶�DMA�����ݴ�������
//		DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);    //ʹ��DMA�Ĵ�������ж�
//		DMA_Cmd(DMA1_Stream6, ENABLE);
//        
//		DMA_ClearITPendingBit(DMA1_Stream6,DMA_IT_TCIF6);                       //���DMA��TCIF��־
//	}
//}


/**
  * @brief  TIM4�жϴ�����
  * @param  
  * @retval None
  */
void TIM4_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET ) 
	{
        TIM_ClearFlag(TIM4, TIM_FLAG_Update);           //���TIM4�ĸ����¼���־
	}	
}





