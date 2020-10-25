
#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>
#include <stdbool.h>
#include "stm32f3xx_hal.h"

typedef enum errorTypes{
	NotDefined 						= 0x00,
	BufferNotInitialized
}Error_Typedef;

#define setBit	(data,bit)	 (data |= 	1<<bit)
#define resetBit(data,bit) 	 (data &= ~(1<<bit))

extern char TXBuffer[];
extern char RXBuffer[];


		#define USARTx_TX_PIN                    GPIO_PIN_9						// pin 30
		#define USARTx_TX_GPIO_PORT              GPIOA
		#define USARTx_TX_AF                     GPIO_AF7_USART1
		
		#define USARTx_RX_PIN                    GPIO_PIN_10					// pin 31
		#define USARTx_RX_GPIO_PORT              GPIOA
		#define USARTx_RX_AF                     GPIO_AF7_USART1

		#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
		#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()		
		#define USARTx_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE()
		#define DMAx_CLK_ENABLE()                __HAL_RCC_DMA1_CLK_ENABLE()
		#define USARTx_FORCE_RESET()             __HAL_RCC_USART1_FORCE_RESET()
		#define USARTx_RELEASE_RESET()           __HAL_RCC_USART1_RELEASE_RESET()

		#define USARTx                           USART1
		#define USARTx_TX_DMA_CHANNEL            DMA1_Channel4
		#define USARTx_RX_DMA_CHANNEL            DMA1_Channel5 

		#define USARTx_DMA_TX_IRQn               DMA1_Channel4_IRQn
		#define USARTx_DMA_RX_IRQn               DMA1_Channel5_IRQn
		#define USARTx_DMA_TX_IRQHandler         DMA1_Channel4_IRQHandler
		#define USARTx_DMA_RX_IRQHandler         DMA1_Channel5_IRQHandler

		#define USARTx_IRQn                      USART1_IRQn
		#define USARTx_IRQHandler                USART1_IRQHandler

		/* Size of Reception buffer */
		#define RXBUFFERSIZE                     3




void TIM_Config(TIM_HandleTypeDef* htim,TIM_TypeDef* TIMx,uint16_t period,uint16_t prescaler);
void CalcTimerPeriodAndPrescaler(uint32_t sysFreq,uint32_t DesiredTrigFreq,uint16_t* dacPer, uint16_t* dacPre);

bool DAC_ArbitraryWaveGeneration_EXTTIM(DAC_HandleTypeDef* hdac,DAC_TypeDef* DAC_,uint32_t dac_channel,TIM_HandleTypeDef* SlaveHandler,void* array,uint8_t pointerSize,uint32_t size,void (*MSPCallBack)(DAC_HandleTypeDef *hdac,uint8_t pointerSize));
void DAC_CH1_MSPInit(DAC_HandleTypeDef *hdac,uint8_t dataLength);
void DAC_CH2_MSPInit(DAC_HandleTypeDef *hdac,uint8_t dataLength);
void reAssignDACDMARegisters(DAC_HandleTypeDef* dacHandle,uint16_t newSize);
void reAssignDMARegisters(DMA_HandleTypeDef* dmaHandle,uint32_t newMemoryAddress,uint16_t newSize);


void ErrorHandler(UART_HandleTypeDef* UartHandle,Error_Typedef err);

void UART_Init(UART_HandleTypeDef* UartHandle,uint32_t baudRate);
void sendStringDataSelf(UART_HandleTypeDef *UartHandle,char* ptr);
void sendStringDataMan(UART_HandleTypeDef *UartHandle,char* ptr,uint16_t size);
void reAssignUARTDMARegisters(UART_HandleTypeDef* uartHandle,uint16_t newSize);

#endif
