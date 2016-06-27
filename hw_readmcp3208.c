
#include "mcp3208.h"
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "delay.h"

static const int MOSI       = 12;
static const int MISO       = 13;
static const int CLK        = 14;
static const int SEL        = 10;
static const int CHANNEL    = 0;
static const int CLK_SPEED  = (int)1e6;

static const char* outfn = "output.txt";
int stop = 0;

void signalled(int sig) {
    if (sig == SIGINT) {
        stop = 1;
        fprintf(stderr, "Caught SIGINT stopping.\n");
    }
}

int main() {

    // Initialize the wireingpi library to use the remapped numbering
    // system, thus DON'T use the broadcom numbers.
    wiringPiSetup();
    int fd = wiringPiSPISetup(CHANNEL, CLK_SPEED);
    if (fd < 0) {
        perror("Unable to setup SPI");
        return -1;
    }

    struct mcp3208 adc;
    struct time_delay delay;
    struct time_delay now;

    mcp3208_init(&adc, CLK, MOSI, MISO, CHANNEL);
    delay.t.tv_nsec = (int) 1e6;
    delay.t.tv_sec  = 0;

    FILE* fid = fopen(outfn, "wb");
    if (!fid) {
        perror("Unable to open output file.");
        return -1;
    }

    for (int i = 0; i < 1000 && !stop; i++) {
        time_delay_sleep(&delay);
        time_delay_init_now(&now);

        fprintf(fid,
                "%ld.%ld\t%d\n",
                now.t.tv_sec,
                now.t.tv_nsec,
                mcp3208_read_hw(&adc, CHANNEL)
                );
    }

    fclose(fid);
    return 0;
}
