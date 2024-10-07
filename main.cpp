#include "PNG Compr.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        return 1;
    }

    PNGCompressor compressor;

    if (!compressor.read_png_file(argv[1])) {
        return 1;
    }

    if (!compressor.write_png_file(argv[2])) {
        return 1;
    }

    return 0;
}