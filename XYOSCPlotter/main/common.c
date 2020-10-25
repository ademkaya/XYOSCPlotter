
#include <stdlib.h>
#include "common.h"


static uint32_t my_strlen(const char *s);
char TXBuffer[RXBUFFERSIZE];
char RXBuffer[RXBUFFERSIZE];



void TIM_Config(TIM_HandleTypeDef* htim,TIM_TypeDef* TIMx,uint16_t period,uint16_t prescaler){

	TIM_MasterConfigTypeDef sMasterConfig;
  TIM_ClockConfigTypeDef  sClockSourceConfig;
	
  /*##-1- Configure the TIM peripheral #######################################*/
  /* Time base configuration */
  htim->Instance 							 = TIMx;
	htim->Init.Prescaler         = prescaler;
  htim->Init.Period            = period;
  htim->Init.ClockDivision     = 0;
  htim->Init.CounterMode       = TIM_COUNTERMODE_UP;
  htim->Init.RepetitionCounter = 0;
  htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(htim);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(htim, &sClockSourceConfig) != HAL_OK) { ;  }
			
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(htim, &sMasterConfig) != HAL_OK) { ;  }

	//HAL_NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 0, 0);
	//HAL_NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);
	
  /*##-2- Enable TIM peripheral counter ######################################*/
  // outside
}

/*
		sysFreq 					in Khz
		DesiredTrigFreq		in Hertz
*/
void CalcTimerPeriodAndPrescaler(uint32_t sysFreq,uint32_t DesiredTrigFreq,uint16_t* dacPer, uint16_t* dacPre){

	uint32_t temp = (sysFreq) / DesiredTrigFreq;
	uint32_t tempDacPer = 0;
	*dacPre = 1;
	*dacPer = 1;	
	
	while(true){
	tempDacPer = 	temp / (*dacPre);
		
		if (tempDacPer<0xffff)
			break;
		
		*dacPre *= 2;
	}
	
	*dacPer = (uint16_t)(tempDacPer - 1);
	*dacPre -= 1;
	
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM1){
		__HAL_RCC_TIM1_CLK_ENABLE();
	}else if  (htim->Instance == TIM2){
		__HAL_RCC_TIM2_CLK_ENABLE();
	}else if (htim->Instance == TIM3){
		__HAL_RCC_TIM3_CLK_ENABLE();	
	}else if (htim->Instance == TIM4){
		__HAL_RCC_TIM4_CLK_ENABLE();	
	}else if (htim->Instance == TIM6){
		__HAL_RCC_TIM6_CLK_ENABLE();	
	}else if (htim->Instance == TIM7){
		__HAL_RCC_TIM7_CLK_ENABLE();	
	}else if (htim->Instance == TIM8){
		__HAL_RCC_TIM8_CLK_ENABLE();	
	}else if (htim->Instance == TIM15){
		__HAL_RCC_TIM15_CLK_ENABLE();	
	}else if (htim->Instance == TIM16){
		__HAL_RCC_TIM16_CLK_ENABLE();	
	}else if (htim->Instance == TIM17){
		__HAL_RCC_TIM17_CLK_ENABLE();	
	}
}

bool DAC_ArbitraryWaveGeneration_EXTTIM(DAC_HandleTypeDef* hdac,DAC_TypeDef* DAC_,uint32_t dac_channel,TIM_HandleTypeDef* SlaveHandler,void* array,uint8_t pointerSize,uint32_t size,void (*MSPCallBack)(DAC_HandleTypeDef *hdac,uint8_t pointerSize)){
	
	DAC_ChannelConfTypeDef sConfig;

	if (pointerSize>2)
		return true;
	
	hdac->Instance = DAC_;

  if (HAL_DAC_Init(hdac) != HAL_OK) { ; }

	MSPCallBack(hdac,pointerSize*8); /* pointer points a xxx array */
	
	if 				 (SlaveHandler->Instance == TIM2 ) {	sConfig.DAC_Trigger = DAC_TRIGGER_T2_TRGO;  }
	else if  	 (SlaveHandler->Instance == TIM3 ) {	sConfig.DAC_Trigger = DAC_TRIGGER_T3_TRGO;  }
	else if 	 (SlaveHandler->Instance == TIM4 ) {  sConfig.DAC_Trigger = DAC_TRIGGER_T4_TRGO;  }
  else if 	 (SlaveHandler->Instance == TIM6 ) {	sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;  }
	else if  	 (SlaveHandler->Instance == TIM7 ) {  sConfig.DAC_Trigger = DAC_TRIGGER_T7_TRGO;  }
	else if 	 (SlaveHandler->Instance == TIM15) {  sConfig.DAC_Trigger = DAC_TRIGGER_T15_TRGO; }	

  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

  if (HAL_DAC_ConfigChannel(hdac, &sConfig, dac_channel) != HAL_OK) { while(1){;} }

	if (sConfig.DAC_Trigger == DAC_TRIGGER_T3_TRGO)
		__HAL_REMAPTRIGGER_ENABLE(HAL_REMAPTRIGGER_DAC1_TRIG);  
	
	if (HAL_DAC_Start_DMA(hdac, dac_channel, (uint32_t *)array, size, (16-pointerSize*8)) != HAL_OK) { while(1){;} }
	
	return false;
}


