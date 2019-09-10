/*

Steganografy simple test

Works only with 24 BPP bitmap.

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
  
int main(int argc, char *argv[]) 
{
	// Check argument
	if (argc < 3)
	{	
		puts("Syntax: stegano [e|d] /path/to/image.bmp");
		return 1;
	}
	
	// Decode or encode?
	char op = argv[1][0];

	// Open the file
	FILE *fp;
	if (op == 'e')
		fp = fopen(argv[2], "rb+");
	else
		fp = fopen(argv[2], "rb");

	// Check errors
	if (!fp)
	{
		printf("Syntax: stegano [e|d] /path/to/image.bmp\nError opening file %s", argv[2]);
		return 2;
	}	

	// Get the pixels array offset
	fseek(fp, 10, SEEK_SET);

	int32_t pixel_start = fgetc(fp) | fgetc(fp) << 8 | fgetc(fp) << 16 | fgetc(fp) << 24;

	// Get image height and width
	fseek(fp, 18, SEEK_SET);
	
	int32_t width = fgetc(fp) | fgetc(fp) << 8 | fgetc(fp) << 16 | fgetc(fp) << 24;

	int32_t height = fgetc(fp) | fgetc(fp) << 8 | fgetc(fp) << 16 | fgetc(fp) << 24;

	// Calculate the padding
	int8_t padding;
	if (padding = (width * 3) % 4)
		padding = 4 - padding;

	// Set the pointer to the pixels data
	fseek(fp, pixel_start, SEEK_SET);

	// Allocate buffer for the message
	int32_t message_max_lenght = width*height/3;
	char* message = calloc(message_max_lenght, 1);

	// Get the message from the user
	if (op == 'e')
	{
		puts("Enter the message to write on the image:");
		fgets(message, message_max_lenght, stdin);
	}

	// Loop over the image pixels
	int64_t i = 0;
	for (int32_t row = 0; row < height; row++) 
	{
		for (int32_t col = 0; col < width; col++) 
		{
			for (int32_t chan = 0; chan < 3; chan++)
			{
				// Encode
				if (op == 'e')
				{
					char col = fgetc(fp) & 254;
					char msg = (message[i/9] >> i%8) & 1;
					fseek(fp, -1, SEEK_CUR);
					fputc(col | msg, fp);

					if ((message[i/9-1] == '\0') && i > 9)
						break;
				}

				// Decode
				else
				{
					char col = fgetc(fp) & 1;
					message[i/9] |= col << i%8;

					if ((message[i/9-1] == '\0') && i > 9)
						break;
				}

				i++;
			}
		}

		// Skip padding pixels
		fseek(fp, padding, SEEK_CUR);
	}

	// Close the file
	fclose(fp);

	// Print the message
	if (op == 'd')
		puts(message);

	// Exit
	return 0;
}
