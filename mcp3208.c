
#include <assert.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#include <string.h>
#include "mcp3208.h"
#include "bitwriter.h"


void mcp3208_init(mcp3208_t pmcp, int clk, int mosi, int miso, int sel)
{
    pmcp->clk_pin   = clk;
    pmcp->mosi_pin  = mosi;
    pmcp->miso_pin  = miso;
    pmcp->sel_pin   = sel;

    // set pins to the correct mode.
    pinMode(pmcp->clk_pin   , OUTPUT);
    pinMode(pmcp->mosi_pin  , OUTPUT);
    pinMode(pmcp->miso_pin  , INPUT);
    pinMode(pmcp->sel_pin   , OUTPUT);

    // set clock and selection pin high, so that subsequent call don't
    // have to do that.
    digitalWrite(pmcp->clk_pin, HIGH);
    digitalWrite(pmcp->sel_pin, HIGH);
}

int mcp3208_init_hw(mcp3208_t pmcp, int clkspeed, int channel)
{
    int ret = wiringPiSPISetup(channel, clkspeed);
    pmcp->sel_pin = channel;
    return ret;
}

static int bang_bits(mcp3208_t pmcp, int channel)
{
    int bits = channel;
    int nullbit = 0;
    int result = 0;

    digitalWrite(pmcp->sel_pin, LOW);

    // write start bit
    digitalWrite(pmcp->clk_pin, LOW);
    digitalWrite(pmcp->mosi_pin, HIGH );
    digitalWrite(pmcp->clk_pin, HIGH);

    // write the control bits.
    for (int i = 0; i < 5; i++) {
        digitalWrite(pmcp->clk_pin, LOW);
        digitalWrite(pmcp->mosi_pin, bits & (1<<i) );
        digitalWrite(pmcp->clk_pin, HIGH);
    }

    // read nullbit
    digitalWrite(pmcp->clk_pin, LOW);
    nullbit = digitalRead(pmcp->miso_pin);
    assert(!nullbit);
    digitalWrite(pmcp->clk_pin, HIGH);
    if (nullbit) {
        digitalWrite(pmcp->sel_pin, HIGH);
        digitalWrite(pmcp->clk_pin, LOW);
        return MCP3208_E_NULLBIT;
    }

    // We recieve first bit with index 11 and the last one is index 0
    for (int i = 11; i >= 0; i--) {
        digitalWrite(pmcp->clk_pin, LOW);
        int bit = digitalRead(pmcp->miso_pin);
        result |= (bit << i);
        digitalWrite(pmcp->clk_pin, HIGH);
    }

    // we set selection pin high, and the clock low.
    // then it is ready for the next readout.
    digitalWrite(pmcp->clk_pin, LOW);
    digitalWrite(pmcp->sel_pin, HIGH);
    return result;
}

/* read a single channel from the mcp3208
 */
int mcp3208_read_channel(mcp3208_t pmcp, unsigned channel)
{
    return bang_bits(pmcp, channel);
}

/* read a single channel from the mcp3208
 */
int mcp3204_read_channel(mcp3208_t pmcp, int channel)
{
    //specified a channel that is not on the device.
    if (channel & 0x02)
        return MCP3208_E_CHANNEL;

    return bang_bits(pmcp, channel);
}

void buffer_print_bits(unsigned char* buffer, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 7; j >= 0; j--) {
            if (buffer[i] & 1 << j)
                fprintf(stdout, "%d", 1);
            else
                fprintf(stdout, "%d", 0);
        }
        fprintf(stdout, " ");
    }
    fprintf(stdout, "\n");
}

int mcp3208_read_hw(mcp3208_t pmcp, unsigned channel)
{
    unsigned char buffer[6];
    struct bitwriter bits;
    bitwriter_init_zero(&bits, buffer, 6);
    // first 5 bits should be low
    for (int i = 0; i < 5; i++)
        bitwriter_push_zero(&bits);

    // write start bit.
    bitwriter_push_one(&bits);

    // write single/differential bit and D2 bit
    for (int i = 3; i >= 0; i--) {
        if ( channel & (1 << i) )
            bitwriter_push_one(&bits);
        else 
            bitwriter_push_zero(&bits);
    }

    //buffer_print_bits(buffer, 6);
    int result = wiringPiSPIDataRW(pmcp->sel_pin,
                                   buffer,
                                   sizeof(buffer)/sizeof(buffer[0])
                                   );
    buffer_print_bits(buffer, 6);

    int nullbit = buffer[1] & 0x10;
    if (nullbit)
        return MCP3208_E_NULLBIT;

    result = (buffer[2] & 0x04) << 8 & buffer[3];

    return result;
}