void reAssignDACDMARegisters(DAC_HandleTypeDef* dacHandle,uint16_t newSize){
	
	/* stop dac dma's */
	if (dacHandle->DMA_Handle1)
		dacHandle->DMA_Handle1->Instance->CCR &= ~(1<<0);
	if (dacHandle->DMA_Handle2)
		dacHandle->DMA_Handle2->Instance->CCR &= ~(1<<0);
	
	/* assign new size */
	dacHandle->DMA_Handle1->Instance->CNDTR = newSize;
	dacHandle->DMA_Handle2->Instance->CNDTR = newSize;
	
		/* start dma's */
	dacHandle->DMA_Handle1->Instance->CCR |=  (1<<0);
	dacHandle->DMA_Handle2->Instance->CCR |=  (1<<0);
	
}

void reAssignUARTDMARegisters(UART_HandleTypeDef* uartHandle,uint16_t newSize){
	
	if (!uartHandle->hdmarx)
			return;
	
		uartHandle->hdmarx->Instance->CCR  &= ~(1<<0);	
		uartHandle->hdmarx->Instance->CNDTR = newSize;
		uartHandle->hdmarx->Instance->CCR  |=  (1<<0);
	
}


void reAssignDMARegisters(DMA_HandleTypeDef* dmaHandle,uint32_t newMemoryAddress,uint16_t newSize){
	/* stop dma */
	dmaHandle->Instance->CCR &= ~(1<<0);
	/*Assign new memoryAddress*/
	
	if (newMemoryAddress)
	dmaHandle->Instance->CMAR = newMemoryAddress;
	
	/* assign new size */
	dmaHandle->Instance->CNDTR = newSize;
	/* start dma */
	dmaHandle->Instance->CCR |=  (1<<0);
}

