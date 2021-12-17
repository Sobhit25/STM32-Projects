/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
**/

#include "main.h"
#include "string.h"

UART_HandleTypeDef huart2;



/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void GPIO_Init(void);
static void USART2_UART_Init(void);

void ErrorHandler(void);

int main(void)
{
  HAL_Init();

  SystemClock_Config();

  GPIO_Init();
  USART2_UART_Init();

  char* msg;

  char *pData = "RCC Clock Modification -  with HSE\r\n";
  HAL_UART_Transmit(&huart2, (uint8_t *)pData, strlen(pData), HAL_MAX_DELAY);

  memset(msg,0,sizeof(msg));
  uint8_t clock = (uint8_t)(HAL_RCC_GetHCLKFreq()/1000000);
  sprintf(msg,"The HCLK is Running at %d Mhz\r\n",clock);
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

  memset(msg,0,sizeof(msg));
  clock = HAL_RCC_GetPCLK1Freq()/1000000;
  sprintf(msg,"The PCLK1 is Running at %d Mhz\r\n",clock);
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

  memset(msg,0,sizeof(msg));
  clock = HAL_RCC_GetPCLK2Freq()/1000000;
  sprintf(msg,"The PCLK2 is Running at %d Mhz\r\n\n",clock);
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
  HAL_Delay(2000);

  while (1);
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_Osc_Init_Struct = {0};
	RCC_ClkInitTypeDef RCC_Clk_Init_Struct = {0};

	RCC_Osc_Init_Struct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_Osc_Init_Struct.HSEState = RCC_HSE_BYPASS;

	if(HAL_RCC_OscConfig(&RCC_Osc_Init_Struct) != HAL_OK){
		ErrorHandler();
	}

	RCC_Clk_Init_Struct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_Clk_Init_Struct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	RCC_Clk_Init_Struct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_Clk_Init_Struct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_Clk_Init_Struct.APB2CLKDivider = RCC_HCLK_DIV2;

	if(HAL_RCC_ClockConfig(&RCC_Clk_Init_Struct, 0) != HAL_OK){
		ErrorHandler();
	}

	__HAL_RCC_HSI_DISABLE(); // To save some battery

	HAL_SYSTICK_Config(4000);
}


static void USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    ErrorHandler();
  }

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();


  /*Configure GPIO pin : LED */
  GPIO_InitStruct.Pin = 12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}


void ErrorHandler(void)
{
}

