#ifndef _USART2_H_
#define _USART2_H_

/******************************** VOFA ����+DMA���� ***********************************/
//���ڲ�����
#define      VOFA_USART_BAUD_RATE                       115200
//���ô���
#define      VOFA_USARTx                                USART2
//����ʱ��
#define      VOFA_USART_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define      VOFA_USART_CLK                             RCC_APB1Periph_USART2
//����GPIO��ʱ��
#define      VOFA_USART_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define      VOFA_USART_GPIO_CLK                        RCC_APB2Periph_GPIOA

//TX��
#define      VOFA_USART_TX_PORT                         GPIOA   
#define      VOFA_USART_TX_PIN                          GPIO_Pin_2


//RX��
#define      VOFA_USART_RX_PORT                         GPIOA
#define      VOFA_USART_RX_PIN                          GPIO_Pin_3

//TX_DMAʱ��
#define      VOFA_DMA_TX_AHBxClock_FUN                  RCC_AHBPeriphClockCmd
#define      VOFA_DMA_TX_CLK                            RCC_AHBPeriph_DMA1

//TX_DMAͨ��
#define			 VOFA_DMA_TX_CHANNEL												DMA1_Channel7
#define      VOFA_DMA_TX_IRQ                            DMA1_Channel7_IRQn
#define      VOFA_DMA_TX_INT_FUN                        DMA1_Channel7_IRQHandler
#define			 VOFA_DMA_FLAG_TCIF													DMA1_FLAG_TC7
#define      VOFA_DMA_IT                                DMA1_IT_TC7

void VOFA_USART_Configuration(void);
void VOFA_Send(void);

#endif
