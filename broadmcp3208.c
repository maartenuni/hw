
#include <stdio.h>
#include "broadmcp3208.h"
#include "mcp3208.h"
#include "bitwriter.h"
#include "utils.h"

int init_bcm_mpc3208_lib()
{
    if (!bcm2835_init())
    {
        fprintf(stderr,
                "bcm2835_init failed. Are you running as root??\n"
                );
        return 1;
    }

    if (!bcm2835_spi_begin())
    {
        fprintf(stderr,
                "bcm2835_spi_begin failed. Are you running as root??\n"
                );
        return 1;
    }
    return 0;
}

void bcm_mpc3208_init(bcm_mpc3208_t dev, uint8_t clk_sel) {
    dev->bit_order  = BCM2835_SPI_BIT_ORDER_MSBFIRST;
    dev->mode       = BCM2835_SPI_MODE0;
    dev->clk_sel    = clk_sel;
    dev->clk_devider= BCM2835_SPI_CLOCK_DIVIDER_256;

    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256);
    bcm2835_spi_chipSelect(dev->clk_sel);
    bcm2835_spi_setChipSelectPolarity(dev->clk_sel, LOW);
}

int bcm_mpc3208_read(bcm_mpc3208_t dev, uint8_t channel)
{
    const int sz = 6;
    char buffer[sz];
    struct bitwriter bits;
    int i, nullbit, result;

    bitwriter_init_zero(&bits, buffer, sz);
    for (i = 0; i < 5; i++)
        bitwriter_push_zero(&bits);

    // push start bit
    bitwriter_push_one(&bits);

    // push single/diff and d2, d1 and d0 bits
    for (i = 0x01; i <= 0x08; i <<= 1) {
        if (channel & i)
            bitwriter_push_one(&bits);
        else
            bitwriter_push_zero(&bits);
    }

    print_bits(buffer, sz);

    for (i = 0; i < sz; i++)
        buffer[i] = bcm2835_spi_transfer(buffer[i]);
    print_bits(buffer, sz);
    nullbit = buffer[1] & 0x10;
    if (nullbit)
        return MCP3208_E_NULLBIT; 

    result = buffer[2] | (buffer[1] << 8);
    return result;
}

