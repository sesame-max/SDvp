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
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sLog.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MPU_GYRO_CFG_REG 0x1B    // 陀螺仪配置
#define MPU_ACCEL_CFG_REG 0x1C   // 加速度计配置
#define MPU_ACCEL_CFG2_REG 0x1D  // 加速度计配置2
#define MPU_CFG_REG 0x1A         // 配置寄存器
#define MPU_SAMPLE_RATE_REG 0x19 // 采样率分频器

#define MPU_PWR_MGMT1_REG 0x6B // 电源管理1
#define MPU_PWR_MGMT2_REG 0x6C // 电源管理2
#define MPU_USER_CTRL_REG 0x6A // 用户控制
#define MPU_DEVICE_ID_REG 0x75 // 器件ID寄存器

#define MPU_SIGPATH_RST_REG 0X68 // 信号通道复位寄存器

#define MPU_INT_STA_REG 0X3A // 中断状态寄存器

#define MPU_ACCEL_XOUTH_REG 0X3B // 加速度值,X轴高8位寄存器
#define MPU_ACCEL_XOUTL_REG 0X3C // 加速度值,X轴低8位寄存器
#define MPU_ACCEL_YOUTH_REG 0X3D // 加速度值,Y轴高8位寄存器
#define MPU_ACCEL_YOUTL_REG 0X3E // 加速度值,Y轴低8位寄存器
#define MPU_ACCEL_ZOUTH_REG 0X3F // 加速度值,Z轴高8位寄存器
#define MPU_ACCEL_ZOUTL_REG 0X40 // 加速度值,Z轴低8位寄存器

#define MPU_TEMP_OUTH_REG 0X41 // 温度值高八位寄存器
#define MPU_TEMP_OUTL_REG 0X42 // 温度值低8位寄存器

#define MPU_GYRO_XOUTH_REG 0X43 // 陀螺仪值,X轴高8位寄存器
#define MPU_GYRO_XOUTL_REG 0X44 // 陀螺仪值,X轴低8位寄存器
#define MPU_GYRO_YOUTH_REG 0X45 // 陀螺仪值,Y轴高8位寄存器
#define MPU_GYRO_YOUTL_REG 0X46 // 陀螺仪值,Y轴低8位寄存器
#define MPU_GYRO_ZOUTH_REG 0X47 // 陀螺仪值,Z轴高8位寄存器
#define MPU_GYRO_ZOUTL_REG 0X48 // 陀螺仪值,Z轴低8位寄存器

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int16_t ACC_DATA[7];
uint8_t mpu_tx, mpu_rx; // 用于数据的发送交换
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// spi write a byte
uint8_t mpu6500_Write_Byte(uint8_t reg, uint8_t data)
{
  HAL_GPIO_WritePin(GYRO_CS_GPIO_Port, GYRO_CS_Pin, GPIO_PIN_RESET);
  mpu_tx = reg & 0x7F;                                      // 使第一位为0，写模式
  HAL_SPI_TransmitReceive(&hspi2, &mpu_tx, &mpu_rx, 1, 55); // 写入命令地址
  mpu_tx = data;
  HAL_SPI_TransmitReceive(&hspi2, &mpu_tx, &mpu_rx, 1, 55); // 写入数据
  HAL_GPIO_WritePin(GYRO_CS_GPIO_Port, GYRO_CS_Pin, GPIO_PIN_SET);
  return 0;
}

// spi read a byte
uint8_t mpu6500_Read_Byte(uint8_t reg)
{
  HAL_GPIO_WritePin(GYRO_CS_GPIO_Port, GYRO_CS_Pin, GPIO_PIN_RESET);
  mpu_tx = reg | 0x80;                                      // 使第一位为1，读模式
  HAL_SPI_TransmitReceive(&hspi2, &mpu_tx, &mpu_rx, 1, 55); // 写入读取地址
  HAL_SPI_TransmitReceive(&hspi2, &mpu_tx, &mpu_rx, 1, 55); // 为读取的数据提供存储
  HAL_GPIO_WritePin(GYRO_CS_GPIO_Port, GYRO_CS_Pin, GPIO_PIN_SET);
  return mpu_rx;
}

