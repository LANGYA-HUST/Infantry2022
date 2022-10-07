#include "main.h"
/**********************************************************************************************************
 * @文件     My_GPIO_Init.c
 * @说明     相机硬触发
 * @版本  	 V1.0
 * @作者     郭嘉豪
 * @日期     2022.7
**********************************************************************************************************/
void My_GPIO_Init(void)//初始化某个只需要用来拉高拉低的GPIO口
{
	GPIO_InitTypeDef gpio;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	/*相机外触发*/
	gpio.GPIO_Pin = GPIO_Pin_0;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB,&gpio);
	PBout(0)=0;
//	
//	/*发射机构电压采样*/
//	gpio.GPIO_Pin = GPIO_Pin_11;
//	gpio.GPIO_Mode = GPIO_Mode_IN;
//	gpio.GPIO_OType = GPIO_OType_PP;
//	gpio.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_Init(GPIOB,&gpio);
}
//软延时
int state_flag=0;
int state_num=0;//高电平持续时间计数器
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
		if(state_num>=cur_num){//高电平持续时间到达
			PBout(0) = 0;
			state_flag = 0;
		}
	}
	
}
