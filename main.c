#include <malloc.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "utils.h"


/**
 * 256 numbers of 'random' digits from the Pi number
 */
static const PiDigits S = {
    0x29, 0x2E, 0x43, 0xC9, 0xA2, 0xD8, 0x7C, 0x01, 0x3D, 0x36, 0x54, 0xA1, 0xEC, 0xF0, 0x06, 0x13,
    0x62, 0xA7, 0x05, 0xF3, 0xC0, 0xC7, 0x73, 0x8C, 0x98, 0x93, 0x2B, 0xD9, 0xBC, 0x4C, 0x82, 0xCA,
    0x1E, 0x9B, 0x57, 0x3C, 0xFD, 0xD4, 0xE0, 0x16, 0x67, 0x42, 0x6F, 0x18, 0x8A, 0x17, 0xE5, 0x12,
    0xBE, 0x4E, 0xC4, 0xD6, 0xDA, 0x9E, 0xDE, 0x49, 0xA0, 0xFB, 0xF5, 0x8E, 0xBB, 0x2F, 0xEE, 0x7A,
    0xA9, 0x68, 0x79, 0x91, 0x15, 0xB2, 0x07, 0x3F, 0x94, 0xC2, 0x10, 0x89, 0x0B, 0x22, 0x5F, 0x21,
    0x80, 0x7F, 0x5D, 0x9A, 0x5A, 0x90, 0x32, 0x27, 0x35, 0x3E, 0xCC, 0xE7, 0xBF, 0xF7, 0x97, 0x03,
    0xFF, 0x19, 0x30, 0xB3, 0x48, 0xA5, 0xB5, 0xD1, 0xD7, 0x5E, 0x92, 0x2A, 0xAC, 0x56, 0xAA, 0xC6,
    0x4F, 0xB8, 0x38, 0xD2, 0x96, 0xA4, 0x7D, 0xB6, 0x76, 0xFC, 0x6B, 0xE2, 0x9C, 0x74, 0x04, 0xF1,
    0x45, 0x9D, 0x70, 0x59, 0x64, 0x71, 0x87, 0x20, 0x86, 0x5B, 0xCF, 0x65, 0xE6, 0x2D, 0xA8, 0x02,
    0x1B, 0x60, 0x25, 0xAD, 0xAE, 0xB0, 0xB9, 0xF6, 0x1C, 0x46, 0x61, 0x69, 0x34, 0x40, 0x7E, 0x0F,
    0x55, 0x47, 0xA3, 0x23, 0xDD, 0x51, 0xAF, 0x3A, 0xC3, 0x5C, 0xF9, 0xCE, 0xBA, 0xC5, 0xEA, 0x26,
    0x2C, 0x53, 0x0D, 0x6E, 0x85, 0x28, 0x84, 0x09, 0xD3, 0xDF, 0xCD, 0xF4, 0x41, 0x81, 0x4D, 0x52,
    0x6A, 0xDC, 0x37, 0xC8, 0x6C, 0xC1, 0xAB, 0xFA, 0x24, 0xE1, 0x7B, 0x08, 0x0C, 0xBD, 0xB1, 0x4A,
    0x78, 0x88, 0x95, 0x8B, 0xE3, 0x63, 0xE8, 0x6D, 0xE9, 0xCB, 0xD5, 0xFE, 0x3B, 0x00, 0x1D, 0x39,
    0xF2, 0xEF, 0xB7, 0x0E, 0x66, 0x58, 0xD0, 0xE4, 0xA6, 0x77, 0x72, 0xF8, 0xEB, 0x75, 0x4B, 0x0A,
    0x31, 0x44, 0x50, 0xB4, 0x8F, 0xED, 0x1F, 0x1A, 0xDB, 0x99, 0x8D, 0x33, 0x9F, 0x11, 0x83, 0x14
};

/**
 * Add padding bytes to file bytes.
 * In the MD2 algorithm the file is always padded.
 * The padding makes file length = 0 modulo 16.
 *
 * Returns padding size.
 */
