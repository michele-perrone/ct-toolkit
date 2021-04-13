#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


#include "app/quadtree.hpp"
#include "app/imgtools.hpp"

#include "gui/mainwindow.h"
#include <QApplication>



int main(int argc, char* argv[])
{
    if(argc == 2 && *argv[1] == 'G')
    {
        // Set up the main window
        qputenv("QT_STYLE_OVERRIDE","");
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
    }

    char IMG_filename[MAX_NAME_LENGTH]; // = "../PosnetkiGlave/0120.img";
    char LUT_filename[MAX_NAME_LENGTH]; // = "../BarvnePalete/smart.lut";
    char BW_bitmap_filename[] = "bw_bitmap.bmp";
    char CLR_bitmap_filename[] = "clr_bitmap.bmp";
    char tree_filename[] = "tree.bin";
    char reconstructed_BMP_filename[] = "rcstr_bw_bitmap.bmp";

    uint16_t IMG [512][512];
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
        IMGTools::load_IMG(IMG_filename, &IMG);

        // Create a grayscale BitMap from the IMG matrix
        IMGTools::IMG_to_grayscale_BMP_file(&IMG, BW_bitmap_filename);

        // Store the LUT color matrix
        IMGTools::load_LUT(LUT_filename, &LUT);

        // Convert the IMG into a color bitmap
        IMGTools::IMG_to_color_BMP_file(&IMG, &LUT, CLR_bitmap_filename);

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
        IMGTools::load_IMG(IMG_filename, &IMG);

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
        IMGTools::IMG_to_grayscale_BMP_file(quadtree->IMG, reconstructed_BMP_filename);
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


