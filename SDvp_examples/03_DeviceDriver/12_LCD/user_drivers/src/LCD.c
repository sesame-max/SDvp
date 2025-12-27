#include "main.h"
#include "LCD.h"
#include "user_spi.h"

// void LCD_display_on(void);
// void LCD_display_off(void);
// void LCD_clear(uint16_t Color);
// void LCD_set_cursor(uint16_t Xpos, uint16_t Ypos);
// void LCD_draw_point(uint16_t x, uint16_t y);     // 画点
// uint16_t LCD_read_point(uint16_t x, uint16_t y); // 读点
// void LCD_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
// void LCD_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
// void LCD_set_windows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd);

LCDTypedef lcddev;

void LCD_WR_REG(uint8_t data)
{
    LCD_CS_CLR;
    LCD_RS_CLR;
    user_spi_transmit_receive(SPI1, data);
    LCD_CS_SET;
}
void LCD_WR_DATA(uint8_t data)
{
    LCD_CS_CLR;
    LCD_RS_SET;
    user_spi_transmit_receive(SPI1, data);
    LCD_CS_SET;
}
void Lcd_write_data_16Bit(uint16_t data)
{
    LCD_CS_CLR;
    LCD_RS_SET;
    user_spi_transmit_receive(SPI1, data >> 8);
    user_spi_transmit_receive(SPI1, data);
    LCD_CS_SET;
}
void LCD_write_reg(uint8_t Reg, uint16_t RegValue)
{
    LCD_WR_REG(Reg);
    LCD_WR_DATA(RegValue);
}
void LCD_writeRAM_prepare(void)
{
    LCD_WR_REG(lcddev.wramcmd);
}

void LCD_reset(void)
{
    LCD_RST_CLR;
    HAL_Delay(100);
    LCD_RST_SET;
    HAL_Delay(50);
}

void LCD_display_on(void)
{
    LCD_BK_SET;
}

void LCD_display_off(void)
{
    LCD_BK_CLR;
}

void LCD_direction(uint8_t direction)
{
    lcddev.setxcmd = 0x2A;
    lcddev.setycmd = 0x2B;
    lcddev.wramcmd = 0x2C;
    switch (direction)
    {
    case 0:
        lcddev.dir = 0;
        lcddev.width = LCD_W;
        lcddev.height = LCD_H;
        LCD_write_reg(0x36, (1 << 3) | (0 << 7) | (1 << 6) | (0 << 5)); // BGR==1,MY==0,MX==0,MV==0
        break;
    case 1:
        lcddev.dir = 0;
        lcddev.width = LCD_W;
        lcddev.height = LCD_H;
        LCD_write_reg(0x36, (1 << 3) | (1 << 7) | (0 << 6) | (0 << 5)); // BGR==1,MY==1,MX==0,MV==1
        break;
    case 2:
        lcddev.dir = 1;
        lcddev.width = LCD_H;
        lcddev.height = LCD_W;
        LCD_write_reg(0x36, (1 << 3) | (1 << 7) | (1 << 6) | (1 << 5)); // BGR==1,MY==0,MX==0,MV==0
        break;
    case 3:
        lcddev.dir = 1;
        lcddev.width = LCD_H;
        lcddev.height = LCD_W;
        LCD_write_reg(0x36, (1 << 3) | (0 << 7) | (0 << 6) | (1 << 5)); // BGR==1,MY==1,MX==0,MV==1
        break;
    default:
        break;
    }
}

void LCD_set_windows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd)
{
    uint16_t twidth,theight;
	twidth=xEnd-xStar;
	theight=yEnd-yStar;
    LCD_WR_REG(lcddev.setxcmd);
    LCD_WR_DATA(xStar >> 8);
    LCD_WR_DATA(0x00FF & xStar);
    LCD_WR_DATA(twidth >> 8);
    LCD_WR_DATA(0x00FF & twidth);

    LCD_WR_REG(lcddev.setycmd);
    LCD_WR_DATA(yStar >> 8);
    LCD_WR_DATA(0x00FF & yStar);
    LCD_WR_DATA(theight >> 8);
    LCD_WR_DATA(0x00FF & theight);

    LCD_writeRAM_prepare(); // 开始写入GRAM
}

void LCD_set_cursor(uint16_t Xpos, uint16_t Ypos)
{
    LCD_set_windows(Xpos, Ypos, Xpos, Ypos);
}

