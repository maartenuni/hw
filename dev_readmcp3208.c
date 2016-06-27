
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "delay.h"
#include "bitwriter.h"
#include "pispi.h"
#include "mcp3208.h"

//static const int MOSI       = 12;
//static const int MISO       = 13;
//static const int CLK        = 14;
//static const int SEL        = 10;
//static const int CHANNEL    = 11;

static const int CLK_SPEED  = 500000;
const char* devfn = "/dev/spidev0.0";

static const char* outfn = "output.txt";
int stop = 0;

void signalled(int sig) {
    if (sig == SIGINT) {
        stop = 1;
        fprintf(stderr, "Caught SIGINT stopping.\n");
    }
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


int read_mcp3208(pispi_t dev, unsigned channel)
{
    unsigned char buffer[6];
    unsigned char output[6];
    struct bitwriter bits;
    bitwriter_init_zero(&bits, buffer, 6);
    memset(&output, 0, 6);
    // first 5 bits should be low
    for (int i = 0; i < 5; i++)
        bitwriter_push_zero(&bits);

    // write start bit.
    bitwriter_push_one(&bits);

    // write single/differential bit and D2 bit
    for (int i = 0; i < 4; i++) {
        if ( channel & (1 << i) )
            bitwriter_push_one(&bits);
        else 
            bitwriter_push_zero(&bits);
    }

    buffer_print_bits(buffer, 6);
    int result = pispi_transfer(dev,
                                buffer,
                                output,
                                6
                                );

    int nullbit = output[1] & 0x10;
    if (nullbit)
        return MCP3208_E_NULLBIT;

    result = (output[2] & 0x04) << 8 | buffer[3];
    buffer_print_bits(output, 6);

    return result;
}

int main() {

    struct time_delay delay;
    struct time_delay now;

    delay.t.tv_nsec = (int) 1e6;
    delay.t.tv_sec  = 0;

    struct pispi hwdev;
    pispi_init(&hwdev, 0,
                       SPI_MSB_FIRST,
                       8,
                       CLK_SPEED
                       );

    int result = pispi_open(&hwdev, devfn);
    if (result) {
        perror("Unable to open spidevice");
        return -1;
    }

    FILE* fid = fopen(outfn, "wb");
    if (!fid) {
        perror("Unable to open output file");
        return -1;
    }

    for (int i = 0; i < 10 && !stop; i++) {
        time_delay_sleep(&delay);
        time_delay_init_now(&now);

        int value = read_mcp3208(&hwdev, CHANNEL_0);

        fprintf(fid,
                "%ld.%ld\t%d\n",
                now.t.tv_sec,
                now.t.tv_nsec,
                value
                );
        //mcp3208_read_hw(&adc, CHANNEL)
    }

    fclose(fid);
    return 0;
}
