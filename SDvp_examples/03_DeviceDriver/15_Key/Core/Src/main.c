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
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "key.h"
#include "led.h"
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

/* USER CODE BEGIN PV */
keyTypeDef key1, key2;
ledTypedef ledR, ledG, ledB;
uint32_t delayTime = 4;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
  key_init(&key1, "key1", KEY_ACTIVATE_LOW);
  key_init(&key2, "key2", KEY_ACTIVATE_LOW);
  led_init(&ledR, LED_ACTIVATE_LOW);
  led_init(&ledG, LED_ACTIVATE_LOW);
  led_init(&ledB, LED_ACTIVATE_LOW);
  led_set_blink(&ledR, 10, 1000, LED_INFINITY_CYCLE);
  led_set_freq(&ledB, 250);
  HAL_TIM_Base_Start_IT(&htim7);
  //  HAL_Delay(5000);
  //  led_set_off(&ledG);
  //  HAL_Delay(5000);
  //  led_set_blink(&ledG, 50, 500, 5);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    led_triger(&ledB);
    HAL_Delay(delayTime);
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
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void key_pressed_callback(keyTypeDef *key)
{
  if (key == &key2)
  {
    /* head key pressed event */
    led_set_on(&ledB);
    // HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
  }
}

void key_released_callback(keyTypeDef *key)
{
  if (key == &key2)
  {
    /* head key released event */
    led_set_off(&ledB);
    // HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_SET);
  }
}

keyLevel key_get_status(keyTypeDef *key)
{
  if (key == &key1)
  {
    /* return keyLevel */
    return (keyLevel)HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin);
  }
  else if (key == &key2)
  {
    /* return keyLevel */
    return (keyLevel)HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin);
  }
  return KEY_LOW;
}
void key_click_callback(keyTypeDef *key)
{
  if (key == &key1)
  {
    /* head key clicked event */
    delayTime++;
    // led_set_on(&ledR);
  }
}

void key_double_click_callback(keyTypeDef *key)
{
  if (key == &key1)
  {
    /* head key double click event */
    delayTime--;
    // led_set_off(&ledR);
  }
}

void key_long_press_callback(keyTypeDef *key)
{
  if (key == &key1)
  {
    /* head key long press event */
    led_set_on(&ledG);
  }
}

void key_click_long_press_callback(keyTypeDef *key)
{
  if (key == &key1)
  {
    /* head key click long press event */
    led_set_off(&ledG);
  }
}

void led_set_level(ledTypedef *led, ledActivateLevel level)
{
  if (led == &ledR)
  {
    HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, (GPIO_PinState)level);
  }
  else if (led == &ledG)
  {
    HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, (GPIO_PinState)level);
  }
  else if (led == &ledB)
  {
    HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, (GPIO_PinState)level);
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM7)
  {
    key_tick();
    led_tick();
  }
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

#ifdef USE_FULL_ASSERT
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
