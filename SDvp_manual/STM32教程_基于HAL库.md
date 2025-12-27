# STM32教程——基于HAL库



## 基础外设教程

### 0. 环境搭建

安装前禁用Microsoft Defender，防止安装文件丢失。具体操作见[windows11彻底关闭Microsoft Defender的3种方法 - 知乎](https://zhuanlan.zhihu.com/p/494923217)，使用方法一即可。

#### keil5安装

**keil5**是STM32的集成开发软件(IDE)，集成了程序编写、编译和调试功能，本教程主要使用**keil5**作为STM32的编译器和调试软件。下面介绍keil5 的安装教程：

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

**STM32CubeMX** 是意法半导体（STMicroelectronics）为STM32系列微控制器开发的**图形化配置工具和代码生成器**。它极大地简化了STM32项目的初始化和外设配置流程。通过该工具我们可以直观观察到STM32的外设配置，自动生成初始化代码不但减少了初始化外设的工作量，也能够避免初始化错误导致外设不能正常工作。下面介绍**CubeMX**的安装：

1. 解压CubeMx

   <img src=".\picture\0_environment\unzip_cubemx_1.jpg" style="zoom:80%;" />

   <img src=".\picture\0_environment\unzip_cubemx_2.jpg" style="zoom:100%;" />

2. 安装CubeMx

   <img src=".\picture\0_environment\install_cubemx_1.jpg" style="zoom:80%;" />

<img src=".\picture\0_environment\install_cubemx_2.jpg" style="zoom:100%;" />

<img src=".\picture\0_environment\install_cubemx_3.jpg" style="zoom:100%;" />

<img src=".\picture\0_environment\install_cubemx_4.jpg" style="zoom:100%;" />

<img src=".\picture\0_environment\install_cubemx_5.jpg" style="zoom:100%;" />



<img src=".\picture\0_environment\install_cubemx_6.jpg" style="zoom:100%;" />

<img src=".\picture\0_environment\install_cubemx_7.jpg" style="zoom:100%;" />

<img src=".\picture\0_environment\install_cubemx_8.jpg" style="zoom:100%;" />

<img src=".\picture\0_environment\install_cubemx_9.jpg" style="zoom:100%;" />

#### VSCode安装

**Visual Studio Code** 是微软开发的一款**免费、开源、跨平台**的现代化代码编辑器。由于**Keil**自带的代码编辑器使用不便，这里安装**VSCode**作为代码的编辑器，**Keil**仅作为编译和调试工具。下面介绍**VSCode**的安装教程：

1. 安装VSCode

   <img src=".\picture\0_environment\install_vscode_1.jpg" style="zoom:80%;" />

   <img src=".\picture\0_environment\install_vscode_2.jpg" style="zoom:100%;" />

   <img src=".\picture\0_environment\install_vscode_3.jpg" style="zoom:100%;" />

   <img src=".\picture\0_environment\install_vscode_4.jpg" style="zoom:100%;" />

   <img src=".\picture\0_environment\install_vscode_5.jpg" style="zoom:100%;" />

   <img src=".\picture\0_environment\install_vscode_6.jpg" style="zoom:100%;" />

   <img src=".\picture\0_environment\install_vscode_7.jpg" style="zoom:100%;" />

2. 安装中文插件

   <img src=".\picture\0_environment\install_vscode_chinese_1.jpg" style="zoom:80%;" />

   <img src=".\picture\0_environment\install_vscode_chinese_2.jpg" style="zoom:80%;" />

   <img src=".\picture\0_environment\install_vscode_chinese_3.jpg" style="zoom:80%;" />

   <img src=".\picture\0_environment\install_vscode_chinese_4.jpg" style="zoom:80%;" />

   <img src=".\picture\0_environment\install_vscode_chinese_5.jpg" style="zoom:80%;" />

3. 安装c/c++插件

   <img src=".\picture\0_environment\install_vscode_c_1.jpg" style="zoom:80%;" />

4. 安装翻译插件var-translate-en

   <img src=".\picture\0_environment\install_vscode_translate_1.jpg" style="zoom:80%;" />

#### VOFA安装

**VOFA+**（常被译为“伏特加”）是一款面向嵌入式开发的**数据可视化调试上位机软件**。它的核心目标是**简化调试过程**，通过直观的图形界面取代传统的命令行串口调试工具，让开发者能“看见”数据。可以说**VOFA**是一个增强版的串口助手，不仅能够发送和接收串口数据，也能够解析特定格式的数据并将数据绘制出来。下面介绍**VOFA**的安装教程

<img src=".\picture\0_environment\install_vofa_1.jpg" style="zoom:80%;" />

<img src=".\picture\0_environment\install_vofa_2.jpg" style="zoom:100%;" />

<img src=".\picture\0_environment\install_vofa_3.jpg" style="zoom:100%;" />

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

### 10. AHT20

### 11. 陀螺仪

### 12. LCD触摸屏

### 13. FLASH

### 14. TF卡