void DAC_CH2_MSPInit(DAC_HandleTypeDef *hdac,uint8_t pointerSize){

	 static DMA_HandleTypeDef  hdma_dac_ch2;
	 GPIO_InitTypeDef          GPIO_InitStruct;
	
			__HAL_RCC_GPIOA_CLK_ENABLE();
			
			__HAL_RCC_DAC1_CLK_ENABLE();
			
			__HAL_RCC_DMA2_CLK_ENABLE();
	
			GPIO_InitStruct.Pin  = GPIO_PIN_5;
			GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
			HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

			/* Set the parameters to be configured for DACx_DMA1_CHANNEL3 */
			hdma_dac_ch2.Instance = 	DMA2_Channel4;

			hdma_dac_ch2.Init.Direction 						= DMA_MEMORY_TO_PERIPH;
			hdma_dac_ch2.Init.PeriphInc 						= DMA_PINC_DISABLE;
			hdma_dac_ch2.Init.MemInc 								= DMA_MINC_ENABLE;
			
			if (pointerSize==32)				{		/*WORD*/
				hdma_dac_ch2.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
				hdma_dac_ch2.Init.MemDataAlignment 		= DMA_MDATAALIGN_WORD;
			} else if (pointerSize==16)	{		/*HALFWORD*/
				hdma_dac_ch2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
				hdma_dac_ch2.Init.MemDataAlignment 		= DMA_MDATAALIGN_HALFWORD;
			}	else if (pointerSize==8)	{		/*BYTE*/
				hdma_dac_ch2.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
				hdma_dac_ch2.Init.MemDataAlignment 		= DMA_MDATAALIGN_BYTE;
			}
			
			hdma_dac_ch2.Init.Mode 								= DMA_CIRCULAR;
			hdma_dac_ch2.Init.Priority 						= DMA_PRIORITY_HIGH;

			HAL_DMA_Init(&hdma_dac_ch2);

			/* Associate the initialized DMA handle to the the DAC handle */
			__HAL_LINKDMA(hdac, DMA_Handle2, hdma_dac_ch2);
	
}
void DAC_CH1_MSPInit(DAC_HandleTypeDef *hdac,uint8_t pointerSize){
	 static DMA_HandleTypeDef  hdma_dac_ch1;
	 GPIO_InitTypeDef          GPIO_InitStruct;
	
			__HAL_RCC_GPIOA_CLK_ENABLE();
			
			__HAL_RCC_DAC1_CLK_ENABLE();
			
			__HAL_RCC_DMA2_CLK_ENABLE();
	
			GPIO_InitStruct.Pin  = GPIO_PIN_4;
			GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
			HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

			/*##-3- Configure the DMA ##########################################*/
			/* Set the parameters to be configured for DACx_DMA1_CHANNEL3 */
			hdma_dac_ch1.Instance = 	DMA2_Channel3;
			
			hdma_dac_ch1.Init.Direction 					= DMA_MEMORY_TO_PERIPH;
			hdma_dac_ch1.Init.PeriphInc 					= DMA_PINC_DISABLE;
			hdma_dac_ch1.Init.MemInc 							= DMA_MINC_ENABLE;
			
			if (pointerSize==32){						/*WORD*/
				hdma_dac_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
				hdma_dac_ch1.Init.MemDataAlignment 		= DMA_MDATAALIGN_WORD;
			}else if (pointerSize==16){			/*HALFWORD*/
				hdma_dac_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
				hdma_dac_ch1.Init.MemDataAlignment 		= DMA_MDATAALIGN_HALFWORD;
			}else if (pointerSize==8){			/*BYTE*/
				hdma_dac_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
				hdma_dac_ch1.Init.MemDataAlignment 		= DMA_MDATAALIGN_BYTE;
			}
			
			hdma_dac_ch1.Init.Mode 								= DMA_CIRCULAR;
			hdma_dac_ch1.Init.Priority 						= DMA_PRIORITY_HIGH;

			HAL_DMA_Init(&hdma_dac_ch1);

			/* Associate the initialized DMA handle to the the DAC handle */
			__HAL_LINKDMA(hdac, DMA_Handle1, hdma_dac_ch1);
			
			
			#if true		/* enable interrupt */
			/* NVIC configuration for DMA Input data interrupt */
			HAL_NVIC_SetPriority(DMA2_Channel3_IRQn, 0, 0);
			HAL_NVIC_EnableIRQ(DMA2_Channel3_IRQn);			
			#endif
			
}

void UART_Init(UART_HandleTypeDef* UartHandle,uint32_t baudRate){ 
	
	
	UartHandle->Instance        = USARTx;
  UartHandle->Init.BaudRate   = baudRate;
  UartHandle->Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle->Init.StopBits   = UART_STOPBITS_1;
  UartHandle->Init.Parity     = UART_PARITY_NONE;
  UartHandle->Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle->Init.Mode       = UART_MODE_TX_RX;
  
	UartHandle->pRxBuffPtr 			= (uint8_t*)&RXBuffer[0];
	UartHandle->pTxBuffPtr 			= (uint8_t*)&TXBuffer[0];

  if(HAL_UART_Init(UartHandle) != HAL_OK) { ; }

	HAL_UART_Receive_DMA(UartHandle, (uint8_t *)RXBuffer, RXBUFFERSIZE);
	
}

