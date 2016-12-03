#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./copy infile outfile\n");
        return 1;
    }
    
    int factor = atoi(argv[1]);
    if (factor>100 || factor < 0)
    {
        printf("please enter a postive number less than 101");
        return 1;
    }

    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf,bf_1;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    bf_1 = bf;

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi,bi_1;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    bi_1 = bi;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    bi_1.biHeight = bi.biHeight * factor;
    bi_1.biWidth = bi.biWidth * factor;
    int padding_1 =  (4 - (bi_1.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    bi_1.biSizeImage = (abs(bi_1.biHeight) * bi_1.biWidth) * 3 + (padding_1 * abs(bi_1.biHeight));
    bf_1.bfSize = 54 + bi_1.biSizeImage;
    int line =  bi.biWidth * 3 + padding;

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf_1, sizeof(BITMAPFILEHEADER), 1, outptr);
    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_1, sizeof(BITMAPINFOHEADER), 1, outptr);

    int i, j, k, l;
    RGBTRIPLE triple;
    for (i = 0; i < abs(bi.biHeight); i++)
    {
        fseek(inptr, 54 + i * line, SEEK_SET);
        for (k = 0; k < factor; k++)
        {
            for (j = 0; j < bi.biWidth; j++)
            {
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                for (l = 0; l < factor; l++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            // then add it back (to demonstrate how)
            for (l = 0; l < padding_1; l++)
            {   
                fputc(0x00, outptr);
            }
            fseek(inptr, 54 + i * line, SEEK_SET);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
