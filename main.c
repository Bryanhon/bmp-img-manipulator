/*!
 **************************************************************************************************************************
 * Sources used:                                                                                                          *
 **************************************************************************************************************************
 * Books:                                                                                                                 *
 *  - https://www.bol.com/nl/f/c-programming-language/9200000010082279/?country=BE                                        *
 *                                                                                                                        *
 * Sites:                                                                                                                 *
 *  - http://getoptgenerator.dafuer.es                                                                                    *
 *  - https://linux.die.net/man/3/getopt_long                                                                             *
 *  - https://en.wikipedia.org/wiki/Endianness                                                                            *
 *  - https://en.wikipedia.org/wiki/BMP_file_format                                                                       *
 *  - https://en.wikipedia.org/wiki/XOR_swap_algorithm                                                                    *
 *  - http://blog.paphus.com/blog/2012/08/14/write-your-own-bitmaps/                                                      *
 *  - https://stackoverflow.com/questions/14279242/read-bitmap-file-into-structure                                        *
 *  - https://www.techrepublic.com/blog/how-do-i/how-do-i-convert-images-to-grayscale-and-sepia-tone-using-c/             *
 *  - https://web.archive.org/web/20080912171714/http://www.fortunecity.com/skyscraper/windows/364/bmpffrmt.html          *
 *  - https://stackoverflow.com/questions/9448478/what-is-wrong-with-this-sepia-tone-conversion-algorithm/9448635#9448635 *
 **************************************************************************************************************************
!*/

#include "helper.h"

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        help();
        exit(EXIT_SUCCESS);
    }

    const struct option long_options[] = {
        { "help",       0, NULL, 'h' },
        { "verbose",    0, NULL, 'v' },
        { "invert",     0, NULL, 'i' },
        { "rotate",     1, NULL, 'r' },
        { "outputfile", 1, NULL, 'o' },
        { "filter",     1, NULL, 'f' },
        { NULL,         0, NULL, 0 }
    };
    const char *short_options = "hvir:o:f:";

    bool verbose          = false;
    int32_t rotation      = 0;
    uint8_t filter        = 0;
    uint32_t next_option  = 0;
    uint8_t invert        = 0;
    uint8_t endian        = endianness();
    uint8_t *bmpData      = NULL;
    int8_t *outputfile    = NULL;

    bmpFileHeader_t bmpFH;
    bmpInfoHeader_t bmpIH;

    // parse options
    while (1) {
        // obtain a option
        next_option = getopt_long(argc, argv, short_options, long_options, NULL);

        if (next_option == -1)
            break; // no more options. Break loop

        switch (next_option) {
                   case 'h':    help();
                                exit(EXIT_SUCCESS);
            break; case 'v':    verbose = true;
            break; case 'i':    invert = 1;
            break; case 'r':    rotation = atoi(optarg);
            break; case 'o':    outputfile = malloc(sizeof(char) * (strlen(optarg) + 1));
                                strcpy((char *)outputfile, optarg);
            break; case 'f':    filter = atoi(optarg);
            break; case '?':    help();
                   case -1:     // no more options
            break; default:     exit(EXIT_FAILURE);
        }
    }

    // load in the .bmp file
    bmpData = loadBmp(argv[optind], &bmpFH, &bmpIH);
    rotate(&bmpFH, &bmpIH, bmpData, 0);

#ifdef _DEBUG
    if (verbose) {
        printf("System endianness == %s\n", (endian ? "little endian" : "big endian"));
        printf(".bmp file before editing:\n");
        bmpDump(&bmpFH, &bmpIH, bmpData);
    }
#endif

    // if the invert flag is set invert the image
    if (invert) bmpData = reverseBmp(bmpData, bmpIH.SizeImage);

    // if there is a filter pressent apply it
    if (filter) bmpData = applyFilter(bmpData, bmpIH.SizeImage, filter);

    // if the outputfile hasn't been declared take on default name of "output.bmp"
    if (outputfile != NULL)
        saveBmp((char *)outputfile, &bmpFH, &bmpIH, bmpData);
    else
        saveBmp("output.bmp", &bmpFH, &bmpIH, bmpData);

#ifdef _DEBUG
    if (verbose) {
        printf(".bmp file after editing:\n");
        bmpDump(&bmpFH, &bmpIH, bmpData);
    }
#endif

    free(bmpData);

    return 0;
}
