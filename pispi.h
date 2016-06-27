

#if !defined(PISPI_H)
#   define PISPI_H 1

#include <stdint.h>

struct pispi {
    /** file descriptor */
    int fd;
    /** spi mode */
    unsigned char mode;
    /** bit justification 0 is msb first, other are lsb first*/
    unsigned char bitjust;
    /** the number of bits per word */
    unsigned char wordbits;
    /** clock_speed*/
    uint32_t clk_speed;
};

typedef struct pispi* pispi_t;

enum pispi_mode {
    SPI_CLOCK_POLARITY_BIT = 0x01,
    SPI_CLOCK_PHASE_BIT    = 0x02
};

enum pspi_bit_justify {
    SPI_MSB_FIRST   = 0,
    SPI_LSB_FIRST   = 1
};

void pispi_init(pispi_t dev,
                int mode,
                int bjust,
                int wordbits,
                unsigned clk_speed
        );

int pispi_open(pispi_t dev,
               const char* adress
        );

int pispi_close(pispi_t dev);

int pispi_transfer(pispi_t dev, void* buf_int, void* buf_out, int length);


#endif 
