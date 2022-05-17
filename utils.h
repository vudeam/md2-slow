#ifndef MD2_UTILS_H
#define MD2_UTILS_H

#include <stddef.h>
#include <stdint.h>


typedef uint8_t Byte;
typedef Byte * M;              /* Message (bytes) buffer */
typedef Byte Checksum[16];
typedef Byte PiDigits[256];
typedef Byte MDBuff[48];

/* maximum padding size */
extern const size_t MD2_MAX_PADSIZE;

extern const size_t MD2_CHECKSUM_SIZE;

/* size of message block processed */
extern const size_t MD2_MSG_BLOCK_SIZE;

extern const size_t MD2_MD_BUF_SIZE;


/**
 * Write error message to stderr and terminate program.
 * Note: never returns.
 */
__attribute__((noreturn)) void err(const char * msg, ...);


#endif        /* MD2_UTILS_H */

