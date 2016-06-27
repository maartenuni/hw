
#include "bitwriter.h"
#include <string.h>


void bitwriter_init(bitwriter_t pbits, void* buf, int size)
{
    pbits->buffer   = buf;
    pbits->buffsiz  = size;
    pbits->bytepos  = 0;
    pbits->bitpos   = 7;
}

void bitwriter_init_zero(bitwriter_t pbits, void* buf, int size)
{
    pbits->buffer   = buf;
    pbits->buffsiz  = size;
    pbits->bytepos  = 0;
    pbits->bitpos   = 7;
    memset(buf, 0, size);
}

int bitwriter_push_one(bitwriter_t pbits)
{
    if (pbits->bitpos < 0){
        pbits->bytepos++;
        pbits->bitpos = 7;
    }
    if (pbits->bytepos >= pbits->buffsiz)
        return BIT_WRITER_OUTOFRANGE;

    pbits->buffer[pbits->bytepos] |= 1 << pbits->bitpos;
    return pbits->bitpos--;
}

int bitwriter_push_zero(bitwriter_t pbits)
{
    if (pbits->bitpos < 0){
        pbits->bytepos++;
        pbits->bitpos = 7;
    }
    if (pbits->bytepos >= pbits->buffsiz)
        return BIT_WRITER_OUTOFRANGE;

    return pbits->bitpos--;
}

