#ifndef __LCD_H
#define __LCD_H
#include "main.h"

// // GPIO置位（拉高）
// #define LCD_CS_SET HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, 1)  // 片选端口
// #define LCD_RS_SET HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, 1)    // 数据/命令
// #define LCD_RST_SET HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, 1) // 复位
// #define LCD_BK_SET HAL_GPIO_WritePin(LED_BK_GPIO_Port, LED_BK_Pin, 1)    // 背光

// // GPIO复位（拉低）
// #define LCD_CS_CLR HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, 0)  // 片选端口
// #define LCD_RS_CLR HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, 0)    // 数据/命令
// #define LCD_RST_CLR HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, 0) // 复位
// #define LCD_BK_CLR HAL_GPIO_WritePin(LED_BK_GPIO_Port, LED_BK_Pin, 0)    // 背光

// GPIO置位（拉高）
#define LCD_CS_SET HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin,GPIO_PIN_SET)          // 片选端口
#define LCD_RS_SET HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,GPIO_PIN_SET)          // 数据/命令
#define LCD_RST_SET HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_SET)       // 复位
#define LCD_BK_SET HAL_GPIO_WritePin(LCD_LED_GPIO_Port,LCD_LED_Pin,GPIO_PIN_SET)        // 背光

// GPIO复位（拉低）
#define LCD_CS_CLR HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin,GPIO_PIN_RESET)        // 片选端口
#define LCD_RS_CLR HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,GPIO_PIN_RESET)        // 数据/命令
#define LCD_RST_CLR HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_RESET)     // 复位
#define LCD_BK_CLR HAL_GPIO_WritePin(LCD_LED_GPIO_Port,LCD_LED_Pin,GPIO_PIN_RESET)      // 背光

// LCD重要参数集
typedef struct
{
    uint16_t width;   // LCD 宽度
    uint16_t height;  // LCD 高度
    uint16_t id;      // LCD ID
    uint8_t dir;      // 横屏还是竖屏控制：0，竖屏；1，横屏。
    uint16_t wramcmd; // 开始写gram指令
    uint16_t setxcmd; // 设置x坐标指令
    uint16_t setycmd; // 设置y坐标指令
} LCDTypedef;

extern LCDTypedef lcddev;

#define USE_HORIZONTAL 2 // 定义液晶屏顺时针旋转方向 	0-0度旋转，1-180度旋转，2-90度旋转，3-270度旋转

// 定义LCD的尺寸
#define LCD_W 320
#define LCD_H 480

// 画笔颜色
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40 // 棕色
#define BRRED 0XFC07 // 棕红色
#define GRAY 0X8430  // 灰色

void LCD_WR_REG(uint8_t data);
void LCD_WR_DATA(uint8_t data);
void Lcd_write_data_16Bit(uint16_t data);
void LCD_write_reg(uint8_t Reg, uint16_t RegValue);
void LCD_writeRAM_prepare(void);
void LCD_reset(void);
void LCD_display_on(void);
void LCD_display_off(void);
void LCD_direction(uint8_t direction);
void LCD_set_windows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd);
void LCD_set_cursor(uint16_t Xpos, uint16_t Ypos);
void LCD_init(void);
void LCD_clear(uint16_t color);
void LCD_draw_point(uint16_t x, uint16_t y, uint16_t color);
void LCD_Fill(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd, uint16_t color);

#endif
