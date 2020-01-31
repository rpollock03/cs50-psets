#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<ctype.h>

#include "bmp.h"

int main(int argc, char *argv[]) //pointers to the parameters passed into system
{
    //Must have 3 arguments passed in (argv0 being program name) otherwise return error
    if (argc != 4)
    {
        fprintf(stderr, "Usage: copy infile outfile\n");
        return 1;
    }

    // remember filenames and scale
    char *infile = argv[2]; //file to be read
    char *outfile = argv[3]; //file to be written

    int scale = atoi(argv[1]); //convert argv1 to integer for scaling
    if (scale <= 0 || scale > 99) // if scaling not positive or less than 100 return error
    {
        fprintf(stderr, "Please use positive number, 0-99\n");
        return 1;
    }

    // open input file by setting pointer to that file
    FILE *inptr = fopen(infile, "r"); // name and r for reading
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w"); //name and w for writing
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf; //new variable bf of type BITMAPFILEHEADER
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr); //input file bitmapfileheader assigned to bf variable

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi; //new variable bi of type BITMAPINFOHEADER
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr); //input file bitmapinfoheader assigned to bi variable

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    //store original header info for later use
    //we access the variables within the structs via dot notation
    int paddingOriginal = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int originalImageWidth = abs(bi.biWidth); //in number of pixels
    int originalImageHeight = abs(bi.biHeight); //in number of pixels

    //implement changes to header info
    bi.biWidth *= scale;
    int paddingNew = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    bi.biHeight *= scale;
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + paddingNew) * abs(bi.biHeight); //total size in bytes of pixels and padding

    //implement changes to file header
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0; i < originalImageHeight; i++) //for each row of original image
    {
        for (int x = 0; x < scale; x++) //repeat scale number of times for vertical resize
        {
            fseek(inptr, 54 + (originalImageWidth * 3 + paddingOriginal) * i, SEEK_SET); //reset infile image scanline
            //note 54 is number of bytes of file header and info. So seeking 54 bytes from start (seekset) brings us to image
            for (int j = 0; j < originalImageWidth; j++) //for each pixel in scanline
            {
                // temporary storage
                RGBTRIPLE triple;
                // read RGB triple from infile, 1 RGB triple at a time and store it to triple variable
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                for (int m = 0; m < scale; m++) //write triple to outfile scale times for horizontal resize
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            fseek(inptr, paddingOriginal, SEEK_CUR); // skip padding on original image
            for (int k = 0; k < paddingNew; k++) //add padding to output image
            {
                fputc(0x00, outptr); // padding indicated by oxoo
            }
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}

