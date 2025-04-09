#ifndef ASSIGN3_H
#define ASSIGN3_H

#include <stdio.h>

typedef unsigned short WORD;
typedef unsigned int DWORD; // could also use long on Windows systems
typedef signed int LONG; // could also use long on Windows systems
typedef unsigned char BYTE;

#pragma pack(push) // Used to store the default byte alignment
#pragma pack(1) // Set the byte alignment

typedef struct
{
	// Bitmap file header - 14 bytes
	WORD wType; // Should be 'B' 'M'
	DWORD dwFileSize;
	WORD wReserved1;
	WORD wReserved2;
	DWORD dwDataOffset; // Should always be 54 bytes for our application
	// DIB header (bitmap information header)
	DWORD dwHeaderSize; // Should be 40 for our application
	LONG lWidth;
	LONG lHeight;
	WORD wPlanes; // Should be 1
	WORD wBitCount; // Should be 24 for our application
	DWORD dwCompression; // Should be 0 for our application
	DWORD dwImageSize;
	LONG lXPelsPerMeter;
	LONG lYPelsPerMeter;
	DWORD dwClrUsed; // Should be 0 for our application
	DWORD dwClrImportant; // will be used to store the hidden file size
} BITMAPHDR;

typedef struct
{
	// each byte of color combines to create one pixel
	BYTE bBlu, bGrn, bRed;
} PIXEL;

// A struct that represents the entire image
typedef struct
{
	BITMAPHDR* bmHDR;
	PIXEL* bmData;
} IMAGE;

#pragma pack(pop) // Used to reset the default byte alignment

// Open a file given a prompt for the user (for the type of file) and a file mode
FILE* GetFile(const char* cPrompt, const char* cMode);
// Read an image into the image pointer provided from the already opened file pointer
void ReadImage(IMAGE* imgPtr, FILE* infile);

// Read the image header into the image pointer provided from the already opened file pointer
void ReadHeader(IMAGE* imgPtr, FILE* infile);

// Read the image data into the image pointer provided from the already opened file pointer
void ReadData(IMAGE* imgPtr, FILE* infile);

// Write the image header and image data from the image pointer provided
// into the already opened file
void WriteImage(IMAGE* imgPtr, FILE* outfile);

// Free the image header and image data from the passed-in image pointer
void FreeImage(IMAGE* imgPtr);

// Hide data from file passed-in as file pointrer in an image passed-in as image pointer
void HideInImage(IMAGE* imgPtr, FILE* filePtr);
// Helper method to manipulate a Pixel by hiding a byte in its lower bits
void hideByteInPixelFunc(PIXEL* currentPixel, BYTE myByte);

// extract data from a image pointer, and write out ontp file pointer
void ExtractFileFromImage(IMAGE* imgPtr, FILE* filePtr);

// Helper method to extract a BYTE data from  each Pixel passed-in
BYTE readByteInPixelFunc(PIXEL* currentPixel);

// Purpose: determine the size of an given file
unsigned int GetFileSize(FILE* filePtr);


#endif // !ASSIGN3_H