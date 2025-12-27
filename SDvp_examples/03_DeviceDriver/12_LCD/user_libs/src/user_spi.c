#include "main.h"
#include "spi.h"
#include "user_spi.h"

void user_spi_init(void)
{
    LL_SPI_Enable(SPI1);
}
uint8_t user_spi_transmit_receive(SPI_TypeDef *SPIx, uint8_t data)
{
    uint16_t i = 0xffff;
    LL_SPI_TransmitData8(SPIx, data);
    while (LL_SPI_IsActiveFlag_BSY(SPIx) == 1)
    {
        i--;
        if (i == 0)
        {
            return 0;
        }
    }
    return LL_SPI_ReceiveData8(SPIx);
}
