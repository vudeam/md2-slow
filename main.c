#include <malloc.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"


int main(int argc, char * argv[]) {
    if (argc <= 1) {
        err("Not enough arguments proveded.\nUsage: %s [file]\n", argv[0]);
        // fprintf(stderr, "Not enough arguments provided\n");
        // fprintf(stderr, "Usage: %s [file]", argv[0]);

        return -1;
    }

    FILE * in_file;
    uint8_t * buf;
    long int file_len;

    if (!(in_file = fopen(argv[1], "rb"))) {
        //fprintf(stderr, "Error reading file %s", argv[1]);
        err("Error reading file '%s'\n", argv[1]);

        return -1;
    }

    //in_file = fopen("./test1.txt", "rb");

    fseek(in_file, 0, SEEK_END);
    file_len = ftell(in_file);
    rewind(in_file);

    fclose(in_file);

    printf("File size: %ld\n", file_len);

    buf = malloc(file_len * sizeof(uint8_t));
    fread(buf, file_len, 1, in_file);

    free(buf);

    return 0;
}

