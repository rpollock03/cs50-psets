#include <stdio.h>
#include <stdlib.h>

//#define bufferSize 512 // size of FAT block
#define TRUE 1
#define FALSE 0

int main(int argc, char *argv[])
{
    //Must have 2 arguments passed in (argv0 is program name and argv1 the file name) otherwise return error
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    //remember input filename
    char *infile = argv[1]; //infile is address/pointer to the file inputted via argv

    //open card file
    FILE *inptr = fopen(infile, "r"); //open the file at address infile
    if (infile == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    //define variables and buffer array which will store the blocks read from the infile
    int fileNumber = 0; //files found incrememnted for file name
    unsigned char buffer[512]; // static allocation because we know size 512. unsigned char because char too small for hex
    int isFileOpen = FALSE; //basically a bool for condition loops

    FILE *outfile; //declare outfile pointer

    //load 512 elements of 1 byte into the buffer repeatedly until no more elemnets left.
    while (fread(buffer, 1, 512, inptr) == 512) //If fread returns <512/blocksize we are at endoffile
    {
        //if new jpg header signature found
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (isFileOpen == TRUE)
            {
                fclose(outfile); // close any previously opened file before making a new one
            }

            fileNumber++; //increment new files found for the file name

            //create output file
            char fileName[8]; // declaration - ###.jpg\0 so 7 characters plus null = 8
            //sprintf like print, except reutnrs to first parameter ie filename. -1 because we want to increment from 000
            sprintf(fileName, "%03i.jpg", fileNumber - 1);
            outfile = fopen(fileName, "w"); //outfile is a pointer to the new file called filename, with write permission.
            //in this case it creates a file because no outfile exists with that name
            if (outfile == NULL) //if no space made for new file
            {
                fclose(inptr);//close input file
                fprintf(stderr, "Could not create %s.\n", fileName); //filename rather than the pointer outfile
                return 3;
            }
            //write buffer to the outfile
            fwrite(&buffer, 1, 512, outfile); // fwrite creates a file if none existent
            //make sure program remembers that an existing file is being made
            isFileOpen = TRUE;
        }

        else // if block contains no header elements add to the recently worked on file
        {
            // IMPORTANT. Without this condition, fwrite would try to write buffer to memory that not been allocated. SEG fault.
            if (isFileOpen == TRUE)
            {
                fwrite(&buffer, 1, 512, outfile);
            }
        }

    }
    //else fread returns <512 so at end of the file
    // close infile and outfile (address/pointer)
    fclose(inptr);
    fclose(outfile);
    // success
    return 0;
}