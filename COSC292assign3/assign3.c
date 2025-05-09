#define _CRT_SECURE_NO_WARNINGS // to use older functions like fopen instead of fopen_s
#include <stdlib.h> // for malloc
#include "assign3.h"

#define MAX_FILENAME_SIZE	256

// Open a file given a prompt for the user (for the type of file) and a file mode
FILE* GetFile(const char* cPrompt, const char* cMode)
{
	FILE* aFile = NULL;
	char cFileName[MAX_FILENAME_SIZE];

	// Get the file name from the user
	printf("%s", cPrompt);
	gets_s(cFileName, MAX_FILENAME_SIZE);

	// Open the file with the given name and mode passed in
	aFile = fopen(cFileName, cMode);

	return aFile;
}

// Read an image into the image pointer provided from the already opened file pointer
void ReadImage(IMAGE* imgPtr, FILE* infile)
{
	// Read in the header
	ReadHeader(imgPtr, infile);

	if (imgPtr->bmHDR != NULL)
	{
		// Read in the data
		ReadData(imgPtr, infile);
	}
}

// Read the image header into the image pointer provided from the already opened file pointer
void ReadHeader(IMAGE* imgPtr, FILE* infile)
{
	// Allocate memory for the bitmap header
	imgPtr->bmHDR = malloc(sizeof(BITMAPHDR));

	if (imgPtr->bmHDR != NULL)
	{
		if (fread(imgPtr->bmHDR, sizeof(BITMAPHDR), 1, infile) != 1)
		{
			// No success reading, but malloc worked
			// Free the memory
			free(imgPtr->bmHDR);
			imgPtr->bmHDR = NULL;
			printf("Reading header from file did not work\n");
		}
	}
}


// Read the image data into the image pointer provided from the already opened file pointer
void ReadData(IMAGE* imgPtr, FILE* infile)
{
	// The padding, in bytes, for the image is:
	unsigned int padding = imgPtr->bmHDR->lWidth % 4;
	// Calculate the image size in bytes = size of row * number of rows
	unsigned int imageSize = (imgPtr->bmHDR->lWidth * sizeof(PIXEL) + padding)
		* imgPtr->bmHDR->lHeight;

	if (imageSize != imgPtr->bmHDR->dwImageSize)
	{
		printf("File corrupted?\n");
	}

	// Allocate memory for the pixel data
	imgPtr->bmData = malloc(imageSize);
	if (imgPtr->bmData != NULL)
	{
		// Read in the pixel array from the file
		if (fread(imgPtr->bmData, imageSize, 1, infile) != 1)
		{
			// Failed to read image data
			printf("Reading data from file did not work\n");
			FreeImage(imgPtr);
		}
	}
	else
	{
		free(imgPtr->bmHDR);
		imgPtr->bmHDR = NULL;
	}
}

// Write the image header and image data from the image pointer provided
// into the already opened file
void WriteImage(IMAGE* imgPtr, FILE* outfile)
{
	// Write the header
	if (fwrite(imgPtr->bmHDR, sizeof(BITMAPHDR), 1, outfile) != 1)
	{
		printf("Failed to write image header\n");
	}
	else
	{
		// Write the image data
		if (fwrite(imgPtr->bmData, imgPtr->bmHDR->dwImageSize, 1, outfile) != 1)
		{
			printf("Failed to write image data\n");
		}
	}
}

// Free the image header and image data from the passed-in image pointer
void FreeImage(IMAGE* imgPtr)
{
	// Free the image
	free(imgPtr->bmHDR);
	imgPtr->bmHDR = NULL;
	if (imgPtr->bmData != NULL)
	{
		free(imgPtr->bmData);
		imgPtr->bmData = NULL;
	}
}

// Hide data from file passed-in as file pointrer in an image passed-in as image pointer
void HideInImage(IMAGE* imgPtr, FILE* filePtr)
{
	PIXEL* currentPixel = imgPtr->bmData;
	// The padding, in bytes, for the image is:
	unsigned int padding = imgPtr->bmHDR->lWidth % 4;
	int myByte;
	for (int i = 0; i < imgPtr->bmHDR->lHeight; i++) // for each row
	{
		for (int j = 0; j < imgPtr->bmHDR->lWidth; j++) // for each pixel
		{
			myByte = fgetc(filePtr);
			if (myByte == EOF)
			{
				return;
			}
			hideByteInPixelFunc(currentPixel, (unsigned char)myByte); // manipulate that pixel
			currentPixel++; // move to the next pixel
		}
		// At the end of the row, move padding BYTEs forward
		currentPixel = (PIXEL*)(((BYTE*)currentPixel) + padding);
	}
}

// Helper method to manipulate a Pixel by hiding a byte in its lower bits
void hideByteInPixelFunc(PIXEL* currentPixel, BYTE myByte)
{
	BYTE bits74 = ((myByte >> 4) & 0b00001111);      // high 4 bits
	BYTE bits32 = (myByte >> 2) & 0b00000011;        // middle 2 bits
	BYTE bits10 = myByte & 0b00000011;               // low 2 bits

	currentPixel->bBlu = (currentPixel->bBlu & 0b11110000) | bits74;
	currentPixel->bGrn = (currentPixel->bGrn & 0b11111100) | bits32;
	currentPixel->bRed = (currentPixel->bRed & 0b11111100) | bits10;
}

// extract data from a image pointer, and write out ontp file pointer
void ExtractFileFromImage(IMAGE* imgPtr, FILE* filePtr)
{
	PIXEL* currentPixel = imgPtr->bmData;
	// The padding, in bytes, for the image is:
	unsigned int padding = imgPtr->bmHDR->lWidth % 4;
	unsigned int count = 0;
	for (int i = 0; i < imgPtr->bmHDR->lHeight; i++) // for each row
	{
		for (int j = 0; j < imgPtr->bmHDR->lWidth; j++) // for each pixel
		{
			BYTE myByte = readByteInPixelFunc(currentPixel);
			fputc(myByte, filePtr);
			if (++count >= imgPtr->bmHDR->dwClrImportant)
			{
				return;
			}
			currentPixel++; // move to the next pixel
		}
		// At the end of the row, move padding BYTEs forward
		currentPixel = (PIXEL*)(((BYTE*)currentPixel) + padding);
	}
}

BYTE readByteInPixelFunc(PIXEL* currentPixel)
{
	BYTE result = 0;
	result = ((currentPixel->bBlu & 0b00001111) << 4) | result;  // extracting bits 7-4
	result = ((currentPixel->bGrn & 0b00000011) << 2) | result;  // extracting bits 3-2
	result = ((currentPixel->bRed & 0b00000011) << 0) | result;  // extracting bits 1-0
	//printf("%c \t", (unsigned char)result);
	return result;
}

/*
* Purpose: determine the size of an given file
* Reference source for:
* ftell(): https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/ftell-ftelli64?view=msvc-170
* fseek(): https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fseek-fseeki64?view=msvc-170
*/

unsigned int GetFileSize(FILE* filePtr)
{
	long currentPos = ftell(filePtr);	  // Save current position, 0 in this case as we just open the file
	fseek(filePtr, 0, SEEK_END);          // Move 0 byte from EOF = to end of file
	long size = ftell(filePtr);           // Get position from the start = file size
	fseek(filePtr, currentPos, SEEK_SET); // more curPos byte from the Start = Restore original position, 0 in our case

	return (unsigned int)size;
}