size_t pad_file(M const buf, long int fsize);

/**
 * Calculate checksum and append it to provided buffer.
 * Note: reallocates buffer to ensure capacity (changes file size).
 *
 * Returns new buf size (with checksum).
 */
size_t append_checksum(M buf, long int fsize);

int main(int argc, char * argv[]) {
    if (argc <= 1) {
        err("Not enough arguments provided\nUsage: %s [file]\n", argv[0]);

        return -1;
    }

    FILE * in_file;
    M msg;
    long int file_len;

    fprintf(stderr, "[+] Reading file '%s'\n", argv[1]);
    if (!(in_file = fopen(argv[1], "rb"))) {
        err("[!] Error reading file '%s'\n", argv[1]);
    }

    fseek(in_file, 0, SEEK_END);
    file_len = ftell(in_file);
    rewind(in_file);

    fprintf(stderr, "[+] File size: %ld\n", file_len);

    msg = malloc(MD2_MAX_PADSIZE + file_len * sizeof(Byte));
    unsigned long int read_blocks = fread(msg, sizeof(Byte), file_len, in_file);
    fprintf(stderr, "[+] Read blocks: %lu\n", read_blocks);
    fclose(in_file);

    /**
     * Step 1 - add file padding
     */
    fprintf(stderr, "[+] Calculating padding...\n");
    file_len += pad_file(msg, file_len);

    /**
     * Step 2 - calculate checksum C and append it to the message
     */
    fprintf(stderr, "[+] Calculating checksum...\n");
    file_len = append_checksum(msg, file_len);

    /**
     * Step 3 - MD buffer initialization
     */
    fprintf(stderr, "[+] Initializing MD buffer...\n");
    MDBuff X = { 0 };

    /**
     * Step 4 - message processing
     */
    fprintf(stderr, "[+] Processing input message...\n");
    for (size_t i = 0; i < file_len / MD2_MSG_BLOCK_SIZE; i++) {
        for (size_t j = 0; j < MD2_MSG_BLOCK_SIZE; j++) {
            X[16 + j] = msg[MD2_MSG_BLOCK_SIZE * i + j];
            X[32 + j] = X[16 + j] ^ X[j];
        }

        Byte t = 0;

        for (size_t j = 0; j < 18; j++) {
            for (size_t k = 0; k < MD2_MD_BUF_SIZE; k++) {
                X[k] = X[k] ^ S[t];
                t = X[k];
            }
            t = t + j % 256;
        }
    }

    free(msg);

    fprintf(stderr, "\n[*] Hash calculation complete\n");
    fprintf(stderr, "[*] File: '%s'\n", argv[1]);
    fprintf(stderr, "[*] Hash: ");
    for (size_t i = 0; i < 16; i++) {
        fprintf(stderr, "%02" PRIX8 " ", X[i]);
    }
    fputs("\n", stderr);

    return 0;
}

size_t pad_file(M const buf, long int fsize) {
    const size_t bytes_to_pad = 16 - fsize % 16;

    for (size_t pad_byte = 0; pad_byte < bytes_to_pad; pad_byte++) {
        buf[fsize + pad_byte] = bytes_to_pad;
    }

    return bytes_to_pad;
}

size_t append_checksum(M buf, long int fsize) {
    Checksum C = { 0 };
    Byte c = 0, L = 0;

    for (size_t i = 0; i < fsize / MD2_MSG_BLOCK_SIZE; i++) {
        for (size_t j = 0; j < 16; j++) {
            c = buf[MD2_MSG_BLOCK_SIZE * i + j];
            C[j] = S[c ^ L] ^ C[j];
            L = C[j];
        }
    }

    if (!(buf = realloc(buf, fsize + MD2_CHECKSUM_SIZE))) {
        free(buf);
        err("(%s)\tUnable to reallocate buffer\n", __func__);
    }

    memcpy(buf + fsize, C, MD2_CHECKSUM_SIZE);

    return fsize + MD2_CHECKSUM_SIZE;
}

