

#if !defined(BROAD_SPI_H)
#   define BROAD_SPI_H 1

#include <bcm2835.h>

struct bcm_mpc3208{
    uint8_t     clk_sel;
    unsigned    clk_devider;
    uint8_t     bit_order;
    uint8_t     mode;
};

int init_bcm_mpc3208_lib();

typedef struct bcm_mpc3208* bcm_mpc3208_t;

void bcm_mpc3208_init(bcm_mpc3208_t dev, uint8_t clk_sel);

int bcm_mpc3208_read(bcm_mpc3208_t dev, uint8_t channel);


#endif