// 设置MPU6050陀螺仪传感器满量程范围
// fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
// 返回值:0,设置成功
//     其他,设置失败
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
  return mpu6500_Write_Byte(MPU_GYRO_CFG_REG, fsr << 3); // 设置陀螺仪满量程范围
}
// 设置MPU6050加速度传感器满量程范围
// fsr:0,±2g;1,±4g;2,±8g;3,±16g
// 返回值:0,设置成功
//     其他,设置失败
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
  return mpu6500_Write_Byte(MPU_ACCEL_CFG_REG, fsr << 3); // 设置加速度传感器满量程范围
}
// 设置MPU6050的数字低通滤波器
// lpf:数字低通滤波频率(Hz)
// 返回值:0,设置成功
//     其他,设置失败
uint8_t MPU_Set_LPF(uint16_t lpf)
{
  uint8_t data = 0;
  if (lpf >= 188)
    data = 1;
  else if (lpf >= 98)
    data = 2;
  else if (lpf >= 42)
    data = 3;
  else if (lpf >= 20)
    data = 4;
  else if (lpf >= 10)
    data = 5;
  else
    data = 6;
  return mpu6500_Write_Byte(MPU_CFG_REG, data); // 设置数字低通滤波器
}
// 设置MPU6050的采样率(假定Fs=1KHz)
// rate:4~1000(Hz)
// 返回值:0,设置成功
//     其他,设置失败
uint8_t MPU_Set_Rate(uint16_t rate)
{
  uint8_t data;
  if (rate > 1000)
    rate = 1000; // 最大1khz采样率
  if (rate < 4)
    rate = 4;
  data = 1000 / rate - 1;                               // 根据公式算出得
  data = mpu6500_Write_Byte(MPU_SAMPLE_RATE_REG, data); // 设置采样率
                                                        // return MPU_Set_LPF(rate/2);	//自动设置LPF为采样率的一半
  return 0;
}

