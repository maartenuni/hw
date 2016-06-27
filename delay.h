

#if !defined(TIME_DELAY_H)
#   define TIME_DELAY_H 1

//#if __STDC_VERSION__ >= 199901L
//#define _XOPEN_SOURCE 600
//#else
//#define _XOPEN_SOURCE 500
//#endif

#include <time.h>

struct time_delay {
    struct timespec t;
};

typedef struct time_delay* time_delay_t;

void time_delay_init_now(time_delay_t obj);
void time_delay_add_us(time_delay_t obj, int us);
void time_delay_add_ms(time_delay_t obj, int ms);
void time_delay_sleep(time_delay_t obj);



#endif
