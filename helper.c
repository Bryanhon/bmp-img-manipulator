#include "helper.h"

/*!
 ******************************************************************************
 * Function Name: loadBmp                                                     *
 ******************************************************************************
 * Summary:                                                                   *
 *  Opens a file pointer to the .bmp image fills in the structs with the info *
 *  info from the .bmp image and ordens the pixel array in RGB order          *
 *                                                                            *
 * Parameters:                                                                *
 *  char *fileName                                                            *
 *  bmpFileHeader_t *bmpFH                                                    *
 *  bmpInfoHeader_t *bmpIH                                                    *
 *                                                                            * 
 * Return:                                                                    *
 *  The Pixel array location                                                  *
 ******************************************************************************
!*/
uint8_t *
loadBmp(char *fileName, bmpFileHeader_t *bmpFH ,bmpInfoHeader_t *bmpIH)
{
    FILE *fp;           // The file pointer
    uint8_t *bmpimg;    // pointer to store the image data
    
    // open filename in read binary mode & check if it openend correctly
    fp = fopen(fileName, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Failed opening file \"%s\"\n", fileName);
        exit(EXIT_FAILURE);
    }

    // read the bmp file header
    fread(bmpFH, sizeof(bmpFileHeader_t), 1, fp);
    
    // verify that this is a bmp file by checking the bitmap ID
    if (bmpFH->Type != 0x4D42) {
        fprintf(stderr, "bitmap ID check error.\n");
        exit(EXIT_FAILURE);
    }

    // read the bmp info header
    fread(bmpIH, sizeof(bmpInfoHeader_t), 1, fp);

    // move file point to the beginning of bitmap data
    fseek(fp, bmpFH->OffBits, SEEK_SET);
    
    // allocate enough memory for the bitmap image data
    bmpimg = (malloc(bmpIH->SizeImage));

    // verify memory allocation
    if (!bmpimg) {
        fprintf(stderr, "bmpimg memory allocation failure\n");
        exit(EXIT_FAILURE);
    }

    // read in the bitmap image data
    fread(bmpimg, bmpIH->SizeImage, 1, fp);

    // make sure the bitmap image data was read
    if (bmpimg == NULL) {
        fprintf(stderr, "error reading image data\n");
        exit(EXIT_FAILURE);
    }
    
    fclose(fp); // Closes the stream. All buffers are flushed
    return bmpimg;
}

/*!
 ******************************************************************************
 * Function Name: saveBmp                                                     *
 ******************************************************************************
 * Summary:                                                                   *
 *                                                                            *
 * Parameters:                                                                *
 *  char *fileName                                                            *
 *  bmpFileHeader_t *bmpFH                                                    *
 *  bmpInfoHeader_t *bmpIH                                                    *
 *                                                                            * 
 * Return:                                                                    *
 *  none                                                                      *
 ******************************************************************************
!*/
void 
saveBmp(
    char *fileName, bmpFileHeader_t *bmpFH, bmpInfoHeader_t *bmpIH, 
    uint8_t *bmpData)
{
    FILE *fp;
    
    // open filename in write binary mode & check if it openend correctly
    fp = fopen(fileName, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Failed opening file \"%s\"\n", fileName);
        exit(EXIT_FAILURE);
    }
    
    // write the bmp file header
    fwrite(bmpFH, 1, sizeof(bmpFileHeader_t), fp);
    
    // write the bmp info header
    fwrite(bmpIH, 1, sizeof(bmpInfoHeader_t), fp);
    
    // write the bmp pixel array
    fwrite(bmpData, 1, bmpIH->SizeImage, fp);
    
    fclose(fp); // Closes the stream. All buffers are flushed
    return;
}