// 同时读多个寄存器
// reg:起始寄存器地址
// len：读寄存器的总个数
//*buf：存储内存起始指针
uint8_t mpu6500_Read_Len(uint8_t reg, uint8_t len, uint8_t *buf)
{
  uint8_t tmp = 0;
  mpu_tx = reg | 0x80; // 使第一位为1，读模式
  HAL_GPIO_WritePin(GYRO_CS_GPIO_Port, GYRO_CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(&hspi2, &mpu_tx, &mpu_rx, 1, 55); // 写入读取地址
  HAL_SPI_Receive(&hspi2, buf, len, HAL_MAX_DELAY);
  HAL_GPIO_WritePin(GYRO_CS_GPIO_Port, GYRO_CS_Pin, GPIO_PIN_SET);
  return tmp;
}

void Get_MPU6500_Accelerometer(int16_t *ax, int16_t *ay, int16_t *az)
{
  uint8_t BUF[6];
  BUF[0] = mpu6500_Read_Byte(MPU_ACCEL_XOUTL_REG);
  BUF[1] = mpu6500_Read_Byte(MPU_ACCEL_XOUTH_REG);
  *ax = ((uint16_t)BUF[1] << 8) | BUF[0];

  BUF[2] = mpu6500_Read_Byte(MPU_ACCEL_YOUTL_REG);
  BUF[3] = mpu6500_Read_Byte(MPU_ACCEL_YOUTH_REG);
  *ay = ((uint16_t)BUF[3] << 8) | BUF[2];

  BUF[4] = mpu6500_Read_Byte(MPU_ACCEL_ZOUTL_REG);
  BUF[5] = mpu6500_Read_Byte(MPU_ACCEL_ZOUTH_REG);
  *az = ((uint16_t)BUF[5] << 8) | BUF[4];
}

void Get_MPU6500_GYRO(int16_t *gx, int16_t *gy, int16_t *gz)
{
  uint8_t BUF[6];
  BUF[0] = mpu6500_Read_Byte(MPU_GYRO_XOUTL_REG);
  BUF[1] = mpu6500_Read_Byte(MPU_GYRO_XOUTH_REG);
  *gx = ((uint16_t)BUF[1] << 8) | BUF[0];

  BUF[2] = mpu6500_Read_Byte(MPU_GYRO_YOUTL_REG);
  BUF[3] = mpu6500_Read_Byte(MPU_GYRO_YOUTH_REG);
  *gy = ((uint16_t)BUF[3] << 8) | BUF[2];

  BUF[4] = mpu6500_Read_Byte(MPU_GYRO_ZOUTL_REG);
  BUF[5] = mpu6500_Read_Byte(MPU_GYRO_ZOUTH_REG);
  *gz = ((uint16_t)BUF[5] << 8) | BUF[4];
}

int16_t MPU_Get_Temperature(void)
{
  uint8_t BUF[2]; // 用于存放温度
  int16_t raw;
  float temp;
  BUF[0] = mpu6500_Read_Byte(MPU_TEMP_OUTL_REG);
  BUF[1] = mpu6500_Read_Byte(MPU_TEMP_OUTH_REG);
  raw = ((uint16_t)BUF[1] << 8) | BUF[0];    // 将8位数据整合为16位
  temp = 21 + ((double)(raw - 21)) / 333.87; // 读取计算出温度

  return temp * 100; // 为了好处理数据，这里扩大100倍
}

uint8_t MPU6500_Init(void)
{
  uint8_t res;

  mpu6500_Write_Byte(MPU_PWR_MGMT1_REG, 0X80);   // 复位mpu6500
  HAL_Delay(200);                                // see 《register map》page 42 - delay 100ms
  mpu6500_Write_Byte(MPU_SIGPATH_RST_REG, 0X07); // reset GYR+ACC+TEMP
  HAL_Delay(200);                                // page 42 - delay 100ms
  mpu6500_Write_Byte(MPU_USER_CTRL_REG, 0x11);   // SET spi mode+Reset all gyro digital signal path, accel digital signal path, and temp
  HAL_Delay(1000);

  res = mpu6500_Read_Byte(MPU_DEVICE_ID_REG);
  if (res == 0x70) // 器件ID正确
  {

    MPU_Set_Gyro_Fsr(2);  // 陀螺仪传感器,±250dps
    HAL_Delay(10);        // 每写一个寄存器注意延时！不然会有意想不到的错误lol --yulong
    MPU_Set_Accel_Fsr(2); // 加速度传感器,±2g
    HAL_Delay(10);
    mpu6500_Write_Byte(MPU_CFG_REG, 0X03); // gyr Fs=1khz,bandwidth=41hz
    HAL_Delay(10);
    mpu6500_Write_Byte(MPU_ACCEL_CFG2_REG, 0X03); // Acc Fs=1khz, bandtidth=41hz
    HAL_Delay(10);
    // mpu6500_Write_Byte(MPU_INTBP_CFG_REG,0XA0);	//INT引脚低电平有效,推完输出
    HAL_Delay(10);
    // mpu6500_Write_Byte(MPU_INT_EN_REG,0X01);	//raw data inter open
    HAL_Delay(10);
    // mpu6500_Write_Byte(MPU_PWR_MGMT1_REG,0X01);	//设置CLKSEL,PLL X轴为参考
    HAL_Delay(10);
    mpu6500_Write_Byte(MPU_PWR_MGMT2_REG, 0X00); // 加速度与陀螺仪都工作
    HAL_Delay(10);
    MPU_Set_Rate(200); // 设置采样率为200Hz
    HAL_Delay(10);
  }
  else
  {
    return 1; // 错误退出
  }
  // just for test --yulong 2017/9/20
  // loop all the time(send data to com)
  while (1)
  {
    uint8_t res;
    uint8_t raw_datas[14];
    res = mpu6500_Read_Byte(MPU_INT_STA_REG); // 默认读此寄存器能够清此标志位.故循环查询此寄存器即可
    // printf("int status:%x\n", res);
    HAL_Delay(1);
    if (res == 0x01) // 数据ready
    {
      mpu6500_Read_Len(MPU_ACCEL_XOUTH_REG, 8, &raw_datas[0]);
      //HAL_Delay(1);
      mpu6500_Read_Len(MPU_GYRO_XOUTH_REG, 6, &raw_datas[8]);
      //HAL_Delay(1);

      ACC_DATA[0] = ((uint16_t)raw_datas[0] << 8) | raw_datas[1]; // 三轴加速度
      ACC_DATA[1] = ((uint16_t)raw_datas[2] << 8) | raw_datas[3];
      ACC_DATA[2] = ((uint16_t)raw_datas[4] << 8) | raw_datas[5];
      ACC_DATA[3] = ((uint16_t)raw_datas[8] << 8) | raw_datas[9]; // 三轴角速度
      ACC_DATA[4] = ((uint16_t)raw_datas[10] << 8) | raw_datas[11];
      ACC_DATA[5] = ((uint16_t)raw_datas[12] << 8) | raw_datas[13];
      ACC_DATA[6] = ((uint16_t)raw_datas[6] << 8) | raw_datas[7]; // 温度数据
      sLog_print("%d,%d,%d,%d,%d,%d,%d\n", ACC_DATA[0], ACC_DATA[1], ACC_DATA[2], ACC_DATA[3], ACC_DATA[4], ACC_DATA[5], ACC_DATA[6]);
    }
  }
  return 0;
}

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
  sLog_init();
  HAL_Delay(500);
  MPU6500_Init();
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
