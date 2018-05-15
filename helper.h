#ifndef _HELPER_H_
#define _HELPER_H_

#include <stdio.h>   // fclose, fopen, fseek, fwrite, fread, printf
#include <stdlib.h>  // atoi, malloc, free, exit,  
#include <stdint.h>  // int typedefs
#include <stdbool.h> // true, false
#include <string.h>  // strcmp
#include <getopt.h>  //
#include <math.h>    //

#define _DEBUG
     
enum filterID_e {
    none         = 0,
    sepia        = 1,
    greyscale    = 2,
};

// structure for holding the bitmaps file header
#pragma pack(push, 1)
typedef struct bitmapFileHeader_s {
    uint16_t Type;       // specifies the file type
    uint32_t Size;       // specifies the size in bytes of the bitmap file
    uint16_t Reserved1;  // reserved; MUST be 0
    uint16_t Reserved2;  // reserved; MUST be 0
    uint32_t OffBits;    // specifies the off in bytes from the bitmapfileheader to the bitmap bits        
} bmpFileHeader_t;
#pragma pack(pop)

// structure for holding the bitmaps info header
#pragma pack(push, 1)
typedef struct bitmapFileInfoHeader_s {
    uint32_t Size;           // specifies the number of bytes required by the struct
    uint32_t Width;          // specifies width in pixels
    uint32_t Height;         // species height in pixels
    uint16_t Planes;         // specifies the number of color planes, must be 1
    uint16_t BitCount;       // specifies the number of bit per pixel
    uint32_t Compression;    // spcifies the type of compression
    uint32_t SizeImage;      // size of image in bytes
    uint32_t XPelsPerMeter;  // number of pixels per meter in x axis
    uint32_t YPelsPerMeter;  // number of pixels per meter in y axis
    uint32_t ClrUsed;        // number of colors used by th ebitmap
    uint32_t ClrImportant;   // number of colors that are important
} bmpInfoHeader_t;
#pragma pack(pop)

// loads in the info of the bitmap
uint8_t *loadBmp(char *fp, bmpFileHeader_t *bmpFH, bmpInfoHeader_t *bmpIH);

// saves the bmp 
void saveBmp(
    char *fp, bmpFileHeader_t *bmpFH, bmpInfoHeader_t *bmpIH, 
    uint8_t *bmpData);

// inverses the colors of the bmp pixel array
uint8_t *reverseBmp(uint8_t *bmpimg, uint32_t SizeImage);

// apply a filter to the image
uint8_t *applyFilter(uint8_t *bmpimg, uint32_t SizeImage, enum filterID_e filerID);

// checks if the system the program runs on is little or big endian
uint8_t endianness(void);

// help screen
void help(void);

// dump info about the bmp file
void bmpDump(bmpFileHeader_t *bmpFH, bmpInfoHeader_t *bmpIH, uint8_t *bmpData);

// rotates the given images by the given amount of degrees
void rotate(
    bmpFileHeader_t *bmpFH, bmpInfoHeader_t *bmpIH,
    uint8_t *bmpData, float degree);

#endif//_HELPER_H_