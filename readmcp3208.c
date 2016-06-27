
#include "mcp3208.h"
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "delay.h"

static const int MOSI= 12;
static const int MISO= 13;
static const int CLK = 14;
static const int SEL = 11;

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

    if (piHiPri(99))
        perror("unable to set priority.");

    struct mcp3208 adc;
    struct time_delay delay;
    struct time_delay now;

    mcp3208_init(&adc, CLK, MOSI, MISO, SEL);
    delay.t.tv_nsec = (int) 1e6;
    delay.t.tv_sec  = 0;

    FILE* fid = fopen(outfn, "wb");
    if (!fid) {
        perror("Unable to open output file.");
        return -1;
    }

    for (int i = 0; i < 10000 && !stop; i++) {
        time_delay_sleep(&delay);
        time_delay_init_now(&now);

        fprintf(fid,
                "%ld.%ld\t%d\n",
                now.t.tv_sec,
                now.t.tv_nsec,
                mcp3204_read_channel(&adc, CHANNEL_0 )
                );
    }

    fclose(fid);
    return 0;
}
