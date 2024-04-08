/*
recover.c
 
Computer Science 50
Problem Set 4

Recovers JPEGs from a forensic image.
*/

/*    
Old Hardcode
fseek(card, 1024, SEEK_SET);
fread(&buffer, 512, 1, card);
jpeg_pointer = fopen("000.jpg", "w");
fwrite(&buffer, 512, 1, jpeg_pointer);
*/

#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: recover ___.raw\n");
        return 1;
    }
    FILE* card = fopen(argv[1], "r");
    if (card==NULL) {
        fprintf(stderr, "Incompatible File");
        return 1;
    }
    unsigned char buffer[512];
    char jpegname[8]; // Needs a extra byte for NULL terminator
    FILE* jpeg_pointer = NULL;
    int jpeg_count = 0;

    while (fread(&buffer, 512, 1, card) == 1) {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] >> 4) == 0xe)) {
            if (jpeg_count > 0)
                fclose(jpeg_pointer);
            sprintf(jpegname, "%03i.jpg", jpeg_count++);
            jpeg_pointer = fopen(jpegname, "w");
            fwrite(&buffer, 512, 1, jpeg_pointer);
        }
        else if (jpeg_count > 0)
            fwrite(&buffer, 512, 1, jpeg_pointer);
    }
    if (jpeg_count == 0)
        return 1;
    fclose(jpeg_pointer);
    fclose(card);
    return 0;
}