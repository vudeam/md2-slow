#ifndef MD2_UTILS_H
#define MD2_UTILS_H


/**
 * Write error message to stderr and terminate program.
 * Note: never returns.
 */
__attribute__((noreturn)) void err(const char * msg, ...);


#endif        /* MD2_UTILS_H */

