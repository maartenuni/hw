
#include "utils.h"
#include <stdio.h>

void print_bits(void* buf, int bufsiz)
{
    unsigned char* b = buf;
    for (int i = 0; i < bufsiz; ++i) {
        for (int bit = 0x80; bit; bit >>= 1) {
            fprintf(stderr, "%d", (b[i] & bit) != 0);
        }
        fprintf(stderr, " ");
    }
    fprintf(stderr, "\n");
}
