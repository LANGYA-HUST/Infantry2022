#include "main.h"
/**********************************************************************************************************
 * @�ļ�     My_GPIO_Init.c
 * @˵��     ���Ӳ����
 * @�汾  	 V1.0
 * @����     ���κ�
 * @����     2022.7
**********************************************************************************************************/
void My_GPIO_Init(void)//��ʼ��ĳ��ֻ��Ҫ�����������͵�GPIO��
{
	GPIO_InitTypeDef gpio;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	/*����ⴥ��*/
	gpio.GPIO_Pin = GPIO_Pin_0;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB,&gpio);
	PBout(0)=0;
//	
//	/*���������ѹ����*/
//	gpio.GPIO_Pin = GPIO_Pin_11;
//	gpio.GPIO_Mode = GPIO_Mode_IN;
//	gpio.GPIO_OType = GPIO_OType_PP;
//	gpio.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_Init(GPIOB,&gpio);
}
//����ʱ
int state_flag=0;
int state_num=0;//�ߵ�ƽ����ʱ�������
void Cmera_rising_edge(int *state,int cur_num)
{
	if(*state){
		state_flag = 1;
		state_num = 0;
		*state = 0;
	}
	if(state_flag){
		PBout(0) = 1;
		state_num++;
		if(state_num>=cur_num){//�ߵ�ƽ����ʱ�䵽��
			PBout(0) = 0;
			state_flag = 0;
		}
	}
	
}
