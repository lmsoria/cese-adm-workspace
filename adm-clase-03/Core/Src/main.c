/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "asm_func.h"

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
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */

static void svc_privileges();

// 1) Realizar una función que inicialice un vector con ceros.
static void zeros(uint32_t* vector, uint32_t longitud);

// 2) Realizar una función que realice el producto de un vector y un escalar (por ejemplo, podría servir
// para cambiar el nivel de amplitud de una señal)
static void producto_escalar32(uint32_t* vector_in, uint32_t longitud, uint32_t escalar);

// 3) Adapte la función del ejercicio 2 para realizar operaciones sobre vectores de 16 bits
static void producto_escalar16(uint16_t* vector_in, uint32_t longitud, uint16_t escalar);

// 4) Adapte la función del ejercicio 3 para saturar el resultado del producto a 12 bits
static void producto_escalar12(uint16_t* vector_in, uint32_t longitud, uint16_t escalar);

// 5) Realice una función que implemente un filtro de ventana móvil de 10 valores sobre un vector de muestras
static void filtro_ventana10(uint16_t* vector_in, uint16_t* vector_out, uint32_t longitud_vector_in);

// 6) Realizar una función que reciba un vector de números signados de 32 bits y los “empaquete” en
// otro vector de 16 bits. La función deberá adecuar los valores de entrada a la nueva precisión
static void pack32_to_16(int32_t* vector_in, int16_t vector_out, uint32_t longitud);

// 7) Realizar una función que reciba un vector de números signados de 32 bits y devuelva la posición
// del máximo del vector.
static int32_t max(int32_t* vector_in, uint32_t longitud);

// 8) Realizar una función que reciba un vector de muestras signadas de 32 bits y lo decime
// descartando una cada N muestras.
static void downsample_N(int32_t* vector_in, int32_t* vector_out, uint32_t longitud, uint32_t N);

// 9) Realizar una función que reciba un vector de muestras no signadas de 16 bits e invierta su orden.
void invertir (uint16_t* vector, uint32_t longitud);

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
  MX_USART3_UART_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

//  svc_privileges();

  uint16_t vec[5] = {0, 1, 2, 3, 4};

  uint16_t vec_out[5];

//  asm_zeros(vec_out, 5);

//  asm_producto_escalar32(vec, vec_out, 5, 3);
  asm_producto_escalar16(vec, vec_out, 5, 3);

  int32_t vec32 = { -1, 5, 10, 3, 1, 0 };

  int32_t max = max(vec32, 6);


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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 384;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  RCC_OscInitStruct.PLL.PLLR = 2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

// Test what happens if we want to change the CONTROL register
static void svc_privileges()
{
	// Read CONTROL register for the fist time. Since we are in privileged mode, it should return 0.
	uint32_t control = __get_CONTROL();

	// Set LSB to change the mode to non-privileged and update CONTROL.
	control |= 0x01;
	__set_CONTROL(control);

	// Check that we are in non-privileged mode
	control = __get_CONTROL();

	// Clear LSB again, and see what happens...
	control &= ~0x01;
	__set_CONTROL(control);

	control = __get_CONTROL();

	// nothing will happen, since we are trying to change the mode from non-privileged to privileged.
	// One way of addressing this is to go to Handler mode by triggering an exception, and perform the
	// change of mode in the ISR. Let's trigger the SVC (Supervisor Call, or System service call) exception.
	asm_svc();

	// The rest of the code will be at stm32f4xx_it.c -> SVC_Handler(void)
	control = __get_CONTROL();
}

static void zeros(uint32_t* vector, uint32_t longitud)
{
	for(uint32_t i = 0; i < longitud; i++) {
		vector[i] = 0;
	}
}

static void producto_escalar32(uint32_t* vector_in, uint32_t longitud, uint32_t escalar)
{
	for(uint32_t i = 0; i < longitud; i++) {
		vector_in[i] *= escalar;
	}
}

static void producto_escalar16(uint16_t* vector_in, uint32_t longitud, uint16_t escalar)
{
	for(uint32_t i = 0; i < longitud; i++) {
		vector_in[i] *= escalar;
	}
}


static void producto_escalar12(uint16_t* vector_in, uint32_t longitud, uint16_t escalar)
{
	for(uint32_t i = 0; i < longitud; i++) {
		uint32_t res = (uint32_t)vector_in[i] * (uint32_t)escalar;
		vector_in[i] = (res > 4095) ? 4095 : (uint16_t)res;
	}
}

static void filtro_ventana10(uint16_t* vector_in, uint16_t* vector_out, uint32_t longitud_vector_in)
{

}

static void pack32_to_16(int32_t* vector_in, int16_t vector_out, uint32_t longitud)
{

}

static int32_t max(int32_t* vector_in, uint32_t longitud)
{
	int32_t max = vector_in[0];
	for(uint32_t i = 1; i <  longitud; i++) {
		max = (vector_in[i] > max) ? vector_in[i] : max;
	}

	return max;
}


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
