
/*****************************************************************

 Copyright 2001   PIER LUCA MONTESSORO

 University of Udine
 ITALY

 montessoro@uniud.it
 www.montessoro.it

 This file is part of a freeware open source software package.
 It can be freely used (as it is or modified) as long as this
 copyright note is not removed.

******************************************************************/


#include <stdint.h>

/* Windows bitmap files (.BMP) version 3 */

#ifndef MMLIB_DATATYPES_DEFINED
typedef  uint8_t       _byte;
typedef  uint16_t      _word;
typedef  uint32_t      _dword;
#define MMLIB_DATATYPES_DEFINED
#endif


#define BMPFILETYPE 0x4D42


#ifdef GENERIC_COMPILER

/* header structure definitions */

typedef struct tagFILEHEADER
{
   word  ImageFileType;
   dword FileSize;
   word  Reserved1;
   word  Reserved2;
   dword ImageDataOffset;
} FILEHEADER;

typedef struct tagBMPHEADER
{
   dword HeaderSize;
   dword ImageWidth;
   dword ImageHeight;
   word  NumberOfImagePlanes;
   word  BitsPerPixel;
   dword CompressionMethod;
   dword SizeOfBitmap;
   dword HorizonalResolution;
   dword VerticalResolution;
   dword NumberOfColorsUsed;
   dword NumberOfSignificantColors;
} BMPHEADER;

typedef struct tagCOLORTRIPLE
{
   byte blue;
   byte green;
   byte red;
} COLORTRIPLE;

#endif



#ifdef __GNUC__

/* header structure definitions for GCC on Intel architectures,
   to avoid 32-bit alignement of 16-bit words */

typedef struct tagFILEHEADER
{
   _word  ImageFileType             __attribute__ ((packed));
   _dword FileSize                  __attribute__ ((packed));
   _word  Reserved1                 __attribute__ ((packed));
   _word  Reserved2                 __attribute__ ((packed));
   _dword ImageDataOffset           __attribute__ ((packed));
} FILEHEADER;

typedef struct tagBMPHEADER
{
   _dword HeaderSize                 __attribute__ ((packed));
   _dword ImageWidth                 __attribute__ ((packed));
   _dword ImageHeight                __attribute__ ((packed));
   _word  NumberOfImagePlanes        __attribute__ ((packed));
   _word  BitsPerPixel               __attribute__ ((packed));
   _dword CompressionMethod          __attribute__ ((packed));
   _dword SizeOfBitmap               __attribute__ ((packed));
   _dword HorizonalResolution        __attribute__ ((packed));
   _dword VerticalResolution         __attribute__ ((packed));
   _dword NumberOfColorsUsed         __attribute__ ((packed));
   _dword NumberOfSignificantColors  __attribute__ ((packed));
} BMPHEADER;

typedef struct tagCOLORTRIPLE
{
   _byte blue;
   _byte green;
   _byte red;
} COLORTRIPLE;

#endif

/* IMPORTANT NOTES:

   The number of bytes in one row of the file must always be adjusted to
   fit into the border of a multiple of four. Bytes set to zero must be
   appended if necessary.

   The image is stored upside down.
*/



typedef struct tagBITMAP
{
   _dword width;
   _dword height;
   COLORTRIPLE *pixel;
   FILEHEADER fileheader;
   BMPHEADER bmpheader;
} BITMAP;


/* useful macro */

#define PIXEL(image, row, column)  \
        (image).pixel [(row) * image.width + (column)]


/* functions prototipes */

BITMAP  ReadBitmap (FILE *fp);
void    WriteBitmap (BITMAP bitmap, FILE *fp);
BITMAP  CreateEmptyBitmap (_dword height, _dword width);
void    ReleaseBitmapData (BITMAP *bitmap);

