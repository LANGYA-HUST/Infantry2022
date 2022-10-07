#ifndef __MICROSW_H__
#define __MICROSW_H__

void MicroSwConfigration(void);

#define MicroSw_On 		GPIO_ResetBits(GPIOC,GPIO_Pin_15)
#define MicroSw_Off 	GPIO_SetBits(GPIOC,GPIO_Pin_15)
#endif
