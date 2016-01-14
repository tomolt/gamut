/* Adapted from http://tools.suckless.org/farbfeld/invert.c */

#include "farbfeld.h"


int main(int argc, char** argv)
{
    uint32_t width, height;

    if (read_header(&width, &height, stdin)) { FATALF(); }

    if (write_header(width, height, stdout)) {
        perror(NULL);
        return 1;
    }

    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            pixel_t rgba;

            if (read_pixel(&rgba, stdin)) FATALF();

            /* invert colors */
            rgba[0] = 65535 - rgba[0];
            rgba[1] = 65535 - rgba[1];
            rgba[2] = 65535 - rgba[2];

            if (write_pixel(rgba, stdin)) FATALF();
        }
    }

    return 0;
}
