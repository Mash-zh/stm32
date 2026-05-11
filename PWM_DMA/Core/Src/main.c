/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BUFFER_SIZE (64 * 32)
#define Depth 2
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;
DMA_HandleTypeDef hdma_tim2_up;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	
	/*CLK波由PB14产生，数组长�?*2*//*
	static uint16_t dma_buffer[BUFFER_SIZE*Depth*2] = { 0 };
	for (uint16_t n = 0; n < Depth; n++) {
		
		for (uint16_t i = 0; i < BUFFER_SIZE; i++) { // OE
			dma_buffer[i + n * BUFFER_SIZE] |= 1 << 0;
		}
		
		for (uint16_t i = 0; i < BUFFER_SIZE; i++) {// OE
			if (i%128 <32 + n*64)
			{
				dma_buffer[i + n * BUFFER_SIZE] &= ~(1 << 0);
			}	
		}
		
		for(uint16_t i = 0; i < BUFFER_SIZE; i++) { // ABCDE
			dma_buffer[i + n * BUFFER_SIZE] |= i/128 << 3;
		}
		for (uint16_t i = 1; i < BUFFER_SIZE; i++) { //LAT
			if (i % 128 == 0) {
				dma_buffer[i + n * BUFFER_SIZE] |= 1 << 1;
			}
		}
		if(n == 0){
			for (uint16_t i = 1; i < BUFFER_SIZE; i++) { //R1,R2 = 1
				dma_buffer[i + n * BUFFER_SIZE] |=  0x09<< 8;     //0b 00 1001
			}
		}
		if(n == 1){
			for (uint16_t i = 1; i < BUFFER_SIZE; i++) { //R1,R2 = 1
				dma_buffer[i + n * BUFFER_SIZE] |= 0x34 << 8;    //0b 11 0010
			}
		}
		
		for (uint16_t i = 1; i < BUFFER_SIZE; i++) { //CLK
			if (i % 2 != 0) {
				dma_buffer[i + n * BUFFER_SIZE] |= 1 << 14;
			}
		}
		
	} */
	/*CLK波由PB14产生，数组长�?*2*/
	
	/*CLK波由TIM2 channel2 (PA1)产生*/
	static uint16_t dma_buffer[BUFFER_SIZE*Depth] = { 0 };
	for (uint16_t n = 0; n < Depth; n++) {
		
		for (uint16_t i = 0; i < BUFFER_SIZE; i++) { // OE
			dma_buffer[i + n * BUFFER_SIZE] |= 1 << 0;
		}
		
		for (uint16_t i = 0; i < BUFFER_SIZE; i++) {// OE
			if (i%64 <10 + 2*16)
			{
				dma_buffer[i + n * BUFFER_SIZE] &= ~(1 << 0);
			}	
		}
		
		for(uint16_t i = 0; i < BUFFER_SIZE; i++) { // ABCDE
			dma_buffer[i + n * BUFFER_SIZE] |= i/64 << 3;
		}
		for (uint16_t i = 1; i < BUFFER_SIZE; i++) { //LAT
			if (i % 64 == 0) {
				dma_buffer[i + n * BUFFER_SIZE] |= 1 << 1;
			}
		}
		if(n == 0){
			for (uint16_t i = 1; i < BUFFER_SIZE; i++) { //R1,R2 = 1
				dma_buffer[i + n * BUFFER_SIZE] |=  0x29<< 8;    //0b 101 001
			}
		}
		if(n == 1){
			for (uint16_t i = 1; i < BUFFER_SIZE; i++) { //R1,R2 = 1
				dma_buffer[i + n * BUFFER_SIZE] |= 0x36 << 8;    //0b 110 110
			}
		}
		if(n == 2){
			for (uint16_t i = 1; i < BUFFER_SIZE; i++) { //R1,R2 = 1
				dma_buffer[i + n * BUFFER_SIZE] |= 0x33 << 8;    //0b 110 011
			}
		}
		
	}
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	
  /* 连接定时器与DMA， 定时器设置频率，DMA搬运数据
	*  若定时器频率为1KHz，则DMA每1ms搬运一个数据。*/
	HAL_DMA_Start(&hdma_tim2_up, (uint32_t)dma_buffer, (uint32_t)&GPIOB->ODR, sizeof(dma_buffer)/sizeof(dma_buffer[0]));
	__HAL_TIM_ENABLE_DMA(&htim2, TIM_DMA_UPDATE);
	HAL_TIM_Base_Start(&htim2);
	
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); //开启PWM CLK（PA1）
	/* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 3;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 2;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, OE_Pin|LAT_Pin|CLK_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, B1_Pin|R2_Pin|G2_Pin|B2_Pin
                          |A_Pin|B_Pin|C_Pin|D_Pin
                          |E_Pin|R1_Pin|G1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : OE_Pin LAT_Pin B1_Pin R2_Pin
                           G2_Pin B2_Pin CLK_Pin R1_Pin
                           G1_Pin */
  GPIO_InitStruct.Pin = OE_Pin|LAT_Pin|B1_Pin|R2_Pin
                          |G2_Pin|B2_Pin|CLK_Pin|R1_Pin
                          |G1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : A_Pin B_Pin C_Pin D_Pin
                           E_Pin */
  GPIO_InitStruct.Pin = A_Pin|B_Pin|C_Pin|D_Pin
                          |E_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