/*!
 ******************************************************************************
 * Function Name: reverseBmp                                                  *
 ******************************************************************************
 * Summary:                                                                   *
 *  Reverses the values of the bmp's pixel array                              *
 *                                                                            *
 * Parameters:                                                                *
 *  uint8_t *bmpimg                                                           *
 *  uint32_t SizeImage                                                        *
 *                                                                            * 
 * Return:                                                                    *
 *  the edited image                                                          *
 ******************************************************************************
!*/
uint8_t *
reverseBmp(uint8_t *bmpimg, uint32_t SizeImage)
{
    uint32_t imgIdx = 0; // image index counter
    
    // go trough the image and invert every pixel byte
    for (imgIdx = 0; imgIdx < SizeImage; imgIdx++) {
        bmpimg[imgIdx] = ~bmpimg[imgIdx];
    }
    
    return bmpimg;
}

/*!
 ******************************************************************************
 * Function Name: applyFilter                                                 *
 ******************************************************************************
 * Summary:                                                                   *
 *  Applies the selected filter                                               *
 *                                                                            *
 * Parameters:                                                                *
 *  uint8_t *bmpimg                                                           *
 *  uint32_t SizeImage                                                        *
 *  enum filterID_e filterID                                                  *
 *                                                                            * 
 * Return:                                                                    *
 *  the edited image                                                          *
 ******************************************************************************
!*/
uint8_t *
applyFilter(uint8_t *bmpimg, uint32_t SizeImage, enum filterID_e filterID)
{
    uint32_t imgIdx   = 0; // image index counter
    uint8_t greyColor = 0; // temp file for the greyscale value
    
    switch (filterID) {
                case none:
        break;    case sepia:
        
        for (imgIdx = 0; imgIdx < SizeImage; imgIdx += 3) {
            bmpimg[imgIdx + 2]    // Red
                = (uint8_t)fmin((bmpimg[imgIdx + 2] * 0.393) + (bmpimg[imgIdx] * 0.769) + (bmpimg[imgIdx + 1] * 0.189), 255.0);
            bmpimg[imgIdx + 1]    // Green
                = (uint8_t)fmin((bmpimg[imgIdx + 2] * 0.349) + (bmpimg[imgIdx] * 0.686) + (bmpimg[imgIdx + 1] * 0.168), 255.0);
            bmpimg[imgIdx]        // Blue
                = (uint8_t)fmin((bmpimg[imgIdx + 2] * 0.272) + (bmpimg[imgIdx] * 0.534) + (bmpimg[imgIdx + 1] * 0.131), 255.0);
        }
        
        break;    case greyscale:
        
        for (imgIdx = 0; imgIdx < SizeImage; imgIdx += 3) {
            greyColor = (uint8_t)fmin(((bmpimg[imgIdx] + bmpimg[imgIdx + 1] + bmpimg[imgIdx + 2]) / 3), 255.0);
            bmpimg[imgIdx] = bmpimg[imgIdx + 1] = bmpimg[imgIdx + 2] = greyColor;
        }
        
        break;    default:
        break;
    }
    
    return bmpimg;
}

/*!
 ******************************************************************************
 * Function Name: endianess                                                   *
 ******************************************************************************
 * Summary:                                                                   *
 *  Checks if the system the program runs on is little or big endian          *
 *                                                                            *
 * Parameters:                                                                *
 *  None                                                                      *
 *                                                                            * 
 * Return:                                                                    *
 *  0 for big endian                                                          *
 *  1 for little endian                                                       *
 ******************************************************************************
!*/
uint8_t
endianness(void)
{
    volatile uint32_t x = 0x1;
    uint8_t *c = (uint8_t *)&x;

    return (*c);
}

/*!
 ******************************************************************************
 * Function Name: help                                                        *
 ******************************************************************************
 * Summary:                                                                   *
 *  Prints the help screen                                                    *
 *                                                                            *
 * Parameters:                                                                *
 *  None                                                                      *
 *                                                                            * 
 * Return:                                                                    *
 *  None                                                                      *
 ******************************************************************************
 !*/
