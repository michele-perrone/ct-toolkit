#include "imgtools.hpp"


void IMGTools::load_IMG(const char filename[], uint16_t (*IMG)[512][512])
{
    FILE* IMG_file = fopen(filename, "rb");
    int16_t temp;
    for (int i = 0; i < 512; i++)
    {
        for (int j = 0; j < 512; j++)
        {
            // Each pixel takes 2 bytes.
            // The values are between -4096 and +4095
            fread(&temp, sizeof(int16_t), 1, IMG_file);

            // Put the luminance between -65536 and +65535
            temp += 2048;
            (*IMG)[i][j] = temp * 16;
        }
    }
    fclose (IMG_file);

    return;
}

void IMGTools::IMG_to_grayscale_BMP_file(uint16_t (*IMG)[512][512], char file_name[])
{
    // Convert the IMG into a BW bitmap
    FILE* BW_bitmap_file = fopen(file_name, "wb");
    BITMAP BW_bitmap = CreateEmptyBitmap(512, 512);
    for (int i = 0; i < 512; i++)
    {
      for (int j = 0; j < 512; j++)
      {
         PIXEL (BW_bitmap, i, j).red =
         PIXEL (BW_bitmap, i, j).green =
         PIXEL (BW_bitmap, i, j).blue = (*IMG)[i][j] / 256;
      }
    }
    WriteBitmap (BW_bitmap, BW_bitmap_file);
    ReleaseBitmapData (&BW_bitmap);
    fclose (BW_bitmap_file);

    return;
}

void IMGTools::load_LUT(const char LUT_filename[], unsigned char (*LUT)[256][3])
{
    FILE* LUT_file = fopen(LUT_filename, "rb");
    //int LUT_file_fd = LUT_file->_fileno;

    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            fread(&((*LUT)[i][j]), 1, 1, LUT_file);
            //read(LUT_file_fd, &((*LUT)[i][j]), 1); // Each LUT channel is 1 byte
        }
    }
    fclose(LUT_file);

    return;
}

void IMGTools::IMG_to_color_BMP_file(uint16_t (*IMG)[512][512], unsigned char (*LUT)[256][3], char CLR_bitmap_filename[])
{
    FILE* CLR_bitmap_file = fopen(CLR_bitmap_filename, "wb");
    BITMAP CLR_bitmap = CreateEmptyBitmap(512, 512);
    int current_LUT_index = 0;
    for (int i = 0; i < 512; i++)
    {
      for (int j = 0; j < 512; j++)
      {
          current_LUT_index = (*IMG)[i][j] / 256;
          PIXEL (CLR_bitmap, i, j).red = ( (*LUT)[current_LUT_index][0] );
          PIXEL (CLR_bitmap, i, j).green = ( (*LUT)[current_LUT_index][1] );
          PIXEL (CLR_bitmap, i, j).blue = ( (*LUT)[current_LUT_index][2] );
      }
    }
    WriteBitmap (CLR_bitmap, CLR_bitmap_file);
    ReleaseBitmapData (&CLR_bitmap);
    fclose (CLR_bitmap_file);

    return;
}

BITMAP IMGTools::IMG_to_color_BMP(uint16_t (*IMG)[512][512], unsigned char (*LUT)[256][3])
{
    BITMAP CLR_bitmap = CreateEmptyBitmap(512, 512);
    int current_LUT_index = 0;
    for (int i = 0; i < 512; i++)
    {
      for (int j = 0; j < 512; j++)
      {
          current_LUT_index = (*IMG)[i][j] / 256;
          PIXEL (CLR_bitmap, i, j).red = ( (*LUT)[current_LUT_index][0] );
          PIXEL (CLR_bitmap, i, j).green = ( (*LUT)[current_LUT_index][1] );
          PIXEL (CLR_bitmap, i, j).blue = ( (*LUT)[current_LUT_index][2] );
      }
    }

    return CLR_bitmap;
}

BITMAP IMGTools::IMG_to_grayscale_BMP(uint16_t (*IMG)[512][512])
{
    BITMAP grayscale_bitmap = CreateEmptyBitmap(512, 512);
    for (int i = 0; i < 512; i++)
    {
      for (int j = 0; j < 512; j++)
      {
          PIXEL (grayscale_bitmap, i, j).red =
          PIXEL (grayscale_bitmap, i, j).green =
          PIXEL (grayscale_bitmap, i, j).blue = (*IMG)[i][j] / 256;
      }
    }

    return grayscale_bitmap;
}

