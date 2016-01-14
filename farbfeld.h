/* Utility functions, adapted from farbfeld's ff2png.c and png2ff.c */

#pragma once

#include <arpa/inet.h>

#include <stdint.h>
#include <stdio.h>
#include <strings.h>

#define _FF_HEADER "farbfeld########"

char* ff_error = NULL;

#define FATALF() { fprintf(stderr, "%s: %s\n", argv[0], ff_error); return 1; }

typedef uint16_t pixel_t[4];

static int read_header(uint32_t* width, uint32_t* height, FILE* fp)
{
    uint8_t hdr[16] = {0};

    if (fread(hdr, 1, strlen(_FF_HEADER), fp) != strlen(_FF_HEADER)) {
        ff_error = "incomplete header\n";
        return 1;
    }

    if (memcmp("farbfeld", hdr, strlen("farbfeld"))) {
        ff_error = "invalid magic value\n";
        return 1;
    }

    *width = ntohl(*((uint32_t *)(hdr + 8)));
    *height = ntohl(*((uint32_t *)(hdr + 12)));

    return 0;
}


static int write_header(uint32_t width, uint32_t height, FILE* fp)
{
    uint16_t tmp;

    fputs("farbfeld", fp);
    tmp = htonl(width);

    if (fwrite(&tmp, sizeof(uint32_t), 1, fp) != 1)
        return 1;

    tmp = htonl(height);
    if (fwrite(&tmp, sizeof(uint32_t), 1, fp) != 1)
        return 1;

    return 0;
}


static inline int read_pixel(pixel_t* rgba, FILE* fp)
{
    if (fread(rgba, sizeof(uint16_t), 4, fp) != 4) {
        ff_error = "unexpected EOF";
        return 1;
    }

    for (uint32_t k = 0; k < 4; k++) {
        *rgba[k] = ntohs(*rgba[k]);
    }

    return 0;
}


static inline int write_pixel(pixel_t rgba, FILE* fp)
{
    pixel_t copy;

    for (uint32_t k = 0; k < 4; k++) {
        copy[k] = htons(rgba[k]);
    }

    if (fwrite(copy, sizeof(uint16_t), 4, fp) != 4) {
        fprintf(stderr, "write error\n");
        return 1;
    }

    return 0;
}
