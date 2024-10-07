#ifndef PNG_FILE_COMPRESSOR_H
#define PNG_FILE_COMPRESSOR_H

#include <png.h>
#include <zlib.h>

class PNGCompressor {
public:
    PNGCompressor();
    ~PNGCompressor();

    bool read_png_file(const char* filename);
    bool write_png_file(const char* filename);

private:
    png_structp png_ptr;
    png_infop info_ptr;
    png_bytep* row_pointers;
    int width, height;

    void clear_memory();
};

#endif // PNG_FILE_COMPRESSOR_H