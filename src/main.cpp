#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "bmp.hpp"
#include "quadtree.hpp"

void load_IMG(char filename[], short int (*IMG)[512][512]);
void load_LUT(char LUT_filename[], unsigned char (*LUT)[256][3]);
void IMG_to_BMP(short int (*IMG)[512][512], char file_name[]);
void IMG_to_color_BMP(short int (*IMG)[512][512], unsigned char (*LUT)[256][3], char CLR_bitmap_filename[]);

int main(int argc, char* argv[])
{
    char IMG_filename[MAX_NAME_LENGTH]; //= "../PosnetkiGlave/0120.img";
    char LUT_filename[MAX_NAME_LENGTH]; //= "../BarvnePalete/smart.lut";
    char BW_bitmap_filename[] = "bw_bitmap.bmp";
    char CLR_bitmap_filename[] = "clr_bitmap.bmp";
    char tree_filename[] = "tree.bin";
    char reconstructed_BMP_filename[] = "rcstr_bw_bitmap.bmp";

    short int IMG [512][512];
    unsigned char LUT [256][3];
    short int compression_parameter;

    char bash_command[128];

    // Show the IMG
    if (*argv[1] == 'S' && argc == 4)
    {
        // Store the file names
        sscanf(argv[2], "%s", IMG_filename);
        sscanf(argv[3], "%s", LUT_filename);

        // Store the IMG file into a short int matrix
        load_IMG(IMG_filename, &IMG);

        // Create a grayscale BitMap from the IMG matrix
        IMG_to_BMP(&IMG, BW_bitmap_filename);

        // Store the LUT color matrix
        load_LUT(LUT_filename, &LUT);

        // Convert the IMG into a color bitmap
        IMG_to_color_BMP(&IMG, &LUT, CLR_bitmap_filename);

        printf("Opening up the bitmaps...\n");
        sprintf(bash_command, "xdg-open %s & xdg-open %s &", BW_bitmap_filename, CLR_bitmap_filename);
        system(bash_command);

    }
    // Compress
    else if (*argv[1] == 'C' && argc == 5)
    {
        sscanf(argv[2], "%s", IMG_filename);
        sscanf(argv[3], "%s", tree_filename);
        compression_parameter = atoi (argv[4]);

        // Store the IMG file into a short int matrix
        load_IMG(IMG_filename, &IMG);

        printf("Compressing... ");
        // Create a quadtree and save it to a file
        QuadTree* quadtree = new QuadTree(&IMG, 512, compression_parameter);
        quadtree->construct_tree(quadtree->root_node);
        quadtree->write_binarized_tree_to_file(tree_filename);
        delete quadtree;
        printf("done!\n");
    }
    // Decompress
    else if (*argv[1] == 'D' && argc == 4)
    {
        sscanf(argv[2], "%s", tree_filename);
        sscanf(argv[3], "%s", reconstructed_BMP_filename);

        printf("Decompressing... ");
        // Read the binarized quadtree from file and reconstruct the IMG
        QuadTree* quadtree = new QuadTree(&IMG, 512, 0);
        quadtree->read_binarized_tree_from_file(tree_filename);
        quadtree->reconstruct_IMG();
        IMG_to_BMP(quadtree->IMG, reconstructed_BMP_filename);
        delete quadtree;
        printf("done! Opening up the bitmap...\n");
        sprintf(bash_command, "xdg-open %s &", reconstructed_BMP_filename);
        system(bash_command);
    }
    // Usage help
    else
    {
        printf("Usage:\n");
        printf("   Show IMG:\t\t[S] <foo.IMG> <foo.LUT>\n");
        printf("   Compress IMG:\t[C] <foo.IMG> <foo.BIN>\n");
        printf("   Decompress BIN:\t[D] <foo.BIN> <foo.BMP>\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}


void load_IMG(char filename[], short int (*IMG)[512][512])
{
    FILE* IMG_file = fopen(filename, "rb");
    //int IMG_file_fd = IMG_file->_fileno;
    for (int i = 0; i < 512; i++)
    {
        for (int j = 0; j < 512; j++)
        {
            fread(&((*IMG)[i][j]), sizeof(short int), 1, IMG_file);
            //read(IMG_file_fd, &((*IMG)[i][j]), 2); // Each pixel is 2 bytes, aka short int
        }
    }
    fclose (IMG_file);

    return;
}

void IMG_to_BMP(short int (*IMG)[512][512], char file_name[])
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
         PIXEL (BW_bitmap, i, j).blue = ( (((*IMG)[i][j] + 2048) / 4095.) * 255 );
      }
    }
    WriteBitmap (BW_bitmap, BW_bitmap_file);
    ReleaseBitmapData (&BW_bitmap);
    fclose (BW_bitmap_file);

    return;
}

void load_LUT(char LUT_filename[], unsigned char (*LUT)[256][3])
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

void IMG_to_color_BMP(short int (*IMG)[512][512], unsigned char (*LUT)[256][3], char CLR_bitmap_filename[])
{
    FILE* CLR_bitmap_file = fopen(CLR_bitmap_filename, "wb");
    BITMAP CLR_bitmap = CreateEmptyBitmap(512, 512);
    int current_LUT_index = 0;
    for (int i = 0; i < 512; i++)
    {
      for (int j = 0; j < 512; j++)
      {
          current_LUT_index = (((*IMG)[i][j] + 2048) / 4095. * 255);
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
