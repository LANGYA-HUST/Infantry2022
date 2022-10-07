#ifndef __IWDG_H
#define __IWDG_H

#include "stm32f4xx_iwdg.h"

void IWDG_Config(uint8_t prv ,uint16_t rlv);
void IWDG_Feed(void);

#endif
