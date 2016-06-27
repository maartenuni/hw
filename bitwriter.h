
#ifndef BIT_WRITER_H
#define BIT_WRITER_H

#include <stdint.h>

/**
 * structure that holds
 */
struct bitwriter {
    uint8_t*    buffer;  // pointer to the bitbuffer
    unsigned    buffsiz; // size in byte of the buffer
    unsigned    bytepos; // The byte we are currently writing to.
    int         bitpos;  // The bit we are about to write to.
};

//typedef to pointer of the bitwriter struct
typedef struct bitwriter* bitwriter_t;

enum bitwriter_errors {
    BIT_WRITER_OUTOFRANGE   = -1
};

/**
 * Initializes bitwriter.
 *
 * @param pbits The pointer to bitwriter
 * @param buf   The already allocated buffer we are writing to.
 * @param size  The size of the buffer in bytes.
 */
void bitwriter_init(bitwriter_t pbits, void* buf, int size);

/**
 * Initializes bitwriter. Zero's out all bits.
 *
 * @param pbits The pointer to bitwriter
 * @param buf   The already allocated buffer we are writing to.
 * @param size  The size of the buffer in bytes.
 */
void bitwriter_init_zero(bitwriter_t pbits, void* buff, int size);


/**
 * Push a HIGH (1) bit to the end of the vector.
 *
 * @param pbits The bitwriter
 */
int bitwriter_push_one(bitwriter_t pbits);

/**
 * Push a LOW (0) bit to the end of the vector.
 *
 * @param pbits The bitwriter
 */
int bitwriter_push_zero(bitwriter_t pbits);

#endif
