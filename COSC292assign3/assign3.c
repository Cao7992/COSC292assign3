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

void HideInImage(IMAGE* imgPtr, FILE* filePtr)
{

}

void ExtractFileFromImage(IMAGE* imgPtr, FILE* filePtr)
{

}

unsigned int GetFileSize(FILE* filePtr)
{

}