# STM32教程——基于HAL库



## 基础外设教程

### 0. 环境搭建

安装前禁用Microsoft Defender，防止安装文件丢失。具体操作见[windows11彻底关闭Microsoft Defender的3种方法 - 知乎](https://zhuanlan.zhihu.com/p/494923217)，使用方法一即可。

#### keil5安装

keil5是STM32的集成开发软件(IDE)，集成了程序编写、编译和调试功能，本教程主要使用keil5作为STM32的编译器和调试软件。下面介绍keil5 的安装教程：

1. 新建软件目录文件夹

   在你喜欢的位置新建keil5的软件安装文件夹，注意软件路径最好为**纯英文**

   <img src=".\picture\0_environment\create_dir.jpg" style="zoom:80%;" />

2. 解压软件安装包中的keil5软件压缩包

   <img src=".\picture\0_environment\unzip_keil5.jpg" style="zoom:80%;" />

   <img src=".\picture\0_environment\unzip_keil5_2.jpg" style="zoom:80%;" />

3. 安装keil5

   <img src=".\picture\0_environment\install_keil5_1.jpg" style="zoom:80%;" />

   ![](.\picture\0_environment\install_keil5_2.jpg)

   ![](.\picture\0_environment\install_keil5_3.jpg)

   ![](.\picture\0_environment\install_keil5_4.jpg)

   ![](.\picture\0_environment\install_keil5_5.jpg)

   ![](.\picture\0_environment\install_keil5_6.jpg)

   等待进度条跑完后软件就安装完成了

4. 破解keil5

   <img src=".\picture\0_environment\crack_keil5_1.jpg" style="zoom: 50%;" />

   <img src=".\picture\0_environment\crack_keil5_2.jpg" style="zoom:80%;" />

   <img src=".\picture\0_environment\crack_keil5_3.jpg" style="zoom:80%;" />

   <img src=".\picture\0_environment\crack_keil5_4.jpg" style="zoom:80%;" />

   ![](.\picture\0_environment\crack_keil5_7.jpg)

   ![](.\picture\0_environment\crack_keil5_8.jpg)

   <img src=".\picture\0_environment\crack_keil5_9.jpg" style="zoom:80%;" />

   <img src=".\picture\0_environment\crack_keil5_10.jpg" style="zoom:80%;" />

5. 安装芯片pack，打开Keil5文件夹中的pack，双击即可安装对应的芯片pack，这里安装F1的pack，开发其他芯片可选择安装其他pack

   <img src=".\picture\0_environment\crack_keil5_11.jpg" style="zoom:80%;" />

   6. 至此，keil5完全安装完毕

#### CubeMx安装

#### VSCode安装

#### vofa安装

#### sourcetree安装

### 1. GPIO教程

### 2. 外部中断

### 3. 定时器教程

### 4. PWM教程

### 5. 串口教程

### 6. ADC教程

### 7. SPI教程

### 8. I2C教程

### 9. RTC教程

## 进阶应用

### 3.1 定时器进阶

### 5.1 串口进阶

### 6.1. ADC进阶

## 设备驱动

### 12. LCD触摸屏

### 13. 陀螺仪

### 14. FLASH

### 15. TF卡

### 16. AHT20