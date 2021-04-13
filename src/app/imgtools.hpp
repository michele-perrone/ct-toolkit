#ifndef IMGTOOLS_HPP
#define IMGTOOLS_HPP

#include <cstdio>
#include "app/bmp.hpp"

namespace IMGTools
{
    void load_IMG(const char filename[], uint16_t (*IMG)[512][512]);
    void load_LUT(const char LUT_filename[], unsigned char (*LUT)[256][3]);
    void IMG_to_grayscale_BMP_file(uint16_t (*IMG)[512][512], char file_name[]);
    void IMG_to_color_BMP_file(uint16_t (*IMG)[512][512], unsigned char (*LUT)[256][3], char CLR_bitmap_filename[]);
    BITMAP IMG_to_color_BMP(uint16_t (*IMG)[512][512], unsigned char (*LUT)[256][3]);
}

#endif // IMGTOOLS_HPP
