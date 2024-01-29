/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "POV_Display.h"
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

/* Definitions for MeasureTimeTask */
osThreadId_t MeasureTimeTaskHandle;
const osThreadAttr_t MeasureTimeTask_attributes = {
  .name = "MeasureTimeTask",
  .stack_size = 85 * 4,
  .priority = (osPriority_t) osPriorityNormal7,
};
/* Definitions for POVPrintTask */
osThreadId_t POVPrintTaskHandle;
const osThreadAttr_t POVPrintTask_attributes = {
  .name = "POVPrintTask",
  .stack_size = 85 * 4,
  .priority = (osPriority_t) osPriorityLow6,
};
/* Definitions for ClockTask */
osThreadId_t ClockTaskHandle;
const osThreadAttr_t ClockTask_attributes = {
  .name = "ClockTask",
  .stack_size = 85 * 4,
  .priority = (osPriority_t) osPriorityLow5,
};
/* Definitions for String1Task */
osThreadId_t String1TaskHandle;
const osThreadAttr_t String1Task_attributes = {
  .name = "String1Task",
  .stack_size = 85 * 4,
  .priority = (osPriority_t) osPriorityLow4,
};
/* Definitions for InitTask */
osThreadId_t InitTaskHandle;
const osThreadAttr_t InitTask_attributes = {
  .name = "InitTask",
  .stack_size = 85 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for SecondsTimer */
osTimerId_t SecondsTimerHandle;
const osTimerAttr_t SecondsTimer_attributes = {
  .name = "SecondsTimer"
};
/* Definitions for DispBinarySem */
osSemaphoreId_t DispBinarySemHandle;
const osSemaphoreAttr_t DispBinarySem_attributes = {
  .name = "DispBinarySem"
};
/* Definitions for ICUBinarySem */
osSemaphoreId_t ICUBinarySemHandle;
const osSemaphoreAttr_t ICUBinarySem_attributes = {
  .name = "ICUBinarySem"
};
/* Definitions for Del1BinarySem */
osSemaphoreId_t Del1BinarySemHandle;
const osSemaphoreAttr_t Del1BinarySem_attributes = {
  .name = "Del1BinarySem"
};
/* USER CODE BEGIN PV */

const uint8_t data[RESOLUTION] =      /* �?لسطين،حرة */
{
		0x04, 0x0c, 0x1c, 0x3c, 0x7c, 0xfc, 0x7c, 0x3c,
		0x1c, 0x0c, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x71, 0xcc, 0xd9, 0x70, 0x00, 0x80,
		0x80, 0xcc, 0x78, 0x30, 0x30, 0x30, 0x30, 0x30,


		0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
		0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x34,
		0x36, 0x16, 0x1c, 0x18, 0x18, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,


		0x70, 0xc8, 0xc1, 0xc0, 0x64, 0x38, 0x30, 0x30,
		0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0xb0, 0x30,
		0x9c, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
		0x30, 0x3f, 0x38, 0x34, 0x36, 0x36, 0x1c, 0x30,


		0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
		0x30, 0x30, 0x30, 0x30, 0x30, 0x18, 0x30, 0x30,
		0x1c, 0x30, 0x36, 0x1c, 0x30, 0x30, 0x30, 0x30,
		0x30, 0x1f, 0x30, 0x30, 0x30, 0x36, 0x15, 0x0e
};

uint8_t DayNight[3] = "PM";
const uint8_t Message[][20] =
{
	{"David S. Alexander"},
	{"Free Palestine"    },
	{"Never Give Up"     },
	{"123 ABC @#$%^&{*+("},
	{"< God Is Love >"   }
};
uint8_t MessageCounter = 0;
uint8_t Seconds     = 35;
uint8_t Minutes     = 59;
uint8_t Hours       = 12;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
void MeasureTask(void *argument);
void PrintPOVTask(void *argument);
void UpdateClockTask(void *argument);
void StringOneTask(void *argument);
void InitializationTask(void *argument);
void GetSeconds(void *argument);

/* USER CODE BEGIN PFP */
void POV_DisplayIntro(void);
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
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of DispBinarySem */
  DispBinarySemHandle = osSemaphoreNew(1, 1, &DispBinarySem_attributes);

  /* creation of ICUBinarySem */
  ICUBinarySemHandle = osSemaphoreNew(1, 1, &ICUBinarySem_attributes);

  /* creation of Del1BinarySem */
  Del1BinarySemHandle = osSemaphoreNew(1, 0, &Del1BinarySem_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of SecondsTimer */
  SecondsTimerHandle = osTimerNew(GetSeconds, osTimerPeriodic, NULL, &SecondsTimer_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  osTimerStart(SecondsTimerHandle, 1000U);
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of MeasureTimeTask */
  MeasureTimeTaskHandle = osThreadNew(MeasureTask, NULL, &MeasureTimeTask_attributes);

  /* creation of POVPrintTask */
  POVPrintTaskHandle = osThreadNew(PrintPOVTask, NULL, &POVPrintTask_attributes);

  /* creation of ClockTask */
  ClockTaskHandle = osThreadNew(UpdateClockTask, NULL, &ClockTask_attributes);

  /* creation of String1Task */
  String1TaskHandle = osThreadNew(StringOneTask, NULL, &String1Task_attributes);

  /* creation of InitTask */
  InitTaskHandle = osThreadNew(InitializationTask, NULL, &InitTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */
  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
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
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 10;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 724;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA1 PA2 PA3 PA4
                           PA5 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/**
 * @brief Display an introductory animation on the POV display.
 *
 * This function creates a visual introduction with a moving border and pixel animation.
 * It uses the POV_DrawLine, POV_WritePixel, POV_Clear, and osDelay functions to control
 * the display. The animation consists of a moving border that expands and contracts,
 * followed by a pixel animation moving across the display.
 *
 * @note This function uses osDelay for introducing delays between animation steps.
 */
void POV_DisplayIntro(void)
{
    /* Clear the POV display */
    POV_Clear();

    /* Moving border animation - expanding */
    for(uint8_t BorderCounter = 0; BorderCounter < PIXELS; BorderCounter++)
    {
        for(uint8_t LinesCounter = 0; LinesCounter < RESOLUTION; LinesCounter += 8)
        {
            POV_DrawLine(LinesCounter, 0, (LinesCounter + BorderCounter), BorderCounter);
        }
        osDelay(200);
    }

    osDelay(200);

    /* Moving border animation - contracting */
    for(uint8_t BorderCounter = PIXELS - 1; (BorderCounter >= 0 && BorderCounter < PIXELS); BorderCounter--)
    {
        for(uint8_t LinesCounter = 0; LinesCounter < RESOLUTION; LinesCounter += 8)
        {
            POV_DrawLine(LinesCounter, 0, (LinesCounter + BorderCounter), BorderCounter);
        }
        osDelay(200);
        POV_Clear();
    }

    /* Pixel animation - turning pixels on */
    for(uint8_t LinesCounter = 0; LinesCounter < (RESOLUTION - 8);)
    {
        for(uint8_t BorderCounter = 0; BorderCounter < PIXELS; BorderCounter++)
        {
            POV_WritePixel(BorderCounter, LinesCounter++, ON);
            POV_WritePixel(BorderCounter, LinesCounter++, ON);
        }
        osDelay(200);
    }

    osDelay(200);

    /* Pixel animation - turning pixels off */
    for(uint8_t LinesCounter = 0; LinesCounter < (RESOLUTION - 8);)
    {
        for(uint8_t BorderCounter = 0; BorderCounter < PIXELS; BorderCounter++)
        {
            POV_WritePixel(BorderCounter, LinesCounter++, OFF);
            POV_WritePixel(BorderCounter, LinesCounter++, OFF);
        }
        osDelay(200);
    }
}

/**
  * @brief Callback function for TIM2 input capture interrupt.
  *
  * This function is called when an input capture event occurs on TIM2.
  * It calculates the time difference between consecutive capture events, sets the period for TIM3 interrupts,
  * and resets relevant counters and registers for further measurements.
  *
  * @param htim: Pointer to the TIM_HandleTypeDef structure that contains the configuration information for TIM2.
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM2)
	{
		osSemaphoreRelease(ICUBinarySemHandle);
	}
}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_MeasureTask */
/**
  * @brief  Function implementing the MeasureTimeTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_MeasureTask */
void MeasureTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
	  osSemaphoreAcquire(ICUBinarySemHandle, osWaitForever);

	  /* Measure the period the feed the interrupt period */
	  POV_MeasurePeriod();
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_PrintPOVTask */
/**
* @brief Function implementing the POVPrintTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_PrintPOVTask */
void PrintPOVTask(void *argument)
{
  /* USER CODE BEGIN PrintPOVTask */
  /* Infinite loop */
  for(;;)
  {
	  osSemaphoreAcquire(DispBinarySemHandle, osWaitForever);
	  /* Print the required pattern */
      POV_ControlFrames();
  }
  /* USER CODE END PrintPOVTask */
}

/* USER CODE BEGIN Header_UpdateClockTask */
/**
* @brief Function implementing the ClockTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_UpdateClockTask */
void UpdateClockTask(void *argument)
{
  /* USER CODE BEGIN UpdateClockTask */
  /* Infinite loop */
  for(;;)
  {
	osSemaphoreAcquire(Del1BinarySemHandle, osWaitForever);
    POV_Clear();
    uint8_t SecCounter = 0;
    /* Update the clock 4 seconds */
    for(; SecCounter < 8; SecCounter++)
    {
  	  POV_SetCursor(0);
  	  POV_WriteInteger(Hours/10);
  	  POV_WriteInteger(Hours%10);
  	  POV_WriteChar(':');
  	  POV_WriteInteger(Minutes/10);
  	  POV_WriteInteger(Minutes%10);
  	  POV_WriteChar(':');
  	  POV_WriteInteger(Seconds/10);
  	  POV_WriteInteger(Seconds%10);
  	  POV_WriteChar(' ');
  	  POV_WriteString(DayNight);

  	  /* Write the RPM counts at the position 16 */
  	  POV_WriteIntegerInPos(POV_MeasureRPM(), 16);
  	  POV_WriteString((const uint8_t*)" RPM");
  	  osDelay(500);
    }
    osSemaphoreRelease(Del1BinarySemHandle);
    osDelay(3000);
  }
  /* USER CODE END UpdateClockTask */
}

/* USER CODE BEGIN Header_StringOneTask */
/**
* @brief Function implementing the String1Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StringOneTask */
void StringOneTask(void *argument)
{
  /* USER CODE BEGIN StringOneTask */
  /* Infinite loop */
  for(;;)
  {
	osSemaphoreAcquire(Del1BinarySemHandle, osWaitForever);
    POV_Clear();
    uint8_t DelCounter = 0;
    for(; DelCounter < 12; DelCounter++)
    {
      POV_Clear();
      POV_WriteStringInPos(Message[MessageCounter], DelCounter);
      osDelay(250);
    }
    osSemaphoreRelease(Del1BinarySemHandle);
    /* Update Counter position for the next message */
    MessageCounter = (MessageCounter + 1) % 5;
  }
  /* USER CODE END StringOneTask */
}

/* USER CODE BEGIN Header_InitializationTask */
/**
* @brief Function implementing the InitTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_InitializationTask */
void InitializationTask(void *argument)
{
  /* USER CODE BEGIN InitializationTask */
  /* Infinite loop */
  POV_Init();
  POV_DrawBitmap(data, (sizeof(data) / sizeof(data[0])));
  osDelay(8000);
  POV_DisplayIntro();
  osSemaphoreRelease(Del1BinarySemHandle);

  /* Task to initialize once and terminate */
  osThreadTerminate(InitTaskHandle);
  /* USER CODE END InitializationTask */
}

/* GetSeconds function */
void GetSeconds(void *argument)
{
  /* USER CODE BEGIN GetSeconds */
	  Seconds++;
	  if (Seconds >= 60)
	  {
		  Seconds = 0;
		  Minutes++;
		  if (Minutes >= 60)
		  {
			  Minutes = 0;
			  Hours++;
			  if (Hours >= 13)
			  {
				  Hours = 1;
				  if (strcmp((char*)DayNight, "PM") == 0)
				  {
					  /* DayNight is "PM", so change it to "AM" */
					  memcpy(DayNight, "AM", sizeof("AM") - 1);
				  }
				  else
				  {
					  /* DayNight is "AM", change it to "PM" */
					  memcpy(DayNight, "PM", sizeof("PM") - 1);
				  }
			  }
		  }
	  }
  /* USER CODE END GetSeconds */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  /* Check if the interrupt is triggered by TIM2 */
  else if (htim->Instance == TIM2)
  {
	  /* Increment the overflow counter for TIM2 */
      ICU_TIM_OVC++;
  }
  /* Check if the interrupt is triggered by TIM3 */
  else if (htim->Instance == TIM3)
  {
	  osSemaphoreRelease(DispBinarySemHandle);
  }
  /* USER CODE END Callback 1 */
}

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
