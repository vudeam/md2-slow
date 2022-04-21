#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

const size_t MD2_MAX_PADSIZE = 16U;
const size_t MD2_CHECKSUM_SIZE = 16U;
const size_t MD2_MSG_BLOCK_SIZE = 16U;
const size_t MD2_MD_BUF_SIZE = 48U;

__attribute__((noreturn)) void err(const char * msg, ...) {
    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);

    exit(1);
}

