/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include "bmp.h"

int main(int argc, char *argv[]) {
    
    if (argc != 4) {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    float f = atof(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL) {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL) {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0) {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    int biRootWidth = bi.biWidth;
    int biRootHeight = bi.biHeight;
    int rtpad = (4 - (biRootWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    bi.biWidth = f * biRootWidth;
    bi.biHeight = f * biRootHeight;
    int outpad =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + outpad) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    RGBTRIPLE triple;
    for (int newsl = 0; newsl < biRootHeight; newsl++) { 
        for (int slrepeat = 0; slrepeat < f; slrepeat++) { 
            for (int pxl = 0; pxl < biRootWidth; pxl++) {
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                for (int i = 0; i < f; i++)
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
            for (int topad = 0; topad < outpad; topad++) 
                fputc(0x00, outptr);
            fseek(inptr, -(biRootWidth * 3), SEEK_CUR);
        }
        fseek(inptr, (biRootWidth * 3) + rtpad, SEEK_CUR);
    }
    fclose(inptr);
    fclose(outptr);
    return 0;
}