#ifndef _UART4_H_
#define _UART4_H_

/******************************** VOFA ����+DMA���� ***********************************/
//���ڲ�����
#define      VOFA_USART_BAUD_RATE                       115200
//���ô���
#define      VOFA_USARTx                                UART4
//����ʱ��
#define      VOFA_USART_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define      VOFA_USART_CLK                             RCC_APB1Periph_UART4
//����GPIO��ʱ��
#define      VOFA_USART_GPIO_APBxClock_FUN              RCC_AHB1PeriphClockCmd
#define      VOFA_USART_GPIO_CLK                        RCC_AHB1Periph_GPIOC

//TX��
#define      VOFA_USART_TX_PORT                         GPIOC   
#define      VOFA_USART_TX_PIN                          GPIO_Pin_10
#define      VOFA_USART_TX_AF                           GPIO_AF_UART4
#define      VOFA_USART_TX_SOURCE                       GPIO_PinSource10 

//RX��
#define      VOFA_USART_RX_PORT                         GPIOC
#define      VOFA_USART_RX_PIN                          GPIO_Pin_11
#define      VOFA_USART_RX_AF                           GPIO_AF_UART4
#define      VOFA_USART_RX_SOURCE                       GPIO_PinSource11

//TX_DMAʱ��
#define      VOFA_DMA_TX_AHBxClock_FUN                  RCC_AHB1PeriphClockCmd
#define      VOFA_DMA_TX_CLK                            RCC_AHB1Periph_DMA1

//TX_DMAͨ��
#define			 VOFA_DMA_TX_STREAM													DMA1_Stream4
#define			 VOFA_DMA_TX_CHANNEL												DMA_Channel_4
#define      VOFA_DMA_TX_IRQ                            DMA1_Stream4_IRQn
#define      VOFA_DMA_TX_INT_FUN                        DMA1_Stream4_IRQHandler
#define			 VOFA_DMA_FLAG_TCIF													DMA_FLAG_TCIF4
#define      VOFA_DMA_IT_STATUS                         DMA_IT_TCIF4
void VOFA_USART_Configuration(void);
void VOFA_Send(void);

#endif
