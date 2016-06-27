

#include "delay.h"

void time_delay_init_now(time_delay_t time)
{
    clock_gettime(CLOCK_MONOTONIC, &time->t);
}

void time_delay_add_us(time_delay_t time, int us)
{
    long ns     = (long)us * 1000;
    time_t sec  = ns/1000000000;
    ns          = ns%1000000000;
    time->t.tv_nsec += ns;
    time->t.tv_sec  += sec;
    if (time->t.tv_nsec > 1000000000) {
        time->t.tv_nsec -= 1000000000;
        time->t.tv_sec++;
    }
}

void time_delay_add_ms(time_delay_t time, int ms)
{
    long ns     = (long)ms * 1000000;
    time_t sec  = ns/1000000000;
    ns          = ns%1000000000;
    time->t.tv_nsec += ns;
    time->t.tv_sec  += sec;
    if (time->t.tv_nsec > 1000000000) {
        time->t.tv_nsec -= 1000000000;
        time->t.tv_sec++;
    }
}

void time_delay_sleep(time_delay_t time)
{
    while (nanosleep(&time->t, NULL) != 0)
        ;
}
