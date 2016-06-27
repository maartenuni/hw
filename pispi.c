
#include "pispi.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/spi/spidev.h>
#include <stdio.h>


void pispi_init(pispi_t dev,
                int mode,
                int bjust,
                int wordbits,
                unsigned clk_speed
        )
{
    dev->fd         = -1;
    dev->mode       = mode;
    dev->bitjust    = bjust;
    dev->wordbits   = wordbits;
    dev->clk_speed  = clk_speed;
}

int pispi_open(pispi_t dev,
                const char* devfn
        )
{
    int ret;
    dev->fd = open(devfn, O_RDWR);
    if (dev->fd < 0) {
        return -1;
    }

    ret = ioctl(dev->fd, SPI_IOC_WR_MODE, &dev->mode);
    if (ret) {
        perror("Unable to set SPI mode");
        return ret;
    }

    ret = ioctl(dev->fd, SPI_IOC_WR_LSB_FIRST, &dev->bitjust);
    if (ret) {
        perror("Unable to set lsb/msb first.");
        return ret;
    }

    ret = ioctl(dev->fd, SPI_IOC_WR_BITS_PER_WORD, &dev->wordbits);
    if (ret) {
        perror("Unable to set SPI nubmer of bits per word.");
        return ret;
    }

    ret = ioctl(dev->fd, SPI_IOC_WR_MAX_SPEED_HZ, &dev->clk_speed);
    if (ret) {
        perror("Unable to set SPI clock speed");
        return ret;
    }
    return ret;
}

int pispi_close(pispi_t dev)
{
    return close(dev->fd);
}

int pispi_transfer(pispi_t dev,
                   void* buf_in,
                   void* buf_out,
                   int length)
{
    struct spi_ioc_transfer trans;

    trans.tx_buf        = (unsigned long)buf_in;
    trans.rx_buf        = (unsigned long)buf_out;
    trans.len           = length;
    trans.speed_hz      = dev->clk_speed;
    trans.delay_usecs   = 1;
    trans.bits_per_word = dev->wordbits;
    trans.cs_change     = 1;
    trans.pad           = 0;

    return ioctl(dev->fd, SPI_IOC_MESSAGE(1), &trans);
}

