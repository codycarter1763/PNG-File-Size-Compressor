#include "PNG Compr.h"
#include <stdlib.h>

PNGCompressor::PNGCompressor() : png_ptr(nullptr), info_ptr(nullptr), row_pointers(nullptr), width(0), height(0) {}

PNGCompressor::~PNGCompressor() {
    clear_memory();
    if (png_ptr && info_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    }
}

void PNGCompressor::clear_memory() {
    if (row_pointers) {
        for (int i = 0; i < height; i++) {
            free(row_pointers[i]);
        }
        free(row_pointers);
        row_pointers = nullptr;
    }
}

bool PNGCompressor::read_png_file(const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        perror("File open failed");
        return false;
    }

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    info_ptr = png_create_info_struct(png_ptr);

    if (setjmp(png_jmpbuf(png_ptr))) {
        perror("Error during init_io");
        fclose(fp);
        return false;
    }

    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);

    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);

    // Adjust the PNG format as necessary
    if (bit_depth == 16)
        png_set_strip_16(png_ptr);
    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_ptr);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png_ptr);
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_ptr);
    png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);

    png_read_update_info(png_ptr, info_ptr);

    // Allocate memory for rows
    row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr, info_ptr));
    }

    png_read_image(png_ptr, row_pointers);
    fclose(fp);
    return true;
}

bool PNGCompressor::write_png_file(const char* filename) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        perror("File open failed");
        return false;
    }

    png_init_io(png_ptr, fp);
    png_set_compression_level(png_ptr, 9); // Max compression level
    png_set_filter(png_ptr, 0, PNG_FILTER_NONE); // Choose a filter

    png_write_info(png_ptr, info_ptr);
    png_write_image(png_ptr, row_pointers);
    png_write_end(png_ptr, NULL);

    fclose(fp);
    return true;
}