void
help(void)
{
    printf("bmp - bmp\n\n");
    printf("Usage:\n");
    printf("bmp [(-h|--help)] [(-v|--verbose)] [(-i|--invert)] [(-r|--rotate) integer] [(-o|--outputfile) string] [(-f|--filter) integer]\n\n");
    printf("Usage example:\n");
    printf("bmp -i input.bmp -r90 -o output.bmp -f1\n");
    printf("This line will invert the image rotate it by 90* and than apply the sepia filter to it.\n\n");
    printf("Options:\n");
    printf("-h or --help: Displays this information.\n");
    printf("-v or --verbose: Verbose mode.\n");
    printf("-i or --invert: invert.\n");
    printf("-r or --rotate integer: Rotate in orders of 90.\n");
    printf("-o or --outputfile string: outputfile.\n");
    printf("-f or --filter integer: apply filter to the image.\n");
    printf("filter codes are:\n");
    printf("1 = sepia\n");
    printf("2 = greyscale\n");
    printf("NOTE: No 2 filters can be applied at the same time\n\n");
    
    return;
}

/*!
 ******************************************************************************
 * Function Name: bmpDump                                                     *
 ******************************************************************************
 * Summary:                                                                   *
 *  Print out imformation about the .bmp file                                 *
 *                                                                            *
 * Parameters:                                                                *
 *  bmpFileHeader_t *bmpFH                                                    *
 *  bmpInfoHeader_t *bmpIH                                                    *
 *  uint8_t *bmpData                                                          *
 *                                                                            * 
 * Return:                                                                    *
 *  None                                                                      *
 ******************************************************************************
!*/
void 
bmpDump(bmpFileHeader_t *bmpFH, bmpInfoHeader_t *bmpIH, uint8_t *bmpData)
{
    printf("bitmap file header:\n");
    printf("Type\t\t = 0x%X\n", bmpFH->Type);
    printf("Size\t\t = 0x%X\n", bmpFH->Size);
    printf("Reserved\t = 0x%x\n", bmpFH->Reserved1);
    printf("Reserved\t = 0x%x\n", bmpFH->Reserved2);
    printf("OffBits\t\t = %d bits\n", bmpFH->OffBits);
    printf("\n");
    printf("bitmap info header:\n");
    printf("Size\t\t = %d Bytes\n", bmpIH->Size);
    printf("Width\t\t = %d pixels\n", bmpIH->Width);
    printf("Height\t\t = %d pixels\n", bmpIH->Height);
    printf("Planes\t\t = %d\n", bmpIH->Planes);
    printf("BitCount\t = %d bits/pixel\n", bmpIH->BitCount);
    printf("Compression\t = 0x%X\n", bmpIH->Compression);
    printf("SizeImage\t = %d Bits\n", bmpIH->SizeImage);
    printf("XPelsPerMeter\t = %d\n", bmpIH->XPelsPerMeter);
    printf("YPelsPerMeter\t = %d\n", bmpIH->YPelsPerMeter);
    printf("ClrUsed\t\t = 0x%X\n", bmpIH->ClrUsed);
    printf("ClrImportant\t = 0x%X\n", bmpIH->ClrImportant);
    printf("\n\n");
    
    // prints the pixel array in hex format
    printf("Pixel Array:\n ");
    for (uint32_t i = 0; i < bmpIH->SizeImage; i++) {
        printf("%X", bmpData[i]);
        if (bmpData[i] == 0) printf("%X", 0);;
        if (((i + 1) % 32) == 0) printf("\n");
        printf(" ");
    } printf("\n");
    
    return;
}

/*!
 ******************************************************************************
 * Function Name: rotate                                                      *
 ******************************************************************************
 * Summary:                                                                   *
 *  rotates the image by a given amount of degrees                            *
 *                                                                            *
 * Parameters:                                                                *
 *  bmpFileHeader_t *bmpFH                                                    *
 *  bmpInfoHeader_t *bmpIH                                                    *
 *  uint8_t *bmpData                                                          *
 *  float degree                                                              *
 *                                                                            * 
 * Return:                                                                    *
 *  None                                                                      *
 ******************************************************************************
!*/
void 
rotate(
    bmpFileHeader_t *bmpFH, bmpInfoHeader_t *bmpIH,
    uint8_t *bmpData, float degree)
{
	
    return;
}