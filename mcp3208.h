
#if !defined (MCP3208_H)
    #define MCP3208_H 1

struct mcp3208 {
    int clk_pin;
    int mosi_pin;
    int miso_pin;
    int sel_pin;
};

typedef struct mcp3208*  mcp3208_t;

/*
 * These channels are used for both differential and single ended use.
 * The set bits are from the table on page 19 from the 2008 Microchip
 * productsheet of the mcp3204 and mcp3208.
 */

// single ended.
#define CHANNEL_0   0x01                      
#define CHANNEL_1   0x01               | 0x08
#define CHANNEL_2   0x01        | 0x04       
#define CHANNEL_3   0x01        | 0x04 | 0x08
#define CHANNEL_4   0x01 | 0x02              
#define CHANNEL_5   0x01 | 0x02        | 0x08
#define CHANNEL_6   0x01 | 0x02 | 0x04       
#define CHANNEL_7   0x01 | 0x02 | 0x04 | 0x08
// diferential first channel num is in positive other is negative
#define CHANNEL_01  0x00                     
#define CHANNEL_10  0x00               | 0x08
#define CHANNEL_23  0x00        | 0x04       
#define CHANNEL_32  0x00        | 0x04 | 0x08
#define CHANNEL_45  0x00 | 0x02              
#define CHANNEL_54  0x00 | 0x02        | 0x08
#define CHANNEL_67  0x00 | 0x02 | 0x04       
#define CHANNEL_76  0x00 | 0x02 | 0x04 | 0x08

enum mcp3208_error {
    MCP3208_E_NULLBIT = -1,     // Nullbit was not 0.
    MCP3208_E_CHANNEL = -2,     // wrong channel specified.
    MCP3208_E_CHECK_ERRNO= -3   // unknown error occurred errno
                                // might be of use.
};

/**
 * Initializes the mcp3208_t.
 *
 * Initializes the mcp3208_t struct. And puts the pins in the right modi.
 * Also sets the clock and selection pin high, to prepare for usage.
 *
 * Note   that the setup function of the wiringPi library affects the pins.
 * NoteI  Don't use this initializer for hardware function
 * 
 *
 * @param pmcp  A pointer to struct mcp3204
 * @param clk   The clock pin number
 * @param mosi  The Master Out Slave In pin number
 * @param miso  The Master In Slave Out pin number
 * @param sel   The selction pin. 
 */

void mcp3208_init(mcp3208_t pmcp, int clk, int mosi, int miso, int sel);

/**
 * Initializes the mcp3208_t.
 *
 * Initializes the mcp3208_t struct.
 *
 * use this initializer to setup for using the hardware.
 *
 * @param pmcp       A pointer to struct mcp3204
 * @param clkspeed   The clock speed in Hz
 * @param channel    The selction pin. should be 0 or 1
 */
int mcp3208_init_hw(mcp3208_t pmcp, int clkspeed, int channel);

/**
 * Reads a value from the mcp3208 chip.
 *
 * @return a value between 0 including and 4096 excluding when succesfull
 * or MCP3208_E_CHANNEL when an invalid channel is chosen.
 */
int mcp3204_read_channel(mcp3208_t pmcp, int channel);

/**
 * Reads a value from the mcp3204 chip.
 *
 * @return a value between 0 including and 4096 excluding when succesfull
 * or MCP3208_E_CHANNEL when an invalid channel is chosen.
 */
int mcp3204_read_channel(mcp3208_t pmcp, int channel);

/**
 * Reads a value from the mcp3204 chip.
 *
 * @return a value between 0 including and 4096 excluding on success.
 *         or MCP3208_E_CHANNEL when an invalid channel is chosen.
 */
int mcp3208_read_hw(mcp3208_t pmcp, unsigned channel);

#endif
