#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include "assign3.h"
#include <string.h>
#include <errno.h>

// for detecting memory leaks
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

void testHideInImage()
{
	FILE* infile = NULL;
	FILE* hidefile = NULL;
	FILE* outfile = NULL;

	infile = GetFile("Enter image to hide file in: ", "rb");
	hidefile = GetFile("Enter file to hide: ", "rb");

	// Create an image struct instance
	IMAGE img = { NULL, NULL };

	if (infile != NULL && hidefile != NULL)
	{
		// Read in the image
		ReadImage(&img, infile);
		// close the file
		fclose(infile);

		if (img.bmHDR != NULL)
		{
			// If the hidden file will fit within the image
			if (img.bmHDR->lWidth * img.bmHDR->lHeight >= GetFileSize(hidefile))
			{
				HideInImage(&img, hidefile);
				fclose(hidefile);

				// Write the image with hidden data out to file
				outfile = GetFile("Enter name of image with hidden file: ", "wb");
				WriteImage(&img, outfile);
				fclose(outfile);
			}
			else
			{
				printf("File is too big to hide in image\n");
				fclose(hidefile);
			}

			// Free any memory
			FreeImage(&img);
		}
		else {
			printf("Read in empty image\n");
		}
	}
	else {
		printf("Can not open one of the files\n");
	}

}


void testExtractFileFromImage()
{
	FILE* infile = GetFile("Enter image to extract from: ", "rb");
	FILE* outfile = GetFile("Enter filename to extract to: ", "wb");
	IMAGE img = { NULL, NULL };

	// Read in the image
	ReadImage(&img, infile);
	// Close the file
	fclose(infile);

	ExtractFileFromImage(&img, outfile);
	fclose(outfile);

	// Free any memory required

}


int main()
{
	//printf("Size of BITMAPHDR: %zu\n", sizeof(BITMAPHDR));
	//printf("Size of PIXEL: %zu\n", sizeof(PIXEL));

	testHideInImage();
	//testExtractFileFromImage();

	_CrtDumpMemoryLeaks();

	return EXIT_SUCCESS;
}