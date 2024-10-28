#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Accept a single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // Open the memory card
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Could not open file: %s\n", argv[1]);
        return 1;
    }

    // Create a buffer for a block of data
    uint8_t buffer[512];

    // Variables to keep track of JPEGs
    int file_count = 0;
    FILE *jpeg = NULL;

    // Read blocks from the memory card
    while (fread(buffer, 1, 512, card) == 512)
    {
        // Check if it's the start of a JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // Close previous JPEG file if open
            if (jpeg != NULL)
                fclose(jpeg);

            // Create new JPEG file
            char filename[8];
            sprintf(filename, "%03d.jpg", file_count++);
            jpeg = fopen(filename, "w");
            if (jpeg == NULL)
            {
                fprintf(stderr, "Could not create file: %s\n", filename);
                fclose(card);
                return 1;
            }
        }

        // Write block to JPEG file if one is open
        if (jpeg != NULL)
            fwrite(buffer, 1, 512, jpeg);
    }

    // Close files
    if (jpeg != NULL)
        fclose(jpeg);
    fclose(card);

    return 0;
}
