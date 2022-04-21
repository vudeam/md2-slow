#include <malloc.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "utils.h"


static const size_t MAX_PADSIZE = 16U;    /* maximum padding size */

/**
 * Add padding bytes to file bytes.
 * In the MD2 algorithm the file is always padded.
 * The padding makes file length = 0 modulo 16.
 *
 * Returns padding size.
 */
size_t pad_file(uint8_t * const buf, long int fsize);

int main(int argc, char * argv[]) {
    if (argc <= 1) {
        err("Not enough arguments provided.\nUsage: %s [file]\n", argv[0]);

        return -1;
    }

    FILE * in_file;
    uint8_t * buf;
    long int file_len;

    if (!(in_file = fopen(argv[1], "rb"))) {
        err("Error reading file '%s'\n", argv[1]);
    }

    fseek(in_file, 0, SEEK_END);
    file_len = ftell(in_file);
    rewind(in_file);

    printf("File size: %ld\n", file_len);

    buf = malloc(file_len * sizeof(uint8_t) + MAX_PADSIZE);
    fread(buf, file_len, 1, in_file);
    fclose(in_file);

    file_len += pad_file(buf, file_len);
    fwrite(buf, file_len, 1, fopen("padded.txt", "wb"));

    free(buf);

    return 0;
}

/*
void examine_bytes(FILE * fp, uint8_t * const buf, size_t size) {
    for (size_t byte = 0; byte < size; byte++) {
        fprintf(fp, "%" PRIx16 " ", buf[byte]);
        if (!(byte % 8) && !byte) fprintf(fp, "\n");
    }
    fprintf(fp, "\n");
}
*/

size_t pad_file(uint8_t * const buf, long int fsize) {
    const size_t bytes_to_pad = 16 - fsize % 16;
        // ? 16 - fsize % 16
        // : 16;

    for (size_t pad_byte = 0; pad_byte < bytes_to_pad; pad_byte++) {
        buf[fsize + pad_byte] = pad_byte;
    }

    return bytes_to_pad;
}

