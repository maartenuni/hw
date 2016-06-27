
#include <signal.h>
#include <stdio.h>
#include "broadmcp3208.h"
#include "mcp3208.h"
#include "delay.h"

int stop = 0;
const int CS = BCM2835_SPI_CS_0;
const char* outfn = "output.txt";

void signalled(int sig) {
    if (sig == SIGINT) {
        stop = 1;
        fprintf(stderr, "Caught SIGINT stopping.\n");
    }
}

int main() {

    if (signal(SIGINT, signalled) == SIG_ERR)
        perror("Unable to install signal_handler");

    // initialize the library.
    if (init_bcm_mpc3208_lib())
        return 1;

    struct time_delay delay;
    struct time_delay now;

    struct bcm_mpc3208 dev;
    bcm_mpc3208_init(&dev, CS);

    delay.t.tv_nsec = (int) 1e6;
    delay.t.tv_sec  = 0;

    FILE* fid = fopen(outfn, "wb");
    if (!fid) {
        perror("Unable to open output file.");
        return -1;
    }

    // looping to get signal.
    for (int i = 0; i < 10 && !stop; i++) {
        time_delay_sleep(&delay);
        time_delay_init_now(&now);

        fprintf(fid,
                "%ld.%ld\t%d\n",
                now.t.tv_sec,
                now.t.tv_nsec,
                bcm_mpc3208_read(&dev, CHANNEL_0 )
                );
    }

    fclose(fid);
    return 0;
}