/**
  * @brief UART MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  *           - DMA configuration for transmission request by peripheral 
  *           - NVIC configuration for DMA interrupt request enable
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
 GPIO_InitTypeDef  GPIO_InitStruct;

	static DMA_HandleTypeDef hdma_tx;
	static DMA_HandleTypeDef hdma_rx;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* Enable GPIO clock */
	USARTx_TX_GPIO_CLK_ENABLE();
	USARTx_RX_GPIO_CLK_ENABLE();

	/* Enable USARTx clock */
	USARTx_CLK_ENABLE();

	/* Enable DMA clock */
	DMAx_CLK_ENABLE();

	/*##-2- Configure peripheral GPIO ##########################################*/
	/* UART TX GPIO pin configuration  */
	GPIO_InitStruct.Pin       			 = USARTx_TX_PIN;
	GPIO_InitStruct.Mode      			 = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      			 = GPIO_PULLUP;
	GPIO_InitStruct.Speed     			 = GPIO_SPEED_FREQ_HIGH;  
	GPIO_InitStruct.Alternate 			 = USARTx_TX_AF;
	HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

	/* UART RX GPIO pin configuration  */
	GPIO_InitStruct.Pin 						 = USARTx_RX_PIN;
	GPIO_InitStruct.Alternate 			 = USARTx_RX_AF;
	HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);

	/*##-3- Configure the DMA ##################################################*/
	/* Configure the DMA handler for Transmission process */
	hdma_tx.Instance                 = USARTx_TX_DMA_CHANNEL;
	hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
	hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
	hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	hdma_tx.Init.Mode                = DMA_NORMAL;
	hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
	//	hdma_tx.XferCpltCallback				 = dma_tx_xfercmplt;
	HAL_DMA_Init(&hdma_tx);

	/* Associate the initialized DMA handle to the UART handle */
	__HAL_LINKDMA(huart, hdmatx, hdma_tx);

	/* Configure the DMA handler for reception process */
	hdma_rx.Instance                 = USARTx_RX_DMA_CHANNEL;
	hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
	hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
	hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	hdma_rx.Init.Mode                = DMA_NORMAL;
	hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
	//	hdma_rx.XferCpltCallback				 = dma_rx_xfercmplt;

	HAL_DMA_Init(&hdma_rx);

	/* Associate the initialized DMA handle to the the UART handle */
	__HAL_LINKDMA(huart, hdmarx, hdma_rx);

	/*##-4- Configure the NVIC for DMA #########################################*/
	/* NVIC configuration for DMA transfer complete interrupt (USARTx_TX) */
	HAL_NVIC_SetPriority(USARTx_DMA_TX_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ	(USARTx_DMA_TX_IRQn			 );

	/* NVIC configuration for DMA transfer complete interrupt (USARTx_RX) */
	HAL_NVIC_SetPriority(USARTx_DMA_RX_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ	(USARTx_DMA_RX_IRQn			 );

	/* NVIC configuration for USART, to catch the TX complete */
	HAL_NVIC_SetPriority(USARTx_IRQn, 1, 2);
	HAL_NVIC_EnableIRQ	(USARTx_IRQn			);
 
}

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{

  /*##-1- Reset peripherals ##################################################*/
  USARTx_FORCE_RESET();
  USARTx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Configure USARTx Tx as alternate function  */
  HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
  /* Configure USARTx Rx as alternate function  */
  HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);
   
  /*##-3- Disable the DMA #####################################################*/
  /* De-Initialize the DMA channel associated to reception process */
  if(huart->hdmarx != 0)
  {
    HAL_DMA_DeInit(huart->hdmarx);
  }
  /* De-Initialize the DMA channel associated to transmission process */
  if(huart->hdmatx != 0)
  {
    HAL_DMA_DeInit(huart->hdmatx);
  }  
  
  /*##-4- Disable the NVIC for DMA ###########################################*/
  HAL_NVIC_DisableIRQ(USARTx_DMA_RX_IRQn);
}


void ErrorHandler(UART_HandleTypeDef* UartHandle,Error_Typedef err){
	while(1){
		
		if (err==BufferNotInitialized)
			sendStringDataSelf(UartHandle,"BufferNotInitialized\n");
		
		
			HAL_Delay(100);
	}
}

void sendStringDataSelf(UART_HandleTypeDef *UartHandle,char* ptr){
		HAL_UART_Transmit_DMA((UART_HandleTypeDef *)UartHandle, (uint8_t*)ptr, my_strlen((const char*)ptr));
}

void sendStringDataMan(UART_HandleTypeDef *UartHandle,char* ptr,uint16_t size){
		HAL_UART_Transmit_DMA((UART_HandleTypeDef *)UartHandle, (uint8_t*)ptr, size);
}

static uint32_t my_strlen(const char *s){	
  const char *anchor = s;
  while(*s)
   s++;
  return (s - anchor);
}
