/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
// ICM-42688-P寄存器定义
#define WHO_AM_I_REG 0x75
#define PWR_MGMT0_REG 0x4E
#define GYRO_CONFIG0_REG 0x4F
#define ACCEL_CONFIG0_REG 0x50
#define FIFO_CONFIG_REG 0x16
#define FIFO_DATA_REG 0x34
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int16_t rawAccelerationData[3] = {0};
int16_t rawAngleData[3] = {0};
float accelerationData[3] = {0.0};
float angleData[3] = {0.0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void ICM42688_WriteRegister(uint8_t reg, uint8_t data) {
  uint8_t tx_buffer[2] = {reg & 0x7F, data}; // 清除最高位(写操作)

  HAL_GPIO_WritePin(GYRO_CS_GPIO_Port, GYRO_CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi2, tx_buffer, 2, HAL_MAX_DELAY);
  HAL_GPIO_WritePin(GYRO_CS_GPIO_Port, GYRO_CS_Pin, GPIO_PIN_SET);
}

uint8_t ICM42688_ReadRegister(uint8_t reg) {
  uint8_t tx_buffer[2] = {reg | 0x80, 0x00}; // 设置最高位(读操作)
  uint8_t rx_buffer[2] = {0};

  HAL_GPIO_WritePin(GYRO_CS_GPIO_Port, GYRO_CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(&hspi2, tx_buffer, rx_buffer, 2, HAL_MAX_DELAY);
  HAL_GPIO_WritePin(GYRO_CS_GPIO_Port, GYRO_CS_Pin, GPIO_PIN_SET);
  return rx_buffer[1];
}
void ICM42688_Init(void) {
  uint8_t whoami;
  uint8_t tx_data;

  // 复位设备
  ICM42688_WriteRegister(PWR_MGMT0_REG, 0x00);
  HAL_Delay(10);

  // 检查WHO_AM_I
  whoami = ICM42688_ReadRegister(WHO_AM_I_REG);
  if(whoami != 0x47) { // ICM-42688-P的WHO_AM_I值应为0x42
      while(1);
  }

  // 配置电源管理
  tx_data = 0x0F; // 启用加速度计和陀螺仪
  ICM42688_WriteRegister(PWR_MGMT0_REG, tx_data);

  // 配置陀螺仪
  tx_data = (0x03 << 5) | 0x06; // ±500dps, 1kHz ODR
  ICM42688_WriteRegister(GYRO_CONFIG0_REG, tx_data);

  // 配置加速度计
  tx_data = (0x01 << 5) | 0x06; // ±8g, 1kHz ODR
  ICM42688_WriteRegister(ACCEL_CONFIG0_REG, tx_data);

  // 配置FIFO
  tx_data = 0x03; // 启用加速度计和陀螺仪数据到FIFO
  ICM42688_WriteRegister(FIFO_CONFIG_REG, tx_data);

  HAL_Delay(100); // 等待传感器稳定
}
// 读取传感器数据
void ReadSensorData(int16_t* accel, int16_t* gyro) {
  // 读取加速度计数据
  accel[0] = (int16_t)((ICM42688_ReadRegister(0x1F) << 8) | ICM42688_ReadRegister(0x20));
  accel[1] = (int16_t)((ICM42688_ReadRegister(0x21) << 8) | ICM42688_ReadRegister(0x22));
  accel[2] = (int16_t)((ICM42688_ReadRegister(0x23) << 8) | ICM42688_ReadRegister(0x24));

  // 读取陀螺仪数据
  gyro[0] = (int16_t)((ICM42688_ReadRegister(0x25) << 8) | ICM42688_ReadRegister(0x26));
  gyro[1] = (int16_t)((ICM42688_ReadRegister(0x27) << 8) | ICM42688_ReadRegister(0x28));
  gyro[2] = (int16_t)((ICM42688_ReadRegister(0x29) << 8) | ICM42688_ReadRegister(0x2A));

}

// 将原始数据转换为实际值
void ConvertRawData(int16_t raw_accel[3], int16_t raw_gyro[3], float* accel_g, float* gyro_dps) {
  // 加速度计转换 (±8g范围)
  for(int i = 0; i < 3; i++) {
      accel_g[i] = (float)raw_accel[i] / 4096.0f; // 4096 LSB/g (对于±8g范围)
  }

  // 陀螺仪转换 (±500dps范围)
  for(int i = 0; i < 3; i++) {
      gyro_dps[i] = (float)raw_gyro[i] / 65.5f; // 65.5 LSB/dps (对于±500dps范围)
  }
}

#define TEMP_SENSITIVITY 132.48f  // LSB/℃
#define TEMP_OFFSET      25.0f    // 25℃时输出0

float ICM42688_ReadTemperature(void) {
  // 读取两个8位寄存器并组合成16位数据
  int16_t temp_raw = (int16_t)((ICM42688_ReadRegister(0x1D) << 8) | ICM42688_ReadRegister(0x1E));

  // 转换为实际温度值
  return ((float)temp_raw / TEMP_SENSITIVITY) + TEMP_OFFSET;
}
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
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */
  ICM42688_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    ReadSensorData(rawAccelerationData,rawAngleData);
    ConvertRawData(rawAccelerationData,rawAngleData,accelerationData,angleData);
    MyPrintf("%f,%f,%f,%f,%f,%f\r\n",accelerationData[0],accelerationData[1],accelerationData[2],angleData[0],angleData[1],angleData[2]);
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
