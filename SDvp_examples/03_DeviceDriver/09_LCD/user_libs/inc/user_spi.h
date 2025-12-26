#ifndef __USER_SPI_H
#define __USER_SPI_H

#include "main.h"

void user_spi_init(void);
uint8_t user_spi_transmit_receive(SPI_TypeDef *SPIx, uint8_t data);

#endif
