#ifndef MD2_UTILS_H
#define MD2_UTILS_H

#include <stdint.h>


typedef uint8_t Byte;
typedef Byte Checksum[16];
typedef Byte PiDigits[256];

/**
 * Write error message to stderr and terminate program.
 * Note: never returns.
 */
__attribute__((noreturn)) void err(const char * msg, ...);


#endif        /* MD2_UTILS_H */

