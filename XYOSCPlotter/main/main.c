#include <stdlib.h>
#include <string.h>
#include "stm32f3xx_hal.h"
#include "common.h"
#include "arrays.h"


/* Todos
	-none
*/

UART_HandleTypeDef 			UART_Handler;
DAC_HandleTypeDef				DAC_Handler;
TIM_HandleTypeDef				DAC_Trigger;

void 		 SystemClock_Config(void);

/* DAC RELATED */
#define		dacTriggerTimer						TIM2
#define 	dacTriggerFreqency		 	 (uint32_t)250000 	// Hertz

#define 	ARRAYLENGTH 10000
char* 		XArr 			= NULL;
char* 		YArr 			= NULL;

bool		  sendNextDataFrameRequest					= false;
bool		  dataframeReceived									= false;
bool 		  sendConnectionQueryAnswer 				= false;

bool 		  nextBufferSize_Changed 						= false;
uint16_t  previousBufferSize 								= 0;
uint16_t  nextBufferSize = 0;
uint16_t  dacPer = 0;
uint16_t  dacPre = 0;

/* UART RELATED*/
#define BaudRate	3000000
const char ConnectionQueryAnswer[] 			 = "Mxx";
const char TransferCompletedAnswer[] 		 = "Txx";

/**
	-	Fxx					 incoming frame size format (z: doesn't matter) (each char)- max 65535(short)
	- Dxy					 data  format(z: doesn't matter - x/y > coordinates (in char))
	- M??		 			 uart connection request format
	- Txx					 Frame data is received completely
	- Sxx					 State Info
	- Udd					 Trigger Frequency fine tune, d : byte data
*/
int main(void)
{				
  HAL_Init();
  SystemClock_Config();

	XArr =calloc(ARRAYLENGTH,sizeof(uint8_t));
	YArr =calloc(ARRAYLENGTH,sizeof(uint8_t));
	
	if ((XArr==NULL)||(YArr==NULL))
		ErrorHandler(&UART_Handler,BufferNotInitialized);
	
	memcpy(XArr,XArrTemp,ARRAYLENGTHTemp);
	memcpy(YArr,YArrTemp,ARRAYLENGTHTemp);
	nextBufferSize = ARRAYLENGTHTemp;
	nextBufferSize_Changed = true;

	
	UART_Init(&UART_Handler,BaudRate);	
	
	CalcTimerPeriodAndPrescaler(SystemCoreClock,dacTriggerFreqency,&dacPer,&dacPre);	
	TIM_Config((TIM_HandleTypeDef*)&DAC_Trigger,dacTriggerTimer,dacPer,dacPre);
	
	DAC_ArbitraryWaveGeneration_EXTTIM(&DAC_Handler,DAC1,DAC_CHANNEL_2,&DAC_Trigger,XArr,sizeof(XArr[0]),ARRAYLENGTH,&DAC_CH2_MSPInit);
	DAC_ArbitraryWaveGeneration_EXTTIM(&DAC_Handler,DAC1,DAC_CHANNEL_1,&DAC_Trigger,YArr,sizeof(YArr[0]),ARRAYLENGTH,&DAC_CH1_MSPInit);
	HAL_Delay(10);

	HAL_TIM_Base_Start((TIM_HandleTypeDef*) &DAC_Trigger);				//  DAC Timer Started	
	HAL_Delay(100); 
	
	while(true)		{
		
		if(nextBufferSize_Changed){
			 nextBufferSize_Changed = false;							
			
			if (previousBufferSize<nextBufferSize){
					reAssignDACDMARegisters(&DAC_Handler,nextBufferSize);
			}
			else
			{
					memset(XArr+nextBufferSize,0,sizeof(char)*(unsigned)(previousBufferSize-nextBufferSize));
					memset(YArr+nextBufferSize,0,sizeof(char)*(unsigned)(previousBufferSize-nextBufferSize));
			}
											
			previousBufferSize = nextBufferSize;
			
		} else if (sendConnectionQueryAnswer){			
			sendConnectionQueryAnswer = false;
			sendStringDataMan(&UART_Handler,(char*)&ConnectionQueryAnswer[0],sizeof(ConnectionQueryAnswer)-1);
			
		} else if (sendNextDataFrameRequest){
			sendNextDataFrameRequest = false;
			sendStringDataMan(&UART_Handler,(char*)&TransferCompletedAnswer[0],sizeof(TransferCompletedAnswer)-1);			
			
		}
		
		// Sleep ... ZZZZZZZZZZZZZZZZZZZZzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
		 HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

	}
	
	free(XArr);
	free(YArr);
	
	return 0;
}

static uint16_t incomingCntr=0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	if  (RXBuffer[0]=='D'){
		
			XArr[incomingCntr] = RXBuffer[1];
			YArr[incomingCntr] = RXBuffer[2];
			incomingCntr += 1;
		
			if (incomingCntr == nextBufferSize){
					incomingCntr = 0;
					dataframeReceived = true;	
			}
			
	}else if (RXBuffer[0]=='F'){
		
			uint16_t retVal = 0;

				retVal = ((RXBuffer[1]<<8)&0xFF00) | (RXBuffer[2]&0x00FF);
		
			incomingCntr = 0;
			nextBufferSize = retVal;
			nextBufferSize_Changed 	 = true;
			
					
	}else if (RXBuffer[0]=='M'){
			/* Connection query is received*/
			sendConnectionQueryAnswer  = true;
		
	} else if ((RXBuffer[0]=='S')&&(!sendNextDataFrameRequest)){
			sendNextDataFrameRequest = true;
		
	}
	
	HAL_UART_Receive_DMA(UartHandle, (uint8_t *)RXBuffer, RXBUFFERSIZE);

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle){
	/* puts the uart into receive mode after tx completed*/
	HAL_UART_Receive_DMA(UartHandle, (uint8_t *)RXBuffer, RXBUFFERSIZE);
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    while(true){;}
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    while(true){;}
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    while(true){;}
  }
}

void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* hdac){
		if (dataframeReceived){
				dataframeReceived = false;
				sendNextDataFrameRequest = true;
		}
}

void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);

  /* System interrupt init*/
  /* MemoryManagement_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
  /* BusFault_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
  /* UsageFault_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
  /* SVCall_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);
  /* DebugMonitor_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
  /* PendSV_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);
	
//  /* SysTick_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 1);

}

	

