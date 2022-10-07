#include "main.h"
/**
  * @brief  º§π‚µ∆≈‰÷√
  * @param  None
  * @retval None
  */
void LASER_Configration(void)
{
	  GPIO_InitTypeDef gpio;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
		gpio.GPIO_Pin = GPIO_Pin_1;
		gpio.GPIO_Mode = GPIO_Mode_OUT;
		gpio.GPIO_OType = GPIO_OType_PP;
		gpio.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOC,&gpio);
}