void LCD_init(void)
{
    LCD_reset(); // LCD 复位
    //*************3.5inch ST7796S初始化**********/
    HAL_Delay(120); // Delay 120ms
    LCD_WR_REG(0x11); // Sleep Out
    HAL_Delay(120); // Delay 120ms
    LCD_WR_REG(0xf0) ;
    LCD_WR_DATA(0xc3) ;
    LCD_WR_REG(0xf0) ;
    LCD_WR_DATA(0x96) ;
    LCD_WR_REG(0x36);
    LCD_WR_DATA(0x48); 
    LCD_WR_REG(0x3A);
    LCD_WR_DATA(0x55);
    LCD_WR_REG(0xB4);
    LCD_WR_DATA(0x01);
    LCD_WR_REG(0xB7) ;
    LCD_WR_DATA(0xC6) ;
    LCD_WR_REG(0xe8);
    LCD_WR_DATA(0x40);
    LCD_WR_DATA(0x8a);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x29);
    LCD_WR_DATA(0x19);
    LCD_WR_DATA(0xa5);
    LCD_WR_DATA(0x33);
    LCD_WR_REG(0xc1);
    LCD_WR_DATA(0x06);
    LCD_WR_REG(0xc2);
    LCD_WR_DATA(0xa7);
    LCD_WR_REG(0xc5);
    LCD_WR_DATA(0x18);
    LCD_WR_REG(0xe0); //Positive Voltage Gamma Control
    LCD_WR_DATA(0xf0);
    LCD_WR_DATA(0x09);
    LCD_WR_DATA(0x0b);
    LCD_WR_DATA(0x06);
    LCD_WR_DATA(0x04);
    LCD_WR_DATA(0x15);
    LCD_WR_DATA(0x2f);
    LCD_WR_DATA(0x54);
    LCD_WR_DATA(0x42);
    LCD_WR_DATA(0x3c);
    LCD_WR_DATA(0x17);
    LCD_WR_DATA(0x14);
    LCD_WR_DATA(0x18);
    LCD_WR_DATA(0x1b);
    LCD_WR_REG(0xe1); //Negative Voltage Gamma Control
    LCD_WR_DATA(0xf0);
    LCD_WR_DATA(0x09);
    LCD_WR_DATA(0x0b);
    LCD_WR_DATA(0x06);
    LCD_WR_DATA(0x04);
    LCD_WR_DATA(0x03);
    LCD_WR_DATA(0x2d);
    LCD_WR_DATA(0x43);
    LCD_WR_DATA(0x42);
    LCD_WR_DATA(0x3b);
    LCD_WR_DATA(0x16);
    LCD_WR_DATA(0x14);
    LCD_WR_DATA(0x17);
    LCD_WR_DATA(0x1b);
    LCD_WR_REG(0xf0);
    LCD_WR_DATA(0x3c);
    LCD_WR_REG(0xf0);
    LCD_WR_DATA(0x69);
    LCD_WR_REG(0x21);
    HAL_Delay(120); //Delay 120ms
    LCD_WR_REG(0x29);// Display on

    LCD_direction(USE_HORIZONTAL); // 设置LCD显示方向
    LCD_BK_SET;
    LCD_clear(WHITE); // 清全屏白色
}

void LCD_clear(uint16_t color)
{
    unsigned int i, m;
    LCD_set_windows(0, 0, lcddev.width - 1, lcddev.height - 1);
    LCD_CS_CLR;
    LCD_RS_SET;
    for (i = 0; i < lcddev.height; i++)
    {
        for (m = 0; m < lcddev.width; m++)
        {
            Lcd_write_data_16Bit(color);
        }
    }
    LCD_CS_SET;
}

void LCD_draw_point(uint16_t x, uint16_t y, uint16_t color)
{
    LCD_set_cursor(x, y); // 设置光标位置
    Lcd_write_data_16Bit(color);
}

void LCD_Fill(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd, uint16_t color)
{
    uint16_t i, j;
    uint16_t width = xEnd - xStar + 1;         // 得到填充的宽度
    uint16_t height = yEnd - yStar + 1;        // 高度
    LCD_set_windows(xStar, yStar, xEnd, yEnd); // 设置显示窗口
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
            Lcd_write_data_16Bit(color); // 写入数据
    }
    LCD_set_windows(0, 0, lcddev.width - 1, lcddev.height - 1); // 恢复窗口设置为全屏
}
