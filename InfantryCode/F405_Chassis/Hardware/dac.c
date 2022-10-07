#include "main.h"
#include "stm32f4xx_dac.h"
#include "stm32f4xx_rcc.h"
/**
  * @brief  DACͨ��1�����ʼ��
  * @param  None
  * @retval None
  */
void DAC1_Init(void)
{
  
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;


  
	/* Enable peripheral clocks ------------------------------------------------*/   
	/* GPIOA Periph clock enable */     
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);    
	/* GPIOC Periph clock enable */  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);	  //ʹ��DACͨ��ʱ�� 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;				 // �˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		 //ģ������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5)	;//PA.4 �����
					 
	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//��ʹ�ô������� TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�ò��η���
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1�������ر� BOFF1=1
  DAC_Init(DAC_Channel_1,&DAC_InitType);	 //��ʼ��DACͨ��1
	DAC_Init(DAC_Channel_2,&DAC_InitType);	 //��ʼ��DACͨ��1
	DAC_Cmd(DAC_Channel_1, ENABLE);  //ʹ��DAC1
	DAC_Cmd(DAC_Channel_2, ENABLE);  //ʹ��DAC2
  
  DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ 
  DAC_SetChannel2Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ
}

/**
  * @brief  ����ͨ��1�����ѹ
  * @param  None
  * @retval vol:0~3300,����0~3.3V
  */
void DAC1_Set_Vol(u16 vol)			//���OUT2
{
	float temp=vol;
	//temp/=1000;
	//temp=temp*4096/3.3;
	DAC_SetChannel2Data(DAC_Align_12b_R,temp);//12λ�Ҷ������ݸ�ʽ����DACֵ
}




