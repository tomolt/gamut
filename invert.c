/* Adapted from http://tools.suckless.org/farbfeld/invert.c */

#include "farbfeld.h"


int main(int argc, char** argv)
{
    uint32_t width, height;
    pixel_t rgba;

    if (argc != 1) {
        fprintf(stderr, "usage: %s\n", argv[0]);
        return 1;
    }

    if (read_header(&width, &height)) FATALF();

    if (write_header(width, height)) {
        perror("write header");
        return 1;
    }

    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            if (read_pixel(&rgba)) FATALF();

            /* invert colors */
            rgba.r = 65535 - rgba.r;
            rgba.g = 65535 - rgba.g;
            rgba.b = 65535 - rgba.b;

            if (write_pixel(&rgba)) FATALF();
        }
    }

    return 0;
}
